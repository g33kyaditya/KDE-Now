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

#include "flightreservation.h"
#include "src/singletonfactory.h"
#include "src/datamap.h"
#include <KCoreAddons/KPluginFactory>

#include <QtCore/QDateTime>
#include <QtCore/QDebug>

void FlightReservation::setPluginName(const QString& pluginName)
{
    m_pluginName = pluginName;
}

QString FlightReservation::plugin() const
{
    return m_pluginName;
}

void FlightReservation::start()
{
    m_map = SingletonFactory::instanceFor<DataMap>()->map();
    QString type = m_map["@type"].toString();
    if (type == "FlightReservation") {
        setPluginName("FlightReservation");
        extract();
    }
    else {
        qDebug() << "Not a Flight Reservation";
        return;
    }
}

void FlightReservation::extract()
{
    QString reservationNumber = m_map["reservationNumber"].toString();
    QString name = m_map["underName"].toMap().value("name").toString();
    QVariantMap reservationForMap = m_map["reservationFor"].toMap();

    QString flightNameCode = reservationForMap["airline"].toMap().value("iataCode").toString();
    QString flightNumber = reservationForMap["flightNumber"].toString();
    QString flight = flightNameCode + flightNumber;

    QString departureAirportName = reservationForMap["departureAirport"].toMap().value("name").toString();
    QString departureAirportCode = reservationForMap["departureAirport"].toMap().value("iataCode").toString();
    QDateTime departureTime = reservationForMap["departureTime"].toDateTime();

    QString arrivalAirportName = reservationForMap["arrivalAirport"].toMap().value("name").toString();
    QString arrivalAirportCode = reservationForMap["arrivalAirport"].toMap().value("iataCode").toString();
    QDateTime arrivalTime = reservationForMap["arrivalTime"].toDateTime();
}

#include "flightreservation.moc"