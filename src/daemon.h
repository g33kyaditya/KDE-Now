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

#ifndef DAEMON_H
#define DAEMON_H

#include "kdenowcore_export.h"
#include "usercredentials.h"
#include "emailmanager.h"

#include <QtCore/QObject>
#include <QtCore/QList>

class KDENOWCORE_EXPORT Daemon : public QObject
{
        Q_OBJECT
        Q_CLASSINFO("D-Bus Interface", "org.kde.kdenow");

    public:
        Daemon(QObject* parent = 0);
        void startEmailManagers();

    public Q_SLOTS:
        Q_SCRIPTABLE QString startDaemon();
        void setCredentials(QList < UserCredentials >& credentialsList);

    private:
        QList < EmailManager* > m_emailManagersList;
};

#endif //DAEMON_H
