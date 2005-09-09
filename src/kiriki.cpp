/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <stdlib.h>

#include <QHBoxLayout>
#include <QHeaderView>
#include <QTimer>
#include <QTreeView>

#include <kapplication.h>
#include <kscoredialog.h>
#include <kstdgameaction.h>

#include "computer.h"
#include "kiriki.h"
#include "lateralwidget.h"
#include "scores.h"

kiriki::kiriki() : KMainWindow()
{
	QWidget *w = new QWidget(this);
	QHBoxLayout *lay = new QHBoxLayout(w);
	
	m_lateral = new lateralWidget(w);
	lay -> addWidget(m_lateral);
	
	m_scoresWidget = new QTreeView(w);
	m_scoresWidget -> setAlternatingRowColors(true);
	m_scoresWidget -> setSelectionBehavior(QAbstractItemView::SelectRows);
	connect(m_scoresWidget, SIGNAL(pressed(const QModelIndex &)), this, SLOT(pressed(const QModelIndex &)));
	lay -> addWidget(m_scoresWidget, 1);
	
	m_scores = 0;
	
	KStdGameAction::gameNew(this, SLOT(newGame()), actionCollection(), "newGame");
	KStdGameAction::highscores(this, SLOT(showHighScores()), actionCollection(), "showHS");
	KStdGameAction::quit(kapp, SLOT(quit()), actionCollection(), "quit");
	
	setCentralWidget(w);
	setupGUI(Keys | Save | Create);
	show();
	
	newGame();
}

void kiriki::pressed(const QModelIndex &index)
{
	if (!m_scores -> currentPlayer().isHuman()) return;

	play(index);
}

void kiriki::play(const QModelIndex &index)
{
	int score = -1;
	if (index.row() == 0) score = m_lateral -> getOnes();
	else if (index.row() == 1) score = m_lateral -> getTwos();
	else if (index.row() == 2) score = m_lateral -> getThrees();
	else if (index.row() == 3) score = m_lateral -> getFours();
	else if (index.row() == 4) score = m_lateral -> getFives();
	else if (index.row() == 5) score = m_lateral -> getSixs();
	else if (index.row() == 9) score = m_lateral -> getThreeOfAKind();
	else if (index.row() == 10) score = m_lateral -> getFourOfAKind();
	else if (index.row() == 11) score = m_lateral -> getFullHouse();
	else if (index.row() == 12) score = m_lateral -> getSStraight();
	else if (index.row() == 13) score = m_lateral -> getLStraight();
	else if (index.row() == 14) score = m_lateral -> getKiriki();
	else if (index.row() == 15) score = m_lateral -> totalSum();
	else return;
	
	Q_ASSERT(score != -1);
	if (m_scores -> setData(index, score, Qt::EditRole)) nextTurn();
}

void kiriki::newGame()
{
	delete m_scores;
	m_scores = new scores();
	m_scoresWidget -> setModel(m_scores);
	m_lateral -> nextTurn();

	int hWidth = 0;
	m_scoresWidget -> header() -> setStretchLastSection(false);
	for (int i = 0; i < m_scores -> columnCount(QModelIndex()); i++)
	{
		m_scoresWidget -> resizeColumnToContents(i);
		hWidth += m_scoresWidget -> header() -> sectionSize(i);
	}
	m_scoresWidget -> header() -> setMovable(false);
	m_scoresWidget -> header() -> setResizeMode(QHeaderView::Custom);
	m_scoresWidget -> setMinimumSize(hWidth, 0);
	m_scoresWidget -> header() -> setStretchLastSection(true);
//	m_scoresWidget -> setMaximumSize(hWidth + 5, 0);
//	setFixedSize(hWidth + 5 + sizeHint().width() - m_scoresWidget -> sizeHint().width(), sizeHint().height());
}

void kiriki::endGame()
{
	const player &p = m_scores -> winner();
	m_lateral->setEnabled(false);
	if (p.isHuman())
	{
		KScoreDialog sc(KScoreDialog::Name | KScoreDialog::Score, this);
		if (sc.addScore(p.grandTotal(), KScoreDialog::FieldInfo()))
		{
			sc.exec();
		}
	}
}

void kiriki::showHighScores()
{
	KScoreDialog sc(KScoreDialog::Name | KScoreDialog::Score, this);
	sc.exec();
}

void kiriki::nextTurn()
{
	if (m_scores -> allScores()) endGame();
	else
	{
		m_scores -> nextPlayer();

		while (m_scores -> currentPlayer().allScores()) m_scores -> nextPlayer();
		
		m_lateral -> nextTurn();
		if (!m_scores -> currentPlayer().isHuman())
		{
			m_lateral -> setEnabled(false);

			for (int i = 0; i < 5; i++) setComputerDiceValue(i, m_lateral -> getDice(i));
			ComputerRolling(m_scores -> currentPlayer(), m_lateral -> getRolls());

			while ( computerDiceSelected() && m_lateral -> getRolls() < 3)
			{
				for (int i = 0; i < 5; i++) m_lateral -> selectDice(i, getComputerDiceSelected(i));

				m_lateral -> roll();
				for (int i = 0; i < 5; i++) setComputerDiceValue(i, m_lateral -> getDice(i));
				ComputerRolling(m_scores -> currentPlayer(), m_lateral -> getRolls());
			}
			int row;
			row = ComputerScoring(m_scores -> currentPlayer());
			if (row > 5) row += 3;
			play(m_scores -> index(row, 0));
		}
		else m_lateral -> setEnabled(true);
	 }
}

#include "kiriki.moc"
