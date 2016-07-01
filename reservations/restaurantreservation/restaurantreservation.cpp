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
#include "src/datamap.h"
#include "restaurantadaptor.h"

#include <QtCore/QDateTime>
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
    connect(this, &RestaurantReservation::addedToDatabase, this, &RestaurantReservation::setDBusData);
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
    m_map = SingletonFactory::instanceFor<DataMap>()->map();
    extract();
}

void RestaurantReservation::extract()
{
    m_reservationNumber = m_map["reservationNumber"].toString();
    m_name = m_map["underName"].toMap().value("name").toString();
    QVariantMap reservationForMap = m_map["reservationFor"].toMap();

    m_startTime = m_map["startTime"].toDateTime();
    m_partySize = m_map["partySize"].toInt();

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
    QString queryString = "insert into Restaurant values (:id, :reservationNumber, :name, :startTime, :partySize, :restaurantName, :streetAddress, :addressLocality, :addressRegion)";
    updateQuery.prepare(queryString);
    updateQuery.bindValue(":reservationNumber", m_reservationNumber);
    updateQuery.bindValue(":name", m_name);
    updateQuery.bindValue(":startTime", m_startTime.toString());
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
        emit addedToDatabase();
    }
}

void RestaurantReservation::initDatabase()
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
    QString queryString = "create table if not exists Restaurant(id integer primary key autoincrement, reservationNumber varchar, name varchar, startTime varchar, partySize integer, restaurantName varchar, streetAddress varchar, addressLocality varchar, addressRegion varchar)";

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
    m_dbusData.insert("startTime", m_startTime.toString());
    m_dbusData.insert("partySize", m_partySize);
    m_dbusData.insert("restaurantName", m_restaurantName);
    m_dbusData.insert("streetAddress", m_streetAddress);
    m_dbusData.insert("addressLocality", m_addressLocality);
    m_dbusData.insert("addressRegion", m_addressRegion);
}

QVariantMap RestaurantReservation::getMap()
{
    return m_dbusData;
}

#include "restaurantreservation.moc"
