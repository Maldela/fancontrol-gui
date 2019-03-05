/*
 * Copyright (C) 2019  Malte Veerman <malte.veerman@gmail.com>
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
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.3 as Kirigami
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import Fancontrol.Qml 1.0 as Fancontrol


ColumnLayout {
    id: root

    readonly property QtObject fan: Fancontrol.Base.pwmFanModel.fan(plasmoid.configuration.Fan)

    Layout.minimumWidth: Plasmoid.formFactor == PlasmaCore.Types.Horizontal ? height : header.implicitWidth
    Layout.minimumHeight: Plasmoid.formFactor == PlasmaCore.Types.Vertical ? width  : header.implicitHeight + Kirigami.Units.gridUnit * 5
    Plasmoid.preferredRepresentation: Plasmoid.fullRepresentation
    Plasmoid.associatedApplication: "fancontrol_gui"
    Plasmoid.icon: "org.kde.fancontrol.gui"
    Kirigami.Theme.backgroundColor: PlasmaCore.ColorScope.backgroundColor
    Kirigami.Theme.textColor: PlasmaCore.ColorScope.textColor

    Component.onCompleted: {
        Fancontrol.Base.load();
    }

    Fancontrol.FanHeader {
        id: header

        fan: root.fan
        editable: false
        Layout.fillWidth: true
    }

    Fancontrol.FanItem {
        fan: root.fan
        showControls: false
        editable: false
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
}
