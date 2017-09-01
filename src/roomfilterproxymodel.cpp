/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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

#include "roomfilterproxymodel.h"
#include "roommodel.h"
#include <QDebug>

RoomFilterProxyModel::RoomFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    sort(0);
}

RoomFilterProxyModel::~RoomFilterProxyModel()
{
}

bool RoomFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    QString leftData = sourceModel()->data(left, RoomModel::RoomOrder).toString();
    QString rightData = sourceModel()->data(right, RoomModel::RoomOrder).toString();
    if (leftData == rightData) {
        const QString leftString = sourceModel()->data(left, RoomModel::RoomName).toString();
        const QString rightString = sourceModel()->data(right, RoomModel::RoomName).toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    } else {
        return leftData < rightData;
    }
}


QHash<int, QByteArray> RoomFilterProxyModel::roleNames() const
{
    if (QAbstractItemModel *source = sourceModel()) {
        return source->roleNames();
    }
    return QHash<int, QByteArray>();
}
