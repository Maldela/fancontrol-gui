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

#include "guibase.h"
#include "hwmon.h"
#include "pwmfan.h"
#include "fan.h"
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

Loader::Loader(GUIBase *parent) : QObject(parent),
    m_reactivateAfterTesting(true),
    m_interval(10),
    m_configUrl(QUrl::fromLocalFile(QStringLiteral(STANDARD_CONFIG_FILE))),
    m_timer(new QTimer(this)),
    m_sensorsDetected(false)
{
    if (parent)
        connect(this, &Loader::error, parent, &GUIBase::handleError);

    m_timer->setSingleShot(false);
    m_timer->start(1000);

    connect(m_timer, &QTimer::timeout, this, &Loader::sensorsUpdateNeeded);
}

void Loader::parseHwmons()
{
    const auto hwmonDir = QDir(QStringLiteral(HWMON_PATH));
    QStringList list;
    if (hwmonDir.isReadable())
        list = hwmonDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

    else if (hwmonDir.exists())
    {
        emit error(i18n("File is not readable: \"%1\"", QStringLiteral(HWMON_PATH)), true);
        return;
    }
    else
    {
        emit error(i18n("File does not exist: \"%1\"", QStringLiteral(HWMON_PATH)), true);
        return;
    }

    QStringList dereferencedList;
    while (!list.isEmpty())
        dereferencedList << QFile::symLinkTarget(hwmonDir.absoluteFilePath(list.takeFirst()));

    for (const auto &hwmon : m_hwmons)
    {
        if (!dereferencedList.contains(hwmon->path()))
        {
            hwmon->deleteLater();
            m_hwmons.removeOne(hwmon);
            emit hwmonsChanged();
        }
        else
            hwmon->initialize();
    }

    for (const auto &hwmonPath : dereferencedList)
    {
        auto hwmonExists = false;

        for (const auto &hwmon : m_hwmons)
        {
            if (hwmon->path() == hwmonPath)
            {
                hwmonExists = true;
                break;
            }
        }

        if (!hwmonExists)
        {
            auto newHwmon = new Hwmon(hwmonPath, this);
            if (newHwmon->isValid())
            {
                connect(this, &Loader::sensorsUpdateNeeded, newHwmon, &Hwmon::sensorsUpdateNeeded);
                m_hwmons << newHwmon;
                emit hwmonsChanged();
            }
            else
                delete newHwmon;
        }
    }
}

PwmFan * Loader::pwmFan(int hwmonIndex, int pwmFanIndex) const
{
    const auto hwmon = m_hwmons.value(hwmonIndex, Q_NULLPTR);

    if (!hwmon)
        return Q_NULLPTR;

    return hwmon->pwmFan(pwmFanIndex);
}

Temp * Loader::temp(int hwmonIndex, int tempIndex) const
{
    const auto hwmon = m_hwmons.value(hwmonIndex, Q_NULLPTR);

    if (!hwmon)
        return Q_NULLPTR;

    return hwmon->temp(tempIndex);
}

Fan * Loader::fan(int hwmonIndex, int fanIndex) const
{
    const auto hwmon = m_hwmons.value(hwmonIndex, Q_NULLPTR);

    if (!hwmon)
        return Q_NULLPTR;

    return hwmon->fan(fanIndex);
}

QPair<int, int> Loader::getEntryNumbers(const QString &entry)
{
    if (entry.isEmpty())
        return QPair<int, int>(-1, -1);

    auto list = entry.split('/', QString::SkipEmptyParts);
    if (list.size() != 2)
    {
        emit error(i18n("Invalid entry: \"%1\"", entry));
        return QPair<int, int>(-1, -1);
    }
    auto &hwmon = list[0];
    auto &sensor = list[1];

    if (!hwmon.startsWith(QStringLiteral("hwmon")))
    {
        emit error(i18n("Invalid entry: \"%1\"", entry));
        return QPair<int, int>(-1, -1);
    }
    if (!sensor.contains(QRegExp("^(pwm|fan|temp)\\d+")))
    {
        emit error(i18n("Invalid entry: \"%1\"", entry));
        return QPair<int, int>(-1, -1);
    }

    auto success = false;

    hwmon.remove(QStringLiteral("hwmon"));
    sensor.remove(QRegExp("^(pwm|fan|temp)"));
    sensor.remove(QStringLiteral("_input"));

    const auto hwmonResult = hwmon.toInt(&success);
    if (!success)
    {
        emit error(i18n("Invalid entry: \"%1\"", entry));
        return QPair<int, int>(-1, -1);
    }
    const auto sensorResult = sensor.toInt(&success);
    if (!success)
    {
        emit error(i18n("Invalid entry: \"%1\"", entry));
        return QPair<int, int>(-1, -1);
    }

    return QPair<int, int>(hwmonResult, sensorResult - 1);
}

