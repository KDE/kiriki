/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <aacid@kde.org>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KIRIKI_H
#define KIRIKI_H
 
#include <KXmlGuiWindow>

class QModelIndex;
class QStyledItemDelegate;
class QTreeView;

class QAction;
class KToggleAction;

class lateralWidget;
class scores;
class itemDelegate;

class kiriki : public KXmlGuiWindow
{
Q_OBJECT
	public:
		kiriki();

	Q_SIGNALS:
		void demoStarted(bool t = true);
	
	private Q_SLOTS:
		void newGame();
		void demo();
		void pressed(const QModelIndex &index);
		void showHighScores();
		void showPreferences();
		void print();
		void playComputer();
		void showHint();
		void settingsChanged();
	
	private:
		void endGame();
		void play(const QModelIndex &index);
		void play(int scoreRow);
		void nextTurn();

		QTreeView *m_scoresWidget;
		scores *m_scores;
		lateralWidget *m_lateral;
		KToggleAction *m_demoAction;
		QStyledItemDelegate *m_delegateHighlighted;
		int m_highlightedRowIndex;
		bool m_hintGiven;
		QAction * m_hintAction;
		int m_fontSize;
		itemDelegate *m_itemDelegate;
		int m_rowHeight;
};

#endif
