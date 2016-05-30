/*
 * Copyright (C) 2016  Aditya Dev Sharma <aditya.sharma15696@gmail.com>
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

#ifndef SINGLETONFACTORY_H
#define SINGLETONFACTORY_H

#include <typeinfo>
#include <QtCore/QObject>
#include <QtCore/QHash>

class SingletonFactory
{
public:
    template <class T> static T* instanceFor()
    {
        const QString typeName(typeid(T).name());

        if (!m_totalInstances.contains(typeName)) {
            //Create a new instance of that type, insert in Map
            m_totalInstances.insert(typeName, static_cast< QObject* >(new T()));
        }

        return static_cast< T* >(m_totalInstances.value(typeName));
    }
private:
    static QHash<QString, QObject*> m_totalInstances;
};

#endif // SINGLETONFACTORY_H