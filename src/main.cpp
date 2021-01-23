/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <aacid@kde.org>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <KAboutData>
#include <KCrash>

#include <KLocalizedString>
#include <QApplication>
#include <QCommandLineParser>
#include <KDBusService>
#include "kiriki.h"

#include "kiriki_version.h"

int main(int argc, char *argv[])
{
	// Fixes blurry icons with fractional scaling
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);

	KLocalizedString::setApplicationDomain("kiriki");
	KAboutData about(QStringLiteral("kiriki"), i18n("Kiriki"),
            QStringLiteral(KIRIKI_VERSION_STRING),
            i18n("A Yahtzee dice game by KDE"),
            KAboutLicense::GPL,
            i18n("© 2005-2007 Albert Astals Cid"));
	about.addAuthor(i18n("Albert Astals Cid"), QString(), QStringLiteral("aacid@kde.org"));
	about.setHomepage(QStringLiteral("https://kde.org/applications/games/org.kde.kiriki"));
	QCommandLineParser parser;
	KAboutData::setApplicationData(about);
	KCrash::initialize();
	about.setupCommandLine(&parser);
	parser.process(app);
	about.processCommandLine(&parser);
	KDBusService service;
	kiriki *w = new kiriki();
	w->show();
	app.setWindowIcon(QIcon::fromTheme(QStringLiteral("kiriki")));
	return app.exec();
}
