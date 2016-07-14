set(eventreservation_SRCS
    eventreservation.cpp
)

set(dbus_SRCS org.kde.kdenow.event.xml)

qt5_generate_dbus_interface(eventreservation.h
                            org.kde.kdenow.event.xml
                            OPTIONS -M -s
                            )

qt5_add_dbus_adaptor(dbus_SRCS
                     ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.event.xml
                     eventreservation.h
                     EventReservation
                    )

add_library(kdenow_eventreservation MODULE ${eventreservation_SRCS} ${dbus_SRCS})
set_target_properties(kdenow_eventreservation PROPERTIES PREFIX "")

target_include_directories(kdenow_eventreservation
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )

target_link_libraries(kdenow_eventreservation
    kdenowcore
    Qt5::Sql
)

kcoreaddons_desktop_to_json(kdenow_eventreservation eventreservation.desktop)

install(TARGETS kdenow_eventreservation DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES eventreservation.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.event.xml DESTINATION ${DBUS_INTERFACES_INSTALL_DIR})
