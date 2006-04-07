kde4_header()

include_directories(${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

########### next target ###############

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

target_link_libraries(kiriki ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} kdegames)

install_targets(/bin kiriki)


########### install files ###############

install_files(/share/apps/kiriki FILES kirikiui.rc)

kde4_footer()

#original Makefile.am contents follow:

#INCLUDES = $(all_includes)
#
#bin_PROGRAMS = kiriki
#kiriki_SOURCES = main.cpp kiriki.cpp player.cpp scores.cpp diceswidget.cpp lateralwidget.cpp computer.cpp settings.kcfgc configPage.ui configwidget.cpp
#kiriki_LDADD = $(LIB_KDEUI) -lkdegames
#kiriki_LDFLAGS = $(all_libraries)
#
## let automoc handle all of the meta source files (moc)
#METASOURCES = AUTO
#
## this is where the XML-GUI resource file goes
#rcdir = $(kde_datadir)/kiriki
#rc_DATA = kirikiui.rc
