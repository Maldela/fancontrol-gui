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
import QtQuick.Layouts 1.1

FocusScope {
    property alias text: textField.text
    property alias font: textField.font
    property alias inputMethodHints: textField.inputMethodHints
    property alias validator: textField.validator
    property alias color: textField.color
    property real margin: 6
    property var value
    property string type: "int"
    property var locale: Qt.locale()

    id: root
    implicitHeight: textField.implicitHeight + margin*2
    implicitWidth: textField.implicitWidth + margin*2
    
    onValueChanged: {
        if (type == "int" || type == "double") {
            if (textField.text != Number(value).toLocaleString()) {
                textField.text = Number(value).toLocaleString();
            }
        }
        else if (type == "string") {
            if (textField.text != value) {
                textField.text = value;
            }
        }
    }

    Rectangle {
        id: rect
        anchors.fill: parent
        border.width: 1
        radius: 2
        border.color: enabled ? palette.text : disabledPalette.text

        TextInput {
            id: textField
            anchors.fill: parent
            anchors.leftMargin: margin
            horizontalAlignment: TextEdit.AlignLeft
            verticalAlignment: TextEdit.AlignVCenter
            renderType: Text.NativeRendering
            selectByMouse: true
            color: enabled ? palette.text : disabledPalette.text

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.IBeamCursor
                acceptedButtons: Qt.NoButton
            }
        }

        SystemPalette {
            id: palette
        }
        SystemPalette {
            id: disabledPalette
            colorGroup: SystemPalette.Disabled
        }
    }
}
