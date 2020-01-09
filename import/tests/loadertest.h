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

#ifndef LOADERTEST_H
#define LOADERTEST_H

#include <QtCore/QObject>

#include "loader.h"
#include "hwmon.h"
#include "temptest.h"
#include "fantest.h"
#include "pwmfantest.h"


using namespace Fancontrol;

class TestHwmon : public Hwmon
{
    Q_OBJECT

public:

    explicit TestHwmon(const QString &name, const QList<QString *> &rpms, const QList<QString *> &pwms, const QList<QString *> &pwmmodes, const QList<QString *> &temps, uint index = 0, Loader *parent = Q_NULLPTR) : Hwmon(QString(), parent)
    {
        m_name = name;
        m_index = index;

        if (parent)
        {
            connect(this, &Hwmon::configUpdateNeeded, parent, &Loader::updateConfig);
            connect(this, &Hwmon::error, parent, &Loader::error);
        }

        if (pwms.size() != pwmmodes.size())
            return;

        if (pwms.size() > rpms.size())
            return;

        for (auto i=0; i<pwms.size(); i++)
        {
            const auto newPwmFan = new TestPwmFan(pwms.at(i), pwmmodes.at(i), rpms.at(i), i+1, this);
            connect(this, &Hwmon::sensorsUpdateNeeded, newPwmFan, &PwmFan::update);

            if (parent)
                connect(newPwmFan, &PwmFan::testStatusChanged, parent, &Loader::handleTestStatusChanged);

            m_pwmFans.insert(newPwmFan->index(), newPwmFan);
            m_fans.insert(newPwmFan->index(), newPwmFan);
        }
        emit pwmFansChanged();

        for (auto i=m_pwmFans.size()-1; i<rpms.size(); i++)
        {
            const auto newFan = new TestFan(rpms.at(i), i+1, this);
            connect(this, &Hwmon::sensorsUpdateNeeded, newFan, &Fan::update);

            m_fans.insert(newFan->index(), newFan);
        }
        emit fansChanged();

        for (auto i=0; i<temps.size(); i++)
        {
            const auto newTemp = new TestTemp(temps.at(i), i+1, this);
            connect(this, &Hwmon::sensorsUpdateNeeded, newTemp, &Temp::update);

            m_temps.insert(newTemp->index(), newTemp);
        }
        emit tempsChanged();

        m_valid = true;
    }
};

class TestLoader : public Loader
{
    Q_OBJECT

public:

    explicit TestLoader(GUIBase *parent = Q_NULLPTR) : Loader(parent) {}

    void addHwmon(Hwmon *hwmon)
    {
        connect(this, &Loader::sensorsUpdateNeeded, hwmon, &Hwmon::sensorsUpdateNeeded);
        m_hwmons << hwmon;
        emit hwmonsChanged();
    }
    void parse(const QString &string)
    {
        parseConfig(string);
    }
    QString createConfig() const
    {
        return Loader::createConfig();
    }
    QPair<uint, uint> getEntryNumbers(const QString &entry)
    {
        return Loader::getEntryNumbers(entry);
    }
};

class LoaderTest : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void parseIntervalTest_data();
    void parseIntervalTest();

    void parseFctempTest_data();
    void parseFctempTest();

    void parseDevnameTest_data();
    void parseDevnameTest();

    void parseMintempTest_data();
    void parseMintempTest();

    void parseMaxtempTest_data();
    void parseMaxtempTest();

    void parseMinstartTest_data();
    void parseMinstartTest();

    void parseMinstopTest_data();
    void parseMinstopTest();

    void parseMinpwmTest_data();
    void parseMinpwmTest();

    void parseMaxpwmTest_data();
    void parseMaxpwmTest();

    void parseUnrecognizableLineTest_data();
    void parseUnrecognizableLineTest();

    void getEntryNumberTest_data();
    void getEntryNumberTest();

    void createConfigTest();


private:

    TestLoader *m_loader;
    QList<QList<QString *> > m_rpms;
    QList<QList<QString *> > m_pwms;
    QList<QList<QString *> > m_temps;
    QList<QList<QString *> > m_pwmModes;
};

#endif // LOADERTEST_H
