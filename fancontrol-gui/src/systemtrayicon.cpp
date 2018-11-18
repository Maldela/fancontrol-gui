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

#include "systemtrayicon.h"

#include <KLocalizedString>

#include <QDebug>

SystemTrayIcon::SystemTrayIcon(QObject *parent) : KStatusNotifierItem(QStringLiteral("fancontrold.gui"), parent)
{
    setCategory(KStatusNotifierItem::ApplicationStatus);

    m_profilesMenu = contextMenu()->addMenu(i18n("Profiles"));
}

void SystemTrayIcon::setProfileModel(QStringListModel* model)
{
    qDebug() << "new model:" << model;

    if (m_profileModel == model)
        return;

    m_profileModel = model;
    emit profileModelChanged();

    if (!m_profileModel)
    {
        setProfiles(QStringList());
        return;
    }

    setProfiles(m_profileModel->stringList());

    connect(m_profileModel, &QStringListModel::dataChanged, this, [this] () { setProfiles(m_profileModel->stringList()); });
    connect(m_profileModel, &QStringListModel::rowsInserted, this, [this] () { setProfiles(m_profileModel->stringList()); });
    connect(m_profileModel, &QStringListModel::rowsRemoved, this, [this] () { setProfiles(m_profileModel->stringList()); });
    connect(m_profileModel, &QStringListModel::modelReset, this, [this] () { setProfiles(m_profileModel->stringList()); });
}

void SystemTrayIcon::setProfiles(const QStringList& profiles)
{
    qDebug() << "new profiles:" << profiles;

    m_profilesMenu->clear();

    for (const auto &profile : profiles)
    {
        const auto action = m_profilesMenu->addAction(profile);
        connect(action, &QAction::triggered, this, [this, profile] () { emit activateProfile(profile); });
    }
}
