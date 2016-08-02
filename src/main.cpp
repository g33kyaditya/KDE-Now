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
#include "usercredentials.h"
#include "walletmanager.h"

#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QCoreApplication>

#include <KConfigCore/KConfig>
#include <KConfigCore/KConfigGroup>
#include <KConfigCore/KSharedConfig>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    WalletManager wallet;

    Daemon* daemon = new Daemon;
    QObject::connect(&wallet, &WalletManager::setDaemonData, daemon, &Daemon::setCredentials);
    wallet.getCredentials();

    return app.exec();
}
