/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2015  <copyright holder> <email>
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

#include "fancontrolkcm.h"

#include <QtQml>
#include <KAboutData>
#include <KLocalizedString>
#include <KPluginFactory>


K_PLUGIN_FACTORY_WITH_JSON(FancontrolKCMFactory, "kcm_fancontrol.json", registerPlugin<FancontrolKCM>();)

FancontrolKCM::FancontrolKCM(QObject *parent, const QVariantList& args)
    : ConfigModule(parent, args),
    m_base(new GUIBase(this))
{
    if (m_base->hasSystemdCommunicator())
        m_manualControl = m_base->systemdCommunicator()->serviceEnabled();
    else
        qFatal("Fancontrol-gui-lib was compiled without systemd support!");
    
    KAboutData *about = new KAboutData("kcm_fancontrol",
                                       i18n("Fancontrol-KCM"),
                                       "0.1",
                                       i18n("KDE Fancontrol Module"),
                                       KAboutLicense::KAboutLicense::GPL_V2,
                                       "Copyright (C) 2015 Malte Veerman",
                                       QString(),
                                       "http://github.com/maldela/fancontrol-gui",
                                       "http://github.com/maldela/fancontrol-gui/issues");
    about->addAuthor(i18n("Malte Veerman"), i18n("Main Developer"), "maldela@halloarsch.de"); 
    setAboutData(about);
    
    setButtons(Apply | Default);
    setAuthActionName("fancontrol.gui.helper.action");
    
    connect(m_base->loader(), &Loader::configFileChanged, [this] () { setNeedsSave(true); });
    
    qmlRegisterType<GUIBase>();
}

void FancontrolKCM::save()
{
    bool needsSave = false;
    needsSave = m_base->loader()->save() ? needsSave : true;
    
    if (m_base->systemdCommunicator()->serviceActive() && m_manualControl)
        needsSave = m_base->systemdCommunicator()->restartService() ? needsSave : true;
    else 
        needsSave = m_base->systemdCommunicator()->setServiceActive(m_manualControl) ? needsSave : true;

    needsSave = m_base->systemdCommunicator()->setServiceEnabled(m_manualControl) ? needsSave : true;
    setNeedsSave(needsSave);
}

void FancontrolKCM::load()
{
    setNeedsSave(!m_base->loader()->load(QUrl::fromLocalFile("/etc/fancontrol")));
}

void FancontrolKCM::defaults() 
{
    m_base->systemdCommunicator()->setServiceEnabled(false);
    m_base->systemdCommunicator()->setServiceActive(false);        
}

void FancontrolKCM::setManualControl(bool manualControl)
{
    if (m_manualControl != manualControl)
    {
        m_manualControl = manualControl;
        emit manualControlChanged();
        setNeedsSave(true);
    }
}



#include "fancontrolkcm.moc"