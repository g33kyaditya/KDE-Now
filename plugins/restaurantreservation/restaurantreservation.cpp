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

#include "restaurantreservation.h"
#include "src/singletonfactory.h"
#include "restaurantadaptor.h"

#include <QtCore/QDateTime>
#include <QtCore/QStandardPaths>
#include <QtCore/QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <KCoreAddons/KPluginFactory>

K_PLUGIN_FACTORY_WITH_JSON( KdeNowPluginFactory,
                            "restaurantreservation.json",
                            registerPlugin< RestaurantReservation >();
                          )


RestaurantReservation::RestaurantReservation(QObject* parent, const QVariantList& args)
                                    : AbstractReservationPlugin(parent, args)
{
    new RestaurantAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/Restaurant", this);
    dbus.registerService("org.kde.kdenow.restaurant");
    m_pluginName = "restaurantDataExtractor";
    connect(this, &RestaurantReservation::extractedData, this, &RestaurantReservation::cacheData);
    connect(this, &RestaurantReservation::extractedData, this, &RestaurantReservation::setDBusData);
    initDatabase();
    getDataFromDatabase();
}

RestaurantReservation::~RestaurantReservation()
{

}

QString RestaurantReservation::plugin() const
{
    return m_pluginName;
}

void RestaurantReservation::start()
{
    foreach(QVariantMap map, m_maps) {
        if (map["@type"] == "FoodEstablishmentReservation") {
            extract(map);
        }
    }
}

void RestaurantReservation::extract(QVariantMap& map)
{
    m_reservationNumber = map["reservationNumber"].toString();
    m_name = map["underName"].toMap().value("name").toString();
    QVariantMap reservationForMap = map["reservationFor"].toMap();

    QDateTime startDateTime = map["startTime"].toDateTime();
    m_startDate = startDateTime.date().toString();
    m_startTime = startDateTime.time().toString("h:mm AP");
    m_partySize = map["partySize"].toInt();

    QVariantMap addressMap = reservationForMap["address"].toMap();
    m_restaurantName = reservationForMap["name"].toString();
    m_streetAddress = addressMap["streetAddress"].toString();
    m_addressLocality = addressMap["addressLocality"].toString();
    m_addressRegion = addressMap["addressRegion"].toString();

    emit extractedData();
}

void RestaurantReservation::cacheData()
{
    if (m_db.connectionName().isEmpty()) {
        initDatabase();
    }

    QSqlQuery updateQuery(m_db);
    QString queryString = "insert into Restaurant values (:id, :reservationNumber, :name, :startDate, :startTime, :partySize, :restaurantName, :streetAddress, :addressLocality, :addressRegion)";
    updateQuery.prepare(queryString);
    updateQuery.bindValue(":reservationNumber", m_reservationNumber);
    updateQuery.bindValue(":name", m_name);
    updateQuery.bindValue(":startDate", m_startDate);
    updateQuery.bindValue(":startTime", m_startTime);
    updateQuery.bindValue(":partySize", m_partySize);
    updateQuery.bindValue(":restaurantName", m_restaurantName);
    updateQuery.bindValue(":streetAddress", m_streetAddress);
    updateQuery.bindValue(":addressLocality", m_addressLocality);
    updateQuery.bindValue(":addressRegion", m_addressRegion);

    if (!updateQuery.exec()) {
        qWarning() << "Unable to add entries into Database for Restaurant Table";
        qWarning() << updateQuery.lastError();
    }
    else {
        qDebug() << "Updated Table Successfully";
    }
}

void RestaurantReservation::initDatabase()
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
    QString queryString = "create table if not exists Restaurant(id integer primary key autoincrement, reservationNumber varchar, name varchar, startDate varchar, startTime varchar, partySize integer, restaurantName varchar, streetAddress varchar, addressLocality varchar, addressRegion varchar)";

    if (!addQuery.exec(queryString)) {
        qWarning() << "Unable to create table";
        qWarning() << addQuery.lastError();
    }
    else {
        qDebug() << "Opened/Created successfully";
    }
}

void RestaurantReservation::setDBusData()
{
    m_dbusData.insert("reservationNumber", m_reservationNumber);
    m_dbusData.insert("name", m_name);
    m_dbusData.insert("startDate", m_startDate);
    m_dbusData.insert("startTime", m_startTime);
    m_dbusData.insert("partySize", m_partySize);
    m_dbusData.insert("restaurantName", m_restaurantName);
    m_dbusData.insert("streetAddress", m_streetAddress);
    m_dbusData.insert("addressLocality", m_addressLocality);
    m_dbusData.insert("addressRegion", m_addressRegion);

    emit update(m_dbusData);
}

void RestaurantReservation::getDataFromDatabase()
{
    QSqlQuery dataQuery(m_db);
    QString queryString = "select * from Restaurant";
    dataQuery.prepare(queryString);

    if (!dataQuery.exec()) {
        qWarning() << "Unable to fetch existing records from database";
        qWarning() << dataQuery.lastError();
    }
    else {
        qDebug() << "Fetched Records from Table Restaurant Successfully";
    }

    QList< QVariantMap > listOfMapsInDatabase;
    while(dataQuery.next()) {
        QVariantMap map;
        map.insert("reservationNumber", dataQuery.value(1).toString());
        map.insert("name", dataQuery.value(2).toString());
        map.insert("startDate", dataQuery.value(3).toString());
        map.insert("startTime", dataQuery.value(4).toString());
        map.insert("partySize", dataQuery.value(5).toInt());
        map.insert("restaurantName", dataQuery.value(6).toString());
        map.insert("streetAddress", dataQuery.value(7).toString());
        map.insert("addressLocality", dataQuery.value(8).toString());
        map.insert("addressRegion", dataQuery.value(9).toString());
        listOfMapsInDatabase.append(map);
    }
    qDebug() << listOfMapsInDatabase << "\n";
    foreach(QVariantMap map, listOfMapsInDatabase) {
        m_reservationNumber = map["reservationNumber"].toString();
        m_name = map["name"].toString();
        m_startDate = map["startDate"].toString();
        m_startTime = map["startTime"].toString();
        m_partySize = map["partySize"].toInt();
        m_restaurantName = map["restaurantName"].toString();
        m_streetAddress = map["streetAddress"].toString();
        m_addressLocality = map["addressLocality"].toString();
        m_addressRegion = map["addressRegion"].toString();
        setDBusData();
    }
}

#include "restaurantreservation.moc"
