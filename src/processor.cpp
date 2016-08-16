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

#include "processor.h"
#include "parser.h"
#include "pluginsloader.h"
#include "singletonfactory.h"

#include <QtCore/QVariant>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>

Processor::Processor(QObject* parent): QObject(parent)
{
    pluginsLoaded = false;
}

void Processor::process(KIMAP::MessagePtr messagePtr)
{
    KMime::Content *bodyContent = messagePtr->mainBodyPart("text/html");
    if (!bodyContent) {
        qDebug() << "Could not find text/html in mainBodyPart";
        return;
    }

    if (!bodyContent->contentType(false)->isHTMLText()) {      //Don't need to create header
        qDebug() << "ABORT!";
        return;
    }

    QByteArray htmlDoc = bodyContent->decodedContent();
    //qDebug() << htmlDoc;
    Parser parser;
    QList< QVariantMap > listOfMap;
    listOfMap = parser.parse(htmlDoc);
    //qDebug() << "\n\nlistOfMap = " << listOfMap;
    if (listOfMap.isEmpty()) {
        qDebug() << "listOfMap is empty";
        return;
    }

    extractNeededData(listOfMap);
}

void Processor::extractNeededData(QList < QVariantMap >& listOfMap)
{
    if (!pluginsLoaded) {
        loadPlugins();
    }

    foreach (AbstractReservationPlugin* plugin, m_pluginList) {
        plugin->setMap(listOfMap);
        plugin->start();
    }
}

void Processor::loadPlugins()
{
    if (!pluginsLoaded) {
        PluginsLoader loader;
        m_pluginList = loader.load();
        pluginsLoaded = true;
    }
}