bool Loader::parseConfig(QString config)
{
    //Disconnect hwmons for performance reasons
    //They get reconnected later
    for (const auto &hwmon : m_hwmons)
    {
        disconnect(hwmon, &Hwmon::configUpdateNeeded, this, &Loader::updateConfig);
        const auto pwmFans = hwmon->pwmFans();
        for (const auto &pwmFan : pwmFans)
        {
            qobject_cast<PwmFan *>(pwmFan)->reset();
        }
    }

    reset();

    QTextStream stream;
    stream.setString(&config, QIODevice::ReadOnly);
    QStringList lines;
    do
    {
        auto line(stream.readLine());

        if (line.startsWith('#') || line.trimmed().isEmpty())
            continue;

        const auto offset = line.indexOf('#');

        if (offset != -1)
            line.truncate(offset);

        line = line.simplified();
        lines << line;
    }
    while (!stream.atEnd());

    for (auto line : lines)
    {
        if (line.startsWith(QStringLiteral("INTERVAL=")))
        {
            line.remove(QStringLiteral("INTERVAL="));
            line = line.simplified();
            auto success = false;
            const auto interval = line.toInt(&success);

            if (success)
                setInterval(interval, false);
            else
            {
                //Connect hwmons again
                for (const auto &hwmon : m_hwmons)
                    connect(hwmon, &Hwmon::configUpdateNeeded, this, &Loader::updateConfig);

                emit error(i18n("Unable to parse interval line: \"%1\"", line), true);
                return false;
            }
        }
        else if (line.startsWith(QStringLiteral("FCTEMPS=")))
        {
            line.remove(QStringLiteral("FCTEMPS="));
            line = line.simplified();
            const auto fctemps = line.split(' ');
            for (const auto &fctemp : fctemps)
            {
                const auto nameValuePair = fctemp.split('=');
                if (nameValuePair.size() == 2)
                {
                    const auto pwmFanString = nameValuePair.at(0);
                    const auto tempString = nameValuePair.at(1);
                    const auto pwmPointer = pwmFan(getEntryNumbers(pwmFanString));
                    const auto tempPointer = temp(getEntryNumbers(tempString));

                    if (pwmPointer && tempPointer)
                    {
                        pwmPointer->setTemp(tempPointer);
                        pwmPointer->setHasTemp(true);
                        pwmPointer->setMinPwm(0);
                    }
                    else
                    {
                        if (!pwmPointer)
                            emit error(i18n("Invalid fan entry: \"%1\"", pwmFanString), true);

                        if (!tempPointer)
                            emit error(i18n("Invalid temp entry: \"%1\"", tempString), true);
                    }
                }
                else
                    emit error(i18n("Invalid entry: \"%1\"", fctemp), true);
            }
        }
        else if (line.startsWith(QStringLiteral("DEVNAME=")))
        {
            line.remove(QStringLiteral("DEVNAME="));
            line = line.simplified();
            const auto devnames = line.split(' ');
            for (const auto &devname : devnames)
            {
                const auto indexNamePair = devname.split('=');

                if (indexNamePair.size() == 2)
                {
                    auto index = indexNamePair.at(0);
                    const auto &name = indexNamePair[1];
                    auto success = false;
                    index.remove(QStringLiteral("hwmon"));
                    const auto hwmonPointer = m_hwmons.value(index.toInt(&success), Q_NULLPTR);

                    if (!success)
                    {
                        //Connect hwmons again
                        for (const auto &hwmon : m_hwmons)
                            connect(hwmon, &Hwmon::configUpdateNeeded, this, &Loader::updateConfig);

                        emit error(i18n("Invalid DEVNAME: \"%1\"!", devname), true);
                        return false;
                    }

                    if (!hwmonPointer)
                    {
                        //Connect hwmons again
                        for (const auto &hwmon : m_hwmons)
                            connect(hwmon, &Hwmon::configUpdateNeeded, this, &Loader::updateConfig);

                        emit error(i18n("Invalid DEVNAME: \"%1\"! No hwmon with index %2", devname, index), true);
                        return false;
                    }

                    if (hwmonPointer->name().split('.').first() != name)
                    {
                        //Connect hwmons again
                        for (const auto &hwmon : m_hwmons)
                            connect(hwmon, &Hwmon::configUpdateNeeded, this, &Loader::updateConfig);

                        emit error(i18n("Wrong name for hwmon %1! Should be \"%2\"", index, hwmonPointer->name().split('.').first()), true);
                        return false;
                    }
                }
                else
                    emit error(i18n("Invalid DEVNAME: \"%1\"!", devname), true);
            }
        }
        else if (line.startsWith(QStringLiteral("MINTEMP=")))
        {
            line.remove(QStringLiteral("MINTEMP="));
            parseConfigLine(line.simplified(), &PwmFan::setMinTemp);
        }
        else if (line.startsWith(QStringLiteral("MAXTEMP=")))
        {
            line.remove(QStringLiteral("MAXTEMP="));
            parseConfigLine(line.simplified(), &PwmFan::setMaxTemp);
        }
        else if (line.startsWith(QStringLiteral("MINSTART=")))
        {
            line.remove(QStringLiteral("MINSTART="));
            parseConfigLine(line.simplified(), &PwmFan::setMinStart);
        }
        else if (line.startsWith(QStringLiteral("MINSTOP=")))
        {
            line.remove(QStringLiteral("MINSTOP="));
            parseConfigLine(line.simplified(), &PwmFan::setMinStop);
        }
        else if (line.startsWith(QStringLiteral("MINPWM=")))
        {
            line.remove(QStringLiteral("MINPWM="));
            parseConfigLine(line.simplified(), &PwmFan::setMinPwm);
        }
        else if (line.startsWith(QStringLiteral("MAXPWM=")))
        {
            line.remove(QStringLiteral("MAXPWM="));
            parseConfigLine(line.simplified(), &PwmFan::setMaxPwm);
        }
        else if (!line.startsWith(QStringLiteral("DEVPATH=")) &&
            !line.startsWith(QStringLiteral("FCFANS=")))
        {
            //Connect hwmons again
            for (const auto &hwmon : m_hwmons)
                connect(hwmon, &Hwmon::configUpdateNeeded, this, &Loader::updateConfig);

            emit error(i18n("Unrecognized line in config: \"%1\"", line), true);
            return false;
        }
    }

    updateConfig();

    //Connect hwmons again
    for (const auto &hwmon : m_hwmons)
        connect(hwmon, &Hwmon::configUpdateNeeded, this, &Loader::updateConfig);

    return true;
}

