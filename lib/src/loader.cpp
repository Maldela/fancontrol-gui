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

#include "loader.h"
#include "hwmon.h"
#include "sensors.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QTimer>
#include <QDebug>

#include <KF5/KAuth/kauthexecutejob.h>

#define HWMON_PATH "/sys/class/hwmon"

Loader::Loader(QObject *parent) : QObject(parent),
    m_interval(10),
    m_configUrl(QUrl::fromLocalFile("/etc/fancontrol")),
    m_error("Success"),
    m_timer(new QTimer(this))
{
    parseHwmons();
    
    m_timer->setSingleShot(false);
    m_timer->start(1);
    
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateSensors()));
}

void Loader::parseHwmons()
{
    foreach (Hwmon *hwmon, m_hwmons)
    {
        hwmon->deleteLater();
    }
    m_hwmons.clear();

    QDir hwmonDir(HWMON_PATH);
    QStringList list;
    if (hwmonDir.isReadable())
    {
        list = hwmonDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
    }
    else
    {
        qDebug() << HWMON_PATH << " is not readable!";
        return;
    }

    foreach (QString hwmonPath, list)
    {
        Hwmon *hwmon = new Hwmon(QFile::symLinkTarget(hwmonDir.absoluteFilePath(hwmonPath)), this);
        connect(hwmon, SIGNAL(configUpdateNeeded()), this, SLOT(createConfigFile()));
        connect(hwmon, SIGNAL(pwmFansChanged()), this, SLOT(emitAllPwmFansChanged()));
        connect(hwmon, SIGNAL(tempsChanged()), this, SLOT(emitAllTempsChanged()));
        connect(this, SIGNAL(sensorsUpdateNeeded()), hwmon, SLOT(updateSensors()));
        m_hwmons << hwmon;
    }
    emit hwmonsChanged();
    emit allPwmFansChanged();
    emit allTempsChanged();
}

