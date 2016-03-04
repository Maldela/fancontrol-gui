/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2015  Malte Veerman maldela@halloarsch.de
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

#include <QDebug>


namespace Fancontrol
{

TempModel::TempModel(QObject *parent) : QStringListModel(parent),
m_unit(0)
{
}

QString TempModel::composeText(Temp *temp)
{
    QString suffix = m_unit == 0 ? QStringLiteral("°C") : m_unit == 2 ? QStringLiteral("°F") : QStringLiteral("K");

    return temp->name() + ": " + QString::number(temp->value()) + suffix + "   (" + temp->path() + ")";
}


void TempModel::setTemps(const QList<Temp *> &temps)
{
    m_temps = temps;
    emit tempsChanged();

    QStringList list;

    foreach (Temp *const temp, temps)
    {
        connect(temp, SIGNAL(nameChanged()), this, SLOT(updateTemp()));
        connect(temp, SIGNAL(valueChanged()), this, SLOT(updateTemp()));
        list << composeText(temp);
    }

    setStringList(list);
}

void TempModel::addTemps(const QList<Temp *> &temps)
{
    if (!temps.isEmpty())
    {
        m_temps += temps;
        emit tempsChanged();

        int oldSize = rowCount();

        insertRows(oldSize, temps.size());

        foreach (Temp *const temp, temps)
        {
            connect(temp, SIGNAL(nameChanged()), this, SLOT(updateTemp()));
            connect(temp, SIGNAL(valueChanged()), this, SLOT(updateTemp()));
            updateTemp(temp);
        }
    }
}

void TempModel::updateTemp(Temp *temp)
{
    if (!temp)
        return;

    int i = m_temps.indexOf(temp);
    if (i == -1)
        return;

    setData(index(i, 0), composeText(temp));
    emit dataChanged(index(i, 0), index(i, 0));
}

void TempModel::updateTemp()
{
    Temp *temp = qobject_cast<Temp *>(sender());

    updateTemp(temp);
}

void TempModel::updateAll()
{
    for (int i=0; i<m_temps.size(); i++)
    {
        Temp *temp = m_temps.at(i);
        setData(index(i, 0), composeText(temp));
    }
    emit dataChanged(index(0, 0), index(count(), 0));
}

QList<QObject *> TempModel::temps() const
{
    QList<QObject *> list;
    foreach(Temp *const temp, m_temps)
        list << qobject_cast<QObject *>(temp);
    return list;
}

}

