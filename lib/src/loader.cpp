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

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QTimer>
#include <QtCore/QDebug>

#include <KAuth/KAuthExecuteJob>


#define HWMON_PATH "/sys/class/hwmon"


namespace Fancontrol
{

Loader::Loader(QObject *parent) : QObject(parent),
    m_interval(10),
    m_configUrl(QUrl::fromLocalFile("/etc/fancontrol")),
    m_error(""),
    m_timer(new QTimer(this))
{   
    parseHwmons();

    m_timer->setSingleShot(false);
    m_timer->start(1);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateSensors()));
}

void Loader::parseHwmons()
{
    QDir hwmonDir(HWMON_PATH);
    QStringList list;
    if (hwmonDir.isReadable())
        list = hwmonDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

    else if (hwmonDir.exists())
    {
        setError(QString(HWMON_PATH) + " is not readable!");
        return;
    }
    else
    {
        qCritical() << HWMON_PATH << " is not readable!";
        return;
    }

    QStringList dereferencedList;
    while (!list.isEmpty())
        dereferencedList << QFile::symLinkTarget(hwmonDir.absoluteFilePath(list.takeFirst()));

    foreach (Hwmon *hwmon, m_hwmons)
    {
        if (!dereferencedList.contains(hwmon->path()))
        {
            m_hwmons.removeOne(hwmon);
            emit hwmonsChanged();
            hwmon->deleteLater();
        }
        else
            hwmon->initialize();
    }

    foreach (const QString &hwmonPath, dereferencedList)
    {
        bool hwmonExists = false;

        foreach (const Hwmon *hwmon, m_hwmons)
        {
            if (hwmon->path() == hwmonPath)
            {
                hwmonExists = true;
                break;
            }
        }

        if (!hwmonExists)
        {
            Hwmon *newHwmon = new Hwmon(hwmonPath, this);
            if (newHwmon->isValid())
            {
                connect(this, SIGNAL(sensorsUpdateNeeded()), newHwmon, SLOT(updateSensors()));
                m_hwmons << newHwmon;
                emit hwmonsChanged();
            }
            else
                delete newHwmon;
        }
    }
}

PwmFan * Loader::getPwmFan(const QPair<int, int> &indexPair) const
{
    Hwmon *hwmon = m_hwmons.value(indexPair.first, Q_NULLPTR);

    if (!hwmon)
        return Q_NULLPTR;

    return hwmon->pwmFan(indexPair.second);
}

Temp * Loader::getTemp(const QPair<int, int> &indexPair) const
{
    Hwmon *hwmon = m_hwmons.value(indexPair.first, Q_NULLPTR);
    
    if (!hwmon)
        return Q_NULLPTR;

    return hwmon->temp(indexPair.second);
}

QPair<int, int> Loader::getEntryNumbers(const QString &entry)
{
    if (entry.isEmpty())
    {
        qWarning() << "Loader::getHwmonNumber(): given empty string.";
        return QPair<int, int>(-1, -1);
    }

    QStringList list = entry.split('/', QString::SkipEmptyParts);
    if (list.size() != 2)
    {
        qWarning() << "Invalid entry to parse:" << entry << "Should contain exactly one \'/\'";
        return QPair<int, int>(-1, -1);
    }
    QString hwmon = list.at(0);
    QString sensor = list.at(1);

    if (!hwmon.startsWith("hwmon"))
    {
        qWarning() << "Invalid entry to parse:" << entry << "Should begin with \"hwmon\"";
        return QPair<int, int>(-1, -1);
    }
    if (!sensor.contains(QRegExp("^(pwm|fan|temp)\\d+")))
    {
        qWarning() << "Invalid entry to parse:" << entry << "\n Sensor should begin with  pwm|fan|temp followed by a number";
        return QPair<int, int>(-1, -1);
    }

    bool success;

    hwmon.remove("hwmon");
    sensor.remove(QRegExp("^(pwm|fan|temp)"));
    sensor.remove("_input");

    int hwmonResult = hwmon.toInt(&success);
    if (!success)
    {
        qWarning() << "Invalid entry to parse:" << entry << "Could not convert" << hwmon << "to int";
        return QPair<int, int>(-1, -1);
    }
    int sensorResult = sensor.toInt(&success);
    if (!success)
    {
        qWarning() << "Invalid entry to parse:" << entry << "Could not convert" << sensor << "to int";
        return QPair<int, int>(-1, -1);
    }

    return QPair<int, int>(hwmonResult, sensorResult - 1);
}

