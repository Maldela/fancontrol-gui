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
import "../scripts/coordinates.js" as Coordinates

Rectangle {
    property QtObject fan
    property QtObject loader
    property QtObject systemdCom
    property real minTemp: 20.0
    property real maxTemp: 100.0
    property int margin: 5
    property int minimizeDuration: 400
    property int unit: 0
    property bool minimizable: true

    id: root
    implicitWidth: 800
    implicitHeight: 600
    color: "transparent"
    border.color: "black"
    border.width: 2
    radius: 10
    clip: false
    state: fan.active ? "" : minimizable ? "minimized" : ""

    function update() {
        if (fan) {
            hasTempCheckBox.checked = Qt.binding(function() { return fan.hasTemp; })
            fanOffCheckBox.checked = Qt.binding(function() { return (fan.minPwm == 0); })
            minStartInput.text = Qt.binding(function() { return fan.minStart; })
            if (fan.hasTemp && loader) {
                tempBox.currentIndex = loader.allTemps.indexOf(fan.temp);
            }
        }
        canvas.requestPaint();
    }
    
    onFanChanged: update()
    onLoaderChanged: update()
    onUnitChanged: if (fan) canvas.requestPaint()
    onMinTempChanged: if (fan) canvas.requestPaint()
    onMaxTempChanged: if (fan) canvas.requestPaint()
    
    Connections {
        target: loader
        onConfigUrlChanged: update()
    }

    states: [
        State {
            name: "minimized"

            PropertyChanges {
                target: root
                height: header.height + 2*margin
            }
        },
        State {
            name: "maximized"

//            PropertyChanges {
//                target: root
//                height:
//            }
        }
    ]
    transitions: Transition {
        NumberAnimation {
            target: root
            property: "height"
            easing.amplitude: 1.5
            easing.type: Easing.InOutQuad
            duration: minimizeDuration
        }
    }

    SystemPalette {
        id: palette
    }

    RowLayout {
        id: header
        anchors {
            left: parent.left
            leftMargin: margin
            right: parent.right
            rightMargin: margin
            top: parent.top
            topMargin: margin
        }
        z: -1
        clip: true
        spacing: margin

        TextEdit {
            id: nameField
            Layout.alignment: Qt.AlignTop
            text: fan.name
            onTextChanged: fan.name = text;
            horizontalAlignment: TextEdit.AlignLeft
            wrapMode: TextEdit.Wrap
            font.bold: true
            font.pointSize: 14
            selectByMouse: true
            Layout.fillWidth: true

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.IBeamCursor
                acceptedButtons: Qt.NoButton
            }
        }

        Rectangle {
            id: collapseButton
            height: 16
            width: 16
            Layout.alignment: Qt.AlignTop
            color: collapseMouseArea.containsMouse ? "red" : "transparent"
            radius: width / 2
            visible: minimizable
            enabled: minimizable

            Label {
                anchors.fill: parent
                text: root.state == "minimized" ? "-" : "X"
                color: collapseMouseArea.containsMouse ? "black" : "red"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                id: collapseMouseArea
                anchors.fill: parent
                hoverEnabled: true
                acceptedButtons: Qt.LeftButton
                onClicked: fan.active = fan.active ? false : true
            }
        }
    }

    Canvas {
        property int fontSize: Math.max(9, Math.min(height / 25, 20))
        property int leftPadding: fontSize * 4
        property int rightPadding: fontSize * 2
        property int topPadding: fontSize
        property int bottomPadding: fontSize * 2
        property int plotWidth: width - leftPadding - rightPadding
        property int plotHeight: height - topPadding - bottomPadding
        property alias minTemp: root.minTemp
        property alias maxTemp: root.maxTemp

        id: canvas
        renderTarget: Canvas.FramebufferObject
        anchors {
            left: parent.left
            right: parent.right
            top: header.bottom
            bottom: settingsArea.top
        }
        opacity: state == "minimized" ? 0 : 1

        Behavior on opacity {
            NumberAnimation { duration: minimizeDuration / 2 }
        }

        Rectangle {
            property real unscaledTemp: fan.temp ? fan.temp.value : minTemp
            property real unscaledPwm: fan.pwm
            
            id: currentPwm
            x: parent.scaleX(unscaledTemp) - width/2
            y: parent.scaleY(unscaledPwm) - height/2
            width: canvas.fontSize
            height: width
            radius: width / 2
            color: "black"
            visible: parent.contains(Coordinates.centerOf(this)) && fan.hasTemp
            
            Behavior on unscaledTemp {
                SpringAnimation { 
//                     duration: 2000
                    epsilon: 0.1
                    spring: 1.0
                    damping: 0.5
                }
            }
            Behavior on unscaledPwm {
                SpringAnimation { 
//                     duration: 2000
                    epsilon: 0.1
                    spring: 1.0
                    damping: 0.5
                }
            }
        }
        PwmPoint {
            id: stopPoint
            color: "blue"
            size: canvas.fontSize
            drag.maximumX: Math.min(canvas.scaleX(canvas.scaleTemp(maxPoint.x)-1), maxPoint.x-1)
            drag.minimumY: Math.max(canvas.scaleY(canvas.scalePwm(maxPoint.y)-1), maxPoint.y+1)
            x: canvas.scaleX(MoreMath.bound(minTemp, fan.minTemp, maxTemp)) - width/2
            y: canvas.scaleY(fan.minStop) - height/2
            visible: parent.contains(Coordinates.centerOf(this)) && parent.height > 0
            drag.onActiveChanged: {
                if (!drag.active) {
                    fan.minStop = canvas.scalePwm(centerY);
                    fan.minTemp = canvas.scaleTemp(centerX);
                    if (!fanOffCheckBox.checked) fan.minPwm = fan.minStop;
                }
            }
        }
        PwmPoint {
            id: maxPoint
            color: "red"
            size: canvas.fontSize
            drag.minimumX: stopPoint.x
            drag.maximumY: stopPoint.y
            x: canvas.scaleX(MoreMath.bound(minTemp, fan.maxTemp, maxTemp)) - width/2
            y: canvas.scaleY(fan.maxPwm) - height/2
            visible: parent.contains(Coordinates.centerOf(this)) && parent.height > 0
            drag.onActiveChanged: {
                if (!drag.active) {
                    fan.maxPwm = canvas.scalePwm(centerY);
                    fan.maxTemp = canvas.scaleTemp(centerX);
                }
            }
        }

        function scaleX(temp) {
            var scaledX = (temp - minTemp) * plotWidth / (maxTemp - minTemp);
            return leftPadding + scaledX;
        }
        function scaleY(pwm) {
            var scaledY = pwm * plotHeight / 255;
            return height - bottomPadding - scaledY;
        }
        function scaleTemp(x) {
            var scaledTemp = (x - leftPadding) / plotWidth * (maxTemp - minTemp);
            return  minTemp + scaledTemp;
        }
        function scalePwm(y) {
            var scaledPwm = (y - topPadding) / plotHeight * 255;
            return 255 - scaledPwm;
        }

        onPaint: {
            var c = canvas.getContext("2d");
            c.font = canvas.fontSize + "px sans-serif";

            c.clearRect(0, 0, width, height);
            c.fillStyle = palette.base;
            c.fillRect(leftPadding, topPadding, plotWidth, plotHeight);

            var fillGradient = c.createLinearGradient(0, 0, width, 0);
            fillGradient.addColorStop(0, "rgb(0, 0, 255)");
            fillGradient.addColorStop(1, "rgb(255, 0, 0)");
            var strokeGradient = c.createLinearGradient(0, 0, width, 0);
            strokeGradient.addColorStop(0, "rgb(0, 0, 255)");
            strokeGradient.addColorStop(1, "rgb(255, 0, 0)");
            c.fillStyle = fillGradient;
            c.strokeStyle = strokeGradient;
            c.lineWidth = 2;
            c.lineJoin = "round";
            c.beginPath();
            if (fanOffCheckBox.checked) {
                c.moveTo(scaleX(minTemp), scaleY(0));
                c.lineTo(stopPoint.centerX, scaleY(0));
            } else {
                c.moveTo(scaleX(minTemp), stopPoint.centerY);
            }
            c.lineTo(stopPoint.centerX, stopPoint.centerY);
            c.lineTo(maxPoint.centerX, maxPoint.centerY);
            c.lineTo(scaleX(maxTemp), maxPoint.centerY);
            c.stroke();
            c.lineTo(scaleX(maxTemp), height - bottomPadding);
            c.lineTo(leftPadding, height - bottomPadding);
            c.fill();
            fillGradient = c.createLinearGradient(0, 0, 0, height);
            fillGradient.addColorStop(0, "rgba(127, 127, 127, 0.6)");
            fillGradient.addColorStop(1, "rgba(127, 127, 127, 0.9)");
            c.fillStyle = fillGradient;
            c.fill();
            c.closePath();

            c.textAlign = "right";
            c.textBaseline = "middle";
            c.strokeStyle = palette.dark;
            c.fillStyle = palette.dark;
            c.lineWidth = 1;
            c.strokeRect(leftPadding-0.5, topPadding-0.5, plotWidth+0.5, plotHeight+1.5);
            for (var i=0; i<=100; i+=20) {
                var y = scaleY(i*2.55);
                c.fillText(i + '%', leftPadding - 2, y);
                if (i != 0 && i != 100) {
                    for (var j=leftPadding; j<=width-rightPadding; j+=15) {
                        c.moveTo(j, y);
                        c.lineTo(Math.min(j+5, width-rightPadding), y);
                    }
                    c.stroke();
                }
            }
            c.textAlign = "center";
            c.textBaseline = "top";
            var convertedMinTemp = Units.fromCelsius(minTemp, unit);
            var convertedMaxTemp = Units.fromCelsius(maxTemp, unit);
            for (i=convertedMinTemp; i<convertedMaxTemp; i+= 10) {
                var x = scaleX(Units.toCelsius(i, unit));
                c.fillText(i + '°', x, topPadding+plotHeight+fontSize/2);
                if (i != convertedMinTemp) {
                    for (var j=scaleY(255); j<=scaleY(0); j+=20) {
                        c.moveTo(x, j);
                        c.lineTo(x, Math.min(j+5, width-rightPadding));
                    }
                    c.stroke();
                }
            }
            c.fillText(convertedMaxTemp + '°', scaleX(maxTemp), topPadding+plotHeight+fontSize/2);
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
        visible: root.height >= header.height + height + 2*margin
        opacity: canvas.opacity
        clip: true
        spacing: 2

        RowLayout {
            CheckBox {
                id: hasTempCheckBox
                text: i18n("Controlled by:")
                checked: fan.hasTemp
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                onCheckedChanged: fan.hasTemp = checked
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
                canvas.requestPaint();
            }
        }

        RowLayout {
            enabled: fanOffCheckBox.checked && fanOffCheckBox.enabled

            Label {
                text: i18n("Pwm value for fan to start:")
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                renderType: Text.NativeRendering
            }
            OptionInput {
                id: minStartInput
                Layout.fillWidth: true
                text: fan.minStart
                onTextChanged: fan.minStart = parseInt(text)
            }
        }

        RowLayout {
            visible: systemdCom
            enabled: fanOffCheckBox.checked && fanOffCheckBox.enabled
            
            Label {
                text: i18n("Test start and stop values")
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                renderType: Text.NativeRendering
            }
            Item {
                Layout.fillWidth: true
            }
            Button {
                property bool reactivateAfterTesting

                id: testButton
                text: fan.testing? i18n("Abort") : i18n("Test")
                anchors.right: parent.right
                onClicked: {
                    if (fan.testing) {
                        fan.abortTest();
                        systemdCom.serviceActive = true;
                    } else {
                        reactivateAfterTesting = systemdCom.serviceActive;
                        systemdCom.serviceActive = false;
                        minStartInput.text = Qt.binding(function() { return fan.minStart });
                        fan.test();
                    }
                }
                
                Connections {
                    target: fan
                    onTestingChanged: if (!fan.testing) systemdCom.serviceActive = testButton.reactivateAfterTesting
                }
            }
        }
    }
}
