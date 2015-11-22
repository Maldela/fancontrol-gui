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


#include "hwmon.h"

#include <QtCore/QDir>
#include <QtCore/QTextStream>


namespace Fancontrol
{

Hwmon::Hwmon(const QString &path, QObject *parent) : QObject(parent),
    m_path(path),
    m_valid(true)
{
    QDir dir(path);
    if (!dir.isReadable())
    {
        emit errorChanged(path + " is not readable!");
        m_valid = false;
    }

    bool success;
    m_index = path.split('/').last().remove("hwmon").toInt(&success);
    if (!success)
    {
        emit errorChanged(path + "is invalid!");
        m_valid = false;
    }

    QFile nameFile(path + "/name");
    if (nameFile.open(QFile::ReadOnly))
        m_name = QTextStream(&nameFile).readLine();
    else
        m_name = path.split('/').last();

    connect(this, SIGNAL(configUpdateNeeded()), parent, SLOT(createConfigFile()));
    connect(this, SIGNAL(pwmFansChanged()), parent, SLOT(emitAllPwmFansChanged()));
    connect(this, SIGNAL(tempsChanged()), parent, SLOT(emitAllTempsChanged()));
    connect(this, SIGNAL(errorChanged(QString)), parent, SLOT(setError(QString)));

    if (m_valid)
        initialize();
}

void Hwmon::initialize()
{
    QDir dir(m_path);
    QStringList entrys = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    foreach (QString entry, entrys)
    {
        QString str = entry;
        uint index = str.remove(QRegExp("\\D+")).toUInt();
        if (entry.contains("fan") && entry.contains("input"))
        {
            if (QFile::exists(m_path + "/pwm" + QString::number(index)))
            {
                PwmFan *newPwmFan = Q_NULLPTR;

                foreach (PwmFan *pwmFan, m_pwmFans)
                {
                    if (pwmFan->index() == index)
                    {
                        newPwmFan = pwmFan;
                        newPwmFan->reset();
                        break;
                    }
                }

                if (!newPwmFan)
                {
                    newPwmFan = new PwmFan(this, index);
                    connect(this, SIGNAL(sensorsUpdateNeeded()), newPwmFan, SLOT(update()));
                    m_pwmFans << newPwmFan;
                    emit pwmFansChanged();
                }

                Fan *newFan = qobject_cast<Fan *>(newPwmFan);
                if (!m_fans.contains(newFan))
                {
                    m_fans << newFan;
                    emit fansChanged();
                }
            }
            else
            {
                Fan *newFan = Q_NULLPTR;

                foreach (Fan *fan, m_fans)
                {
                    if (fan->index() == index)
                    {
                        newFan = fan;
                        newFan->reset();
                        break;
                    }
                }

                if (!newFan)
                {
                    newFan = new Fan(this, index);
                    connect(this, SIGNAL(sensorsUpdateNeeded()), newFan, SLOT(update()));
                    m_fans << newFan;
                    emit fansChanged();
                }
            }
        }

        if (entry.contains("temp") && entry.contains("input"))
        {
            Temp *newTemp = Q_NULLPTR;

            foreach (Temp *temp, m_temps)
            {
                if (temp->index() == index)
                {
                    newTemp = temp;
                    newTemp->reset();
                    break;
                }
            }

            if (!newTemp)
            {
                newTemp = new Temp(this, index);
                connect(this, SIGNAL(sensorsUpdateNeeded()), newTemp, SLOT(update()));
                m_temps << newTemp;
                emit tempsChanged();
            }
        }
    }
}

QList<QObject *> Hwmon::fans() const
{
    QList<QObject *> list;
    foreach (Fan *fan, m_fans)
    {
        list << qobject_cast<QObject *>(fan);
    }
    return list;
}

QList<QObject *> Hwmon::pwmFans() const
{
    QList<QObject *> list;
    foreach (PwmFan *pwmFan, m_pwmFans)
    {
        list << qobject_cast<QObject *>(pwmFan);
    }
    return list;
}

QList<QObject *> Hwmon::temps() const
{
    QList<QObject *> list;
    foreach (Temp *temp, m_temps)
    {
        list << qobject_cast<QObject *>(temp);
    }
    return list;
}

void Hwmon::testFans()
{
    foreach (PwmFan *const fan, m_pwmFans)
    {
        fan->test();
    }
}

void Hwmon::abortTestingFans()
{
    foreach (PwmFan *const fan, m_pwmFans)
    {
        fan->abortTest();
    }
}

Fan* Hwmon::fan(int i) const
{
    return m_fans.value(i, Q_NULLPTR);
}

PwmFan* Hwmon::pwmFan(int i) const
{
    return m_pwmFans.value(i, Q_NULLPTR);
}

Temp* Hwmon::temp(int i) const
{
    return m_temps.value(i, Q_NULLPTR);
}

void Hwmon::setError(const QString &error)
{
    emit errorChanged(error);
}

}
