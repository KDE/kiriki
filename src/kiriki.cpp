/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "kiriki.h"

#include <stdlib.h>

#include <QHBoxLayout>
#include <QHeaderView>
#include <QPainter>
#include <QTimer>
#include <QTreeView>

#include <kapplication.h>
#include <kconfigdialog.h>
#include <kmessagebox.h>
#include <kprinter.h>
#include <kscoredialog.h>
#include <kstandardaction.h>
#include <kstandardgameaction.h>
#include <kactioncollection.h>
#include <kaction.h>

#include "computer.h"
#include "configwidget.h"
#include "lateralwidget.h"
#include "scores.h"
#include "settings.h"

kiriki::kiriki() : KXmlGuiWindow()
{
	QWidget *w = new QWidget(this);
	QHBoxLayout *lay = new QHBoxLayout(w);
	
	m_lateral = new lateralWidget(w);
	lay -> addWidget(m_lateral);
	
	m_scoresWidget = new QTreeView(w);
	m_scoresWidget -> setSelectionBehavior(QAbstractItemView::SelectRows);
	m_scoresWidget -> setRootIsDecorated(false);
	m_scoresWidget -> header() -> setResizeMode(QHeaderView::Stretch);
	m_scoresWidget -> header() -> setMovable(false);
	m_scoresWidget -> header() -> setStretchLastSection(false);
	m_scoresWidget -> setItemsExpandable(false);
	connect(m_scoresWidget, SIGNAL(pressed(const QModelIndex &)), this, SLOT(pressed(const QModelIndex &)));
	lay -> addWidget(m_scoresWidget, 1);
	
	m_scores = 0;
	
	// Game
	KStandardGameAction::gameNew(this, SLOT(newGame()), actionCollection());
	KStandardGameAction::highscores(this, SLOT(showHighScores()), actionCollection());
	KStandardGameAction::print(this, SLOT(print()), actionCollection());
	KStandardGameAction::quit(kapp, SLOT(quit()), actionCollection());
	
	// Preferences
	KStandardAction::preferences(this, SLOT(showPreferences()), actionCollection());
	
	setCentralWidget(w);
	setupGUI(Keys | Save | Create);
	show();
	
	newGame();
}

void kiriki::pressed(const QModelIndex &index)
{
	if (!m_scores -> currentPlayer().isHuman()) return;

	if (index.column() == 0 || index.column() == m_scores -> currentPlayerNumber() + 1) play(index);
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

	m_scoresWidget -> header() -> setResizeMode(0, QHeaderView::Custom);
	m_scoresWidget -> resizeColumnToContents(0);
}

void kiriki::endGame()
{
	const player &p = m_scores -> winner();
	m_lateral->setEnabled(false);
	if (p.isHuman())
	{
		KScoreDialog sc(KScoreDialog::Name | KScoreDialog::Score, this);
		if (sc.addScore(p.grandTotal()))
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

void kiriki::showPreferences()
{
	if(KConfigDialog::showDialog("settings"))
		return;

	int nPlayers = kirikiSettings::numberOfPlayers();
	QString player1Name = kirikiSettings::player1Name();
	QString player2Name = kirikiSettings::player2Name();
	QString player3Name = kirikiSettings::player3Name();
	QString player4Name = kirikiSettings::player4Name();
	QString player5Name = kirikiSettings::player5Name();
	QString player6Name = kirikiSettings::player6Name();
	bool player2IsHuman = kirikiSettings::player2IsHuman();
	bool player3IsHuman = kirikiSettings::player3IsHuman();
	bool player4IsHuman = kirikiSettings::player4IsHuman();
	bool player5IsHuman = kirikiSettings::player5IsHuman();
	bool player6IsHuman = kirikiSettings::player6IsHuman();
	
	KConfigDialog *configDialog = new KConfigDialog(this, "settings", kirikiSettings::self());
	configDialog->setFaceType(KConfigDialog::Plain);
	configDialog -> addPage(new configWidget(configDialog), QString(), QString());
	configDialog -> exec();
	delete configDialog;

	bool changed = nPlayers != kirikiSettings::numberOfPlayers() ||
	               player1Name != kirikiSettings::player1Name() || 
	               player2Name != kirikiSettings::player2Name() || 
	               player3Name != kirikiSettings::player3Name() || 
	               player4Name != kirikiSettings::player4Name() || 
	               player5Name != kirikiSettings::player5Name() || 
	               player6Name != kirikiSettings::player6Name() || 
		       player2IsHuman != kirikiSettings::player2IsHuman() ||
		       player3IsHuman != kirikiSettings::player3IsHuman() ||
		       player4IsHuman != kirikiSettings::player4IsHuman() ||
		       player5IsHuman != kirikiSettings::player5IsHuman() ||
		       player6IsHuman != kirikiSettings::player6IsHuman();
	if (changed)
	{
		KMessageBox::information(this, i18n("Changes will be applied on next game."));
	}
}

void kiriki::print()
{
	KPrinter printer;
	printer.setFullPage( true );
	if ( printer.setup( this ) )
	{
		QPainter painter(&printer);
		m_scores->print(painter, printer.width(), printer.height());
	}
}

void kiriki::nextTurn()
{
	if (m_scores -> allScores()) endGame();
	else
	{
		m_scores -> nextPlayer();

		while (m_scores -> currentPlayer().allScores()) m_scores -> nextPlayer();
		
		m_lateral -> nextTurn();
		if (!m_scores -> currentPlayer().isHuman()) QTimer::singleShot(kirikiSettings::waitTime(), this, SLOT(playComputer()));
		else m_lateral -> setEnabled(true);
	 }
}

void kiriki::playComputer()
{
	m_lateral -> setEnabled(false);
	for (int i = 0; i < 5; i++) setComputerDiceValue(i, m_lateral -> getDice(i));
	ComputerRolling(m_scores -> currentPlayer(), m_lateral -> getRolls());
	while (computerDiceSelected() && m_lateral -> getRolls() < 3)
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

#include "kiriki.moc"
