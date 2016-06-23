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

#include "processor.h"
#include "parser.h"
#include "datamap.h"
#include "pluginsloader.h"
#include "singletonfactory.h"

#include <QtCore/QVariant>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>

Processor::Processor(QObject* parent): QObject(parent)
{

}

void Processor::process(KIMAP::MessagePtr messagePtr)
{
    m_messagePtr = messagePtr;

    /*if(isAlreadyDownloaded()) {
        //  TODO
        //  Either check from DB, or from a hash table of emails (preferable) since
        //  it minimises work
        //return;
    }*/
    extract();
}

bool Processor::isIdentifiedSchema(QList<QVariantMap>& listOfMap)
{
    QList<QVariantMap>::iterator it;
    QVariantMap map;
    for (it = listOfMap.begin(); it != listOfMap.end(); ++it) {
        map = *it;
        qDebug() << "Map in list = " << map;
        QString type = map["@type"].toString();
        if (!(type == "FlightReservation" || type == "EventReservation" ||
           type == "LodgingReservation" || type == "FoodEstablishmentReservation")) {
               it = listOfMap.erase(it);          //Remove all those, that are not needed
               it--;
        }
    }

    if (listOfMap.count() == 1)          //One valid Reservation JSON Data
        return true;

    return false;
}


void Processor::extract()
{
    KMime::Content *bodyContent = m_messagePtr->mainBodyPart("text/html");
    if (!bodyContent) {
        qDebug() << "Could not find text/html in mainBodyPart";
        return;
    }

    if (!bodyContent->contentType(false)->isHTMLText()) {      //Don't need to create header
        qDebug() << "ABORT!";
        return;
    }

    QByteArray htmlDoc = bodyContent->decodedContent();
    //qDebug() << htmlDoc;
    Parser parser;
    QList<QVariantMap> listOfMap;
    listOfMap = parser.parse(htmlDoc);
    //qDebug() << "\n\nlistOfMap = " << listOfMap;
    if (listOfMap.isEmpty()) {
        qDebug() << "listOfMap is empty";
        return;
    }

    if (!isIdentifiedSchema(listOfMap)) {
        //qDebug() << "Not a recognized schema (Flight, Hotel, Event, Food)Reservation";
        return;
    }

    m_map = listOfMap.at(0);
    //qDebug() << m_map;
    //Extracting all needed data from QVariantMap
    extractNeededData();
}

void Processor::extractNeededData()
{
    DataMap* map = SingletonFactory::instanceFor<DataMap>();
    map->setMap(m_map);
    PluginsLoader loader;
    loader.load();
}

void Processor::extractEventData()
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

void Processor::extractHotelData()
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

void Processor::extractRestaurantData()
{
    QString reservationNumber = m_map["reservationNumber"].toString();
    QString name = m_map["underName"].toMap().value("name").toString();
    QVariantMap reservationForMap = m_map["reservationFor"].toMap();

    QDateTime startTime = m_map["startTime"].toDateTime();
    int partySize = m_map["partySize"].toInt();
    Q_UNUSED(partySize)

    QVariantMap addressMap = reservationForMap["address"].toMap();
    QString restaurantName = reservationForMap["name"].toString();
    QString streetAddress = addressMap["streetAddress"].toString();
    QString addressLocality = addressMap["addressLocality"].toString();
    QString addressRegion = addressMap["addressRegion"].toString();
}
