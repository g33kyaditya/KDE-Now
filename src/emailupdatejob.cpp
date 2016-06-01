/*
 * Copyright (C) 2015 Aditya Dev Sharma <aditya.sharma15696@gmail.com>
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

#include "emailupdatejob.h"
#include "singletonfactory.h"

EmailUpdateJob::EmailUpdateJob(QObject* parent)
    : QObject(parent)
{

}

void EmailUpdateJob::update(KIMAP::IdleJob *job, const QString &mailBox,
                            int messageCount, int recentCount)
{
    qDebug() << "Update !";
    Q_UNUSED(mailBox)
    Q_UNUSED(messageCount)
    Q_UNUSED(recentCount)

    job->stop();
    SingletonFactory::instanceFor<EmailSessionJob>()->close();
    updatesReceived();
}

void EmailUpdateJob::updatesReceived()
{
    SingletonFactory::instanceFor<EmailSessionJob>()->initiate();
    qDebug() << SingletonFactory::instanceFor<EmailSessionJob>()->currentSession() << "\n";

    KIMAP::LoginJob *loginJob = new KIMAP::LoginJob(SingletonFactory::instanceFor<EmailSessionJob>()->currentSession());
    loginJob->setUserName(SingletonFactory::instanceFor<EmailSessionJob>()->getUserName());
    loginJob->setPassword(SingletonFactory::instanceFor<EmailSessionJob>()->getPassword());
    loginJob->setEncryptionMode(KIMAP::LoginJob::AnySslVersion);

    EmailSelectJob *wrapperSelectJob = new EmailSelectJob();

    QObject::connect(loginJob, &KJob::result, wrapperSelectJob, &EmailSelectJob::loginJobFinished);
    loginJob->start();
}
