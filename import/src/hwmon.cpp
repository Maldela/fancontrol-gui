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


#include "hwmon.h"

#include "loader.h"
#include "temp.h"
#include "fan.h"
#include "pwmfan.h"

#include <QtCore/QDir>
#include <QtCore/QTextStream>

#include <KI18n/KLocalizedString>


namespace Fancontrol
{

Hwmon::Hwmon(const QString &path, Loader *parent) : QObject(parent),
    m_parent(parent),
    m_valid(true),
    m_path(path)
{
    if (parent)
    {
        connect(this, &Hwmon::configUpdateNeeded, parent, &Loader::updateConfig);
        connect(this, &Hwmon::error, parent, &Loader::error);
    }

    if (!path.isEmpty())
    {
        QDir dir(path);
        if (!dir.isReadable())
        {
            emit error(i18n("%1 is not readable!", path));
            m_valid = false;
        }

        auto success = false;
        m_index = path.split('/').last().remove(QStringLiteral("hwmon")).toUInt(&success);

        if (!success)
        {
            emit error(i18n("%1 is invalid!", path));
            m_valid = false;
        }

        auto nameFile = new QFile(path + "/name");

        if (nameFile->open(QFile::ReadOnly))
            m_name = QTextStream(nameFile).readLine();
        else
        {
            delete nameFile;
            nameFile = new QFile(path + "/device/name");

            if (nameFile->open(QFile::ReadOnly))
                m_name = QTextStream(nameFile).readLine();
            else
                m_name = path.split('/').last();
        }

        delete nameFile;
    }

    if (m_valid && !m_path.isEmpty())
        initialize();
}

void Hwmon::initialize()
{
    QDir dir(m_path);
    const auto entries = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    for (const auto &entry : entries)
    {
        if (!entry.contains(QStringLiteral("input")))
            continue;

        auto str = entry;
        auto success = false;
        const auto index = str.remove(QRegExp("\\D+")).toUInt(&success);

        if (!success)
        {
            emit error(i18n("Not a valid sensor: \'%1\'", entry));
            continue;
        }

        if (entry.contains(QStringLiteral("fan")))
        {
            if (QFile::exists(m_path + "/pwm" + QString::number(index)))
            {
                if (!m_pwmFans.contains(index))
                {
                    auto newPwmFan = new PwmFan(index, this);
                    connect(this, &Hwmon::sensorsUpdateNeeded, newPwmFan, &PwmFan::update);

                    if (m_parent)
                        connect(newPwmFan, &PwmFan::testStatusChanged, m_parent, &Loader::handleTestStatusChanged);

                    m_pwmFans.insert(index, newPwmFan);
                    emit pwmFansChanged();

                    m_fans.insert(index, newPwmFan);
                    emit fansChanged();
                }
            }
            else
            {
                if (!m_fans.contains(index))
                {
                    auto newFan = new Fan(index, this);
                    connect(this, &Hwmon::sensorsUpdateNeeded, newFan, &Fan::update);

                    m_fans.insert(index, newFan);
                    emit fansChanged();
                }
            }
        }

        if (entry.contains(QStringLiteral("temp")))
        {
            if (!m_temps.contains(index))
            {
                auto newTemp = new Temp(index, this);
                connect(this, &Hwmon::sensorsUpdateNeeded, newTemp, &Temp::update);

                m_temps.insert(index, newTemp);
                emit tempsChanged();
            }
        }
    }

    if (isEmpty())
    {
        QDir deviceDir(m_path + "/device");
        const auto entries = deviceDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        for (const auto &entry : entries)
        {
            if (!entry.contains(QStringLiteral("input")))
                continue;

            auto str = entry;
            auto success = false;
            const auto index = str.remove(QRegExp("\\D+")).toUInt(&success);

            if (!success)
            {
                emit error(i18n("Not a valid sensor: \'%1\'", entry));
                continue;
            }

            if (entry.contains(QStringLiteral("fan")))
            {
                if (QFile::exists(m_path + "/pwm" + QString::number(index)))
                {
                    if (!m_pwmFans.contains(index))
                    {
                        auto newPwmFan = new PwmFan(index, this, true);
                        connect(this, &Hwmon::sensorsUpdateNeeded, newPwmFan, &PwmFan::update);

                        if (m_parent)
                            connect(newPwmFan, &PwmFan::testStatusChanged, m_parent, &Loader::handleTestStatusChanged);

                        m_pwmFans.insert(index, newPwmFan);
                        emit pwmFansChanged();

                        m_fans.insert(index, newPwmFan);
                        emit fansChanged();
                    }
                }
                else
                {
                    if (!m_fans.contains(index))
                    {
                        auto newFan = new Fan(index, this, true);
                        connect(this, &Hwmon::sensorsUpdateNeeded, newFan, &Fan::update);

                        m_fans.insert(index, newFan);
                        emit fansChanged();
                    }
                }
            }

            if (entry.contains(QStringLiteral("temp")))
            {
                if (!m_temps.contains(index))
                {
                    auto newTemp = new Temp(index, this, true);
                    connect(this, &Hwmon::sensorsUpdateNeeded, newTemp, &Temp::update);

                    m_temps.insert(index, newTemp);
                    emit tempsChanged();
                }
            }
        }
    }
}

QList<QObject *> Hwmon::fansAsObjects() const
{
    QList<QObject *> list;

    for (const auto &fan : m_fans.values())
        list << fan;

    return list;
}

QList<QObject *> Hwmon::pwmFansAsObjects() const
{
    QList<QObject *> list;

    for (const auto &pwmFan : m_pwmFans.values())
        list << pwmFan;

    return list;
}

QList<QObject *> Hwmon::tempsAsObjects() const
{
    QList<QObject *> list;

    for (const auto &temp : m_temps.values())
        list << temp;

    return list;
}

void Hwmon::testFans()
{
    for (const auto &pwmFan : m_pwmFans.values())
        pwmFan->test();
}

void Hwmon::abortTestingFans()
{
    for (const auto &pwmFan : m_pwmFans.values())
        pwmFan->abortTest();
}

bool Hwmon::testing() const
{
    auto testing = false;

    for (const auto &fan : m_pwmFans)
    {
        if (fan->testing())
        {
            testing = true;
            break;
        }
    }

    return testing;
}

void Hwmon::toDefault() const
{
    for (const auto &pwmFan : m_pwmFans)
        pwmFan->toDefault();
}

}
