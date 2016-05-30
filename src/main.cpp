/*
 * Copyright (C) 2015  Vishesh Handa <vhanda@kde.org>
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

#include <QtCore/QObject>
#include <QtCore/QCoreApplication>

#include <QCommandLineParser>
#include <QCommandLineOption>

#include <KIMAP/Session>
#include <KIMAP/LoginJob>

#include "emailsessionjob.h"
#include "singletonfactory.h"
#include "emailselectjob.h"
/*#include "emailsearchjob.h"
#include "emailfetchjob.h"
*/

//#include "processor.h"
//#include "emailupdatejob.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addOption(QCommandLineOption("imapServer", "Imap server", "server"));
    parser.addOption(QCommandLineOption("imapPort", "Imap port", "number"));
    parser.addOption(QCommandLineOption("username", "Email", "email"));
    parser.addOption(QCommandLineOption("password", "Email password", "password"));
    parser.addHelpOption();
    parser.process(app);

    if (!parser.isSet("imapServer") || !parser.isSet("imapPort") || !parser.isSet("username")
        || !parser.isSet("password")) {
        parser.showHelp(1);
    }

    EmailSessionJob* sessionJob = SingletonFactory::instanceFor<EmailSessionJob>();
    sessionJob->setHostName(parser.value("imapServer"));
    sessionJob->setUserName(parser.value("username"));
    sessionJob->setPassword(parser.value("password"));
    sessionJob->setPort(parser.value("imapPort").toUShort());

    sessionJob->initiate();

    qDebug() << sessionJob->currentSession()<< "\n";

    KIMAP::LoginJob* loginJob = new KIMAP::LoginJob(sessionJob->currentSession());
    loginJob->setUserName(sessionJob->getUserName());
    loginJob->setPassword(sessionJob->getPassword());
    loginJob->setEncryptionMode(KIMAP::LoginJob::AnySslVersion);

    EmailSelectJob *selectJob = new EmailSelectJob();

    QObject::connect(loginJob, SIGNAL(finished(KJob*)), selectJob, SLOT(loginJobFinished(KJob*)));
    loginJob->start();

    return app.exec();
}
