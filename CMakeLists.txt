project(kdenow)
cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 0.0.9 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${ECM_MODULE_PATH}")

find_package(Qt5 REQUIRED NO_MODULE COMPONENTS Qml Quick)
find_package(KF5 5.5 REQUIRED NO_MODULE COMPONENTS Plasma I18n Declarative)
find_package(KF5 REQUIRED COMPONENTS Imap Mime)

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(GenerateExportHeader)
include(ECMInstallIcons)




# Space for other includes like src, libs etc





feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
