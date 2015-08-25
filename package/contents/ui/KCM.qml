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
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import org.kde.kcm 1.0
import "../scripts/arrayfunctions.js" as ArrayFunctions
import "../scripts/units.js" as Units

Item {
    property QtObject base: kcm.base
    property var locale: Qt.locale()
    property real textWidth: 0
    
    id: root
    implicitWidth: 1024
    implicitHeight: 768
       
    Column {
        id: header
        width: parent.width
        
        CheckBox {
            id: enabledBox
            visible: kcm.loader.allPwmFans.length > 0
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            text: i18n("Control fans manually")
            checked: kcm.manualControl
            onCheckedChanged: kcm.manualControl = checked
            
            Connections {
                target: kcm
                onManualControlChanged: enabledBox.checked = kcm.manualControl
            }
        }
        
        Label {
            visible: kcm.loader.allPwmFans.length == 0
            text: i18n("There are no pwm capable fans in your system.")
            anchors.top: enabledBox.bottom
            anchors.margins: 20
        }
        
        Button {
            text: i18n("Detect fans")
            visible: kcm.loader.allPwmFans.length == 0
            onClicked: kcm.loader.detectSensors()
        }
    }
        
    ColumnLayout {
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.top: header.bottom
        visible: enabledBox.checked

        RowLayout {  
            visible: enabledBox.checked && kcm.loader.allPwmFans.length > 0
            
            Label {
                text: i18n("Fan:")
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                renderType: Text.NativeRendering
            }
            ComboBox {
                id: fanCombobox
                model: ArrayFunctions.namesWithPaths(kcm.loader.allPwmFans)
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }
            Button {
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                text: i18n("Detect fans")
                onClicked: kcm.loader.detectSensors()
            }
        }
        
        Loader {
            Layout.fillWidth: true
            Layout.fillHeight: true
            active: !!kcm.loader.allPwmFans[fanCombobox.currentIndex]
            sourceComponent: PwmFan {
                minimizable: false
                unit: kcm.base.unit
                fan: kcm.loader.allPwmFans[fanCombobox.currentIndex]
                loader: kcm.loader
                systemdCom: kcm.systemdCom
                minTemp: kcm.base.minTemp
                maxTemp: kcm.base.maxTemp
            }
        }
        
        Label {
            property bool expanded: false
            property string prefix: expanded ? "v " : "> "

            id: expandLabel
            text: prefix + i18n("Advanced settings")
            font.bold: true
            
            MouseArea {
                anchors.fill: parent
                onClicked: parent.expanded = parent.expanded ? false : true
            }
        }
        
        RowLayout {
            visible: expandLabel.expanded

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
                value: base.loader ? base.loader.interval : 1
                type: "int"
                
                onTextChanged: {
                    if (activeFocus && text && root.locale) {
                        var value = Number.fromLocaleString(root.locale, text);
                        if (value) base.loader.interval = value;
                    }
                }
            }
        }
        RowLayout {
            visible: expandLabel.expanded

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
                validator: DoubleValidator { top: base.maxTemp }
                value: Units.fromCelsius(base.minTemp, base.unit)
                type: "double"
                
                onTextChanged: {
                    if (activeFocus && text && root.locale) {
                        var value = Units.toCelsius(Number.fromLocaleString(locale, text), base.unit);
                        if (value) base.minTemp = value;
                    }
                }
            }
        }
        RowLayout {
            visible: expandLabel.expanded

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
                validator: DoubleValidator { bottom: base.minTemp }
                value: Units.fromCelsius(base.maxTemp, base.unit)
                type: "double"
                
                onTextChanged: {
                    if (activeFocus && text && root.locale) {
                        var value = Units.toCelsius(Number.fromLocaleString(locale, text), base.unit);
                        if (value) base.maxTemp = value;
                    }
                }
            }
        }        
        RowLayout {
            visible: expandLabel.expanded

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
                color: base.systemdCom.serviceExists ? "green" : "red"
                value: base.serviceName
                type: "string"
                onTextChanged: base.serviceName = text
            }
        }
    }
}