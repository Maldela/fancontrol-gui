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
        m_index = path.split('/').last().remove(QStringLiteral("hwmon")).toInt(&success);

        if (!success)
        {
            emit error(i18n("%1 is invalid!", path));
            m_valid = false;
        }

        const auto nameFile = new QFile(path + "/name");

        if (nameFile->open(QFile::ReadOnly))
            m_name = QTextStream(nameFile).readLine();
        else
            m_name = path.split('/').last();

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
                PwmFan *newPwmFan = Q_NULLPTR;

                for (const auto &pwmFan : qAsConst(m_pwmFans))
                {
                    if (pwmFan->index() == index)
                    {
                        newPwmFan = pwmFan;
                        break;
                    }
                }

                if (!newPwmFan)
                {
                    newPwmFan = new PwmFan(index, this);
                    connect(this, &Hwmon::sensorsUpdateNeeded, newPwmFan, &PwmFan::update);

                    if (m_parent)
                        connect(newPwmFan, &PwmFan::testStatusChanged, m_parent, &Loader::handleTestStatusChanged);

                    m_pwmFans << newPwmFan;
                    emit pwmFansChanged();
                }

                if (!m_fans.contains(newPwmFan))
                {
                    m_fans << newPwmFan;
                    emit fansChanged();
                }
            }
            else
            {
                Fan *newFan = Q_NULLPTR;

                for (const auto &fan : qAsConst(m_fans))
                {
                    if (fan->index() == index)
                    {
                        newFan = fan;
                        newFan->toDefault();
                        break;
                    }
                }

                if (!newFan)
                {
                    newFan = new Fan(index, this);
                    connect(this, &Hwmon::sensorsUpdateNeeded, newFan, &Fan::update);
                    m_fans << newFan;
                    emit fansChanged();
                }
            }
        }

        if (entry.contains(QStringLiteral("temp")))
        {
            Temp *newTemp = Q_NULLPTR;

            for (const auto &temp : qAsConst(m_temps))
            {
                if (temp->index() == index)
                {
                    newTemp = temp;
                    newTemp->toDefault();
                    break;
                }
            }

            if (!newTemp)
            {
                newTemp = new Temp(index, this);
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

    for (const auto &fan : qAsConst(m_fans))
        list << fan;

    return list;
}

QList<QObject *> Hwmon::pwmFansAsObjects() const
{
    QList<QObject *> list;

    for (const auto &pwmFan : qAsConst(m_pwmFans))
        list << pwmFan;

    return list;
}

QList<QObject *> Hwmon::tempsAsObjects() const
{
    QList<QObject *> list;

    for (const auto &temp : qAsConst(m_temps))
        list << temp;

    return list;
}

void Hwmon::testFans()
{
    for (const auto &pwmFan : qAsConst(m_pwmFans))
        pwmFan->test();
}

void Hwmon::abortTestingFans()
{
    for (const auto &pwmFan : qAsConst(m_pwmFans))
        pwmFan->abortTest();
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
