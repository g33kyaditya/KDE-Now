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

#include "hotelreservation.h"
#include "src/singletonfactory.h"
#include "hoteladaptor.h"

#include <QtCore/QDateTime>
#include <QtCore/QStandardPaths>
#include <QtCore/QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <KCoreAddons/KPluginFactory>

K_PLUGIN_FACTORY_WITH_JSON( KdeNowPluginFactory,
                            "hotelreservation.json",
                            registerPlugin< HotelReservation >();
                          )


HotelReservation::HotelReservation(QObject* parent, const QVariantList& args)
                                    : AbstractReservationPlugin(parent, args)
{
    new HotelAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/Hotel", this);
    dbus.registerService("org.kde.kdenow.hotel");
    m_pluginName = "hotelDataExtractor";
    connect(this, &HotelReservation::extractedData, this, &HotelReservation::cacheData);
    connect(this, &HotelReservation::extractedData, this, &HotelReservation::setDBusData);
    initDatabase();
    getDataFromDatabase();
}

HotelReservation::~HotelReservation()
{

}

QString HotelReservation::plugin() const
{
    return m_pluginName;
}

void HotelReservation::start()
{
    foreach(QVariantMap map, m_maps) {
        if (map["@type"] == "LodgingReservation") {
            extract(map);
        }
    }
}

void HotelReservation::extract(QVariantMap& map)
{
    m_reservationNumber = map["reservationNumber"].toString();
    m_name = map["underName"].toMap().value("name").toString();
    QVariantMap reservationForMap = map["reservationFor"].toMap();

    QDateTime checkinDateTime = map["checkinDate"].toDateTime();
    m_checkinDate = checkinDateTime.date().toString();
    QDateTime checkoutDateTime = map["checkoutDate"].toDateTime();
    m_checkoutDate = checkoutDateTime.date().toString();

    m_telephone = reservationForMap["telephone"].toString();
    QVariantMap addressMap = reservationForMap["address"].toMap();
    m_hotelName = reservationForMap["name"].toString();
    m_streetAddress = addressMap["streetAddress"].toString();
    m_addressLocality = addressMap["addressLocality"].toString();
    m_addressRegion = addressMap["addressRegion"].toString();

    emit extractedData();
}

void HotelReservation::cacheData()
{
    if (m_db.connectionName().isEmpty()) {
        initDatabase();
    }

    QSqlQuery updateQuery(m_db);
    QString queryString = "insert into Hotel values (:id, :reservationNumber, :name, :checkinDate, :checkoutDate, :telephone, :hotelName, :streetAddress, :addressLocality, :addressRegion)";
    updateQuery.prepare(queryString);
    updateQuery.bindValue(":reservationNumber", m_reservationNumber);
    updateQuery.bindValue(":name", m_name);
    updateQuery.bindValue(":checkinDate", m_checkinDate);
    updateQuery.bindValue(":checkoutDate", m_checkoutDate);
    updateQuery.bindValue(":telephone", m_telephone);
    updateQuery.bindValue(":hotelName", m_hotelName);
    updateQuery.bindValue(":streetAddress", m_streetAddress);
    updateQuery.bindValue(":addressLocality", m_addressLocality);
    updateQuery.bindValue(":addressRegion", m_addressRegion);

    if (!updateQuery.exec()) {
        qWarning() << "Unable to add entries into Database for Hotel Table";
        qWarning() << updateQuery.lastError();
    }
    else {
        qDebug() << "Updated Table Successfully";
    }
}

void HotelReservation::initDatabase()
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
    QString queryString = "create table if not exists Hotel(id integer primary key autoincrement, reservationNumber varchar, name varchar, checkinDate varchar, checkoutDate varchar, telephone varchar, hotelName varchar, streetAddress varchar, addressLocality varchar, addressRegion varchar)";

    if (!addQuery.exec(queryString)) {
        qWarning() << "Unable to create table";
        qWarning() << addQuery.lastError();
    }
    else {
        qDebug() << "Opened/Created successfully";
    }
}

void HotelReservation::setDBusData()
{
    m_dbusMap.insert("reservationNumber", m_reservationNumber);
    m_dbusMap.insert("name", m_name);
    m_dbusMap.insert("checkinDate", m_checkinDate);
    m_dbusMap.insert("checkoutDate", m_checkoutDate);
    m_dbusMap.insert("telephone", m_telephone);
    m_dbusMap.insert("hotelName", m_hotelName);
    m_dbusMap.insert("streetAddress", m_streetAddress);
    m_dbusMap.insert("addressLocality", m_addressLocality);
    m_dbusMap.insert("addressRegion", m_addressRegion);

    emit update();
}

void HotelReservation::getDataFromDatabase()
{
    QSqlQuery dataQuery(m_db);
    QString queryString = "select * from Hotel";
    dataQuery.prepare(queryString);

    if (!dataQuery.exec()) {
        qWarning() << "Unable to fetch existing records from database";
        qWarning() << dataQuery.lastError();
    }
    else {
        qDebug() << "Fetched Records from Table Hotel Successfully";
    }

    QList< QVariantMap > listOfMapsInDatabase;
    while(dataQuery.next()) {
        QVariantMap map;
        map.insert("reservationNumber", dataQuery.value(1).toString());
        map.insert("name", dataQuery.value(2).toString());
        map.insert("checkinDate", dataQuery.value(3).toString());
        map.insert("checkoutDate", dataQuery.value(4).toString());
        map.insert("telephone", dataQuery.value(5).toString());
        map.insert("hotelName", dataQuery.value(6).toString());
        map.insert("streetAddress", dataQuery.value(7).toString());
        map.insert("addressLocality", dataQuery.value(8).toString());
        map.insert("addressRegion", dataQuery.value(9).toString());
        listOfMapsInDatabase.append(map);
    }

    foreach(QVariantMap map, listOfMapsInDatabase) {
        m_reservationNumber = map["reservationNumber"].toString();
        m_name = map["name"].toString();
        m_checkinDate = map["checkinDate"].toString();
        m_checkoutDate = map["checkoutDate"].toString();
        m_telephone = map["telephone"].toString();
        m_hotelName = map["hotelName"].toString();
        m_streetAddress = map["streetAddress"].toString();
        m_addressLocality = map["addressLocality"].toString();
        m_addressRegion = map["addressRegion"].toString();
        setDBusData();
    }
}

QVariantMap HotelReservation::getMap()
{
    return m_dbusMap;
}


#include "hotelreservation.moc"
