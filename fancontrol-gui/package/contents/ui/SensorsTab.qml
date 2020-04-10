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


import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.3 as Kirigami
import Fancontrol.Qml 1.0 as Fancontrol


Kirigami.ScrollablePage {
    id: root

    readonly property QtObject loader: Fancontrol.Base.loader

    spacing: Kirigami.Units.smallSpacing

    ListView {
        id: listView

        width: root.width
        topMargin: spacing
        spacing: Kirigami.Units.largeSpacing * 2
        headerPositioning: ListView.OverlayHeader

        model: loader.hwmons.length

        delegate: Rectangle {
            readonly property QtObject hwmon: loader.hwmons[index]

            height: childrenRect.height
            width: listView.width - listView.spacing * 2
            x: listView.spacing
            color: Kirigami.Theme.backgroundColor

            Column {
                id: column

                width: parent.width
                padding: root.spacing

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: hwmon.name
                    font.pointSize: 12
                    horizontalAlignment: Text.horizontalCenter
                }

                Repeater {
                    model: hwmon.fans.length

                    RowLayout {
                        width: parent.width - parent.padding * 2

                        Label {
                            Layout.alignment: Qt.AlignLeft
                            text: i18n("Fan %1:", index+1)
                        }
                        Label {
                            id: rpmValue

                            Layout.alignment: Qt.AlignRight
                            text: i18n("%1 rpm", hwmon.fans[index].rpm)
                        }
                    }
                }
                Repeater {
                    model: hwmon.temps.length

                    RowLayout {
                        width: parent.width - parent.padding * 2

                        Label {
                            text: i18n("%1:", hwmon.temps[index].name)
                            Layout.alignment: Qt.AlignLeft
                        }
                        Label {
                            id: tempValue

                            text: hwmon.temps[index].value + i18n("°C")
                            Layout.alignment: Qt.AlignRight
                        }
                    }
                }
            }
        }
    }
}
