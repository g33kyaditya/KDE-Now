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
#include "flightadaptor.h"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QStandardPaths>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <KCoreAddons/KPluginFactory>
#include <KConfigCore/KSharedConfig>
#include <KConfigCore/KConfigGroup>

K_PLUGIN_FACTORY_WITH_JSON( KdeNowPluginFactory,
                            "flightreservation.json",
                            registerPlugin< FlightReservation >();
                          )


FlightReservation::FlightReservation(QObject* parent, const QVariantList& args)
                                    : AbstractReservationPlugin(parent, args)
{
    new FlightAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/Flight", this);
    dbus.registerService("org.kde.kdenow.flight");
    m_pluginName = "flightDataExtractor";
    connect(this, &FlightReservation::extractedData, this, &FlightReservation::cacheData);
    connect(this, &FlightReservation::extractedData, this, &FlightReservation::setDBusData);
    initDatabase();
    getDataFromDatabase();
}

FlightReservation::~FlightReservation()
{

}

QString FlightReservation::plugin() const
{
    return m_pluginName;
}

void FlightReservation::start()
{
    foreach(QVariantMap map, m_maps) {
        if (map["@type"] == "FlightReservation") {
            extract(map);
        }
    }
}

void FlightReservation::extract(QVariantMap& map)
{
    m_reservationNumber = map["reservationNumber"].toString();
    m_name = map["underName"].toMap().value("name").toString();
    QVariantMap reservationForMap = map["reservationFor"].toMap();

    QString flightNameCode = reservationForMap["airline"].toMap().value("iataCode").toString();
    QString flightNumber = reservationForMap["flightNumber"].toString();
    m_flight = flightNameCode + flightNumber;

    m_departureAirportName = reservationForMap["departureAirport"].toMap().value("name").toString();
    m_departureAirportCode = reservationForMap["departureAirport"].toMap().value("iataCode").toString();
    QDateTime departureDateTime = reservationForMap["departureTime"].toDateTime();
    m_departureDate = departureDateTime.date().toString();
    m_departureTime = departureDateTime.time().toString("h:mm AP");

    m_arrivalAirportName = reservationForMap["arrivalAirport"].toMap().value("name").toString();
    m_arrivalAirportCode = reservationForMap["arrivalAirport"].toMap().value("iataCode").toString();
    QDateTime arrivalDateTime = reservationForMap["arrivalTime"].toDateTime();
    m_arrivalDate = arrivalDateTime.date().toString();
    m_arrivalTime = arrivalDateTime.time().toString("h:mm AP");

    emit extractedData();
}

void FlightReservation::cacheData()
{
    if (m_db.connectionName().isEmpty()) {
        initDatabase();
    }

    QSqlQuery updateQuery(m_db);
    QString queryString = "insert into Flight values (:id, :reservationNumber, :name, :flight, :departureAirportName, :departureAirportCode, :departureDate, :departureTime, :arrivalAirportName, :arrivalAirportCode, :arrivalDate, :arrivalTime)";
    updateQuery.prepare(queryString);
    updateQuery.bindValue(":reservationNumber", m_reservationNumber);
    updateQuery.bindValue(":name", m_name);
    updateQuery.bindValue(":flight", m_flight);
    updateQuery.bindValue(":departureAirportName", m_departureAirportName);
    updateQuery.bindValue(":departureAirportCode", m_departureAirportCode);
    updateQuery.bindValue(":departureDate", m_departureDate);
    updateQuery.bindValue(":departureTime", m_departureTime);
    updateQuery.bindValue(":arrivalAirportName", m_arrivalAirportName);
    updateQuery.bindValue(":arrivalAirportCode", m_arrivalAirportCode);
    updateQuery.bindValue(":arrivalDate", m_arrivalDate);
    updateQuery.bindValue(":arrivalTime", m_arrivalTime);

    if (!updateQuery.exec()) {
        qWarning() << "Unable to add entries into Database for Flight Table";
        qWarning() << updateQuery.lastError();
    }
    else {
        qDebug() << "Updated Table Successfully";
    }
}

