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

function bound(floor, value, ceiling) {
    if (value >= floor) {
        if (value <= ceiling) return value;
        else return ceiling;
    }
    return floor;
}

function intervals(lower, upper, delta) {
    var array = [lower];
    for (var i=lower+1; i<upper; i++) {
        if (i % delta == 0) {
            array.push(i);
        }
    }
    array.push(upper);
    return array;
}
