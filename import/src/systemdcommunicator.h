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


#ifndef SYSTEMDCOMMUNICATOR_H
#define SYSTEMDCOMMUNICATOR_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariantMap>
#include <QtCore/QVariantList>


class QDBusInterface;
class KJob;

namespace Fancontrol
{

class GUIBase;

class SystemdCommunicator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool serviceExists READ serviceExists NOTIFY serviceNameChanged)
    Q_PROPERTY(bool serviceEnabled READ serviceEnabled WRITE setServiceEnabled NOTIFY serviceEnabledChanged)
    Q_PROPERTY(bool serviceActive READ serviceActive WRITE setServiceActive NOTIFY serviceActiveChanged)
    Q_PROPERTY(bool needsApply READ needsApply NOTIFY needsApplyChanged)

public:

    explicit SystemdCommunicator(GUIBase *parent = Q_NULLPTR, const QString &serviceName = QString());
    virtual ~SystemdCommunicator();

    QString serviceName() const { return m_serviceName; }
    void setServiceName(const QString &name);
    bool serviceExists() const;
    bool serviceEnabled() const { return m_serviceEnabled; }
    bool serviceActive() const { return m_serviceActive; }
    void setServiceEnabled(bool enabled);
    void setServiceActive(bool active);
    bool needsApply() const;
    Q_INVOKABLE bool restartService();
    Q_INVOKABLE void apply(bool serviceRestart = false);
    Q_INVOKABLE void reset();


signals:

    void serviceNameChanged();
    void serviceEnabledChanged();
    void serviceActiveChanged();
    void needsApplyChanged();
    void error(QString, bool = false) const;
    void info(QString) const;


protected slots:

    void updateServiceProperties(QString, QVariantMap, QStringList);


protected:

    bool dbusAction(const QString &method, const QVariantList &arguments = QVariantList());
    bool systemdServiceActive() const;
    bool systemdServiceEnabled() const;


private:

    QString m_serviceName;
    QString m_serviceObjectPath;
    QDBusInterface * const m_managerInterface;
    QDBusInterface *m_serviceInterface;
    bool m_serviceEnabled;
    bool m_serviceActive;
};

}

#endif // SYSTEMDCOMMUNICATOR_H
