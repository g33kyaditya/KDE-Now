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
    connect(this, &FlightReservation::extractedData, this, &FlightReservation::sendDataOverDBus);
    initDatabase();
    recordsInDatabase();
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
    QString reservationNumber = map["reservationNumber"].toString();
    QString name = map["underName"].toMap().value("name").toString();
    QVariantMap reservationForMap = map["reservationFor"].toMap();

    QString flightNameCode = reservationForMap["airline"].toMap().value("iataCode").toString();
    QString flightNumber = reservationForMap["flightNumber"].toString();
    QString flight = flightNameCode + flightNumber;

    QString departureAirportName = reservationForMap["departureAirport"].toMap().value("name").toString();
    QString departureAirportCode = reservationForMap["departureAirport"].toMap().value("iataCode").toString();
    QDateTime departureDateTime = reservationForMap["departureTime"].toDateTime();
    QString departureDate = departureDateTime.date().toString();
    QString departureTime = departureDateTime.time().toString("h:mm AP");

    QString arrivalAirportName = reservationForMap["arrivalAirport"].toMap().value("name").toString();
    QString arrivalAirportCode = reservationForMap["arrivalAirport"].toMap().value("iataCode").toString();
    QDateTime arrivalDateTime = reservationForMap["arrivalTime"].toDateTime();
    QString arrivalDate = arrivalDateTime.date().toString();
    QString arrivalTime = arrivalDateTime.time().toString("h:mm AP");

    QVariantMap requiredMap;
    requiredMap.insert("reservationNumber", reservationNumber);
    requiredMap.insert("name", name);
    requiredMap.insert("flight", flight);
    requiredMap.insert("departureAirportName", departureAirportName);
    requiredMap.insert("departureAirportCode", departureAirportCode);
    requiredMap.insert("departureDate", departureDate);
    requiredMap.insert("departureTime", departureTime);
    requiredMap.insert("arrivalAirportName", arrivalAirportName);
    requiredMap.insert("arrivalAirportCode", arrivalAirportCode);
    requiredMap.insert("arrivalDate", arrivalDate);
    requiredMap.insert("arrivalTime", arrivalTime);

    emit extractedData(requiredMap);
}

void FlightReservation::cacheData(QVariantMap& map)
{
    if (m_db.connectionName().isEmpty()) {
        initDatabase();
    }

    QSqlQuery updateQuery(m_db);
    QString queryString = "insert into Flight values (:id, :reservationNumber, :name, :flight, :departureAirportName, :departureAirportCode, :departureDate, :departureTime, :arrivalAirportName, :arrivalAirportCode, :arrivalDate, :arrivalTime)";
    updateQuery.prepare(queryString);
    updateQuery.bindValue(":reservationNumber", map["reservationNumber"].toString());
    updateQuery.bindValue(":name", map["name"].toString());
    updateQuery.bindValue(":flight", map["flight"].toString());
    updateQuery.bindValue(":departureAirportName", map["departureAirportName"].toString());
    updateQuery.bindValue(":departureAirportCode", map["departureAirportCode"].toString());
    updateQuery.bindValue(":departureDate", map["departureDate"].toString());
    updateQuery.bindValue(":departureTime", map["departureTime"].toString());
    updateQuery.bindValue(":arrivalAirportName", map["arrivalAirportName"].toString());
    updateQuery.bindValue(":arrivalAirportCode", map["arrivalAirportCode"].toString());
    updateQuery.bindValue(":arrivalDate", map["arrivalDate"].toString());
    updateQuery.bindValue(":arrivalTime", map["arrivalTime"].toString());

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

void FlightReservation::sendDataOverDBus(QVariantMap& map)
{
    QStringList keys, values;   // QTBUG-21577 : qdbusxml2cpp fails to parse QVariantMap parameter
                                //               for D-Bus signal

    keys.append("reservationNumber");
    keys.append("name");
    keys.append("flight");
    keys.append("departureAirportName");
    keys.append("departureAirportCode");
    keys.append("departureDate");
    keys.append("departureTime");
    keys.append("arrivalAirportName");
    keys.append("arrivalAirportCode");
    keys.append("arrivalDate");
    keys.append("arrivalTime");

    values.append(map["reservationNumber"].toString());
    values.append(map["name"].toString());
    values.append(map["flight"].toString());
    values.append(map["departureAirportName"].toString());
    values.append(map["departureAirportCode"].toString());
    values.append(map["departureDate"].toString());
    values.append(map["departureTime"].toString());
    values.append(map["arrivalAirportName"].toString());
    values.append(map["arrivalAirportCode"].toString());
    values.append(map["arrivalDate"].toString());
    values.append(map["arrivalTime"].toString());

    emit update(keys, values);
}

void FlightReservation::recordsInDatabase()
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
        m_listOfMapsInDatabase.append(map);
    }
    emit loadedFlightPlugin();
}

void FlightReservation::getDatabaseRecordsOverDBus()
{
    foreach(QVariantMap map, m_listOfMapsInDatabase) {
        sendDataOverDBus(map);
    }
}

#include "flightreservation.moc"
