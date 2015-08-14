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

ColumnLayout {
    id: root
    implicitWidth: 1024
    implicitHeight: 768
    
    CheckBox {
        id: enabledBox
        anchors.left: parent.left
        anchors.right: parent.right
        text: i18n("Control fans manually")
        checked: kcm.base.systemdCom.serviceEnabled
        onCheckedChanged: kcm.base.systemdCom.serviceEnabled = checked
    }
    
    RowLayout {
        enabled: enabledBox.checked
        
        Label {
            text: i18n("Fan:")
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            renderType: Text.NativeRendering
        }
        ComboBox {
            id: fanCombobox
            model: ArrayFunctions.names(kcm.base.loader.allPwmFans)
            Layout.fillWidth: true
            Layout.maximumWidth: 300
        }
    }
    
    PwmFan {
        id: fan
        enabled: enabledBox.checked
        minimizable: false
        fan: kcm.base.loader.allPwmFans[fanCombobox.currentIndex]
        loader: kcm.base.loader
        systemdCom: kcm.base.systemdCom
        anchors.left: parent.left
        anchors.right: parent.right
        Layout.fillHeight: true
    }
}