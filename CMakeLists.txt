cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "21")
set(RELEASE_SERVICE_VERSION_MINOR "03")
set(RELEASE_SERVICE_VERSION_MICRO "70")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kiriki VERSION ${RELEASE_SERVICE_VERSION})

set (QT_MIN_VERSION "5.10.0")
set (KF5_MIN_VERSION "5.55.0")

find_package(ECM ${KF5_REQUIRED_VERSION} REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} )

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets PrintSupport)
find_package(KF5 ${KF5_REQUIRED_VERSION} REQUIRED COMPONENTS
    CoreAddons 
    Config 
    Crash
    DBusAddons 
    DocTools
    I18n 
    ConfigWidgets 
    IconThemes 
    XmlGui
)
    
find_package(KF5KDEGames 4.9.0 REQUIRED)

include(FeatureSummary)
include(ECMAddAppIcon)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(ECMSetupVersion)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
if (${KF5Config_VERSION} STRGREATER "5.56.0")
        add_definitions(-DQT_NO_FOREACH)
        MESSAGE(STATUS "compile without foreach")
endif()

add_subdirectory(src)
add_subdirectory(images)
add_subdirectory(icons)
add_subdirectory(doc)
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
