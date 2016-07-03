add_executable(kdenowd
    main.cpp
)

add_library(kdenowcore
    daemon.cpp
    processor.cpp
    parser.cpp
    abstractreservationplugin.cpp
    pluginsloader.cpp
    singletonfactory.cpp
    datamap.cpp
)

target_link_libraries(kdenowcore
PUBLIC
    Qt5::Core
    Qt5::Xml
    KF5::IMAP
    KF5::KIOCore
    KF5::CoreAddons
    KF5::Service
)

target_link_libraries(kdenowd
    kdenowcore
)

generate_export_header(kdenowcore
    EXPORT_FILE_NAME kdenowcore_export.h
    EXPORT_MACRO_NAME KDENOWCORE_EXPORT
)

configure_file(data/org.kde.kdenow.event.service.in ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.event.service)
configure_file(data/org.kde.kdenow.flight.service.in ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.flight.service)
configure_file(data/org.kde.kdenow.hotel.service.in ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.hotel.service)
configure_file(data/org.kde.kdenow.restaurant.service.in ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.restaurant.service)

install(TARGETS kdenowd DESTINATION ${LIBEXEC_INSTALL_DIR})
install(TARGETS kdenowcore EXPORT KDENowCoreTargets
        ${INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)
install(EXPORT KDENowCoreTargets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(FILES data/servicetypes/kdenow_plugins.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.event.service DESTINATION ${DBUS_SERVICES_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.flight.service DESTINATION ${DBUS_SERVICES_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.hotel.service DESTINATION ${DBUS_SERVICES_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.restaurant.service DESTINATION ${DBUS_SERVICES_INSTALL_DIR})