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
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.3 as Kirigami
import Fancontrol.Gui 1.0 as Gui
import Fancontrol.Qml 1.0 as Fancontrol


Kirigami.ApplicationWindow {
    id: window

    property string leftPage
    readonly property QtObject pwmFanModel: Fancontrol.Base.pwmFanModel
    property QtObject fan: pwmFanModel.length > 0 ? pwmFanModel.fan(0) : null

    function showWindow() {
        window.show()
        window.raise()
        window.requestActivate()
    }

    title: i18n("Fancontrol-GUI")
    reachableModeEnabled: false
    pageStack.defaultColumnWidth: Kirigami.Units.gridUnit * 30

    onLeftPageChanged: {
        window.pageStack.clear();
        if (leftPage)
            window.pageStack.push(Qt.resolvedUrl(leftPage));
    }

    onWideScreenChanged: drawer.drawerOpen = wideScreen

    onClosing: {
        windowConfig.save(window);
        if (Fancontrol.Base.needsApply && !saveOnCloseDialog.answered) {
            close.accepted = false;
            saveOnCloseDialog.open();
            return;
        }
    }

    Component.onCompleted: {
        Fancontrol.Base.load();
        windowConfig.restore(window);
        window.visible = !Fancontrol.Base.startMinimized;
        leftPage = "SensorsTab.qml";
    }

    globalDrawer: Kirigami.GlobalDrawer {
        id: drawer

        width: Kirigami.Units.gridUnit * 10
        modal: !window.wideScreen
        handleVisible: !window.wideScreen
        resetMenuOnTriggered: false

        function populateFans() {
            for (var i=fansAction.children.length-1; i>=0; i--) {
                fansAction.children[i].destroy();
            }
            var actions = [];
            for (var i=0; i<20; i++) {
                var action = fanActionComponent.createObject(fansAction, { "index": i });
                actions.push(action);
            }
            fansAction.children = actions;
        }

        Component {
            id: fanActionComponent

            Kirigami.Action {
                property int index
                property QtObject fan: pwmFanModel.fan(index)

                text: !!fan ? fan.name : ""
                visible: !!fan
                checked: window.fan === fan

                onTriggered: window.fan = fan
            }
        }

        Component.onCompleted: populateFans()

        Connections {
            target: pwmFanModel
            onFansChanged: {
                for (var i=0; i<pwmFanModel.length && i<fansAction.children.length; i++) {
                    fansAction.children[i].fan = pwmFanModel.fan(i);
                }
            }
        }

        actions: [
            Kirigami.Action {
                text: i18n("Sensors")
                checked: window.leftPage === "SensorsTab.qml"

                onTriggered: window.leftPage = "SensorsTab.qml"
            },
            Kirigami.Action {
                id: fansAction

                text: i18n("Fans")
                checked: window.leftPage === "PwmFansTab.qml"

                onTriggered: window.leftPage = "PwmFansTab.qml"
            },
            Kirigami.Action {
                text: i18n("Configfile")
                checked: window.leftPage === "ConfigfileTab.qml"

                onTriggered: window.leftPage = "ConfigfileTab.qml"
            },
            Kirigami.Action {
                text: i18n("Settings")
                checked: window.leftPage === "SettingsTab.qml"

                onTriggered: window.leftPage = "SettingsTab.qml"
            }
        ]
    }

    contextDrawer: Kirigami.ContextDrawer {}

    Loader {
        id: trayLoader

        active: Fancontrol.Base.showTray

        sourceComponent: Component {
            Gui.SystemTrayIcon {
                title: "Fancontrol-GUI"
                iconName: "org.kde.fancontrol.gui"
                profileModel: Fancontrol.Base.profileModel

                onActivateRequested: window.showWindow()
                onActivateProfile: {
                    Fancontrol.Base.applyProfile(profile);
                    Fancontrol.Base.apply();
                }
            }
        }
    }

    Fancontrol.ErrorDialog {
        id: errorDialog

        visible: false
        anchors.centerIn: window.contentItem
    }

    Dialog {
        id: saveOnCloseDialog

        property bool answered: false

        visible: false
        modal: true
        title: i18n("Unsaved changes")
        standardButtons: Dialog.Cancel | Dialog.Discard | Dialog.Apply
        anchors.centerIn: window.contentItem

        onRejected: close()
        onDiscarded: {
            answered = true;
            close();
            window.close();
        }
        onApplied: {
            Fancontrol.Base.apply();
            answered = true;
            close();
            window.close();
        }

        Label {
            id: text
            anchors.centerIn: parent
            text: i18n("There are unsaved changes.\nDo you want to apply these changes?")
        }
    }
}
