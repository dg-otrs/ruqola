/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "usersinroomlabel.h"
#include "roomwrapper.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "dialogs/directchannelinfodialog.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMenu>
#include <QPointer>

UsersInRoomLabel::UsersInRoomLabel(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mIconLabel = new QLabel(this);
    mIconLabel->setObjectName(QStringLiteral("mIconLabel"));
    mainLayout->addWidget(mIconLabel);

    mUserNameLabel = new UserLabel(this);
    mUserNameLabel->setObjectName(QStringLiteral("mUserNameLabel"));
    mainLayout->addWidget(mUserNameLabel);
}

UsersInRoomLabel::~UsersInRoomLabel()
{
}

void UsersInRoomLabel::setUserInfo(const UsersInRoomLabel::UserInfo &info)
{
    mUserNameLabel->setText(info.userDisplayName);
    mIconLabel->setPixmap(QIcon::fromTheme(info.iconStatus).pixmap(18, 18));
    mUserNameLabel->setUserId(info.userId);
    mUserNameLabel->setUserName(info.userName);
}

void UsersInRoomLabel::setRoomWrapper(RoomWrapper *roomWrapper)
{
    mUserNameLabel->setRoomWrapper(roomWrapper);
}

UserLabel::UserLabel(QWidget *parent)
    : QLabel(parent)
{
    setTextFormat(Qt::RichText);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &UserLabel::customContextMenuRequested, this, &UserLabel::slotCustomContextMenuRequested);
}

UserLabel::~UserLabel()
{
}

void UserLabel::setRoomWrapper(RoomWrapper *roomWrapper)
{
    mRoomWrapper = roomWrapper;
}

void UserLabel::setUserName(const QString &userName)
{
    mUserName = userName;
}

void UserLabel::setUserId(const QString &userId)
{
    mUserId = userId;
}

void UserLabel::slotOpenConversation()
{
    Q_EMIT Ruqola::self()->rocketChatAccount()->openLinkRequested(QStringLiteral("ruqola:/user/") + mUserName);
}

void UserLabel::slotBlockUser()
{
    const bool userIsBlocked = mRoomWrapper->blocker();
    Ruqola::self()->rocketChatAccount()->blockUser(mRoomWrapper->roomId(), !userIsBlocked);
}

void UserLabel::slotIgnoreUser()
{
    const bool userIsIgnored = mRoomWrapper->userIsIgnored(mUserId);
    Ruqola::self()->rocketChatAccount()->ignoreUser(mRoomWrapper->roomId(), mUserId, !userIsIgnored);
}

void UserLabel::slotRemoveFromRoom()
{
    Ruqola::self()->rocketChatAccount()->kickUser(mRoomWrapper->roomId(), mUserId, mRoomWrapper->channelType());
}

void UserLabel::slotCustomContextMenuRequested(const QPoint &pos)
{
    const bool canManageUsersInRoom = mRoomWrapper->canChangeRoles();
    const QString ownUserId = Ruqola::self()->rocketChatAccount()->userID();
    const bool isAdirectChannel = mRoomWrapper->channelType() == QStringLiteral("d");
    const bool isNotMe = mUserId != ownUserId;
    QMenu menu(this);

    if (isNotMe && !isAdirectChannel) {
        QAction *conversationAction = new QAction(i18n("Conversation"), &menu);
        connect(conversationAction, &QAction::triggered, this, &UserLabel::slotOpenConversation);
        menu.addAction(conversationAction);
    }
    if (isNotMe) {
        if (!menu.isEmpty()) {
            menu.addSeparator();
        }
        QAction *userInfoAction = new QAction(i18n("User Info"), &menu);
        userInfoAction->setIcon(QIcon::fromTheme(QStringLiteral("documentinfo")));
        connect(userInfoAction, &QAction::triggered, this, &UserLabel::slotUserInfo);
        menu.addAction(userInfoAction);
    }
    if (canManageUsersInRoom && !isAdirectChannel) {
        if (!menu.isEmpty()) {
            menu.addSeparator();
        }
        const bool hasOwnerRole = mRoomWrapper->userHasOwnerRole(mUserId);
        QAction *removeAsUser = new QAction(hasOwnerRole ? i18n("Remove as Owner") : i18n("Add as Owner"), &menu);
        connect(removeAsUser, &QAction::triggered, this, [this, hasOwnerRole]() {
            Ruqola::self()->rocketChatAccount()->changeRoles(mRoomWrapper->roomId(), mUserId, mRoomWrapper->channelType(), hasOwnerRole ? RocketChatAccount::RemoveOwner : RocketChatAccount::AddOwner);
        });

        menu.addAction(removeAsUser);

        const bool hasLeaderRole = mRoomWrapper->userHasLeaderRole(mUserId);
        QAction *removeAsLeader = new QAction(hasLeaderRole ? i18n("Remove as Leader") : i18n("Add as Leader"), &menu);
        connect(removeAsLeader, &QAction::triggered, this, [this, hasLeaderRole]() {
            Ruqola::self()->rocketChatAccount()->changeRoles(mRoomWrapper->roomId(), mUserId, mRoomWrapper->channelType(), hasLeaderRole ? RocketChatAccount::RemoveLeader : RocketChatAccount::AddLeader);
        });
        menu.addAction(removeAsLeader);

        const bool hasModeratorRole = mRoomWrapper->userHasModeratorRole(mUserId);
        QAction *removeAsModerator = new QAction(hasModeratorRole ? i18n("Remove as Moderator") : i18n("Add as Moderator"), &menu);
        connect(removeAsModerator, &QAction::triggered, this, [this, hasModeratorRole]() {
            Ruqola::self()->rocketChatAccount()->changeRoles(mRoomWrapper->roomId(), mUserId, mRoomWrapper->channelType(), hasModeratorRole ? RocketChatAccount::RemoveModerator : RocketChatAccount::AddModerator);
        });
        menu.addAction(removeAsModerator);

        QAction *removeFromRoom = new QAction(i18n("Remove from Room"), &menu);
        connect(removeFromRoom, &QAction::triggered, this, &UserLabel::slotRemoveFromRoom);
        menu.addAction(removeFromRoom);
    }
    if (isNotMe) {
        if (!menu.isEmpty()) {
            menu.addSeparator();
        }

        const bool userIsIgnored = mRoomWrapper->userIsIgnored(mUserId);
        QAction *ignoreAction = new QAction(userIsIgnored ? i18n("Unignore") : i18n("Ignore"), &menu);
        connect(ignoreAction, &QAction::triggered, this, &UserLabel::slotIgnoreUser);
        menu.addAction(ignoreAction);
        if (isAdirectChannel) {
            if (!menu.isEmpty()) {
                menu.addSeparator();
            }
            const bool userIsBlocked = mRoomWrapper->blocker();
            QAction *blockAction = new QAction(userIsBlocked ? i18n("Unblock User") : i18n("Block User"), &menu);
            connect(blockAction, &QAction::triggered, this, &UserLabel::slotBlockUser);
            menu.addAction(blockAction);
        }
    }
    menu.exec(mapToGlobal(pos));
}

void UserLabel::slotUserInfo()
{
    QPointer<DirectChannelInfoDialog> dlg = new DirectChannelInfoDialog(this);
    dlg->setUserName(mUserName);
    dlg->exec();
    delete dlg;
}
