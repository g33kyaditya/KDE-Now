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

#include <QtCore/QDebug>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusConnection>

FlightReservation::FlightReservation(QObject* parent): QObject(parent)
{
    QDBusConnection dbus = QDBusConnection::sessionBus();
    m_interface = new QDBusInterface("org.kde.kdenow", "/Flight");
    dbus.connect("org.kde.kdenow", "/Flight", "org.kde.kdenow.flight",
                 "update", this, SLOT(onMapReceived()));
}

void FlightReservation::onMapReceived()
{
    QDBusReply<QVariantMap> reply = m_interface->call("getMap");
    if (reply.isValid()) {
        qDebug() << "Valid Reply received from org.kde.kdenow /Flight";
        qDebug() << reply.value();
    }
    else {
        qDebug() << "Did not receive a valid reply from org.kde.kdenow /Flight";
        return;
    }
    m_map = reply.value();
}

QString FlightReservation::reservationNumber() const
{
    return m_map.value("reservationNumber").toString();
}

QString FlightReservation::name() const
{
    return m_map.value("name").toString();
}

QString FlightReservation::flight() const
{
    return m_map.value("flight").toString();
}

QString FlightReservation::departureAirportName() const
{
    return m_map.value("departureAirportName").toString();
}

QString FlightReservation::departureAirportCode() const
{
    return m_map.value("departureAirportCode").toString();
}

QDateTime FlightReservation::departureTime() const
{
    return m_map.value("departureTime").toDateTime();
}

QString FlightReservation::arrivalAirportName() const
{
    return m_map.value("arrivalAirportName").toString();
}

QString FlightReservation::arrivalAirportCode() const
{
    return m_map.value("arrivalAirportCode").toString();
}

QDateTime FlightReservation::arrivalTime() const
{
    return m_map.value("arrivalTime").toDateTime();
}
