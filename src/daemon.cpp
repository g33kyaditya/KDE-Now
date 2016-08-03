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

#include "daemon.h"
#include "processor.h"
#include "kdenowadaptor.h"
#include "emailmanager.h"
#include "singletonfactory.h"

Daemon::Daemon(QObject* parent): QObject(parent)
{
    new KdenowAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/KDENow", this);
    dbus.registerService("org.kde.kdenow");
}

void Daemon::setCredentials(QList < UserCredentials >& credentialsList) {
    foreach (UserCredentials credentials, credentialsList) {
        EmailManager manager;
        manager.setCredentials(credentials);
        m_emailManagersList.append(manager);
    }
}

void Daemon::startEmailManagers()
{
    foreach(EmailManager manager, m_emailManagersList) {
        Processor* processor = SingletonFactory::instanceFor< Processor >();
        connect(&manager, &EmailManager::fetchedEmail, processor, &Processor::process);
        manager.start();
    }
}

QString Daemon::startDaemon()
{
    QString ret;
    ret = "Started Daemon...";
    return ret;
}
