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


Dialog {
    id: dialog

    title: i18n("Error")
    width: Math.min(text.implicitWidth + Kirigami.Units.smallSpacing * 2, parent.width - Kirigami.Units.largeSpacing * 2)
    standardButtons: Dialog.Close
    onRejected: close()

    Label {
        id: text

        anchors.centerIn: parent
        text: ""
        wrapMode: Text.Wrap
        width: parent.width - Kirigami.Units.smallSpacing * 2
    }

    Connections {
        target: Fancontrol.Base
        onCriticalError: {
            text.text = Fancontrol.Base.error;
            dialog.open();
        }
    }
}
