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

#include <QtCore/QObject>
#include <QtGui/QApplication>

#include <KConfigCore/KConfig>
#include <KConfigCore/KConfigGroup>
#include <KConfigCore/KSharedConfig>

#include "daemon.h"
#include "credentialshandler.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    CredentialsHandler handler;
    UserCredentials credentials;

    KSharedConfigPtr config = KSharedConfig::openConfig("kdenowrc");
    KConfigGroup generalGroup(config, "General");
    QString state = generalGroup.readEntry("STATE", QString());

    if (state != "Update") {
        qDebug() << "FirstRun";
        generalGroup.writeEntry("STATE", "FirstRun");
        generalGroup.config()->sync();

        handler.showUserCredentialsPage();
        credentials = handler.getUserCredentials();
    }
    else {
        credentials = handler.getUserCredentialsFromKWallet();
    }

    Daemon* daemon = new Daemon;
    daemon->setHostName(credentials.imapServer);
    daemon->setUsername(credentials.username);
    daemon->setPassword(credentials.password);
    daemon->setPort(credentials.imapPort.toUShort());
    return app.exec();
}
