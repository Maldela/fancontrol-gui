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
    title: i18n("Fancontrol-GUI")
    width: 1024
    height: 768
    visible: true

    menuBar: MenuBar {
        Menu {
            title: i18n("File")
            MenuItem { action: loadAction }
            MenuItem { action: saveAction }
            MenuItem { 
		text: i18n("Save configuration file as")
		onTriggered: saveFileDialog.open()
		shortcut: StandardKey.SaveAs
	    }
            MenuItem {
                text: i18n("Exit")
                onTriggered: Qt.quit()
		shortcut: StandardKey.Quit
            }
        }
    }

    toolBar: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton { action: loadAction }
            ToolButton { action: saveAction }
            Loader {
                active: gui.hasSystemdCommunicator()
                sourceComponent: ToolButton {
                    iconName: gui.systemdCom.serviceActive ? "system-reboot" : "system-run"
                    onClicked: gui.systemdCom.serviceActive ? gui.systemdCom.restartService() : gui.systemdCom.serviceActive = true;
		    tooltip: gui.systemdCom.serviceActive ? i18n("Restart fancontrol") : i18n("Start fancontrol")
                }
            }
            Loader {
                active: gui.hasSystemdCommunicator()
                sourceComponent: ToolButton {
                    iconName: "system-shutdown"
                    enabled: gui.systemdCom.serviceActive
                    onClicked: gui.systemdCom.serviceActive = false;
                    tooltip: i18n("Stop fancontrol")
                }
            }
            Item {
                Layout.fillWidth: true
            }
            Slider {
                id: sizeSlider
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: 200
                value: 0.4
                visible: tabView.currentIndex == 1
            }
        }
    }

    TabView {
        property real minTemp: 30.0
        property real maxTemp: 90.0
        property string unit: i18n("Celsius")

        id: tabView
        anchors.fill: parent
        anchors.topMargin: 5
        frameVisible: true

        Tab {
            title: i18n("Sensors")
            SensorsTab {
                loader: gui.loader
            }
        }
        Tab {
            title: i18n("PwmFans")
            PwmFansTab {
                size: sizeSlider.value
                minTemp: tabView.minTemp
                maxTemp: tabView.maxTemp
                unit: tabView.unit
                loader: gui.loader
            }
        }
        Tab {
            title: i18n("Configfile")
            ConfigfileTab {
                loader: gui.loader
            }
        }
        Tab {
            title: i18n("Settings")
            SettingsTab {
                id: settingsTab
                interval: gui.loader.interval
                minTemp: tabView.minTemp
                maxTemp: tabView.maxTemp
                onMinTempChanged: tabView.minTemp = minTemp
                onMaxTempChanged: tabView.maxTemp = maxTemp
                onUnitChanged: tabView.unit = unit
                loader: gui.loader
                systemdCom: gui.hasSystemdCommunicator() ? gui.systemdCom : null
            }
        }
    }

    statusBar: StatusBar {
        Label {
            property string systemdError: gui.hasSystemdCommunicator() ? gui.systemdCom.error : ""
            property string loaderError: gui.loader.error

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
    
    Action {
	id: loadAction
        text: i18n("Load configuration file")
        iconName: "document-open"
	onTriggered: openFileDialog.open()
	tooltip: i18n("Load configuration file")
	shortcut: StandardKey.Open
    }
    Action {
	id: saveAction
	text: i18n("Save configuration file")
        onTriggered: gui.loader.save()
        iconName: "document-save"
	tooltip: i18n("Save configuration file")
	shortcut: StandardKey.Save
    }
    
    FileDialog {
        id: openFileDialog
        title: i18n("Please choose a configuration file")
        folder: "file:///etc"
        selectExisting: true
        selectMultiple: false
        modality: Qt.NonModal

        onAccepted: {
            gui.loader.load(fileUrl);
        }
    }
    FileDialog {
        id: saveFileDialog
        title: i18n("Save configuration file as")
        folder: "file:///etc"
        selectExisting: false
        selectMultiple: false
        modality: Qt.NonModal

        onAccepted: {
            gui.loader.save(fileUrl);
        }
    }
}
