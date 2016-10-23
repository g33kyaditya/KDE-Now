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

#ifndef WALLETMANAGER_H
#define WALLETMANAGER_H

#include "kdenowcore_export.h"
#include "usercredentials.h"

#include <QtCore/QObject>
#include <QtCore/QList>

#include <KWallet/KWallet>

/**
 *  A manager for the KDE System Wallet (KWallet5) that stores the remote IMAP
 *  account credentials. Each group of credentials has 4 key:value pairs for
 *  IMAP Address, IMAP Port, USername and Password.
 *  There could be multiple number of such groups for each of the remote IMAP
 *  account that the user adds.
 */

class KDENOWCORE_EXPORT WalletManager : public QObject
{
        Q_OBJECT

    public:
        explicit WalletManager(QObject* parent = 0);
        
        /**
         *  Opens the KDE System Wallet (KWallet5) and fetches all the groups of
         *  credentials stored in it for KDE Now. Then saves them into a QList of
         *  UserCredentials.
         */
        void getCredentials();

    Q_SIGNALS:
        /**
         *  A signal that is emitted when the WalletManager has finished with fetching the
         *  credentials and saving them into a QList
         *  @param credentialsList List of UserCredentials that the WalletManager has
         */
        void setDaemonData(QList< UserCredentials >& credentialsList);

    private:
        KWallet::Wallet* m_wallet;
        QList< UserCredentials > m_credentialsList;
};

#endif //WALLETMANAGER_H
