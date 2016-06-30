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
#include "src/datamap.h"

#include <QtCore/QDateTime>
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
    m_pluginName = "hotelDataExtractor";
    connect(this, &HotelReservation::extractedData, this, &HotelReservation::cacheData);
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
    m_map = SingletonFactory::instanceFor<DataMap>()->map();
    extract();
}

void HotelReservation::extract()
{
    m_reservationNumber = m_map["reservationNumber"].toString();
    m_name = m_map["underName"].toMap().value("name").toString();
    QVariantMap reservationForMap = m_map["reservationFor"].toMap();

    m_checkinDate = m_map["checkinDate"].toDateTime();
    m_checkoutDate = m_map["checkoutDate"].toDateTime();

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
    updateQuery.bindValue(":checkinDate", m_checkinDate.toString());
    updateQuery.bindValue(":checkoutDate", m_checkoutDate.toString());
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
    m_db.setDatabaseName("kdenowdb");

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


#include "hotelreservation.moc"
