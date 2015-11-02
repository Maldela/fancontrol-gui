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
    Q_PROPERTY(bool testing READ testing NOTIFY testingChanged)
    Q_PROPERTY(int pwmMode READ pwmMode WRITE setPwmMode NOTIFY pwmModeChanged)

public:

    explicit PwmFan(Hwmon *parent, uint index);
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
    int pwmMode() const { return m_pwmMode; }
    bool active() const;
    bool testing() const;
    bool setPwm(int pwm, bool write = true) Q_DECL_OVERRIDE;
    void setTemp(Temp *temp) { setHasTemp(temp != Q_NULLPTR); if (temp != m_temp) { m_temp = temp; emit tempChanged(); } }
    void setHasTemp(bool hasTemp) { if (hasTemp != m_hasTemp) { m_hasTemp = hasTemp; emit hasTempChanged(); } }
    void setMinTemp(int minTemp) { if (minTemp != m_minTemp) { m_minTemp = minTemp; emit minTempChanged(); } }
    void setMaxTemp(int maxTemp) { if (maxTemp != m_maxTemp) { m_maxTemp = maxTemp; emit maxTempChanged(); } }
    void setMinPwm(int minPwm) { if (minPwm != m_minPwm) { m_minPwm = minPwm; emit minPwmChanged(); } }
    void setMaxPwm(int maxPwm) { if (maxPwm != m_maxPwm) { m_maxPwm = maxPwm; emit maxPwmChanged(); } }
    void setMinStart(int minStart) { if (minStart != m_minStart) { m_minStart = minStart; emit minStartChanged(); } }
    void setMinStop(int minStop) { if (minStop != m_minStop) { m_minStop = minStop; emit minStopChanged(); } }
    bool setPwmMode(int pwmMode, bool write = true);
    void setActive(bool active);
    void reset() Q_DECL_OVERRIDE;
    Q_INVOKABLE bool test();
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
    void testingChanged();
    void pwmModeChanged();


protected slots:

    void update() Q_DECL_OVERRIDE;
    void continueTest();
    void handleSetPwmResult(KJob *job);
    void handleSetPwmModeResult(KJob *job);
    void handleTestAuthReply(KJob *job);


private:

    int m_pwm;
    QTextStream *m_pwmStream;
    QTextStream *m_modeStream;
    Temp *m_temp;
    bool m_hasTemp;
    int m_minTemp;
    int m_maxTemp;
    int m_minPwm;
    int m_maxPwm;
    int m_minStart;
    int m_minStop;
    int m_pwmMode;
    int m_zeroRpm;

    enum
    {
        NotStarted,
        FindingStop1,
        FindingStop2,
        FindingStart,
        Finished,
        Cancelled,
        Error
    } m_testStatus;
};

}

#endif // PWMFAN_H
