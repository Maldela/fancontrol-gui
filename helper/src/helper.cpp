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

#include "helper.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QProcess>
#include <QtCore/QFileInfo>

#include <KAuth/KAuthHelperSupport>
#include <KI18n/KLocalizedString>

#ifndef NO_SYSTEMD
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMetaType>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusVariant>


struct StringStruct
{
    QString type;
    QString filename;
    QString destination;
};

typedef QList<StringStruct> StringStructArray;

Q_DECLARE_METATYPE(StringStruct)
Q_DECLARE_METATYPE(StringStructArray)

QDBusArgument &operator<<(QDBusArgument &argument, const StringStruct &structure)
{
    argument.beginStructure();
    argument << structure.type << structure.filename << structure.destination;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, StringStruct &structure)
{
    argument.beginStructure();
    argument >> structure.type >> structure.filename >> structure.destination;
    argument.endStructure();
    return argument;
}
#endif


ActionReply Helper::action(const QVariantMap &arguments)
{
    ActionReply reply;

#ifndef NO_SYSTEMD
    if (arguments[QStringLiteral("action")] == "dbusaction")
    {
        qDBusRegisterMetaType<StringStruct>();
        qDBusRegisterMetaType<StringStructArray>();

        const auto method = arguments[QStringLiteral("method")].toString();
        const auto argsForCall = arguments[QStringLiteral("arguments")].toList();

        const auto systembus = QDBusConnection::systemBus();
        const auto iface = new QDBusInterface (QStringLiteral("org.freedesktop.systemd1"),
                                               QStringLiteral("/org/freedesktop/systemd1"),
                                               QStringLiteral("org.freedesktop.systemd1.Manager"),
                                               systembus,
                                               this);
        QDBusMessage dbusmessage;

        if (iface->isValid())
        {
            if (argsForCall.isEmpty())
                dbusmessage = iface->call(QDBus::AutoDetect, method);
            else
                dbusmessage = iface->callWithArgumentList(QDBus::AutoDetect, method, argsForCall);

            if (method != QStringLiteral("Reexecute"))
            {
                if (dbusmessage.type() == QDBusMessage::ErrorMessage)
                {
                    reply.setErrorCode(ActionReply::DBusError);
                    reply.setErrorDescription(dbusmessage.errorMessage());
                }
                else if (dbusmessage.type() == QDBusMessage::ReplyMessage)
                {
                    if (dbusmessage.signature() == QStringLiteral("a(sss)"))
                    {
                        QDBusReply<StringStructArray> dbusreply(dbusmessage);
                        if (dbusreply.isValid())
                        {
                            QMap<QString, QVariant> map;
                            map.insert(QStringLiteral("type"), dbusreply.value().value(0).type);
                            map.insert(QStringLiteral("filename"), dbusreply.value().value(0).filename);
                            map.insert(QStringLiteral("destination"), dbusreply.value().value(0).destination);
                            reply.addData(QStringLiteral("reply"), map);
                        }
                        else
                        {
                            reply = ActionReply::HelperErrorReply();
                            reply.setErrorDescription(dbusreply.error().message());
                        }
                    }
                    else if (dbusmessage.signature() == QStringLiteral("ba(sss)"))
                    {
                        QDBusReply<bool> dbusreply(dbusmessage); //QDBusReply only extracts the first return argument("b")
                        if (dbusreply.isValid())
                        {
                            QMap<QString, QVariant> map;
                            map.insert(QStringLiteral("enableInfo"), dbusreply.value());
                            auto changes = qdbus_cast<StringStructArray>(qvariant_cast<QDBusArgument>(dbusmessage.arguments().value(1))); //Extract the second argument("a(sss)")
                            map.insert(QStringLiteral("type"), changes.value(0).type);
                            map.insert(QStringLiteral("filename"), changes.value(0).filename);
                            map.insert(QStringLiteral("destination"), changes.value(0).destination);
                            reply.addData(QStringLiteral("reply"), map);
                        }
                        else
                        {
                            reply = ActionReply::HelperErrorReply();
                            reply.setErrorDescription(dbusreply.error().message());
                        }
                    }
                    else if (dbusmessage.signature() == QStringLiteral("o"))
                    {
                        QDBusReply<QDBusObjectPath> dbusreply(dbusmessage);
                        if (dbusreply.isValid())
                        {
                            QMap<QString, QVariant> map;
                            map.insert(QStringLiteral("job"), dbusreply.value().path());
                            reply.addData(QStringLiteral("reply"), map);
                        }
                        else
                        {
                            reply = ActionReply::HelperErrorReply();
                            reply.setErrorDescription(dbusreply.error().message());
                        }
                    }
                }
            }
        }
        else
        {
            reply = ActionReply::HelperErrorReply();
            reply.setErrorDescription(i18n("Could not create dbus interface"));
        }
        delete iface;
    }
    else
#endif
    if (arguments[QStringLiteral("action")] == "read")
    {
        const auto filename = arguments[QStringLiteral("filename")].toString();
        QFile file(filename);

        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            const auto content = stream.readAll();

            reply.addData(QStringLiteral("content"), content);
        }
        else
        {
            reply = ActionReply::HelperErrorReply();
            reply.setErrorDescription(file.errorString());
        }
    }

    else if (arguments[QStringLiteral("action")] == "write")
    {
        const auto filename = arguments[QStringLiteral("filename")].toString();
        QFile file(filename);

        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&file);
            stream << arguments[QStringLiteral("content")].toString();
        }
        else
        {
            reply = ActionReply::HelperErrorReply();
            reply.setErrorDescription(file.errorString());
        }
    }

    else if (arguments[QStringLiteral("action")] == "detectSensors")
    {
        const auto program = QStringLiteral("sensors-detect");
        const auto arguments = QStringList() << QStringLiteral("--auto");

        QProcess process;
        process.start(program, arguments);

        if (!process.waitForStarted(1000))
        {
            reply = ActionReply::HelperErrorReply();
            reply.setErrorDescription(process.errorString());
        }
        else if (!process.waitForFinished(10000))
        {
            reply = ActionReply::HelperErrorReply();
            reply.setErrorDescription(process.errorString());
        }
    }

    else
    {
        reply = ActionReply::HelperErrorReply();
        reply.setErrorDescription(i18n("This action does not exist!"));
    }

    return reply;
}

KAUTH_HELPER_MAIN("fancontrol.gui.helper", Helper)
