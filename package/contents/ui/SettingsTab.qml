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
import QtQuick.Controls 1.2
import QtQml 2.2
import "../scripts/arrayfunctions.js" as ArrayFunctions
import "../scripts/units.js" as Units


Item {
    property QtObject gui
    property QtObject systemdCom: gui && gui.hasSystemdCommunicator() ? gui.systemdCom : null
    property QtObject loader : gui ? gui.loader : null
    property int padding: 10
    property real textWidth: 0
    property var locale: Qt.locale()

    id: root
    anchors.fill: parent
    anchors.topMargin: 5

    Column {
        id: column
        anchors.fill: parent
        anchors.margins: padding
        spacing: 5

        RowLayout {
            width: parent.width

            Label {
                Layout.preferredWidth: root.textWidth
                clip: true
                text: i18n("Interval:")
                horizontalAlignment: Text.AlignRight
                Component.onCompleted: root.textWidth = Math.max(root.textWidth, contentWidth)
            }
            SpinBox {
                Layout.minimumWidth: implicitWidth
                Layout.fillWidth: true
                value: gui.loader ? gui.loader.interval : 1
                suffix: " " + (value > 1 ? i18n("seconds") : i18n("second"))
                minimumValue: 1.0
                onValueChanged: gui.loader.interval = value
            }
        }
        RowLayout {
            width: parent.width

            Label {
                Layout.preferredWidth: root.textWidth
                clip: true
                text: i18n("Minimum temperature for fan graphs:")
                horizontalAlignment: Text.AlignRight
                Component.onCompleted: root.textWidth = Math.max(root.textWidth, contentWidth)
            }
            SpinBox {
                id: minTempBox
                Layout.minimumWidth: implicitWidth
                Layout.fillWidth: true
                decimals: 2
                maximumValue: maxTempBox.value
                minimumValue: Units.fromKelvin(0, gui.unit)
                value: Units.fromCelsius(gui.minTemp, gui.unit)
                suffix: gui.unit == 0 ? i18n("°C") : gui.unit == 1 ? i18n("K") : i18n("°F")
                onValueChanged: gui.minTemp = value
            }
        }
        RowLayout {
            width: parent.width

            Label {
                Layout.preferredWidth: root.textWidth
                clip: true
                text: i18n("Maximum temperature for fan graphs:")
                horizontalAlignment: Text.AlignRight
                Component.onCompleted: root.textWidth = Math.max(root.textWidth, contentWidth)
            }
            SpinBox {
                id: maxTempBox
                Layout.minimumWidth: implicitWidth
                Layout.fillWidth: true
                decimals: 2
                maximumValue: Number.POSITIVE_INFINITY
                minimumValue: minTempBox.value
                value: Units.fromCelsius(gui.maxTemp, gui.unit)
                suffix: gui.unit == 0 ? i18n("°C") : gui.unit == 1 ? i18n("K") : i18n("°F")
                onValueChanged: gui.maxTemp = value
            }
        }
        Loader {
            active: systemdCom
            sourceComponent: RowLayout {
                width: column.width

                Label {
                    Layout.preferredWidth: root.textWidth
                    clip: true
                    text: i18n("Name of the fancontrol systemd service:")
                    horizontalAlignment: Text.AlignRight
                    Component.onCompleted: root.textWidth = Math.max(root.textWidth, contentWidth)
                }
                OptionInput {
                    Layout.minimumWidth: implicitWidth
                    Layout.fillWidth: true
                    color: systemdCom.serviceExists ? "green" : "red"
                    value: gui.serviceName
                    onTextChanged: gui.serviceName = text
                }
            }
        }
        Loader {
            active: systemdCom
            sourceComponent: RowLayout {
                width: column.width

                Label {
                    Layout.preferredWidth: root.textWidth
                    clip: true
                    text: i18n("Fancontrol systemd service autostart:")
                    horizontalAlignment: Text.AlignRight
                    Component.onCompleted: root.textWidth = Math.max(root.textWidth, contentWidth)
                }
                CheckBox {
                    id: autostartBox
                    Layout.minimumWidth: implicitWidth
                    Layout.fillWidth: true
                    checked: systemdCom.serviceEnabled
                    onCheckedChanged: systemdCom.serviceEnabled = checked
                }
            }
        }
    }
}
