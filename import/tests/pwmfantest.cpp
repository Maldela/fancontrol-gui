/*
 * Copyright 2015  <copyright holder> <email>
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

#include "pwmfantest.h"

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>


void PwmFanTest::initTestCase()
{
    m_pwmString = QStringLiteral("0");
    m_modeString = QStringLiteral("0");
    m_fan = new TestPwmFan(&m_pwmString, &m_modeString, Q_NULLPTR);
}

void PwmFanTest::cleanupTestCase()
{
    delete m_fan;
}

void PwmFanTest::init()
{
    // Called before each testfunction is executed
}

void PwmFanTest::cleanup()
{
    // Called after every testfunction
}

void PwmFanTest::nameTest_data()
{
    QTest::addColumn<QString>("name");

    QTest::newRow("SmallFan")  << "SmallFan";
    QTest::newRow("fan4")      << "fan4";
}

void PwmFanTest::nameTest()
{
    QFETCH(QString, name);

    m_fan->setName(name);

    QCOMPARE(m_fan->name(), name);
}

void PwmFanTest::pwmTest_data()
{
    QTest::addColumn<int>("value");
    QTest::addColumn<QString>("error");

    QTest::newRow("0")    << 0    << "";
    QTest::newRow("100")  << 100  << "";
    QTest::newRow("255")  << 255  << "";
    QTest::newRow("-1")   << -1   << "Pwm cannot exceed 0-255!";
    QTest::newRow("256")  << 256  << "Pwm cannot exceed 0-255!";
}

void PwmFanTest::pwmTest()
{
    QFETCH(int, value);
    QFETCH(QString, error);

    QSignalSpy spy(m_fan, SIGNAL(error(QString,bool)));

    m_fan->setPwm(value);

    if (error.isEmpty())
        QCOMPARE(m_fan->pwm(), value);
    else
    {
        QCOMPARE(spy.at(0).at(0).toString(), error);
        QCOMPARE(spy.at(0).at(1).toBool(), true);
    }
}

void PwmFanTest::enableTest_data()
{
    QTest::addColumn<TestPwmFan::PwmEnable>("value");
    QTest::addColumn<QString>("error");

    QTest::newRow("0") << TestPwmFan::FullSpeed << "";
    QTest::newRow("1") << TestPwmFan::ManualControl << "";
    QTest::newRow("2") << TestPwmFan::BiosControl << "";
}

void PwmFanTest::enableTest()
{
    QFETCH(TestPwmFan::PwmEnable, value);
    QFETCH(QString, error);

    QSignalSpy spy(m_fan, SIGNAL(error(QString,bool)));

    m_fan->setPwmEnable(value);

    if (error.isEmpty())
        QCOMPARE(m_fan->pwmEnable(), value);
    else
    {
        QCOMPARE(spy.at(0).at(0).toString(), error);
        QCOMPARE(spy.at(0).at(1).toBool(), true);
    }
}

void PwmFanTest::activeTest()
{
    m_fan->setActive(false);

    QCOMPARE(m_fan->active(), false);

    m_fan->setActive(true);

    QCOMPARE(m_fan->active(), true);
}


QTEST_MAIN(PwmFanTest)
