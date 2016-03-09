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
import Fancontrol.Qml 1.0 as Fancontrol


ApplicationWindow {
    id: window
    title: i18n("Fancontrol-GUI")
    width: 1024
    height: 768
    visible: true

    onClosing: {
        Fancontrol.base.save();
        windowConfig.save(window);
    }
    
    Component.onCompleted: {
        Fancontrol.base.load();
        windowConfig.restore(window);
    }

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
                active: Fancontrol.base.hasSystemdCommunicator()
                sourceComponent: ToolButton {
                    iconName: Fancontrol.base.systemdCom.serviceActive ? "system-reboot" : "system-run"
                    onClicked: {
                        Fancontrol.base.loader.abortTestingFans();
                        Fancontrol.base.systemdCom.serviceActive ? base.systemdCom.restartService() : base.systemdCom.serviceActive = true;
                    }
                    tooltip: Fancontrol.base.systemdCom.serviceActive ? i18n("Restart fancontrol") : i18n("Start fancontrol")
                }
            }
            Loader {
                active: Fancontrol.base.hasSystemdCommunicator()
                sourceComponent: ToolButton {
                    iconName: "system-shutdown"
                    enabled: Fancontrol.base.systemdCom.serviceActive
                    onClicked: Fancontrol.base.systemdCom.serviceActive = false;
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
                loader: Fancontrol.base.loader
            }
        }
        Tab {
            title: i18n("PwmFans")
            PwmFansTab {
                baseObject: Fancontrol.base
            }
        }
        Tab {
            title: i18n("Configfile")
            ConfigfileTab {
                loader: Fancontrol.base.loader
            }
        }
        Tab {
            id: settingsTab
            title: i18n("Settings")
            SettingsTab {
                gui: Fancontrol.base
            }
        }
    }

    Fancontrol.ErrorDialog {
        id: errorDialog
        visible: false
        modality: Qt.ApplicationModal
        loader: Fancontrol.base.loader
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
        onTriggered: base.save(true)
        iconName: "document-save"
        tooltip: i18n("Save configuration file") + " (" + Fancontrol.base.loader.configUrl.toString() + ")"
        shortcut: StandardKey.Save
    }

    FileDialog {
        id: openFileDialog
        title: i18n("Please choose a configuration file")
        folder: "file:///etc"
        selectExisting: true
        selectMultiple: false
        modality: Qt.NonModal

        onAccepted: Fancontrol.base.configUrl = fileUrl;
    }
    FileDialog {
        id: saveFileDialog
        title: i18n("Save configuration file as")
        folder: "file:///etc"
        selectExisting: false
        selectMultiple: false
        modality: Qt.NonModal

        onAccepted: Fancontrol.base.save(true, fileUrl);
    }
}
