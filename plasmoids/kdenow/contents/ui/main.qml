import QtQuick 2.0
import QtQml.Models 2.1
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.kde.now as KDENow

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
}

