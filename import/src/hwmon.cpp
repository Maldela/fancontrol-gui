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

#include "loader.h"

#include <QtCore/QDir>
#include <QtCore/QTextStream>


namespace Fancontrol
{

Hwmon::Hwmon(const QString &path, Loader *parent) : QObject(parent),
    m_parent(parent),
    m_path(path),
    m_valid(true)
{
    QDir dir(path);
    if (!dir.isReadable())
    {
        emit errorChanged(path + " is not readable!");
        m_valid = false;
    }

    auto success = false;
    m_index = path.split('/').last().remove(QStringLiteral("hwmon")).toInt(&success);

    if (!success)
    {
        emit errorChanged(path + "is invalid!");
        m_valid = false;
    }

    const auto nameFile = new QFile(path + "/name");

    if (nameFile->open(QFile::ReadOnly))
        m_name = QTextStream(nameFile).readLine();
    else
        m_name = path.split('/').last();

    delete nameFile;

    connect(this, &Hwmon::configUpdateNeeded, parent, &Loader::createConfigFile);
    connect(this, &Hwmon::pwmFansChanged, parent, &Loader::emitAllPwmFansChanged);
    connect(this, &Hwmon::tempsChanged, parent, &Loader::emitAllTempsChanged);
    connect(this, &Hwmon::errorChanged, parent, &Loader::setError);

    if (m_valid)
        initialize();
}

void Hwmon::initialize()
{
    QDir dir(m_path);
    const auto entries = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    foreach (auto entry, entries)
    {
        if (!entry.contains(QStringLiteral("input")))
            continue;

        auto str = entry;
        auto success = false;
        const auto index = str.remove(QRegExp("\\D+")).toUInt(&success);

        if (!success)
        {
            emit errorChanged("Not a valid Sensor:" + entry);
            continue;
        }

        if (entry.contains(QStringLiteral("fan")))
        {
            if (QFile::exists(m_path + "/pwm" + QString::number(index)))
            {
                PwmFan *newPwmFan = Q_NULLPTR;

                foreach (const auto &pwmFan, m_pwmFans)
                {
                    if (pwmFan->index() == index)
                    {
                        newPwmFan = pwmFan;
                        break;
                    }
                }

                if (!newPwmFan)
                {
                    newPwmFan = new PwmFan(this, index);
                    connect(this, &Hwmon::sensorsUpdateNeeded, newPwmFan, &PwmFan::update);
                    connect(newPwmFan, &PwmFan::testStatusChanged, m_parent, &Loader::handleTestStatusChanged);
                    m_pwmFans << newPwmFan;
                    emit pwmFansChanged();
                }

                const auto newFan = qobject_cast<Fan *>(newPwmFan);
                if (!m_fans.contains(newFan))
                {
                    m_fans << newFan;
                    emit fansChanged();
                }
            }
            else
            {
                Fan *newFan = Q_NULLPTR;

                foreach (const auto &fan, m_fans)
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
                    connect(this, &Hwmon::sensorsUpdateNeeded, newFan, &Fan::update);
                    m_fans << newFan;
                    emit fansChanged();
                }
            }
        }

        if (entry.contains(QStringLiteral("temp")))
        {
            Temp *newTemp = Q_NULLPTR;

            foreach (const auto &temp, m_temps)
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
                connect(this, &Hwmon::sensorsUpdateNeeded, newTemp, &Temp::update);
                m_temps << newTemp;
                emit tempsChanged();
            }
        }
    }
}

QList<QObject *> Hwmon::fansAsObjects() const
{
    QList<QObject *> list;
    foreach (const auto &fan, m_fans)
    {
        list << qobject_cast<QObject *>(fan);
    }
    return list;
}

QList<QObject *> Hwmon::pwmFansAsObjects() const
{
    QList<QObject *> list;
    foreach (const auto &pwmFan, m_pwmFans)
    {
        list << qobject_cast<QObject *>(pwmFan);
    }
    return list;
}

QList<QObject *> Hwmon::tempsAsObjects() const
{
    QList<QObject *> list;
    foreach (const auto &temp, m_temps)
    {
        list << qobject_cast<QObject *>(temp);
    }
    return list;
}

void Hwmon::testFans()
{
    foreach (const auto &fan, m_pwmFans)
    {
        fan->test();
    }
}

void Hwmon::abortTestingFans()
{
    foreach (const auto &fan, m_pwmFans)
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

void Hwmon::setError(const QString &error, bool critical)
{
    emit errorChanged(error, critical);
}

bool Hwmon::testing() const
{
    auto testing = false;

    foreach(const auto &fan, m_pwmFans)
    {
        if (fan->testing())
        {
            testing = true;
            break;
        }
    }

    return testing;
}

}
