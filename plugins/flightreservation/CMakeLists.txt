set(flightreservation_SRCS
    flightreservation.cpp
)

set(dbus_SRCS org.kde.kdenow.flight.xml)

qt5_generate_dbus_interface(flightreservation.h
                            org.kde.kdenow.flight.xml
                            OPTIONS -M -s
                            )

qt5_add_dbus_adaptor(dbus_SRCS
                     ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.flight.xml
                     flightreservation.h
                     FlightReservation
                    )

add_library(kdenow_flightreservation MODULE ${flightreservation_SRCS} ${dbus_SRCS})
set_target_properties(kdenow_flightreservation PROPERTIES PREFIX "")

target_include_directories(kdenow_flightreservation
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )

target_link_libraries(kdenow_flightreservation
    kdenowcore
    Qt5::Sql
)

kcoreaddons_desktop_to_json(kdenow_flightreservation flightreservation.desktop)

install(TARGETS kdenow_flightreservation DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES flightreservation.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.flight.xml DESTINATION ${DBUS_INTERFACES_INSTALL_DIR})
