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
import Fancontrol.Qml 1.0 as Fancontrol


Dialog {
    title: i18n("Manage profiles")
    width: Kirigami.Units.gridUnit * 20
    height: Kirigami.Units.gridUnit * 20
    standardButtons: Dialog.Close

    RowLayout {
        anchors.fill: parent

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Kirigami.Theme.colorSet: Kirigami.Theme.View
            color: Kirigami.Theme.backgroundColor
            border.width: 1
            border.color: Kirigami.Theme.textColor

            ListView {
                id: profilesListView

                anchors.fill: parent
                anchors.margins: parent.border.width
                model: Fancontrol.Base.profileModel
                clip: true
                currentIndex: Fancontrol.Base.currentProfileIndex
                boundsBehavior: Flickable.StopAtBounds
                flickableDirection: Flickable.AutoFlickIfNeeded
                header: Kirigami.BasicListItem {
                    label: '<b>' + i18n("Profiles") + '</b>'
                    reserveSpaceForIcon: false
                    hoverEnabled: false
                    separatorVisible: false
                    leftPadding: Kirigami.Units.smallSpacing
                }
                delegate: Kirigami.BasicListItem {
                    label: {
                        if (Fancontrol.Base.currentProfileIndex === index)
                            return display + ' ' + i18n("(current profile)")
                        else
                            return display
                    }
                    reserveSpaceForIcon: false
                    hoverEnabled: true
                    highlighted: ListView.isCurrentItem
                    separatorVisible: false

                    onPressedChanged: if (pressed) profilesListView.currentIndex = index;
                }
            }
        }

        ColumnLayout {
            Layout.fillHeight: true

            Button {
                text: i18n("Apply profile")
                enabled: Fancontrol.Base.currentProfileIndex != profilesListView.currentIndex
                onClicked: Fancontrol.Base.applyProfile(profilesListView.currentIndex)
            }
            Button {
                text: i18n("Create new profile")
                enabled: Fancontrol.Base.currentProfileIndex == -1
                onClicked: newProfileNameDialog.open()
            }
            Button {
                text: i18n("Save to profile")
                enabled: Fancontrol.Base.currentProfileIndex != profilesListView.currentIndex && profilesListView.currentIndex >= 0
                onClicked: Fancontrol.Base.saveProfile(profilesListView.currentItem.label)
            }
            Button {
                text: i18n("Delete profile")
                enabled: profilesListView.currentIndex >= 0
                onClicked: Fancontrol.Base.deleteProfile(profilesListView.currentIndex)
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }

    Dialog {
        id: newProfileNameDialog

        title: i18n("New profile's name")
        standardButtons: Dialog.Ok | Dialog.Cancel
        visible: false
        anchors.centerIn: parent

        onAccepted: {
            Fancontrol.Base.saveProfile(newProfileNameField.text);
            newProfileNameField.text = "";
        }
        onRejected: newProfileNameField.text = ""

        TextField {
            id: newProfileNameField
        }
    }
}
