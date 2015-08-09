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
import QtQuick.Controls 1.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import org.kde.kcm 1.0

TabView {
    property real minTemp: 30.0
    property real maxTemp: 90.0
    property string unit: "Celsius"

    id: tabView
    anchors.fill: parent
    anchors.margins: 4
    frameVisible: true
    implicitHeight: 480
    implicitWidth: 640
    
    Tab {
        title: i18n("Sensors")
        SensorsTab {
            loader: kcm.loader
        }
    }
    Tab {
        title: i18n("PwmFans")
        PwmFansTab {
            size: sizeSlider.value
            minTemp: tabView.minTemp
            maxTemp: tabView.maxTemp
            unit: tabView.unit
            loader: kcm.loader
        }
    }
    Tab {
        title: i18n("Configfile")
        ConfigfileTab {
            loader: kcm.loader
        }
    }
    Tab {
        title: i18n("Settings")
        SettingsTab {
            id: settingsTab
            minTemp: tabView.minTemp
            maxTemp: tabView.maxTemp
            onMinTempChanged: tabView.minTemp = minTemp
            onMaxTempChanged: tabView.maxTemp = maxTemp
            onUnitChanged: tabView.unit = unit
            loader: kcm.loader
            systemdCom: kcm.systemdCom
        }
    }
    
    SystemPalette {
        id: palette
    }
}