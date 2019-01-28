/*
 * Copyright 2015  Malte Veerman <malte.veerman@gmail.com>
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

#include <KConfigCore/KConfigGroup>
#include <KConfigCore/KSharedConfig>
#include <KAuth/KAuthExecuteJob>
#include <KI18n/KLocalizedString>


#define TEST_HWMON_NAME "test"
#define MAX_ERRORS_FOR_RPM_ZERO 10


namespace Fancontrol
{

PwmFan::PwmFan(uint index, Hwmon *parent) : Fan(index, parent),
    m_pwmStream(new QTextStream),
    m_enableStream(new QTextStream),
    m_pwm(0),
    m_pwmEnable(FullSpeed),
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
    if (parent)
    {
        connect(this, &PwmFan::tempChanged, parent, &Hwmon::configUpdateNeeded);
        connect(this, &PwmFan::hasTempChanged, parent, &Hwmon::configUpdateNeeded);
        connect(this, &PwmFan::minTempChanged, parent, &Hwmon::configUpdateNeeded);
        connect(this, &PwmFan::maxTempChanged, parent, &Hwmon::configUpdateNeeded);
        connect(this, &PwmFan::minPwmChanged, parent, &Hwmon::configUpdateNeeded);
        connect(this, &PwmFan::maxPwmChanged, parent, &Hwmon::configUpdateNeeded);
        connect(this, &PwmFan::minStartChanged, parent, &Hwmon::configUpdateNeeded);
        connect(this, &PwmFan::minStopChanged, parent, &Hwmon::configUpdateNeeded);
        connect(this, &PwmFan::testStatusChanged, parent, &Hwmon::configUpdateNeeded);

        if (QDir(parent->path()).isReadable())
        {
            const auto pwmFile = new QFile(parent->path() + "/pwm" + QString::number(index), this);

            if (pwmFile->open(QFile::ReadWrite))
            {
                m_pwmStream->setDevice(pwmFile);
                PwmFan::setPwm(m_pwmStream->readAll().toInt(), false);
            }
            else if (pwmFile->open(QFile::ReadOnly))
            {
                m_pwmStream->setDevice(pwmFile);
                PwmFan::setPwm(m_pwmStream->readAll().toInt(), false);
            }
            else
            {
                emit error(i18n("Can't open pwm file: \'%1\'", pwmFile->fileName()));
                delete pwmFile;
            }

            const auto pwmEnableFile = new QFile(parent->path() + "/pwm" + QString::number(index) + "_enable", this);

            if (pwmEnableFile->open(QFile::ReadWrite))
            {
                m_enableStream->setDevice(pwmEnableFile);
                setPwmEnable((PwmEnable)m_enableStream->readAll().toInt(), false);
            }
            else if (pwmEnableFile->open(QFile::ReadOnly))
            {
                m_enableStream->setDevice(pwmEnableFile);
                setPwmEnable((PwmEnable)m_enableStream->readAll().toInt(), false);
            }
            else
            {
                emit error(i18n("Can't open pwm_enable file: \'%1\'", pwmEnableFile->fileName()));
                delete pwmEnableFile;
            }
        }
    }
}

PwmFan::~PwmFan()
{
    auto device = m_pwmStream->device();
    delete m_pwmStream;
    delete device;
    device = m_enableStream->device();
    delete m_enableStream;
    delete device;
}

void PwmFan::update()
{
    Fan::update();

    m_pwmStream->seek(0);
    setPwm(m_pwmStream->readAll().toInt(), false);

    m_enableStream->seek(0);
    setPwmEnable((PwmEnable)m_enableStream->readAll().toInt(), false);
}

void PwmFan::toDefault()
{
    Fan::toDefault();

    setHasTemp(false);
    setTemp(Q_NULLPTR);
    setPwm(0, false);
    setPwmEnable(FullSpeed, false);
    setMinTemp(0);
    setMaxTemp(100);
    setMinPwm(255);
    setMaxPwm(255);
    setMinStart(255);
    setMinStop(255);
    m_zeroRpm = 0;

    if (m_testStatus != NotStarted)
    {
        m_testStatus = NotStarted;
        emit testStatusChanged();
    }

    if (m_pwmStream->device() && m_enableStream->device() && m_parent)
    {
        auto device = m_pwmStream->device();
        m_pwmStream->setDevice(Q_NULLPTR);
        delete device;

        device = m_enableStream->device();
        m_enableStream->setDevice(Q_NULLPTR);
        delete device;

        const auto pwmFile = new QFile(m_parent->path() + "/pwm" + QString::number(m_index), this);

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
        {
            emit error(i18n("Can't open pwm file: \'%1\'", pwmFile->fileName()));
            delete pwmFile;
        }

        const auto pwmEnableFile = new QFile(m_parent->path() + "/pwm" + QString::number(m_index) + "_enable", this);

        if (pwmEnableFile->open(QFile::ReadWrite))
        {
            m_enableStream->setDevice(pwmEnableFile);
            int pwmEnable;
            *m_enableStream >> pwmEnable;
            m_pwmEnable = (PwmEnable)pwmEnable;
        }
        else if (pwmEnableFile->open(QFile::ReadOnly))
        {
            m_enableStream->setDevice(pwmEnableFile);
            int pwmEnable;
            *m_enableStream >> pwmEnable;
            m_pwmEnable = (PwmEnable)pwmEnable;
        }
        else
        {
            emit error(i18n("Can't open pwm_enable file: \'%1\'", pwmEnableFile->fileName()));
            delete pwmEnableFile;
        }
    }
}

bool PwmFan::isValid() const
{
    return Fan::isValid() && (m_pwmStream->device() || m_pwmStream->string()) && (m_enableStream->device() || m_enableStream->string());
}

bool PwmFan::setPwm(int pwm, bool write)
{
    if (pwm < 0 || pwm > 255)
    {
        emit error(i18n("Pwm cannot exceed 0-255!"), true);
        return false;
    }

    if (m_pwm != pwm)
    {
        m_pwm = pwm;
        emit pwmChanged();

        if (write)
        {
            setPwmEnable(ManualControl, true);

            if (m_pwmStream->string() || (m_pwmStream->device() && m_pwmStream->device()->isWritable()))
                *m_pwmStream << pwm;
            else
            {
                auto action = newFancontrolAction();

                if (action.isValid())
                {
                    QVariantMap map;
                    map[QStringLiteral("action")] = "write";
                    map[QStringLiteral("filename")] = qobject_cast<QFile *>(m_pwmStream->device())->fileName();
                    map[QStringLiteral("content")] = QString::number(pwm);
                    action.setArguments(map);

                    const auto job = action.execute();
                    if (!job->exec())
                    {
                        if (job->error() == KAuth::ActionReply::HelperBusyError)
                        {
//                            qDebug() << "Helper busy...";

                            QTimer::singleShot(50, this, [this] (){ setPwmEnable(m_pwmEnable); });
                        }

                        emit error(i18n("Could not set pwm: %1", job->errorText()));
                    }
                    update();
                }
                else
                    emit error(i18n("Action not supported! Try running the application as root."), true);
            }
        }
    }
    return true;
}

bool PwmFan::setPwmEnable(PwmEnable pwmEnable, bool write)
{
    if (m_pwmEnable != pwmEnable)
    {
        m_pwmEnable = pwmEnable;
        emit pwmEnableChanged();

        if (write)
        {
            if (m_enableStream->string() || (m_enableStream->device() && m_enableStream->device()->isWritable()))
                *m_enableStream << pwmEnable;

            else
            {
                auto action = newFancontrolAction();

                if (action.isValid())
                {
                    QVariantMap map;
                    map[QStringLiteral("action")] = QVariant("write");
                    map[QStringLiteral("filename")] = qobject_cast<QFile *>(m_enableStream->device())->fileName();
                    map[QStringLiteral("content")] = QString::number(pwmEnable);
                    action.setArguments(map);

                    const auto job = action.execute();
                    if (!job->exec())
                    {
                        if (job->error() == KAuth::ActionReply::HelperBusyError)
                        {
//                            qDebug() << "Helper busy...";

                            QTimer::singleShot(50, this, [this] (){ setPwmEnable(m_pwmEnable); });
                        }

                        emit error(i18n("Could not set pwm enable: %1", job->errorText()));
                    }
                    update();
                }
                else
                    emit error(i18n("Action not supported! Try running the application as root."), true);
            }
        }
    }
    return true;
}

void PwmFan::setMinPwm(int minPwm)
{
    if (minPwm < 0 || minPwm > 255)
    {
        emit error(i18n("MinPwm cannot exceed 0-255!"), true);
        return;
    }

    if (minPwm != m_minPwm)
    {
        m_minPwm = minPwm;
        emit minPwmChanged();
    }
}

void PwmFan::setMaxPwm(int maxPwm)
{
    if (maxPwm < 0 || maxPwm > 255)
    {
        emit error(i18n("MaxPwm cannot exceed 0-255!"), true);
        return;
    }

    if (maxPwm != m_maxPwm)
    {
        m_maxPwm = maxPwm;
        emit maxPwmChanged();
    }
}

void PwmFan::test()
{
    if ((!m_enableStream->device()->isWritable() && !m_enableStream->string()) ||
        (!m_pwmStream->device()->isWritable() && !m_pwmStream->string()))
    {
        auto action = newFancontrolAction();

        if (action.isValid())
        {
            const auto job = action.execute(KAuth::Action::AuthorizeOnlyMode);

            if (!job->exec())
            {
                emit error(i18n("Authorization error: %1", job->errorText()));
                m_testStatus = Error;
                emit testStatusChanged();
                return;
            }
        }
        else
        {
            emit error(i18n("Action not supported! Try running the application as root."), true);
            return;
        }
    }

    setPwm(255, true);

    m_testStatus = FindingStop1;
    emit testStatusChanged();

    QTimer::singleShot(500, this, &PwmFan::continueTest);
//    qDebug() << "Start testing...";
}

void PwmFan::abortTest()
{
    if (m_testStatus >= FindingStop1 && m_testStatus <= FindingStart)
    {
//        qDebug() << "Abort testing";

        m_testStatus = Cancelled;
        emit testStatusChanged();

        setPwm(255);
        setPwmEnable(FullSpeed);
    }
}

void PwmFan::continueTest()
{
    if ((!m_enableStream->device()->isWritable() && !m_enableStream->string()) ||
        (!m_pwmStream->device()->isWritable() && !m_pwmStream->string()))
    {
        auto action = newFancontrolAction();

        if (action.status() != KAuth::Action::AuthorizedStatus)
        {
            m_testStatus = Error;
            emit testStatusChanged();
            return;
        }
    }

    update();

    switch (m_testStatus)
    {
    case FindingStop1:
        if (rpm() > 0)
        {
            if (m_pwm == 0)
            {
                error(i18n("Fan never stops."), false);
                setMinStart(0);
                setMinStop(0);
                setMinPwm(0);
                setPwm(255);
                m_testStatus = Finished;
                emit testStatusChanged();
                return;
            }

            setPwm(qMax(0, (int)qMin(m_pwm * 0.95, m_pwm - 5.0)));
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
//                qDebug() << "Start finding start value...";
            }
        }
        QTimer::singleShot(500, this, &PwmFan::continueTest);
        break;

    case FindingStart:
        if (rpm() == 0)
            if (m_pwm >= 255)
            {
                m_testStatus = Finished;
                emit testStatusChanged();

                m_zeroRpm = 0;
                setMinStop(255);
                setMinStart(255);

                break;
            }
            else
                setPwm(qMin(m_pwm + 2, 255));
        else
        {
            m_testStatus = FindingStop2;
            setMinStart(m_pwm);
//            qDebug() << "Start finding stop value...";
        }
        QTimer::singleShot(1000, this, &PwmFan::continueTest);
        break;

    case FindingStop2:
        if (rpm() > 0)
        {
            setPwm(qMax(0, m_pwm - 1));
            m_zeroRpm = 0;
            QTimer::singleShot(1000, this, &PwmFan::continueTest);
        }
        else
        {
            if (m_zeroRpm < MAX_ERRORS_FOR_RPM_ZERO)
            {
                m_zeroRpm++;
                QTimer::singleShot(500, this, &PwmFan::continueTest);
            }
            else
            {
                m_testStatus = Finished;
                emit testStatusChanged();
                m_zeroRpm = 0;
                setMinStop(qMin(255, m_pwm + 5));
                setMinPwm(qMin(m_minPwm, m_minStop));
                setPwm(255);
//                qDebug() << "Finished testing PwmFan" << m_index;
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
    const auto active = KSharedConfig::openConfig(QStringLiteral("fancontrol-gui"))->group("active");
    const auto localActive = active.group(m_parent ? m_parent->name() : QStringLiteral(TEST_HWMON_NAME));
    return localActive.readEntry("pwmfan" + QString::number(m_index), true);
}

void PwmFan::setActive(bool a)
{
    const auto active = KSharedConfig::openConfig(QStringLiteral("fancontrol-gui"))->group("active");
    auto localActive = active.group(m_parent ? m_parent->name() : QStringLiteral(TEST_HWMON_NAME));
    if (a != localActive.readEntry("pwmfan" + QString::number(m_index), true))
    {
        localActive.writeEntry("pwmfan" + QString::number(m_index), a);
        emit activeChanged();
    }
}

}
