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

#include "emailsearchjob.h"
#include "singletonfactory.h"
#include "emailfetchjob.h"
#include "emailsessionjob.h"

#include <QtCore/QDate>

#include <KIMAP/SearchJob>
#include <KIMAP/SelectJob>
#include <KIMAP/FetchJob>

#include <KConfigCore/KConfig>
#include <KConfigCore/KConfigGroup>
#include <KConfigCore/KSharedConfig>

EmailSearchJob::EmailSearchJob(QObject* parent): QObject(parent)
{

}

bool EmailSearchJob::firstRun()
{
    KSharedConfigPtr config = KSharedConfig::openConfig("kdenowrc");
    KConfigGroup generalGroup(config, "General");
    QString state = generalGroup.readEntry("STATE", QString());
    if (state == "FirstRun") {
        qDebug() << "State I see now = " << state << "\n";
        return true;
    }
    return false;
}

void EmailSearchJob::selectJobFinished(KJob* job)
{
    auto selectJob = static_cast<KIMAP::SelectJob*>(job);
    if (selectJob->error()) {
        qDebug() << selectJob->errorString();
        return;
    }
    else {
        qDebug() << "Select Job Done ";
    }

    if (firstRun()) {
        qDebug() << "First Run";
        qint64 nextUid = selectJob->nextUid();  //This will be the next UID of future EmailFetchJob
        KSharedConfigPtr config = KSharedConfig::openConfig("kdenowrc");
        KConfigGroup generalGroup(config, "General");
        generalGroup.writeEntry("STATE", "Update");
        generalGroup.writeEntry("UIDNEXT", QString::number(nextUid));
        generalGroup.writeEntry("MESSAGE_COUNT", QString::number(selectJob->messageCount()));
        generalGroup.config()->sync();
        selectJob->kill();

        KIMAP::Term term(KIMAP::Term::Since, QDate::currentDate().addMonths(-1));
        KIMAP::SearchJob* searchJob = new KIMAP::SearchJob(SingletonFactory::instanceFor<EmailSessionJob>()->currentSession());
        EmailFetchJob* wrapperFetchJob = new EmailFetchJob();
        searchJob->setTerm(term);
        connect(searchJob, &KJob::result, wrapperFetchJob, &EmailFetchJob::searchJobFinished);
        searchJob->start();
    }
    else {
        qDebug() << "Not First Run";
        KSharedConfigPtr config = KSharedConfig::openConfig("kdenowrc");
        KConfigGroup generalGroup(config, "General");
        QString uidName = generalGroup.readEntry("UIDNEXT", QString());
        qint64 uidNext = uidName.toULongLong();
        qint64 uidToBeSaved = selectJob->nextUid();
        generalGroup.writeEntry("UIDNEXT", QString::number(uidToBeSaved));
        generalGroup.writeEntry("MESSAGE_COUNT", QString::number(selectJob->messageCount()));
        generalGroup.config()->sync();
        selectJob->kill();

        KIMAP::ImapInterval interval;
        interval.setBegin(uidNext);
        KIMAP::ImapSet set;
        set.add(interval);
        KIMAP::Term term(KIMAP::Term::Uid, set);

        KIMAP::SearchJob* searchJob = new KIMAP::SearchJob(SingletonFactory::instanceFor<EmailSessionJob>()->currentSession());
        EmailFetchJob* wrapperFetchJob = new EmailFetchJob();
        searchJob->setTerm(term);
        connect(searchJob, &KJob::result, wrapperFetchJob, &EmailFetchJob::searchJobFinished);
        searchJob->start();
    }
}
