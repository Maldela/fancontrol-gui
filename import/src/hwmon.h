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


namespace Fancontrol
{

class Loader;
class Fan;
class Temp;
class PwmFan;

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

    explicit Hwmon(const QString &path, Loader *parent = Q_NULLPTR);

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
    bool testing() const;
    void reset() const;


signals:

    void fansChanged();
    void pwmFansChanged();
    void tempsChanged();
    void configUpdateNeeded();
    void sensorsUpdateNeeded();
    void error(QString, bool = false);


protected:

    QString m_name;
    int m_index;
    Loader *const m_parent;
    bool m_valid;
    QList<Fan *> m_fans;
    QList<PwmFan *> m_pwmFans;
    QList<Temp *> m_temps;

private:

    const QString m_path;
};

}

#endif // HWMON_H
