import QtQuick 2.1
import QtQuick.Layouts 1.1
import QtQml.Models 2.1

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.kde.kdenow 0.1 as KDENow

ColumnLayout {
    id: root
    Layout.minimumWidth: 300
    Layout.maximumWidth: 300
    Layout.minimumHeight: 600
    Layout.maximumHeight: 600

    KDENow.DataHandler {
        id: dataHandler
        onCredentialsInput: {
            var page = credentialsCardComponent.createObject(root)
            plasmoidModel.append(page)
        }
        onEventDataReceived: {
            prepareEventCard(map)
        }
        onFlightDataReceived: {
            prepareFlightCard(map)
        }
        onHotelDataReceived: {
            prepareHotelCard(map)
        }
        onRestaurantDataReceived: {
            prepareRestaurantCard(map)
        }
    }

    PlasmaExtras.ScrollArea {
        anchors.fill: parent
        ListView {
            anchors.fill: parent
            anchors.centerIn: parent
            model: plasmoidModel
        }
    }

    ObjectModel {
        id: plasmoidModel
    }

    function prepareEventCard(map) {
        var reservation = eventReservationComponent.createObject(root)
        reservation.setMap(map)

        var card = eventCardComponent.createObject(root,
            {
                "reservationNumber": reservation.reservationNumber,
                "name": reservation.name,
                "eventName": reservation.eventName,
                "startDate": reservation.startDate,
                "startTime": reservation.startTime,
                "location": reservation.location,
                "streetAddress": reservation.streetAddress,
                "addressLocality": reservation.addressLocality,
            }
        )
        plasmoidModel.append(card)
    }

    function prepareFlightCard(map) {
        var reservation = flightReservationComponent.createObject(root)
        reservation.setMap(map)

        var card = flightCardComponent.createObject(root,
            {
                "reservationNumber": reservation.reservationNumber,
                "name": reservation.name,
                "flight": reservation.flight,
                "departureAirportName": reservation.departureAirportName,
                "departureAirportCode": reservation.departureAirportCode,
                "departureDate": reservation.departureDate,
                "departureTime": reservation.departureTime,
                "arrivalAirportName": reservation.arrivalAirportName,
                "arrivalAirportCode": reservation.arrivalAirportCode,
                "arrivalDate": reservation.arrivalDate,
                "arrivalTime": reservation.arrivalTime,
            }
        )
        plasmoidModel.append(card)
    }

    function prepareHotelCard(map) {
        var reservation = hotelReservationComponent.createObject(root)
        reservation.setMap(map)

        var card = hotelCardComponent.createObject(root,
            {
                "reservationNumber": reservation.reservationNumber,
                "name": reservation.name,
                "checkinDate": reservation.checkinDate,
                "checkoutDate": reservation.checkoutDate,
                "telephone": reservation.telephone,
                "hotelName": reservation.hotelName,
                "streetAddress": reservation.streetAddress,
                "addressLocality": reservation.addressLocality,
                "addressRegion": reservation.addressRegion,
            }
        )
        plasmoidModel.append(card)
    }

    function prepareRestaurantCard(map) {
        var reservation = restaurantReservationComponent.createObject(root)
        reservation.setMap(map)

        var card = restaurantCardComponent.createObject(root,
            {
                "reservationNumber": reservation.reservationNumber,
                "name": reservation.name,
                "startDate": reservation.startDate,
                "startTime": reservation.startTime,
                "partySize": reservation.partySize,
                "restaurantName": reservation.restaurantName,
                "streetAddress": reservation.streetAddress,
                "addressLocality": reservation.addressLocality,
                "addressRegion": reservation.addressRegion,
            }
        )
        plasmoidModel.append(card)
    }

    Component {
        id: eventReservationComponent
        KDENow.EventReservation {
        }
    }

    Component {
        id: flightReservationComponent
        KDENow.FlightReservation {
        }
    }

    Component {
        id: hotelReservationComponent
        KDENow.HotelReservation {
        }
    }

    Component {
        id: restaurantReservationComponent
        KDENow.RestaurantReservation {
        }
    }

    Component {
        id: eventCardComponent
        KDENow.EventInformation {
        }
    }

    Component {
        id: flightCardComponent
        KDENow.FlightInformation {
        }
    }

    Component {
        id: hotelCardComponent
        KDENow.HotelInformation {
        }
    }

    Component {
        id: restaurantCardComponent
        KDENow.RestaurantInformation {
        }
    }

    Component {
        id: credentialsCardComponent
        KDENow.UserCredentials {
            id: userCredentialsCard
            onCredentialsInput: {
                plasmoidModel.remove(0)
                dataHandler.onCredentialsInput(imapServer, imapPort, username, password)
            }
            onCancelSignal: {
                plasmoidModel.remove(0)
            }
        }
    }
}   //End KDE Now
