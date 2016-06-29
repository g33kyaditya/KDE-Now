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

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
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
    m_pluginName = "flightDataExtractor";
    connect(this, &FlightReservation::extractedData, this, &FlightReservation::cacheData);
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
    m_map = SingletonFactory::instanceFor<DataMap>()->map();
    extract();
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
    m_departureTime = reservationForMap["departureTime"].toDateTime();

    m_arrivalAirportName = reservationForMap["arrivalAirport"].toMap().value("name").toString();
    m_arrivalAirportCode = reservationForMap["arrivalAirport"].toMap().value("iataCode").toString();
    m_arrivalTime = reservationForMap["arrivalTime"].toDateTime();

    emit extractedData();
}

void FlightReservation::cacheData()
{
    if (m_db.connectionName().isEmpty()) {
        initDatabase();
    }

    QSqlQuery updateQuery(m_db);
    QString queryString = "insert into Flight values (:id, :reservationNumber, :name, :flight, :departureAirportName, :departureAirportCode, :departureTime, :arrivalAirportName, :arrivalAirportCode, :arrivalTime)";
    updateQuery.prepare(queryString);
    updateQuery.bindValue(":reservationNumber", m_reservationNumber);
    updateQuery.bindValue(":name", m_name);
    updateQuery.bindValue(":flight", m_flight);
    updateQuery.bindValue(":departureAirportName", m_departureAirportName);
    updateQuery.bindValue(":departureAirportCode", m_departureAirportCode);
    updateQuery.bindValue(":departureTime", m_departureTime.toString());
    updateQuery.bindValue(":arrivalAirportName", m_arrivalAirportName);
    updateQuery.bindValue(":arrivalAirportCode", m_arrivalAirportCode);
    updateQuery.bindValue(":arrivalTime", m_arrivalTime.toString());

    if (!updateQuery.exec(queryString)) {
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
    m_db.setDatabaseName("kdenowdb");

    if (!m_db.open()) {
        qWarning() << "Unable to open database";
        qWarning() << m_db.lastError();
    }
    else {
        qDebug() << "Database opened successfully";
    }

    QSqlQuery addQuery(m_db);
    QString queryString = "create table if not exists Flight(id integer primary key autoincrement, reservationNumber varchar, name varchar, flight varchar, departureAirportName varchar, departureAirportCode varchar, departureTime varchar, arrivalAirportName varchar, arrivalAirportCode varchar, arrivalTime varchar)";

    if (!addQuery.exec(queryString)) {
        qWarning() << "Unable to create table";
        qWarning() << addQuery.lastError();
    }
    else {
        qDebug() << "Opened/Created successfully";
    }
}


#include "flightreservation.moc"
