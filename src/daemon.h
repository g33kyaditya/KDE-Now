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

#ifndef DAEMON_H
#define DAEMON_H

#include "kdenowcore_export.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QString>

#include <KIMAP/Session>
#include <KIMAP/LoginJob>
#include <KIMAP/SelectJob>
#include <KIMAP/FetchJob>
#include <KIMAP/IdleJob>


class KDENOWCORE_EXPORT Daemon : public QObject
{
        Q_OBJECT
        Q_CLASSINFO("D-Bus Interface", "org.kde.kdenow");

    public:
        Daemon(QObject* parent = 0);
        void setHostName(const QString& hostName);
        void setPort(qint64 port);
        void setUsername(const QString& username);
        void setPassword(const QString& password);

        void login();
        bool validUids();
        void searchNewMessages();
        void searchAllMessages();
        void fetchEmails(KIMAP::SelectJob* job);
        void startIdle();

    Q_SIGNALS:
        void fetchedEmail(KIMAP::MessagePtr message);
        void signalUpdateProcess();

    public Q_SLOTS:
        QString startDaemon();

    private Q_SLOTS:
        void onLoginJobFinished(KJob* job);
        void onSelectJobFinished(KJob* job);
        void onSearchJobFinished(KJob* job);
        void onFetchJobFinished(const QString &mailBox, const QMap<qint64, qint64> &uids,
                              const QMap<qint64, KIMAP::MessagePtr> &messages);
        void onIdleChanged(KIMAP::IdleJob *job, const QString &mailBox,
                            int messageCount, int recentCount);
        void start();
        //void startIdle();

    private:
        KIMAP::Session* m_session;

        QString m_hostName;
        qint64 m_port;
        QString m_username;
        QString m_password;

        qint64 m_nextUid;
        qint64 m_uidValidity;
        int m_messageCount;
};

#endif //DAEMON_H
