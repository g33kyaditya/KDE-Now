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

#include "restaurantreservation.h"

#include <QtCore/QDebug>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusConnection>

RestaurantReservation::RestaurantReservation(QObject* parent): QObject(parent)
{
    QDBusInterface* interface = new QDBusInterface("org.kde.kdenow", "/Restaurant");

    QDBusReply<QVariantMap> reply = interface->call("getMap");
    if (reply.isValid()) {
        qDebug() << "Valid Reply received from org.kde.kdenow /Restaurant";
        qDebug() << reply.value();
    }
    else {
        qDebug() << "Did not receive a valid reply from org.kde.kdenow /Restaurant";
        return;
    }

    m_map = reply.value();
}

QString RestaurantReservation::reservationNumber() const
{
    return m_map.value("reservationNumber").toString();
}

QString RestaurantReservation::name() const
{
    return m_map.value("name").toString();
}

QDateTime RestaurantReservation::startTime() const
{
    return m_map.value("startTime").toDateTime();
}

int RestaurantReservation::partySize() const
{
    return m_map.value("partySize").toInt();
}

QString RestaurantReservation::restaurantName() const
{
    return m_map.value("restaurantName").toString();
}

QString RestaurantReservation::streetAddress() const
{
    return m_map.value("streetAddress").toString();
}

QString RestaurantReservation::addressLocality() const
{
    return m_map.value("addressLocality").toString();
}

QString RestaurantReservation::addressRegion() const
{
    return m_map.value("addressRegion").toString();
}
