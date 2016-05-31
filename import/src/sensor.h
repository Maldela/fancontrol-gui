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


#ifndef SENSOR_H
#define SENSOR_H

#include <QtCore/QObject>
#include <QtCore/QString>


namespace Fancontrol
{

class Hwmon;

class Sensor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint index READ index CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString path READ path CONSTANT)
    Q_PROPERTY(Hwmon * parent READ parent CONSTANT)

public:

    explicit Sensor(Hwmon *parent, uint index, const QString &path = QString());

    virtual QString name() const = 0;
    virtual void setName(const QString &name) = 0;
    virtual void reset() = 0;
    QString path() const { return m_path; }
    Hwmon * parent() const { return m_parent; }
    uint index() const { return m_index; }

    bool operator==(const Sensor &other) { return m_path == other.path(); }
    bool operator!=(const Sensor &other) { return m_path != other.path(); }


public slots:

    virtual void update() = 0;


signals:

    void nameChanged();
    void error(QString, bool = false);


protected:

    Hwmon *const m_parent;
    const uint m_index;
    const QString m_path;
};

}

#endif // SENSOR_H
