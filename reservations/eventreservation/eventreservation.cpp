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
#include "src/singletonfactory.h"
#include "src/datamap.h"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <KCoreAddons/KPluginFactory>

K_PLUGIN_FACTORY_WITH_JSON( KdeNowPluginFactory,
                            "eventreservation.json",
                            registerPlugin< EventReservation >();
                          )


EventReservation::EventReservation(QObject* parent, const QVariantList& args)
                                    : AbstractReservationPlugin(parent, args)
{
    m_pluginName = "Event Data Extractor";
}

EventReservation::~EventReservation()
{

}

QString EventReservation::plugin() const
{
    return m_pluginName;
}

void EventReservation::start()
{
    m_map = SingletonFactory::instanceFor<DataMap>()->map();
    extract();
}

void EventReservation::extract()
{
    QString reservationNumber = m_map["reservationNumber"].toString();
    QString name = m_map["underName"].toMap().value("name").toString();
    QVariantMap reservationForMap = m_map["reservationFor"].toMap();

    QString eventName = reservationForMap["name"].toString();
    QDateTime startDate = reservationForMap["startDate"].toDateTime();

    QVariantMap addressMap = reservationForMap["location"].toMap().value("address").toMap();
    QString location = reservationForMap["location"].toMap().value("name").toString();
    QString streetAddress = addressMap["streetAddress"].toString();
    QString addressLocality = addressMap["addressLocality"].toString();
}

#include "eventreservation.moc"

