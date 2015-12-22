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


#ifndef HWMON_H
#define HWMON_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QList>

#include "temp.h"
#include "fan.h"
#include "pwmfan.h"


namespace Fancontrol
{

class Hwmon : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString path READ path CONSTANT)
    Q_PROPERTY(int index READ index CONSTANT)
    Q_PROPERTY(QList<QObject *> fans READ fansAsObjects NOTIFY fansChanged)
    Q_PROPERTY(QList<QObject *> pwmFans READ pwmFansAsObjects NOTIFY pwmFansChanged)
    Q_PROPERTY(QList<QObject *> temps READ tempsAsObjects NOTIFY tempsChanged)


public:

    explicit Hwmon(const QString &path, QObject *parent = Q_NULLPTR);

    void initialize();
    QString name() const { return m_name; }
    QString path() const { return m_path; }
    int index() const { return m_index; }
    QList<Fan *> fans() const { return m_fans; }
    QList<PwmFan *> pwmFans() const { return m_pwmFans; }
    QList<Temp *> temps() const { return m_temps; }
    QList<QObject *> fansAsObjects() const;
    QList<QObject *> pwmFansAsObjects() const;
    QList<QObject *> tempsAsObjects() const;
    Q_INVOKABLE void testFans();
    Q_INVOKABLE void abortTestingFans();
    Fan * fan(int i) const;
    PwmFan * pwmFan(int i) const;
    Temp * temp(int i) const;
    bool isValid() const { return m_valid; }


public slots:

    void updateConfig() { emit configUpdateNeeded(); }
    void updateSensors() { emit sensorsUpdateNeeded(); }


protected slots:

    void setError(const QString &error);


signals:

    void fansChanged();
    void pwmFansChanged();
    void tempsChanged();
    void configUpdateNeeded();
    void sensorsUpdateNeeded();
    void errorChanged(QString);


private:

    QString m_name;
    const QString m_path;
    bool m_valid;
    int m_index;
    QList<Fan *> m_fans;
    QList<PwmFan *> m_pwmFans;
    QList<Temp *> m_temps;
};

}

#endif // HWMON_H
