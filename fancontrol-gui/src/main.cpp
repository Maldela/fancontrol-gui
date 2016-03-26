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

#include <QtWidgets/QApplication>
#include <QtQml/QQmlContext>
#include <QtCore/QCommandLineParser>
#include <QtCore/QLoggingCategory>
#include <QtGui/QIcon>

#include <KDeclarative/QmlObject>
#include <KI18n/KLocalizedString>
#include <KCoreAddons/KAboutData>

#include "windowconfig.h"


Q_DECLARE_LOGGING_CATEGORY(FANCONTROL)
Q_LOGGING_CATEGORY(FANCONTROL, "fancontrol-gui")

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("fancontrol_gui")));

    KLocalizedString::setApplicationDomain("kcm_fancontrol");

    KAboutData about(QStringLiteral("fancontrol_gui"),
                     i18n("Fancontrol-GUI"),
                     QStringLiteral("0.4"),
                     i18n("Graphical user interface for fancontrol"),
                     KAboutLicense::KAboutLicense::GPL_V2,
                     QStringLiteral("Copyright (C) 2015 Malte Veerman"),
                     QString(),
                     QStringLiteral("http://github.com/maldela/fancontrol-gui"),
                     QStringLiteral("http://github.com/maldela/fancontrol-gui/issues"));
    about.addAuthor(i18n("Malte Veerman"), i18n("Main Developer"), QStringLiteral("maldela@halloarsch.de"));
    KAboutData::setApplicationData(about);
    QCommandLineParser parser;
    about.setupCommandLine(&parser);
    parser.process(app);
    about.processCommandLine(&parser);

    KDeclarative::QmlObject qmlObject;
    qmlObject.rootContext()->setContextProperty(QStringLiteral("windowConfig"), WindowConfig::instance());

    qmlObject.loadPackage("fancontrol-gui");
    
    return app.exec();
}
