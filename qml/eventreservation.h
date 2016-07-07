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

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QVariantMap>

class EventReservation : public QObject
{
        Q_OBJECT
        Q_PROPERTY(QString reservationNumber READ reservationNumber)
        Q_PROPERTY(QString name READ name)
        Q_PROPERTY(QString eventName READ eventName)
        Q_PROPERTY(QDateTime startDate READ startDate)
        Q_PROPERTY(QString location READ location)
        Q_PROPERTY(QString streetAddress READ streetAddress)
        Q_PROPERTY(QString addressLocality READ addressLocality)

    public:
        explicit EventReservation(QObject* parent = 0);
        QString reservationNumber() const;
        QString name() const;
        QString eventName() const;
        QDateTime startDate() const;
        QString location() const;
        QString streetAddress() const;
        QString addressLocality() const;

    public Q_SLOTS:
        void setMap(QVariantMap& map);

    private:
        QVariantMap m_map;
};


#endif //EVENTRESERVATION_H