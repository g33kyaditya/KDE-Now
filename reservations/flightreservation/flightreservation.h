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

#ifndef FLIGHTRESERVATION_H
#define FLIGHTRESERVATION_H

#include "src/abstractreservationplugin.h"

class FlightReservation : public AbstractReservationPlugin
{
        Q_OBJECT
    public:
        void setPluginName(const QString& pluginName);
        void start();
        QString plugin() const;
        void extract();

    private:
        QString m_pluginName;
        QVariantMap m_map;
};

#endif //FLIGHTRESERVATION_H
