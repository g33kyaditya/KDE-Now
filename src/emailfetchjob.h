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

#ifndef EMAILFETCHJOB_H
#define EMAILFETCHJOB_H

#include <KIMAP/FetchJob>
#include <KIMAP/Session>

class EmailFetchJob : public QObject
{
        Q_OBJECT

    public:
        explicit EmailFetchJob(QObject* parent = 0);
        void done(KIMAP::Session*);

    Q_SIGNALS:
        void fetchedEmail(KIMAP::MessagePtr email);

    public Q_SLOTS:
        void searchJobFinished(KJob*);
        void slotMessagesReceived(const QString &mailBox, const QMap<qint64, qint64> &uids,
                              const QMap<qint64, KIMAP::MessagePtr> &messages);

    private:
        KIMAP::FetchJob* fetchJob;
};

#endif  //EMAILFETCHJOB_H
