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
import QtQuick.Dialogs 1.2
import org.kde.kirigami 2.3 as Kirigami
import org.kde.kcm 1.0 as KCM
import Fancontrol.Qml 1.0 as Fancontrol


Kirigami.Page {
    property QtObject loader: Fancontrol.Base.loader
    property QtObject systemdCom: Fancontrol.Base.systemdCom
    property QtObject pwmFanModel: Fancontrol.Base.pwmFanModel
    property QtObject tempModel: Fancontrol.Base.tempModel
    property QtObject profileModel: Fancontrol.Base.profileModel
    property real textWidth: 0
    property var pwmFans: pwmFanModel.fans
    property QtObject fan: pwmFans[fansListView.currentIndex]

    id: root

    implicitWidth: Kirigami.Units.gridUnit * 50
    implicitHeight: Kirigami.Units.gridUnit * 40

    KCM.ConfigModule.quickHelp: i18n("This module lets you configure your PWM fans.")

    Connections {
        target: Fancontrol.Base
        onNeedsApplyChanged: kcm.needsSave = Fancontrol.Base.needsApply
    }

    Connections {
        target: kcm
        onAboutToSave: {
            Fancontrol.Base.apply();
        }
        onAboutToLoad: {
            Fancontrol.Base.load();
            enabledBox.checked = systemdCom.serviceActive;
        }
        onAboutToDefault: enabledBox.checked = false
    }

//     contextualActions: [
//         Kirigami.Action {
//             text: i18n("Profiles")
//
//             Kirigami.Action {
//                 id: loadProfilesAction
//
//                 text: i18n("Load profile")
//             }
//             Kirigami.Action {
//                 text: i18n("Save profile")
//                 icon.name: "document-save"
//                 onTriggered: Fancontrol.Base.saveProfile(profileComboBox.saveText)
//             }
//             Kirigami.Action {
//                 text: i18n("Delete profile")
//                 icon.name: "edit-delete"
//                 onTriggered: Fancontrol.Base.deleteProfile(profileComboBox.currentIndex)
//             }
//         },
//         Kirigami.Action {
//             text: loader.sensorsDetected ? i18n("Detect fans again") : i18n("Detect fans")
//             icon.name: "dialog-password"
//             onTriggered: loader.detectSensors()
//         },
//         Kirigami.Action {
//             visible: !!systemdCom && !!fan
//             text: !!fan ? fan.testing ? i18n("Abort test") : i18n("Test start and stop values") : ""
//             icon.name: "dialog-password"
//             onTriggered: {
//                 if (fan.testing) {
//                     fan.abortTest();
//                 } else {
//                     fan.test();
//                 }
//             }
//         }
//     ]

    ColumnLayout {
        id: noFansInfo

        width: root.width
        y: root.height / 2 - height / 2
        spacing: Kirigami.Units.smallSpacing * 2
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
            icon.name: kcm.needsAuthorization ? "dialog-password" : ""
            onClicked: loader.detectSensors()
        }
    }

    header: CheckBox {
        id: enabledBox

        text: i18n("Control fans manually")
        checked: systemdCom.serviceEnabled && systemdCom.serviceActive;
        onCheckedChanged: {
            systemdCom.serviceActive = enabledBox.checked;
            loader.restartServiceAfterTesting = checked;
        }

        Connections {
            target: systemdCom
            onServiceActiveChanged: if (systemdCom.serviceActive != enabledBox.checked) enabledBox.checked = systemdCom.serviceActive
        }
    }

    Rectangle {
        id: fansListViewBackground

        Kirigami.Theme.colorSet: Kirigami.Theme.View

        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
        }
        width: root.width / 5
        color: Kirigami.Theme.backgroundColor
        visible: enabledBox.checked
        border.width: 1
        border.color: Kirigami.Theme.textColor

        ListView {
            id: fansListView

            anchors.fill: parent
            anchors.margins: fansListViewBackground.border.width
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.AutoFlickIfNeeded
            model: pwmFanModel
            header: Kirigami.BasicListItem {
                label: '<b>' + i18n("Fans") + '</b>'
                reserveSpaceForIcon: false
                hoverEnabled: false
                separatorVisible: false
                leftPadding: Kirigami.Units.smallSpacing
            }
            delegate: Kirigami.BasicListItem {
                label: display
                reserveSpaceForIcon: false
                hoverEnabled: true
                highlighted: ListView.isCurrentItem
                separatorVisible: false

                onPressedChanged: if (pressed) fansListView.currentIndex = index;
            }
        }
    }

    Fancontrol.FanHeader {
        id: fanHeader

        fan: root.fan
        visible: enabledBox.checked
        anchors {
            top: parent.top
            left: fansListViewBackground.right
            right: parent.right
            margins: Kirigami.Units.smallSpacing
        }
    }

    Loader {
        anchors {
            top: fanHeader.bottom
            left: fansListViewBackground.right
            right: parent.right
            bottom: settingsColumn.top
        }
        active: !!root.fan
        visible: enabledBox.checked
        sourceComponent: Fancontrol.FanItem {
            unit: Fancontrol.Base.unit
            fan: root.fan
            systemdCom: root.systemdCom
            tempModel: root.tempModel
            minTemp: Fancontrol.Base.minTemp
            maxTemp: Fancontrol.Base.maxTemp
        }
    }

    Column {
        id: settingsColumn

        anchors {
            left: fansListViewBackground.right
            right: parent.right
            bottom: parent.bottom
        }
        height: childrenRect.height
        visible: enabledBox.checked

        Item {
            id: advancedButton

            property bool expanded: false

            width: parent.width
            height: childrenRect.height

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

        Fancontrol.SettingsForm {
            id: settingsArea

            width: parent.width
            showAll: false
            clip: true
            state: advancedButton.expanded ? "VISIBLE" : "HIDDEN"

            states: [
                State {
                    name: "VISIBLE"

                    PropertyChanges {
                        target: settingsArea
                        height: implicitHeight
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
        }
    }

    Fancontrol.ErrorDialog {
        id: errorDialog

        modal: true
        anchors.centerIn: root
    }
}
