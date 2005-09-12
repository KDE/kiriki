/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include <QWidget>

#include "configPage.h"

class configWidget : public QWidget, private Ui::configPage
{
Q_OBJECT
	public:
		configWidget(QWidget *parent);
	
	private slots:
		void numberOfPlayersChanged(int numberOfPlayers);
};

#endif

