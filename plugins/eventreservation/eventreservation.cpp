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
#include "src/datamap.h"
#include "eventadaptor.h"

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
    m_map = m_dataMap->map();
    if (m_map["@type"] == "EventReservation") {
        extract();
    }
    else {
        qDebug() << "Not an Event Reservation JSON";
        return;
    }
}

void EventReservation::extract()
{
    m_reservationNumber = m_map["reservationNumber"].toString();
    m_name = m_map["underName"].toMap().value("name").toString();
    QVariantMap reservationForMap = m_map["reservationFor"].toMap();

    m_eventName = reservationForMap["name"].toString();
    QDateTime startDateTime = reservationForMap["startDate"].toDateTime();
    m_startDate = startDateTime.date();
    m_startTime = startDateTime.time();

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
    updateQuery.bindValue(":startDate", m_startDate.toString());
    updateQuery.bindValue(":startTime", m_startTime.toString());
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
    m_dbusMap.insert("startDate", m_startDate.toString());
    m_dbusMap.insert("startTime", m_startTime.toString());
    m_dbusMap.insert("location", m_location);
    m_dbusMap.insert("streetAddress", m_streetAddress);
    m_dbusMap.insert("addressLocality", m_addressLocality);

    emit update();
}

QVariantMap EventReservation::getMap()
{
    return m_dbusMap;
}

#include "eventreservation.moc"
