
include_directories( ${CMAKE_SOURCE_DIR}/libkdegames/highscore )

set(kiriki_SRCS 
   main.cpp 
   kiriki.cpp 
   player.cpp 
   scores.cpp 
   diceswidget.cpp 
   lateralwidget.cpp 
   computer.cpp 
   configwidget.cpp )

kde4_add_ui_files(kiriki_SRCS configPage.ui)

kde4_add_kcfg_files(kiriki_SRCS settings.kcfgc)

set(kiriki_icons
    ${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi16-app-kiriki.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi32-app-kiriki.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi48-app-kiriki.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi64-app-kiriki.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi128-app-kiriki.png
)
kde4_add_win32_app_icon(kiriki_SRCS ${kiriki_icons})
kde4_add_executable(kiriki ${kiriki_SRCS})

target_link_libraries(kiriki ${KDE4_KDEUI_LIBS} kdegames)

install(TARGETS kiriki DESTINATION ${BIN_INSTALL_DIR})


########### install files ###############

install(FILES kirikiui.rc DESTINATION ${DATA_INSTALL_DIR}/kiriki )
install(FILES kiriki.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
