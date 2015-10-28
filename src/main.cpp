/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <aacid@kde.org>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <KAboutData>


#include <KLocalizedString>
#include <QApplication>
#include <QCommandLineParser>
#include <KDBusService>
#include "kiriki.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	KLocalizedString::setApplicationDomain("kiriki");
	KAboutData about(QStringLiteral("kiriki"), i18n("Kiriki"), QStringLiteral("0.3.0"), i18n("A Yahtzee dice game for KDE"), KAboutLicense::GPL, i18n("© 2005-2007 Albert Astals Cid"), QStringLiteral("http://games.kde.org/kiriki") );
	about.addAuthor(i18n("Albert Astals Cid"), QString(), QStringLiteral("aacid@kde.org"));
	QCommandLineParser parser;
	KAboutData::setApplicationData(about);
	parser.addVersionOption();
	parser.addHelpOption();
	about.setupCommandLine(&parser);
	parser.process(app);
	about.processCommandLine(&parser);
	KDBusService service;
	kiriki *w = new kiriki();
	w->show();
	return app.exec();
}
