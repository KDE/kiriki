/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <aacid@kde.org>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include <QWidget>

#include "ui_configPage.h"

class configWidget : public QWidget, private Ui::configPage
{
Q_OBJECT
	public:
		explicit configWidget(QWidget *parent);
	
	private Q_SLOTS:
		void numberOfPlayersChanged(int numberOfPlayers);
		void minimumRowHeightChanged(int height);
};

#endif

