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

void WalletManager::getCredentials()
{
    m_wallet = KWallet::Wallet::openWallet("KDENowWallet",
                                            0,
                                            KWallet::Wallet::Synchronous
                                          );
    m_wallet->setFolder("KDENow");
    QStringList keys = m_wallet->entryList();
    QMap<QString, QString> walletMap;
    UserCredentials credentials;
    foreach (QString key, keys) {
        m_wallet->readMap(key, walletMap);
        qDebug() << "WalletMap = " << walletMap;
        credentials.imapServer = walletMap.value("imapServer");
        credentials.imapPort = walletMap.value("imapPort");
        credentials.username = walletMap.value("username");
        credentials.password = walletMap.value("password");
        m_credentialsList.append(credentials);
    }
    KWallet::Wallet::closeWallet("KDENowWallet", true);
    if (!m_credentialsList.isEmpty()) {
        emit setDaemonData(m_credentialsList);
    }
    else {
        qDebug() << "Credentials List is empty";
    }
}
