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

#include <KIMAP/SearchJob>
#include <KIMAP/ImapSet>

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

    //qDebug() << searchJob->results();
    KIMAP::ImapSet set;
    set.add(searchJob->results());

    KIMAP::FetchJob* fetchJob = new KIMAP::FetchJob(SingletonFactory::instanceFor<EmailSessionJob>()->currentSession());
    fetchJob->setSequenceSet(set);

    connect(fetchJob, SIGNAL(messagesReceived(QString,QMap<qint64,qint64>,QMap<qint64,KIMAP::MessagePtr>)),
            this, SLOT(slotMessagesReceived(QString,QMap<qint64,qint64>,QMap<qint64,KIMAP::MessagePtr>)));
    fetchJob->start();
}

void EmailFetchJob::slotMessagesReceived(const QString& mailBox,
                                         const QMap<qint64, qint64>& uids,
                                         const QMap<qint64, KIMAP::MessagePtr>& messages)
{
    QMapIterator<qint64, KIMAP::MessagePtr> it(messages);
    while (it.hasNext()) {
        it.next();

        emit fetchedEmail(it.value());
        qDebug() << it.value() << "\n";
    }

    //emit done(m_session);
}
