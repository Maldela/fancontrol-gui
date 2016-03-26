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
#include "fancontrolaction.h"

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QTimer>
#include <QtCore/QProcess>
#include <QtCore/QDebug>

#include <KAuth/KAuthExecuteJob>
#include <KI18n/KLocalizedString>


#define HWMON_PATH "/sys/class/hwmon"

#ifndef STANDARD_CONFIG_FILE
#define STANDARD_CONFIG_FILE "/etc/fancontrol"
#endif


namespace Fancontrol
{

Loader::Loader(QObject *parent) : QObject(parent),
    m_reactivateAfterTesting(true),
    m_interval(10),
    m_configUrl(QUrl::fromLocalFile(QStringLiteral(STANDARD_CONFIG_FILE))),
    m_timer(new QTimer(this)),
    m_sensorsDetected(false)
{
    parseHwmons();

    m_timer->setSingleShot(false);
    m_timer->start(1);

    connect(m_timer, &QTimer::timeout, this, &Loader::updateSensors);
}

void Loader::parseHwmons()
{
    QDir hwmonDir(QStringLiteral(HWMON_PATH));
    QStringList list;
    if (hwmonDir.isReadable())
        list = hwmonDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

    else if (hwmonDir.exists())
    {
        setError(i18n("%1 is not readable!", QStringLiteral(HWMON_PATH)), true);
        return;
    }
    else
    {
        setError(i18n("%1 does not exist!", QStringLiteral(HWMON_PATH)), true);
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
                connect(this, &Loader::sensorsUpdateNeeded, newHwmon, &Hwmon::updateSensors);
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

    if (!hwmon.startsWith(QStringLiteral("hwmon")))
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

    hwmon.remove(QStringLiteral("hwmon"));
    sensor.remove(QRegExp("^(pwm|fan|temp)"));
    sensor.remove(QStringLiteral("_input"));

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
        qDebug() << "Given empty url. Fallback to" << m_configUrl;
        fileName = m_configUrl.toLocalFile();
    }
    else if (url.isValid())
    {
        if (url.isLocalFile())
            fileName = url.toLocalFile();

        else
        {
            setError(i18n("%1 is not a local file!", url.toDisplayString()));
            return false;
        }
    }
    else
    {
        setError(i18n("%1 is not a valid url!", url.toDisplayString()));
        return false;
    }

    QTextStream stream;
    QFile file(fileName);
    QString fileContent;

    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        if (!url.isEmpty())
        {
            m_configUrl = url;
            emit configUrlChanged();
        }

        stream.setDevice(&file);
        fileContent = stream.readAll();
    }
    else if (file.exists())
    {
        KAuth::Action action = newFancontrolAction();

        if (action.isValid())
        {
            QVariantMap map;
            map[QStringLiteral("action")] = QVariant("read");
            map[QStringLiteral("filename")] = fileName;
            action.setArguments(map);
            KAuth::ExecuteJob *reply = action.execute();
            if (!reply->exec())
            {
                if (reply->error() == 4)
                {
                    qDebug() << "Aborted by user";
                    return false;
                }

                qDebug() << "Error while loading:" << reply->error();
                setError(reply->errorString() + reply->errorText(), true);
                return false;
            }
            else
            {
                if (!url.isEmpty())
                {
                    m_configUrl = url;
                    emit configUrlChanged();
                }

                fileContent = reply->data().value(QStringLiteral("content")).toString();
            }
        }
        else
            setError(i18n("Action not supported! Try running the application as root."), true);
    }
    else
    {
        if (!url.isEmpty())
        {
            emit invalidConfigUrl();
            setError(i18n("%1 does not exist!", file.fileName()));
        }

        return false;
    }

