cmake_minimum_required (VERSION 3.16 FATAL_ERROR)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "21")
set(RELEASE_SERVICE_VERSION_MINOR "08")
set(RELEASE_SERVICE_VERSION_MICRO "0")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kiriki VERSION ${RELEASE_SERVICE_VERSION})

set (QT_MIN_VERSION "5.15.0")
set (KF5_MIN_VERSION "5.83.0")

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
    
find_package(KF5KDEGames 7.3.0 REQUIRED)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(FeatureSummary)
include(ECMAddAppIcon)
include(ECMInstallIcons)
include(ECMSetupVersion)

add_definitions(
    -DQT_NO_FOREACH
    -DQT_NO_KEYWORDS
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_USE_QSTRINGBUILDER
    -DQT_STRICT_ITERATORS
    -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT
)

add_subdirectory(src)
add_subdirectory(images)
add_subdirectory(icons)
add_subdirectory(doc)

ki18n_install(po)
kdoctools_install(po)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
