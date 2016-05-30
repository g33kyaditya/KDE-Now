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

#include "emailsessionjob.h"

EmailSessionJob::EmailSessionJob(QObject* parent): QObject(parent)
{

}

void EmailSessionJob::initiate()
{
    m_session = new KIMAP::Session(m_hostName, m_port, this);
}

void EmailSessionJob::setHostName(const QString& hostName)
{
    m_hostName = hostName;
}

void EmailSessionJob::setUserName(const QString& userName)
{
    m_userName = userName;
}

void EmailSessionJob::setPassword(const QString& password)
{
    m_password = password;
}

void EmailSessionJob::setPort(qint16 port)
{
    m_port = port;
}

KIMAP::Session* EmailSessionJob::currentSession()
{
    return m_session;
}

QString EmailSessionJob::getPassword() const
{
    return m_password;
}

QString EmailSessionJob::getUserName() const
{
    return m_userName;
}
