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
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: window
    title: qsTr("Fancontrol-GUI")
    width: 1024
    height: 768
    visible: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("Load configuration file")
                onTriggered: openFileDialog.open();
            }
            MenuItem {
                text: qsTr("Save configuration file")
                onTriggered: loader.save();
            }
            MenuItem {
                text: qsTr("Save configuration file as")
                onTriggered: saveFileDialog.open();
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    toolBar: ToolBar {
        id: toolBar
        RowLayout {
            id: toolBarLayout
            anchors.fill: parent

            ToolButton {
                iconName: "document-open"
                onClicked: openFileDialog.open();

                ToolTip {
                    text: "Open config file"
                }
            }
            ToolButton {
                iconName: "document-save"
                onClicked: loader.save();

                ToolTip {
                    text: "Save config file"
                }
            }
            ToolButton {
                id: restartButton
                iconName: activeTimer.serviceActive ? "system-reboot" : "system-run"
                visible: typeof systemdCom !== "undefined"
                onClicked: systemdCom.dbusAction("ReloadOrRestartUnit",
                                                 [systemdCom.serviceName + ".service", "replace"])

                ToolTip {
                    text: activeTimer.serviceActive ? "Restart fancontrol" : "Start fancontrol"
                }


            }
            ToolButton {
                iconName: "system-shutdown"
                visible: typeof systemdCom !== "undefined"
                enabled: activeTimer.serviceActive
                onClicked: systemdCom.dbusAction("StopUnit",
                                                 [systemdCom.serviceName + ".service", "replace"])

                ToolTip {
                    text: "Stop fancontrol"
                }
            }
            Item {
                id: placeholder
                Layout.fillWidth: true
            }
            Slider {
                id: sizeSlider
                Layout.alignment: Qt.AlignRight
                Layout.maximumWidth: 200
                value: 0.4
                visible: tabView.currentIndex == 1
            }
        }
    }

    TabView {
        property real minTemp: 30.0
        property real maxTemp: 90.0
        property string unit: "Celsius"

        id: tabView
        anchors.fill: parent

        Tab {
            title: "Sensors"
            SensorsTab {}
        }
        Tab {
            title: "PwmFans"
            PwmFansTab {
                size: sizeSlider.value
                minTemp: tabView.minTemp
                maxTemp: tabView.maxTemp
                unit: tabView.unit
            }
        }
        Tab {
            title: "Configfile"
            ConfigfileTab {}
        }

        Tab {
            title: "Settings"
            SettingsTab {
                id: settingsTab
                interval: loader.interval
                minTemp: tabView.minTemp
                maxTemp: tabView.maxTemp
                onMinTempChanged: tabView.minTemp = minTemp
                onMaxTempChanged: tabView.maxTemp = maxTemp
                onUnitChanged: tabView.unit = unit
            }
        }
    }

    statusBar: StatusBar {
        Text {
            property string systemdError: typeof systemdCom != "undefined" ? systemdCom.error : ""
            property string loaderError: loader.error

            color: "red"

            onSystemdErrorChanged: {
                if (systemdError !== "Success" && systemdError.search("succeeded") == -1)
                    text = systemdError;
                else if (loaderError === "Success" || loaderError === "")
                    text = ""
            }
            onLoaderErrorChanged: {
                if (loaderError !== "Success")
                    text = loaderError;
                else if (systemdError === "Success" || systemdError === "")
                    text = ""
            }
        }
    }

    Timer {
        property bool serviceActive

        id: activeTimer
        interval: 1000
        running: typeof systemdCom !== "undefined"
        repeat: true
        onTriggered: serviceActive = systemdCom.serviceActive()
    }

    FileDialog {
        id: openFileDialog
        title: "Please choose a configuration file"
        folder: "file:///etc"
        selectExisting: true
        selectMultiple: false
        modality: Qt.NonModal

        onAccepted: {
            loader.configUrl = fileUrl;
        }
    }

    FileDialog {
        id: saveFileDialog
        title: "Save configuration file as"
        folder: "file:///etc"
        selectExisting: false
        selectMultiple: false
        modality: Qt.NonModal

        onAccepted: {
            loader.save(fileUrl);
        }
    }
}
