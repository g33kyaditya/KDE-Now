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
#include "eventadaptor.h"

#include <QtCore/QList>
#include <QtCore/QDateTime>
#include <QtCore/QStandardPaths>
#include <QtCore/QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <KCoreAddons/KPluginFactory>

K_PLUGIN_FACTORY_WITH_JSON( KdeNowPluginFactory,
                            "eventreservation.json",
                            registerPlugin< EventReservation >();
                          )


EventReservation::EventReservation(QObject* parent, const QVariantList& args)
                                    : AbstractReservationPlugin(parent, args)
{
    new EventAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/Event", this);
    dbus.registerService("org.kde.kdenow.event");
    m_pluginName = "eventDataExtractor";
    connect(this, &EventReservation::extractedData, this, &EventReservation::cacheData);
    connect(this, &EventReservation::extractedData, this, &EventReservation::setDBusData);
    initDatabase();
    getDataFromDatabase();
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
    foreach(QVariantMap map, m_maps) {
        if (map["@type"] == "EventReservation") {
            extract(map);
        }
    }
}

void EventReservation::extract(QVariantMap& map)
{
    m_reservationNumber = map["reservationNumber"].toString();
    m_name = map["underName"].toMap().value("name").toString();
    QVariantMap reservationForMap = map["reservationFor"].toMap();

    m_eventName = reservationForMap["name"].toString();
    QDateTime startDateTime = reservationForMap["startDate"].toDateTime();
    m_startDate = startDateTime.date().toString();
    m_startTime = startDateTime.time().toString("h:mm AP");

    QVariantMap addressMap = reservationForMap["location"].toMap().value("address").toMap();
    m_location = reservationForMap["location"].toMap().value("name").toString();
    m_streetAddress = addressMap["streetAddress"].toString();
    m_addressLocality = addressMap["addressLocality"].toString();

    emit extractedData();
}

void EventReservation::cacheData()
{
    if (m_db.connectionName().isEmpty()) {
        initDatabase();
    }

    QSqlQuery updateQuery(m_db);
    QString queryString = "insert into Event values (:id, :reservationNumber, :name, :eventName, :startDate, :startTime, :location, :streetAddress, :addressLocality)";
    updateQuery.prepare(queryString);
    updateQuery.bindValue(":reservationNumber", m_reservationNumber);
    updateQuery.bindValue(":name", m_name);
    updateQuery.bindValue(":eventName", m_eventName);
    updateQuery.bindValue(":startDate", m_startDate);
    updateQuery.bindValue(":startTime", m_startTime);
    updateQuery.bindValue(":location", m_location);
    updateQuery.bindValue(":streetAddress", m_streetAddress);
    updateQuery.bindValue(":addressLocality", m_addressLocality);

    if (!updateQuery.exec()) {
        qWarning() << "Unable to add entries into Database for Event Table";
        qWarning() << updateQuery.lastError();
    }
    else {
        qDebug() << "Updated Table Successfully";
    }
}

void EventReservation::initDatabase()
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
    QString queryString = "create table if not exists Event(id integer primary key autoincrement, reservationNumber varchar, name varchar, eventName varchar, startDate varchar, startTime varchar, location varchar, streetAddress varchar, addressLocality varchar)";

    if (!addQuery.exec(queryString)) {
        qWarning() << "Unable to create table";
        qWarning() << addQuery.lastError();
    }
    else {
        qDebug() << "Opened/Created successfully";
    }
}

void EventReservation::setDBusData()
{
    m_dbusMap.insert("reservationNumber", m_reservationNumber);
    m_dbusMap.insert("name", m_name);
    m_dbusMap.insert("eventName", m_eventName);
    m_dbusMap.insert("startDate", m_startDate);
    m_dbusMap.insert("startTime", m_startTime);
    m_dbusMap.insert("location", m_location);
    m_dbusMap.insert("streetAddress", m_streetAddress);
    m_dbusMap.insert("addressLocality", m_addressLocality);

    emit update();
}

void EventReservation::getDataFromDatabase()
{
    QSqlQuery dataQuery(m_db);
    QString queryString = "select * from Event";
    dataQuery.prepare(queryString);

    if (!dataQuery.exec()) {
        qWarning() << "Unable to fetch existing records from database";
        qWarning() << dataQuery.lastError();
    }
    else {
        qDebug() << "Fetched Records from Table Event Successfully";
    }

    QList< QVariantMap > listOfMapsInDatabase;
    while(dataQuery.next()) {
        QVariantMap map;
        map.insert("reservationNumber", dataQuery.value(1).toString());
        map.insert("name", dataQuery.value(2).toString());
        map.insert("eventName", dataQuery.value(3).toString());
        map.insert("startDate", dataQuery.value(4).toString());
        map.insert("startTime", dataQuery.value(5).toString());
        map.insert("location", dataQuery.value(6).toString());
        map.insert("streetAddress", dataQuery.value(7).toString());
        map.insert("addressLocality", dataQuery.value(8).toString());
        listOfMapsInDatabase.append(map);
    }

    foreach(QVariantMap map, listOfMapsInDatabase) {
        m_reservationNumber = map["reservationNumber"].toString();
        m_name = map["name"].toString();
        m_eventName = map["eventName"].toString();
        m_startDate = map["startDate"].toString();
        m_startTime = map["startTime"].toString();
        m_location = map["location"].toString();
        m_streetAddress = map["streetAddress"].toString();
        m_addressLocality = map["addressLocality"].toString();
        setDBusData();
    }
}


QVariantMap EventReservation::getMap()
{
    return m_dbusMap;
}

#include "eventreservation.moc"
