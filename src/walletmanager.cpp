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

#include "walletmanager.h"

#include <QtCore/QMap>
#include <QtCore/QDebug>

WalletManager::WalletManager(QObject* parent): QObject(parent)
{

}

void WalletManager::onUserCredentialsReceived(UserCredentials credentials)
{
    m_credentials = credentials;
    emit finished();
}

void WalletManager::getCredentials()
{
    m_wallet = KWallet::Wallet::openWallet("KDENowWallet",
                                           0,
                                           KWallet::Wallet::Synchronous
                                           );
    m_wallet->setFolder("KDENow");
    QMap<QString, QString> walletMap;
    m_wallet->readMap("KDENowKey", walletMap);
    qDebug() << "WalletMap = " << walletMap;
    m_credentials.imapServer = walletMap.value("imapServer");
    m_credentials.imapPort = walletMap.value("imapPort");
    m_credentials.username = walletMap.value("username");
    m_credentials.password = walletMap.value("password");

    KWallet::Wallet::closeWallet("KDENowWallet", true);
    emit setDaemonData(m_credentials);
}

void WalletManager::addToWallet()
{
    m_wallet = KWallet::Wallet::openWallet("KDENowWallet",
                                           0,
                                           KWallet::Wallet::Synchronous
                                           );
    if (m_wallet->createFolder("KDENow") && m_wallet->setFolder("KDENow")) {
        qDebug() << "Created and set the folder in KWallet";
    }
    else {
        qDebug() << "Could not create or set the folder";
        return;
    }

    QMap<QString, QString> walletMap;
    walletMap.insert("imapServer", m_credentials.imapServer);
    walletMap.insert("imapPort", m_credentials.imapPort);
    walletMap.insert("username", m_credentials.username);
    walletMap.insert("password", m_credentials.password);

    int fail = m_wallet->writeMap("KDENowKey", walletMap);
    if (fail) {
        qDebug() << "Could not write map";
        return;
    }
    else {
        qDebug() << "Succesfully written map";
    }
    KWallet::Wallet::closeWallet("KDENowWallet", true);
    emit addedToWallet();
}
