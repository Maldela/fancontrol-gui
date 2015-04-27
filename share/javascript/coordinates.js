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

function absoluteCoordinatesOf(item) {
    if (typeof item === "undefined") {
        item = this;
    }
    var offset = Qt.point(0, 0);
    while (item.parent) {
        offset.x += item.x;
        offset.y += item.y;
        item = item.parent
    }
    return offset;
}

function centerOf(item) {
    var p = Qt.point(0, 0);
    p.x = item.x + item.width / 2;
    p.y = item.y + item.height / 2;
    return p;
}
