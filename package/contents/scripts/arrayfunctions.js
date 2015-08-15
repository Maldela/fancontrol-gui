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

function names(array) {
    var names = [];
    for (var i=0; i<array.length; i++) {
        names[i] = array[i].name;
    }
    return names;
}

function namesWithPaths(array) {
    var namesWithPaths = [];
    for (var i=0; i<array.length; i++) {
        namesWithPaths[i] = array[i].name + "  (" + array[i].path + ")";
    }
    return namesWithPaths;
}

function labels(array) {
    var labels = [];
    for (var i=0; i<array.length; i++) {
        labels[i] = array[i].label;
    }
    return labels;
}

function labelsWithPaths(array) {
    var labelsWithPaths = [];
    for (var i=0; i<array.length; i++) {
        labelsWithPaths[i] = array[i].label + "  (" + array[i].path + ")";
    }
    return labelsWithPaths;
}

function maxProperty(array, prop) {
    var max = 0;
    for (var i=0; i<array.length; i++) {
        max = Math.max(max, array[i][prop]);
    }
    return max;
}

function minProperty(array, prop) {
    var min = 0;
    for (var i=0; i<array.length; i++) {
        min = Math.min(min, array[i][prop]);
    }
    return min;
}