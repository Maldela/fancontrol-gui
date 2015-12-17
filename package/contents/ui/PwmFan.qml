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
import QtQuick.Layouts 1.1
import "../scripts/arrayfunctions.js" as ArrayFunctions
import "../scripts/math.js" as MoreMath
import "../scripts/units.js" as Units
import "../scripts/colors.js" as Colors


Rectangle {
    property QtObject fan
    property QtObject loader
    property QtObject systemdCom
    property real minTemp: 40.0
    property real maxTemp: 90.0
    property int margin: 5
    property int minimizeDuration: 400
    property int unit: 0
    property real convertedMinTemp: Units.fromCelsius(minTemp, unit)
    property real convertedMaxTemp: Units.fromCelsius(maxTemp, unit)
    readonly property alias name: nameField.text

    id: root
    color: "transparent"
    border.color: palette.windowText
    border.width: 2
    radius: 10
    clip: false

    function update() {
        if (fan) {
            hasTempCheckBox.checked = Qt.binding(function() { return fan.hasTemp; })
            fanOffCheckBox.checked = Qt.binding(function() { return (fan.minPwm == 0); })
            if (fan.hasTemp && loader) {
                tempBox.currentIndex = loader.allTemps.indexOf(fan.temp);
            }
        }
        bgCanvas.requestPaint();
    }

    onFanChanged: update();
    onLoaderChanged: update()
    onConvertedMinTempChanged: if (fan) bgCanvas.requestPaint()
    onConvertedMaxTempChanged: if (fan) bgCanvas.requestPaint()

    Connections {
        target: loader
        onConfigUrlChanged: update()
    }

    SystemPalette {
        id: palette
        colorGroup: SystemPalette.Active
    }
    SystemPalette {
        id: disabledPalette
        colorGroup: SystemPalette.Disabled
    }

    TextEdit {
        id: nameField
        anchors {
            left: parent.left
            leftMargin: margin
            right: parent.right
            rightMargin: margin
            top: parent.top
            topMargin: margin
        }
        visible: root.height >= height + margin*2
        text: fan.name
        onTextChanged: fan.name = text
        color: palette.text
        horizontalAlignment: TextEdit.AlignLeft
        wrapMode: TextEdit.Wrap
        font.bold: true
        font.pointSize: 14
        selectByMouse: true

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.IBeamCursor
            acceptedButtons: Qt.NoButton
        }
    }

    Item {
        id: graph

        property int fontSize: MoreMath.bound(8, height / 20 + 1, 16)
        property QtObject pal: fan.hasTemp ? palette : disabledPalette
        property string suffix: (root.unit == 0) ? "°C" : (root.unit == 1) ? "K" : "°F"
        property int verticalScalaCount: 6
        property var horIntervals: MoreMath.intervals(root.convertedMinTemp, root.convertedMaxTemp, 10)

        anchors {
            left: parent.left
            right: parent.right
            top: nameField.bottom
            bottom: settingsArea.top
        }
        visible: background.height > 0 && background.width > 0

        Item {
            id: verticalScala

            anchors {
                top: background.top
                bottom: background.bottom
                left: parent.left
            }
            width: graph.fontSize * 3

            Repeater {
                model: graph.verticalScalaCount

                Label {
                    x: 0
                    width: verticalScala.width - graph.fontSize / 3
                    y: background.scaleY(255 / (graph.verticalScalaCount - 1) * index) - graph.fontSize / 2
                    horizontalAlignment: Text.AlignRight
                    color: graph.pal.text
                    text: index * (100 / (graph.verticalScalaCount - 1)) + "%"
                    font.pixelSize: graph.fontSize
                }
            }
        }
        Item {
            id: horizontalScala

            anchors {
                right: background.right
                bottom: parent.bottom
                left: background.left
            }
            height: graph.fontSize * 2

            Repeater {
                model: graph.horIntervals.length;

                Label {
                    x: Math.min(horizontalScala.width, background.scaleX(Units.toCelsius(graph.horIntervals[index]), root.unit)) - width / 2
                    y: graph.fontSize / 2
                    color: graph.pal.text
                    text: graph.horIntervals[index] + graph.suffix
                    font.pixelSize: graph.fontSize
                }
            }
        }

        Rectangle {
            id: background

            property alias pal: graph.pal

            color: pal.light
            border.color: pal.text
            border.width: 2
            radius: 1

            anchors {
                top: parent.top
                left: verticalScala.right
                bottom: horizontalScala.top
                right: parent.right
                topMargin: parent.fontSize
                bottomMargin: 0
                rightMargin: parent.fontSize * 2
                leftMargin: 0
            }

            function scaleX(temp) {
                return (temp - minTemp) * width / (maxTemp - minTemp);
            }
            function scaleY(pwm) {
                return height - pwm * height / 255;
            }
            function scaleTemp(x) {
                return x / width * (maxTemp - minTemp) + minTemp;
            }
            function scalePwm(y) {
                return 255 - y / height * 255;
            }

            Canvas {
                id: bgCanvas

                anchors.fill: parent
                anchors.margins: parent.border.width
                renderStrategy: Canvas.Cooperative

                property alias pal: background.pal

                onPaint: {
                    var c = bgCanvas.getContext("2d");
                    c.clearRect(0, 0, width, height);
                    var gradient = c.createLinearGradient(0, 0, width, 0);
                    gradient.addColorStop(0, "rgb(0, 0, 255)");
                    gradient.addColorStop(1, "rgb(255, 0, 0)");
                    c.fillStyle = gradient;
                    c.lineWidth = 2;
                    c.strokeStyle = gradient;
                    c.lineJoin = "round";
                    c.beginPath();
                    if (fanOffCheckBox.checked) {
                        c.moveTo(stopPoint.centerX, height);
                    } else {
                        c.moveTo(0, stopPoint.centerY);
                    }
                    c.lineTo(stopPoint.centerX, stopPoint.centerY);
                    c.lineTo(maxPoint.centerX, maxPoint.centerY);
                    c.lineTo(width, maxPoint.centerY);
                    c.stroke();
                    c.lineTo(width, height);
                    if (fanOffCheckBox.checked) {
                        c.lineTo(stopPoint.centerX, height);
                    } else {
                        c.lineTo(0, height);
                    }
                    c.fill();

                    //blend background
                    gradient = c.createLinearGradient(0, 0, 0, height);
                    gradient.addColorStop(0, Colors.setAlpha(background.color, 0.5));
                    gradient.addColorStop(1, Colors.setAlpha(background.color, 0.9));
                    c.fillStyle = gradient;
                    c.fill();

                    //draw mesh
                    c.strokeStyle = Colors.setAlpha(pal.text, 0.3);
                    for (var i=0; i<=100; i+=20) {
                        var y = background.scaleY(i*2.55);
                        if (i != 0 && i != 100) {
                            for (var j=0; j<=width; j+=15) {
                                c.moveTo(j, y);
                                c.lineTo(Math.min(j+5, width), y);
                            }
                        }
                    }
                    if (graph.horIntervals.length > 1) {
                        for (var i=1; i<graph.horIntervals.length; i++) {
                            var x = background.scaleX(Units.toCelsius(graph.horIntervals[i], unit));
                            for (var j=0; j<=height; j+=20) {
                                c.moveTo(x, j);
                                c.lineTo(x, Math.min(j+5, width));
                            }
                        }
                    }
                    c.stroke();
                }
            }
            StatusPoint {
                id: currentPwm
                size: graph.fontSize
                visible: background.contains(center) && fan.hasTemp
                fan: root.fan
                unit: root.unit
            }
            PwmPoint {
                id: stopPoint
                color: fan.hasTemp ? "blue" : Qt.tint(graph.pal.light, Qt.rgba(0, 0, 1, 0.5))
                size: graph.fontSize
                unit: root.unit
                enabled: fan.hasTemp
                drag.maximumX: Math.min(background.scaleX(background.scaleTemp(maxPoint.x)-1), maxPoint.x-1)
                drag.minimumY: Math.max(background.scaleY(background.scalePwm(maxPoint.y)-1), maxPoint.y+1)
                x: fan.hasTemp ? background.scaleX(MoreMath.bound(minTemp, fan.minTemp, maxTemp)) - width/2 : -width/2
                y: fan.hasTemp ? background.scaleY(fan.minStop) - height/2 : -height/2
                drag.onActiveChanged: {
                    if (!drag.active) {
                        fan.minStop = Math.round(background.scalePwm(centerY));
                        fan.minTemp = Math.round(background.scaleTemp(centerX));
                        if (!fanOffCheckBox.checked) fan.minPwm = fan.minStop;
                    }
                }
                onPositionChanged: bgCanvas.requestPaint()
            }
            PwmPoint {
                id: maxPoint
                color: fan.hasTemp ? "red" : Qt.tint(graph.pal.light, Qt.rgba(1, 0, 0, 0.5))
                size: graph.fontSize
                unit: root.unit
                enabled: fan.hasTemp
                drag.minimumX: Math.max(background.scaleX(background.scaleTemp(stopPoint.x)+1), stopPoint.x+1)
                drag.maximumY: Math.min(background.scaleY(background.scalePwm(stopPoint.y)+1), stopPoint.y-1)
                x: fan.hasTemp ? background.scaleX(MoreMath.bound(minTemp, fan.maxTemp, maxTemp)) - width/2 : background.width - width/2
                y: fan.hasTemp ? background.scaleY(fan.maxPwm) - height/2 : -height/2
                drag.onActiveChanged: {
                    if (!drag.active) {
                        fan.maxPwm = Math.round(background.scalePwm(centerY));
                        fan.maxTemp = Math.round(background.scaleTemp(centerX));
                    }
                }
                onPositionChanged: bgCanvas.requestPaint()
            }
        }
    }

    ColumnLayout {
        property int padding: 10

        id: settingsArea
        anchors {
            left: parent.left
            leftMargin: padding
            right: parent.right
            rightMargin: padding
            bottom: parent.bottom
            bottomMargin: padding
        }
        visible: root.height >= nameField.height + height + 2*margin
        clip: true
        spacing: 2

        RowLayout {
            CheckBox {
                id: hasTempCheckBox
                text: i18n("Controlled by:")
                checked: fan.hasTemp
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                onCheckedChanged: {
                    fan.hasTemp = checked;
                    if (checked) {
                        fan.temp = loader.allTemps[tempBox.currentIndex];
                    }
                    bgCanvas.requestPaint();
                }
            }
            RowLayout {
                ComboBox {
                    id: tempBox
                    Layout.fillWidth: true
                    model: ArrayFunctions.namesWithPaths(loader.allTemps)
                    enabled: hasTempCheckBox.checked
                    onCurrentIndexChanged: {
                        if (hasTempCheckBox.checked)
                            fan.temp = loader.allTemps[currentIndex];
                    }
                }
            }
        }

        CheckBox {
            id: fanOffCheckBox
            text: i18n("Turn Fan off if temp < MINTEMP")
            enabled: hasTempCheckBox.checked
            checked: fan.minPwm == 0
            onCheckedChanged: {
                fan.minPwm = checked ? 0 : fan.minStop;
                bgCanvas.requestPaint();
            }
        }

        RowLayout {
            enabled: fanOffCheckBox.checked && fanOffCheckBox.enabled

            Label {
                text: i18n("Pwm value for fan to start:")
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                renderType: Text.NativeRendering
            }
            SpinBox {
                id: minStartInput
                Layout.fillWidth: true
                minimumValue: 1
                maximumValue: 100
                decimals: 1
                value: Math.round(fan.minStart / 2.55)
                suffix: i18n("%")
                onValueChanged: fan.minStart = Math.round(value * 2.55)
            }
        }

        RowLayout {
            visible: systemdCom

            Item {
                Layout.fillWidth: true
            }
            Button {
                property bool reactivateAfterTesting

                id: testButton
                text: fan.testing ? i18n("Abort test") : i18n("Test start and stop values")
                iconName: "dialog-password"
                anchors.right: parent.right
                onClicked: {
                    if (fan.testing) {
                        systemdCom.serviceActive = reactivateAfterTesting;
                        fan.abortTest();
                    } else {
                        reactivateAfterTesting = systemdCom.serviceActive;
                        systemdCom.serviceActive = false;
                        minStartInput.value = Qt.binding(function() { return Math.round(fan.minStart / 2.55) });
                        fan.test();
                    }
                }
            }
        }
    }
}