void Loader::parseConfigLine(const QString &line, void (PwmFan::*memberSetFunction)(int)) const
{
    if (!memberSetFunction)
    {
        qWarning() << "Loader::parseConfigLine(): Null for member function pointer";
        return;
    }

    QStringList entries = line.split(' ');

    foreach (const QString &entry, entries)
    {
        QStringList fanValuePair = entry.split('=');
        if (fanValuePair.size() == 2)
        {
            QString fanString = fanValuePair.at(0);
            QString valueString = fanValuePair.at(1);
            bool success;
            int value = valueString.toInt(&success);

            if (success)
            {
                PwmFan *fan = getPwmFan(getEntryNumbers(fanString));
                if (fan)
                    (fan->*memberSetFunction)(value);
            }
            else
                qWarning() << valueString << "is not an int";
        }
        else
            qWarning() << "Invalid Entry:" << entry;
    }
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
            qDebug() << reply->error();
            setError(reply->errorString() + reply->errorText());
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
        if (line.startsWith('#'))
            continue;
        int offset = line.indexOf('#');
        if (offset != -1)
            line.truncate(offset-1);
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
            {
                setInterval(interval, false);
            }
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
            foreach (const QString &fctemp, fctemps)
            {
                QStringList nameValuePair = fctemp.split('=');
                if (nameValuePair.size() == 2)
                {
                    QString pwm = nameValuePair.at(0);
                    QString temp = nameValuePair.at(1);
                    PwmFan *pwmPointer = getPwmFan(getEntryNumbers(pwm));
                    Temp *tempPointer = getTemp(getEntryNumbers(temp));

                    if (pwmPointer && tempPointer)
                    {
                        pwmPointer->setTemp(tempPointer);
                        pwmPointer->setHasTemp(true);
                        pwmPointer->setMinPwm(0);
                    }
                }
                else
                    qWarning() << "Invalid entry:" << fctemp;
            }
        }
        else if (line.startsWith("MINTEMP="))
        {
            line.remove("MINTEMP=");
            parseConfigLine(line, &PwmFan::setMinTemp);
        }
        else if (line.startsWith("MAXTEMP="))
        {
            line.remove("MAXTEMP=");
            parseConfigLine(line, &PwmFan::setMaxTemp);
        }
        else if (line.startsWith("MINSTART="))
        {
            line.remove("MINSTART=");
            parseConfigLine(line, &PwmFan::setMinStart);
        }
        else if (line.startsWith("MINSTOP="))
        {
            line.remove("MINSTOP=");
            parseConfigLine(line, &PwmFan::setMinStop);
        }
        else if (line.startsWith("MINPWM="))
        {
            line.remove("MINPWM=");
            parseConfigLine(line, &PwmFan::setMinPwm);
        }
        else if (line.startsWith("MAXPWM="))
        {
            line.remove("MAXPWM=");
            parseConfigLine(line, &PwmFan::setMaxPwm);
        }
        else if (!line.startsWith("DEVNAME=") &&
                 !line.startsWith("DEVPATH=") &&
                 !line.startsWith("FCFANS="))
            qWarning() << "Unrecognized line in config:" << line;
    }

    //Connect hwmons again
    foreach (Hwmon *hwmon, m_hwmons)
    {
        connect(hwmon, SIGNAL(configUpdateNeeded()), this, SLOT(createConfigFile()));
    }

    emit configUrlChanged();

    m_configFile = fileContent;
    emit configFileChanged();

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
            qDebug() << reply->error();
            setError(reply->errorString() + reply->errorText());
            return false;
        }
    }

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

void Loader::detectSensors()
{
    KAuth::Action action("fancontrol.gui.helper.action");
    action.setHelperId("fancontrol.gui.helper");
    QVariantMap map;
    map["action"] = "detectSensors";

    action.setArguments(map);
    KAuth::ExecuteJob *job = action.execute();
    
    connect(job, SIGNAL(result(KJob*)), this, SLOT(handleDetectSensorsResult(KJob*)));
    job->start();
}

void Loader::handleDetectSensorsResult(KJob *job)
{
    if (job->error())
    {
        qDebug() << job->error();
        setError(job->errorString() + job->errorText());
    }
    else
        parseHwmons();
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

QList<QObject *> Loader::allPwmFans() const
{
    QList<QObject *> list;
    foreach (const Hwmon *hwmon, m_hwmons)
    {
        list += hwmon->pwmFans();
    }
    return list;
}

QList<QObject *> Loader::allTemps() const
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
    m_error = error;
    emit errorChanged();
    qCritical() << error;
}

}
