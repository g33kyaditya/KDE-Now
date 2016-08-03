/*
 * Copyright (C) 2016 Aditya Dev Sharma <aditya.sharma15696@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

import QtQuick 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

GridLayout {
    id: root
    columns: 2

    signal credentialsInput(string imapServer, string imapPort, string username, string password)
    signal cancelSignal()

    Label {
        color: "black"
        text: "IMAP"
    }
    TextField {
        id: imapServer
        placeholderText: qsTr("IMAP Server")
    }

    Label {
        color: "black"
        text: "Port"
    }
    TextField {
        id: imapPort
        placeholderText: qsTr("Port Number")
    }

    Label {
        color: "black"
        text: "Username"
     }
    TextField {
        id: username
        placeholderText: qsTr("Username")
    }

    Label {
        color: "black"
        text: "Password"
    }
    TextField {
        id: password
        placeholderText: qsTr("Password")
        echoMode: TextInput.Password
    }

    Button {
        text: "OK"
        onClicked: root.credentialsInput(imapServer.text, imapPort.text, username.text, password.text)
    }

    Button {
        text: "Cancel"
        onClicked: root.cancelSignal()
    }
}   //End UserCredentials

