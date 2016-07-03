set (qml_plugins_SRCS
    qmlextensionsplugin.cpp
    eventreservation.cpp
    flightreservation.cpp
    hotelreservation.cpp
    #restaurantreservation.cpp
)

add_library(kdenowqmlplugin SHARED ${qml_plugins_SRCS})

target_link_libraries(kdenowqmlplugin
    Qt5::Qml
    Qt5::DBus
)

install (TARGETS kdenowqmlplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/now)
install (FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/now)
