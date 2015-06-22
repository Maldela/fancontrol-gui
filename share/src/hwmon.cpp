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
#include <QDir>
#include <QTextStream>
#include <QDebug>

Hwmon::Hwmon(const QString &path, Loader *parent) : QObject(parent)
{
    m_parent = parent;
    m_path = path;
    m_index = path.split('/').last().remove("hwmon").toInt();
    QFile nameFile(path + "/name");
    if (nameFile.open(QFile::ReadOnly))
        m_name = QTextStream(&nameFile).readLine();
    else
        m_name = path.split('/').last();

    QDir dir(m_path);
    QStringList entrys = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    foreach (QString entry, entrys)
    {
        QString str = entry;
        int index = str.remove(QRegExp("\\D+")).toInt();
        if (entry.contains("fan") && entry.contains("input"))
        {
            if (QFile::exists(m_path + "/pwm" + QString::number(index)))
            {
                PwmFan *newPwmFan = new PwmFan(this, index);
                connect(this, SIGNAL(sensorsUpdateNeeded()), newPwmFan, SLOT(update()));
                m_pwmFans << newPwmFan;
                emit pwmFansChanged();
                m_fans << qobject_cast<Fan *>(newPwmFan);
                emit fansChanged();
            }
            else
            {
                Fan *newFan = new Fan(this, index);
                connect(this, SIGNAL(sensorsUpdateNeeded()), newFan, SLOT(update()));
                m_fans << newFan;
                emit fansChanged();
            }
        }

        if (entry.contains("temp") && entry.contains("input"))
        {
            Temp *newTemp = new Temp(this, index);
            connect(this, SIGNAL(sensorsUpdateNeeded()), newTemp, SLOT(update()));
            m_temps << newTemp;
            emit tempsChanged();
        }
    }
//    qDebug() << "New Hwmon" << m_temps.size() << m_pwmFans.size();
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
    for (int i=0; i<m_pwmFans.size(); i++)
    {
        m_pwmFans.at(i)->test();
    }
}
