set(flightreservation_SRCS
    flightreservation.cpp
)

add_library(kdenow_flightreservation MODULE ${flightreservation_SRCS})
set_target_properties(kdenow_flightreservation PROPERTIES PREFIX "")

target_link_libraries(kdenow_flightreservation
    kdenow
)

install(TARGETS kdenow_flightreservation DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/kdenow/reservations)
install(FILES flightreservation.desktop DESTINATION ${SERVICES_INSTALL_DIR}/plasma/kdenow/reservations )