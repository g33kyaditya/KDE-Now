set (qml_plugins_SRCS
    qmlextensionsplugin.cpp
)

add_library(kdenowqmlplugin SHARED ${qml_plugins_SRCS})

target_link_libraries(kdenowqmlplugin
    Qt5::Qml
)

install (TARGETS kdenowqmlplugin DESTINATION ${CMAKE_SOURCE_DIR}/bin/org/kde/now)
install (FILES qmldir DESTINATION ${CMAKE_SOURCE_DIR}/bin/org/kde/now)
