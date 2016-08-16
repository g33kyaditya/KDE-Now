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

#include "emailmanager.h"
#include "kdenowadaptor.h"

#include <QtCore/QString>
#include <QtDBus/QDBusConnection>

#include <KConfigCore/KSharedConfig>
#include <KConfigCore/KConfigGroup>
#include <KIMAP/SearchJob>

#define CAST void (KIMAP::FetchJob::*)(const QString&, \
        const QMap<qint64, qint64>&, const QMap<qint64, KIMAP::MessagePtr>&)

EmailManager::EmailManager(QObject* parent): QObject(parent)
{
    connect(this, &EmailManager::signalUpdateProcess, this, &EmailManager::start);
}

void EmailManager::setCredentials(UserCredentials& credentials)
{
    m_imapServer = credentials.imapServer;
    m_imapPort = credentials.imapPort.toULong();
    m_username = credentials.username;
    m_password = credentials.password;
}


void EmailManager::start()
{
    m_session = new KIMAP::Session(m_imapServer, m_imapPort, this);
    login();
}

bool EmailManager::validUids()
{

    KSharedConfigPtr config = KSharedConfig::openConfig("kdenowrc");
    KConfigGroup group(config, m_username);
    QString uidValidity = group.readEntry("UIDVALIDITY", QString());
    if (uidValidity.toLongLong() == m_uidValidity) {
        //UID's are all valid. Also, not a first run
        return true;
    }
    //UID's are invalid, hence we need to fetch ALL the emails again
    return false;


}


void EmailManager::login()
{
    KIMAP::LoginJob* loginJob = new KIMAP::LoginJob(m_session);
    loginJob->setUserName(m_username);
    loginJob->setPassword(m_password);
    loginJob->setEncryptionMode(KIMAP::LoginJob::AnySslVersion);    //sdfsfsfs
    connect(loginJob, &KJob::result, this, &EmailManager::onLoginJobFinished);
    loginJob->start();
}

void EmailManager::onLoginJobFinished(KJob* job)
{
    auto loginJob = static_cast<KIMAP::LoginJob*>(job);
    if (loginJob->error()) {
        qDebug() << loginJob->errorString();
        return;
    }
    else {
         qDebug() << "Login Job Done Successfully";
    }
    loginJob->kill();
    KIMAP::SelectJob* selectJob = new KIMAP::SelectJob(m_session);
    selectJob->setMailBox("INBOX");
    selectJob->setOpenReadOnly(true);
    connect(selectJob, &KJob::result, this, &EmailManager::onSelectJobFinished);
    selectJob->start();
}

void EmailManager::onSelectJobFinished(KJob* job)
{
    auto selectJob = static_cast<KIMAP::SelectJob*>(job);
    if (selectJob->error()) {
        qDebug() << selectJob->errorString();
        return;
    }
    else {
        qDebug() << "Select Job Done Successfully ";
    }

    fetchEmails(selectJob);
}

void EmailManager::fetchEmails(KIMAP::SelectJob* selectJob)
{
    m_nextUid = selectJob->nextUid();  //This will be the next UID of future Email
    m_uidValidity = selectJob->uidValidity();    //Check the validity of UIDs in mail box
    m_messageCount = selectJob->messageCount();
    selectJob->kill();

    if (validUids()) {
        qDebug() << "Fetching only the new messages";
        searchNewMessages();
    }
    else {
        qDebug() << "Fetch Everything";
        searchAllMessages();
    }
}

void EmailManager::searchAllMessages()
{
    KSharedConfigPtr config = KSharedConfig::openConfig("kdenowrc");
    KConfigGroup group(config, m_username);
    group.writeEntry("UIDVALIDITY", QString::number(m_uidValidity));
    group.writeEntry("UIDNEXT", QString::number(m_nextUid));
    group.writeEntry("MESSAGE_COUNT", QString::number(m_messageCount));
    group.config()->sync();

    KIMAP::Term term(KIMAP::Term::Since, QDate::currentDate().addMonths(-1));
    KIMAP::SearchJob* searchJob = new KIMAP::SearchJob(m_session);
    searchJob->setTerm(term);
    connect(searchJob, &KJob::result, this, &EmailManager::onSearchJobFinished);
    searchJob->start();
}

