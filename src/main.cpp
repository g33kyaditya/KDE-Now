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

#include "daemon.h"
#include "credentialshandler.h"
#include "usercredentials.h"
#include "walletmanager.h"

#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtGui/QApplication>

#include <KConfigCore/KConfig>
#include <KConfigCore/KConfigGroup>
#include <KConfigCore/KSharedConfig>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    CredentialsHandler handler;
    UserCredentials credentials;
    WalletManager wallet;

    QObject::connect(&handler, &CredentialsHandler::gotUserCredentials,
            &wallet, &WalletManager::onUserCredentialsReceived);
    QObject::connect(&wallet, &WalletManager::finished, &wallet, &WalletManager::addToWallet);
    QObject::connect(&wallet, &WalletManager::addedToWallet, &wallet, &WalletManager::getCredentials);

    KSharedConfigPtr config = KSharedConfig::openConfig("kdenowrc");
    KConfigGroup generalGroup(config, "General");
    QString state = generalGroup.readEntry("STATE", QString());

    Daemon* daemon = new Daemon;
    QObject::connect(&wallet, &WalletManager::setDaemonData, daemon, &Daemon::setCredentials);
    if (state != "Update") {
        qDebug() << "FirstRun";
        generalGroup.writeEntry("STATE", "FirstRun");
        generalGroup.config()->sync();

        handler.showUserCredentialsPage();
    }
    else {
        wallet.getCredentials();
    }
    return app.exec();
}