    //Disconnect hwmons for performance reasons
    //They get reconnected later
    foreach (Hwmon *hwmon, m_hwmons)
    {
        disconnect(hwmon, &Hwmon::configUpdateNeeded, this, &Loader::createConfigFile);
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
        if (line.startsWith('#') || line.trimmed().isEmpty())
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
        if (line.startsWith(QStringLiteral("INTERVAL=")))
        {
            line.remove(QStringLiteral("INTERVAL="));
            bool success;
            int interval = line.toInt(&success);
            if (success)
            {
                setInterval(interval, false);
            }
            else
            {
                //Connect hwmons again
                foreach (Hwmon *hwmon, m_hwmons)
                    connect(hwmon, &Hwmon::configUpdateNeeded, this, &Loader::createConfigFile);

                setError(i18n("Unable to parse interval line: \n %1", line), true);
                return false;
            }
        }
        else if (line.startsWith(QStringLiteral("FCTEMPS=")))
        {
            line.remove(QStringLiteral("FCTEMPS="));
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
        else if (line.startsWith(QStringLiteral("DEVNAME=")))
        {
            line.remove(QStringLiteral("DEVNAME="));
            QStringList devnames = line.split(' ');
            foreach (const QString &devname, devnames)
            {
                QStringList indexNamePair = devname.split('=');
                if (indexNamePair.size() == 2)
                {
                    QString hwmon = indexNamePair.at(0);
                    QString name = indexNamePair.at(1);
                    bool success;
                    Hwmon *hwmonPointer = m_hwmons.value(hwmon.remove(QStringLiteral("hwmon")).toInt(&success), Q_NULLPTR);
                    if (!success)
                    {
                        //Connect hwmons again
                        foreach (Hwmon *hwmon, m_hwmons)
                            connect(hwmon, &Hwmon::configUpdateNeeded, this, &Loader::createConfigFile);

                        setError(i18n("Can not parse %1", devname), true);
                        return false;
                    }

                    if (!hwmonPointer || hwmonPointer->name().split('.').first() != name)
                    {
                        //Connect hwmons again
                        foreach (Hwmon *hwmon, m_hwmons)
                            connect(hwmon, &Hwmon::configUpdateNeeded, this, &Loader::createConfigFile);

                        setError(i18n("Invalid config file!"), true);
                        return false;
                    }
                }
            }
        }
        else if (line.startsWith(QStringLiteral("MINTEMP=")))
        {
            line.remove(QStringLiteral("MINTEMP="));
            parseConfigLine(line, &PwmFan::setMinTemp);
        }
        else if (line.startsWith(QStringLiteral("MAXTEMP=")))
        {
            line.remove(QStringLiteral("MAXTEMP="));
            parseConfigLine(line, &PwmFan::setMaxTemp);
        }
        else if (line.startsWith(QStringLiteral("MINSTART=")))
        {
            line.remove(QStringLiteral("MINSTART="));
            parseConfigLine(line, &PwmFan::setMinStart);
        }
        else if (line.startsWith(QStringLiteral("MINSTOP=")))
        {
            line.remove(QStringLiteral("MINSTOP="));
            parseConfigLine(line, &PwmFan::setMinStop);
        }
        else if (line.startsWith(QStringLiteral("MINPWM=")))
        {
            line.remove(QStringLiteral("MINPWM="));
            parseConfigLine(line, &PwmFan::setMinPwm);
        }
        else if (line.startsWith(QStringLiteral("MAXPWM=")))
        {
            line.remove(QStringLiteral("MAXPWM="));
            parseConfigLine(line, &PwmFan::setMaxPwm);
        }
        else if (!line.startsWith(QStringLiteral("DEVPATH=")) &&
            !line.startsWith(QStringLiteral("FCFANS=")))
        {
            //Connect hwmons again
            foreach (Hwmon *hwmon, m_hwmons)
                connect(hwmon, &Hwmon::configUpdateNeeded, this, &Loader::createConfigFile);

            setError(i18n("Unrecognized line in config:\n%1", line), true);
            return false;
        }
    }

    createConfigFile();

    //Connect hwmons again
    foreach (Hwmon *hwmon, m_hwmons)
        connect(hwmon, &Hwmon::configUpdateNeeded, this, &Loader::createConfigFile);

    emit configUrlChanged();

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
        setError(i18n("%1 is not a local file!", url.toDisplayString()), true);
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
        KAuth::Action action = newFancontrolAction();

        if (action.isValid())
        {
            QVariantMap map;
            map[QStringLiteral("action")] = QVariant("write");
            map[QStringLiteral("filename")] = fileName;
            map[QStringLiteral("content")] = m_configFile;

            action.setArguments(map);
            KAuth::ExecuteJob *reply = action.execute();

            if (!reply->exec())
            {
                if (reply->error() == 4)
                {
                    qDebug() << "Aborted by user";
                    return false;
                }

                qDebug() << "Error while saving:" << reply->error();
                setError(reply->errorString() + reply->errorText(), true);
                return false;
            }
        }
        else
            setError(i18n("Action not supported! Try running the application as root."), true);
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
            if (pwmFan->hasTemp() && pwmFan->temp() && !pwmFan->testing())
            {
                usedFans << pwmFan;
                if (!usedHwmons.contains(pwmFan->temp()->parent()))
                    usedHwmons << pwmFan->temp()->parent();
            }
        }
    }

    QString configFile = QStringLiteral("# This file was created by Fancontrol-GUI") + QChar(QChar::LineFeed);

    if (m_interval != 0)
        configFile += QStringLiteral("INTERVAL=") + QString::number(m_interval) + QChar(QChar::LineFeed);

    if (!usedHwmons.isEmpty())
    {
        configFile += QStringLiteral("DEVPATH=");
        foreach (Hwmon *hwmon, usedHwmons)
        {
            QString sanitizedPath = hwmon->path();
            sanitizedPath.remove(QRegExp("^/sys/"));
            sanitizedPath.remove(QRegExp("/hwmon/hwmon\\d\\s*$"));
            configFile += QStringLiteral("hwmon") + QString::number(hwmon->index()) + "=" + sanitizedPath + QChar(QChar::Space);
        }
        configFile += QChar(QChar::LineFeed);

        configFile += QStringLiteral("DEVNAME=");
        foreach (Hwmon *hwmon, usedHwmons)
        {
            configFile += QStringLiteral("hwmon") + QString::number(hwmon->index()) + "=" + hwmon->name().split('.').first() + QChar(QChar::Space);
        }
        configFile += QChar(QChar::LineFeed);

        if (!usedFans.isEmpty())
        {
            configFile += QStringLiteral("FCTEMPS=");
            foreach (PwmFan *pwmFan, usedFans)
            {
                configFile += QStringLiteral("hwmon") + QString::number(pwmFan->parent()->index()) + "/";
                configFile += QStringLiteral("pwm") + QString::number(pwmFan->index()) + "=";
                configFile += QStringLiteral("hwmon") + QString::number(pwmFan->temp()->parent()->index()) + "/";
                configFile += QStringLiteral("temp") + QString::number(pwmFan->temp()->index()) + QStringLiteral("_input ");
            }
            configFile += QChar(QChar::LineFeed);

            configFile += QStringLiteral("FCFANS=");
            foreach (PwmFan *pwmFan, usedFans)
            {
                configFile += QStringLiteral("hwmon") + QString::number(pwmFan->parent()->index()) + "/";
                configFile += QStringLiteral("pwm") + QString::number(pwmFan->index()) + "=";
                configFile += QStringLiteral("hwmon") + QString::number(pwmFan->parent()->index()) + "/";
                configFile += QStringLiteral("fan") + QString::number(pwmFan->index()) + QStringLiteral("_input ");
            }
            configFile += QChar(QChar::LineFeed);

            configFile += QStringLiteral("MINTEMP=");
            foreach (PwmFan *pwmFan, usedFans)
            {
                configFile += QStringLiteral("hwmon") + QString::number(pwmFan->parent()->index()) + "/";
                configFile += QStringLiteral("pwm") + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->minTemp()) + QChar(QChar::Space);
            }
            configFile += QChar(QChar::LineFeed);

            configFile += QStringLiteral("MAXTEMP=");
            foreach (PwmFan *pwmFan, usedFans)
            {
                configFile += QStringLiteral("hwmon") + QString::number(pwmFan->parent()->index()) + "/";
                configFile += QStringLiteral("pwm") + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->maxTemp()) + QChar(QChar::Space);
            }
            configFile += QChar(QChar::LineFeed);

            configFile += QStringLiteral("MINSTART=");
            foreach (PwmFan *pwmFan, usedFans)
            {
                configFile += QStringLiteral("hwmon") + QString::number(pwmFan->parent()->index()) + "/";
                configFile += QStringLiteral("pwm") + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->minStart()) + QChar(QChar::Space);
            }
            configFile += QChar(QChar::LineFeed);

            configFile += QStringLiteral("MINSTOP=");
            foreach (PwmFan *pwmFan, usedFans)
            {
                configFile += QStringLiteral("hwmon") + QString::number(pwmFan->parent()->index()) + "/";
                configFile += QStringLiteral("pwm") + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->minStop()) + QChar(QChar::Space);
            }
            configFile += QChar(QChar::LineFeed);

            configFile += QStringLiteral("MINPWM=");
            foreach (PwmFan *pwmFan, usedFans)
            {
                configFile += QStringLiteral("hwmon") + QString::number(pwmFan->parent()->index()) + "/";
                configFile += QStringLiteral("pwm") + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->minPwm()) + QChar(QChar::Space);
            }
            configFile += QChar(QChar::LineFeed);

            configFile += QStringLiteral("MAXPWM=");
            foreach (PwmFan *pwmFan, usedFans)
            {
                configFile += QStringLiteral("hwmon") + QString::number(pwmFan->parent()->index()) + "/";
                configFile += QStringLiteral("pwm") + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->maxPwm()) + QChar(QChar::Space);
            }
            configFile += QChar(QChar::LineFeed);
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

