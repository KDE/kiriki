/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>

#include "kiriki.h"

int main(int argc, char *argv[])
{
	KAboutData about("kiriki", I18N_NOOP("Kiriki"), "0.1", I18N_NOOP("A dice game"), KAboutData::License_GPL, "© 2005 Albert Astals Cid");
	about.addAuthor("Albert Astals Cid", "", "aacid@kde.org");
	KCmdLineArgs::init(argc, argv, &about);
	KApplication app;
	app.setTopWidget(new kiriki());
	return app.exec();
}
