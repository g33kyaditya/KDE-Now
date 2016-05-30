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

#include <KIMAP/SearchJob>
#include <KIMAP/SelectJob>
#include <KIMAP/FetchJob>
#include <QtCore/QDate>

EmailSearchJob::EmailSearchJob(QObject* parent): QObject(parent)
{

}

void EmailSearchJob::selectJobFinished(KJob* job)
{
    auto selectJob = static_cast<KIMAP::SelectJob*>(job);
    if (selectJob->error()) {
        qDebug() << selectJob->errorString();
        return;
    }

    KIMAP::Term term(KIMAP::Term::Since, QDate::currentDate().addMonths(-1));
    KIMAP::SearchJob* searchJob = new KIMAP::SearchJob(SingletonFactory::instanceFor<EmailSessionJob>()->currentSession());
    KIMAP::FetchJob* fetchJob = new KIMAP::FetchJob(SingletonFactory::instanceFor<EmailSessionJob>()->currentSession());
    searchJob->setTerm(term);
    connect(searchJob, SIGNAL(finsished(KJob*)), fetchJob, SLOT(searchJobFinished(KJob*)));
    searchJob->start();
}
