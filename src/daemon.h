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

/** A Daemon that runs continuously and manages logging into remote IMAP account,
 *  downloading emails, parsing and extracting data and finally sending the data
 *  to plasmoid over D-Bus. The Daemon does this by using various other components
 *  like EmailManager, Processor and Plugins and uses each of the said components'
 *  functionality.
 */

class KDENOWCORE_EXPORT Daemon : public QObject
{
        Q_OBJECT
        Q_CLASSINFO("D-Bus Interface", "org.kde.kdenow");

    public:
        Daemon(QObject* parent = 0);
        
        /**
         * Starts each EmailManager from the list of of all EmailManagers one
         * by one. Each EmailManager contains credentials pertainging to an email
         * account that the user might have added earlier/can add. 
         */
        void startEmailManagers();

    public Q_SLOTS:
        
        /** A D-Bus method which is called by the Plasmoid to start the daemon */
        Q_SCRIPTABLE QString startDaemon();
        
        /** Set the proper credentials: IMAP Address, Port, Username and Password
         *  for each of the email EmailManagers
         *  @param credentialsList List of UserCredentials stored for KDE Now in KWallet
         */ 
        void setCredentials(QList < UserCredentials >& credentialsList);

    private:
        QList < EmailManager* > m_emailManagersList;
};

#endif //DAEMON_H
