set(flightreservation_SRCS
    flightreservation.cpp
)

add_library(kdenow_flightreservation MODULE ${flightreservation_SRCS})
set_target_properties(kdenow_flightreservation PROPERTIES PREFIX "")

target_link_libraries(kdenow_flightreservation
    kdenowcore
    Qt5::Sql
)

kcoreaddons_desktop_to_json(kdenow_flightreservation flightreservation.desktop)

install(TARGETS kdenow_flightreservation DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES flightreservation.desktop DESTINATION ${SERVICES_INSTALL_DIR})