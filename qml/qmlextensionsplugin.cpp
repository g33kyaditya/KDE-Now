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

#include "qmlextensionsplugin.h"

#include "eventreservation.h"
#include "flightreservation.h"
#include "hotelreservation.h"
#include "restaurantreservation.h"

#include <QtQml/qqml.h>

void QmlExtensionsPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{

}

void QmlExtensionsPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<EventReservation>(uri, 0, 1, "EventReservation");
    qmlRegisterType<FlightReservation>(uri, 0, 1, "FlightReservation");
    qmlRegisterType<HotelReservation>(uri, 0, 1, "HotelReservation");
    qmlRegisterType<RestaurantReservation>(uri, 0, 1, "RestaurantReservation");
}

