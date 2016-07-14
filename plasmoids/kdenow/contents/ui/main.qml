import QtQuick 2.0
import QtQml.Models 2.1
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.kde.kdenow as KDENow

Item {
    Layout.minimumWidth: 300
    Layout.maximumWidth: 300
    Layout.minimumHeight: 600
    Layout.maximumHeight: 600

    KDENow.DataHandler {
        id: dataHandler
        onEventDataReceived: prepareEventCard(m_map)
        onFlightDataReceived: prepareFlightCard(m_map)
        onHotelDataReceived: prepareHotelCard(m_map)
        onRestaurantDataReceived: prepareRestaurantCard(m_map)
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
        KDENow.EventReservation {
            id: reservation
        }
        reservation.setMap(map)

        KDENow.EventInformation {
            id: card
            reservationNumber: reservation.reservationNumber
            name: reservation.name
            eventName: reservation.eventName
            startDate: reservation.startDate
            location: reservation.location
            streetAddress: reservation.streetAddress
            addressLocality: reservation.addressLocality
        }
        plasmoidModel.append(card)
    }

    function prepareFlightCard(map) {
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
    }
}

