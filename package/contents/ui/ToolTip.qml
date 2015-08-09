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
import "../scripts/coordinates.js" as Coordinates

Rectangle {
    property alias text: label.text
    property alias textColor: label.color
    property alias backgroundColor: rect.color
    property int delay: 1000
    property Item target: parent
    readonly property bool hovered: target.hovered

    id: rect
    width: label.width + 20
    height: label.height + 10
    anchors.top: target.bottom
    anchors.horizontalCenter: target.horizontalCenter
    anchors.horizontalCenterOffset: Math.max(0, width/2 - (Coordinates.absoluteCoordinatesOf(target).x+target.width/2))
    radius: 6
    color: palette.dark
    visible: false

    onHoveredChanged: {
        if (hovered) {
            if (timer.running)
                timer.restart();
            else
                timer.start();
        }
        else
        {
            if (timer.running)
                timer.stop();
            visible = false;
        }
    }

    Label {
        id: label
        anchors.centerIn: parent
        color: palette.highlightedText
    }

    Timer {
        id: timer
        interval: delay
        repeat: false
        onTriggered: parent.visible = true
    }

    SystemPalette {
        id: palette
    }
}