void FlightReservation::initDatabase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", m_pluginName);
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    m_db.setDatabaseName(path);

    if (!m_db.open()) {
        qWarning() << "Unable to open database";
        qWarning() << m_db.lastError();
    }
    else {
        qDebug() << "Database opened successfully";
    }

    QSqlQuery addQuery(m_db);
    QString queryString = "create table if not exists Flight(id integer primary key autoincrement, reservationNumber varchar, name varchar, flight varchar, departureAirportName varchar, departureAirportCode varchar, departureDate varchar, departureTime varchar, arrivalAirportName varchar, arrivalAirportCode varchar, arrivalDate varchar, arrivalTime varchar)";

    if (!addQuery.exec(queryString)) {
        qWarning() << "Unable to create table";
        qWarning() << addQuery.lastError();
    }
    else {
        qDebug() << "Opened/Created successfully";
    }
}

void FlightReservation::setDBusData()
{
    m_dbusMap.insert("reservationNumber", m_reservationNumber);
    m_dbusMap.insert("name", m_name);
    m_dbusMap.insert("flight", m_flight);
    m_dbusMap.insert("departureAirportName", m_departureAirportName);
    m_dbusMap.insert("departureAirportCode", m_departureAirportCode);
    m_dbusMap.insert("departureDate", m_departureDate);
    m_dbusMap.insert("departureTime", m_departureTime);
    m_dbusMap.insert("arrivalAirportName", m_arrivalAirportName);
    m_dbusMap.insert("arrivalAirportCode", m_arrivalAirportCode);
    m_dbusMap.insert("arrivalDate", m_arrivalDate);
    m_dbusMap.insert("arrivalTime", m_arrivalTime);

    emit update();
}

void FlightReservation::getDataFromDatabase()
{
    QSqlQuery dataQuery(m_db);
    QString queryString = "select * from Flight";
    dataQuery.prepare(queryString);

    if (!dataQuery.exec()) {
        qWarning() << "Unable to fetch existing records from database";
        qWarning() << dataQuery.lastError();
    }
    else {
        qDebug() << "Fetched Records from Table Flight Successfully";
    }

    QList< QVariantMap > listOfMapsInDatabase;
    while(dataQuery.next()) {
        QVariantMap map;
        map.insert("reservationNumber", dataQuery.value(1).toString());
        map.insert("name", dataQuery.value(2).toString());
        map.insert("flight", dataQuery.value(3).toString());
        map.insert("departureAirportName", dataQuery.value(4).toString());
        map.insert("departureAirportCode", dataQuery.value(5).toString());
        map.insert("departureDate",dataQuery.value(6).toString());
        map.insert("departureTime", dataQuery.value(7).toString());
        map.insert("arrivalAirportName", dataQuery.value(8).toString());
        map.insert("arrivalAirportCode", dataQuery.value(9).toString());
        map.insert("arrivalDate", dataQuery.value(10).toString());
        map.insert("arrivalTime", dataQuery.value(11).toString());
        listOfMapsInDatabase.append(map);
    }

    foreach(QVariantMap map, listOfMapsInDatabase) {
        m_reservationNumber = map["reservationNumber"].toString();
        m_name = map["name"].toString();
        m_flight = map["flight"].toString();
        m_departureAirportName = map["departureAirportName"].toString();
        m_departureAirportCode = map["departureAirportCode"].toString();
        m_departureDate = map["departureDate"].toString();
        m_departureTime = map["departureTime"].toString();
        m_arrivalAirportName = map["arrivalAirportName"].toString();
        m_arrivalAirportCode = map["arrivalAirportCode"].toString();
        m_arrivalDate = map["arrivalDate"].toString();
        m_arrivalTime = map["arrivalTime"].toString();
        setDBusData();
    }
}

QVariantMap FlightReservation::getMap()
{
    return m_dbusMap;
}


#include "flightreservation.moc"
