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
import QtQuick.Controls 1.2


Rectangle {
    id: root
    
    property QtObject fan
    property Item canvas: parent
    property real unscaledTemp: fan.temp ? fan.temp.value : minTemp
    property real unscaledPwm: fan.pwm
    property var locale: Qt.locale()
    readonly property real centerX: x + width / 2
    readonly property real centerY: y + height / 2
    property int size: 10
    
    width: size
    height: size
    radius: size / 2
    x: canvas.scaleX(unscaledTemp) - width/2
    y: canvas.scaleY(unscaledPwm) - height/2
    color: "black"
    
    Behavior on unscaledTemp {
        SpringAnimation {
            epsilon: 0.1
            spring: 1.0
            damping: 0.4
        }
    }
    Behavior on unscaledPwm {
        SpringAnimation {
            epsilon: 0.1
            spring: 1.0
            damping: 0.4
        }
    }
    
    MouseArea {
        id: pwmMouse
        anchors.fill: parent
        hoverEnabled: root.enabled ? true : false
    }
    
    Rectangle {
        id: tooltip
        x: parent.width
        y: - height
        width: Math.max(pwm.width, rpm.width)
        height: pwm.height + rpm.height
        radius: 4
        color: Qt.rgba(parent.color.r, parent.color.g, parent.color.b, 0.5)
        visible: root.enabled && pwmMouse.containsMouse
        
        Column {
            Label {
                id: pwm
                font.pixelSize: root.height * 1.5
                text: Number(Math.round(canvas.scalePwm(root.centerY)) / 2.55).toLocaleString(locale, 'f', 1) + '%'
            }
            Label {
                id: rpm
                font.pixelSize: root.height * 1.5
                text: fan.rpm + i18n("rpm")
            }
        }
    }
}