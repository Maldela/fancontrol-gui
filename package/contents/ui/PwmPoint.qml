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
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQml 2.2
import "../scripts/units.js" as Units

Rectangle {
    property Item canvas: parent
    property point center: Qt.point(x + width / 2, y + height / 2);
    readonly property real centerX: x + width / 2
    readonly property real centerY: y + height / 2
    property alias drag: pwmMouse.drag
    property alias size: root.width
    property int unit: 0
    property var locale: Qt.locale()

    id: root
    width: 10
    height: width
    radius: width / 2
    border.width: pwmMouse.containsMouse || drag.active ? 1 : 0

    onXChanged: parent.requestPaint();
    onYChanged: parent.requestPaint();

    Drag.dragType: Drag.Automatic

    MouseArea {
        id: pwmMouse
        anchors.fill: parent
        hoverEnabled: root.enabled ? true : false
        drag.target: root
        drag.axis: Drag.XAndYAxis
        drag.smoothed: false
        drag.minimumX: canvas.scaleX(canvas.minTemp) - root.width/2
        drag.maximumX: canvas.scaleX(canvas.maxTemp) - root.width/2
        drag.minimumY: canvas.scaleY(255) - root.height/2
        drag.maximumY: canvas.scaleY(0) - root.height/2
    }

    Rectangle {
        id: tooltip
        x: parent.width
        y: - height
        width: Math.max(pwm.width, temp.width)
        height: pwm.height + temp.height
        radius: 4
        color: Qt.rgba(parent.color.r, parent.color.g, parent.color.b, 0.5)
        visible: root.enabled && (pwmMouse.containsMouse || drag.active)

        Column {
            Label {
                id: pwm
                font.pixelSize: root.size * 1.5
                text: Number(Math.round(canvas.scalePwm(root.centerY)) / 2.55).toLocaleString(locale, 'f', 1) + '%'
            }
            Label {
                property string suffix: (unit == 0) ? "°C" : (unit == 1) ? "K" : "°F"
                
                id: temp
                font.pixelSize: root.size * 1.5
                text: Units.fromCelsius(Math.round(canvas.scaleTemp(root.centerX)), unit) + suffix
            }
        }
    }
}
