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

#include <QtCore/QVariant>
#include <QtCore/QFile>
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
    if (!isIdentifiedSchema(listOfMap)) {
        qDebug() << "Not a recognized schema (Flight, Hotel, Event, Food)Reservation";
        return;
    }

    m_map = listOfMap.at(0);
    //Extracting all needed data from QVariantMap
    extractNeededData();
}

void Processor::extractNeededData()
{
    QString type = m_map["@type"].toString();
        if (type == "FlightReservation") {
            extractFlightData();
        }
        else if (type == "EventReservation") {
            extractEventData();
        }
        else if (type == "LodgingReservation") {
            extractHotelData();
        }
        else if (type == "FoodEstablishmentReservation") {
            extractRestaurantData();
        }
        else {
            qDebug() << "Unidentified Schema";
            return;
        }
}

void Processor::extractFlightData()
{
    QString reservationNumber = m_map["reservationNumber"].toString();
    QString name = m_map["underName"].toMap().value("name").toString();

    QString flightNameCode = m_map["reservationFor"].toMap().value("airline").toMap().value("iataCode").toString();
    QString flightNumber = m_map["reservationFor"].toMap().value("flightNumber").toString();
    QString flight = flightNameCode + flightNumber;

    QString departureAirportName = m_map["departureAirport"].toMap().value("name").toString();
    QString departureAirportCode = m_map["departureAirport"].toMap().value("iataCode").toString();
    QDateTime departureTime = m_map["departureTime"].toDateTime();

    QString arrivalAirportName = m_map["arrivalAirport"].toMap().value("name").toString();
    QString arrivalAirportCode = m_map["arrivalAirport"].toMap().value("iataCode").toString();
    QDateTime arrivalTime = m_map["arrivalTime"].toDateTime();
}

void Processor::extractEventData()
{
    QString reservationNumber = m_map["reservationNumber"].toString();
    QString name = m_map["underName"].toMap().value("name").toString();

    QString eventName = m_map["reservationFor"].toMap().value("name").toString();
    QDateTime startDate = m_map["reservationFor"].toMap().value("startDate").toDateTime();

    QVariantMap addressMap = m_map["reservationFor"].toMap().value("location").toMap().value("address").toMap();
    QString location = m_map["reservationFor"].toMap().value("location").toMap().value("name").toString();
    QString streetAddress = addressMap["streetAddress"].toString();
    QString addressLocality = addressMap["addressLocality"].toString();
}

void Processor::extractHotelData()
{
    QString reservationNumber = m_map["reservationNumber"].toString();
    QString name = m_map["underName"].toMap().value("name").toString();
    QString telephone = m_map["telephone"].toString();

    QDateTime checkinDate = m_map["checkinDate"].toDateTime();
    QDateTime checkoutDate = m_map["checkoutDate"].toDateTime();

    QVariantMap addressMap = m_map["reservationFor"].toMap().value("address").toMap();
    QString hotelName = m_map["reservationFor"].toMap().value("name").toString();
    QString streetAddress = addressMap["streetAddress"].toString();
    QString addressLocality = addressMap["addressLocality"].toString();
    QString addressRegion = addressMap["addressRegion"].toString();
}

void Processor::extractRestaurantData()
{
    QString reservationNumber = m_map["reservationNumber"].toString();
    QString name = m_map["underName"].toMap().value("name").toString();

    QDateTime startTime = m_map["startTime"].toDateTime();
    int partySize = m_map["partySize"].toInt();

    QVariantMap addressMap = m_map["reservationFor"].toMap().value("address").toMap();
    QString restaurantName = m_map["reservationFor"].toMap().value("name").toString();
    QString streetAddress = addressMap["streetAddress"].toString();
    QString addressLocality = addressMap["addressLocality"].toString();
    QString addressRegion = addressMap["addressRegion"].toString();
}
