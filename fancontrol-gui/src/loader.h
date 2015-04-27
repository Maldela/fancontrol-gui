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

#include <QObject>
#include <QStringList>
#include <QUrl>
#include <QTimer>

#include "hwmon.h"


class Loader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl configUrl READ configUrl WRITE setConfigUrl NOTIFY configUrlChanged)
    Q_PROPERTY(QString configFile READ configFile NOTIFY configFileChanged)
    Q_PROPERTY(QList<QObject *> hwmons READ hwmons NOTIFY hwmonsChanged)
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
    

public:

    explicit Loader(QObject *parent = 0);

    Q_INVOKABLE void parseHwmons();
    Q_INVOKABLE void open(const QUrl & = QUrl());
    Q_INVOKABLE void save(const QUrl & = QUrl());
    QUrl configUrl() const { return m_configUrl; }
    void setConfigUrl(const QUrl &url) { open(url); if (m_configUrl != url) { m_configUrl = url; emit configUrlChanged(); } }
    QString configFile() const { return m_configFile; }
    QList<QObject *> hwmons() const;
    int interval() { return m_interval; }
    void setInterval(int interval) { if (interval != m_interval) { m_interval = interval; emit intervalChanged(m_interval*1000); createConfigFile(); } }
    Hwmon * hwmon(int i) { return m_hwmons.value(i, nullptr); }
    QString error() const { return m_error; }
    
    static int getHwmonNumber(const QString &str) { return str.split('/').value(0).remove("hwmon").toInt(); }
    static int getSensorNumber(const QString &str) { return str.split('/').value(1).remove(QRegExp("pwm|fan|temp|_input")).toInt() - 1; }

public slots:

    void updateSensors() { emit sensorsUpdateNeeded(); }


protected slots:

    void createConfigFile();


protected:

    bool m_parsed;
    int m_interval;
    QList<Hwmon *> m_hwmons;
    QUrl m_configUrl;
    QString m_configFile;
    QString m_error;
    QTimer m_timer;


signals:

    void configUrlChanged();
    void configFileChanged();
    void hwmonsChanged();
    void intervalChanged(int);
    void errorChanged();
    void sensorsUpdateNeeded();
};

#endif // LOADER_H
