set(restaurantreservation_SRCS
    restaurantreservation.cpp
)

set(dbus_SRCS org.kde.kdenow.restaurant.xml)

qt5_generate_dbus_interface(restaurantreservation.h
                            org.kde.kdenow.restaurant.xml
                            OPTIONS -M
                            )

qt5_add_dbus_adaptor(dbus_SRCS
                     ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.restaurant.xml
                     restaurantreservation.h
                     RestaurantReservation
                    )

add_library(kdenow_restaurantreservation MODULE ${restaurantreservation_SRCS} ${dbus_SRCS})
set_target_properties(kdenow_restaurantreservation PROPERTIES PREFIX "")

target_include_directories(kdenow_restaurantreservation
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )

target_link_libraries(kdenow_restaurantreservation
    kdenowcore
    Qt5::Sql
)

kcoreaddons_desktop_to_json(kdenow_restaurantreservation restaurantreservation.desktop)

install(TARGETS kdenow_restaurantreservation DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES restaurantreservation.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.restaurant.xml DESTINATION ${DBUS_INTERFACES_INSTALL_DIR})

