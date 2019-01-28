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
import org.kde.kirigami 2.4 as Kirigami
import Fancontrol.Qml 1.0 as Fancontrol


Item {
    property QtObject systemdCom: Fancontrol.Base.hasSystemdCommunicator() ? Fancontrol.Base.systemdCom : null
    property QtObject loader: Fancontrol.Base.loader
    property int padding: Kirigami.Units.smallSpacing
    property real textWidth: 0
    property var locale: Qt.locale()

    id: root
    anchors.fill: parent
    anchors.margins: Kirigami.Units.smallSpacing

    Column {
        id: column
        anchors.fill: parent
        anchors.margins: padding
        spacing: Kirigami.Units.smallSpacing

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
                minimumValue: Fancontrol.Units.fromKelvin(0, Fancontrol.Base.unit)
                value: Fancontrol.Units.fromCelsius(Fancontrol.Base.minTemp, Fancontrol.Base.unit)
                suffix: Fancontrol.Base.unit
                onValueChanged: {
                    Fancontrol.Base.minTemp = Fancontrol.Units.toCelsius(value, Fancontrol.Base.unit);
                    if (value >= maxTempBox.value) maxTempBox.value = value + 1;
                }

                Connections {
                    target: Fancontrol.Base
                    onMinTempChanged: {
                        if (Fancontrol.Units.fromCelsius(Fancontrol.Base.minTemp, Fancontrol.Base.unit) != minTempBox.value) {
                            minTempBox.value = Fancontrol.Units.fromCelsius(Fancontrol.Base.minTemp, Fancontrol.Base.unit);
                        }
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
                minimumValue: Fancontrol.Units.fromKelvin(0, Fancontrol.Base.unit)
                value: Fancontrol.Units.fromCelsius(Fancontrol.Base.maxTemp, Fancontrol.Base.unit)
                suffix: Fancontrol.Base.unit
                onValueChanged: {
                    Fancontrol.Base.maxTemp = Fancontrol.Units.toCelsius(value, Fancontrol.Base.unit);
                    if (value <= minTempBox.value) minTempBox.value = value - 1;
                }

                Connections {
                    target: Fancontrol.Base
                    onMaxTempChanged: {
                        if (Fancontrol.Units.fromCelsius(Fancontrol.Base.maxTemp, Fancontrol.Base.unit) != maxTempBox.value) {
                            maxTempBox.value = Fancontrol.Units.fromCelsius(Fancontrol.Base.maxTemp, Fancontrol.Base.unit);
                        }
                    }
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
                value: Fancontrol.Base.configUrl.toString().replace("file://", "")
                onTextChanged: Fancontrol.Base.configUrl = text;
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
                    value: Fancontrol.Base.serviceName
                    onTextChanged: Fancontrol.Base.serviceName = text
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
        RowLayout {
            width: column.width

            Label {
                Layout.preferredWidth: root.textWidth
                clip: true
                text: i18n("Show tray icon:")
                horizontalAlignment: Text.AlignRight
                Component.onCompleted: root.textWidth = Math.max(root.textWidth, contentWidth)
            }
            CheckBox {
                id: trayBox

                Layout.minimumWidth: implicitWidth
                Layout.fillWidth: true
                checked: Fancontrol.Base.showTray
                onCheckedChanged: Fancontrol.Base.showTray = checked

                Connections {
                    target: Fancontrol.Base
                    onShowTrayChanged: if (Fancontrol.Base.showTray != trayBox.checked) trayBox.checked = Fancontrol.Base.showTray
                }
            }
        }
        RowLayout {
            width: column.width
            enabled: Fancontrol.Base.showTray

            Label {
                Layout.preferredWidth: root.textWidth
                clip: true
                text: i18n("Start minimized:")
                horizontalAlignment: Text.AlignRight
                Component.onCompleted: root.textWidth = Math.max(root.textWidth, contentWidth)
            }
            CheckBox {
                id: startMinimizedBox

                Layout.minimumWidth: implicitWidth
                Layout.fillWidth: true
                checked: Fancontrol.Base.startMinimized
                onCheckedChanged: Fancontrol.Base.startMinimized = checked

                Connections {
                    target: Fancontrol.Base
                    onStartMinimizedChanged: if (Fancontrol.Base.startMinimized != startMinimizedBox.checked) startMinimizedBox.checked = Fancontrol.Base.startMinimized
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
