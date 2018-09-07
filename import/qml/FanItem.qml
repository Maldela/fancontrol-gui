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


import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import org.kde.kirigami 2.4 as Kirigami
import Fancontrol.Qml 1.0 as Fancontrol
import "math.js" as MoreMath
import "units.js" as Units
import "colors.js" as Colors


Rectangle {
    property QtObject fan
    property QtObject systemdCom
    property QtObject tempModel
    property real minTemp: Fancontrol.Base.minTemp
    property real maxTemp: Fancontrol.Base.maxTemp
    property int margin: Kirigami.Units.smallSpacing
    property string unit: Fancontrol.Base.unit
    property real convertedMinTemp: Units.fromCelsius(minTemp, unit)
    property real convertedMaxTemp: Units.fromCelsius(maxTemp, unit)

    id: root
    color: "transparent"
    border.color: palette.windowText
    border.width: 2
    radius: Kirigami.Units.smallSpacing
    clip: false

    onConvertedMinTempChanged: {
        meshCanvas.requestPaint();
        curveCanvas.requestPaint();
    }
    onConvertedMaxTempChanged: {
        meshCanvas.requestPaint();
        curveCanvas.requestPaint();
    }
    onFanChanged: curveCanvas.requestPaint()

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
        visible: root.height >= height + margin * 2
        text: !!fan ? fan.name : ""
        color: palette.text
        horizontalAlignment: TextEdit.AlignLeft
        wrapMode: TextEdit.Wrap
        font.bold: true
        font.pointSize: 14
        selectByMouse: true

        onTextChanged: if (!!fan && fan.name != text) fan.name = text

        Connections {
            target: fan
            onNameChanged: if (fan.name != nameField.text) nameField.text = fan.name
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.IBeamCursor
            acceptedButtons: Qt.NoButton
        }
    }

    Item {
        id: graph

        property int fontSize: MoreMath.bound(8, height / 20 + 1, 16)
        property QtObject pal: !!fan ? fan.hasTemp ? palette : disabledPalette : disabledPalette
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
            width: MoreMath.maxWidth(children) + graph.fontSize

            Repeater {
                id: verticalRepeater

                model: graph.verticalScalaCount

                Label {
                    x: verticalScala.width - implicitWidth - graph.fontSize / 3
                    y: background.height - background.height / (graph.verticalScalaCount - 1) * index - graph.fontSize * 2 / 3
                    horizontalAlignment: Text.AlignRight
                    color: graph.pal.text
                    text: i18n("%1\%", index * (100 / (graph.verticalScalaCount - 1)))
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
                    x: background.scaleX(Units.toCelsius(graph.horIntervals[index], unit)) - width/2
                    y: horizontalScala.height / 2 - implicitHeight / 2
                    color: graph.pal.text
                    text: i18n("%1" + unit, graph.horIntervals[index])
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
                id: curveCanvas

                anchors.fill: parent
                anchors.margins: parent.border.width
                renderStrategy: Canvas.Cooperative

                property alias pal: background.pal

                onPaint: {
                    var c = curveCanvas.getContext("2d");
                    c.clearRect(0, 0, width, height);

                    if (!fan || !fan.hasTemp) {
                        return;
                    }

                    var gradient = c.createLinearGradient(0, 0, width, 0);
                    gradient.addColorStop(0, "rgb(0, 0, 255)");
                    gradient.addColorStop(1, "rgb(255, 0, 0)");
                    c.fillStyle = gradient;
                    c.lineWidth = graph.fontSize / 3;
                    c.strokeStyle = gradient;
                    c.lineJoin = "round";
                    c.beginPath();
                    if (fan.minPwm == 0) {
                        c.moveTo(stopPoint.centerX, height);
                    } else {
                        c.moveTo(0, stopPoint.centerY);
                        c.lineTo(stopPoint.centerX, stopPoint.centerY);
                    }
                    c.lineTo(stopPoint.centerX, stopPoint.centerY);
                    c.lineTo(maxPoint.centerX, maxPoint.centerY);
                    c.lineTo(width, maxPoint.centerY);
                    c.stroke();
                    c.lineTo(width, height);
                    if (fan.minPwm == 0) {
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
                }
            }
            Canvas {
                id: meshCanvas

                anchors.fill: parent
                anchors.margins: parent.border.width
                renderStrategy: Canvas.Cooperative

                property alias pal: background.pal

                onPaint: {
                    var c = meshCanvas.getContext("2d");
                    c.clearRect(0, 0, width, height);

                    //draw mesh
                    c.beginPath();
                    c.strokeStyle = Colors.setAlpha(pal.text, 0.3);

                    //horizontal lines
                    for (var i=0; i<=100; i+=20) {
                        var y = background.scaleY(i*2.55);
                        if (i != 0 && i != 100) {
                            for (var j=0; j<=width; j+=15) {
                                c.moveTo(j, y);
                                c.lineTo(Math.min(j+5, width), y);
                            }
                        }
                    }

                    //vertical lines
                    if (graph.horIntervals.length > 1) {
                        for (var i=1; i<graph.horIntervals.length; i++) {
                            var x = background.scaleX(Units.toCelsius(graph.horIntervals[i], unit));
                            for (var j=0; j<=height; j+=20) {
                                c.moveTo(x, j);
                                c.lineTo(x, Math.min(j+5, height));
                            }
                        }
                    }
                    c.stroke();
                }
            }
            StatusPoint {
                id: currentPwm
                size: graph.fontSize
                visible: background.contains(center) && !!fan && fan.hasTemp
                fan: root.fan
            }
            PwmPoint {
                id: stopPoint
                color: !!fan ? fan.hasTemp ? "blue" : Qt.tint(graph.pal.light, Qt.rgba(0, 0, 1, 0.5)) : "transparent"
                size: graph.fontSize
                visible: !!fan ? fan.hasTemp : false
                drag.maximumX: Math.min(background.scaleX(background.scaleTemp(maxPoint.x)-1), maxPoint.x-1)
                drag.minimumY: Math.max(background.scaleY(background.scalePwm(maxPoint.y)-1), maxPoint.y+1)
                x: !!fan && fan.hasTemp ? background.scaleX(MoreMath.bound(root.minTemp, fan.minTemp, root.maxTemp)) - width/2 : -width/2
                y: !!fan && fan.hasTemp ? background.scaleY(fan.minStop) - height/2 : -height/2
                temp: !!fan && fan.hasTemp ? drag.active ? background.scaleTemp(centerX) : fan.minTemp : root.minTemp
                pwm: !!fan && fan.hasTemp ? drag.active ? background.scalePwm(centerY) : fan.minStop : 255
                drag.onActiveChanged: {
                    if (!drag.active) {
                        fan.minStop = Math.round(background.scalePwm(centerY));
                        fan.minTemp = Math.round(background.scaleTemp(centerX));
                        if (!fanOffCheckBox.checked) fan.minPwm = fan.minStop;
                    }
                }
                onPositionChanged: {
                    var left = fanOffCheckBox.checked ? x : 0;
                    var width = maxPoint.x - left;
                    var height = y - maxPoint.y;
                    curveCanvas.markDirty(Qt.rect(left, maxPoint.y, width, height));
                }
            }
            PwmPoint {
                id: maxPoint
                color: !!fan ? fan.hasTemp ? "red" : Qt.tint(graph.pal.light, Qt.rgba(1, 0, 0, 0.5)) : "transparent"
                size: graph.fontSize
                visible: !!fan ? fan.hasTemp : false
                drag.minimumX: Math.max(background.scaleX(background.scaleTemp(stopPoint.x)+1), stopPoint.x+1)
                drag.maximumY: Math.min(background.scaleY(background.scalePwm(stopPoint.y)+1), stopPoint.y-1)
                x: !!fan && fan.hasTemp ? background.scaleX(MoreMath.bound(root.minTemp, fan.maxTemp, root.maxTemp)) - width/2 : background.width - width/2
                y: !!fan && fan.hasTemp ? background.scaleY(fan.maxPwm) - height/2 : -height/2
                temp: !!fan && fan.hasTemp ? drag.active ? background.scaleTemp(centerX) : fan.maxTemp : root.maxTemp
                pwm: !!fan && fan.hasTemp ? drag.active ? background.scalePwm(centerY) : fan.maxPwm : 255
                drag.onActiveChanged: {
                    if (!drag.active) {
                        fan.maxPwm = Math.round(background.scalePwm(centerY));
                        fan.maxTemp = Math.round(background.scaleTemp(centerX));
                    }
                }
                onPositionChanged: {
                    var width = x - stopPoint.x;
                    var height = stopPoint.y - y;
                    curveCanvas.markDirty(Qt.rect(stopPoint.x, y, width, height));
                }
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
                checked: !!fan ? fan.hasTemp : false
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                onCheckedChanged: {
                    if (!!fan) {
                        fan.hasTemp = checked;
                        if (checked && !!tempModel.temps[tempBox.currentIndex]) {
                            fan.temp = tempModel.temps[tempBox.currentIndex];
                        }
                        curveCanvas.requestPaint();
                    }
                }

                Connections {
                    target: root
                    onFanChanged: hasTempCheckBox.checked = !!fan ? fan.hasTemp : false
                }
                Connections {
                    target: fan
                    onHasTempChanged: hasTempCheckBox.checked = fan.hasTemp
                }
            }
            RowLayout {
                ComboBox {
                    id: tempBox
                    Layout.fillWidth: true
                    model: tempModel
                    textRole: "display"
                    enabled: hasTempCheckBox.checked
                    onCurrentIndexChanged: {
                        if (hasTempCheckBox.checked)
                            fan.temp = tempModel.temps[currentIndex];
                    }
                }

                Connections {
                    target: root
                    onFanChanged: if (!!fan && fan.hasTemp) tempBox.currentIndex = tempModel.temps.indexOf(fan.temp)
                }
                Connections {
                    target: fan
                    onTempChanged: if (fan.hasTemp) tempBox.currentIndex = tempModel.temps.indexOf(fan.temp)
                }
            }
        }

        CheckBox {
            id: fanOffCheckBox
            text: i18n("Turn Fan off if temp < MINTEMP")
            enabled: hasTempCheckBox.checked
            checked: !!fan ? fan.minPwm == 0 : false
            onCheckedChanged: {
                if (!!fan) {
                    fan.minPwm = checked ? 0 : fan.minStop;
                    curveCanvas.markDirty(Qt.rect(0, 0, stopPoint.x, stopPoint.y));
                }
            }

            Connections {
                target: root
                onFanChanged: if (!!fan) fanOffCheckBox.checked = fan.minPwm == 0
            }
            Connections {
                target: fan
                onMinPwmChanged: fanOffCheckBox.checked = fan.minPwm == 0
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
                minimumValue: 0
                maximumValue: 100
                decimals: 1
                value: !!fan ? Math.round(fan.minStart / 2.55) : 0
                suffix: i18n("%")
                onValueChanged: {
                    if (!!fan) {
                        fan.minStart = Math.round(value * 2.55)
                    }
                }

                Connections {
                    target: root
                    onFanChanged: if (!!fan) minStartInput.value = Math.round(fan.minStart / 2.55)
                }
                Connections {
                    target: fan
                    onMinStartChanged: minStartInput.value = Math.round(fan.minStart / 2.55)
                }
            }
        }

        RowLayout {
            visible: !!systemdCom

            Item {
                Layout.fillWidth: true
            }
            Button {
                id: testButton

                text: !!fan ? fan.testing ? i18n("Abort test") : i18n("Test start and stop values") : ""
                iconName: "dialog-password"
                Layout.alignment: Qt.AlignRight
                onClicked: {
                    if (fan.testing) {
                        fan.abortTest();
                    } else {
                        minStartInput.value = Qt.binding(function() { return Math.round(fan.minStart / 2.55) });
                        fan.test();
                    }
                }
            }
        }
    }
}
