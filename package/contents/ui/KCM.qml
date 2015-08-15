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
        text: i18n("Control fans manually")
        checked: kcm.manualControl
        onCheckedChanged: kcm.manualControl = checked
    }
    
    RowLayout {
        enabled: enabledBox.checked
        
        Label {
            id: fanLabel
            text: i18n("Fan:")
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            renderType: Text.NativeRendering
        }
        ComboBox {
            id: fanCombobox
            model: ArrayFunctions.namesWithPaths(kcm.base.loader.allPwmFans)
            Layout.fillWidth: true
            Layout.maximumWidth: root.width - fanLabel.width - parent.spacing
        }
    }
    
    PwmFan {
        id: fan
        enabled: enabledBox.checked
        minimizable: false
        unit: kcm.base.unit
        fan: kcm.base.loader.allPwmFans[fanCombobox.currentIndex]
        loader: kcm.base.loader
        systemdCom: kcm.base.systemdCom
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
}