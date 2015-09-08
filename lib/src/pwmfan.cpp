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

#include <QtCore/QTextStream>
#include <QtCore/QTimer>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QDebug>

#include <KConfigCore/KConfigGroup>
#include <KConfigCore/KSharedConfig>
#include <KAuth/KAuthExecuteJob>


#define MAX_ERRORS_FOR_RPM_ZERO 10


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
    m_testStatus(notTesting)
{
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
            qDebug() << "Can't open pwmFile " << pwmFile->fileName();

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
            qDebug() << "Can't open pwmModeFile " << pwmModeFile->fileName();
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
            qDebug() << "Can't open pwmFile " << pwmFile->fileName();

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
            qDebug() << "Can't open pwmModeFile " << pwmModeFile->fileName();
}

void PwmFan::setPwm(int pwm, bool write)
{
    if (m_pwm != pwm)
    {
        m_pwm = pwm;
        emit pwmChanged();

        if (write)
        {
            if (m_pwmStream->device()->isWritable())
                *m_pwmStream << pwm;
            else
            {
                KAuth::Action action("fancontrol.gui.helper.action");
                action.setHelperId("fancontrol.gui.helper");
                QVariantMap map;
                map["action"] = "write";
                map["filename"] = qobject_cast<QFile *>(m_pwmStream->device())->fileName();
                map["content"] = QString::number(pwm);
                action.setArguments(map);
                KAuth::ExecuteJob *reply = action.execute();

                if (!reply->exec())
                    qDebug() << reply->errorString() << reply->errorText();
            }
        }
    }
}

void PwmFan::setPwmMode(int pwmMode, bool write)
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
                KAuth::Action action("fancontrol.gui.helper.action");
                action.setHelperId("fancontrol.gui.helper");
                QVariantMap map;
                map["action"] = "write";
                map["filename"] = qobject_cast<QFile *>(m_modeStream->device())->fileName();
                map["content"] = QString::number(pwmMode);
                action.setArguments(map);
                KAuth::ExecuteJob *reply = action.execute();

                if (!reply->exec())
                    qDebug() << reply->errorString() << reply->errorText();
            }
        }
    }
}

void PwmFan::test()
{
    m_testStatus = findingStop1;
    emit testingChanged();

    setPwmMode(1);
    setPwm(255);
    QTimer::singleShot(500, this, SLOT(continueTest()));
    qDebug() << "Start testing...";
}

void PwmFan::abortTest()
{
    disconnect(SLOT(continueTest()));

    m_testStatus = notTesting;
    emit testingChanged();

    setPwm(255);
}

void PwmFan::continueTest()
{
    update();
    switch (m_testStatus)
    {
    case findingStop1:
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
                m_testStatus = findingStart;
                m_zeroRpm = 0;
                qDebug() << "Start finding start value...";
            }
        }
        QTimer::singleShot(500, this, SLOT(continueTest()));
        break;

    case findingStart:
        if (m_rpm == 0)
            setPwm(m_pwm + 2);
        else
        {
            m_testStatus = findingStop2;
            setMinStart(m_pwm);
            qDebug() << "Start finding stop value...";
        }
        QTimer::singleShot(1000, this, SLOT(continueTest()));
        break;

    case findingStop2:
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
                m_testStatus = notTesting;
                emit testingChanged();
                m_zeroRpm = 0;
                setMinStop(m_pwm + 5);
                qDebug() << "Finished testing!";
            }
        }
        break;

    case notTesting:
        break;

    default:
        break;
    }
}

bool PwmFan::active() const
{
    KConfigGroup active = KSharedConfig::openConfig("fancontrol-gui")->group("active");
    KConfigGroup localActive = active.group(m_parent->name());
    return localActive.readEntry("pwmfan" + QString::number(m_index), true);
}

void PwmFan::setActive(bool a)
{
    KConfigGroup active = KSharedConfig::openConfig("fancontrol-gui")->group("active");
    KConfigGroup localActive = active.group(m_parent->name());
    if (a != localActive.readEntry("pwmfan" + QString::number(m_index), true))
    {
        localActive.writeEntry("pwmfan" + QString::number(m_index), a);
        emit activeChanged();
    }
}