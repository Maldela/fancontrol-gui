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

#include "loadertest.h"

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>


void LoaderTest::initTestCase()
{
    m_loader = new TestLoader;

    m_rpms << (QList<QString *>() << new QString << new QString << new QString);
    m_rpms << (QList<QString *>() << new QString << new QString << new QString);

    m_temps << (QList<QString *>() << new QString << new QString << new QString);
    m_temps << (QList<QString *>() << new QString << new QString << new QString);

    m_pwms << (QList<QString *>() << new QString << new QString);
    m_pwms << (QList<QString *>() << new QString << new QString);

    m_pwmModes << (QList<QString *>() << new QString << new QString);
    m_pwmModes << (QList<QString *>() << new QString << new QString);

    QCOMPARE(m_loader->hwmons().size(), 0);

    m_loader->addHwmon(new TestHwmon(QStringLiteral("radeon"), m_rpms.at(0), m_pwms.at(0), m_pwmModes.at(0), m_temps.at(0), 0, m_loader));
    m_loader->addHwmon(new TestHwmon(QStringLiteral("coretemp"), m_rpms.at(1), m_pwms.at(1), m_pwmModes.at(1), m_temps.at(1), 1, m_loader));

    QCOMPARE(m_loader->hwmons().size(), 2);
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

void LoaderTest::parseIntervalTest_data()
{
    QTest::addColumn<QString>("config");
    QTest::addColumn<int>("result");
    QTest::addColumn<QString>("error");
    QTest::addColumn<bool>("critical");

    QTest::newRow("valid4") << "INTERVAL=4" << 4 << "" << false;
    QTest::newRow("valid2") << "INTERVAL=2" << 2 << "" << false;
    QTest::newRow("valid2") << "INTERVAL= 3" << 3 << "" << false;
    QTest::newRow("invalid0") << "INTERVAL=0" << 0 << "Interval must be greater or equal to one!" << true;
    QTest::newRow("invalid6") << "INTERVA=6" << 6 << "Unrecognized line in config: \'INTERVA=6\'" << true;
    QTest::newRow("invalid1") << "INTEVAL=1" << 1 << "Unrecognized line in config: \'INTEVAL=1\'" << true;
}

void LoaderTest::parseIntervalTest()
{
    QFETCH(QString, config);
    QFETCH(int, result);
    QFETCH(QString, error);
    QFETCH(bool, critical);

    QSignalSpy spy(m_loader, SIGNAL(error(QString, bool)));

    m_loader->parse(config);

    QCOMPARE(spy.isEmpty(), error.isEmpty());

    if (spy.count())
    {
        QCOMPARE(spy.at(0).at(0).toString(), error);
        QCOMPARE(spy.at(0).at(1).toBool(), critical);
    }
    else
        QCOMPARE(m_loader->interval(), result);
}

void LoaderTest::parseFctempTest_data()
{
    QTest::addColumn<QString>("config");
    QTest::addColumn<PwmFan *>("fan");
    QTest::addColumn<Temp *>("temp");
    QTest::addColumn<QString>("error");
    QTest::addColumn<bool>("critical");

    QTest::newRow("fan02temp11") << "FCTEMPS=hwmon0/fan2=hwmon1/temp1" << m_loader->hwmons().at(0)->pwmFans().value(2) << m_loader->hwmons().at(1)->temps().value(1) << "" << false;
    QTest::newRow("fan11temp03") << "FCTEMPS=hwmon1/fan1=hwmon0/temp3" << m_loader->hwmons().at(1)->pwmFans().value(1) << m_loader->hwmons().at(0)->temps().value(3) << "" << false;
    QTest::newRow("fan12temp12") << "FCTEMPS=hwmon1/fan2=hwmon1/temp2" << m_loader->hwmons().at(1)->pwmFans().value(2) << m_loader->hwmons().at(1)->temps().value(2) << "" << false;
    QTest::newRow("invalid0") << "FCTEMPS=hwmon2/fan1=hwmon0/temp3" << static_cast<PwmFan *>(Q_NULLPTR) << m_loader->hwmons().at(0)->temps().value(3) << "Invalid fan entry: \'hwmon2/fan1\'" << true;
    QTest::newRow("invalid1") << "FCTEMPS=hwmon0/fan1=hwmon0/temp4" << m_loader->hwmons().at(0)->pwmFans().value(0) << static_cast<Temp *>(Q_NULLPTR) << "Invalid temp entry: \'hwmon0/temp4\'" << true;
}

void LoaderTest::parseFctempTest()
{
    QFETCH(QString, config);
    QFETCH(PwmFan *, fan);
    QFETCH(Temp *, temp);
    QFETCH(QString, error);
    QFETCH(bool, critical);

    QSignalSpy spy(m_loader, SIGNAL(error(QString, bool)));

    m_loader->parse(config);

    QCOMPARE(spy.isEmpty(), error.isEmpty());

    if (fan)
        QCOMPARE(fan->temp(), temp);

    if (spy.count())
    {
        QCOMPARE(spy.at(0).at(0).toString(), error);
        QCOMPARE(spy.at(0).at(1).toBool(), critical);
    }
    else if (fan)
        QCOMPARE(fan->hasTemp(), true);
}

void LoaderTest::parseDevnameTest_data()
{
    QTest::addColumn<QString>("config");
    QTest::addColumn<QString>("error");
    QTest::addColumn<bool>("critical");

    QTest::newRow("valid0") << "DEVNAME=hwmon0=radeon" << "" << false;
    QTest::newRow("valid1") << "DEVNAME=hwmon1=coretemp" << "" << false;
    QTest::newRow("valid2") << "DEVNAME=hwmon0=radeon hwmon1=coretemp" << "" << false;
    QTest::newRow("valid3") << "DEVNAME= hwmon1=coretemp hwmon0=radeon" << "" << false;
    QTest::newRow("invalid0") << "DEVNAME=hwmon2=radeon" << "Invalid DEVNAME: \'hwmon2=radeon\'! No hwmon with index 2" << true;
    QTest::newRow("invalid0") << "DEVNAME=hwmon1=radeon" << "Wrong name for hwmon 1! Should be \'coretemp\'" << true;
}

void LoaderTest::parseDevnameTest()
{
    QFETCH(QString, config);
    QFETCH(QString, error);
    QFETCH(bool, critical);

    QSignalSpy spy(m_loader, SIGNAL(error(QString, bool)));

    m_loader->parse(config);

    QCOMPARE(spy.isEmpty(), error.isEmpty());

    if (spy.count())
    {
        QCOMPARE(spy.at(0).at(0).toString(), error);
        QCOMPARE(spy.at(0).at(1).toBool(), critical);
    }
}

void LoaderTest::parseMintempTest_data()
{
    QTest::addColumn<QString>("config");
    QTest::addColumn<PwmFan *>("fan");
    QTest::addColumn<int>("result");
    QTest::addColumn<QString>("error");
    QTest::addColumn<bool>("critical");

    QTest::newRow("valid01") << "MINTEMP=hwmon0/fan1=20" << m_loader->pwmFan(0, 1) << 20 << "" << false;
    QTest::newRow("valid12") << "MINTEMP=hwmon1/fan2=35" << m_loader->pwmFan(1, 2) << 35 << "" << false;
    QTest::newRow("valid02") << "MINTEMP=hwmon0/fan2=-35" << m_loader->pwmFan(0, 2) << -35 << "" << false;
    QTest::newRow("valid11") << "MINTEMP= hwmon1/fan1=40" << m_loader->pwmFan(1, 1) << 40 << "" << false;
}

void LoaderTest::parseMintempTest()
{
    QFETCH(QString, config);
    QFETCH(PwmFan *, fan);
    QFETCH(int, result);
    QFETCH(QString, error);
    QFETCH(bool, critical);

    QSignalSpy spy(m_loader, SIGNAL(error(QString, bool)));

    m_loader->parse(config);

    QCOMPARE(spy.isEmpty(), error.isEmpty());

    if (spy.count())
    {
        QCOMPARE(spy.at(0).at(0).toString(), error);
        QCOMPARE(spy.at(0).at(1).toBool(), critical);
    }
    else
        QCOMPARE(fan->minTemp(), result);
}

void LoaderTest::parseMaxtempTest_data()
{
    QTest::addColumn<QString>("config");
    QTest::addColumn<PwmFan *>("fan");
    QTest::addColumn<int>("result");
    QTest::addColumn<QString>("error");
    QTest::addColumn<bool>("critical");

    QTest::newRow("valid01") << "MAXTEMP=hwmon0/fan1=80" << m_loader->pwmFan(0, 1) << 80 << "" << false;
    QTest::newRow("valid12") << "MAXTEMP=hwmon1/fan2=78 #iuf" << m_loader->pwmFan(1, 2) << 78 << "" << false;
    QTest::newRow("valid02") << "MAXTEMP=hwmon0/fan2=-78" << m_loader->pwmFan(0, 2) << -78 << "" << false;
    QTest::newRow("valid11") << "MAXTEMP= hwmon1/fan1=53" << m_loader->pwmFan(1, 1) << 53 << "" << false;
}

void LoaderTest::parseMaxtempTest()
{
    QFETCH(QString, config);
    QFETCH(PwmFan *, fan);
    QFETCH(int, result);
    QFETCH(QString, error);
    QFETCH(bool, critical);

    QSignalSpy spy(m_loader, SIGNAL(error(QString, bool)));

    m_loader->parse(config);

    QCOMPARE(spy.isEmpty(), error.isEmpty());

    if (spy.count())
    {
        QCOMPARE(spy.at(0).at(0).toString(), error);
        QCOMPARE(spy.at(0).at(1).toBool(), critical);
    }
    else
        QCOMPARE(fan->maxTemp(), result);
}

void LoaderTest::parseMinstartTest_data()
{
    QTest::addColumn<QString>("config");
    QTest::addColumn<PwmFan *>("fan");
    QTest::addColumn<int>("result");
    QTest::addColumn<QString>("error");
    QTest::addColumn<bool>("critical");

    QTest::newRow("valid01") << "MINSTART=hwmon0/fan1=20" << m_loader->pwmFan(0, 1) << 20 << "" << false;
    QTest::newRow("valid12") << "MINSTART=hwmon1/fan2=35" << m_loader->pwmFan(1, 2) << 35 << "" << false;
    QTest::newRow("valid02") << "MINSTART=hwmon0/fan2=0#rtg" << m_loader->pwmFan(0, 2) << 0 << "" << false;
    QTest::newRow("valid11") << "MINSTART= hwmon1/fan1=40" << m_loader->pwmFan(1, 1) << 40 << "" << false;
}

void LoaderTest::parseMinstartTest()
{
    QFETCH(QString, config);
    QFETCH(PwmFan *, fan);
    QFETCH(int, result);
    QFETCH(QString, error);
    QFETCH(bool, critical);

    QSignalSpy spy(m_loader, SIGNAL(error(QString, bool)));

    m_loader->parse(config);

    QCOMPARE(spy.isEmpty(), error.isEmpty());

    if (spy.count())
    {
        QCOMPARE(spy.at(0).at(0).toString(), error);
        QCOMPARE(spy.at(0).at(1).toBool(), critical);
    }
    else
        QCOMPARE(fan->minStart(), result);
}

void LoaderTest::parseMinstopTest_data()
{
    QTest::addColumn<QString>("config");
    QTest::addColumn<PwmFan *>("fan");
    QTest::addColumn<int>("result");
    QTest::addColumn<QString>("error");
    QTest::addColumn<bool>("critical");

    QTest::newRow("valid01") << "MINSTOP=hwmon0/fan1=20" << m_loader->pwmFan(0, 1) << 20 << "" << false;
    QTest::newRow("valid12") << "MINSTOP=hwmon1/fan2=35" << m_loader->pwmFan(1, 2) << 35 << "" << false;
    QTest::newRow("valid02") << "MINSTOP=hwmon0/fan2=0" << m_loader->pwmFan(0, 2) << 0 << "" << false;
    QTest::newRow("valid11") << "MINSTOP= hwmon1/fan1=40" << m_loader->pwmFan(1, 1) << 40 << "" << false;
}

void LoaderTest::parseMinstopTest()
{
    QFETCH(QString, config);
    QFETCH(PwmFan *, fan);
    QFETCH(int, result);
    QFETCH(QString, error);
    QFETCH(bool, critical);

    QSignalSpy spy(m_loader, SIGNAL(error(QString, bool)));

    m_loader->parse(config);

    QCOMPARE(spy.isEmpty(), error.isEmpty());

    if (spy.count())
    {
        QCOMPARE(spy.at(0).at(0).toString(), error);
        QCOMPARE(spy.at(0).at(1).toBool(), critical);
    }
    else
        QCOMPARE(fan->minStop(), result);
}

void LoaderTest::parseMinpwmTest_data()
{
    QTest::addColumn<QString>("config");
    QTest::addColumn<PwmFan *>("fan");
    QTest::addColumn<int>("result");
    QTest::addColumn<QString>("error");
    QTest::addColumn<bool>("critical");

    QTest::newRow("valid01") << "MINPWM=hwmon0/fan1=20#fgiuh" << m_loader->pwmFan(0, 1) << 20 << "" << false;
    QTest::newRow("valid12") << "MINPWM=hwmon1/fan2=35" << m_loader->pwmFan(1, 2) << 35 << "" << false;
    QTest::newRow("valid02") << "MINPWM=hwmon0/fan2=0" << m_loader->pwmFan(0, 2) << 0 << "" << false;
    QTest::newRow("valid11") << "MINPWM= hwmon1/fan1=40" << m_loader->pwmFan(1, 1) << 40 << "" << false;
    QTest::newRow("invalid02") << "MINPWM=hwmon0/fan2=256" << m_loader->pwmFan(0, 2) << 256 << "MinPwm cannot exceed 0-255!" << true;
    QTest::newRow("invalid11") << "MINPWM=hwmon1/fan2=-2" << m_loader->pwmFan(1, 2) << -2 << "MinPwm cannot exceed 0-255!" << true;
}

void LoaderTest::parseMinpwmTest()
{
    QFETCH(QString, config);
    QFETCH(PwmFan *, fan);
    QFETCH(int, result);
    QFETCH(QString, error);
    QFETCH(bool, critical);

    QSignalSpy spy(m_loader, SIGNAL(error(QString, bool)));

    m_loader->parse(config);

    QCOMPARE(spy.isEmpty(), error.isEmpty());

    if (spy.count())
    {
        QCOMPARE(spy.at(0).at(0).toString(), error);
        QCOMPARE(spy.at(0).at(1).toBool(), critical);
    }
    else
        QCOMPARE(fan->minPwm(), result);
}

void LoaderTest::parseMaxpwmTest_data()
{
    QTest::addColumn<QString>("config");
    QTest::addColumn<PwmFan *>("fan");
    QTest::addColumn<int>("result");
    QTest::addColumn<QString>("error");
    QTest::addColumn<bool>("critical");

    QTest::newRow("valid01") << "MAXPWM=hwmon0/fan1=20" << m_loader->pwmFan(0, 1) << 20 << "" << false;
    QTest::newRow("valid12") << "MAXPWM=hwmon1/fan2=35 #uivnriuhgfdn" << m_loader->pwmFan(1, 2) << 35 << "" << false;
    QTest::newRow("valid02") << "MAXPWM=hwmon0/fan2=0" << m_loader->pwmFan(0, 2) << 0 << "" << false;
    QTest::newRow("valid11") << "MAXPWM= hwmon1/fan1=40" << m_loader->pwmFan(1, 1) << 40 << "" << false;
    QTest::newRow("invalid02") << "MAXPWM=hwmon0/fan2=256" << m_loader->pwmFan(0, 2) << 256 << "MaxPwm cannot exceed 0-255!" << true;
    QTest::newRow("invalid11") << "MAXPWM=hwmon1/fan2=-2" << m_loader->pwmFan(1, 2) << -2 << "MaxPwm cannot exceed 0-255!" << true;
}

void LoaderTest::parseMaxpwmTest()
{
    QFETCH(QString, config);
    QFETCH(PwmFan *, fan);
    QFETCH(int, result);
    QFETCH(QString, error);
    QFETCH(bool, critical);

    QSignalSpy spy(m_loader, SIGNAL(error(QString, bool)));

    m_loader->parse(config);

    QCOMPARE(spy.isEmpty(), error.isEmpty());

    if (spy.count())
    {
        QCOMPARE(spy.at(0).at(0).toString(), error);
        QCOMPARE(spy.at(0).at(1).toBool(), critical);
    }
    else
        QCOMPARE(fan->maxPwm(), result);
}

void LoaderTest::parseUnrecognizableLineTest_data()
{
    QTest::addColumn<QString>("config");
    QTest::addColumn<QString>("error");
    QTest::addColumn<bool>("critical");

    QTest::newRow("valid01") << "MMAXPWM=hwmon0/fan1=20" << "Unrecognized line in config: \'MMAXPWM=hwmon0/fan1=20\'" << true;
}

void LoaderTest::parseUnrecognizableLineTest()
{
    QFETCH(QString, config);
    QFETCH(QString, error);
    QFETCH(bool, critical);

    QSignalSpy spy(m_loader, SIGNAL(error(QString, bool)));

    m_loader->parse(config);

    QCOMPARE(spy.count(), 1);

    if (spy.count())
    {
        QCOMPARE(spy.at(0).at(0).toString(), error);
        QCOMPARE(spy.at(0).at(1).toBool(), critical);
    }
}

void LoaderTest::createConfigTest()
{
    auto pwmFan = m_loader->pwmFan(0, 1);
    pwmFan->setTemp(m_loader->temp(1, 1));
    pwmFan->setMinTemp(20);
    pwmFan->setMaxTemp(60);
    pwmFan->setMaxPwm(200);
    pwmFan->setMinPwm(100);
    pwmFan->setMinStart(120);
    pwmFan->setMinStop(80);

    pwmFan = m_loader->pwmFan(1, 2);
    pwmFan->setTemp(m_loader->temp(1, 3));
    pwmFan->setMinTemp(30);
    pwmFan->setMaxTemp(70);
    pwmFan->setMaxPwm(255);
    pwmFan->setMinPwm(120);
    pwmFan->setMinStart(110);
    pwmFan->setMinStop(75);

    m_loader->setInterval(5);

    auto config = m_loader->createConfig();
    QString expectedConfig = "# This file was created by Fancontrol-GUI\n"
                             "INTERVAL=5\n"
                             "DEVPATH=hwmon0= hwmon1= \n"
                             "DEVNAME=hwmon0=radeon hwmon1=coretemp \n"
                             "FCTEMPS=hwmon0/pwm1=hwmon1/temp1_input hwmon1/pwm2=hwmon1/temp3_input \n"
                             "FCFANS=hwmon0/pwm1=hwmon0/fan1_input hwmon1/pwm2=hwmon1/fan2_input \n"
                             "MINTEMP=hwmon0/pwm1=20 hwmon1/pwm2=30 \n"
                             "MAXTEMP=hwmon0/pwm1=60 hwmon1/pwm2=70 \n"
                             "MINSTART=hwmon0/pwm1=120 hwmon1/pwm2=110 \n"
                             "MINSTOP=hwmon0/pwm1=80 hwmon1/pwm2=75 \n"
                             "MINPWM=hwmon0/pwm1=100 hwmon1/pwm2=120 \n"
                             "MAXPWM=hwmon0/pwm1=200 hwmon1/pwm2=255 \n";

    auto expectedLines = expectedConfig.split(QChar(QChar::LineFeed));
    auto configLines = config.split(QChar(QChar::LineFeed));

    QCOMPARE(configLines.size(), expectedLines.size());

    for (auto i=0; i<configLines.size(); i++)
        QCOMPARE(configLines.at(i), expectedLines.at(i));
}

QTEST_MAIN(LoaderTest);