void Loader::parseConfigLine(const QString &line, void (PwmFan::*memberSetFunction)(int))
{
    if (!memberSetFunction)
        return;

    const auto entries = line.split(' ');

    for (const auto &entry : entries)
    {
        const auto fanValuePair = entry.split('=');
        if (fanValuePair.size() == 2)
        {
            const auto pwmFanString = fanValuePair.at(0);
            const auto valueString = fanValuePair.at(1);
            auto success = false;
            const auto value = valueString.toInt(&success);

            if (success)
            {
                auto pwmFanPointer = pwmFan(getEntryNumbers(pwmFanString));
                if (pwmFanPointer)
                    (pwmFanPointer->*memberSetFunction)(value);
                else
                    emit error(i18n("Invalid fan entry: \"%1\"", pwmFanString), true);
            }
            else
                emit error(i18n("%1 is not an integer!", valueString));
        }
        else
            emit error(i18n("Invalid entry to parse: \"%1\"", entry));
    }
}

bool Loader::load(const QUrl &url)
{
    QString fileName;
    if (url.isEmpty())
    {
//        qDebug() << "Given empty url. Fallback to" << m_configUrl;
        fileName = m_configUrl.toLocalFile();
    }
    else if (url.isValid())
    {
        if (url.isLocalFile())
            fileName = url.toLocalFile();

        else
        {
            emit error(i18n("%1 is not a local file!", url.toDisplayString()));
            return false;
        }
    }
    else
    {
        emit error(i18n("%1 is not a valid url!", url.toDisplayString()));
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
        auto action = newFancontrolAction();

        if (action.isValid())
        {
            auto map = QVariantMap();
            map[QStringLiteral("action")] = QVariant("read");
            map[QStringLiteral("filename")] = fileName;
            action.setArguments(map);
            auto reply = action.execute();
            if (!reply->exec())
            {
                if (reply->error() == 4)
                {
                    qDebug() << "Loading of file aborted by user";
                    return false;
                }

                emit error(reply->errorString() + reply->errorText(), true);
                return false;
            }
            else
                fileContent = reply->data().value(QStringLiteral("content")).toString();
        }
        else
            emit error(i18n("Action not supported! Try running the application as root."), true);
    }
    else
    {
        emit error(i18n("File does not exist: \"%1\"" ,fileName));
        return false;
    }

    auto success = parseConfig(fileContent);

    if (success && !url.isEmpty())
    {
        m_configUrl = url;
        emit configUrlChanged();
    }

    return success;
}

