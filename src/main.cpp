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
#define HAVE_KICONTHEME __has_include(<KIconTheme>)
#if HAVE_KICONTHEME
#include <KIconTheme>
#endif

#define HAVE_STYLE_MANAGER __has_include(<KStyleManager>)
#if HAVE_STYLE_MANAGER
#include <KStyleManager>
#endif
#include "kiriki.h"

#include "kiriki_version.h"

int main(int argc, char *argv[])
{
#if HAVE_KICONTHEME
    KIconTheme::initTheme();
#endif
    QApplication app(argc, argv);
#if HAVE_STYLE_MANAGER
    KStyleManager::initStyle();
#else // !HAVE_STYLE_MANAGER
#if defined(Q_OS_MACOS) || defined(Q_OS_WIN)
    QApplication::setStyle(QStringLiteral("breeze"));
#endif // defined(Q_OS_MACOS) || defined(Q_OS_WIN)
#endif // HAVE_STYLE_MANAGER
	KLocalizedString::setApplicationDomain(QByteArrayLiteral("kiriki"));

	KAboutData about(QStringLiteral("kiriki"), i18n("Kiriki"),
            QStringLiteral(KIRIKI_VERSION_STRING),
            i18n("A Yahtzee dice game by KDE"),
            KAboutLicense::GPL,
            i18n("© 2005-2007 Albert Astals Cid"),
            QString(),
            QStringLiteral("https://apps.kde.org/kiriki"));
	about.addAuthor(i18n("Albert Astals Cid"), QString(), QStringLiteral("aacid@kde.org"));

	KAboutData::setApplicationData(about);
	QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("kiriki")));

	KCrash::initialize();

	QCommandLineParser parser;
	about.setupCommandLine(&parser);
	parser.process(app);
	about.processCommandLine(&parser);

	KDBusService service;
	kiriki *w = new kiriki();
	w->show();
	return app.exec();
}
