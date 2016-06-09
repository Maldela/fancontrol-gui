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
        connect(this, &SystemdCommunicator::error, parent, &GUIBase::handleError);

    if (serviceName.isEmpty())
        setServiceName(QStringLiteral(STANDARD_SERVICE_NAME));
    else
        setServiceName(serviceName);

    emit serviceNameChanged();
    emit serviceEnabledChanged();
    emit serviceActiveChanged();
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
    }
}

bool SystemdCommunicator::serviceExists()
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
    SystemdUnitFileList list = qdbus_cast<SystemdUnitFileList>(dbusreply.arguments().at(0));

    foreach (const auto &unitFile, list)
    {
        if (unitFile.path.contains(m_serviceName + ".service"))
            return true;
    }

    emit error(i18n("Service does not exist: %1", m_serviceName));
    return false;
}

bool SystemdCommunicator::serviceActive()
{
    if (serviceExists() && m_serviceInterface)
    {
        if (m_serviceInterface->property("ActiveState").toString() == QStringLiteral("active"))
            return true;
    }
    return false;
}

bool SystemdCommunicator::serviceEnabled()
{
    if (serviceExists() && m_serviceInterface)
    {
        if (m_serviceInterface->property("UnitFileState").toString() == QStringLiteral("enabled"))
            return true;

    }
    return false;
}

bool SystemdCommunicator::setServiceEnabled(bool enabled)
{
    if (serviceExists())
    {
        if (enabled != serviceEnabled())
        {
            const auto action = enabled ? QStringLiteral("EnableUnitFiles") : QStringLiteral("DisableUnitFiles");
            const auto files = QStringList() << m_serviceName + ".service";
            auto arguments = QVariantList() << files << false;
            if (enabled)
                arguments << true;

            if (dbusAction(action, arguments))
            {
                if (dbusAction(QStringLiteral("Reload")))
                {
                    emit serviceEnabledChanged();
                    return true;
                }
            }
            return false;
        }
        return true;
    }
    return false;
}

bool SystemdCommunicator::setServiceActive(bool active)
{
//    qDebug() << "Set service active:" << active;

    if (serviceExists())
    {
        if (active != serviceActive())
        {
            auto args = QVariantList() << m_serviceName + ".service" << "replace";
            const auto action = active ? QStringLiteral("ReloadOrRestartUnit") : QStringLiteral("StopUnit");

            if (dbusAction(action, args))
            {
                emit serviceActiveChanged();
                return true;
            }
        }

        return true;
    }
    return false;
}

bool SystemdCommunicator::dbusAction(const QString &method, const QVariantList &arguments)
{
    if (!m_managerInterface->isValid())
    {
        emit error(i18n("Invalid manager interface!"), true);
        return false;

    }

    const auto dbusreply = arguments.isEmpty() ? m_managerInterface->call(QDBus::AutoDetect, method) : m_managerInterface->callWithArgumentList(QDBus::AutoDetect, method, arguments);

    if (dbusreply.type() != QDBusMessage::ErrorMessage)
        return true;

    if (dbusreply.errorMessage() == QStringLiteral("Interactive authentication required."))
    {
        auto action = newFancontrolAction();
        QVariantMap map;
        map[QStringLiteral("action")] = "dbusaction";
        map[QStringLiteral("method")] = method;
        map[QStringLiteral("arguments")] = arguments;
        action.setArguments(map);

        const auto job = action.execute();
        connect(job, &KAuth::ExecuteJob::result, this, &SystemdCommunicator::handleDbusActionResult);
        job->start();

        return true;
    }

    emit error(dbusreply.errorMessage());
    return false;

}

void SystemdCommunicator::handleDbusActionResult(KJob *job)
{
    if (job->error())
    {
        if (job->error() == KAuth::ActionReply::HelperBusyError)
        {
//            qDebug() << "Helper busy...";

            const auto executeJob = static_cast<KAuth::ExecuteJob *>(job);
            if (executeJob)
            {
                const auto newJob = executeJob->action().execute();
                connect(newJob, &KAuth::ExecuteJob::result, this, &SystemdCommunicator::handleDbusActionResult);

                QTimer::singleShot(50, newJob, &KAuth::ExecuteJob::start);
                return;
            }
        }

        emit error(job->errorText());
    }
}

bool SystemdCommunicator::restartService()
{
    if (serviceExists())
    {
        QVariantList args;
        args << m_serviceName + ".service" << "replace";

        return dbusAction(QStringLiteral("ReloadOrRestartUnit"), args);
    }

    emit error(i18n("Service does not exist: %1", m_serviceName));
    return false;
}

void SystemdCommunicator::updateServiceProperties(QString, QVariantMap propchanged, QStringList)
{
    if (propchanged.value(QStringLiteral("ActiveState")).isValid())
        emit serviceActiveChanged();

    if (propchanged.value(QStringLiteral("UnitFileState")).isValid())
        emit serviceEnabledChanged();
}

}
