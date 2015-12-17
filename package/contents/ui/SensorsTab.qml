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
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import "../scripts/arrayfunctions.js" as ArrayFunctions


RowLayout {
    property QtObject loader

    id: root
    anchors.fill: parent
    anchors.margins: 2
    anchors.topMargin: 10

    Repeater {
        model: !!loader ? loader.hwmons.length : 0

        Rectangle {
            property QtObject hwmon: loader.hwmons[index]
            property int padding: 10

            Layout.preferredWidth: root.width / loader.hwmons.length - root.spacing
            Layout.maximumWidth: 500
            Layout.fillHeight: true
            color: palette.light
            border.width: 1
            border.color: "black"
            radius: 5

            Column {
                id: column
                anchors.fill: parent

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: hwmon.name
                    font.pointSize: 12
                    horizontalAlignment: Text.horizontalCenter
                }

                Repeater {
                    model: hwmon.fans.length

                    RowLayout {
                        width: parent.width

                        Label {
                            anchors.left: parent.left
                            anchors.leftMargin: padding
                            Layout.maximumWidth: parent.width - rpmValue.width - padding*2
                            clip: true
                            text: "Fan " + (index+1) + " RPM : "
                        }
                        Label {
                            id: rpmValue
                            anchors.right: parent.right
                            anchors.rightMargin: padding
                            text: hwmon.fans[index].rpm
                        }
                    }
                }
                Repeater {
                    model: hwmon.temps.length

                    RowLayout {
                        width: parent.width

                        Label {
                            anchors.left: parent.left
                            anchors.leftMargin: padding
                            text: hwmon.temps[index].name + ": "
                            Layout.maximumWidth: parent.width - tempValue.width - padding*2
                            clip: true
                        }
                        Label {
                            id: tempValue
                            anchors.right: parent.right
                            anchors.rightMargin: padding
                            text: hwmon.temps[index].value
                        }
                    }
                }
            }
        }
    }

    SystemPalette {
        id: palette
    }
}
