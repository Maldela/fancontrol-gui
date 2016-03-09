/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2015  Malte Veerman maldela@halloarsch.de
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "pwmfan.h"

#include "hwmon.h"
#include "fancontrolaction.h"

#include <QtCore/QTextStream>
#include <QtCore/QTimer>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QDebug>

#include <KConfigCore/KConfigGroup>
#include <KConfigCore/KSharedConfig>
#include <KAuth/KAuthExecuteJob>
#include <KI18n/KLocalizedString>


#define MAX_ERRORS_FOR_RPM_ZERO 10


namespace Fancontrol
{

PwmFan::PwmFan(Hwmon *parent, uint index) : Fan(parent, index),
    m_pwmStream(new QTextStream),
    m_modeStream(new QTextStream),
    m_temp(Q_NULLPTR),
    m_hasTemp(false),
    m_minTemp(0),
    m_maxTemp(100),
    m_minPwm(255),
    m_maxPwm(255),
    m_minStart(255),
    m_minStop(255),
    m_zeroRpm(0),
    m_testStatus(NotStarted)
{
    connect(this, SIGNAL(tempChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(hasTempChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(minTempChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(maxTempChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(minPwmChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(maxPwmChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(minStartChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(minStopChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(testingChanged()), parent, SLOT(updateConfig()));

    if (QDir(parent->path()).isReadable())
    {
        QFile *pwmFile = new QFile(parent->path() + "/pwm" + QString::number(index), this);
        if (pwmFile->open(QFile::ReadWrite))
        {
            m_pwmStream->setDevice(pwmFile);
            *m_pwmStream >> m_pwm;
        }
        else if (pwmFile->open(QFile::ReadOnly))
        {
            m_pwmStream->setDevice(pwmFile);
            *m_pwmStream >> m_pwm;
        }
        else
            qWarning() << "Can't open pwmFile " << pwmFile->fileName();

        QFile *pwmModeFile = new QFile(parent->path() + "/pwm" + QString::number(index) + "_mode", this);
        if (pwmModeFile->open(QFile::ReadWrite))
        {
            m_modeStream->setDevice(pwmModeFile);
            *m_modeStream >> m_pwmMode;
        }
        else if (pwmModeFile->open(QFile::ReadOnly))
        {
            m_modeStream->setDevice(pwmModeFile);
            *m_modeStream >> m_pwmMode;
        }
        else
            qWarning() << "Can't open pwmModeFile " << pwmModeFile->fileName();
    }
}

PwmFan::~PwmFan()
{
    delete m_pwmStream;
    delete m_modeStream;
}

void PwmFan::update()
{
    Fan::update();

    m_pwmStream->seek(0);
    setPwm(m_pwmStream->readAll().toInt(), false);

    m_modeStream->seek(0);
    setPwmMode(m_modeStream->readAll().toInt(), false);
}

void PwmFan::reset()
{
    Fan::reset();
    
    setHasTemp(false);
    setTemp(Q_NULLPTR);
    
    QIODevice *oldFile = m_pwmStream->device();
    delete m_pwmStream;
    delete oldFile;
    
    oldFile = m_modeStream->device();
    delete m_modeStream;
    delete oldFile;
    
    QFile *pwmFile = new QFile(m_parent->path() + "/pwm" + QString::number(m_index), this);
        if (pwmFile->open(QFile::ReadWrite))
        {
            m_pwmStream = new QTextStream(pwmFile);
            *m_pwmStream >> m_pwm;
        }
        else if (pwmFile->open(QFile::ReadOnly))
        {
            m_pwmStream = new QTextStream(pwmFile);
            *m_pwmStream >> m_pwm;
        }
        else
            qWarning() << "Can't open pwmFile " << pwmFile->fileName();

        QFile *pwmModeFile = new QFile(m_parent->path() + "/pwm" + QString::number(m_index) + "_mode", this);
        if (pwmModeFile->open(QFile::ReadWrite))
        {
            m_modeStream = new QTextStream(pwmModeFile);
            *m_modeStream >> m_pwmMode;
        }
        else if (pwmModeFile->open(QFile::ReadOnly))
        {
            m_modeStream = new QTextStream(pwmModeFile);
            *m_modeStream >> m_pwmMode;
        }
        else
            qWarning() << "Can't open pwmModeFile " << pwmModeFile->fileName();
}

bool PwmFan::setPwm(int pwm, bool write)
{
    if (m_pwm != pwm)
    {
        m_pwm = pwm;
        emit pwmChanged();

        if (write)
        {
            setPwmMode(1);
            
            if (m_pwmStream->device()->isWritable())
                *m_pwmStream << pwm;
            else
            {
                KAuth::Action action = newFancontrolAction();
                QVariantMap map;
                map[QStringLiteral("action")] = "write";
                map[QStringLiteral("filename")] = qobject_cast<QFile *>(m_pwmStream->device())->fileName();
                map[QStringLiteral("content")] = QString::number(pwm);
                action.setArguments(map);
                KAuth::ExecuteJob *job = action.execute();
                connect(job, SIGNAL(result(KJob*)), this, SLOT(handleSetPwmResult(KJob*)));
                job->start();
            }
        }
    }
    return true;
}

void PwmFan::handleSetPwmResult(KJob *job)
{
    if (job->error())
    {
        if (job->error() == KAuth::ActionReply::HelperBusyError)
        {
            qDebug() << "Helper busy...";
            
            QTimer::singleShot(50, this, [this] (){ setPwm(m_pwm); });
            return;
        }
        
        emit errorChanged(i18n("Could not set pwm: ") + job->errorText());
        return;
    }
    update();
}

bool PwmFan::setPwmMode(int pwmMode, bool write)
{
    if (m_pwmMode != pwmMode)
    {
        m_pwmMode = pwmMode;
        emit pwmModeChanged();

        if (write)
        {
            if (m_modeStream->device()->isWritable())
                *m_modeStream << pwmMode;

            else
            {
                KAuth::Action action = newFancontrolAction();
                
                QVariantMap map;
                map[QStringLiteral("action")] = QVariant("write");
                map[QStringLiteral("filename")] = qobject_cast<QFile *>(m_modeStream->device())->fileName();
                map[QStringLiteral("content")] = QString::number(pwmMode);
                action.setArguments(map);
                KAuth::ExecuteJob *job = action.execute();
                connect(job, SIGNAL(result(KJob*)), this, SLOT(handleSetPwmModeResult(KJob*)));
                job->start();
            }
        }
    }
    return true;
}

void PwmFan::handleSetPwmModeResult(KJob *job)
{
    if (job->error())
    {
        if (job->error() == KAuth::ActionReply::HelperBusyError)
        {
            qDebug() << "Helper busy...";
            
            QTimer::singleShot(50, this, [this] (){ setPwmMode(m_pwmMode); });
            return;
        }
        
        emit errorChanged(i18n("Could not set pwm mode: ") + job->errorText());
        return;
    }
    update();
}

void PwmFan::test()
{
    KAuth::Action action = newFancontrolAction();
    KAuth::ExecuteJob *job = action.execute();
    connect(job, SIGNAL(result(KJob*)), this, SLOT(handleTestAuthReply(KJob*)));
    job->start();
}

void PwmFan::handleTestAuthReply(KJob *job)
{  
    if (job->error())
    {
        if (job->error() == KAuth::ActionReply::HelperBusyError)
        {
            qDebug() << "Helper busy...";
            
            QTimer::singleShot(100, this, &PwmFan::test);
            return;
        }
        
        emit errorChanged(i18n("Authorization error: ") + job->errorText());
        m_testStatus = Error;
        emit testingChanged();
        return;
    }
    
    setPwm(255);
    
    m_testStatus = FindingStop1;
    emit testingChanged();
    
    QTimer::singleShot(500, this, SLOT(continueTest()));
    qDebug() << "Start testing...";
}

void PwmFan::abortTest()
{
    if (m_testStatus >= FindingStop1 && m_testStatus <= FindingStart)
    {
        qDebug() << "Abort testing";
        
        disconnect(this, 0, this, SLOT(continueTest()));

        m_testStatus = Cancelled;
        emit testingChanged();

        setPwm(255);
    }
}

void PwmFan::continueTest()
{
    KAuth::Action action = newFancontrolAction();
    
    if (action.status() != KAuth::Action::AuthorizedStatus)
    {
        m_testStatus = Error;
        emit testingChanged();
        return;
    }
    
    update();
    switch (m_testStatus)
    {
    case FindingStop1:
        if (m_rpm > 0)
        {
            setPwm(qMin(m_pwm * 0.95, m_pwm - 5.0));
            m_zeroRpm = 0;
        }
        else
        {
            if (m_zeroRpm < MAX_ERRORS_FOR_RPM_ZERO)
            {
                m_zeroRpm++;
            }
            else
            {
                m_testStatus = FindingStart;
                m_zeroRpm = 0;
                qDebug() << "Start finding start value...";
            }
        }
        QTimer::singleShot(500, this, SLOT(continueTest()));
        break;

    case FindingStart:
        if (m_rpm == 0)
            setPwm(m_pwm + 2);
        else
        {
            m_testStatus = FindingStop2;
            setMinStart(m_pwm);
            qDebug() << "Start finding stop value...";
        }
        QTimer::singleShot(1000, this, SLOT(continueTest()));
        break;

    case FindingStop2:
        if (m_rpm > 0)
        {
            setPwm(m_pwm - 1);
            m_zeroRpm = 0;
            QTimer::singleShot(1000, this, SLOT(continueTest()));
        }
        else
        {
            if (m_zeroRpm < MAX_ERRORS_FOR_RPM_ZERO)
            {
                m_zeroRpm++;
                QTimer::singleShot(500, this, SLOT(continueTest()));
            }
            else
            {
                m_testStatus = Finished;
                emit testingChanged();
                m_zeroRpm = 0;
                setMinStop(m_pwm + 5);
                qDebug() << "Finished testing!";
            }
        }
        break;

    default:
        break;
    }
}

bool PwmFan::testing() const
{
    return m_testStatus == FindingStop1 || m_testStatus == FindingStop2 || m_testStatus == FindingStart;
}

bool PwmFan::active() const
{
    KConfigGroup active = KSharedConfig::openConfig(QStringLiteral("fancontrol-gui"))->group("active");
    KConfigGroup localActive = active.group(m_parent->name());
    return localActive.readEntry("pwmfan" + QString::number(m_index), true);
}

void PwmFan::setActive(bool a)
{
    KConfigGroup active = KSharedConfig::openConfig(QStringLiteral("fancontrol-gui"))->group("active");
    KConfigGroup localActive = active.group(m_parent->name());
    if (a != localActive.readEntry("pwmfan" + QString::number(m_index), true))
    {
        localActive.writeEntry("pwmfan" + QString::number(m_index), a);
        emit activeChanged();
    }
}

}