bool Loader::load(const QUrl &url)
{
    QString fileName;
    if (url.isEmpty())
    {
        qDebug() << "Given empty url. Fallback to " << m_configUrl;
        fileName = m_configUrl.toLocalFile();
    }
    else if (url.isLocalFile())
        fileName = url.toLocalFile();
    else
    {
        setError("Url is not a local file");
        return false;
    }
    
    QTextStream stream;
    QFile file(fileName);
    QString fileContent;

    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        stream.setDevice(&file);
        fileContent = stream.readAll();
    }
    else if (file.exists())
    {
        KAuth::Action action("fancontrol.gui.helper.action");
        action.setHelperId("fancontrol.gui.helper");
        QVariantMap map;
        map["action"] = "read";
        map["filename"] = fileName;
        action.setArguments(map);
        KAuth::ExecuteJob *reply = action.execute();
        if (!reply->exec())
        {
            setError(reply->errorString());
            return false;
        }
        else
        {
            fileContent = reply->data()["content"].toString();
        }
    }
    else
    {
        setError("File does not exist"); 
        return false;
    }

    //Disconnect hwmons for performance reasons
    //They get reconnected later
    foreach (Hwmon *hwmon, m_hwmons)
    {
        disconnect(hwmon, SIGNAL(configUpdateNeeded()), this, SLOT(createConfigFile()));
        foreach (QObject *pwmFan, hwmon->pwmFans())
        {
            qobject_cast<PwmFan *>(pwmFan)->reset();
        }
    }

    stream.setString(&fileContent);
    QStringList lines;
    do
    {
        QString line(stream.readLine());
        if (line.startsWith('#')) continue;
        int offset = line.indexOf('#');
        if (offset != -1) line.truncate(offset-1);
        line = line.simplified();
        lines << line;
    }
    while(!stream.atEnd());

    foreach (QString line, lines)
    {
        if (line.startsWith("INTERVAL="))
        {
            line.remove("INTERVAL=");
            bool success;
            int interval = line.toInt(&success);
            if (success)
                setInterval(interval, false);
	    
            else
            {
                setError("Unable to parse interval line");
                return false;
            }
        }
        else if (line.startsWith("FCTEMPS="))
        {
            line.remove("FCTEMPS=");
            QStringList fctemps = line.split(' ');
            foreach (QString fctemp, fctemps)
            {
                QStringList nameValuePair = fctemp.split('=');
                if (nameValuePair.size() == 2)
                {
                    QString pwm = nameValuePair.at(0);
                    QString temp = nameValuePair.at(1);
                    int pwmSensorIndex = getSensorNumber(pwm);
                    int tempSensorIndex = getSensorNumber(temp);
                    Hwmon *pwmHwmon = m_hwmons.value(getHwmonNumber(pwm), Q_NULLPTR);

                    if (pwmHwmon)
                    {
                        Hwmon *tempHwmon = m_hwmons.value(getHwmonNumber(temp), Q_NULLPTR);
                        PwmFan *pwmPointer = pwmHwmon->pwmFan(pwmSensorIndex);
                        if (tempHwmon)
                        {
                            Temp *tempPointer = tempHwmon->temp(tempSensorIndex);

                            if (pwmPointer)
                            {
                                pwmPointer->setTemp(tempPointer);
                                pwmPointer->setMinPwm(0);
                            }
                        }
                        else if (pwmPointer)
                            pwmPointer->setTemp(Q_NULLPTR);
                    }
                }
            }
        }
        else if (line.startsWith("MINTEMP="))
        {
            line.remove("MINTEMP=");
            QStringList mintemps = line.split(' ');
            foreach (QString mintemp, mintemps)
            {
                QStringList nameValuePair = mintemp.split('=');
                if (nameValuePair.size() == 2)
                {
                    QString pwm = nameValuePair.at(0);
                    int value = nameValuePair.at(1).toInt();
                    int pwmHwmon = getHwmonNumber(pwm);
                    int pwmSensor = getSensorNumber(pwm);
                    PwmFan *pwmPointer = m_hwmons.value(pwmHwmon, Q_NULLPTR)->pwmFan(pwmSensor);
                    if (pwmPointer)
                    pwmPointer->setMinTemp(value);
                }
            }
        }
        else if (line.startsWith("MAXTEMP="))
        {
            line.remove("MAXTEMP=");
            QStringList maxtemps = line.split(' ');
            foreach (QString maxtemp, maxtemps)
            {
                QStringList nameValuePair = maxtemp.split('=');
                if (nameValuePair.size() == 2)
                {
                    QString pwm = nameValuePair.at(0);
                    int value = nameValuePair.at(1).toInt();
                    int pwmHwmon = getHwmonNumber(pwm);
                    int pwmSensor = getSensorNumber(pwm);
                    PwmFan *pwmPointer = m_hwmons.value(pwmHwmon, Q_NULLPTR)->pwmFan(pwmSensor);
                    if (pwmPointer)
                        pwmPointer->setMaxTemp(value);
                }
            }
        }
        else if (line.startsWith("MINSTART="))
        {
            line.remove("MINSTART=");
            QStringList minstarts = line.split(' ');
            foreach (QString minstart, minstarts)
            {
                QStringList nameValuePair = minstart.split('=');
                if (nameValuePair.size() == 2)
                {
                    QString pwm = nameValuePair.at(0);
                    int value = nameValuePair.at(1).toInt();
                    int pwmHwmon = getHwmonNumber(pwm);
                    int pwmSensor = getSensorNumber(pwm);
                    PwmFan *pwmPointer = m_hwmons.value(pwmHwmon, Q_NULLPTR)->pwmFan(pwmSensor);
                    if (pwmPointer)
                    pwmPointer->setMinStart(value);
                }
            }
        }
        else if (line.startsWith("MINSTOP="))
        {
            line.remove("MINSTOP=");
            QStringList minstops = line.split(' ');
            foreach (QString minstop, minstops)
            {
                QStringList nameValuePair = minstop.split('=');
                if (nameValuePair.size() == 2)
                {
                    QString pwm = nameValuePair.at(0);
                    int value = nameValuePair.at(1).toInt();
                    int pwmHwmon = getHwmonNumber(pwm);
                    int pwmSensor = getSensorNumber(pwm);
                    PwmFan *pwmPointer = m_hwmons.value(pwmHwmon, Q_NULLPTR)->pwmFan(pwmSensor);
                    if (pwmPointer)
                    pwmPointer->setMinStop(value);
                }
            }
        }
        else if (line.startsWith("MINPWM="))
        {
            line.remove("MINPWM=");
            QStringList minpwms = line.split(' ');
            foreach (QString minpwm, minpwms)
            {
                QStringList nameValuePair = minpwm.split('=');
                if (nameValuePair.size() == 2)
                {
                    QString pwm = nameValuePair.at(0);
                    int value = nameValuePair.at(1).toInt();
                    int pwmHwmon = getHwmonNumber(pwm);
                    int pwmSensor = getSensorNumber(pwm);
                    PwmFan *pwmPointer = m_hwmons.value(pwmHwmon, Q_NULLPTR)->pwmFan(pwmSensor);
                    if (pwmPointer)
                    pwmPointer->setMinPwm(value);
                }
            }
        }
        else if (line.startsWith("MAXPWM="))
        {
            line.remove("MAXPWM=");
            QStringList maxpwms = line.split(' ');
            foreach (QString maxpwm, maxpwms)
            {
                QStringList nameValuePair = maxpwm.split('=');
                if (nameValuePair.size() == 2)
                {
                    QString pwm = nameValuePair.at(0);
                    int value = nameValuePair.at(1).toInt();
                    int pwmHwmon = getHwmonNumber(pwm);
                    int pwmSensor = getSensorNumber(pwm);
                    PwmFan *pwmPointer = m_hwmons.value(pwmHwmon, Q_NULLPTR)->pwmFan(pwmSensor);
                    if (pwmPointer)
                    pwmPointer->setMaxPwm(value);
                }
            }
        }
    }
    
    //Connect hwmons again
    foreach (Hwmon *hwmon, m_hwmons)
    {
        connect(hwmon, SIGNAL(configUpdateNeeded()), this, SLOT(createConfigFile()));
    }
    
    m_configUrl = url;
    emit configUrlChanged();
    
    m_configFile = fileContent;
    emit configFileChanged();
    
    success();
    return true;
}

