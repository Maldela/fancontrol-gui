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

#include <QObject>

#include "sensors.h"
#include "loader.h"

class Fan;
class PwmFan;
class Temp;
class Loader;

class Hwmon : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString path READ path CONSTANT)
    Q_PROPERTY(int index READ index CONSTANT)
    Q_PROPERTY(QList<QObject *> fans READ fans NOTIFY fansChanged)
    Q_PROPERTY(QList<QObject *> pwmFans READ pwmFans NOTIFY pwmFansChanged)
    Q_PROPERTY(QList<QObject *> temps READ temps NOTIFY tempsChanged)


public:

    explicit Hwmon(const QString &, Loader *parent);

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

    void fansChanged();
    void pwmFansChanged();
    void tempsChanged();
    void configUpdateNeeded();
    void sensorsUpdateNeeded();


protected:

    Loader *m_parent;
    QString m_name;
    QString m_path;
    int m_index;
    QList<Fan *> m_fans;
    QList<PwmFan *> m_pwmFans;
    QList<Temp *> m_temps;
};

#endif // HWMON_H
