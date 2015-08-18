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

#include "config.h"
#include "guibase.h"
#include "sensors.h"
#include "hwmon.h"

#include <QtQml>
#include <QLocale>

GUIBase::GUIBase(QObject *parent) : QObject(parent),    
    m_config(Config::instance()),

#ifndef NO_SYSTEMD
    m_com(new SystemdCommunicator(m_config->findItem("ServiceName")->property().toString(), this)),
#endif

    m_loader(new Loader(this))
{
    connect(m_config, &Config::configChanged, this, &GUIBase::emitConfigChanged);
        
    QLocale locale = QLocale::system();
    QLocale::MeasurementSystem system = locale.measurementSystem();
    m_unit = (system == QLocale::MetricSystem) ? 0 : 2;

    qmlRegisterType<Loader>();
    qmlRegisterType<Hwmon>();
    qmlRegisterType<Fan>();
    qmlRegisterType<PwmFan>();
    qmlRegisterType<Temp>();
    
#ifndef NO_SYSTEMD
    qmlRegisterType<SystemdCommunicator>();
#endif
    
}

qreal GUIBase::maxTemp() const
{
    return m_config->findItem("MaxTemp")->property().toReal();
}

qreal GUIBase::minTemp() const
{
    return m_config->findItem("MinTemp")->property().toReal();
}

QString GUIBase::serviceName() const
{
    return m_config->findItem("ServiceName")->property().toString();
}

int GUIBase::interval() const
{
    return m_loader->interval();
}

void GUIBase::setMaxTemp(qreal temp)
{
    if (temp != maxTemp())
        m_config->findItem("MaxTemp")->setProperty(temp);
}

void GUIBase::setMinTemp(qreal temp)
{
    if (temp != minTemp())
        m_config->findItem("MinTemp")->setProperty(temp);
}

void GUIBase::setServiceName(const QString& name)
{
    if(name != serviceName())
    {
        m_config->findItem("ServiceName")->setProperty(name);

#ifndef NO_SYSTEMD
        m_com->setServiceName(name);
#endif
            
    }
}

void GUIBase::setInterval(int i)
{
    m_loader->setInterval(i);
}

void GUIBase::saveConfig()
{
    m_config->save();
}