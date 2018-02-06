/*
 * <one line to give the library's name and an idea of what it does.>
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

#include "fancontrolkcm.h"

#include <KCoreAddons/KAboutData>
#include <KCoreAddons/KPluginFactory>
#include <KI18n/KLocalizedString>


K_PLUGIN_FACTORY_WITH_JSON(FancontrolKCMFactory, "fancontrol-kcm.json", registerPlugin<FancontrolKCM>();)


FancontrolKCM::FancontrolKCM(QObject *parent, const QVariantList& args)
    : ConfigModule(parent, args)
{
    auto about = new KAboutData(QStringLiteral("org.kde.fancontrol.kcm"),
                                i18n("Fancontrol-KCM"),
                                QStringLiteral("0.3"),
                                i18n("KDE Fancontrol Module"),
                                KAboutLicense::KAboutLicense::GPL_V2,
                                QStringLiteral("Copyright (C) 2015 Malte Veerman"),
                                QString(),
                                QStringLiteral("http://github.com/maldela/fancontrol-gui"),
                                QStringLiteral("http://github.com/maldela/fancontrol-gui/issues"));
    about->addAuthor(i18n("Malte Veerman"), i18n("Main Developer"), QStringLiteral("malte.veerman@gmail.com"));
    setAboutData(about);

    setButtons(Apply | Default);
    setAuthActionName(QStringLiteral("fancontrol.gui.helper.action"));
}

void FancontrolKCM::save()
{
    emit aboutToSave();

    setNeedsSave(false);
}

void FancontrolKCM::load()
{
    emit aboutToLoad();

    setNeedsSave(false);
}

void FancontrolKCM::defaults()
{
    emit aboutToDefault();

    setNeedsSave(true);
}


#include "fancontrolkcm.moc"
