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
            OptionInput {
                id: intervalValue
                Layout.minimumWidth: implicitWidth
                Layout.fillWidth: true
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 0 }
                text: Number(gui ? gui.interval : 1).toLocaleString(locale, 'f', 0)
                onTextChanged: {
                    if (activeFocus && text && root.locale) {
                        var value = Number.fromLocaleString(root.locale, text);
                        if (value) gui.interval = value;
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
            OptionInput {
                id: minTempValue
                Layout.minimumWidth: implicitWidth
                Layout.fillWidth: true
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                validator: DoubleValidator {}
                Component.onCompleted: text = Number(Units.fromCelsius(gui.minTemp, gui.unit)).toLocaleString()
                onTextChanged: {
                    if (activeFocus && text && root.locale) {
                        var value = Units.toCelsius(Number.fromLocaleString(locale, text), gui.unit);
                        if (value) gui.minTemp = value;
                    }
                }
                
                Connections {
                    target: gui
                    onUnitChanged: minTempValue.text = Units.fromCelsius(gui.minTemp, gui.unit)
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
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                validator: DoubleValidator {}
                Component.onCompleted: text = Number(Units.fromCelsius(gui.maxTemp, gui.unit)).toLocaleString()
                onTextChanged: {
                    if (activeFocus && text && root.locale) {
                        var value = Units.toCelsius(Number.fromLocaleString(locale, text), gui.unit);
                        if (value) gui.maxTemp = value;
                    }
                }
                
                Connections {
                    target: gui
                    onUnitChanged: maxTempValue.text = Units.fromCelsius(gui.maxTemp, gui.unit)
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
                    text: gui.serviceName
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
        
        Button {
            x: maxTempValue.x
            text: i18n("Detect fans")
            onClicked: loader.detectSensors()
        }
    }
}