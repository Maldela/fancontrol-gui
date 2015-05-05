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

#include <QFile>
#include <QTextStream>

#ifndef NO_SYSTEMD
#include <QtDBus>

ActionReply Helper::dbusaction(const QVariantMap &arguments)
{
    QString method = arguments["method"].toString();
    QVariantList argsForCall = arguments["arguments"].toList();

    ActionReply reply;

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

    return reply;
}
#endif

ActionReply Helper::read(const QVariantMap &args)
{
    ActionReply reply;
    QString filename = args["filename"].toString();
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
       reply = ActionReply::HelperErrorType;
       reply.setErrorCode(ActionReply::AuthorizationDeniedError);

       return reply;
    }

    QTextStream stream(&file);
    QString content = stream.readAll();

    QVariantMap retdata;
    retdata["content"] = content;

    reply.setData(retdata);
    return reply;
}

ActionReply Helper::write(const QVariantMap &args)
{
    ActionReply reply;
    QString filename = args["filename"].toString();
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)) {
       reply = ActionReply::HelperErrorType;
       reply.addData("errorDescription", file.errorString());

       return reply;
    }

    QTextStream stream(&file);
    stream << args["content"].toString();

    return reply;
}

KAUTH_HELPER_MAIN("fancontrol.gui.helper", Helper)
