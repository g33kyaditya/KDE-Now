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
    connect(this, &RestaurantReservation::extractedData, this, &RestaurantReservation::sendDataOverDBus);
    initDatabase();
    recordsInDatabase();
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
    QString reservationNumber = map["reservationNumber"].toString();
    QString name = map["underName"].toMap().value("name").toString();
    QVariantMap reservationForMap = map["reservationFor"].toMap();

    QDateTime startDateTime = map["startTime"].toDateTime();
    QString startDate = startDateTime.date().toString();
    QString startTime = startDateTime.time().toString("h:mm AP");
    int partySize = map["partySize"].toInt();

    QVariantMap addressMap = reservationForMap["address"].toMap();
    QString restaurantName = reservationForMap["name"].toString();
    QString streetAddress = addressMap["streetAddress"].toString();
    QString addressLocality = addressMap["addressLocality"].toString();
    QString addressRegion = addressMap["addressRegion"].toString();

    QVariantMap requiredMap;
    requiredMap.insert("reservationNumber", reservationNumber);
    requiredMap.insert("name", name);
    requiredMap.insert("startDate", startDate);
    requiredMap.insert("startTime", startTime);
    requiredMap.insert("partySize", partySize);
    requiredMap.insert("restaurantName", restaurantName);
    requiredMap.insert("streetAddress", streetAddress);
    requiredMap.insert("addressLocality", addressLocality);
    requiredMap.insert("addressRegion", addressRegion);

    emit extractedData(requiredMap);
}

void RestaurantReservation::cacheData(QVariantMap& map)
{
    if (m_db.connectionName().isEmpty()) {
        initDatabase();
    }

    QSqlQuery updateQuery(m_db);
    QString queryString = "insert into Restaurant values (:id, :reservationNumber, :name, :startDate, :startTime, :partySize, :restaurantName, :streetAddress, :addressLocality, :addressRegion)";
    updateQuery.prepare(queryString);
    updateQuery.bindValue(":reservationNumber", map["reservationNumber"].toString());
    updateQuery.bindValue(":name", map["name"].toString());
    updateQuery.bindValue(":startDate", map["startDate"].toString());
    updateQuery.bindValue(":startTime", map["startTime"].toString());
    updateQuery.bindValue(":partySize", map["partySize"].toInt());
    updateQuery.bindValue(":restaurantName", map["restaurantName"].toString());
    updateQuery.bindValue(":streetAddress", map["streetAddress"].toString());
    updateQuery.bindValue(":addressLocality", map["addressLocality"].toString());
    updateQuery.bindValue(":addressRegion", map["addressRegion"].toString());

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

void RestaurantReservation::sendDataOverDBus(QVariantMap& map)
{
    QStringList keys, values;   // QTBUG-21577 : qdbusxml2cpp fails to parse QVariantMap parameter
                                //               for D-Bus signal

    keys.append("reservationNumber");
    keys.append("name");
    keys.append("startDate");
    keys.append("startTime");
    keys.append("partySize");
    keys.append("restaurantName");
    keys.append("streetAddress");
    keys.append("addressLocality");
    keys.append("addressRegion");

    values.append(map["reservationNumber"].toString());
    values.append(map["name"].toString());
    values.append(map["startDate"].toString());
    values.append(map["startTime"].toString());
    values.append(map["partySize"].toString());
    values.append(map["restaurantName"].toString());
    values.append(map["streetAddress"].toString());
    values.append(map["addressLocality"].toString());
    values.append(map["addressRegion"].toString());

    emit update(keys, values);
}

void RestaurantReservation::recordsInDatabase()
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
        m_listOfMapsInDatabase.append(map);
    }
    emit loadedRestaurantPlugin();
}

void RestaurantReservation::getDatabaseRecordsOverDBus()
{
    foreach(QVariantMap map, m_listOfMapsInDatabase) {
        sendDataOverDBus(map);
    }
}

#include "restaurantreservation.moc"
