/*
 * Copyright 2015  Malte Veerman <malte.veerman@gmail.com>
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


#ifndef TEMPMODEL_H
#define TEMPMODEL_H


#include <QtCore/QAbstractListModel>



namespace Fancontrol {


class Temp;

class TempModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int length READ rowCount NOTIFY tempsChanged)

public:

    enum Roles
    {
        DisplayRole,
        ObjectRole
    };
    Q_ENUM(Roles)

    TempModel(QObject *parent = Q_NULLPTR);
    void setTemps(const QList<Temp *> &temps);
    void addTemps(QList<Temp *> newTemps);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override { Q_UNUSED(parent) return m_temps.size(); }
    virtual QVariant data(const QModelIndex &index, int role = DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    void updateTemp(Temp *temp);
    void setUnit(const QString &unit) { if (unit != m_unit) { m_unit = unit; updateAll(); } }
    Q_INVOKABLE QObject *temp(int index) const;
    Q_INVOKABLE int indexOf(QObject *temp) const;


protected:

    void updateAll();


signals:

    void tempsChanged();


private:

    void updateTemp();

    QList<Temp *> m_temps;
    QString m_unit;
};

}


#endif //TEMPMODEL_H
