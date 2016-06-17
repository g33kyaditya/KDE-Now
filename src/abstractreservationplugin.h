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

#ifndef ABSTRACTRESERVATIONPLUGIN_H
#define ABSTRACTRESERVATIONPLUGIN_H

#include <QtCore/QVariantMap>

class AbstractReservationPluginPrivate;

class AbstractReservationPlugin : public QObject
{
        Q_OBJECT

    public:
        explicit AbstractReservationPlugin(QObject* parent = 0,
                                           const QVariantList& = QVariantList());
        ~AbstractReservationPlugin();
        void setDataMap(QVariantMap& map);
        void setPluginName(QString& plugin);
        QString plugin() const;

    private:
        AbstractReservationPluginPrivate * const d;
};

#endif //ABSTRACTRESERVATIONPLUGIN_H