void Loader::abortTestingFans()
{
    for (int i=0; i<m_hwmons.size(); i++)
    {
        m_hwmons.at(i)->abortTestingFans();
    }
}

void Loader::detectSensors()
{
    QString program = QStringLiteral("sensors-detect");
    QStringList arguments = QStringList() << QStringLiteral("--auto");

    QProcess *process = new QProcess(this);
    process->start(program, arguments);

    connect(process, static_cast<void(QProcess::*)(int)>(&QProcess::finished),
            this, static_cast<void(Loader::*)(int)>(&Loader::handleDetectSensorsResult));
}

void Loader::handleDetectSensorsResult(int exitCode)
{
    QProcess *process = qobject_cast<QProcess *>(sender());

    if (exitCode)
    {
        if (process)
            setError(process->readAllStandardOutput());

        KAuth::Action action = newFancontrolAction();

        if (action.isValid())
        {
            QVariantMap map;
            map[QStringLiteral("action")] = QVariant("detectSensors");

            action.setArguments(map);
            KAuth::ExecuteJob *job = action.execute();

            connect(job, &KAuth::ExecuteJob::result, this, static_cast<void(Loader::*)(KJob *)>(&Loader::handleDetectSensorsResult));
            job->start();
        }
        else
            setError(i18n("Action not supported! Try running the application as root."), true);
    }
    else
    {
        if (!m_sensorsDetected)
        {
            m_sensorsDetected = true;
            emit sensorsDetectedChanged();
        }

        parseHwmons();
    }

    if (process)
        process->deleteLater();
}

