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

#include "helper.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QProcess>

#include <KAuth/KAuthHelperSupport>
#include <KI18n/KLocalizedString>

#ifndef NO_SYSTEMD
#include <QtDBus/QDBusInterface>
#endif

ActionReply Helper::action(const QVariantMap &arguments)
{
    ActionReply reply;

#ifndef NO_SYSTEMD
    if (arguments["action"] == "dbusaction")
    {
        QString method = arguments["method"].toString();
        QVariantList argsForCall = arguments["arguments"].toList();

        QDBusConnection systembus = QDBusConnection::systemBus();

        QDBusInterface *iface = new QDBusInterface ("org.freedesktop.systemd1",
                                                    "/org/freedesktop/systemd1",
                                                    "org.freedesktop.systemd1.Manager",
                                                    systembus,
                                                    this);

        QDBusMessage dbusreply;

        if (iface->isValid())
            dbusreply = iface->callWithArgumentList(QDBus::AutoDetect, method, argsForCall);
        delete iface;

        if (method != "Reexecute")
        {
            if (dbusreply.type() == QDBusMessage::ErrorMessage)
            {
                reply.setErrorCode(ActionReply::DBusError);
                reply.setErrorDescription(dbusreply.errorMessage());
            }
        }
    }
    else
#endif
    if (arguments["action"] == "read")
    {
        QString filename = arguments["filename"].toString();
        QFile file(filename);

        if (!file.open(QIODevice::ReadOnly))
        {
           reply = ActionReply::HelperErrorType;
           reply.setErrorDescription(file.errorString());

           return reply;
        }

        QTextStream stream(&file);
        QString content = stream.readAll();

        QVariantMap returnData;
        returnData["content"] = content;

        reply.setData(returnData);
    }

    else if (arguments["action"] == "write")
    {
        QString filename = arguments["filename"].toString();
        QFile file(filename);

        if (!file.open(QIODevice::WriteOnly))
        {
           reply = ActionReply::HelperErrorType;
           reply.setErrorDescription(file.errorString());

           return reply;
        }

        QTextStream stream(&file);
        stream << arguments["content"].toString();
    }

    else if (arguments["action"] == "detectSensors")
    {
        QString program = "sensors-detect";
        QStringList arguments = QStringList() << "--auto";
        
        QProcess process;
        process.start(program, arguments);

        if (!process.waitForStarted(10000))
        {
            reply = ActionReply::HelperErrorType;
            reply.setErrorDescription(process.errorString());
            
            return reply;
        }
        
        if (!process.waitForFinished(10000))
        {
            reply = ActionReply::HelperErrorType;
            reply.setErrorDescription(process.errorString());
            
            return reply;
        }
    }
    
    else
    {
        reply.setType(ActionReply::HelperErrorType);
        reply.setErrorCode(ActionReply::NoSuchActionError);
        reply.setErrorDescription(i18n("This action does not exist!"));
    }
    
    return reply;
}

KAUTH_HELPER_MAIN("fancontrol.gui.helper", Helper)
