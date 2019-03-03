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
    readonly property QtObject loader: Fancontrol.Base.loader
    readonly property QtObject systemdCom: Fancontrol.Base.systemdCom
    readonly property QtObject pwmFanModel: Fancontrol.Base.pwmFanModel
    readonly property QtObject tempModel: Fancontrol.Base.tempModel
    readonly property QtObject profileModel: Fancontrol.Base.profileModel
    readonly property QtObject fan: fansListView.currentItem ? fansListView.currentItem.fan : null

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

    header: CheckBox {
        id: enabledBox

        text: i18n("Control fans manually")
        checked: systemdCom.serviceEnabled && systemdCom.serviceActive && pwmFanModel.length > 0
        enabled: pwmFanModel.length > 0
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
                property QtObject fan: object

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
            fan: root.fan
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
