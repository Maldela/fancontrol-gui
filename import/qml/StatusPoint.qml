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
import "math.js" as MoreMath


Rectangle {
    id: root

    property QtObject fan
    property Item background: parent
    property real unscaledTemp: !!fan && fan.hasTemp && !!fan.temp ? fan.temp.value : 0
    property real unscaledPwm: !!fan ? fan.pwm : 0
    property real unscaledRpm: !!fan ? fan.rpm : 0
    readonly property real centerX: x + width / 2
    readonly property real centerY: y + height / 2
    readonly property point center: Qt.point(centerX, centerY)
    property int size: Kirigami.Units.smallSpacing * 2
    readonly property string unit: Fancontrol.Base.unit

    width: size
    height: size
    radius: size / 2
    x: MoreMath.bound(-width/2, background.scaleX(unscaledTemp) - width/2, background.width-width/2)
    y: background.scaleY(unscaledPwm) - height/2
    color: "black"

    Behavior on unscaledTemp {
        SpringAnimation {
            epsilon: 0.1
            spring: 1.0
            damping: 0.6
        }
    }
    Behavior on unscaledPwm {
        SpringAnimation {
            epsilon: 0.1
            spring: 1.0
            damping: 0.6
        }
    }
    Behavior on unscaledRpm {
        SpringAnimation {
            epsilon: 0.1
            spring: 1.0
            damping: 0.6
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
        width: column.width
        height: column.height
        radius: Kirigami.Units.smallSpacing / 2
        color: Qt.rgba(parent.color.r, parent.color.g, parent.color.b, 0.3)
        visible: root.enabled && pwmMouse.containsMouse

        Column {
            id: column

            padding: Kirigami.Units.smallSpacing

            Text {
                font.pixelSize: root.size * 1.5
                text: Number(Units.fromCelsius(root.unscaledTemp, unit)).toLocaleString(Qt.locale(), 'f', 1) + i18n(unit)
                color: Kirigami.Theme.textColor
            }
            Text {
                font.pixelSize: root.size * 1.5
                text: Number(unscaledPwm / 2.55).toLocaleString(Qt.locale(), 'f', 1) + Qt.locale().percent
                color: Kirigami.Theme.textColor
            }
            Text {
                font.pixelSize: root.size * 1.5
                text: Number(unscaledRpm).toLocaleString(Qt.locale(), 'f', 0) + i18n("rpm")
                color: Kirigami.Theme.textColor
            }
        }
    }
}
