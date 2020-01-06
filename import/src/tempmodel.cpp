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


#include "tempmodel.h"
#include "hwmon.h"
#include "temp.h"


namespace Fancontrol
{

TempModel::TempModel(QObject *parent) : QAbstractListModel(parent),
    m_unit(QStringLiteral("°C"))
{
}

QHash<int, QByteArray> TempModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;

    roleNames.insert(DisplayRole, "display");
    roleNames.insert(ObjectRole, "object");

    return roleNames;
}

QVariant TempModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const int row = index.row();

    if (row >= rowCount())
        return QVariant();

    const auto temp = m_temps.at(row);

    if (!temp)
        return QVariant();

    switch (role)
    {
        case DisplayRole:
            return temp->name() + ": " + QString::number(temp->value()) + m_unit + "   (" + temp->id() + ")";

        case ObjectRole:
            return QVariant::fromValue(temp);

        default:
            return QVariant();
    }
}

void TempModel::setTemps(QList<Temp *> temps)
{
    std::sort(temps.begin(), temps.end(), [] (Temp *a, Temp *b) { if (a->parent() == b->parent()) return a->index() < b->index(); return a->parent()->name() < b->parent()->name(); });

    if (m_temps == temps)
        return;

    beginResetModel();

    m_temps = temps;
    emit tempsChanged();

    for (const auto &temp : temps)
    {
        connect(temp, &Temp::nameChanged, this, static_cast<void(TempModel::*)()>(&TempModel::updateTemp));
        connect(temp, &Temp::valueChanged, this, static_cast<void(TempModel::*)()>(&TempModel::updateTemp));
    }

    endResetModel();
}

void TempModel::addTemp(Temp *newTemp)
{
    for (const auto &oldTemp : qAsConst(m_temps))
    {
        if (*oldTemp == *newTemp)
            return;
    }

    auto newTemps = m_temps << newTemp;
    std::sort(newTemps.begin(), newTemps.end(), [] (Temp *a, Temp *b) { if (a->parent() == b->parent()) return a->index() < b->index(); return a->parent()->name() < b->parent()->name(); });
    int index = newTemps.indexOf(newTemp);

    connect(newTemp, &Temp::nameChanged, this, static_cast<void(TempModel::*)()>(&TempModel::updateTemp));

    beginInsertRows(QModelIndex(), index, index);
    m_temps = newTemps;
    emit tempsChanged();
    endInsertRows();
}

void TempModel::addTemps(const QList<Temp *> &newTemps)
{
    for (const auto &newTemp : newTemps)
    {
        addTemp(newTemp);
    }
}

void TempModel::updateTemp(Temp *temp)
{
    if (!temp)
        return;

    const auto i = m_temps.indexOf(temp);
    if (i == -1)
        return;

    emit dataChanged(index(i, 0), index(i, 0), QVector<int>{ DisplayRole });
}

void TempModel::updateTemp()
{
    const auto temp = qobject_cast<Temp *>(sender());

    updateTemp(temp);
}

void TempModel::updateAll()
{
    emit dataChanged(index(0, 0), index(m_temps.size(), 0), QVector<int>{ DisplayRole });
}

QObject * TempModel::temp(int index) const
{
    return m_temps.value(index);
}

int TempModel::indexOf(QObject* temp) const
{
    return m_temps.indexOf(qobject_cast<Temp*>(temp));
}


}

