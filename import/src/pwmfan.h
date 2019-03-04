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


#ifndef PWMFAN_H
#define PWMFAN_H


#include "temp.h"
#include "fan.h"


class QTextStream;
class KJob;

namespace Fancontrol
{

class PwmFan : public Fan
{
    Q_OBJECT
    Q_PROPERTY(int pwm READ pwm WRITE setPwm NOTIFY pwmChanged)
    Q_PROPERTY(Temp * temp READ temp WRITE setTemp NOTIFY tempChanged)
    Q_PROPERTY(bool hasTemp READ hasTemp WRITE setHasTemp NOTIFY hasTempChanged)
    Q_PROPERTY(int minTemp READ minTemp WRITE setMinTemp NOTIFY minTempChanged)
    Q_PROPERTY(int maxTemp READ maxTemp WRITE setMaxTemp NOTIFY maxTempChanged)
    Q_PROPERTY(int minPwm READ minPwm WRITE setMinPwm NOTIFY minPwmChanged)
    Q_PROPERTY(int maxPwm READ maxPwm WRITE setMaxPwm NOTIFY maxPwmChanged)
    Q_PROPERTY(int minStart READ minStart WRITE setMinStart NOTIFY minStartChanged)
    Q_PROPERTY(int minStop READ minStop WRITE setMinStop NOTIFY minStopChanged)
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(bool testing READ testing NOTIFY testStatusChanged)
    Q_PROPERTY(TestStatus testStatus READ testStatus NOTIFY testStatusChanged)
    Q_PROPERTY(PwmEnable pwmEnable READ pwmEnable WRITE setPwmEnable NOTIFY pwmEnableChanged)

public:

    enum TestStatus
    {
        NotStarted,
        FindingStop1,
        FindingStop2,
        FindingStart,
        Finished,
        Cancelled,
        Error
    };
    Q_ENUM(TestStatus)

    enum PwmEnable
    {
        FullSpeed = 0,
        ManualControl = 1,
        BiosControl = 2
    };
    Q_ENUM(PwmEnable)

    explicit PwmFan(uint index, Hwmon *parent = Q_NULLPTR);
    virtual ~PwmFan();

    int pwm() const Q_DECL_OVERRIDE { return m_pwm; }
    Temp * temp() const { return m_temp; }
    bool hasTemp() const { return m_hasTemp; }
    int minTemp() const { return m_minTemp; }
    int maxTemp() const { return m_maxTemp; }
    int minPwm() const { return m_minPwm; }
    int maxPwm() const { return m_maxPwm; }
    int minStart() const { return m_minStart; }
    int minStop() const { return m_minStop; }
    PwmEnable pwmEnable() const { return m_pwmEnable; }
    TestStatus testStatus() const { return m_testStatus; }
    bool active() const;
    bool testing() const;
    bool setPwm(int pwm, bool write = true) Q_DECL_OVERRIDE;
    void setTemp(Temp *temp) { setHasTemp(temp != Q_NULLPTR); if (temp != m_temp) { m_temp = temp; emit tempChanged(); } }
    void setHasTemp(bool hasTemp) { if (hasTemp != m_hasTemp) { m_hasTemp = hasTemp; emit hasTempChanged(); } }
    void setMinTemp(int minTemp) { if (minTemp != m_minTemp) { m_minTemp = minTemp; emit minTempChanged(); } }
    void setMaxTemp(int maxTemp) { if (maxTemp != m_maxTemp) { m_maxTemp = maxTemp; emit maxTempChanged(); } }
    void setMinPwm(int minPwm);
    void setMaxPwm(int maxPwm);
    void setMinStart(int minStart) { if (minStart != m_minStart) { m_minStart = minStart; emit minStartChanged(); } }
    void setMinStop(int minStop) { if (minStop != m_minStop) { m_minStop = minStop; emit minStopChanged(); } }
    bool setPwmEnable(PwmEnable pwmEnable, bool write = true);
    void setActive(bool active);
    void toDefault() Q_DECL_OVERRIDE;
    bool isValid() const Q_DECL_OVERRIDE;
    void update() Q_DECL_OVERRIDE;
    void continueTest();

    Q_INVOKABLE void test();
    Q_INVOKABLE void abortTest();


signals:

    void pwmChanged();
    void tempChanged();
    void hasTempChanged();
    void minTempChanged();
    void maxTempChanged();
    void minPwmChanged();
    void maxPwmChanged();
    void minStartChanged();
    void minStopChanged();
    void activeChanged();
    void testStatusChanged(bool = false);
    void pwmEnableChanged();


protected:

    QTextStream *m_pwmStream;
    QTextStream *m_enableStream;


private:

    int m_pwm;
    PwmEnable m_pwmEnable;
    Temp *m_temp;
    bool m_hasTemp;
    int m_minTemp;
    int m_maxTemp;
    int m_minPwm;
    int m_maxPwm;
    int m_minStart;
    int m_minStop;
    int m_zeroRpm;
    TestStatus m_testStatus;
};

}

#endif // PWMFAN_H
