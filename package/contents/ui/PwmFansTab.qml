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
import QtQuick.Controls 1.4
import "../scripts/arrayfunctions.js" as ArrayFunctions

ScrollView {
    property QtObject loader
    property real size: 1.0
    property real minTemp: 30.0
    property real maxTemp: 90.0
    property string unit: "Celsius"

    id: scrollView
    anchors.fill: parent
    anchors.topMargin: 5

    Flow {
        spacing: 20 * size
        width: scrollView.viewport.width
        move: Transition {
            NumberAnimation {
                easing.type: Easing.OutQuad
                properties: "x,y"
                duration: 300
            }
        }

        Repeater {
            property var fans: loader ? loader.allPwmFans : []

            id: repeater
            model: fans.length

            PwmFan {
                width: 1000 * size
                hwRatio: 0.8
                fan: repeater.fans[index]
                loader: loader
                minTemp: scrollView.minTemp
                maxTemp: scrollView.maxTemp
                unit: scrollView.unit
            }
        }
    }
    
    Label {
	anchors.margins: 10
	visible: repeater.fans.length == 0
	text: i18n("There are no pwm capable fans in your system.")
	font.bold: true
    }
}
