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


#ifndef WINDOWCONFIG_H
#define WINDOWCONFIG_H


#include <QtCore/QObject>


class QWindow;

class WindowConfig : public QObject
{
    Q_OBJECT
    
public:
    
    static WindowConfig *instance();
    
    Q_INVOKABLE void save(QWindow *window);
    Q_INVOKABLE void restore(QWindow *window);
    
    
private:
    
    WindowConfig(QObject *parent = Q_NULLPTR);
    ~WindowConfig() {}
    Q_DISABLE_COPY(WindowConfig)
    
    static WindowConfig *m_instance;
};

#endif //WINDOWCONFIG_H