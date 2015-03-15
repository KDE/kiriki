

set(kiriki_SRCS 
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
   )

ki18n_wrap_ui(kiriki_SRCS configPage.ui)

kconfig_add_kcfg_files(kiriki_SRCS settings.kcfgc)

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/*-apps-kiriki.png")
ecm_add_app_icon(kiriki_SRCS ICONS ${ICONS_SRCS})
add_executable(kiriki ${kiriki_SRCS})

target_link_libraries(kiriki KF5KDEGames KF5::XmlGui KF5::I18n KF5::IconThemes KF5::DBusAddons)

install(TARGETS kiriki ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############

install(FILES kirikiui.rc DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/kiriki )
install(PROGRAMS org.kde.kiriki.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )

