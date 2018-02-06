/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2015  Malte Veerman <malte.veerman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef TEMP_H
#define TEMP_H


#include "sensor.h"


class QTextStream;

namespace Fancontrol
{

class Temp : public Sensor
{
    Q_OBJECT
    Q_PROPERTY(QString label READ label NOTIFY labelChanged)
    Q_PROPERTY(int value READ value NOTIFY valueChanged)

public:

    explicit Temp(uint index, Hwmon *parent = Q_NULLPTR);
    virtual ~Temp();

    QString label() const { return m_label; }
    int value() const { return m_value; }
    QString name() const Q_DECL_OVERRIDE;
    void setName(const QString &name) Q_DECL_OVERRIDE;
    void toDefault() Q_DECL_OVERRIDE;
    bool isValid() const Q_DECL_OVERRIDE;


public slots:

    void update() Q_DECL_OVERRIDE;


signals:

    void labelChanged();
    void valueChanged();


protected:

    QTextStream *m_valueStream;


private:

    QString m_label;
    int m_value;
};

}

#endif // TEMP_H
