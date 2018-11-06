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


#include "config.h"


#ifndef CONFIG_NAME
#define CONFIG_NAME "fancontrol-gui"
#endif

#ifndef STANDARD_CONFIG_FILE
#define STANDARD_CONFIG_FILE "/etc/fancontrol"
#endif

#ifndef STANDARD_SERVICE_NAME
#define STANDARD_SERVICE_NAME "fancontrol"
#endif


namespace Fancontrol
{

Config *Config::m_instance = Q_NULLPTR;

Config::Config(QObject *parent) : KCoreConfigSkeleton(KSharedConfig::openConfig(QStringLiteral(CONFIG_NAME)), parent)
{
    setCurrentGroup(QStringLiteral("preferences"));
    addItemDouble(QStringLiteral("MinTemp"), m_minTemp, 30.0);
    addItemDouble(QStringLiteral("MaxTemp"), m_maxTemp, 90.0);
    addItemString(QStringLiteral("ServiceName"), m_serviceName, QStringLiteral(STANDARD_SERVICE_NAME));
    addItemPath(QStringLiteral("ConfigUrl"), m_configUrl, QStringLiteral("file://") + STANDARD_CONFIG_FILE);
    addItemStringList(QStringLiteral("Profiles"), m_profiles, QStringList());
    addItemStringList(QStringLiteral("ProfileNames"), m_profileNames, QStringList());

    load();
}

Config* Config::instance()
{
    if (!m_instance)
        m_instance = new Config;

    return m_instance;
}

}
