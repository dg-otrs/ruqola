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

#include "showdiscussionsdialog.h"
#include "showdiscussionswidget.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>

#include <KLocalizedString>
#include <KConfigGroup>
#include <KSharedConfig>

namespace {
static const char myConfigGroupName[] = "ShowDiscussionsDialog";
}
ShowDiscussionsDialog::ShowDiscussionsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Show Discussions"));
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mShowDiscussionsWidget = new ShowDiscussionsWidget(this);
    mShowDiscussionsWidget->setObjectName(QStringLiteral("mShowDiscussionsWidget"));
    mainLayout->addWidget(mShowDiscussionsWidget);

    QDialogButtonBox *button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ShowDiscussionsDialog::reject);
    connect(mShowDiscussionsWidget, &ShowDiscussionsWidget::loadMoreDiscussion, this, &ShowDiscussionsDialog::slotLoadMoreDiscussions);
    readConfig();
}

ShowDiscussionsDialog::~ShowDiscussionsDialog()
{
    writeConfig();
}

void ShowDiscussionsDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ShowDiscussionsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}

void ShowDiscussionsDialog::slotLoadMoreDiscussions()
{
    if (mRoomId.isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "RoomId is empty. It's a bug";
        return;
    }
    Ruqola::self()->rocketChatAccount()->loadMoreDiscussions(mRoomId);
}

void ShowDiscussionsDialog::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

void ShowDiscussionsDialog::setModel(DiscussionsFilterProxyModel *model)
{
    mShowDiscussionsWidget->setModel(model);
}
