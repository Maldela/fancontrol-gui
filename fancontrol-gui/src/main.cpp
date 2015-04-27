#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>

#include "loader.h"

#ifndef NO_SYSTEMD
#include "systemdcommunicator.h"
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    Loader loader;
    engine.rootContext()->setContextProperty("loader", &loader);
#ifndef NO_SYSTEMD
    SystemdCommunicator com;
    engine.rootContext()->setContextProperty("systemdCom", &com);
#endif
    qmlRegisterType<Hwmon>();
    qmlRegisterType<Fan>();
    qmlRegisterType<PwmFan>();
    qmlRegisterType<Temp>();
    engine.load(QUrl(QStringLiteral("file:///usr/share/fancontrol-gui/qml/fancontrol-gui.qml")));

    return app.exec();
}
