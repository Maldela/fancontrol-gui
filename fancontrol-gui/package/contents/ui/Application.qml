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
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import Fancontrol.Qml 1.0 as Fancontrol


ApplicationWindow {
    id: window

    title: i18n("Fancontrol-GUI")
    width: 1024
    height: 768
    color: palette.window
    visible: true

    onClosing: {
        windowConfig.save(window);
    }

    Component.onCompleted: {
        Fancontrol.base.load();
        windowConfig.restore(window);
    }

    toolBar: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton {
                action: applyAction
            }
            ToolButton {
                action: resetAction
            }
            Loader {
                active: !!Fancontrol.base.systemdCom

                sourceComponent: ToolButton {
                    action: startAction
                }
            }
            Loader {
                active: !!Fancontrol.base.systemdCom

                sourceComponent: ToolButton {
                    action: stopAction
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
            SensorsTab {}
        }
        Tab {
            title: i18n("PwmFans")
            PwmFansTab {}
        }
        Tab {
            title: i18n("Configfile")
            ConfigfileTab {}
        }
        Tab {
            id: settingsTab
            title: i18n("Settings")
            SettingsTab {}
        }
    }

    Fancontrol.ErrorDialog {
        id: errorDialog
        visible: false
        modality: Qt.ApplicationModal
    }

    Action {
        id: applyAction
        text: i18n("Apply")
        enabled: Fancontrol.base.needsApply
        onTriggered: Fancontrol.base.apply()
        iconName: "dialog-ok-apply"
        tooltip: i18n("Apply changes")
        shortcut: StandardKey.Apply
    }
    Action {
        id: resetAction
        text: i18n("Reset")
        enabled: Fancontrol.base.needsApply
        onTriggered: Fancontrol.base.reset()
        iconName: "edit-undo"
        tooltip: i18n("Revert changes")
    }
    Action {
        id: startAction
        text: i18n("Start")
        enabled: !!Fancontrol.base.systemdCom && !Fancontrol.base.systemdCom.serviceActive
        onTriggered: {
            Fancontrol.base.systemdCom.serviceActive = true;
        }
        iconName: "media-playback-start"
        tooltip: i18n("Enable manual control")
    }
    Action {
        id: stopAction
        text: i18n("Stop")
        enabled: !!Fancontrol.base.systemdCom && Fancontrol.base.systemdCom.serviceActive
        onTriggered: {
            Fancontrol.base.systemdCom.serviceActive = false;
        }
        iconName: "media-playback-stop"
        tooltip: i18n("Disable manual control")
    }
    SystemPalette {
        id: palette
    }
}
