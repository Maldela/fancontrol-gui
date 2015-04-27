#ifndef SYSTEMDCOMMUNICATOR_H
#define SYSTEMDCOMMUNICATOR_H

#include <QObject>
#include <QtDBus/QtDBus>

class SystemdCommunicator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString serviceName READ serviceName WRITE setServiceName NOTIFY serviceNameChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
    Q_PROPERTY(bool serviceExists READ serviceExists NOTIFY serviceNameChanged)

public:

    explicit SystemdCommunicator(QObject *parent = 0);

    QString serviceName() const { return m_serviceName; }
    void setServiceName(const QString &name);
    bool serviceExists();
    Q_INVOKABLE bool serviceActive();
    void setServiceActive(bool active);
    QString error() const { return m_error; }
    Q_INVOKABLE void dbusAction(const QString &method, const QList<QVariant> &arguments);


signals:

    void serviceNameChanged();
    void errorChanged();


protected:

    QString m_serviceName;
    QString m_servicePath;
    QString m_error;
    QDBusInterface *m_managerInterface;
    QDBusInterface *m_serviceInterface;
};

typedef struct
{
    QString path;
    QString state;
} SystemdUnitFile;

Q_DECLARE_METATYPE(SystemdUnitFile)
typedef QList<SystemdUnitFile> SystemdUnitFileList;
Q_DECLARE_METATYPE(SystemdUnitFileList)

QDBusArgument &operator<<(QDBusArgument &argument, const SystemdUnitFile &unitFile);
const QDBusArgument &operator>>(const QDBusArgument &argument, SystemdUnitFile &unitFile);

#endif // SYSTEMDCOMMUNICATOR_H
