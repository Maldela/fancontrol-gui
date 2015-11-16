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
                active: base.hasSystemdCommunicator()
                sourceComponent: ToolButton {
                    iconName: base.systemdCom.serviceActive ? "system-reboot" : "system-run"
                    onClicked: {
                        base.loader.abortTestingFans();
                        base.systemdCom.serviceActive ? base.systemdCom.restartService() : base.systemdCom.serviceActive = true;
                    }
                    tooltip: base.systemdCom.serviceActive ? i18n("Restart fancontrol") : i18n("Start fancontrol")
                }
            }
            Loader {
                active: base.hasSystemdCommunicator()
                sourceComponent: ToolButton {
                    iconName: "system-shutdown"
                    enabled: base.systemdCom.serviceActive
                    onClicked: base.systemdCom.serviceActive = false;
                    tooltip: i18n("Stop fancontrol")
                }
            }
            Item {
                Layout.fillWidth: true
            }
        }
    }

    TabView {
        id: tabView
        anchors.fill: parent
        anchors.topMargin: 5
        frameVisible: true

        Tab {
            title: i18n("Sensors")
            SensorsTab {
                loader: base.loader
            }
        }
        Tab {
            title: i18n("PwmFans")
            PwmFansTab {
                baseObject: base
            }
        }
        Tab {
            title: i18n("Configfile")
            ConfigfileTab {
                loader: base.loader
            }
        }
        Tab {
            id: settingsTab
            title: i18n("Settings")
            SettingsTab {
                gui: base
            }
        }
    }
    
    ErrorDialog {
        id: errorDialog
        visible: !!base.loader.error
        modality: Qt.ApplicationModal
        text: base.loader.error
        onTextChanged: show()
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
        onTriggered: base.loader.save()
        iconName: "document-save"
        tooltip: i18n("Save configuration file") + " (" + base.loader.configUrl.toString() + ")"
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
            base.loader.load(fileUrl);
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
            base.loader.save(fileUrl);
        }
    }
}
