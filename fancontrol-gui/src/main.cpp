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

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <KDeclarative/KDeclarative>
#include <KI18n/KLocalizedString>
#include <KPackage/PackageLoader>
#include <KAboutData>

#include "gui.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    KLocalizedString::setApplicationDomain("kcm_fancontrol");
    
    KAboutData about("fancontrol_gui",
                    i18n("Fancontrol-GUI"),
                    "0.1",
                    i18n("Graphical user interface for fancontrol"),
                    KAboutLicense::KAboutLicense::GPL_V2,
                    "Copyright (C) 2015 Malte Veerman",
                    QString(),
                    "http://github.com/maldela/fancontrol-gui",
                    "http://github.com/maldela/fancontrol-gui/issues");
    about.addAuthor(i18n("Malte Veerman"), i18n("Main Developer"), "maldela@halloarsch.de"); 
    KAboutData::setApplicationData(about);

    QQmlApplicationEngine engine;
    
    KDeclarative::KDeclarative decl;
    decl.setDeclarativeEngine(&engine);
    decl.setupBindings();
        
    GUI gui;
    engine.rootContext()->setContextProperty("gui", &gui);
    
    KPackage::Package package = KPackage::PackageLoader::self()->loadPackage("KPackage/GenericQML");
    package.setDefaultPackageRoot("kpackage/kcms");
    package.setPath("kcm_fancontrol");

    engine.load(QUrl::fromLocalFile(package.path() + "/contents/ui/Application.qml"));

    return app.exec();
}
