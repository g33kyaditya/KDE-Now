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

#include "hotelreservation.h"
#include "src/singletonfactory.h"
#include "src/datamap.h"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <KCoreAddons/KPluginFactory>

K_PLUGIN_FACTORY_WITH_JSON( KdeNowPluginFactory,
                            "hotelreservation.json",
                            registerPlugin< HotelReservation >();
                          )


HotelReservation::HotelReservation(QObject* parent, const QVariantList& args)
                                    : AbstractReservationPlugin(parent, args)
{
    m_pluginName = "Hotel Data Extractor";
}

HotelReservation::~HotelReservation()
{

}

QString HotelReservation::plugin() const
{
    return m_pluginName;
}

void HotelReservation::start()
{
    m_map = SingletonFactory::instanceFor<DataMap>()->map();
    extract();
}

void HotelReservation::extract()
{
    QString reservationNumber = m_map["reservationNumber"].toString();
    QString name = m_map["underName"].toMap().value("name").toString();
    QVariantMap reservationForMap = m_map["reservationFor"].toMap();

    QDateTime checkinDate = m_map["checkinDate"].toDateTime();
    QDateTime checkoutDate = m_map["checkoutDate"].toDateTime();

    QString telephone = reservationForMap["telephone"].toString();
    QVariantMap addressMap = reservationForMap["address"].toMap();
    QString hotelName = reservationForMap["name"].toString();
    QString streetAddress = addressMap["streetAddress"].toString();
    QString addressLocality = addressMap["addressLocality"].toString();
    QString addressRegion = addressMap["addressRegion"].toString();
}

#include "hotelreservation.moc"

