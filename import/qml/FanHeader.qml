/*
 * Copyright (C) 2019  Malte Veerman <malte.veerman@gmail.com>
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


import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.3 as Kirigami


RowLayout {
    property QtObject fan
    property bool editable: true

    Loader {
        active: !!fan
        sourceComponent: editable ? editableNameComponent : nameComponent
        Layout.alignment: Qt.AlignLeft
        Layout.leftMargin: Kirigami.Units.smallSpacing
    }
    Item {
        Layout.fillWidth: true
    }
    Label {
        Layout.alignment: Qt.AlignRight
        Layout.rightMargin: Kirigami.Units.smallSpacing
        text: !!fan ? fan.path : ""
        horizontalAlignment: Text.AlignRight
    }

    Component {
        id: nameComponent

        Label {
            text: !!fan ? fan.name : ""
            horizontalAlignment: TextEdit.AlignLeft
            wrapMode: TextEdit.Wrap
            font.bold: true
            font.pointSize: Kirigami.Theme.defaultFont.pointSize + 2
        }
    }
    Component {
        id: editableNameComponent

        TextEdit {
            id: nameField

            text: !!fan ? fan.name : ""
            color: Kirigami.Theme.textColor
            horizontalAlignment: TextEdit.AlignLeft
            wrapMode: TextEdit.Wrap
            font.bold: true
            font.pointSize: Kirigami.Theme.defaultFont.pointSize + 2
            selectByMouse: true

            onTextChanged: if (!!fan && fan.name != text) fan.name = text

            Connections {
                target: !!fan ? fan : null
                onNameChanged: if (fan.name != nameField.text) nameField.text = fan.name
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.IBeamCursor
                acceptedButtons: Qt.NoButton
            }
        }
    }
}
