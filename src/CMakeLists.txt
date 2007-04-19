
include_directories( ${CMAKE_SOURCE_DIR}/libkdegames ${CMAKE_SOURCE_DIR}/libkdegames/highscore )

set(kiriki_SRCS 
   main.cpp 
   kiriki.cpp 
   player.cpp 
   scores.cpp 
   diceswidget.cpp 
   lateralwidget.cpp 
   computer.cpp 
   configwidget.cpp )

kde4_automoc(${kiriki_SRCS})

kde4_add_ui_files(kiriki_SRCS configPage.ui)

kde4_add_kcfg_files(kiriki_SRCS settings.kcfgc)

kde4_add_executable(kiriki ${kiriki_SRCS})

target_link_libraries(kiriki ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KDEPRINT_LIBS} kdegames)

install(TARGETS kiriki DESTINATION ${BIN_INSTALL_DIR})


########### install files ###############

install(FILES kirikiui.rc DESTINATION ${DATA_INSTALL_DIR}/kiriki )

