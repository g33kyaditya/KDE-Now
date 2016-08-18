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
    connect(this, &HotelReservation::extractedData, this, &HotelReservation::sendDataOverDBus);
    initDatabase();
    recordsInDatabase();
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
    QString reservationNumber = map["reservationNumber"].toString();
    QString name = map["underName"].toMap().value("name").toString();
    QVariantMap reservationForMap = map["reservationFor"].toMap();

    QDateTime checkinDateTime = map["checkinDate"].toDateTime();
    QString checkinDate = checkinDateTime.date().toString();
    QDateTime checkoutDateTime = map["checkoutDate"].toDateTime();
    QString checkoutDate = checkoutDateTime.date().toString();

    QString telephone = reservationForMap["telephone"].toString();
    QVariantMap addressMap = reservationForMap["address"].toMap();
    QString hotelName = reservationForMap["name"].toString();
    QString streetAddress = addressMap["streetAddress"].toString();
    QString addressLocality = addressMap["addressLocality"].toString();
    QString addressRegion = addressMap["addressRegion"].toString();

    QVariantMap requiredMap;
    requiredMap.insert("reservationNumber", reservationNumber);
    requiredMap.insert("name", name);
    requiredMap.insert("checkinDate", checkinDate);
    requiredMap.insert("checkoutDate", checkoutDate);
    requiredMap.insert("telephone", telephone);
    requiredMap.insert("hotelName", hotelName);
    requiredMap.insert("streetAddress", streetAddress);
    requiredMap.insert("addressLocality", addressLocality);
    requiredMap.insert("addressRegion", addressRegion);

    emit extractedData(requiredMap);
}

void HotelReservation::cacheData(QVariantMap& map)
{
    if (m_db.connectionName().isEmpty()) {
        initDatabase();
    }

    QSqlQuery updateQuery(m_db);
    QString queryString = "insert into Hotel values (:id, :reservationNumber, :name, :checkinDate, :checkoutDate, :telephone, :hotelName, :streetAddress, :addressLocality, :addressRegion)";
    updateQuery.prepare(queryString);
    updateQuery.bindValue(":reservationNumber", map["reservationNumber"].toString());
    updateQuery.bindValue(":name", map["name"].toString());
    updateQuery.bindValue(":checkinDate", map["checkinDate"].toString());
    updateQuery.bindValue(":checkoutDate", map["checkoutDate"].toString());
    updateQuery.bindValue(":telephone", map["telephone"].toString());
    updateQuery.bindValue(":hotelName", map["hotelName"].toString());
    updateQuery.bindValue(":streetAddress", map["streetAddress"].toString());
    updateQuery.bindValue(":addressLocality", map["addressLocality"].toString());
    updateQuery.bindValue(":addressRegion", map["addressRegion"].toString());

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

void HotelReservation::sendDataOverDBus(QVariantMap& map)
{
    QStringList keys, values;   // QTBUG-21577 : qdbusxml2cpp fails to parse QVariantMap parameter
                                //               for D-Bus signal

    keys.append("reservationNumber");
    keys.append("name");
    keys.append("checkinDate");
    keys.append("checkoutDate");
    keys.append("telephone");
    keys.append("hotelName");
    keys.append("streetAddress");
    keys.append("addressLocality");
    keys.append("addressRegion");

    values.append(map["reservationNumber"].toString());
    values.append(map["name"].toString());
    values.append(map["checkinDate"].toString());
    values.append(map["checkoutDate"].toString());
    values.append(map["telephone"].toString());
    values.append(map["hotelName"].toString());
    values.append(map["streetAddress"].toString());
    values.append(map["addressLocality"].toString());
    values.append(map["addressRegion"].toString());

    emit update(keys, values);
}

void HotelReservation::recordsInDatabase()
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
        m_listOfMapsInDatabase.append(map);
    }
    emit loadedHotelPlugin();
}

void HotelReservation::getDatabaseRecordsOverDBus()
{
    foreach(QVariantMap map, m_listOfMapsInDatabase) {
        sendDataOverDBus(map);
    }
}


#include "hotelreservation.moc"
