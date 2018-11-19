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

#include "temptest.h"

#include <QtTest/QtTest>


void TempTest::initTestCase()
{
    m_tempString = QStringLiteral("25");
    m_temp = new TestTemp(&m_tempString);
}

void TempTest::cleanupTestCase()
{
    delete m_temp;
}

void TempTest::init()
{
    // Called before each testfunction is executed
}

void TempTest::cleanup()
{
    // Called after every testfunction
}

void TempTest::nameTest_data()
{
    QTest::addColumn<QString>("name");

    QTest::newRow("radeon") << "radeon";
    QTest::newRow("cpu")    << "cpu";
}

void TempTest::nameTest()
{
    QFETCH(QString, name);

    m_temp->setName(name);

    QCOMPARE(m_temp->name(), name);
}

void TempTest::valueTest_data()
{
    QTest::addColumn<QString>("value");
    QTest::addColumn<int>("result");

    QTest::newRow("0")   << "0"      << 0;
    QTest::newRow("10")  << "10000"  << 10;
    QTest::newRow("100") << "100000" << 100;
    QTest::newRow("55")  << "55000"  << 55;
}

void TempTest::valueTest()
{
    QFETCH(QString, value);
    QFETCH(int, result);

    m_tempString = value;
    m_temp->update();

    QCOMPARE(m_temp->value(), result);
}


QTEST_MAIN(TempTest)
