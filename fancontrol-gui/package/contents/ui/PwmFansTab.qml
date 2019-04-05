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
import org.kde.kirigami 2.3 as Kirigami
import Fancontrol.Qml 1.0 as Fancontrol


Kirigami.Page {
    readonly property QtObject loader: Fancontrol.Base.loader
    readonly property QtObject systemdCom: Fancontrol.Base.hasSystemdCommunicator ? Fancontrol.Base.systemdCom : null
    readonly property QtObject pwmFanModel: Fancontrol.Base.pwmFanModel
    readonly property QtObject profileModel: Fancontrol.Base.profileModel
    property QtObject fan: applicationWindow().fan

    id: root

    header: Fancontrol.FanHeader {
        fan: root.fan
    }

    contextualActions: [
        Kirigami.Action {
            text: i18n("Manage profiles")
            onTriggered: profilesDialog.open()
        },
        Kirigami.Action {
            text: i18n("Service")
            visible: Fancontrol.Base.hasSystemdCommunicator
            tooltip: i18n("Control the systemd service")

            Kirigami.Action {
                text: !!systemdCom && systemdCom.serviceActive ? i18n("Stop service") : i18n("Start service")
                icon.name: !!systemdCom && systemdCom.serviceActive ? "media-playback-stop" : "media-playback-start"

                onTriggered: systemdCom.serviceActive = !systemdCom.serviceActive
            }
            Kirigami.Action {
                text: !!systemdCom && systemdCom.serviceEnabled ? i18n("Disable service") : i18n("Enable service")
                tooltip: !!systemdCom && systemdCom.serviceEnabled ? i18n("Disable service autostart at boot") : i18n("Enable service autostart at boot")

                onTriggered: systemdCom.serviceEnabled = !systemdCom.serviceEnabled
            }
        },
        Kirigami.Action {
            visible: !!systemdCom && !!fan
            text: !!fan ? fan.testing ? i18n("Abort test") : i18n("Test start and stop values") : ""
            icon.name: "dialog-password"
            onTriggered: {
                if (fan.testing) {
                    fan.abortTest();
                } else {
                    fan.test();
                }
            }
        }
    ]

    mainAction: Kirigami.Action {
        text: i18n("Apply")
        enabled: Fancontrol.Base.needsApply
        icon.name: "dialog-ok-apply"
        tooltip: i18n("Apply changes")
        shortcut: StandardKey.Apply

        onTriggered: Fancontrol.Base.apply()
    }
    rightAction: Kirigami.Action {
        text: i18n("Reset")
        enabled: Fancontrol.Base.needsApply
        icon.name: "edit-undo"
        tooltip: i18n("Revert changes")

        onTriggered: Fancontrol.Base.reset()
    }

    Loader {
        anchors.fill: parent
        active: !!root.fan

        sourceComponent: Fancontrol.FanItem {
            fan: root.fan
        }
    }

    Loader {
        anchors.centerIn: parent
        active: pwmFanModel.length === 0

        sourceComponent: Label {
            text: i18n("There are no pwm capable fans in your system.\nTry running 'sensors-detect' in a terminal and restart this application.")
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 14
            font.bold: true
        }
    }

    Fancontrol.ProfilesDialog {
        id: profilesDialog

        visible: false
        modal: true
        x: (root.width - width) / 2
        y: (root.height - height) / 2
    }
}
