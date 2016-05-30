add_executable(kdenow
    main.cpp
    emailfetchjob.cpp
    emailsearchjob.cpp
    emailselectjob.cpp
    emailsessionjob.cpp
    singletonfactory.cpp
)

target_link_libraries(kdenow
    Qt5::Core
    Qt5::Xml
    KF5::IMAP
    KF5::KIOCore
    KF5::CoreAddons
)

install(TARGETS kdenow ${INSTALL_TARGETS_DEFAULT_ARGS})
