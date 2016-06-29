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

#ifndef HOTELRESERVATION_H
#define HOTELRESERVATION_H

#include "src/abstractreservationplugin.h"

#include <QtCore/QDateTime>
#include <QtSql/QSqlDatabase>

class HotelReservation : public AbstractReservationPlugin
{
        Q_OBJECT
    public:
        explicit HotelReservation(QObject* parent = 0, const QVariantList& args = QVariantList());
        ~HotelReservation();
        void start();
        QString plugin() const;
        void extract();
        void initDatabase();

    Q_SIGNALS:
        void extractedData();

    public Q_SLOTS:
        void cacheData();

    private:
        QString m_pluginName;
        QVariantMap m_map;

        QSqlDatabase m_db;

        QString m_reservationNumber;
        QString m_name;
        QDateTime m_checkinDate;
        QDateTime m_checkoutDate;
        QString m_telephone;
        QString m_hotelName;
        QString m_streetAddress;
        QString m_addressLocality;
        QString m_addressRegion;
};

#endif //HOTELRESERVATION_H
