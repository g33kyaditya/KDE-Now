set(hotelreservation_SRCS
    hotelreservation.cpp
)

add_library(kdenow_hotelreservation MODULE ${hotelreservation_SRCS})
set_target_properties(kdenow_hotelreservation PROPERTIES PREFIX "")

target_link_libraries(kdenow_hotelreservation
    kdenowcore
    Qt5::Sql
)

kcoreaddons_desktop_to_json(kdenow_hotelreservation hotelreservation.desktop)

install(TARGETS kdenow_hotelreservation DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES hotelreservation.desktop DESTINATION ${SERVICES_INSTALL_DIR})