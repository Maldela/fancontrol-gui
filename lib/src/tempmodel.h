/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2015  Malte Veerman maldela@halloarsch.de
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


#include <QtCore/QStringListModel>



namespace Fancontrol {


class Temp;

class TempModel : public QStringListModel
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> temps READ temps NOTIFY tempsChanged)
    Q_PROPERTY(int count READ count NOTIFY tempsChanged)

public:

    TempModel(QObject *parent = Q_NULLPTR);
    void setTemps(const QList<Temp *> &temps);
    void addTemps(const QList<Temp *> &temps);
    QList<QObject *> temps() const;
    int count() const { return m_temps.size(); }
    void setUnit(int unit) { if (unit != m_unit) { m_unit = unit; updateAll(); } }


protected:

    QString composeText(Temp *temp);


public slots:

    void updateTemp(Temp *temp);


protected slots:

    void updateAll();


private slots:

    void updateTemp();


signals:

    void tempsChanged();


private:

    QList<Temp *> m_temps;
    int m_unit;
};

}


#endif //TEMPMODEL_H
