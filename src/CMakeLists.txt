
include_directories( ${CMAKE_SOURCE_DIR}/libkdegames/highscore )

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
   )

kde4_add_ui_files(kiriki_SRCS configPage.ui)

kde4_add_kcfg_files(kiriki_SRCS settings.kcfgc)

kde4_add_app_icon(kiriki_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-kiriki.png")
kde4_add_executable(kiriki ${kiriki_SRCS})

target_link_libraries(kiriki ${KDE4_KDEUI_LIBS} kdegames)

install(TARGETS kiriki ${INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############

install(FILES kirikiui.rc DESTINATION ${DATA_INSTALL_DIR}/kiriki )
install(FILES kiriki.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
