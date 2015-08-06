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

function round(number, dec) {
    if (!dec) dec = 5;
    return Math.round(number*10*dec) / (10*dec);
}

function toCelsius(degrees, currentUnit) {
    var float = parseFloat(degrees);
    if (currentUnit == 1) { return float + 273.15; }
    if (currentUnit == 2) { return (float - 32) * 5 / 9; }
    return float;
}

function fromCelsius(degrees, newUnit) {
    var float = parseFloat(degrees);
    if (newUnit == 1) { return round(float - 273.15); }
    if (newUnit == 2) { return round(float * 9 / 5 + 32); }
    return round(float);
}
