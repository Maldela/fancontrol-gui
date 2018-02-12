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
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import Fancontrol.Qml 1.0 as Fancontrol


Item {
    property QtObject systemdCom: Fancontrol.base.hasSystemdCommunicator() ? Fancontrol.base.systemdCom : null
    property QtObject loader: Fancontrol.base.loader
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
                id: intervalSpinBox

                Layout.minimumWidth: implicitWidth
                Layout.fillWidth: true
                value: loader.interval
                suffix: " " + i18np("second", "seconds", loader.interval)
                minimumValue: 1.0
                onValueChanged: loader.interval = value

                Connections {
                    target: loader
                    onIntervalChanged: if (loader.interval != intervalSpinBox.value) intervalSpinBox.value = loader.interval
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
                maximumValue: Number.POSITIVE_INFINITY
                minimumValue: Fancontrol.Units.fromKelvin(0, Fancontrol.base.unit)
                value: Fancontrol.Units.fromCelsius(Fancontrol.base.minTemp, Fancontrol.base.unit)
                suffix: Fancontrol.base.unit
                onValueChanged: {
                    Fancontrol.base.minTemp = Fancontrol.Units.toCelsius(value, Fancontrol.base.unit);
                    if (value > maxTempBox.value) maxTempBox.value = value;
                }

                Connections {
                    target: Fancontrol.base
                    onMinTempChanged: if (Fancontrol.base.minTemp != minTempBox.value) minTempBox.value = Fancontrol.base.minTemp
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
                    Fancontrol.base.maxTemp = Fancontrol.Units.toCelsius(value, Fancontrol.base.unit);
                    if (value < minTempBox.value) minTempBox.value = value;
                }

                Connections {
                    target: Fancontrol.base
                    onMaxTempChanged: if (Fancontrol.base.maxTemp != maxTempBox.value) maxTempBox.value = Fancontrol.base.maxTemp
                }
            }
        }
        RowLayout {
            width: parent.width

            Label {
                Layout.preferredWidth: root.textWidth
                clip: true
                text: i18n("Path to the fancontrol config file:")
                horizontalAlignment: Text.AlignRight
                Component.onCompleted: root.textWidth = Math.max(root.textWidth, contentWidth)
            }
            Fancontrol.OptionInput {
                id: fileInput

                Layout.fillWidth: true
                value: Fancontrol.base.configUrl.toString().replace("file://", "")
                onTextChanged: Fancontrol.base.configUrl = text;
            }
            Button {
                iconName: "document-open"
                tooltip: i18n("Open config file")
                onClicked: openFileDialog.open();
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
                    id: serviceNameInput

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
                    text: i18n("Enable service at boot:")
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

                    Connections {
                        target: systemdCom
                        onServiceEnabledChanged: if (systemdCom.serviceEnabled != autostartBox.checked) autostartBox.checked = systemdCom.serviceEnabled
                    }
                }
            }
        }
        FileDialog {
            id: openFileDialog
            title: i18n("Please choose a configuration file")
            folder: "file:///etc"
            selectExisting: true
            selectMultiple: false
            modality: Qt.NonModal

            onAccepted: fileInput.text = fileUrl;
        }
    }
}
