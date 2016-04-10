import QtQuick 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExxtras

Item {
    Layout.minimumWidth: 300
    Layout.maximumWidth: 300
    Layout.minimumHeight: 600
    Layout.maximumHeight: 600

    PlasmaExtras.ScrollArea {
        anchors.fill: parent
        ListView {
            id: mainList
            focus: true
            boundsBehavior: Flickable.StopAtBounds

            model: cardDelegate
        }
    }

    Component {
        id: cardDelegate

        /*  Flight
         *  Hotel
         *  Event
         *  Restaurant
         */
    }
}