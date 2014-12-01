project(kiriki)

cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)
set (QT_MIN_VERSION "5.3.0")

find_package(ECM 1.3.0 REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets Qml Quick QuickWidgets Svg Test)
find_package(KF5 REQUIRED COMPONENTS 
    CoreAddons 
    Config 
    ItemModels 
    WidgetsAddons 
    WindowSystem 
    Codecs 
    Archive
    Config 
    DBusAddons 
    DNSSD 
    I18n 
    GuiAddons 
    Service 
    ConfigWidgets 
    ItemViews 
    IconThemes 
    Completion 
    XmlGui
    KIO
    NotifyConfig
    KDELibs4Support)
    
find_package(KF5KDEGames 4.9.0 REQUIRED)

include(FeatureSummary)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(-DTRANSLATION_DOMAIN="kiriki")


add_subdirectory(src)
add_subdirectory(images)
add_subdirectory(icons)
add_subdirectory(doc)
