/*
 * Copyright 2018  Malte Veerman <malte.veerman@gmail.com>
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
 */

#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H


#include <KNotifications/KStatusNotifierItem>

#include <QtCore/QStringListModel>
#include <QtWidgets/QMenu>


class SystemTrayIcon : public KStatusNotifierItem
{
    Q_OBJECT
    Q_PROPERTY(QStringListModel* profileModel READ profileModel WRITE setProfileModel NOTIFY profileModelChanged)

public:

    SystemTrayIcon(QObject *parent = nullptr);

    QStringListModel *profileModel() const { return m_profileModel; }
    void setProfileModel(QStringListModel *model);
    void setProfiles(const QStringList &profiles);


signals:

    void activateProfile(QString profile);
    void profileModelChanged();


private:

    QStringListModel *m_profileModel;
    QMenu *m_profilesMenu;
};

#endif // SYSTEMTRAYICON_H
