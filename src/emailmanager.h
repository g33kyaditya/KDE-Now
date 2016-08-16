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

#ifndef EMAILMANAGER_H
#define EMAILMANAGER_H

#include "kdenowcore_export.h"
#include "usercredentials.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QString>

#include <KIMAP/Session>
#include <KIMAP/LoginJob>
#include <KIMAP/SelectJob>
#include <KIMAP/FetchJob>
#include <KIMAP/IdleJob>

class KDENOWCORE_EXPORT EmailManager : public QObject
{
        Q_OBJECT

    public:
        explicit EmailManager(QObject* parent = 0);
        bool validUids();
        void searchNewMessages();
        void searchAllMessages();
        void fetchEmails(KIMAP::SelectJob* job);
        void startIdle();
        void setCredentials(UserCredentials& credentials);
        void login();

    Q_SIGNALS:
        void fetchedEmail(KIMAP::MessagePtr message);
        void signalUpdateProcess();
        void fetchEmailsFromDatabase();

    public Q_SLOTS:
        void start();

    private Q_SLOTS:
        void onLoginJobFinished(KJob* job);
        void onSelectJobFinished(KJob* job);
        void onSearchJobFinished(KJob* job);
        void onFetchJobFinished(const QString &mailBox, const QMap<qint64, qint64> &uids,
                              const QMap<qint64, KIMAP::MessagePtr> &messages);
        void onIdleChanged(KIMAP::IdleJob *job, const QString &mailBox,
                            int messageCount, int recentCount);

    private:
        KIMAP::Session* m_session;

        QString m_imapServer;
        qint64 m_imapPort;;
        QString m_username;
        QString m_password;

        qint64 m_nextUid;
        qint64 m_uidValidity;
        int m_messageCount;
};

#endif //EMAILMANAGER_H
