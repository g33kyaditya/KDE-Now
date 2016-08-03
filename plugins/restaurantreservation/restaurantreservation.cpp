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
            break;
        }
    }
}

void RestaurantReservation::extract(QVariantMap& map)
{
    m_reservationNumber = map["reservationNumber"].toString();
    m_name = map["underName"].toMap().value("name").toString();
    QVariantMap reservationForMap = map["reservationFor"].toMap();

    QDateTime startDateTime = map["startTime"].toDateTime();
    m_startDate = startDateTime.date();
    m_startTime = startDateTime.time();
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
    updateQuery.bindValue(":startDate", m_startDate.toString());
    updateQuery.bindValue(":startTime", m_startTime.toString("h:mm AP"));
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
    m_dbusData.insert("startDate", m_startDate.toString());
    m_dbusData.insert("startTime", m_startTime.toString("h:mm AP"));
    m_dbusData.insert("partySize", m_partySize);
    m_dbusData.insert("restaurantName", m_restaurantName);
    m_dbusData.insert("streetAddress", m_streetAddress);
    m_dbusData.insert("addressLocality", m_addressLocality);
    m_dbusData.insert("addressRegion", m_addressRegion);

    emit update();
}

QVariantMap RestaurantReservation::getMap()
{
    return m_dbusData;
}

#include "restaurantreservation.moc"
