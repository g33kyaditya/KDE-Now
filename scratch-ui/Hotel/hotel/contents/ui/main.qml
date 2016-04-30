/*
 * Copyright (C) 2015 Vishesh Handa <vhanda@kde.org>
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

import org.kde.kquickcontrolsaddons 2.0 as KQuickControls
import org.kde.plasma.extras 2.0 as PlasmaExtras

ColumnLayout {
    property string hotelName : "La Gavroche"
    property string hotelAddress: "43 Upper Brook Street"
    property string hotelCity: "London"
    property string hotelPhoneNumber: "415-771-1440"
    property var checkOutTime : "2015-01-30T12:40:00+01:00"
    property var checkInTime : "2015-01-30T14:50:00+01:00"
    spacing: 5
    id: root
    GridLayout {
    columns: 2
    Layout.alignment: Qt.AlignTop
        
    PlasmaExtras.Heading {
        text: hotelName
        wrapMode: Text.WordWrap
        Layout.alignment: Qt.AlignTop
        Layout.fillWidth: true
    }
        
    Image {
        source: "hotel.png"
        sourceSize.width: root.width/5
        sourceSize.height: root.width/5
    }

    ColumnLayout {
        RowLayout {
            QtControls.Label {
                text: hotelAddress
                wrapMode: Text.WordWrap
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                color: "grey"
            }
        }
        
        RowLayout{
            QtControls.Label {
                text: hotelCity
                wrapMode: Text.WordWrap
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                color: "grey"
            }
        }
        
        RowLayout {
            QtControls.Label {
                text: hotelPhoneNumber
                wrapMode: Text.WordWrap
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                color: "grey"
            }
        }
        
        RowLayout {
            QtControls.Label {
                text: "Reservation in 1 hour"
                wrapMode: Text.WordWrap
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                color: "grey"
            }
        }
    }
        
    }
    // Separator
    Rectangle {
        color: "black"
        opacity: 0.15
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
        height: 2
    }

    ColumnLayout {
        PlasmaExtras.Heading {
            text: "Reservation"
            wrapMode: Text.WordWrap
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
        }
        
        RowLayout {
            QtControls.Label {
                text: "John Smith"
                wrapMode: Text.WordWrap
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                color: "grey"
            }
        }
        
        RowLayout {
            QtControls.Label {
                text: "Reservation Number: ABC156"
                wrapMode: Text.WordWrap
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                color: "grey"
            }
        }
    }
    
    Rectangle {
        color: "black"
        opacity: 0.15
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
        height: 2
    }

    ColumnLayout {
        PlasmaExtras.Heading {
            text: "Check In"
            wrapMode: Text.WordWrap
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
        }
        
        QtControls.Label {
            text: "Sat Feb 10 2016 8:00PM"
            wrapMode: Text.WordWrap
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            color: "grey"
        }
        
        PlasmaExtras.Heading {
            text: "Check Out"
            wrapMode: Text.WordWrap
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
        }
        
        QtControls.Label {
            text: "Sun Feb 11 2016 8:00PM"
            wrapMode: Text.WordWrap
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            color: "grey"
        }
    }
    
}
