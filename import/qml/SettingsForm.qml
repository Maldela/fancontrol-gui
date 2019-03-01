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


import QtQuick 2.6
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.1
import QtQuick.Dialogs 1.2
import org.kde.kirigami 2.3 as Kirigami
import Fancontrol.Qml 1.0 as Fancontrol


Kirigami.FormLayout {
    id: root

    readonly property QtObject systemdCom: Fancontrol.Base.hasSystemdCommunicator ? Fancontrol.Base.systemdCom : null
    readonly property QtObject loader: Fancontrol.Base.loader
    property bool showAll: true

    SpinBox {
        id: intervalSpinBox

        Kirigami.FormData.label: i18n("Interval:")
        Layout.fillWidth: true
        value: loader.interval
        from: 1.0
        editable: true
        textFromValue: function(value, locale) { return Number(value).toLocaleString(locale, 'f', 0) + ' ' + i18np("second", "seconds", value) }
        onValueModified: loader.interval = value

        Connections {
            target: loader
            onIntervalChanged: if (loader.interval != intervalSpinBox.value) intervalSpinBox.value = loader.interval
        }
    }
    SpinBox {
        id: minTempBox

        Kirigami.FormData.label: i18n("Minimum temperature for fan graphs:")
        Layout.fillWidth: true
        from: Fancontrol.Units.fromKelvin(0, Fancontrol.Base.unit)
        inputMethodHints: Qt.ImhFormattedNumbersOnly
        editable: true
        value: Fancontrol.Units.fromCelsius(Fancontrol.Base.minTemp, Fancontrol.Base.unit)
        textFromValue: function(value, locale) { return Number(value).toLocaleString(locale, 'f', 2) + Fancontrol.Base.unit }
        onValueModified: {
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
    SpinBox {
        id: maxTempBox

        Kirigami.FormData.label: i18n("Maximum temperature for fan graphs:")
        Layout.fillWidth: true
        from: Fancontrol.Units.fromKelvin(0, Fancontrol.Base.unit)
        inputMethodHints: Qt.ImhFormattedNumbersOnly
        editable: true
        value: Fancontrol.Units.fromCelsius(Fancontrol.Base.maxTemp, Fancontrol.Base.unit)
        textFromValue: function(value, locale) { return Number(value).toLocaleString(locale, 'f', 2) + Fancontrol.Base.unit }
        onValueModified: {
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
    RowLayout {
        Kirigami.FormData.label: i18n("Path to the fancontrol config file:")
        Layout.fillWidth: true

        TextField {
            id: fileInput

            Layout.fillWidth: true
            text: Fancontrol.Base.configUrl.toString().replace("file://", "")
            onTextChanged: Fancontrol.Base.configUrl = text;

            Connections {
                target: Fancontrol.Base
                onConfigUrlChanged: if(Fancontrol.Base.configUrl.toString().replace("file://", "") != fileInput.text) fileInput.text = Fancontrol.Base.configUrl.toString().replace("file://", "")
            }
        }
        Button {
            icon.name: "document-open"
            //                 tooltip: i18n("Open config file")
            onClicked: openFileDialog.open();
        }
    }
    TextField {
        id: serviceNameInput

        visible: !!systemdCom
        Kirigami.FormData.label: i18n("Name of the fancontrol systemd service:")
        Layout.fillWidth: true
        color: !!systemdCom && systemdCom.serviceExists ? "green" : "red"
        text: Fancontrol.Base.serviceName
        onTextChanged: Fancontrol.Base.serviceName = text

        Connections {
            target: Fancontrol.Base
            onServiceNameChanged: if(Fancontrol.Base.serviceName != serviceNameInput.text) serviceNameInput.text = Fancontrol.Base.serviceName
        }
    }
    CheckBox {
        id: trayBox

        Kirigami.FormData.label: i18n("Show tray icon:")
        checked: Fancontrol.Base.showTray
        visible: showAll
        onCheckedChanged: Fancontrol.Base.showTray = checked

        Connections {
            target: Fancontrol.Base
            onShowTrayChanged: if (Fancontrol.Base.showTray != trayBox.checked) trayBox.checked = Fancontrol.Base.showTray
        }
    }
    CheckBox {
        id: startMinimizedBox

        Kirigami.FormData.label: i18n("Start minimized:")
        checked: Fancontrol.Base.startMinimized
        visible: showAll
        onCheckedChanged: Fancontrol.Base.startMinimized = checked

        Connections {
            target: Fancontrol.Base
            onStartMinimizedChanged: if (Fancontrol.Base.startMinimized != startMinimizedBox.checked) startMinimizedBox.checked = Fancontrol.Base.startMinimized
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
