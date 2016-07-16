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
        onEventDataReceived: {
            var map = dataHandler.getMap()
            prepareEventCard(map)
        }
        onFlightDataReceived: {
            var map = dataHandler.getMap()
            prepareFlightCard(map)
        }
        onHotelDataReceived: {
            var map = dataHandler.getMap()
            prepareHotelCard(map)
        }
        onRestaurantDataReceived: {
            var map = dataHandler.getMap()
            prepareRestaurantCard(map)
        }
    }

    PlasmaExtras.ScrollArea {
        anchors.fill: parent
        ListView {
            anchors.fill: parent
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
                "location": reservation.location,
                "streetAddress": reservation.streetAddress,
                "addressLocality": reservation.addressLocality,
            }
        )
        plasmoidModel.append(card)
    }

    /*function prepareFlightCard(map) {
        KDENow.FlightReservation {
            id: reservation
        }
        reservation.setMap(map)

        KDENow.FlightInformation {
            id: card
            reservationNumber: reservation.reservationNumber
            name: reservation.name
            flight: reservation.flight
            departureAirportName: reservation.departureAirportName
            departureAirportCode: reservation.departureAirportCode
            departureTime: reservation.departureTime
            arrivalAirportName: reservation.arrivalAirportName
            arrivalAirportCode: reservation.arrivalAirportCode
            arrivalTime: reservation.arrivalTime
        }
        plasmoidModel.append(card)
    }

    function prepareHotelCard(map) {
        KDENow.HotelReservation {
            id: reservation
        }
        reservation.setMap(map)

        KDENow.HotelInformation {
            id: card
            reservationNumber: reservation.reservationNumber
            name: reservation.name
            checkinDate: reservation.checkinDate
            checkoutDate: reservation.checkoutDate
            telephone: reservation.telephone
            hotelName: reservation.hotelName
            streetAddress: reservation.streetAddress
            addressLocality: reservation.addressLocality
            addressRegion: reservation.addressRegion
        }
        plasmoidModel.append(card)
    }

    function prepareRestaurantCard(map) {
        KDENow.RestaurantReservation {
            id: reservation
        }
        reservation.setMap(map)

        KDENow.RestaurantInformation {
            id: card
            reservationNumber: reservation.reservationNumber
            name: reservation.name
            startTime: reservation.startTime
            partySize: reservation.partySize
            restaurantName: reservation.restaurantName
            streetAddress: reservation.streetAddress
            addressLocality: reservation.addressLocality
            addressRegion: reservation.addressRegion
        }
        plasmoidModel.append(card)
    }*/

    Component {
        id: eventReservationComponent
        KDENow.EventReservation {
        }
    }

    Component {
        id: eventCardComponent
        KDENow.EventInformation {
        }
    }

    /*Component {
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
    }*/
}   //End KDE Now
