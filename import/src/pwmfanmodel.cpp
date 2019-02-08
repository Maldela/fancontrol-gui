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
#include "pwmfan.h"


namespace Fancontrol
{

PwmFanModel::PwmFanModel(QObject *parent) : QStringListModel(parent)
{
}

void PwmFanModel::setPwmFans(const QList<PwmFan *> &fans)
{
    if (m_fans == fans)
        return;

    m_fans = fans;
    emit fansChanged();

    QStringList list;

    for (const auto &fan : fans)
    {
        connect(fan, &PwmFan::nameChanged, this, static_cast<void(PwmFanModel::*)()>(&PwmFanModel::updateFan));
        list << fan->name() + "  (" + fan->path() + ")";
    }

    setStringList(list);
}

void PwmFanModel::addPwmFans(QList<PwmFan *> newFans)
{
    for (const auto &newFan : newFans)
    {
        for (const auto &fan : qAsConst(m_fans))
        {
            if (*fan == *newFan)
            {
                newFans.removeAll(newFan);
                break;
            }
        }
    }

    if (!newFans.isEmpty())
    {
        m_fans += newFans;
        emit fansChanged();

        const auto oldSize = rowCount();

        insertRows(oldSize, newFans.size());

        for (const auto &fan : qAsConst(newFans))
        {
            connect(fan, &PwmFan::nameChanged, this, static_cast<void(PwmFanModel::*)()>(&PwmFanModel::updateFan));
            updateFan(fan);
        }
    }
}

void PwmFanModel::updateFan(PwmFan *fan)
{
    if (!fan)
        return;

    const auto i = m_fans.indexOf(fan);
    if (i == -1)
        return;

    const auto string = fan->name() + "  (" + fan->path() + ")";
    setData(index(i, 0), string);
    emit dataChanged(index(i, 0), index(i, 0));
}

void PwmFanModel::updateFan()
{
    const auto fan = qobject_cast<PwmFan*>(sender());

    if (!fan)
        return;

    const auto i = m_fans.indexOf(fan);
    if (i == -1)
        return;

    const auto string = fan->name() + "  (" + fan->path() + ")";
    setData(index(i, 0), string);
    emit dataChanged(index(i, 0), index(i, 0));
}

QList<QObject *> PwmFanModel::fans() const
{
    auto list = QList<QObject *>();
    for (const auto &fan : m_fans)
        list << fan;
    return list;
}

}
