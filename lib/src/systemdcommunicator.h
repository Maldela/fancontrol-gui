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


#ifndef SYSTEMDCOMMUNICATOR_H
#define SYSTEMDCOMMUNICATOR_H

#include <QtCore/QObject>

#include "fancontrol_gui_lib_export.h"


class QDBusInterface;

class FANCONTROL_GUI_LIB_EXPORT SystemdCommunicator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
    Q_PROPERTY(bool serviceExists READ serviceExists NOTIFY serviceNameChanged)
    Q_PROPERTY(bool serviceEnabled READ serviceEnabled WRITE setServiceEnabled NOTIFY serviceEnabledChanged)
    Q_PROPERTY(bool serviceActive READ serviceActive WRITE setServiceActive NOTIFY serviceActiveChanged)

public:

    explicit SystemdCommunicator(const QString &serviceName = QString(), QObject *parent = Q_NULLPTR);

    QString serviceName() const { return m_serviceName; }
    void setServiceName(const QString &name);
    bool serviceExists();
    bool serviceEnabled();
    bool serviceActive();
    bool setServiceEnabled(bool enabled);
    bool setServiceActive(bool active);
    QString error() const { return m_error; }
    Q_INVOKABLE bool restartService();


signals:

    void serviceNameChanged();
    void serviceEnabledChanged();
    void serviceActiveChanged();
    void errorChanged();


protected slots:
    
    void updateServiceProperties(QString, QVariantMap, QStringList);
    
    
protected:
    
    bool dbusAction(const QString &method, const QVariantList &arguments = QVariantList());
    void setError(const QString &error) { if (error != m_error) { m_error = error; emit errorChanged(); } }
    void success() { setError("Success"); }
    
    
private:

    QString m_serviceName;
    QString m_serviceObjectPath;
    QString m_error;
    QDBusInterface * const m_managerInterface;
    QDBusInterface *m_serviceInterface;
};

#endif // SYSTEMDCOMMUNICATOR_H
