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

#ifndef GUI_H
#define GUI_H

#include <QObject>

#include "../lib/src/loader.h"

#ifndef NO_SYSTEMD
#include "../lib/src/systemdcommunicator.h"

#define SYSTEMD_BOOL true
#else
#define SYSTEMD_BOOL false
#endif

class GUI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Loader* loader READ loader CONSTANT)
    
#ifndef NO_SYSTEMD
    Q_PROPERTY(SystemdCommunicator* systemdCom READ systemdCommunicator CONSTANT)
#endif

public:
    
    GUI(QObject *parent = Q_NULLPTR);
    
    Loader *loader() const { return m_loader; }
    
    Q_INVOKABLE bool hasSystemdCommunicator() const { return SYSTEMD_BOOL; }
    
#ifndef NO_SYSTEMD
    SystemdCommunicator *systemdCommunicator() const { return m_com; }
#endif

    
protected:
    
    Loader *const m_loader = new Loader(this);
    
#ifndef NO_SYSTEMD
    SystemdCommunicator *const m_com = new SystemdCommunicator(this);
#endif
    
};

#endif // GUI_H
