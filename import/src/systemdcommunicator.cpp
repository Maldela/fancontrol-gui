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


#include "systemdcommunicator.h"

#include "fancontrolaction.h"
#include "guibase.h"

#include <QtCore/QTimer>
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>

#include <KAuth/KAuthExecuteJob>
#include <KI18n/KLocalizedString>


#ifndef STANDARD_SERVICE_NAME
#define STANDARD_SERVICE_NAME "fancontrol"
#endif


typedef struct
{
    QString path;
    QString state;
} SystemdUnitFile;
Q_DECLARE_METATYPE(SystemdUnitFile)

typedef QList<SystemdUnitFile> SystemdUnitFileList;
Q_DECLARE_METATYPE(SystemdUnitFileList)

QDBusArgument& operator <<(QDBusArgument &argument, const SystemdUnitFile &unitFile)
{
    argument.beginStructure();
    argument << unitFile.path << unitFile.state;
    argument.endStructure();
    return argument;
}

const QDBusArgument& operator >>(const QDBusArgument &argument, SystemdUnitFile &unitFile)
{
    argument.beginStructure();
    argument >> unitFile.path >> unitFile.state;
    argument.endStructure();
    return argument;
}


namespace Fancontrol
{

SystemdCommunicator::SystemdCommunicator(GUIBase *parent, const QString &serviceName) : QObject(parent),
    m_managerInterface(new QDBusInterface(QStringLiteral("org.freedesktop.systemd1"),
                                          QStringLiteral("/org/freedesktop/systemd1"),
                                          QStringLiteral("org.freedesktop.systemd1.Manager"),
                                          QDBusConnection::systemBus(),
                                          this)),
    m_serviceInterface(Q_NULLPTR)
{
    if (parent)
    {
        connect(this, &SystemdCommunicator::error, parent, &GUIBase::handleError);
        connect(this, &SystemdCommunicator::info, parent, &GUIBase::handleInfo);
    }

    if (serviceName.isEmpty())
        setServiceName(QStringLiteral(STANDARD_SERVICE_NAME));
    else
        setServiceName(serviceName);

    emit serviceNameChanged();

    m_serviceActive = systemdServiceActive();
    emit serviceActiveChanged();

    m_serviceEnabled = systemdServiceEnabled();
    emit serviceEnabledChanged();
}

void SystemdCommunicator::setServiceName(const QString &name)
{
    if (name != m_serviceName)
    {
        if (m_serviceInterface)
        {
            QDBusConnection::systemBus().disconnect(QStringLiteral("org.freedesktop.systemd1"),
                                                    m_serviceObjectPath,
                                                    QStringLiteral("org.freedesktop.DBus.Properties"),
                                                    QStringLiteral("PropertiesChanged"),
                                                    this,
                                                    SLOT(updateServiceProperties(QString, QVariantMap, QStringList)));
            m_serviceInterface->deleteLater();
            m_serviceInterface = Q_NULLPTR;
        }

        m_serviceName = name;

        if (serviceExists())
        {
            QVariantList arguments;
            arguments << QVariant(m_serviceName + ".service");
            const auto dbusreply = m_managerInterface->callWithArgumentList(QDBus::AutoDetect, QStringLiteral("LoadUnit"), arguments);
            if (dbusreply.type() == QDBusMessage::ErrorMessage)
            {
                emit error(dbusreply.errorMessage());
                m_serviceObjectPath.clear();
            }
            else
            {
                m_serviceObjectPath = qdbus_cast<QDBusObjectPath>(dbusreply.arguments().at(0)).path();

                m_serviceInterface = new QDBusInterface(QStringLiteral("org.freedesktop.systemd1"),
                                                        m_serviceObjectPath,
                                                        QStringLiteral("org.freedesktop.systemd1.Unit"),
                                                        QDBusConnection::systemBus(),
                                                        this);
                QDBusConnection::systemBus().connect(QStringLiteral("org.freedesktop.systemd1"),
                                                     m_serviceObjectPath,
                                                     QStringLiteral("org.freedesktop.DBus.Properties"),
                                                     QStringLiteral("PropertiesChanged"),
                                                     this,
                                                     SLOT(updateServiceProperties(QString, QVariantMap, QStringList)));
            }
        }

        emit serviceNameChanged();
        emit serviceEnabledChanged();
        emit serviceActiveChanged();

        emit info(i18n("New srevice name: \'%1\'", m_serviceName));
    }
}

bool SystemdCommunicator::serviceExists() const
{
    if (m_serviceInterface)
    {
        if (m_serviceInterface->isValid())
            return true;
    }

    QDBusMessage dbusreply;

    if (m_managerInterface && m_managerInterface->isValid())
        dbusreply = m_managerInterface->call(QDBus::AutoDetect, QStringLiteral("ListUnitFiles"));

    if (dbusreply.type() == QDBusMessage::ErrorMessage)
    {
        emit error(dbusreply.errorMessage());
        return false;
    }
    const auto list = qdbus_cast<SystemdUnitFileList>(dbusreply.arguments().at(0));

    for (const auto &unitFile : list)
    {
        if (unitFile.path.contains(m_serviceName + ".service"))
            return true;
    }

    emit error(i18n("Service does not exist: \'%1\'", m_serviceName));
    return false;
}

bool SystemdCommunicator::systemdServiceActive() const
{
    if (serviceExists() && m_serviceInterface)
    {
        if (m_serviceInterface->property("ActiveState").toString() == QStringLiteral("active"))
            return true;
    }
    return false;
}

bool SystemdCommunicator::systemdServiceEnabled() const
{
    if (serviceExists())
    {
        QDBusReply<QString> reply = m_managerInterface->call(QDBus::AutoDetect, QStringLiteral("GetUnitFileState"), m_serviceName + ".service");
        if (reply.isValid())
            return reply.value() == QStringLiteral("enabled");
        else
            emit error(reply.error().message());
    }
    return false;
}

void SystemdCommunicator::setServiceEnabled(bool enabled)
{
    if (m_serviceEnabled == enabled)
        return;

    m_serviceEnabled = enabled;
    emit serviceEnabledChanged();
    emit needsApplyChanged();
}

void SystemdCommunicator::setServiceActive(bool active)
{
    if (m_serviceActive == active)
        return;

    m_serviceActive = active;
    emit serviceActiveChanged();
    emit needsApplyChanged();
}

bool SystemdCommunicator::restartService()
{
    if (serviceExists())
    {
        emit info(i18n("Restarting service: \'%1\'", m_serviceName));

        auto args = QVariantList() << m_serviceName + ".service" << "replace";
        return dbusAction(QStringLiteral("ReloadOrRestartUnit"), args);
    }

    emit error(i18n("Service does not exist: \'%1\'", m_serviceName));
    return false;
}

bool SystemdCommunicator::needsApply() const
{
    return m_serviceActive != systemdServiceActive() || m_serviceEnabled != systemdServiceEnabled();
}

bool SystemdCommunicator::dbusAction(const QString &method, const QVariantList &arguments)
{
    if (!m_managerInterface->isValid())
    {
        emit error(i18n("Invalid manager interface!"), true);
        return false;

    }

    auto action = newFancontrolAction();
    QVariantMap map;
    map[QStringLiteral("action")] = "dbusaction";
    map[QStringLiteral("method")] = method;
    map[QStringLiteral("arguments")] = arguments;
    action.setArguments(map);

    const auto job = action.execute();
    bool success = job->exec();
    if (success)
    {
        if (method == QStringLiteral("EnableUnitFiles") || method == QStringLiteral("DisableUnitFiles"))
        {
            emit serviceEnabledChanged();
            emit needsApplyChanged();
        }
        else if (method == QStringLiteral("StartUnit") || method == QStringLiteral("StopUnit"))
        {
            emit serviceActiveChanged();
            emit needsApplyChanged();
        }
    }
    else
    {
        emit error(i18n("Dbus error: %1", job->errorString()));
    }
    return success;
}

void SystemdCommunicator::apply(bool serviceRestart)
{
    if (serviceExists())
    {
        if (m_serviceEnabled != systemdServiceEnabled())
        {
            QString method;
            if (m_serviceEnabled)
            {
                emit info(i18n("Enabling service autostart at boot:\'%1\'", m_serviceName));
                method = QStringLiteral("EnableUnitFiles");
            }
            else
            {
                emit info(i18n("Disabling service autostart at boot: \'%1\'", m_serviceName));
                method = QStringLiteral("DisableUnitFiles");
            }
            const auto files = QStringList() << m_serviceName + ".service";
            auto args = QVariantList() << files << false;
            if (m_serviceEnabled)
                args << true;

            if (!dbusAction(method, args))
                return;
        }

        if (m_serviceActive != systemdServiceActive())
        {
            QString method;
            if (m_serviceEnabled)
            {
                emit info(i18n("Starting service: \'%1\'", m_serviceName));
                method = QStringLiteral("StartUnit");
            }
            else
            {
                emit info(i18n("Stopping service: \'%1\'", m_serviceName));
                method = QStringLiteral("StopUnit");
            }
            auto args = QVariantList() << m_serviceName + ".service" << "replace";

            if (!dbusAction(method, args))
                return;
        }
        else if (systemdServiceActive() && m_serviceActive && serviceRestart)
            restartService();
    }
}

void SystemdCommunicator::reset()
{
    setServiceActive(systemdServiceActive());
    setServiceEnabled(systemdServiceEnabled());
}

void SystemdCommunicator::updateServiceProperties(QString, QVariantMap propchanged, QStringList)
{
    if (propchanged.value(QStringLiteral("ActiveState")).isValid())
    {
        emit needsApplyChanged();
    }

    if (propchanged.value(QStringLiteral("UnitFileState")).isValid())
    {
        emit needsApplyChanged();
    }
}

}
