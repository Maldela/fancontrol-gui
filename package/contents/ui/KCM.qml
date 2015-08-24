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
import QtQuick.Layouts 1.1
import org.kde.kcm 1.0
import "../scripts/arrayfunctions.js" as ArrayFunctions

Item {
    implicitWidth: 800
    implicitHeight: 600
    
    ColumnLayout {
        anchors.fill: parent    
        
        Label {
            visible: kcm.loader.allPwmFans.length == 0
            text: i18n("There are no pwm capable fans in your system.")
            anchors.top: enabledBox.bottom
            anchors.margins: 20
        }
        
        Button {
            text: i18n("Detect fans")
            visible: kcm.loader.allPwmFans.length == 0
            onClicked: kcm.loader.detectSensors()
        }
        
        CheckBox {
            id: enabledBox
            visible: kcm.loader.allPwmFans.length > 0
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            text: i18n("Control fans manually")
            checked: kcm.manualControl
        }
        
        RowLayout {  
            visible: enabledBox.checked && kcm.loader.allPwmFans.length > 0
            
            Label {
                text: i18n("Fan:")
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                renderType: Text.NativeRendering
            }
            ComboBox {
                id: fanCombobox
                model: ArrayFunctions.namesWithPaths(kcm.loader.allPwmFans)
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }
        }
        
        Loader {
            Layout.fillWidth: true
            Layout.fillHeight: true
            active: enabledBox.checked && !!kcm.loader.allPwmFans[fanCombobox.currentIndex]
            sourceComponent: PwmFan {
                minimizable: false
                unit: kcm.base.unit
                fan: kcm.loader.allPwmFans[fanCombobox.currentIndex]
                loader: kcm.loader
                systemdCom: kcm.systemdCom
                minTemp: kcm.base.minTemp
                maxTemp: kcm.base.maxTemp
            }
        }
    }
}