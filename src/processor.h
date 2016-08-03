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
#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "kdenowcore_export.h"
#include "abstractreservationplugin.h"

#include <QtCore/QList>
#include <QtCore/QVariantMap>

#include <KIMAP/FetchJob>   //For KIMAP::MessagePtr

class KDENOWCORE_EXPORT Processor : public QObject
{
        Q_OBJECT

    public:
        explicit Processor(QObject* parent = 0);
        bool isAlreadyDownloaded();

    public Q_SLOTS:
        void process(KIMAP::MessagePtr messagePtr);

    private:
        void extractNeededData(QList < QVariantMap >& listOfMap);
        void extractFlightData();

        KIMAP::MessagePtr m_messagePtr;
        QList< AbstractReservationPlugin* > m_pluginList;
        bool pluginsLoaded;
};

#endif //PROCESSOR_H

