/*
 * Copyright (C) 2016  Aditya Dev Sharma <aditya.sharma156696@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef EVENTRESERVATION_H
#define EVENTRESERVATION_H

#include "src/abstractreservationplugin.h"

#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QVariantMap>
#include <QtSql/QSqlDatabase>

class EventReservation : public AbstractReservationPlugin
{
        Q_OBJECT
        Q_CLASSINFO("D-Bus Interface", "org.kde.kdenow.event");
    public:
        explicit EventReservation(QObject* parent = 0, const QVariantList& args = QVariantList());
        ~EventReservation();
        void start();
        QString plugin() const;
        void extract(QVariantMap& map);
        void initDatabase();
        void getDataFromDatabase();

    Q_SIGNALS:
        void extractedData();
        Q_SCRIPTABLE void update();

    public Q_SLOTS:
        QVariantMap getMap();

    private Q_SLOTS:
        void cacheData();
        void setDBusData();

    private:
        QString m_pluginName;

        QSqlDatabase m_db;

        QString m_reservationNumber;
        QString m_name;
        QString m_eventName;
        QDate m_startDate;
        QTime m_startTime;
        QString m_location;
        QString m_streetAddress;
        QString m_addressLocality;

        QVariantMap m_dbusMap;
};

#endif //EVENTRESERVATION_H
