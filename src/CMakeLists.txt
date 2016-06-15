set (kdenow_SRCS
    main.cpp
    daemon.cpp
    processor.cpp
    parser.cpp
)

add_library(kdenow SHARED
    ${kdenow_SRCS}
)

set(kdenow_link_libs
    Qt5::Core
    Qt5::Xml
    KF5::IMAP
    KF5::KIOCore
    KF5::CoreAddons
    KF5::ConfigCore
    KF5::Service
)

target_link_libraries(kdenow
    ${kdenow_link_libs}
)

install(TARGETS kdenow ${INSTALL_TARGETS_DEFAULT_ARGS})