bool Loader::save(const QUrl &url)
{
    QString fileName;
    if (url.isEmpty())
    {
        qDebug() << "Given empty url. Fallback to " << m_configUrl;
        fileName = m_configUrl.toLocalFile();
    }
    else if (url.isLocalFile())
        fileName = url.toLocalFile();
    
    else
    {
        setError("Url is not a local file");
        return false;
    }
    
    QFile file(fileName);
    
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream stream(&file);
        stream << m_configFile;
    }
    else
    {
        KAuth::Action action("fancontrol.gui.helper.action");
        action.setHelperId("fancontrol.gui.helper");

        QVariantMap map;
        map["action"] = "write";
        map["filename"] = fileName;
        map["content"] = m_configFile;

        action.setArguments(map);
        KAuth::ExecuteJob *reply = action.execute();

        if (!reply->exec())
        {
            setError(reply->errorString());
            return false;
        }
    }
    
    success();
    return true;
}

void Loader::createConfigFile()
{
    QList<Hwmon *> usedHwmons;
    QList<PwmFan *> usedFans;
    foreach (Hwmon *hwmon, m_hwmons)
    {
        if (hwmon->pwmFans().size() > 0)
            usedHwmons << hwmon;
        foreach (QObject *fan, hwmon->pwmFans())
        {
            PwmFan *pwmFan = qobject_cast<PwmFan *>(fan);
            if (pwmFan->hasTemp() && pwmFan->temp())
            {
                usedFans << pwmFan;
                if (!usedHwmons.contains(pwmFan->temp()->parent()))
                    usedHwmons << pwmFan->temp()->parent();
            }
        }
    }
    
    QString configFile = "# This file was created by Fancontrol-GUI \n";

    if (m_interval != 0)
        configFile += "INTERVAL=" + QString::number(m_interval) + "\n";

    if (!usedHwmons.isEmpty())
    {
        configFile += "DEVPATH=";
        foreach (Hwmon *hwmon, usedHwmons)
        {
            QString sanitizedPath = hwmon->path();
            sanitizedPath.remove(QRegExp("^/sys/"));
            sanitizedPath.remove(QRegExp("/hwmon/hwmon\\d\\s*$"));
            configFile += "hwmon" + QString::number(hwmon->index()) + "=" + sanitizedPath + " ";
        }
        configFile += "\n";

        configFile += "DEVNAME=";
        foreach (Hwmon *hwmon, usedHwmons)
        {
            configFile += "hwmon" + QString::number(hwmon->index()) + "=" + hwmon->name().split('.').first() + " ";
        }
        configFile += "\n";

        if (!usedFans.isEmpty())
        {
            configFile += "FCTEMPS=";
            foreach (PwmFan *pwmFan, usedFans)
            {
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "pwm" + QString::number(pwmFan->index()) + "=";
                configFile += "hwmon" + QString::number(pwmFan->temp()->parent()->index()) + "/";
                configFile += "temp" + QString::number(pwmFan->temp()->index()) + "_input ";
            }
            configFile += "\n";

            configFile += "FCFANS=";
            foreach (PwmFan *pwmFan, usedFans)
            {
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "pwm" + QString::number(pwmFan->index()) + "=";
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "fan" + QString::number(pwmFan->index()) + "_input ";
            }
            configFile += "\n";

            configFile += "MINTEMP=";
            foreach (PwmFan *pwmFan, usedFans)
            {
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "pwm" + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->minTemp()) + " ";
            }
            configFile += "\n";

            configFile += "MAXTEMP=";
            foreach (PwmFan *pwmFan, usedFans)
            {
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "pwm" + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->maxTemp()) + " ";
            }
            configFile += "\n";

            configFile += "MINSTART=";
            foreach (PwmFan *pwmFan, usedFans)
            {
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "pwm" + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->minStart()) + " ";
            }
            configFile += "\n";

            configFile += "MINSTOP=";
            foreach (PwmFan *pwmFan, usedFans)
            {
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "pwm" + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->minStop()) + " ";
            }
            configFile += "\n";

            configFile += "MINPWM=";
            foreach (PwmFan *pwmFan, usedFans)
            {
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "pwm" + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->minPwm()) + " ";
            }
            configFile += "\n";

            configFile += "MAXPWM=";
            foreach (PwmFan *pwmFan, usedFans)
            {
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "pwm" + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->maxPwm()) + " ";
            }
            configFile += "\n";
        }
    }

    if (configFile != m_configFile)
    {
        m_configFile = configFile;
        emit configFileChanged();
    }
}

