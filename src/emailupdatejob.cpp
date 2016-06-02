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

#include <KConfigCore/KConfig>
#include <KConfigCore/KConfigGroup>
#include <KConfigCore/KSharedConfig>

EmailUpdateJob::EmailUpdateJob(QObject* parent)
    : QObject(parent)
{

}

void EmailUpdateJob::update(KIMAP::IdleJob *job, const QString &mailBox,
                            int messageCount, int recentCount)
{
    Q_UNUSED(mailBox)
    Q_UNUSED(recentCount)
    qDebug() << "Update !";

    KSharedConfigPtr config = KSharedConfig::openConfig("kdenowrc");
    KConfigGroup generalGroup(config, "General");
    qint64 currentMessageCount = generalGroup.readEntry("MESSAGE_COUNT", QString()).toULongLong();

    if (messageCount < currentMessageCount) {
        //User has deleted an email. Return
        generalGroup.writeEntry("MESSAGE_COUNT", QString::number(messageCount));
        generalGroup.config()->sync();
        return;
    }
    else {
        //New email has arrived, so fetch it
        generalGroup.writeEntry("MESSAGE_COUNT", QString::number(messageCount));
        generalGroup.config()->sync();
        job->stop();
        SingletonFactory::instanceFor<EmailSessionJob>()->close();
        updatesReceived();
    }
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
