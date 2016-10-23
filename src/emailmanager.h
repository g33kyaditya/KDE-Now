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

/** 
 *  An EmailManager does the task of interacting with the Remote IMAP account.
 *  It logs into an account given the credentials, searches and fetches the
 *  relevant emails and passes the HTML data of those emails to the processor
 */ 

class KDENOWCORE_EXPORT EmailManager : public QObject
{
        Q_OBJECT

    public:
        explicit EmailManager(QObject* parent = 0);
        
        /**
         *  Checks if the UID's are valid. If found invalid, we need to fetch all
         *  emails again
         */
        bool validUids();
        
        /**
         *  Search only the newly arrived emails. Works only when the UID's are valid.
         */
        void searchNewMessages();
        
        /**
         *  Search all the messages in the mailbox. This is done when the UID's become
         *  invalid/corrupt.
         */
        void searchAllMessages();
        
        /**
         *  Fetch Emails given the current SelectJob
         * @param job The current SelectJob pointer
         */
        void fetchEmails(KIMAP::SelectJob* job);
        
        /**
         *  Uses the IDLE feature of IMAP to make the daemon go into an Idle or laid back
         *  state. During this state, the connection with the remote server remains established
         *  and the remote email account would send a signal if it receives a new email item
         */
        void startIdle();
        
        /**
         *  Set the needed credentials for the current EmailManager.
         *  The credentials are IMAP Address, IMAP Port, Username and Password
         *  @param credentials The credentials that need to be set to this WalletManager
         */
        void setCredentials(UserCredentials& credentials);

        /**
         *  Login into the IMAP account
         */
        void login();

    Q_SIGNALS:
        
        /**
         *  Signal is emitted when the EmailManager has finished fetching (means downloading)
         *  the email
         *  @param message The HTML data of the Email wrapped as a KIMAP::MessagePtr
         */
        void fetchedEmail(KIMAP::MessagePtr message);

        /**
         *  Emitted when the Daemon finds that a new email has arrived in the remote mailbox.
         *  This new email needs to be fetched, so the fetch cycle is started again.
         */
        void signalUpdateProcess();

    public Q_SLOTS:

        /**
         *  Starts the Daemon
         */
        void start();

    private Q_SLOTS:

        /**
         *  Starts when the Login Job is finished by the Daemon
         *  @param job The KJob pointer
         */
        void onLoginJobFinished(KJob* job);

        /**
         *  Starts when the Select Job is finished by the Daemon
         *  @param job The KJob pointer
         */
        void onSelectJobFinished(KJob* job);

        /**
         *  Starts when the Search Job is finished by the Daemon
         *  @param job The KJob pointer
         */
        void onSearchJobFinished(KJob* job);

        /**
         *  Starts when the Fetch Job is finished by the Daemon
         */
        void onFetchJobFinished(const QString &mailBox, const QMap<qint64, qint64> &uids,
                              const QMap<qint64, KIMAP::MessagePtr> &messages);

        /**
         *  Starts when the daemon is already running, and a new email arrives
         */
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
