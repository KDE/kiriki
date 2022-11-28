cmake_minimum_required (VERSION 3.16 FATAL_ERROR)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "23")
set(RELEASE_SERVICE_VERSION_MINOR "03")
set(RELEASE_SERVICE_VERSION_MICRO "70")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kiriki VERSION ${RELEASE_SERVICE_VERSION})

set (QT_MIN_VERSION "5.15.2")
set (KF_MIN_VERSION "5.91.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} )

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMAddAppIcon)
include(ECMInstallIcons)
include(ECMSetupVersion)
include(ECMDeprecationSettings)
include(FeatureSummary)

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets PrintSupport)

find_package(KF5 ${KF_MIN_VERSION} REQUIRED COMPONENTS
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

ecm_set_disabled_deprecation_versions(
    QT 5.15.2
    KF 5.91
    KDEGAMES 7.3
)

add_subdirectory(src)
add_subdirectory(images)
add_subdirectory(icons)
add_subdirectory(doc)

ki18n_install(po)
kdoctools_install(po)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
