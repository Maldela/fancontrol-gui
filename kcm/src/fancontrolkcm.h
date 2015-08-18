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

#ifndef FANCONTROLKCM_H
#define FANCONTROLKCM_H

#include <KQuickAddons/ConfigModule>

#include "../lib/src/guibase.h"


using namespace KQuickAddons;

class FancontrolKCM : public ConfigModule
{
    Q_OBJECT
    Q_PROPERTY(GUIBase *base READ base CONSTANT)
    Q_PROPERTY(bool manualControl READ manualControl WRITE setManualControl NOTIFY manualControlChanged)
    Q_PROPERTY(Loader* loader READ loader CONSTANT)
    Q_PROPERTY(SystemdCommunicator* systemdCom READ systemdCommunicator CONSTANT)
    Q_PROPERTY(qreal minTemp READ minTemp WRITE setMinTemp NOTIFY configChanged)
    Q_PROPERTY(qreal maxTemp READ maxTemp WRITE setMaxTemp NOTIFY configChanged)
    Q_PROPERTY(int unit READ unit WRITE setUnit NOTIFY unitChanged)
    Q_PROPERTY(QString serviceName READ serviceName WRITE setServiceName NOTIFY configChanged)
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY configChanged)
    
public:
    
    explicit FancontrolKCM(QObject *parent, const QVariantList &args = QVariantList());
    
    GUIBase *base() const { return m_base; }
    bool manualControl() const { return m_manualControl; }
    void setManualControl(bool manualControl);
    
    //wrap base
    Loader *loader() const { return m_base->loader(); }
    SystemdCommunicator *systemdCommunicator() const { return m_base->systemdCommunicator(); }
    qreal minTemp() const { return m_base->minTemp(); }
    qreal maxTemp() const { return m_base->maxTemp(); }
    int unit() const { return m_base->unit(); }
    QString serviceName() const { return m_base->serviceName(); }
    int interval() const { return m_base->interval(); }
    void setMinTemp(int temp) { m_base->setMinTemp(temp); }
    void setMaxTemp(int temp) { m_base->setMaxTemp(temp); }
    void setUnit(int unit) { m_base->setUnit(unit); }
    void setServiceName(const QString &name) { m_base->setServiceName(name); }
    void setInterval(int interval) { m_base->setInterval(interval); }
    
    
public slots:
    
    void load() Q_DECL_OVERRIDE;
    void save() Q_DECL_OVERRIDE;
    void defaults() Q_DECL_OVERRIDE;
    
    
signals:
    
    void manualControlChanged();
    void configChanged();
    void unitChanged();
    

protected:
    
    void emitConfigChanged() { emit configChanged(); }
    void emitUnitChanged() { emit unitChanged(); }
    
    GUIBase *const m_base;
    bool m_manualControl;
};

#endif // FANCONTROLKCM_H