bool Loader::save(const QUrl &url)
{
    QString fileName;
    if (url.isEmpty())
    {
//        qDebug() << "Given empty url. Fallback to " << m_configUrl;
        fileName = m_configUrl.toLocalFile();
    }
    else if (url.isLocalFile())
        fileName = url.toLocalFile();

    else
    {
        emit error(i18n("%1 is not a local file!", url.toDisplayString()), true);
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
        auto action = newFancontrolAction();

        if (action.isValid())
        {
            QVariantMap map;
            map[QStringLiteral("action")] = QVariant("write");
            map[QStringLiteral("filename")] = fileName;
            map[QStringLiteral("content")] = m_configFile;

            action.setArguments(map);
            auto reply = action.execute();

            if (!reply->exec())
            {
                if (reply->error() == 4)
                {
                    qDebug() << "Saving of file aborted by user";
                    return false;
                }

                emit error(reply->errorString() + reply->errorText(), true);
                return false;
            }
        }
        else
            emit error(i18n("Action not supported! Try running the application as root."), true);
    }

    return true;
}

void Loader::updateConfig()
{
    const auto configFile = createConfig();

    if (configFile != m_configFile)
    {
        m_configFile = configFile;
        emit configFileChanged();
    }
}

QString Loader::createConfig() const
{
    QList<Hwmon *> usedHwmons;
    QList<PwmFan *> usedFans;

    for (const auto &hwmon : m_hwmons)
    {
        if (hwmon->pwmFans().size() > 0 && !usedHwmons.contains(hwmon))
            usedHwmons << hwmon;

        const auto pwmFans = hwmon->pwmFans();
        for (const auto &pwmFan : pwmFans)
        {
            if (pwmFan->hasTemp() && pwmFan->temp() && !pwmFan->testing())
            {
                usedFans << pwmFan;
                if (!usedHwmons.contains(pwmFan->temp()->parent()))
                    usedHwmons << pwmFan->temp()->parent();
            }
        }
    }

    auto configFile = QStringLiteral("# This file was created by Fancontrol-GUI") + QChar(QChar::LineFeed);

    if (m_interval != 0)
        configFile += "INTERVAL=" + QString::number(m_interval) + QChar(QChar::LineFeed);

    if (!usedHwmons.isEmpty())
    {
        configFile += "DEVPATH=";

        for (const auto &hwmon : usedHwmons)
        {
            auto sanitizedPath = hwmon->path();
            sanitizedPath.remove(QRegExp("^/sys/"));
            sanitizedPath.remove(QRegExp("/hwmon/hwmon\\d\\s*$"));
            configFile += "hwmon" + QString::number(hwmon->index()) + "=" + sanitizedPath + QChar(QChar::Space);
        }
        configFile += QChar(QChar::LineFeed);

        configFile += "DEVNAME=";

        for (const auto &hwmon : usedHwmons)
            configFile += "hwmon" + QString::number(hwmon->index()) + "=" + hwmon->name().split('.').first() + QChar(QChar::Space);

        configFile += QChar(QChar::LineFeed);

        if (!usedFans.isEmpty())
        {
            configFile += "FCTEMPS=";

            for (const auto &pwmFan : usedFans)
            {
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "pwm" + QString::number(pwmFan->index()) + "=";
                configFile += "hwmon" + QString::number(pwmFan->temp()->parent()->index()) + "/";
                configFile += "temp" + QString::number(pwmFan->temp()->index()) + "_input ";
            }
            configFile += QChar(QChar::LineFeed);

            configFile += "FCFANS=";

            for (const auto &pwmFan : usedFans)
            {
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "pwm" + QString::number(pwmFan->index()) + "=";
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "fan" + QString::number(pwmFan->index()) + "_input ";
            }
            configFile += QChar(QChar::LineFeed);

            configFile += "MINTEMP=";

            for (const auto &pwmFan : usedFans)
            {
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "pwm" + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->minTemp()) + QChar(QChar::Space);
            }
            configFile += QChar(QChar::LineFeed);

            configFile += "MAXTEMP=";

            for (const auto &pwmFan : usedFans)
            {
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "pwm" + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->maxTemp()) + QChar(QChar::Space);
            }
            configFile += QChar(QChar::LineFeed);

            configFile += "MINSTART=";

            for (const auto &pwmFan : usedFans)
            {
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "pwm" + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->minStart()) + QChar(QChar::Space);
            }
            configFile += QChar(QChar::LineFeed);

            configFile += "MINSTOP=";

            for (const auto &pwmFan : usedFans)
            {
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "pwm" + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->minStop()) + QChar(QChar::Space);
            }
            configFile += QChar(QChar::LineFeed);

            configFile += "MINPWM=";

            for (const auto &pwmFan : usedFans)
            {
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "pwm" + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->minPwm()) + QChar(QChar::Space);
            }
            configFile += QChar(QChar::LineFeed);

            configFile += "MAXPWM=";

            for (const auto &pwmFan : usedFans)
            {
                configFile += "hwmon" + QString::number(pwmFan->parent()->index()) + "/";
                configFile += "pwm" + QString::number(pwmFan->index()) + "=";
                configFile += QString::number(pwmFan->maxPwm()) + QChar(QChar::Space);
            }
            configFile += QChar(QChar::LineFeed);
        }
    }

    return configFile;
}

