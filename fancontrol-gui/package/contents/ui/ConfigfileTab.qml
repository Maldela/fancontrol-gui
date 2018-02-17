/*
 * Copyright (C) 2015  Malte Veerman <malte.veerman@gmail.com>
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
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import Fancontrol.Qml 1.0 as Fancontrol


ColumnLayout {
    property QtObject loader: Fancontrol.Base.loader

    anchors.fill: parent
    anchors.margins: 10

    Label {
        anchors.top: parent.top
        text: !!loader && loader.configEqualToLoadedFile ? loader.configPath : i18n("New config")
    }

    Rectangle {
        Layout.fillHeight: true
        Layout.fillWidth: true
        color: palette.light
        border.width: 1
        radius: 5

        ScrollView {
            id: scrollView
            anchors.fill: parent
            anchors.margins: 5

            TextEdit {
                text: !!loader ? loader.config : ""
                readOnly: true
                color: palette.text
            }
        }
    }

    SystemPalette {
        id: palette
    }
}
