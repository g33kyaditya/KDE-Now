add_executable(kdenowd
    main.cpp
    walletmanager.cpp
)

set(dbus_SRCS org.kde.kdenow.xml)

qt5_generate_dbus_interface(daemon.h
                            org.kde.kdenow.xml
                            OPTIONS -M
                            )

qt5_add_dbus_adaptor(dbus_SRCS
                     ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.xml
                     daemon.h
                     Daemon
                    )

add_library(kdenowcore
    daemon.cpp
    processor.cpp
    parser.cpp
    abstractreservationplugin.cpp
    pluginsloader.cpp
    singletonfactory.cpp
    datamap.cpp
    ${dbus_SRCS}
)

target_link_libraries(kdenowcore
PUBLIC
    Qt5::Core
    Qt5::DBus
    Qt5::Xml
    KF5::IMAP
    KF5::KIOCore
    KF5::CoreAddons
    KF5::Service
    KF5::Wallet
)

target_link_libraries(kdenowd
    kdenowcore
    Qt5::Quick
    Qt5::Gui
    Qt5::Widgets
)

generate_export_header(kdenowcore
    EXPORT_FILE_NAME kdenowcore_export.h
    EXPORT_MACRO_NAME KDENOWCORE_EXPORT
)

configure_file(data/org.kde.kdenow.service.in ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.service)

install(TARGETS kdenowd DESTINATION ${BIN_INSTALL_DIR})
install(TARGETS kdenowcore EXPORT KDENowCoreTargets
        ${INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)
install(EXPORT KDENowCoreTargets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(FILES data/servicetypes/kdenow_plugins.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.xml DESTINATION ${DBUS_INTERFACES_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kdenow.service DESTINATION ${DBUS_SERVICES_INSTALL_DIR})
