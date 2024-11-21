ecm_setup_version(${RELEASE_SERVICE_VERSION}
    VARIABLE_PREFIX KIRIKI
    VERSION_HEADER kiriki_version.h
)

add_executable(kiriki)

target_sources(kiriki PRIVATE
    main.cpp
    kiriki.cpp
    player.cpp
    scores.cpp
    diceswidget.cpp
    lateralwidget.cpp
    computer.cpp
    configwidget.cpp
    row.cpp
    itemdelegate.cpp

    kiriki.qrc
)

ki18n_wrap_ui(kiriki configPage.ui)

kconfig_add_kcfg_files(kiriki settings.kcfgc)

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/*-apps-kiriki.png")
ecm_add_app_icon(kiriki ICONS ${ICONS_SRCS})

target_link_libraries(kiriki
    KDEGames6
    KF6::CoreAddons
    KF6::XmlGui
    KF6::I18n
    KF6::IconThemes
    KF6::DBusAddons
    KF6::Crash
    KF6::IconThemes
    Qt6::PrintSupport
)

install(TARGETS kiriki ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(PROGRAMS org.kde.kiriki.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install(FILES org.kde.kiriki.appdata.xml  DESTINATION  ${KDE_INSTALL_METAINFODIR} )
