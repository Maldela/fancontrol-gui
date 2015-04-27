#ifndef HWMON_H
#define HWMON_H

#include <QObject>

#include "sensors.h"

class Fan;
class PwmFan;
class Temp;

class Hwmon : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY pathChanged)
    Q_PROPERTY(QString path READ path NOTIFY pathChanged)
    Q_PROPERTY(int index READ index NOTIFY pathChanged)
    Q_PROPERTY(QList<QObject *> fans READ fans NOTIFY fansChanged)
    Q_PROPERTY(QList<QObject *> pwmFans READ pwmFans NOTIFY pwmFansChanged)
    Q_PROPERTY(QList<QObject *> temps READ temps NOTIFY tempsChanged)


public:

    explicit Hwmon(const QString &);

    QString name() const { return m_name; }
    QString path() const { return m_path; }
    int index() const { return m_index; }
    QList<QObject *> fans() const;
    QList<QObject *> pwmFans() const;
    QList<QObject *> temps() const;
    Fan * fan(int i) const { return m_fans.value(i, nullptr); }
    PwmFan * pwmFan(int i) const { return m_pwmFans.value(i, nullptr); }
    Temp * temp(int i) const { return m_temps.value(i, nullptr); }


public slots:

    void updateConfig() { emit configUpdateNeeded(); }
    void updateSensors() { emit sensorsUpdateNeeded(); }


signals:

    void pathChanged();
    void fansChanged();
    void pwmFansChanged();
    void tempsChanged();
    void configUpdateNeeded();
    void sensorsUpdateNeeded();


protected:

    QString m_name;
    QString m_path;
    int m_index;
    QList<Fan *> m_fans;
    QList<PwmFan *> m_pwmFans;
    QList<Temp *> m_temps;
};

#endif // HWMON_H
