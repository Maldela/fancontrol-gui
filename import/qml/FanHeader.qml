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


Item {
    property QtObject fan

    implicitHeight: layout.childrenRect.height

    RowLayout {
        id: layout

        anchors.fill: parent
        anchors.leftMargin: Kirigami.Units.smallSpacing
        anchors.rightMargin: Kirigami.Units.smallSpacing

        TextEdit {
            id: nameField

            Layout.alignment: Qt.AlignLeft
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
        Label {
            Layout.alignment: Qt.AlignRight
            text: !!fan ? fan.path : ""
            horizontalAlignment: Text.AlignRight
        }
    }
}
