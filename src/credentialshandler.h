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

#ifndef CREDENTIALSHANDLER_H
#define CREDENTIALSHANDLER_H

#include <QtQuick/QtQuick>
#include <QtCore/QObject>
#include <QtCore/QDebug>

#include <KWallet/KWallet>

struct UserCredentials {
    QString imapServer;
    QString imapPort;
    QString username;
    QString password;
};

class CredentialsHandler : public QObject
{
        Q_OBJECT

    public:
        CredentialsHandler(QObject* parent = 0);
        void showUserCredentialsPage();
        UserCredentials getUserCredentials();
        UserCredentials getUserCredentialsFromKWallet();

    public Q_SLOTS:
        void onOkPressed(QString imapServer, QString imapPort, QString username, QString password);
        void onCancelPressed();
        void onWalletOpened(bool status);

    private:
        KWallet::Wallet* m_wallet;
        UserCredentials m_credentials;
};

#endif //CREDENTIALSHANDLER_H
