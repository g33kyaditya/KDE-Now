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

#include "credentialshandler.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QMap>

CredentialsHandler::CredentialsHandler(QObject* parent): QObject(parent)
{

}

void CredentialsHandler::showUserCredentialsPage()
{
    m_view.setSource(QUrl::fromLocalFile("UserCredentials.qml"));
    QObject* object = m_view.rootObject();
    connect(object, SIGNAL(okSignal(QString, QString, QString, QString)),
            this, SLOT(onOkPressed(QString, QString, QString, QString)));
    connect(object, SIGNAL(cancelSignal()), this, SLOT(onCancelPressed()));
    m_view.show();
}

void CredentialsHandler::onOkPressed(QString imapServer, QString imapPort, QString username, QString password)
{
    m_credentials.imapServer = imapServer;
    m_credentials.imapPort = imapPort;
    m_credentials.username = username;
    m_credentials.password = password;

    emit gotUserCredentials(m_credentials);

}

void CredentialsHandler::onCancelPressed()
{
    qApp->quit();
}
