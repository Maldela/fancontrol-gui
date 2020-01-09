/*
 * Copyright (C) 2015  Malte Veerman <malte.veerman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */


#include "sensor.h"

#include "hwmon.h"


namespace Fancontrol
{

Sensor::Sensor(Hwmon *parent, uint index, const QString &type, bool device) : QObject(parent),
    m_parent(parent),
    m_index(index),
    m_device(device)
{
    if (!parent)
        return;

    m_id = parent->name() + "/" + type + QString::number(index);
    m_path = device ? QStringLiteral("hwmon") + QString::number(parent->index()) + "/device/" + type + QString::number(index) : QStringLiteral("hwmon") + QString::number(parent->index()) + "/" + type + QString::number(index);

    connect(this, &Sensor::error, parent, &Hwmon::error);
}

void Sensor::setId(const QString& id)
{
    if (m_id == id)
        return;

    m_id = id;
}


}
