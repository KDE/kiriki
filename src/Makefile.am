INCLUDES = $(all_includes)

bin_PROGRAMS = kiriki
kiriki_SOURCES = main.cpp kiriki.cpp player.cpp scores.cpp diceswidget.cpp lateralwidget.cpp computer.cpp settings.kcfgc configPage.ui configwidget.cpp
kiriki_LDADD = $(LIB_KDEUI) -lkdegames
kiriki_LDFLAGS = $(all_libraries)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/kiriki
rc_DATA = kirikiui.rc
