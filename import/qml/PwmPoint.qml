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
import "units.js" as Units


Rectangle {
    id: root

    property Item background: parent
    readonly property real centerX: x + width / 2
    readonly property real centerY: y + height / 2
    readonly property point center: Qt.point(centerX, centerY)
    property int temp: !!background ? background.scaleTemp(centerX) : 0
    property int pwm: !!background ? background.scalePwm(centerY) : 0
    property real minimumX: - width / 2
    property real maximumX: parent.width - width / 2
    property real minimumY: - height / 2
    property real maximumY: parent.height - height / 2
    property int size: Kirigami.Units.smallSpacing * 2
    readonly property string unit: Fancontrol.Base.unit
    property bool draggable: true

    signal positionChanged()
    signal dragFinished()

    width: size
    height: size
    radius: size / 2
    border.width: pwmMouse.containsMouse || pwmMouse.drag.active ? 1 : 0

    onXChanged: positionChanged()
    onYChanged: positionChanged()

    Drag.dragType: Drag.Automatic

    MouseArea {
        id: pwmMouse

        anchors.fill: parent
        hoverEnabled: root.enabled ? true : false
        cursorShape: draggable ? containsPress || drag.active ? Qt.DragMoveCursor : Qt.PointingHandCursor : Qt.ArrowCursor
        drag.target: draggable ? root : null
        drag.axis: Drag.XAndYAxis
        drag.smoothed: false
        drag.minimumX: root.minimumX
        drag.maximumX: root.maximumX
        drag.minimumY: root.minimumY
        drag.maximumY: root.maximumY
        drag.onActiveChanged: {
            if (!drag.active)
                root.dragFinished();
        }
    }

    Rectangle {
        id: tooltip

        parent: root.parent
        x: root.width + root.x
        y: root.y - height
        z: 2
        width: column.width
        height: column.height
        radius: Kirigami.Units.smallSpacing / 2
        color: Qt.rgba(root.color.r, root.color.g, root.color.b, 0.3)
        visible: root.enabled && (pwmMouse.containsMouse || pwmMouse.drag.active)

        Column {
            id: column

            padding: Kirigami.Units.smallSpacing

            Text {
                font.pixelSize: root.size * 1.5
                text: Number(Units.fromCelsius(root.temp, unit)).toLocaleString(Qt.locale(), 'f', 0) + i18n(unit)
                color: Kirigami.Theme.textColor
            }
            Text {
                font.pixelSize: root.size * 1.5
                text: Number(root.pwm / 2.55).toLocaleString(Qt.locale(), 'f', 1) + Qt.locale().percent
                color: Kirigami.Theme.textColor
            }
        }
    }
}
