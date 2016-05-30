/*
 * Copyright (C) 2015  Malte Veerman <maldela@halloarsch.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */


#ifndef LOADER_H
#define LOADER_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QPair>


class QTimer;
class KJob;
class QProcess;

namespace Fancontrol
{

class Hwmon;
class PwmFan;
class Temp;
class GUIBase;

class Loader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl configUrl READ configUrl NOTIFY configUrlChanged)
    Q_PROPERTY(QString configFile READ configFile NOTIFY configFileChanged)
    Q_PROPERTY(QList<QObject *> hwmons READ hwmonsAsObjects NOTIFY hwmonsChanged)
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
    Q_PROPERTY(bool sensorsDetected READ sensorsDetected NOTIFY sensorsDetectedChanged)
    Q_PROPERTY(bool restartServiceAfterTesting READ restartServiceAfterTesting WRITE setRestartServiceAfterTesting NOTIFY restartServiceAfterTestingChanged)


public:

    explicit Loader(GUIBase *parent = Q_NULLPTR);

    Q_INVOKABLE void parseHwmons();
    Q_INVOKABLE bool load(const QUrl & = QUrl());
    Q_INVOKABLE bool save(const QUrl & = QUrl());
    Q_INVOKABLE void testFans();
    Q_INVOKABLE void abortTestingFans();
    Q_INVOKABLE void detectSensors();
    QUrl configUrl() const { return m_configUrl; }
    QString configFile() const { return m_configFile; }
    QList<Hwmon *> hwmons() const { return m_hwmons; }
    bool sensorsDetected() const { return m_sensorsDetected; }
    bool restartServiceAfterTesting() const { return m_reactivateAfterTesting; }
    void setRestartServiceAfterTesting(bool restart);
    QList<QObject *> hwmonsAsObjects() const;
    int interval() const { return m_interval; }
    void setInterval(int interval, bool writeNewConfig = true);
    QString error() const { return m_error; }


public slots:

    void updateSensors() { emit sensorsUpdateNeeded(); }
    void createConfigFile();
    void emitAllPwmFansChanged() { emit allPwmFansChanged(); }
    void emitAllTempsChanged() { emit allTempsChanged(); }
    void setError(const QString &error, bool critical = false);
    void handleDetectSensorsResult(KJob *job);
    void handleDetectSensorsResult(int exitCode);
    void handleTestStatusChanged();    


private:

    void parseConfigLine(const QString &line, void (PwmFan::*memberSetFunction)(int value));
    QPair<int, int> getEntryNumbers(const QString &entry);
    PwmFan *getPwmFan(const QPair<int, int> &indexPair) const;
    Temp *getTemp(const QPair<int, int> &indexPair) const;

    bool m_reactivateAfterTesting;
    int m_interval;
    QList<Hwmon *> m_hwmons;
    QUrl m_configUrl;
    QString m_configFile;
    QString m_error;
    QTimer *m_timer;
    bool m_sensorsDetected;


signals:

    void configUrlChanged();
    void configFileChanged();
    void hwmonsChanged();
    void intervalChanged();
    void errorChanged(QString, bool = false);
    void sensorsUpdateNeeded();
    void allPwmFansChanged();
    void allTempsChanged();
    void invalidConfigUrl();
    void sensorsDetectedChanged();
    void restartServiceAfterTestingChanged();
    void requestSetServiceActive(bool);
};

}

#endif // LOADER_H
