project(kdenow)
cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 0.0.9 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Qt5 REQUIRED NO_MODULE COMPONENTS Qml Quick Sql DBus)
find_package(KF5 5.2 REQUIRED COMPONENTS Plasma I18n Declarative)
find_package(KF5 REQUIRED COMPONENTS IMAP Mime KIO)
find_package(KF5Config REQUIRED)
find_package(KF5CoreAddons REQUIRED)
find_package(KF5Service REQUIRED)

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(GenerateExportHeader)
include(ECMInstallIcons)


include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
)

add_subdirectory(src)
add_subdirectory(reservations)
add_subdirectory(qml)

# Space for other includes like src, libs etc

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