void Loader::handleDetectSensorsResult(KJob *job)
{
    if (job->error())
    {
        if (job->error() == 4)
        {
            qDebug() << "Aborted by user";
            return;
        }

        qDebug() << "Error while detecting sensors:" << job->error();
        setError(job->errorString() + job->errorText(), true);
    }
    else
    {
        if (!m_sensorsDetected)
        {
            m_sensorsDetected = true;
            emit sensorsDetectedChanged();
        }

        parseHwmons();
    }
}

QList<QObject *> Loader::hwmonsAsObjects() const
{
    QList<QObject *> list;
    foreach (Hwmon *hwmon, m_hwmons)
    {
        list << qobject_cast<QObject *>(hwmon);
    }
    return list;
}

QList<QObject *> Loader::allTemps() const
{
    QList<QObject *> list;
    foreach (const Hwmon *hwmon, m_hwmons)
    {
        list += hwmon->tempsAsObjects();
    }
    return list;
}

void Loader::setError (const QString &error, bool critical)
{
    m_error = error;
    emit errorChanged();

    if (critical)
    {
        qCritical() << error;
        emit criticalError();
    }
    else
        qWarning() << error;
}

void Loader::handleTestStatusChanged()
{
    bool testing = false;

    foreach (const Hwmon *hwmon, m_hwmons)
    {
        if (hwmon->testing() == true)
        {
            testing = true;
            break;
        }
    }

    if (!testing && !m_reactivateAfterTesting)
        return;

    emit requestSetServiceActive(!testing);
}

void Loader::setRestartServiceAfterTesting(bool restart)
{
    if (m_reactivateAfterTesting == restart)
        return;

    m_reactivateAfterTesting = restart;
    emit restartServiceAfterTestingChanged();
}

}
