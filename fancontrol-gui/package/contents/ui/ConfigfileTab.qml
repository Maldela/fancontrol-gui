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
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.3 as Kirigami
import Fancontrol.Qml 1.0 as Fancontrol


Kirigami.ScrollablePage {
    id: root

    readonly property QtObject loader: Fancontrol.Base.loader

    header: Label {
        text: !!loader && loader.configEqualToLoadedFile ? loader.configPath : i18n("New config")
    }

    TextEdit {
        width: root.width - root.leftPadding - root.rightPadding
        text: !!loader ? loader.config : ""
        readOnly: true
        color: Kirigami.Theme.textColor
        wrapMode: TextEdit.Wrap
    }
}
