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

#include <QtCore/QDebug>
#include <QtCore/QVariant>
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

SystemdCommunicator::SystemdCommunicator(QObject *parent, const QString &serviceName) : QObject(parent),
    m_managerInterface(new QDBusInterface(QStringLiteral("org.freedesktop.systemd1"),
                                          QStringLiteral("/org/freedesktop/systemd1"),
                                          QStringLiteral("org.freedesktop.systemd1.Manager"),
                                          QDBusConnection::systemBus(),
                                          this)),
    m_serviceInterface(Q_NULLPTR)
{
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
            QDBusMessage dbusreply = m_managerInterface->callWithArgumentList(QDBus::AutoDetect, QStringLiteral("LoadUnit"), arguments);
            if (dbusreply.type() == QDBusMessage::ErrorMessage)
            {
                m_error = dbusreply.errorMessage();
                emit errorChanged();
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
        setError(dbusreply.errorMessage());
        return false;
    }
    SystemdUnitFileList list = qdbus_cast<SystemdUnitFileList>(dbusreply.arguments().at(0));

    foreach (const SystemdUnitFile &unitFile, list)
    {
        if (unitFile.path.contains(m_serviceName + ".service"))
            return true;
    }

    setError(i18n("Service %1 doesn't exist", m_serviceName));
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
            QString action = enabled ? QStringLiteral("EnableUnitFiles") : QStringLiteral("DisableUnitFiles");
            QStringList files = QStringList() << m_serviceName + ".service";
            QVariantList arguments = QVariantList() << files << false;
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
    qDebug() << "Set service active:" << active;

    if (serviceExists())
    {
        if (active != serviceActive())
        {
            QVariantList args = QVariantList() << m_serviceName + ".service" << "replace";
            QString action = active ? QStringLiteral("ReloadOrRestartUnit") : QStringLiteral("StopUnit");

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
    QDBusMessage dbusreply;

    if (m_managerInterface->isValid())
    {
        if (arguments.isEmpty())
            dbusreply = m_managerInterface->call(QDBus::AutoDetect, method);
        else
            dbusreply = m_managerInterface->callWithArgumentList(QDBus::AutoDetect, method, arguments);
    }
    else
    {
        qDebug() << "Invalid manager interface!";
        return false;
    }

    if (dbusreply.type() == QDBusMessage::ErrorMessage)
    {
        if (dbusreply.errorMessage() == QStringLiteral("Interactive authentication required."))
        {
            KAuth::Action action = newFancontrolAction();
            QVariantMap map;
            map[QStringLiteral("action")] = "dbusaction";
            map[QStringLiteral("method")] = method;
            map[QStringLiteral("arguments")] = arguments;
            action.setArguments(map);

            KAuth::ExecuteJob *job = action.execute();
            connect(job, SIGNAL(result(KJob*)), this, SLOT(handleDbusActionResult(KJob*)));
            job->start();

            return true;
        }
        setError(dbusreply.errorMessage());
        return false;
    }

    return true;
}

void SystemdCommunicator::handleDbusActionResult(KJob *job)
{
    if (job->error())
    {
        if (job->error() == KAuth::ActionReply::HelperBusyError)
        {
            qDebug() << "Helper busy...";

            KAuth::ExecuteJob *executeJob = static_cast<KAuth::ExecuteJob *>(job);
            if (executeJob)
            {
                KAuth::ExecuteJob *newJob = executeJob->action().execute();
                connect(newJob, SIGNAL(result(KJob*)), this, SLOT(handleDbusActionResult(KJob*)));

                QTimer::singleShot(50, this, [newJob] (){ newJob->start(); });
                return;
            }
        }

        setError(job->errorText());
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

    setError(i18n("Service does not exist"));
    return false;
}

void SystemdCommunicator::updateServiceProperties(QString, QVariantMap propchanged, QStringList)
{
    if (propchanged.value(QStringLiteral("ActiveState")).isValid())
        emit serviceActiveChanged();

    if (propchanged.value(QStringLiteral("UnitFileState")).isValid())
        emit serviceEnabledChanged();
}

void SystemdCommunicator::setError(const QString &error)
{
    qCritical() << error;

    m_error = error;
    emit errorChanged();
}

}
