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

#include "singletonfactory.h"
#include "emailfetchjob.h"
#include "emailsessionjob.h"
#include "emailupdatejob.h"

#include <KIMAP/SearchJob>
#include <KIMAP/ImapSet>
#include <KIMAP/IdleJob>

#define CAST void (KIMAP::FetchJob::*)(const QString&, \
        const QMap<qint64, qint64>&, const QMap<qint64, KIMAP::MessagePtr>&)

EmailFetchJob::EmailFetchJob(QObject* parent) : QObject(parent)
{

}

void EmailFetchJob::searchJobFinished(KJob* job)
{
    auto searchJob = static_cast<KIMAP::SearchJob*>(job);
    if (searchJob->error()) {
        qDebug() << searchJob->errorString();
        return;
    }
    else {
        qDebug() << "Search Job Done";
    }

    qDebug() << searchJob->results();
    KIMAP::ImapSet set;
    set.add(searchJob->results());

    fetchJob = new KIMAP::FetchJob(SingletonFactory::instanceFor<EmailSessionJob>()->currentSession());
    fetchJob->setSequenceSet(set);

    connect(fetchJob, static_cast<CAST>(&KIMAP::FetchJob::messagesReceived), this, &EmailFetchJob::slotMessagesReceived);
    fetchJob->start();
}

void EmailFetchJob::slotMessagesReceived(const QString& mailBox,
                                         const QMap<qint64, qint64>& uids,
                                         const QMap<qint64, KIMAP::MessagePtr>& messages)
{
    Q_UNUSED(mailBox)
    Q_UNUSED(uids)
    qDebug() << messages << "\n";
    QMapIterator<qint64, KIMAP::MessagePtr> it(messages);
    while (it.hasNext()) {
        it.next();

        emit fetchedEmail(it.value());
        qDebug() << it.value()->subject()->asUnicodeString();
    }

    done(SingletonFactory::instanceFor<EmailSessionJob>()->currentSession());

}

void EmailFetchJob::done(KIMAP::Session* session)
{
    KIMAP::IdleJob *idleJob = new KIMAP::IdleJob(session);
    EmailUpdateJob *updateJob = new EmailUpdateJob();
    connect(idleJob, &KIMAP::IdleJob::mailBoxStats, updateJob, &EmailUpdateJob::update);
    qDebug() << "IDLIing the Connection";
    idleJob->start();
}
