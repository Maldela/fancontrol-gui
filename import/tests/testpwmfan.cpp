/*
 * Copyright 2016  Malte Veerman <malte.veerman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "testpwmfan.h"


TestPwmFan::TestPwmFan(QString *pwmString, QString *enableString, QString *rpmString, uint index, Hwmon *parent) : PwmFan(index, parent)
{
    if (pwmString)
        m_pwmStream->setString(pwmString);

    if (enableString)
        m_enableStream->setString(enableString);

    if (rpmString)
        m_rpmStream->setString(rpmString);
}
