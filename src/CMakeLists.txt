add_executable(kdenow
    main.cpp
    daemon.cpp
    processor.cpp
    parser.cpp
    abstractreservationplugin.cpp
    pluginsloader.cpp
    singletonfactory.cpp
    datamap.cpp
)

target_link_libraries(kdenow
    Qt5::Core
    Qt5::Xml
    KF5::IMAP
    KF5::KIOCore
    KF5::CoreAddons
    KF5::Service
)

set_target_properties(
    kdenow PROPERTIES ENABLE_EXPORTS ON
)

install(TARGETS kdenow ${INSTALL_TARGETS_DEFAULT_ARGS})
