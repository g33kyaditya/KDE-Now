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

#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QtCore/QVariantMap>

class DataHandler : public QObject
{
        Q_OBJECT

    public:
        explicit DataHandler(QObject* parent = 0);

    Q_SIGNALS:
        //These signals will be seen by Plasmoid's signal handler
        void eventDataReceived();
        void flightDataReceived();
        void hotelDataReceived();
        void restaurantDataReceived();

    public Q_SLOTS:
        void onEventMapReceived();
        void onFlightMapReceived();
        void onHotelMapReceived();
        void onRestaurantMapReceived();
};


#endif //DATAHANDLER_H
