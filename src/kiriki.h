/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KIRIKI_H
#define KIRIKI_H
 
#include <kmainwindow.h>

class QModelIndex;
class QTreeView;

class lateralWidget;
class scores;

class kiriki : public KMainWindow
{
Q_OBJECT
	public:
		kiriki();
	
	private slots:
		void newGame();
		void pressed(const QModelIndex &index);
		void showHighScores();
		void showPreferences();
		void playComputer();
	
	private:
		void endGame();
		void play(const QModelIndex &index);
		void nextTurn();

		QTreeView *m_scoresWidget;
		scores *m_scores;
		lateralWidget *m_lateral;
};

#endif
