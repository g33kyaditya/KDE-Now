set(eventreservation_SRCS
    eventreservation.cpp
)

add_library(kdenow_eventreservation MODULE ${eventreservation_SRCS})
set_target_properties(kdenow_eventreservation PROPERTIES PREFIX "")

target_link_libraries(kdenow_eventreservation
    kdenowcore
    Qt5::Sql
)

kcoreaddons_desktop_to_json(kdenow_eventreservation eventreservation.desktop)

install(TARGETS kdenow_eventreservation DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES eventreservation.desktop DESTINATION ${SERVICES_INSTALL_DIR})
