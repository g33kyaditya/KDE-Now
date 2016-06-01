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
#include <KConfigCore/KConfig>
#include <KConfigCore/KConfigGroup>
#include <KConfigCore/KSharedConfig>

#include "emailsessionjob.h"
#include "singletonfactory.h"
#include "emailselectjob.h"

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

    KSharedConfigPtr config = KSharedConfig::openConfig("kdenowrc");
    KConfigGroup generalGroup(config, "General");
    generalGroup.writeEntry("UIDNEXT", "FooBar");
    generalGroup.config()->sync();

    EmailSessionJob* wrapperSessionJob = SingletonFactory::instanceFor<EmailSessionJob>();
    wrapperSessionJob->setHostName(parser.value("imapServer"));
    wrapperSessionJob->setUserName(parser.value("username"));
    wrapperSessionJob->setPassword(parser.value("password"));
    wrapperSessionJob->setPort(parser.value("imapPort").toUShort());

    wrapperSessionJob->initiate();

    qDebug() << wrapperSessionJob->currentSession()<< "\n";

    KIMAP::LoginJob* loginJob = new KIMAP::LoginJob(wrapperSessionJob->currentSession());
    loginJob->setUserName(wrapperSessionJob->getUserName());
    loginJob->setPassword(wrapperSessionJob->getPassword());
    loginJob->setEncryptionMode(KIMAP::LoginJob::AnySslVersion);

    EmailSelectJob *wrapperSelectJob = new EmailSelectJob();

    QObject::connect(loginJob, &KJob::result, wrapperSelectJob, &EmailSelectJob::loginJobFinished);
    loginJob->start();

    return app.exec();
}
