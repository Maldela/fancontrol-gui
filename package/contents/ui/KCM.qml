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


import QtQuick 2.4
import QtQuick.Controls 1.3
import org.kde.kcm 1.0

TabView {
    id: tabView
    frameVisible: true
    implicitHeight: 480
    implicitWidth: 640
    
    Tab {
        title: i18n("Sensors")
        SensorsTab {
            loader: kcm.base.loader
        }
    }
    Tab {
        title: i18n("PwmFans")
        PwmFansTab {
            size: 0.5
            baseObject: kcm.base
        }
    }
    Tab {
        title: i18n("Configfile")
        ConfigfileTab {
            loader: kcm.base.loader
        }
    }
    Tab {
        title: i18n("Settings")
        SettingsTab {
            baseObject: kcm.base
        }
    }
    
    SystemPalette {
        id: palette
    }
}