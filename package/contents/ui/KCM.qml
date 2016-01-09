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
import QtQuick.Dialogs 1.2
import org.kde.kcm 1.0
import "../scripts/units.js" as Units


Item {
    property QtObject base: kcm.base
    property QtObject loader: !!base ? base.loader : null
    property QtObject systemdCom: !!base ? base.systemdCom : null
    property QtObject pwmFanModel: !!base ? base.pwmFanModel : null
    property QtObject tempModel: !!base ? base.tempModel : null
    property var locale: Qt.locale()
    property real textWidth: 0
    property var pwmFans: pwmFanModel ? pwmFanModel.fans : null

    id: root
    implicitWidth: 1024
    implicitHeight: 768

    ColumnLayout {
        id: noFansInfo
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter
        spacing: 20
        visible: pwmFanModel.count == 0

        Label {
            Layout.alignment: Qt.AlignCenter
            text: i18n("There are no pwm capable fans in your system.")
            font.pointSize: 14
            font.bold: true
        }

        Button {
            Layout.alignment: Qt.AlignCenter
            text: i18n("Detect fans")
            iconName: kcm.needsAuthorization ? "dialog-password" : ""
            onClicked: loader.detectSensors()
        }
    }

    CheckBox {
        id: enabledBox
        visible: pwmFanModel.count > 0
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        text: i18n("Control fans manually")
        checked: kcm.manualControl
        onCheckedChanged: kcm.manualControl = checked

        Connections {
            target: kcm
            onManualControlChanged: enabledBox.checked = kcm.manualControl
        }
    }

    ColumnLayout {
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.top: enabledBox.bottom
        visible: enabledBox.checked

        RowLayout {
            visible: enabledBox.checked && pwmFanModel.count > 0

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
            sourceComponent: PwmFan {
                unit: base.unit
                fan: pwmFans[fanComboBox.currentIndex]
                systemdCom: root.systemdCom
                tempModel: root.tempModel
                minTemp: base.minTemp
                maxTemp: base.maxTemp
            }
        }

        Row {
            property bool expanded: false

            id: expand

            Image {
                id: arrow
                source: parent.expanded ? "image://icon/go-down" : "image://icon/go-next"
                fillMode: Image.PreserveAspectFit
                height: advancedLabel.implicitHeight
            }
            Label {
                id: advancedLabel
                text: i18n("Advanced settings")
                font.bold: true
            }
        }
        MouseArea {
            anchors.fill: expand
            onClicked: expand.expanded = expand.expanded ? false : true
        }

        RowLayout {
            visible: expand.expanded

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
            visible: expand.expanded

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
                minimumValue: Units.fromKelvin(0, base.unit)
                value: Units.fromCelsius(base.minTemp, base.unit)
                suffix: base.unit == 0 ? i18n("°C") : base.unit == 1 ? i18n("K") : i18n("°F")
                onValueChanged: base.minTemp = value
            }
        }
        RowLayout {
            visible: expand.expanded

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
                value: Units.fromCelsius(base.maxTemp, base.unit)
                suffix: base.unit == 0 ? i18n("°C") : base.unit == 1 ? i18n("K") : i18n("°F")
                onValueChanged: base.maxTemp = value
            }
        }
        RowLayout {
            visible: expand.expanded

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
                value: base.serviceName
                onTextChanged: base.serviceName = text
            }
        }
        RowLayout {
            visible: expand.expanded

            Label {
                Layout.preferredWidth: root.textWidth
                clip: true
                text: i18n("Path to the fancontrol config file:")
                horizontalAlignment: Text.AlignRight
                Component.onCompleted: root.textWidth = Math.max(root.textWidth, contentWidth)
            }
            OptionInput {
                Layout.minimumWidth: implicitWidth
                Layout.fillWidth: true
                text: base.configUrl.toString().replace("file://", "")
                color: base.configValid ? "green" : "red"
                onTextChanged: base.configUrl = text
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

        onAccepted: base.configUrl = fileUrl;
    }

    ErrorDialog {
        id: errorDialog
        modality: Qt.ApplicationModal
        text: loader.error
    }
    Connections {
        target: loader
        onCriticalError: errorDialog.open()
    }
}
