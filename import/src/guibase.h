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


#ifndef GUIBASE_H
#define GUIBASE_H

#include <QtCore/QObject>
#include <QtCore/QStringListModel>
#include <QtCore/QUrl>

#include "loader.h"
#include "pwmfanmodel.h"
#include "tempmodel.h"

#ifndef NO_SYSTEMD
#include "systemdcommunicator.h"
#endif


namespace Fancontrol
{

class Config;

class GUIBase : public QObject
{
    Q_OBJECT

#ifndef NO_SYSTEMD
    Q_PROPERTY(SystemdCommunicator* systemdCom READ systemdCommunicator CONSTANT)
#endif

    Q_PROPERTY(PwmFanModel *pwmFanModel READ pwmFanModel CONSTANT)
    Q_PROPERTY(TempModel *tempModel READ tempModel CONSTANT)
    Q_PROPERTY(QStringListModel *profileModel READ profileModel CONSTANT)
    Q_PROPERTY(Loader* loader READ loader CONSTANT)
    Q_PROPERTY(qreal minTemp READ minTemp WRITE setMinTemp NOTIFY minTempChanged)
    Q_PROPERTY(qreal maxTemp READ maxTemp WRITE setMaxTemp NOTIFY maxTempChanged)
    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)
    Q_PROPERTY(QString serviceName READ serviceName WRITE setServiceName NOTIFY serviceNameChanged)
    Q_PROPERTY(QUrl configUrl READ configUrl WRITE setConfigUrl NOTIFY configUrlChanged)
    Q_PROPERTY(bool configValid READ configValid NOTIFY configUrlChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
    Q_PROPERTY(bool needsApply READ needsApply NOTIFY needsApplyChanged)
    Q_PROPERTY(bool showTray READ showTray WRITE setShowTray NOTIFY showTrayChanged)
    Q_PROPERTY(bool startMinimized READ startMinimized WRITE setStartMinimized NOTIFY startMinimizedChanged)

public:

    explicit GUIBase(QObject *parent = Q_NULLPTR);

    Loader *loader() const { return m_loader; }

#ifndef NO_SYSTEMD
    SystemdCommunicator *systemdCommunicator() const { return m_com; }
#endif

    qreal minTemp() const;
    qreal maxTemp() const;
    QString serviceName() const;
    QUrl configUrl() const;
    bool configValid() const { return m_configValid; }
    QString unit() const { return m_unit; }
    QString error() const { return m_error; }
    void setMinTemp(qreal minTemp);
    void setMaxTemp(qreal maxTemp);
    void setServiceName(const QString &name);
    void setConfigUrl(const QUrl &url);
    void setUnit(const QString &unit) { if (unit != m_unit) { m_unit = unit; emit unitChanged(m_unit); } }
    bool needsApply() const;
    bool showTray() const;
    void setShowTray(bool show);
    bool startMinimized() const;
    void setStartMinimized(bool sm);
    PwmFanModel *pwmFanModel() const { return m_pwmFanModel; }
    TempModel *tempModel() const { return m_tempModel; }
    QStringListModel *profileModel() const { return m_profileModel; }

    Q_INVOKABLE bool hasSystemdCommunicator() const;
    Q_INVOKABLE void apply();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void applyProfile(const QString &profile);
    Q_INVOKABLE void applyProfile(int);
    Q_INVOKABLE void saveProfile(const QString &profile, bool updateModel = true);
    Q_INVOKABLE void deleteProfile(const QString &profile, bool updateModel = true);
    Q_INVOKABLE void deleteProfile(int, bool updateModel = true);

public slots:

    void load();
    void handleError(const QString &error, bool critical = false);
    void handleInfo(const QString &info);

signals:

    void minTempChanged();
    void maxTempChanged();
    void serviceNameChanged();
    void configUrlChanged();
    void unitChanged(QString);
    void errorChanged();
    void criticalError();
    void needsApplyChanged();
    void showTrayChanged();
    void startMinimizedChanged();
    void profileChanged(int profile);

private:

    QString m_error;
    Config *const m_config;

#ifndef NO_SYSTEMD
    SystemdCommunicator *const m_com;
#endif

    Loader *const m_loader;
    QString m_unit;
    bool m_configValid;
    bool m_configChanged;
    PwmFanModel *m_pwmFanModel;
    TempModel *m_tempModel;
    QStringListModel *m_profileModel;
};

}

#endif // GUIBASE_H
