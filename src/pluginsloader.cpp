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
#include <KService/KPluginInfo>
#include <KService/KPluginTrader>
#include <KCoreAddons/KPluginLoader>
#include <KCoreAddons/KPluginFactory>

PluginsLoader::PluginsLoader(QObject* parent): QObject(parent)
{

}

void PluginsLoader::load()
{
    KPluginInfo::List pluginInfo = KPluginTrader::self()->query("plasma/kdenow/reservations");
    if (pluginInfo.isEmpty()) {
        qWarning() << "Extractor Plugins Not Found";
    }

    foreach(const KPluginInfo& info, pluginInfo) {
        KPluginLoader loader(info.libraryPath());
        KPluginFactory* factory = loader.factory();

        const QVariantList args = QVariantList() << loader.metaData().toVariantMap();
        if (factory) {
            AbstractReservationPlugin* extractorPlugin =
            factory->create<AbstractReservationPlugin>(0, args);
            if (extractorPlugin) {
                extractorPlugin->setDataMap(SingletonFactory::instanceFor<DataMap>());
                extractorPlugin->start();
                connect(QCoreApplication::instance(), &QObject::destroyed, extractorPlugin, &QCoreApplication::quit);
                qDebug() << "Created instance for extractor plugin" << info.name();
            }
            else {
                qDebug() << "Could not create instance for the extractor plugin " << info.name();
            }
        }
    }
}
