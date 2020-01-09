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


#ifndef SENSOR_H
#define SENSOR_H

#include <QtCore/QObject>


namespace Fancontrol
{

class Hwmon;

class Sensor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint index READ index CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(Hwmon * parent READ parent CONSTANT)

public:

    explicit Sensor(Hwmon *parent, uint index, const QString &type, bool device = false);

    virtual QString name() const = 0;
    virtual void setName(const QString &name) = 0;
    virtual void toDefault() = 0;
    virtual bool isValid() const = 0;
    virtual void update() = 0;
    QString id() const { return m_id; }
    QString path() const { return m_path; }
    Hwmon * parent() const { return m_parent; }
    uint index() const { return m_index; }
    bool device() const { return m_device; }

    bool operator==(const Sensor &other) { return m_path == other.path(); }
    bool operator!=(const Sensor &other) { return m_path != other.path(); }


signals:

    void nameChanged();
    void error(QString, bool = false);


protected:

    void setId(const QString &id);


private:

    Hwmon *const m_parent;
    const uint m_index;
    QString m_path;
    QString m_id;
    bool m_device;
};

}

#endif // SENSOR_H
