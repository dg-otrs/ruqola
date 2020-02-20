/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#ifndef EMOTICONCATEGORIZEDMODEL_H
#define EMOTICONCATEGORIZEDMODEL_H

#include <QAbstractListModel>
#include <QFont>
#include "libruqolacore_export.h"
#include "emoticons/unicodeemoticon.h"

class LIBRUQOLACORE_EXPORT EmoticonCategorizedModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum EmoticonsRoles {
        Identifier = Qt::UserRole + 1,
        Text,
        UnicodeEmoji
    };
    Q_ENUM(EmoticonsRoles)

    explicit EmoticonCategorizedModel(QObject *parent = nullptr);
    ~EmoticonCategorizedModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    void setEmoticons(const QMap<QString, QVector<UnicodeEmoticon> > &emoticons);

private:
    Q_DISABLE_COPY(EmoticonCategorizedModel)
    QVector<UnicodeEmoticon> mEmoticons;
    QFont mEmojiFont;
};

#endif // EMOTICONCATEGORIZEDMODEL_H