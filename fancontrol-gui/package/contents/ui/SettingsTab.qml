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
import Fancontrol.Qml 1.0


Item {
    property QtObject gui
    property QtObject systemdCom: !!gui && gui.hasSystemdCommunicator() ? gui.systemdCom : null
    property QtObject loader : !!gui ? gui.loader : null
    property int padding: 10
    property int unit: !!gui ? gui.unit : 0
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
                value: !!loader ? loader.interval : 1
                suffix: !!loader ? " " + i18np("second", "seconds", loader.interval): ""
                minimumValue: 1.0
                onValueChanged: {
                    if (!!loader) {
                        loader.interval = value;
                    }
                }
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
                minimumValue: Units.fromKelvin(0, unit)
                value: !!gui ? Units.fromCelsius(gui.minTemp, unit) : 0
                suffix: unit == 0 ? i18n("°C") : unit == 1 ? i18n("K") : i18n("°F")
                onValueChanged: {
                    if (!!gui) {
                        gui.minTemp = value;
                    }
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
                minimumValue: minTempBox.value
                value: !!gui ? Units.fromCelsius(gui.maxTemp, unit) : 0
                suffix: unit == 0 ? i18n("°C") : unit == 1 ? i18n("K") : i18n("°F")
                onValueChanged: {
                    if (!!gui) {
                        gui.maxTemp = value;
                    }
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
                OptionInput {
                    Layout.minimumWidth: implicitWidth
                    Layout.fillWidth: true
                    color: !!systemdCom && systemdCom.serviceExists ? "green" : "red"
                    value: !!gui ? gui.serviceName : ""
                    onTextChanged: {
                        if (!!gui) {
                            gui.serviceName = text;
                        }
                    }
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
