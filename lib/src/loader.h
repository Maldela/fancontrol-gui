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
#include <QUrl>
#include <QTimer>

#include "hwmon.h"
#include "fancontrol_gui_lib_export.h"

class Hwmon;

class FANCONTROL_GUI_LIB_EXPORT Loader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl configUrl READ configUrl NOTIFY configUrlChanged)
    Q_PROPERTY(QString configFile READ configFile NOTIFY configFileChanged)
    Q_PROPERTY(QList<QObject *> hwmons READ hwmons NOTIFY hwmonsChanged)
    Q_PROPERTY(QList<QObject *> allPwmFans READ allPwmFans NOTIFY allPwmFansChanged)
    Q_PROPERTY(QList<QObject *> allTemps READ allTemps NOTIFY allTempsChanged)
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
    

public:

    explicit Loader(QObject *parent = Q_NULLPTR);
    
    Q_INVOKABLE void parseHwmons();
    Q_INVOKABLE bool load(const QUrl & = QUrl());
    Q_INVOKABLE bool save(const QUrl & = QUrl());
    Q_INVOKABLE void testFans();
    QUrl configUrl() const { return m_configUrl; }
    QString configFile() const { return m_configFile; }
    QList<QObject *> hwmons() const;
    QList<QObject *> allPwmFans() const;
    QList<QObject *> allTemps() const;
    int interval() const { return m_interval; }
    void setInterval(int interval, bool writeNewConfig = true);
    Hwmon * hwmon(int i) { return m_hwmons.value(i, Q_NULLPTR); }
    QString error() const { return m_error; }
    
    static int getHwmonNumber(const QString &str); 
    static int getSensorNumber(const QString &str);

    
public slots:

    void updateSensors() { emit sensorsUpdateNeeded(); }


protected slots:

    void createConfigFile();
    void emitAllPwmFansChanged() { emit allPwmFansChanged(); }
    void emitAllTempsChanged() { emit allTempsChanged(); }


protected:
    
    void setError(const QString &error);
    void success() { setError("Success"); }
    
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
    void intervalChanged();
    void errorChanged();
    void sensorsUpdateNeeded();
    void allPwmFansChanged();
    void allTempsChanged();
};

#endif // LOADER_H
