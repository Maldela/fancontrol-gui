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
#include "temp.h"


namespace Fancontrol
{

TempModel::TempModel(QObject *parent) : QStringListModel(parent),
    m_unit(QStringLiteral("°C"))
{
}

QString TempModel::composeText(Temp *temp)
{
    return temp->name() + ": " + QString::number(temp->value()) + m_unit + "   (" + temp->path() + ")";
}


void TempModel::setTemps(const QList<Temp *> &temps)
{
    m_temps = temps;
    emit tempsChanged();

    QStringList list;

    for (const auto &temp : temps)
    {
        connect(temp, &Temp::nameChanged, this, static_cast<void(TempModel::*)()>(&TempModel::updateTemp));
        connect(temp, &Temp::valueChanged, this, static_cast<void(TempModel::*)()>(&TempModel::updateTemp));
        list << composeText(temp);
    }

    setStringList(list);
}

void TempModel::addTemps(QList<Temp *> newTemps)
{

    for (const auto &newTemp : newTemps)
    {
        for (const auto &temp : qAsConst(m_temps))
        {
            if (*temp == *newTemp)
            {
                newTemps.removeAll(newTemp);
                break;
            }
        }
    }

    if (!newTemps.isEmpty())
    {
        m_temps += newTemps;
        emit tempsChanged();

        const auto oldSize = rowCount();

        insertRows(oldSize, newTemps.size());

        for (const auto &temp : qAsConst(newTemps))
        {
            connect(temp, &Temp::nameChanged, this, static_cast<void(TempModel::*)()>(&TempModel::updateTemp));
            connect(temp, &Temp::valueChanged, this, static_cast<void(TempModel::*)()>(&TempModel::updateTemp));
            updateTemp(temp);
        }
    }
}

void TempModel::updateTemp(Temp *temp)
{
    if (!temp)
        return;

    const auto i = m_temps.indexOf(temp);
    if (i == -1)
        return;

    setData(index(i, 0), composeText(temp));
    emit dataChanged(index(i, 0), index(i, 0));
}

void TempModel::updateTemp()
{
    const auto temp = qobject_cast<Temp *>(sender());

    updateTemp(temp);
}

void TempModel::updateAll()
{
    for (int i=0; i<m_temps.size(); i++)
        setData(index(i, 0), composeText(m_temps.at(i)));

    emit dataChanged(index(0, 0), index(m_temps.size(), 0));
}

QList<QObject *> TempModel::temps() const
{
    QList<QObject *> list;

    for (const auto &temp : m_temps)
        list << temp;

    return list;
}

}

