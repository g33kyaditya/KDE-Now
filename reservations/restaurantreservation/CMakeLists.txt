set(restaurantreservation_SRCS
    restaurantreservation.cpp
)

add_library(kdenow_restaurantreservation MODULE ${restaurantreservation_SRCS})
set_target_properties(kdenow_restaurantreservation PROPERTIES PREFIX "")

target_link_libraries(kdenow_restaurantreservation
    kdenowcore
)

kcoreaddons_desktop_to_json(kdenow_restaurantreservation restaurantreservation.desktop)

install(TARGETS kdenow_restaurantreservation DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES restaurantreservation.desktop DESTINATION ${SERVICES_INSTALL_DIR})
