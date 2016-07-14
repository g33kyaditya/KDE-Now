set(hotelreservation_SRCS
    hotelreservation.cpp
)

set(dbus_SRCS org.kde.kdenow.hotel.xml)

qt5_generate_dbus_interface(hotelreservation.h
                            org.kde.kdenow.hotel.xml
                            OPTIONS -M -s
                            )

qt5_add_dbus_adaptor(dbus_SRCS
                     ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.hotel.xml
                     hotelreservation.h
                     HotelReservation
                    )

add_library(kdenow_hotelreservation MODULE ${hotelreservation_SRCS} ${dbus_SRCS})
set_target_properties(kdenow_hotelreservation PROPERTIES PREFIX "")

target_include_directories(kdenow_hotelreservation
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )

target_link_libraries(kdenow_hotelreservation
    kdenowcore
    Qt5::Sql
)

kcoreaddons_desktop_to_json(kdenow_hotelreservation hotelreservation.desktop)

install(TARGETS kdenow_hotelreservation DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES hotelreservation.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.hotel.xml DESTINATION ${DBUS_INTERFACES_INSTALL_DIR})
