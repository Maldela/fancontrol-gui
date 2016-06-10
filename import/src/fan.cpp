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


#include "fan.h"

#include "hwmon.h"

#include <QtCore/QTextStream>
#include <QtCore/QDir>
#include <QtCore/QFile>

#include <KConfigCore/KSharedConfig>
#include <KConfigCore/KConfigGroup>
#include <KI18n/KLocalizedString>


#define TEST_HWMON_NAME "test"


namespace Fancontrol
{

Fan::Fan(uint index, Hwmon *parent) : Sensor(parent, index, parent ? parent->name() + "/fan" + QString::number(index) : QString()),
    m_rpmStream(new QTextStream),
    m_rpm(0)
{
    if (m_parent && QDir(parent->path()).isReadable())
    {
        const auto rpmFile = new QFile(parent->path() + "/fan" + QString::number(index) + "_input", this);

        if (rpmFile->open(QFile::ReadOnly))
        {
            m_rpmStream->setDevice(rpmFile);
            *m_rpmStream >> m_rpm;
        }
        else
        {
            emit error(i18n("Can't open rpm file: \"%1\"", rpmFile->fileName()));
            delete rpmFile;
        }
    }
}

Fan::~Fan()
{
    auto device = m_rpmStream->device();
    delete m_rpmStream;
    delete device;
}

QString Fan::name() const
{
    const auto names = KSharedConfig::openConfig(QStringLiteral("fancontrol-gui"))->group("names");
    const auto localNames = names.group(m_parent ? m_parent->name() : QStringLiteral(TEST_HWMON_NAME));
    const auto name = localNames.readEntry("fan" + QString::number(m_index), QString());

    if (name.isEmpty())
        return "fan" + QString::number(m_index);

    return name;
}

void Fan::setName(const QString &name)
{
    const auto names = KSharedConfig::openConfig(QStringLiteral("fancontrol-gui"))->group("names");
    auto localNames = names.group(m_parent ? m_parent->name() : QStringLiteral(TEST_HWMON_NAME));

    if (name != localNames.readEntry("fan" + QString::number(m_index), QString())
        && !name.isEmpty())
    {
        localNames.writeEntry("fan" + QString::number(m_index), name);
        emit nameChanged();
    }
}

void Fan::reset()
{
    if (m_rpmStream->device() && m_parent)
    {
        auto device = m_rpmStream->device();
        m_rpmStream->setDevice(Q_NULLPTR);
        delete device;

        if (QDir(m_parent->path()).isReadable())
        {
            const auto rpmFile = new QFile(m_parent->path() + "/fan" + QString::number(m_index) + "_input", this);

            if (rpmFile->open(QFile::ReadOnly))
            {
                m_rpmStream->setDevice(rpmFile);
                *m_rpmStream >> m_rpm;
            }
            else
            {
                emit error(i18n("Can't open rpm file: \"%1\"", rpmFile->fileName()));
                delete rpmFile;
            }
        }
    }
}

void Fan::update()
{
    m_rpmStream->seek(0);
    int rpm;
    *m_rpmStream >> rpm;

    if (rpm != m_rpm)
    {
        m_rpm = rpm;
        emit rpmChanged();
    }
}

bool Fan::isValid() const
{
    return m_rpmStream->device() || m_rpmStream->string();
}

}
