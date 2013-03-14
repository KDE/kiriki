/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <aacid@kde.org>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "configwidget.h"

#include <kacceleratormanager.h>

configWidget::configWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	KAcceleratorManager::setNoAccel(this);
	connect(kcfg_numberOfPlayers, SIGNAL(valueChanged(int)), this, SLOT(numberOfPlayersChanged(int)));
}

void configWidget::numberOfPlayersChanged(int numberOfPlayers)
{
	kcfg_player3Name -> setEnabled(true);
	kcfg_player3IsHuman -> setEnabled(true);
	kcfg_player4Name -> setEnabled(true);
	kcfg_player4IsHuman -> setEnabled(true);
	kcfg_player5Name -> setEnabled(true);
	kcfg_player5IsHuman -> setEnabled(true);
	kcfg_player6Name -> setEnabled(true);
	kcfg_player6IsHuman -> setEnabled(true);
	if (numberOfPlayers < 3)
	{
		kcfg_player3Name -> setEnabled(false);
		kcfg_player3IsHuman -> setEnabled(false);
	}
	if (numberOfPlayers < 4)
	{
		kcfg_player4Name -> setEnabled(false);
		kcfg_player4IsHuman -> setEnabled(false);
	}
	if (numberOfPlayers < 5)
	{
		kcfg_player5Name -> setEnabled(false);
		kcfg_player5IsHuman -> setEnabled(false);
	}
	if (numberOfPlayers < 6)
	{
		kcfg_player6Name -> setEnabled(false);
		kcfg_player6IsHuman -> setEnabled(false);
	}
}

#include "configwidget.moc"

