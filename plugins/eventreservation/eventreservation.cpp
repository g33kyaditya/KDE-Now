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
    connect(this, &EventReservation::extractedData, this, &EventReservation::sendDataOverDBus);
    initDatabase();
    recordsInDatabase();
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
    QString reservationNumber = map["reservationNumber"].toString();
    QString name = map["underName"].toMap().value("name").toString();
    QVariantMap reservationForMap = map["reservationFor"].toMap();

    QString eventName = reservationForMap["name"].toString();
    QDateTime startDateTime = reservationForMap["startDate"].toDateTime();
    QString startDate = startDateTime.date().toString();
    QString startTime = startDateTime.time().toString("h:mm AP");

    QVariantMap addressMap = reservationForMap["location"].toMap().value("address").toMap();
    QString location = reservationForMap["location"].toMap().value("name").toString();
    QString streetAddress = addressMap["streetAddress"].toString();
    QString addressLocality = addressMap["addressLocality"].toString();

    QVariantMap requiredMap;
    requiredMap.insert("reservationNumber", reservationNumber);
    requiredMap.insert("name", name);
    requiredMap.insert("eventName", eventName);
    requiredMap.insert("startDate", startDate);
    requiredMap.insert("startTime", startTime);
    requiredMap.insert("location", location);
    requiredMap.insert("streetAddress", streetAddress);
    requiredMap.insert("addressLocality", addressLocality);

    emit extractedData(requiredMap);
}

void EventReservation::cacheData(QVariantMap& map)
{
    if (m_db.connectionName().isEmpty()) {
        initDatabase();
    }

    QSqlQuery updateQuery(m_db);
    QString queryString = "insert into Event values (:id, :reservationNumber, :name, :eventName, :startDate, :startTime, :location, :streetAddress, :addressLocality)";
    updateQuery.prepare(queryString);
    updateQuery.bindValue(":reservationNumber", map["reservationNumber"].toString());
    updateQuery.bindValue(":name", map["name"].toString());
    updateQuery.bindValue(":eventName", map["eventName"].toString());
    updateQuery.bindValue(":startDate", map["startDate"].toString());
    updateQuery.bindValue(":startTime", map["startTime"].toString());
    updateQuery.bindValue(":location", map["location"].toString());
    updateQuery.bindValue(":streetAddress", map["streetAddress"].toString());
    updateQuery.bindValue(":addressLocality", map["addressLocality"].toString());

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

void EventReservation::sendDataOverDBus(QVariantMap& map)
{
    QStringList keys, values;   // QTBUG-21577 : qdbusxml2cpp fails to parse QVariantMap parameter
                                //               for D-Bus signal

    keys.append("reservationNumber");
    keys.append("name");
    keys.append("eventName");
    keys.append("startDate");
    keys.append("startTime");
    keys.append("location");
    keys.append("streetAddress");
    keys.append("addressLocality");

    values.append(map["reservationNumber"].toString());
    values.append(map["name"].toString());
    values.append(map["eventName"].toString());
    values.append(map["startDate"].toString());
    values.append(map["startTime"].toString());
    values.append(map["streetAddress"].toString());
    values.append(map["addressLocality"].toString());

    emit update(keys, values);
}

void EventReservation::recordsInDatabase()
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
        m_listOfMapsInDatabase.append(map);
    }
    emit loadedEventPlugin();
}

void EventReservation::getDatabaseRecordsOverDBus()
{
    foreach(QVariantMap map, m_listOfMapsInDatabase) {
        sendDataOverDBus(map);
    }
}


#include "eventreservation.moc"
