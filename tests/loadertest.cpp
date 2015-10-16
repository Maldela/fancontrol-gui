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

#include "loadertest.h"

#include <QtTest/QtTest>


#define COMMA ,


QTEST_MAIN(LoaderTest);


void LoaderTest::initTestCase()
{
    m_loader = new Loader;
}

void LoaderTest::cleanupTestCase()
{
    delete m_loader;
}

void LoaderTest::init()
{
    // Called before each testfunction is executed
}

void LoaderTest::cleanup()
{
    // Called after every testfunction
}

void LoaderTest::getEntryNumbersTest_data()
{
    QTest::addColumn<QString>("entry");
    QTest::addColumn<QPair<int, int> >("result");
    
    QTest::newRow("valid1")     << "hwmon0/temp1"       << QPair<int, int>(0, 0);
    QTest::newRow("valid2")     << "hwmon1/pwm2"        << QPair<int, int>(1, 1);
    QTest::newRow("valid3")     << "hwmon2/temp8_input" << QPair<int, int>(2, 7);
    QTest::newRow("valid4")     << "hwmon3/fan1"        << QPair<int, int>(3, 0);
    QTest::newRow("invalid1")   << "hwmo0/temp1"        << QPair<int, int>(-1, -1);
    QTest::newRow("invalid2")   << "hwmonn0/temp1"      << QPair<int, int>(-1, -1);
    QTest::newRow("invalid3")   << "hwmon0/1"           << QPair<int, int>(-1, -1);
    QTest::newRow("invalid4")   << "hwmon0/pwmfan1"     << QPair<int, int>(-1, -1);
    QTest::newRow("invalid5")   << "hwmon0/fan1/temp3"  << QPair<int, int>(-1, -1);
}

void LoaderTest::getEntryNumbersTest()
{
    QFETCH(QString, entry);
    QFETCH(QPair<int COMMA int>, result);
    
    QCOMPARE(Loader::getEntryNumbers(entry), result);
}

void LoaderTest::parseConfigLineTest_data()
{
    QTest::addColumn<QString>("line");
    QTest::addColumn<int>("result");
}

void LoaderTest::parseConfigLineTest()
{

}


#include "loadertest.moc"
