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


#ifndef FAN_H
#define FAN_H


#include "sensor.h"


class QTextStream;

namespace Fancontrol
{

class Fan : public Sensor
{
    Q_OBJECT
    Q_PROPERTY(int rpm READ rpm NOTIFY rpmChanged)

public:

    explicit Fan(uint index, Hwmon *parent = Q_NULLPTR);
    virtual ~Fan();

    int rpm() const { return m_rpm; }
    QString name() const Q_DECL_OVERRIDE;
    void setName(const QString &name) Q_DECL_OVERRIDE;
    void toDefault() Q_DECL_OVERRIDE;
    bool isValid() const Q_DECL_OVERRIDE;

    virtual int pwm() const { return 255; }
    virtual bool setPwm(int, bool) { return false; }


signals:

    void rpmChanged();


public slots:

    void update() Q_DECL_OVERRIDE;


protected:

    QTextStream *m_rpmStream;


private:

    int m_rpm;
};

}

#endif // FAN_H
