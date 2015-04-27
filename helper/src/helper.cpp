#include "helper.h"

#include <QFile>
#include <QTextStream>

#ifndef NO_SYSTEMD
#include <QtDBus>

ActionReply Helper::dbusaction(const QVariantMap &arguments)
{
    QString method = arguments["method"].toString();
    QList<QVariant> argsForCall = arguments["arguments"].toList();

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
