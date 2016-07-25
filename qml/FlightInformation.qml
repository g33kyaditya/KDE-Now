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
    property string name: "John Smith"
    property string reservationNumber: "ABC123"
    property string flight : "Vueling 8992"
    property string departureTime : " "
    property string departureDate : " "
    property string arrivalTime : " "
    property string arrivalDate : " "
    property string departureAirportCode : "BCN"
    property string departureAirportName : "Barcelona"
    property string arrivalAirportCode : "BRU"
    property string arrivalAirportName : "Brussels"
    anchors {
        left: parent.left
        right: parent.right
    }

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
            source: "Flight.jpg"
            sourceSize.width: root.width
            PlasmaExtras.Heading {
                text: flight
                font.bold: true
                color: "white"
                level: 2
                anchors.horizontalCenter: background.horizontalCenter
            }
            // Left Side Departure Info
            ColumnLayout {
                spacing: 0
                Layout.alignment: Qt.AlignTop
                anchors {
                    bottom: parent.bottom
                    left: parent.left
                    //leftMargin: 1
                }

                PlasmaExtras.Heading {
                    text: "Depart"
                    font.bold: true
                    level: 3
                }
                RowLayout {
                    PlasmaExtras.Heading {
                        text: departureAirportName
                        level: 4
                    }
                }
                RowLayout {
                    PlasmaExtras.Heading {
                        text: departureAirportCode
                        level: 2
                        Layout.alignment: Qt.AlignTop
                    }
                    PlasmaExtras.Heading {
                        text: departureTime
                        level: 5
                    }
                }
            }

            //Right Side Arrival Info
            ColumnLayout {
                spacing: 0
                Layout.alignment: Qt.AlignTop
                anchors {
                    bottom: parent.bottom
                    right: parent.right
                    //rightMargin: 2
                }

                PlasmaExtras.Heading {
                    text: "Arrive"
                    font.bold: true
                    level: 3
                }
                RowLayout {
                    PlasmaExtras.Heading {
                        text: arrivalAirportName
                        level: 4
                    }
                }
                RowLayout {
                    PlasmaExtras.Heading {
                        text: arrivalAirportCode
                        level: 2
                        Layout.alignment: Qt.AlignTop
                    }

                PlasmaExtras.Heading {
                    text: arrivalTime
                    level: 5
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
            text: "Departure Date"
            level: 5
        }
        PlasmaExtras.Heading {
            text: "Arrival Date"
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
            text: departureDate
            level: 5
        }
        PlasmaExtras.Heading {
            text: arrivalDate
            level: 5
            }
        PlasmaExtras.Heading {
            text: reservationNumber
            level: 5
        }
    }
}
