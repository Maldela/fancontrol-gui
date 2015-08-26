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
import QtQuick.Layouts 1.2
import "../scripts/arrayfunctions.js" as ArrayFunctions

ColumnLayout {
    property QtObject baseObject
    property QtObject loader: baseObject ? baseObject.loader : null
    property QtObject systemdCom: baseObject && baseObject.hasSystemdCommunicator() ? baseObject.systemdCom : null

    id: root
    anchors.fill: parent
    anchors.topMargin: 5

    RowLayout {
        width: parent.width
        visible: loader && loader.allPwmFans.length > 0
        
        Label {
            text: i18n("Fan:")
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            renderType: Text.NativeRendering
        }
        ComboBox {
            id: fanCombobox
            model: ArrayFunctions.namesWithPaths(loader.allPwmFans)
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }
        Button {
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            text: i18n("Detect fans")
            iconName: "dialog-password"
            onClicked: baseObject.loader.detectSensors()
        }
    }
    
    Loader {
        Layout.fillHeight: true
        Layout.fillWidth: true
        active: !!baseObject.loader.allPwmFans[fanCombobox.currentIndex]
        sourceComponent: PwmFan {
            minimizable: false
            unit: baseObject.unit
            fan: loader.allPwmFans[fanCombobox.currentIndex]
            loader: root.loader
            systemdCom: baseObject.systemdCom
            minTemp: baseObject.minTemp
            maxTemp: baseObject.maxTemp
        }
    }
    
    ColumnLayout {
        id: noFansInfo
        anchors.centerIn: parent
        spacing: 20
        visible: loader.allPwmFans.length == 0
        
        Label {
            Layout.alignment: Qt.AlignCenter
            text: i18n("There are no pwm capable fans in your system.")
            font.pointSize: 14
            font.bold: true
        }
        
        Button {
            Layout.alignment: Qt.AlignCenter
            text: i18n("Detect fans")
            iconName: "dialog-password"
            onClicked: loader.detectSensors()
        }
    }
}
