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
import "../javascript/arrayfunctions.js" as ArrayFunctions
import "../javascript/units.js" as Units

Item {
    property real minTemp: 30.0
    property real maxTemp: 90.0
    property int interval: 1
    property int padding: 10
    property string unit: i18n("Celsius")

    id: root
    anchors.fill: parent
    anchors.topMargin: 5

    onIntervalChanged: {
        var fans = ArrayFunctions.allPwmFans(loader.hwmons);
        for (var i=0; i<fans.length; i++) {
            fans[i].interval = interval;
        }
    }

    Rectangle {
        width: 600
        height: parent.height
        border.width: 1
        border.color: "black"
        radius: 5

        Column {
            anchors.fill: parent
            anchors.topMargin: padding
            spacing: 5

            RowLayout {
                width: parent.width

                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: padding
                    Layout.maximumWidth: parent.width - intervalValue.width - padding*2
                    clip: true
                    text: i18n("Interval:")
                }
                OptionInput {
                    id: intervalValue
                    anchors.right: parent.right
                    anchors.rightMargin: padding
                    width: 100
                    inputMethodHints: Qt.ImhDigitsOnly
                    text: interval
                    onTextChanged: if (text != "") loader.interval = parseInt(text)
                }
            }
            RowLayout {
                width: parent.width

                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: padding
                    Layout.maximumWidth: parent.width - minTempValue.width - padding*2
                    clip: true
                    text: i18n("Minimum temperature for fan graphs:")
                }
                OptionInput {
                    id: minTempValue
                    anchors.right: parent.right
                    anchors.rightMargin: padding
                    width: 100
                    inputMethodHints: Qt.ImhDigitsOnly
                    onTextChanged: if (activeFocus) minTemp = Units.toCelsius(text, unit)
                    Component.onCompleted: text = Units.fromCelsius(minTemp, unit)
                }
            }
            RowLayout {
                width: parent.width

                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: padding
                    Layout.maximumWidth: parent.width - maxTempValue.width - padding*2
                    clip: true
                    text: i18n("Maximum temperature for fan graphs:")
                }
                OptionInput {
                    id: maxTempValue
                    anchors.right: parent.right
                    anchors.rightMargin: padding
                    width: 100
                    inputMethodHints: Qt.ImhDigitsOnly
                    text: Units.fromCelsius(maxTemp, unit.currentText)
                    onTextChanged: if (activeFocus) maxTemp = Units.toCelsius(text, unit)
                    Component.onCompleted: text = Units.fromCelsius(maxTemp, unit)
                }
            }
            RowLayout {
                width: parent.width

                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: padding
                    Layout.maximumWidth: parent.width - maxTempValue.width - padding*2
                    clip: true
                    text: i18n("Unit:")
                }
                ComboBox {
                    id: unitBox
                    anchors.right: parent.right
                    anchors.rightMargin: padding
                    model: [i18n("Celsius"), i18n("Kelvin"), i18n("Fahrenheit")]
                    currentIndex: find(root.unit)
                    onCurrentIndexChanged: {
                        unit = currentText;
                        minTempValue.text = Units.fromCelsius(minTemp, unit);
                        maxTempValue.text = Units.fromCelsius(maxTemp, unit);
                    }
                }
            }
            RowLayout {
                width: parent.width
                visible: typeof systemdCom != "undefined"

                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: padding
                    Layout.maximumWidth: parent.width - maxTempValue.width - padding*2
                    clip: true
                    text: i18n("Name of the fancontrol systemd service:")
                }
                OptionInput {
                    id: serviceName
                    anchors.right: parent.right
                    anchors.rightMargin: padding
                    color: systemdCom.serviceExists ? "green" : "red"
                    width: 100
                    text: systemdCom.serviceName
                    onTextChanged: systemdCom.serviceName = text
                }
            }
            RowLayout {
                width: parent.width
                visible: typeof systemdCom != "undefined"

                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: padding
                    Layout.maximumWidth: parent.width - maxTempValue.width - padding*2
                    clip: true
                    text: i18n("Fancontrol systemd service autostart:")
                }
                ComboBox {
                    id: autostartBox
                    anchors.right: parent.right
                    anchors.rightMargin: padding
                    model: ["disabled" , "enabled"]
                    currentIndex: systemdCom.serviceEnabled ? 1 : 0
                    onCurrentIndexChanged: {
                        systemdCom.serviceEnabled = (currentIndex == 1) ? true : false;
                    }
                }
            }
        }
    }
}

