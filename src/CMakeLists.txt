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

set_target_properties(
    kdenowd PROPERTIES ENABLE_EXPORTS ON
)

target_include_directories(kdenowcore PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS kdenowd ${INSTALL_TARGETS_DEFAULT_ARGS})
install(TARGETS kdenowcore EXPORT kdenowLibraryTargets
        ${INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

install(FILES data/servicetypes/kdenow_plugins.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})