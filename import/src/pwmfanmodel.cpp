/*
 * Copyright 2015  Malte Veerman <malte.veerman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "pwmfanmodel.h"
#include "hwmon.h"
#include "pwmfan.h"


namespace Fancontrol
{

PwmFanModel::PwmFanModel(QObject *parent) : QAbstractListModel(parent)
{
}

QHash<int, QByteArray> PwmFanModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;

    roleNames.insert(DisplayRole, "display");
    roleNames.insert(ObjectRole, "object");

    return roleNames;
}

QVariant PwmFanModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const int row = index.row();

    if (row >= rowCount())
        return QVariant();

    const auto fan = m_fans.at(row);

    if (!fan)
        return QVariant();

    switch (role)
    {
        case DisplayRole:
            return fan->name() + "  (" + fan->id() + ")";

        case ObjectRole:
            return QVariant::fromValue(fan);

        default:
            return QVariant();
    }
}

void PwmFanModel::setPwmFans(QList<PwmFan *> fans)
{
    std::sort(fans.begin(), fans.end(), [] (PwmFan *a, PwmFan *b) { if (a->parent() == b->parent()) return a->index() < b->index(); return a->parent()->name() < b->parent()->name(); });

    if (m_fans == fans)
        return;

    beginResetModel();

    m_fans = fans;
    emit fansChanged();

    for (const auto &fan : fans)
        connect(fan, &PwmFan::nameChanged, this, static_cast<void(PwmFanModel::*)()>(&PwmFanModel::updateFan));

    endResetModel();
}

void PwmFanModel::addPwmFan(PwmFan* newFan)
{
    for (const auto &oldFan : qAsConst(m_fans))
    {
        if (*oldFan == *newFan)
            return;
    }

    auto newFans = m_fans << newFan;
    std::sort(newFans.begin(), newFans.end(), [] (PwmFan *a, PwmFan *b) { if (a->parent() == b->parent()) return a->index() < b->index(); return a->parent()->name() < b->parent()->name(); });
    int index = newFans.indexOf(newFan);

    connect(newFan, &PwmFan::nameChanged, this, static_cast<void(PwmFanModel::*)()>(&PwmFanModel::updateFan));

    beginInsertRows(QModelIndex(), index, index);
    m_fans = newFans;
    emit fansChanged();
    endInsertRows();
}

void PwmFanModel::addPwmFans(const QList<PwmFan *> &newFans)
{
    for (const auto &newFan : newFans)
    {
        addPwmFan(newFan);
    }
}

void PwmFanModel::updateFan(PwmFan *fan)
{
    if (!fan)
        return;

    const auto i = m_fans.indexOf(fan);
    if (i == -1)
        return;

    emit dataChanged(index(i, 0), index(i, 0), QVector<int>{ DisplayRole });
}

void PwmFanModel::updateFan()
{
    const auto fan = qobject_cast<PwmFan*>(sender());

    updateFan(fan);
}

QObject * PwmFanModel::fan(int index) const
{
    return m_fans.value(index);
}

}
