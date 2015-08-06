/*
 * Copyright (C) 2015  Malte Veerman <maldela@halloarsch.de>
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

#include "gui.h"
#include "../lib/src/sensors.h"
#include "../lib/src/hwmon.h"

#include <QtQml>

GUI::GUI(QObject *parent) : QObject(parent)
{
    m_loader->load(QUrl::fromLocalFile("/etc/fancontrol"));
    
    qmlRegisterType<Loader>();
    qmlRegisterType<Hwmon>();
    qmlRegisterType<Fan>();
    qmlRegisterType<PwmFan>();
    qmlRegisterType<Temp>();
    
#ifndef NO_SYSTEMD
    qmlRegisterType<SystemdCommunicator>();
#endif
}