void Loader::setInterval(int interval, bool writeNewConfig)
{
    if (interval != m_interval)
    {
        m_interval = interval;
        emit intervalChanged();
        qDebug() << "Changed interval to" << interval;

        if (writeNewConfig)
            createConfigFile();
    }
}

void Loader::testFans()
{
    for (int i=0; i<m_hwmons.size(); i++)
    {
        m_hwmons.at(i)->testFans();
    }
}

QList<QObject *> Loader::hwmons() const
{
    QList<QObject *> list;
    foreach (Hwmon *hwmon, m_hwmons)
    {
        list << qobject_cast<QObject *>(hwmon);
    }
    return list;
}

QList< QObject* > Loader::allPwmFans() const
{
    QList<QObject *> list;
    foreach (const Hwmon *hwmon, m_hwmons)
    {
        list += hwmon->pwmFans();
    }
    return list;
}

QList< QObject* > Loader::allTemps() const
{
    QList<QObject *> list;
    foreach (const Hwmon *hwmon, m_hwmons)
    {
        list += hwmon->temps();
    }
    return list;
}

void Loader::setError (const QString &error)
{
    if (error != m_error) 
    {
        m_error = error;
        emit errorChanged();
        
    }
    qDebug() << error;
}

int Loader::getHwmonNumber(const QString &str)
{
    if (str.isEmpty())
        return -1;
    
    QString hwmon = str.split('/', QString::SkipEmptyParts).at(0);
    
    if (!hwmon.startsWith("hwmon"))
        return -1;
    
    bool success;
    
    hwmon.remove("hwmon");
    
    int result = hwmon.toInt(&success);
    
    if (success)
        return result;
    
    return -1;
}

int Loader::getSensorNumber(const QString &str)
{
    if (str.isEmpty())
        return -1;
    
    QStringList list = str.split('/', QString::SkipEmptyParts);
    
    if (list.size() != 2)
        return -1;
    
    QString sensor = list.at(1);
    
    if (!sensor.contains(QRegExp("pwm|fan|temp|_input")))
        return -1;
    
    bool success;
    
    sensor.remove(QRegExp("pwm|fan|temp|_input"));
    
    int result = sensor.toInt(&success);
    
    if (success)
        return result - 1;
    
    return -1;
}