void Loader::setInterval(int interval, bool writeNewConfig)
{
    if (interval < 1)
    {
        emit error(i18n("Interval must be greater or equal to one!"), true);
        return;
    }

    if (interval != m_interval)
    {
        m_interval = interval;
        emit intervalChanged();

        if (writeNewConfig)
            updateConfig();
    }
}

void Loader::testFans()
{
    for (const auto &hwmon : m_hwmons)
        hwmon->testFans();
}

void Loader::abortTestingFans()
{
    for (const auto &hwmon : m_hwmons)
        hwmon->abortTestingFans();
}

void Loader::detectSensors()
{
    auto program = QStringLiteral("sensors-detect");
    auto arguments = QStringList() << QStringLiteral("--auto");

    auto process = new QProcess(this);
    process->start(program, arguments);

    connect(process, static_cast<void(QProcess::*)(int)>(&QProcess::finished),
            this, static_cast<void(Loader::*)(int)>(&Loader::handleDetectSensorsResult));
}

void Loader::handleDetectSensorsResult(int exitCode)
{
    auto process = qobject_cast<QProcess *>(sender());

    if (exitCode)
    {
        if (process)
            emit error(process->readAllStandardOutput());

        auto action = newFancontrolAction();

        if (action.isValid())
        {
            QVariantMap map;
            map[QStringLiteral("action")] = QVariant("detectSensors");

            action.setArguments(map);
            auto job = action.execute();

            connect(job, &KAuth::ExecuteJob::result, this, static_cast<void(Loader::*)(KJob *)>(&Loader::handleDetectSensorsResult));
            job->start();
        }
        else
            emit error(i18n("Action not supported! Try running the application as root."), true);
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
//            qDebug() << "Aborted by user";
            return;
        }

        emit error(job->errorString() + job->errorText(), true);
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
    auto list = QList<QObject *>();
    for (const auto &hwmon : m_hwmons)
        list << qobject_cast<QObject *>(hwmon);

    return list;
}

void Loader::handleTestStatusChanged()
{
    auto testing = false;

    for (const auto &hwmon : m_hwmons)
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

void Loader::reset() const
{
    for (const auto &hwmon : m_hwmons)
        hwmon->reset();
}

}
