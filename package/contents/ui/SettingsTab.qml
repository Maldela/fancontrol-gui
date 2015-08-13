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
import "../scripts/arrayfunctions.js" as ArrayFunctions
import "../scripts/units.js" as Units

Item {
    property QtObject baseObject
    property QtObject loader: baseObject ? baseObject.loader : null
    property QtObject systemdCom: baseObject ? baseObject.hasSystemdCommunicator() ? baseObject.systemdCom : null : null
    property int interval: loader ? loader.interval : 1
    property int padding: 10
    property real textWidth: 0

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
            OptionInput {
                id: intervalValue
                Layout.minimumWidth: implicitWidth
                Layout.fillWidth: true
                inputMethodHints: Qt.ImhDigitsOnly
                text: interval
                onTextChanged: if (text && text != "0") loader.interval = parseInt(text)
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
            OptionInput {
                id: minTempValue
                Layout.minimumWidth: implicitWidth
                Layout.fillWidth: true
                inputMethodHints: Qt.ImhDigitsOnly
                onTextChanged: if (activeFocus) baseObject.minTemp = Units.toCelsius(text, baseObject.unit)
                Component.onCompleted: text = Units.fromCelsius(baseObject.minTemp, baseObject.unit)
                
                Connections {
                    target: baseObject
                    onUnitChanged: minTempValue.text = Units.fromCelsius(baseObject.minTemp, baseObject.unit)
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
            OptionInput {
                id: maxTempValue
                Layout.minimumWidth: implicitWidth
                Layout.fillWidth: true
                inputMethodHints: Qt.ImhDigitsOnly
                onTextChanged: if (activeFocus) baseObject.maxTemp = Units.toCelsius(text, baseObject.unit)
                Component.onCompleted: text = Units.fromCelsius(baseObject.maxTemp, baseObject.unit)
                
                Connections {
                    target: baseObject
                    onUnitChanged: maxTempValue.text = Units.fromCelsius(baseObject.maxTemp, baseObject.unit)
                }
            }
        }
        RowLayout {
            width: parent.width

            Label {
                Layout.preferredWidth: root.textWidth
                clip: true
                text: i18n("Unit:")
                horizontalAlignment: Text.AlignRight
                Component.onCompleted: root.textWidth = Math.max(root.textWidth, contentWidth)
            }
            ComboBox {
                id: unitBox
                Layout.minimumWidth: implicitWidth
                Layout.fillWidth: true
                model: [i18n("Celsius"), i18n("Kelvin"), i18n("Fahrenheit")]
                currentIndex: baseObject.unit
                onCurrentIndexChanged: {
                    baseObject.unit = currentIndex;
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
                    text: i18n("Name of the fancontrol systemd service:")
                    horizontalAlignment: Text.AlignRight
                    Component.onCompleted: root.textWidth = Math.max(root.textWidth, contentWidth)
                }
                OptionInput {
                    id: serviceName
                    Layout.minimumWidth: implicitWidth
                    Layout.fillWidth: true
                    color: systemdCom.serviceExists ? "green" : "red"
                    text: systemdCom.serviceName
                    onTextChanged: systemdCom.serviceName = text
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
                ComboBox {
                    id: autostartBox
                    Layout.minimumWidth: implicitWidth
                    Layout.fillWidth: true
                    model: [i18n("disabled") , i18n("enabled")]
                    currentIndex: systemdCom.serviceEnabled ? 1 : 0
                    onCurrentIndexChanged: systemdCom.serviceEnabled = (currentIndex == 1) ? true : false
                }
            }
        }
    }
}