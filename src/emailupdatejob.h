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

#ifndef EMAILUPDATEJOB_H
#define EMAILUPDATEJOB_H

#include "emailsessionjob.h"
#include "emailselectjob.h"

#include <QtCore/QObject>
#include <QtCore/QString>

#include <KIMAP/IdleJob>
#include <KIMAP/LoginJob>

class EmailUpdateJob : public QObject {
        Q_OBJECT
    public:
        explicit EmailUpdateJob(QObject* parent = 0);
        void updatesReceived();

    public Q_SLOTS:
        void update(KIMAP::IdleJob *job, const QString &mailBox, int messageCount, int recentCount);

};

#endif // EMAILUPDATEJOB_H
