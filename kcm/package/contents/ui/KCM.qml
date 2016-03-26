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
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2
import org.kde.kcm 1.0
import Fancontrol.Qml 1.0 as Fancontrol


Item {
    property QtObject loader: Fancontrol.base.loader
    property QtObject systemdCom: Fancontrol.base.systemdCom
    property QtObject pwmFanModel: Fancontrol.base.pwmFanModel
    property QtObject tempModel: Fancontrol.base.tempModel
    property var locale: Qt.locale()
    property real textWidth: 0
    property var pwmFans: pwmFanModel ? pwmFanModel.fans : []

    id: root
    implicitWidth: 1024
    implicitHeight: 768

    Connections {
        target: loader
        onConfigFileChanged: kcm.needsSave = true
    }

    Connections {
        target: Fancontrol.base
        onMinTempChanged: kcm.needsSave = true
        onMaxTempChanged: kcm.needsSave = true
        onServiceNameChanged: kcm.needsSave = true
        onConfigUrlChanged: kcm.needsSave = true
    }

    Connections {
        target: kcm
        onAboutToSave: {
            Fancontrol.base.save(true);
            if (systemdCom.serviceActive && enabledBox.checked) {
                systemdCom.restartService();
            } else {
                systemdCom.serviceActive = enabledBox.checked;
            }
            systemdCom.serviceEnabled = enabledBox.checked;
        }
        onAboutToLoad: {
            Fancontrol.base.load();
            enabledBox.checked = systemdCom.serviceEnabled && systemdCom.serviceActive;
        }
        onAboutToDefault: enabledBox.checked = false
    }

    ColumnLayout {
        id: noFansInfo

        width: parent.width
        anchors.verticalCenter: parent.verticalCenter
        spacing: 20
        visible: pwmFans.length === 0

        Label {
            Layout.alignment: Qt.AlignCenter
            text: i18n("There are no pwm capable fans in your system.")
            font.pointSize: 14
            font.bold: true
        }

        Button {
            Layout.alignment: Qt.AlignCenter
            text: loader.sensorsDetected ? i18n("Detect fans again") : i18n("Detect fans")
            iconName: kcm.needsAuthorization ? "dialog-password" : ""
            onClicked: loader.detectSensors()
        }
    }

    CheckBox {
        id: enabledBox

        anchors.top: parent.top
        visible: pwmFans.length > 0
        text: i18n("Control fans manually")
        checked: systemdCom.serviceEnabled && systemdCom.serviceActive;
        onCheckedChanged: if (checked !== systemdCom.serviceActive || checked !== systemdCom.serviceEnabled) {
            kcm.needsSave = true;
            loader.restartServiceAfterTesting = checked;
        }
    }

    ColumnLayout {
        id: bodyLayout

        width: parent.width
        anchors.bottom: advancedButton.top
        anchors.top: enabledBox.bottom
        anchors.bottomMargin: advancedButton.height / 4
        visible: enabledBox.checked

        RowLayout {
            visible: enabledBox.checked && pwmFans.length > 0

            Label {
                text: i18n("Fan:")
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                renderType: Text.NativeRendering
            }
            ComboBox {
                id: fanComboBox
                model: pwmFanModel
                textRole: "display"
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }
            Button {
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                text: i18n("Detect fans")
                iconName: kcm.needsAuthorization ? "dialog-password" : ""
                onClicked: loader.detectSensors()
            }
        }

        Loader {
            Layout.fillWidth: true
            Layout.fillHeight: true
            active: !!pwmFans[fanComboBox.currentIndex]
            sourceComponent: Fancontrol.FanItem {
                unit: Fancontrol.base.unit
                fan: pwmFans[fanComboBox.currentIndex]
                systemdCom: root.systemdCom
                tempModel: root.tempModel
                minTemp: Fancontrol.base.minTemp
                maxTemp: Fancontrol.base.maxTemp
            }
        }
    }

    Item {
        id: advancedButton

        property bool expanded: false

        anchors.bottom: settingsArea.top
        width: parent.width
        height: advancedArrow.height
        visible: enabledBox.checked

        Image {
            id: advancedArrow

            source: parent.expanded ? "image://icon/go-down" : "image://icon/go-next"
            fillMode: Image.PreserveAspectFit
            height: advancedLabel.implicitHeight
        }
        Label {
            id: advancedLabel

            anchors.left: advancedArrow.right
            text: i18n("Advanced settings")
            font.bold: true
        }
        MouseArea {
            anchors.fill: parent
            onClicked: parent.expanded = !parent.expanded
        }
    }

    ColumnLayout {
        id: settingsArea

        visible: enabledBox.checked && parent.height - enabledBox.height - bodyLayout.height - advancedButton.height > height
        width: parent.width
        anchors.bottom: parent.bottom
        clip: true
        state: advancedButton.expanded ? "VISIBLE" : "HIDDEN"

        states: [
            State {
                name: "VISIBLE"

                PropertyChanges {
                    target: settingsArea
                    height: settingsArea.implicitHeight
                }
            },
            State {
                name: "HIDDEN"

                PropertyChanges {
                    target: settingsArea
                    height: 0
                }
            }
        ]

        transitions: Transition {
            NumberAnimation {
                properties: "height"
                easing.type: Easing.InOutQuad
            }
        }

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
                maximumValue: maxTempBox.value
                minimumValue: Units.fromKelvin(0, Fancontrol.base.unit)
                value: Units.fromCelsius(Fancontrol.base.minTemp, Fancontrol.base.unit)
                suffix: Fancontrol.base.unit == 0 ? i18n("°C") : Fancontrol.base.unit == 1 ? i18n("K") : i18n("°F")
                onValueChanged: Fancontrol.base.minTemp = value
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
                value: Units.fromCelsius(Fancontrol.base.maxTemp, Fancontrol.base.unit)
                suffix: Fancontrol.base.unit == 0 ? i18n("°C") : Fancontrol.base.unit == 1 ? i18n("K") : i18n("°F")
                onValueChanged: Fancontrol.base.maxTemp = value
            }
        }
        RowLayout {
            width: parent.width

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
                color: systemdCom.serviceExists ? "green" : "red"
                value: Fancontrol.base.serviceName
                onTextChanged: Fancontrol.base.serviceName = text
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
                Layout.minimumWidth: implicitWidth
                Layout.fillWidth: true
                text: Fancontrol.base.configUrl.toString().replace("file://", "")
                color: Fancontrol.base.configValid ? "green" : "red"
                onTextChanged: Fancontrol.base.configUrl = text
            }
            Button {
                action: loadAction
            }
        }
    }

    Action {
        id: loadAction
        iconName: "document-open"
        onTriggered: openFileDialog.open()
        tooltip: i18n("Load configuration file")
        shortcut: StandardKey.Open
    }

    FileDialog {
        id: openFileDialog
        title: i18n("Please choose a configuration file")
        folder: "file:///etc"
        selectExisting: true
        selectMultiple: false

        onAccepted: Fancontrol.base.configUrl = fileUrl;
    }

    Fancontrol.ErrorDialog {
        id: errorDialog
        modality: Qt.ApplicationModal
        loader: root.loader
    }
}
