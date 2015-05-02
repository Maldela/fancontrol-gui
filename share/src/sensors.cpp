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

#include "sensors.h"
#include <QFile>
#include <QDir>
#include <KConfigGroup>

Sensor::Sensor(Hwmon *parent, uint index) : QObject(parent)
{
    m_parent = parent;
    m_index = index;
    m_config = KSharedConfig::openConfig();
}


Fan::Fan(Hwmon *parent, uint index) : Sensor(parent, index)
{
    if (QDir(parent->path()).isReadable())
    {
        QFile *rpmFile = new QFile(parent->path() + "/fan" + QString::number(index) + "_input", this);

        if (rpmFile->open(QFile::ReadOnly))
        {
            m_rpmStream.setDevice(rpmFile);
            m_rpmStream >> m_rpm;

            setName("fan"+ QString::number(index));
        }
        else
        {
            qDebug() << "Can't open rpmFile " << parent->path() + "/fan" + QString::number(index) + "_input";
        }
    }
}

QString Fan::name() const
{
    KConfigGroup names = m_config->group("names");
    KConfigGroup localNames = names.group(m_parent->name());
    QString name = localNames.readEntry("fan" + QString::number(m_index), QString());
    if (name.isEmpty())
        return "fan" + QString::number(m_index);
    return name;
}

void Fan::setName(const QString &name)
{
    KConfigGroup names = m_config->group("names");
    KConfigGroup localNames = names.group(m_parent->name());
    if (name != localNames.readEntry("fan" + QString::number(m_index), QString()))
    {
        localNames.writeEntry("fan" + QString::number(m_index), name);
        emit nameChanged();
    }
}

void Fan::update()
{
    m_rpmStream.seek(0);
    m_rpmStream >> m_rpm;
    emit rpmChanged();
}


PwmFan::PwmFan(Hwmon *parent, uint index) : Fan(parent, index)
{
    m_temp = nullptr;
    m_hasTemp = false;
    m_minTemp = 0;
    m_maxTemp = 100;
    m_minPwm = 255;
    m_maxPwm = 255;
    m_minStart = 255;
    m_minStop = 255;

    connect(this, SIGNAL(tempChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(hasTempChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(minTempChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(maxTempChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(minPwmChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(maxPwmChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(minStartChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(minStopChanged()), parent, SLOT(updateConfig()));

    if (QDir(parent->path()).isReadable())
    {
        QFile *pwmFile = new QFile(parent->path() + "/pwm" + QString::number(index), this);

        if (pwmFile->open(QFile::ReadOnly)) //TODO ReadWrite with su rights
        {
            m_pwmStream.setDevice(pwmFile);
            m_pwmStream >> m_pwm;
        }
        else
        {
            qDebug() << "Can't open pwmFile " << parent->path() + "/pwm" + QString::number(index);
        }
    }
}

void PwmFan::update()
{
    Fan::update();
    m_pwmStream.seek(0);
    m_pwmStream >> m_pwm;
    emit pwmChanged();
}

//void PwmFan::writePwm()
//{
//    m_pwmStream << m_pwm;
//}

void PwmFan::reset()
{
    setTemp(nullptr);
    setMinTemp(0);
    setMaxTemp(100);
    setMinPwm(255);
    setMaxPwm(255);
    setMinStart(255);
    setMinStop(255);
}


Temp::Temp(Hwmon *parent, uint index) : Sensor(parent, index)
{
    if (QDir(parent->path()).isReadable())
    {
        QFile *valueFile = new QFile(parent->path() + "/temp" + QString::number(index) + "_input", this);
        QFile labelFile(parent->path() + "/temp" + QString::number(index) + "_label");

        if (valueFile->open(QFile::ReadOnly))
        {
            m_valueStream.setDevice(valueFile);
            m_valueStream >> m_value;
            m_value /= 1000;
        }
        else
            qDebug() << "Can't open valueFile " << parent->path() + "/temp" + QString::number(index) + "_input";

        if (labelFile.open(QFile::ReadOnly))
        {
            m_label = QTextStream(&labelFile).readLine();
        }
        else
        {
            qDebug() << "Can't open labelFile " << parent->path() + "/temp" + QString::number(index) + "_label";
        }
    }
}

QString Temp::name() const
{
    KConfigGroup names = m_config->group("names");
    KConfigGroup localNames = names.group(m_parent->name());
    QString name = localNames.readEntry("temp" + QString::number(m_index), QString());
    if (name.isEmpty())
    {
        if (m_label.isEmpty())
            return "temp" + QString::number(m_index);
        return m_label;
    }
    return name;
}

void Temp::setName(const QString &name)
{
    KConfigGroup names = m_config->group("names");
    KConfigGroup localNames = names.group(m_parent->name());
    if (name != localNames.readEntry("temp" + QString::number(m_index), QString()))
    {
        localNames.writeEntry(m_parent->name() + "temp" + QString::number(m_index), name);
        emit nameChanged();
    }
}

void Temp::update()
{
    m_valueStream.seek(0);
    m_valueStream >> m_value;
    m_value /= 1000;
    emit valueChanged();
}
