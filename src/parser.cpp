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

#include "parser.h"

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QJsonValue>
#include <QtCore/QXmlStreamReader>

Parser::Parser(QObject* parent): QObject(parent)
{

}

bool Parser::isValidJson(const QString& res)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if (jsonDoc.isNull()) {
        return false;
    }
    return true;
}

QList<QVariantMap> Parser::parse(QByteArray& htmlDoc)
{
    QList<QVariantMap> list;
    QVariantMap map;
    QXmlStreamReader xmlReader(htmlDoc);
    while (xmlReader.tokenType() != QXmlStreamReader::EndDocument) {
            if (xmlReader.tokenType() == QXmlStreamReader::StartElement) {
                //qDebug() << xml.name().toString();
                if (xmlReader.name().toString() == "script") {
                    qDebug() << xmlReader.readElementText(
                        QXmlStreamReader::IncludeChildElements);
                    QString res = xmlReader.readElementText(
                        QXmlStreamReader::IncludeChildElements);
                    if (isValidJson(res)) {
                        QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
                        if (jsonDoc.isArray()) {
                            map = parseJsonAsArray(jsonDoc.array());
                            list.append(map);
                        }
                        if (jsonDoc.isObject()) {
                            map = parseJsonAsObject(jsonDoc.object());
                            list.append(map);
                        }
                    }
                }
            }
           xmlReader.readNext();
        }
        return list;
}

QVariantMap Parser::parseJsonAsArray(const QJsonArray& jsonArray)
{
    QJsonValue val = jsonArray.at(0);
    QJsonObject jsonObject = val.toObject();
    QVariantMap ret = parseJsonAsObject(jsonObject);
    return ret;
}

QVariantMap Parser::parseJsonAsObject(const QJsonObject& jsonObject)
{
    QVariantMap ret = jsonObject.toVariantMap();
    return ret;
}
