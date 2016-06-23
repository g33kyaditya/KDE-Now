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

#ifndef PARSER_H
#define PARSER_H

#include "kdenowcore_export.h"

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QVariantMap>
#include <QtCore/QJsonDocument>

class KDENOWCORE_EXPORT Parser : public QObject
{
        Q_OBJECT

    public:
        explicit Parser(QObject* parent = 0);
        QList<QVariantMap> parse(QByteArray& htmlDoc);
        bool isValidJson(const QString& res);
        QVariantMap parseJsonAsArray(const QJsonArray& jsonArray);
        QVariantMap parseJsonAsObject(const QJsonObject& jsonObject);
};

#endif //PARSER_H
