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
import Fancontrol.Qml 1.0 as Fancontrol


Item {
    property QtObject systemdCom: Fancontrol.base.hasSystemdCommunicator() ? Fancontrol.base.systemdCom : null
    property QtObject loader : Fancontrol.base.loader
    property int padding: 10
    property real textWidth: 0
    property var locale: Qt.locale()

    id: root
    anchors.fill: parent
    anchors.margins: 10

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
                value: loader.interval
                suffix: " " + i18np("second", "seconds", loader.interval)
                minimumValue: 1.0
                onValueChanged: loader.interval = value
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
                maximumValue: Number.POSITIVE_INFINITY
                minimumValue: Fancontrol.Units.fromKelvin(0, Fancontrol.base.unit)
                value: Fancontrol.Units.fromCelsius(Fancontrol.base.minTemp, Fancontrol.base.unit)
                suffix: Fancontrol.base.unit
                onValueChanged: {
                    Fancontrol.base.minTemp = value;
                    if (value > maxTempBox.value) maxTempBox.value = value;
                }
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
                minimumValue: Fancontrol.Units.fromKelvin(0, Fancontrol.base.unit)
                value: Fancontrol.Units.fromCelsius(Fancontrol.base.maxTemp, Fancontrol.base.unit)
                suffix: Fancontrol.base.unit
                onValueChanged: {
                    Fancontrol.base.maxTemp = value;
                    if (value < minTempBox.value) minTempBox.value = value;
                }
            }
        }
        Loader {
            active: !!systemdCom
            sourceComponent: RowLayout {
                width: column.width

                Label {
                    Layout.preferredWidth: root.textWidth
                    clip: true
                    text: i18n("Name of the fancontrol systemd service:")
                    horizontalAlignment: Text.AlignRight
                    Component.onCompleted: root.textWidth = Math.max(root.textWidth, contentWidth)
                }
                Fancontrol.OptionInput {
                    Layout.minimumWidth: implicitWidth
                    Layout.fillWidth: true
                    color: !!systemdCom && systemdCom.serviceExists ? "green" : "red"
                    value: Fancontrol.base.serviceName
                    onTextChanged: Fancontrol.base.serviceName = text
                }
            }
        }
        Loader {
            active: !!systemdCom
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
                    checked: !!systemdCom ? systemdCom.serviceEnabled : false
                    onCheckedChanged: {
                        if (!!systemdCom) {
                            systemdCom.serviceEnabled = checked;
                        }
                    }
                }
            }
        }
    }
}
