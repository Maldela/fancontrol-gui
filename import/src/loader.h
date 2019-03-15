/*
 * Copyright (C) 2015  Malte Veerman <malte.veerman@gmail.com>
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


class QFileSystemWatcher;
class QTimer;

namespace Fancontrol
{

class Hwmon;
class PwmFan;
class Temp;
class Fan;
class GUIBase;

class Loader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl configUrl READ configUrl NOTIFY configUrlChanged)
    Q_PROPERTY(QString configPath READ configPath NOTIFY configUrlChanged)
    Q_PROPERTY(QString config READ config NOTIFY configChanged)
    Q_PROPERTY(QList<QObject *> hwmons READ hwmonsAsObjects NOTIFY hwmonsChanged)
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
    Q_PROPERTY(bool sensorsDetected READ sensorsDetected NOTIFY sensorsDetectedChanged)
    Q_PROPERTY(bool restartServiceAfterTesting READ restartServiceAfterTesting WRITE setRestartServiceAfterTesting NOTIFY restartServiceAfterTestingChanged)
    Q_PROPERTY(bool needsSave READ needsSave NOTIFY needsSaveChanged)


public:

    explicit Loader(GUIBase *parent = Q_NULLPTR);

    Q_INVOKABLE void parseHwmons(QString path = QString());
    Q_INVOKABLE bool load(const QUrl & = QUrl());
    Q_INVOKABLE bool save(const QUrl & = QUrl());
    Q_INVOKABLE void reset();
    Q_INVOKABLE void testFans();
    Q_INVOKABLE void abortTestingFans();

    bool load(const QString &config);
    QUrl configUrl() const { return m_configUrl; }
    QString configPath() const { return m_configUrl.path(); }
    QString config() const { return m_config; }
    QList<Hwmon *> hwmons() const { return m_hwmons; }
    bool sensorsDetected() const { return m_sensorsDetected; }
    bool restartServiceAfterTesting() const { return m_reactivateAfterTesting; }
    void setRestartServiceAfterTesting(bool restart);
    QList<QObject *> hwmonsAsObjects() const;
    int interval() const { return m_interval; }
    void setInterval(int interval, bool writeNewConfig = true);
    PwmFan *pwmFan(QPair<int, int> indexPair) const { return pwmFan(indexPair.first, indexPair.second); }
    Temp *temp(QPair<int, int> indexPair) const { return temp(indexPair.first, indexPair.second); }
    Fan *fan(QPair<int, int> indexPair) const { return fan(indexPair.first, indexPair.second); }
    PwmFan *pwmFan(int hwmonIndex, int pwmFanIndex) const;
    Temp *temp(int hwmonIndex, int tempIndex) const;
    Fan *fan(int hwmonIndex, int fanIndex) const;
    void toDefault();
    bool needsSave() const { return m_config != m_loadedConfig; }
    void updateConfig();
    void handleTestStatusChanged();


protected:

    bool parseConfig(QString config);
    void parseConfigLine(const QString &line, void (PwmFan::*memberSetFunction)(int value));
    QPair<int, int> getEntryNumbers(const QString &entry);
    QString createConfig() const;
    bool watchPath(const QString &path);

    QList<Hwmon *> m_hwmons;


private:

    bool m_reactivateAfterTesting;
    int m_interval;
    QUrl m_configUrl;
    QString m_config;
    QString m_loadedConfig;
    QTimer *m_timer;
    QFileSystemWatcher *m_watcher;
    bool m_sensorsDetected;


signals:

    void configUrlChanged();
    void configChanged();
    void configFileChanged();
    void hwmonsChanged();
    void intervalChanged();
    void error(QString, bool = false);
    void info(QString);
    void sensorsUpdateNeeded();
    void invalidConfigUrl();
    void sensorsDetectedChanged();
    void restartServiceAfterTestingChanged();
    void requestSetServiceActive(bool);
    void needsSaveChanged();
};

}

#endif // LOADER_H
