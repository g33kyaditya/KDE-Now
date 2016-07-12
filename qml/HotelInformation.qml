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
import QtQuick.Controls 1.1 as QtControls

import org.kde.plasma.extras 2.0 as PlasmaExtras

ColumnLayout {
    id: root
    property string reservationNumber: "ABC123"
    property string name: "John Smith"
    property date checkinDate: "2017-03-06T19:30:00-08:00"
    property date checkoutDate: "2017-03-08T19:30:00-08:00"
    property string telephone: "(415) 771-1400"
    property string hotelName : "Le Gavroche"
    property string streetAddress: "333 O'Farrell St"
    property string addressLocality: "San Francisco"
    property string addressRegion: "CA"

    RowLayout {
        id: top
        anchors {
            top: parent.top
            right: parent.right
            left: parent.left
        }
        Image {
            anchors {
                top: parent.top
                right: parent.right
                left: parent.left
            }
            id: background
            source: "Hotel.jpg"
            sourceSize.width: root.width
            PlasmaExtras.Heading {
                text: "Hotel"
                font.bold: true
                color: "black"
                level: 2
                anchors.horizontalCenter: background.horizontalCenter
            }

            ColumnLayout {
                spacing: 1
                anchors {
                    top: parent.verticalCenter
                    left: parent.left
                    leftMargin: 1
                }
                PlasmaExtras.Heading {
                    text: hotelName
                    font.bold: true
                    id: heading
                    level: 1
                    color: "white"
                    opacity: 1.0
                }
                PlasmaExtras.Paragraph {
                    text: streetAddress
                    color: "white"
                }
                PlasmaExtras.Paragraph {
                    text: addressLocality
                    color: "white"

                }
                RowLayout {
                    spacing: 3
                    Image {
                        source: "ph.png"
                    }
                    PlasmaExtras.Paragraph {
                        text: telephone
                        color: "white"
                    }
                }
            }
        }
    }

    ColumnLayout {
        anchors.top: top.bottom
        anchors.left: root.left
        PlasmaExtras.Heading {
            text: "Name"
            level: 5
        }
        PlasmaExtras.Heading {
            text: "Check In"
            level: 5
        }
        PlasmaExtras.Heading {
            text: "Check Out"
            level: 5
        }
        PlasmaExtras.Heading {
            text: "Reservation Number"
            level: 5
        }
    }

    ColumnLayout {
        anchors.top: top.bottom
        anchors.right: root.right
        PlasmaExtras.Heading {
            text: name
            level: 5
        }
        PlasmaExtras.Heading {
            text: Qt.formatDate(checkinDate, "ddd d MMM yyyy")
            level: 5
        }
        PlasmaExtras.Heading {
            text: Qt.formatDateTime(checkoutDate, "ddd d MMM yyyy")
            level: 5
        }
        PlasmaExtras.Heading {
            text: reservationNumber
            level: 5
        }
    }
}
