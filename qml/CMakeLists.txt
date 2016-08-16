set (qml_plugins_SRCS
    qmlextensionsplugin.cpp
    datahandler.cpp
    eventreservation.cpp
    flightreservation.cpp
    hotelreservation.cpp
    restaurantreservation.cpp
)

add_library(kdenowqmlplugin SHARED ${qml_plugins_SRCS})

target_link_libraries(kdenowqmlplugin
    Qt5::Qml
    Qt5::DBus
    KF5::Wallet
)

install (TARGETS kdenowqmlplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/kdenow)
install (FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/kdenow)
install (FILES EventInformation.qml
               FlightInformation.qml
               HotelInformation.qml
               RestaurantInformation.qml
               UserCredentials.qml
         DESTINATION ${QML_INSTALL_DIR}/org/kde/kdenow
         )
install (FILES
            resources/Event.jpg
            resources/Flight.jpg
            resources/Hotel.jpg
            resources/phone.png
            resources/Restaurant.jpg
        DESTINATION ${QML_INSTALL_DIR}/org/kde/kdenow/resources
        )
