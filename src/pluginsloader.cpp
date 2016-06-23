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

#include "pluginsloader.h"
#include "abstractreservationplugin.h"
#include "singletonfactory.h"

#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <KService/KServiceTypeTrader>
#include <KCoreAddons/KPluginLoader>
#include <KCoreAddons/KPluginFactory>
#include <KService/KPluginInfo>

PluginsLoader::PluginsLoader(QObject* parent): QObject(parent)
{

}

void PluginsLoader::load()
{
    KService::List offers = KServiceTypeTrader::self()->query("KDENow/Plugin");
    if (offers.isEmpty()) {
        qDebug() << "None plugins found";
        return;
    }

    KService::List::const_iterator it;
    for (it = offers.begin(); it != offers.end(); ++it) {
        QString error;
        KService::Ptr service = *it;
        KPluginFactory* factory = KPluginLoader(service->library()).factory();

        if (!factory) {
            qDebug() << "KPluginFactory could not load the plugin " << service->library();
            continue;
        }
        else {
            qDebug() << service->library();
        }

        AbstractReservationPlugin* plugin = factory->create<AbstractReservationPlugin>();
        if (plugin) {
            qDebug() << "Loaded the plugin " << service->name();
            plugin->setDataMap(SingletonFactory::instanceFor<DataMap>());
            plugin->start();
        }
        else {
            qDebug() << "Did not load the plugin " << service->name();
            continue;
        }
    }
}
