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
    m_map = m_dataMap->map();
    if (m_map["@type"] == "FlightReservation") {
        extract();
    }
    else {
        qDebug() << "Not a Flight Reservation JSON";
        return;
    }
}

void FlightReservation::extract()
{
    m_reservationNumber = m_map["reservationNumber"].toString();
    m_name = m_map["underName"].toMap().value("name").toString();
    QVariantMap reservationForMap = m_map["reservationFor"].toMap();

    QString flightNameCode = reservationForMap["airline"].toMap().value("iataCode").toString();
    QString flightNumber = reservationForMap["flightNumber"].toString();
    m_flight = flightNameCode + flightNumber;

    m_departureAirportName = reservationForMap["departureAirport"].toMap().value("name").toString();
    m_departureAirportCode = reservationForMap["departureAirport"].toMap().value("iataCode").toString();
    QDateTime departureDateTime = reservationForMap["departureTime"].toDateTime();
    m_departureDate = departureDateTime.date();
    m_departureTime = departureDateTime.time();

    m_arrivalAirportName = reservationForMap["arrivalAirport"].toMap().value("name").toString();
    m_arrivalAirportCode = reservationForMap["arrivalAirport"].toMap().value("iataCode").toString();
    QDateTime arrivalDateTime = reservationForMap["arrivalTime"].toDateTime();
    m_arrivalDate = arrivalDateTime.date();
    m_arrivalTime = arrivalDateTime.time();

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
    updateQuery.bindValue(":departureDate", m_departureDate.toString());
    updateQuery.bindValue(":departureTime", m_departureTime.toString("h:mm AP"));
    updateQuery.bindValue(":arrivalAirportName", m_arrivalAirportName);
    updateQuery.bindValue(":arrivalAirportCode", m_arrivalAirportCode);
    updateQuery.bindValue(":arrivalDate", m_arrivalDate.toString());
    updateQuery.bindValue(":arrivalTime", m_arrivalTime.toString("h:mm AP"));

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
    m_dbusMap.insert("departureDate", m_departureDate.toString());
    m_dbusMap.insert("departureTime", m_departureTime.toString("h:mm AP"));
    m_dbusMap.insert("arrivalAirportName", m_arrivalAirportName);
    m_dbusMap.insert("arrivalAirportCode", m_arrivalAirportCode);
    m_dbusMap.insert("arrivalDate", m_arrivalDate.toString());
    m_dbusMap.insert("arrivalTime", m_arrivalTime.toString("h:mm AP"));

    emit update();
}

QVariantMap FlightReservation::getMap()
{
    return m_dbusMap;
}


#include "flightreservation.moc"
