/*
 * <one line to give the library's name and an idea of what it does.>
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

#include "fantest.h"

#include <QtTest/QtTest>


void FanTest::initTestCase()
{
    m_rpmString = QStringLiteral("500");
    m_fan = new TestFan(&m_rpmString);
}

void FanTest::cleanupTestCase()
{
    delete m_fan;
}

void FanTest::init()
{
    // Called before each testfunction is executed
}

void FanTest::cleanup()
{
    // Called after every testfunction
}

void FanTest::nameTest_data()
{
    QTest::addColumn<QString>("name");

    QTest::newRow("BigFan")  << "BigFan";
    QTest::newRow("fan2")    << "fan2";
}

void FanTest::nameTest()
{
    QFETCH(QString, name);

    m_fan->setName(name);

    QCOMPARE(m_fan->name(), name);
}

void FanTest::rpmTest_data()
{
    QTest::addColumn<QString>("value");
    QTest::addColumn<int>("result");

    QTest::newRow("0")    << "0"     << 0;
    QTest::newRow("100")  << "100"   << 100;
    QTest::newRow("1000") << "1000"  << 1000;
    QTest::newRow("5500") << "5500"  << 5500;
}

void FanTest::rpmTest()
{
    QFETCH(QString, value);
    QFETCH(int, result);

    m_rpmString = value;
    m_fan->update();

    QCOMPARE(m_fan->rpm(), result);
}


QTEST_MAIN(FanTest)
