/*
 * Copyright 2016  Malte Veerman <malte.veerman@gmail.com>
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

#include "fancontrolqmlextension.h"

#include "fan.h"
#include "pwmfan.h"
#include "temp.h"
#include "hwmon.h"
#include "loader.h"
#include "guibase.h"

#ifndef NO_SYSTEMD
#include "systemdcommunicator.h"
#endif

#include <QtQml/qqml.h>


namespace Fancontrol
{

void FancontrolQmlExtension::registerTypes(const char* uri)
{
    Q_ASSERT(uri == QLatin1String("Fancontrol.Qml"));

    qmlRegisterType<Fan>();
    qmlRegisterUncreatableType<PwmFan>(uri, 1, 0, "PwmFan", QStringLiteral("PwmFan is not instantiable from QML!"));
    qmlRegisterType<Temp>();
    qmlRegisterType<Hwmon>();
    qmlRegisterType<Loader>();

#ifndef NO_SYSTEMD
    qmlRegisterType<SystemdCommunicator>();
#endif

    qmlRegisterSingletonType<GUIBase>(uri, 1, 0, "base", base);
}

QObject * FancontrolQmlExtension::base(QQmlEngine *engine, QJSEngine *jsengine)
{
    Q_UNUSED(engine)
    Q_UNUSED(jsengine)

    return new GUIBase;
}


}

