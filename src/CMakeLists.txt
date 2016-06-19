add_executable(kdenow
    main.cpp
    daemon.cpp
    processor.cpp
    parser.cpp
    abstractreservationplugin.cpp
)

target_link_libraries(kdenow
    Qt5::Core
    Qt5::Xml
    KF5::IMAP
    KF5::KIOCore
    KF5::CoreAddons
    KF5::Service
)

install(TARGETS kdenow ${INSTALL_TARGETS_DEFAULT_ARGS})
