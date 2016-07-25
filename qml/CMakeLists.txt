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
)

install (TARGETS kdenowqmlplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/kdenow)
install (FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/kdenow)
install (FILES EventInformation.qml
               Event.jpg
               FlightInformation.qml
               Flight.jpg
               HotelInformation.qml
               Hotel.jpg
               ph.png
               RestaurantInformation.qml
               Restaurant.jpg
         DESTINATION ${QML_INSTALL_DIR}/org/kde/kdenow
         )