void EmailManager::searchNewMessages()
{
    KSharedConfigPtr config = KSharedConfig::openConfig("kdenowrc");
    KConfigGroup group(config, m_username);
    QString uidName = group.readEntry("UIDNEXT", QString());
    qint64 uidNext = uidName.toULongLong();
    group.writeEntry("UIDNEXT", QString::number(m_nextUid));
    group.writeEntry("MESSAGE_COUNT", QString::number(m_messageCount));
    group.config()->sync();

    if (m_nextUid == uidNext) {
        qDebug() << "Already downloaded top email\n";
        emit fetchEmailsFromDatabase();
        startIdle();
    }
    KIMAP::ImapInterval interval(uidNext);
    KIMAP::ImapSet set;
    set.add(interval);
    KIMAP::Term term(KIMAP::Term::Uid, set);

    KIMAP::SearchJob* searchJob = new KIMAP::SearchJob(m_session);
    searchJob->setTerm(term);
    connect(searchJob, &KJob::result, this, &EmailManager::onSearchJobFinished);
    searchJob->start();
}

void EmailManager::onSearchJobFinished(KJob* job)
{
    auto searchJob = static_cast<KIMAP::SearchJob*>(job);
    if (searchJob->error()) {
        qDebug() << searchJob->errorString();
        return;
    }
    else {
        qDebug() << "Search Job Done Successfully";
    }

    qDebug() << searchJob->results();
    KIMAP::ImapSet set;
    set.add(searchJob->results());
    searchJob->kill();
    KIMAP::FetchJob* fetchJob = new KIMAP::FetchJob(m_session);
    fetchJob->setSequenceSet(set);
    KIMAP::FetchJob::FetchScope scope = KIMAP::FetchJob::FetchScope();
    scope.mode = KIMAP::FetchJob::FetchScope::Content;
    fetchJob->setScope(scope);
    connect(fetchJob, static_cast<CAST>(&KIMAP::FetchJob::messagesReceived),
            this, &EmailManager::onFetchJobFinished);
    fetchJob->start();
}

void EmailManager::onFetchJobFinished(const QString& mailBox, const QMap< qint64, qint64 >& uids,
                                  const QMap< qint64, KIMAP::MessagePtr >& messages)
{
    Q_UNUSED(mailBox)
    Q_UNUSED(uids)
    qDebug() << messages << "\n";

    QMapIterator<qint64, KIMAP::MessagePtr> it(messages);
    while (it.hasNext()) {
        it.next();

        qDebug() << it.value()->subject()->asUnicodeString();
        emit fetchedEmail(it.value());
    }

    startIdle();
}

void EmailManager::startIdle()
{
    KIMAP::IdleJob *idleJob = new KIMAP::IdleJob(m_session);
    connect(idleJob, &KIMAP::IdleJob::mailBoxStats, this, &EmailManager::onIdleChanged);
    qDebug() << "IDLIing the Connection";
    idleJob->start();
}

void EmailManager::onIdleChanged(KIMAP::IdleJob* job, const QString& mailBox,
                           int messageCount, int recentCount)
{
    Q_UNUSED(mailBox)
    Q_UNUSED(recentCount)
    qDebug() << "Update !";

    KSharedConfigPtr config = KSharedConfig::openConfig("kdenowrc");
    KConfigGroup group(config, m_username);
    qint64 currentMessageCount = group.readEntry("MESSAGE_COUNT", QString()).toULongLong();

    if (messageCount < currentMessageCount) {
        qDebug() << "User has deleted an email. Return";
        group.writeEntry("MESSAGE_COUNT", QString::number(messageCount));
        group.config()->sync();
        return;
    }
    else {
        qDebug() << "New email has arrived, so fetch it";
        group.writeEntry("MESSAGE_COUNT", QString::number(messageCount));
        group.config()->sync();
        job->stop();
        m_session->close();
        emit signalUpdateProcess();
    }
}