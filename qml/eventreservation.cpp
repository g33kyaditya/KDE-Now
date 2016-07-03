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

#include "eventreservation.h"

#include <QtCore/QDebug>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusConnection>

EventReservation::EventReservation(QObject* parent): QObject(parent)
{
    QDBusInterface* interface = new QDBusInterface("org.kde.kdenow", "/Event");

    QDBusReply<QVariantMap> reply = interface->call("getMap");
    if (reply.isValid()) {
        qDebug() << "Valid Reply received from org.kde.kdenow /Event";
        qDebug() << reply.value();
    }
    else {
        qDebug() << "Did not receive a valid reply from org.kde.kdenow /Event";
        return;
    }

    m_map = reply.value();
}

QString EventReservation::reservationNumber() const
{
    return m_map.value("reservationNumber").toString();
}

QString EventReservation::name() const
{
    return m_map.value("name").toString();
}

QString EventReservation::eventName() const
{
    return m_map.value("eventName").toString();
}

QDateTime EventReservation::startDate() const
{
    return m_map.value("startDate").toDateTime();
}

QString EventReservation::location() const
{
    return m_map.value("location").toString();
}

QString EventReservation::streetAddress() const
{
    return m_map.value("streetAddress").toString();
}

QString EventReservation::addressLocality() const
{
    return m_map.value("addressLocality").toString();
}
