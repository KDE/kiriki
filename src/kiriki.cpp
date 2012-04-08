/***************************************************************************
 *   Copyright (C) 2005-2007 by Albert Astals Cid <aacid@kde.org>          *
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
#include <QItemDelegate>
#include <QItemSelectionModel>
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
#include <QStyledItemDelegate>
#include <QTimer>
#include <QTreeView>
#include <QDateTime>

#include <kapplication.h>
#include <kconfigdialog.h>
#include <klocalizedstring.h>
#include <kmessagebox.h>
#include <kscoredialog.h>
#include <kstandardaction.h>
#include <kstandardgameaction.h>
#include <kactioncollection.h>
#include <kaction.h>
#include <KStatusBar>
#include <KToggleAction>
#include <KToolBar>
#include <kdeprintdialog.h>

#include "computer.h"
#include "configwidget.h"
#include "lateralwidget.h"
#include "scores.h"
#include "settings.h"

kiriki::kiriki() : KXmlGuiWindow(), m_hintGiven(false)
{
	QWidget *w = new QWidget(this);
	QHBoxLayout *lay = new QHBoxLayout(w);
	
	m_lateral = new lateralWidget(w);
	lay -> addWidget(m_lateral);
	
	m_scoresWidget = new QTreeView(w);

	m_delegateHighlighted = new QStyledItemDelegate(m_scoresWidget);

	m_scoresWidget -> setItemDelegate(new QItemDelegate(m_scoresWidget));
	m_scoresWidget -> setSelectionBehavior(QAbstractItemView::SelectRows);
	m_scoresWidget -> setRootIsDecorated(false);
	m_scoresWidget -> header() -> setResizeMode(QHeaderView::Stretch);
	m_scoresWidget -> header() -> setMovable(false);
	m_scoresWidget -> header() -> setStretchLastSection(false);
	m_scoresWidget -> header() -> hide();
	m_scoresWidget -> setItemsExpandable(false);

	// set minimum section size so that you don't get ... when making the widget small
	QFont f = m_scoresWidget -> font();
	f.setBold(true);
	f.setPointSize(f.pointSize() + 5);
	QFontMetrics fm(f);
	m_scoresWidget -> header() -> setMinimumSectionSize(fm.width("9999"));

	connect(m_scoresWidget, SIGNAL(pressed(QModelIndex)), this, SLOT(pressed(QModelIndex)));
	lay -> addWidget(m_scoresWidget, 1);
	
	m_scores = 0;
	
	// Game
	KAction *gameNewAction = KStandardGameAction::gameNew(this, SLOT(newGame()), actionCollection());
	KStandardGameAction::highscores(this, SLOT(showHighScores()), actionCollection());
	KStandardGameAction::print(this, SLOT(print()), actionCollection());
	KStandardGameAction::quit(kapp, SLOT(quit()), actionCollection());
	m_hintAction = KStandardGameAction::hint(this, SLOT(showHint()), actionCollection());
	m_demoAction = KStandardGameAction::demo(this, SLOT(demo()), actionCollection());
	connect(gameNewAction, SIGNAL(triggered(bool)), m_demoAction, SLOT(setChecked(bool)));
	connect(gameNewAction, SIGNAL(triggered(bool)), m_demoAction, SLOT(setDisabled(bool)));
	connect(gameNewAction, SIGNAL(triggered(bool)), m_hintAction, SLOT(setDisabled(bool)));
	connect(gameNewAction, SIGNAL(triggered(bool)), m_lateral, SLOT(disableDemoMode()));
	connect(gameNewAction, SIGNAL(triggered(bool)), m_lateral, SLOT(unhighlightAllDice()));
	connect(this, SIGNAL(demoStarted(bool)), m_demoAction, SLOT(setDisabled(bool)));
	connect(this, SIGNAL(demoStarted(bool)), m_demoAction, SLOT(setChecked(bool)));
	connect(this, SIGNAL(demoStarted(bool)), m_hintAction, SLOT(setDisabled(bool)));
	connect(this, SIGNAL(demoStarted(bool)), m_lateral, SLOT(unhighlightAllDice()));
	connect(m_lateral, SIGNAL(newGameClicked()), gameNewAction, SLOT(trigger()));
	
	// Preferences
	KStandardAction::preferences(this, SLOT(showPreferences()), actionCollection());
	
	setCentralWidget(w);
	setupGUI(Keys | Save | Create);
	show();
	
	if (kirikiSettings::startupDemoEnabled()) m_demoAction -> trigger();
	else newGame();
}

void kiriki::pressed(const QModelIndex &index)
{
	if (!m_scores -> currentPlayer().isHuman()) return;

	if (index.column() == 0 || index.column() == m_scores -> currentPlayerNumber() + 1) play(index);

	m_scoresWidget -> setItemDelegateForRow(m_highlightedRowIndex, 0);
}

void kiriki::play(const QModelIndex &index)
{
	play(m_scores->row(index.row()).scoreRow());
}

void kiriki::play(int scoreRow)
{
	int score = -1;
	
	if (scoreRow == 0) score = m_lateral -> getOnes();
	else if (scoreRow == 1) score = m_lateral -> getTwos();
	else if (scoreRow == 2) score = m_lateral -> getThrees();
	else if (scoreRow == 3) score = m_lateral -> getFours();
	else if (scoreRow == 4) score = m_lateral -> getFives();
	else if (scoreRow == 5) score = m_lateral -> getSixs();
	else if (scoreRow == 6) score = m_lateral -> getThreeOfAKind();
	else if (scoreRow == 7) score = m_lateral -> getFourOfAKind();
	else if (scoreRow == 8) score = m_lateral -> getFullHouse();
	else if (scoreRow == 9) score = m_lateral -> getSStraight();
	else if (scoreRow == 10) score = m_lateral -> getLStraight();
	else if (scoreRow == 11) score = m_lateral -> getKiriki();
	else if (scoreRow == 12) score = m_lateral -> totalSum();
	else return;
	
	Q_ASSERT(score != -1);
	int row = m_scores -> rowForScoreRow(scoreRow);
	if (m_scores -> setData(m_scores -> index(row, 0), score, Qt::EditRole)) nextTurn();
}

void kiriki::newGame()
{
	delete m_scores;
	m_scores = new scores();
	m_scoresWidget -> setModel(m_scores);
	m_lateral -> nextTurn();

	m_scoresWidget -> header() -> setResizeMode(0, QHeaderView::Custom);
	m_scoresWidget -> resizeColumnToContents(0);
	statusBar()->hide();
	if (m_demoAction -> isChecked()) playComputer();
	connect(m_lateral, SIGNAL(rolled()), statusBar(), SLOT(hide()));
}

void kiriki::demo()
{
	if(
		m_scores &&
		!m_scores -> allScores() &&
		!m_scores -> currentPlayer().noScores() &&
		KMessageBox::warningContinueCancel(
			this,
			i18n("Starting the demo will end the current game. Any progress will be lost. Do you want to continue?")
		) == KMessageBox::Cancel
	)
	{
		m_demoAction -> setChecked(false);
		return;
	}

	bool preDemoHumans[6];
	preDemoHumans[0] = kirikiSettings::player1IsHuman();
	preDemoHumans[1] = kirikiSettings::player2IsHuman();
	preDemoHumans[2] = kirikiSettings::player3IsHuman();
	preDemoHumans[3] = kirikiSettings::player4IsHuman();
	preDemoHumans[4] = kirikiSettings::player5IsHuman();
	preDemoHumans[5] = kirikiSettings::player6IsHuman();
	int preDemoNumPlayers = kirikiSettings::numberOfPlayers();
	kirikiSettings::setPlayer1IsHuman(false);
	kirikiSettings::setPlayer2IsHuman(false);
	kirikiSettings::setPlayer3IsHuman(false);
	kirikiSettings::setPlayer4IsHuman(false);
	kirikiSettings::setPlayer5IsHuman(false);
	kirikiSettings::setPlayer6IsHuman(false);
	kirikiSettings::setNumberOfPlayers(6);
	newGame();
	emit demoStarted();
	disconnect(m_lateral, SIGNAL(rolled()), statusBar(), SLOT(hide()));
	kirikiSettings::setPlayer1IsHuman(preDemoHumans[0]);
	kirikiSettings::setPlayer2IsHuman(preDemoHumans[1]);
	kirikiSettings::setPlayer3IsHuman(preDemoHumans[2]);
	kirikiSettings::setPlayer4IsHuman(preDemoHumans[3]);
	kirikiSettings::setPlayer5IsHuman(preDemoHumans[4]);
	kirikiSettings::setPlayer6IsHuman(preDemoHumans[5]);
	kirikiSettings::setNumberOfPlayers(preDemoNumPlayers);
	statusBar()->showMessage(i18n("Demonstration. Press \"New\" to start a new game."));
	statusBar()->show();
	m_lateral->enableDemoMode();
}

void kiriki::showHint()
{
	if (!m_hintGiven && KMessageBox::Cancel == KMessageBox::warningContinueCancel(
			this,
			i18n("Asking for a hint will disqualify the current game from entering the high score list."),
			i18n("Confirm Hint Request"),
			KGuiItem(i18n("Give Hint Anyway"), "arrow-right")
			)
		) return;
	m_hintGiven = true;

	for (int i = 0; i < 5; ++i) setComputerDiceValue(i, m_lateral -> getDice(i));
	ComputerRolling(m_scores -> currentPlayer(), m_lateral -> getRolls());
	if (computerDiceSelected() && m_lateral -> getRolls() < 3)
	{
		int rollAmount = 0;
		for (int i = 0; i < 5; ++i)
		{
			if (getComputerDiceSelected(i))
			{
				++rollAmount;
				m_lateral -> highlightDice(i, true);
			}
		}
		m_scoresWidget->selectionModel()->clearSelection();
		statusBar()->showMessage(i18np("Roll highlighted die.", "Roll highlighted dice.", rollAmount));
		statusBar()->show();
	}
	else
	{
		static const int scoreRows[13] = { 1, 2, 3, 4, 5, 6, 10, 11, 12, 13, 14, 15, 16 };
		QModelIndex mi;
		mi = m_scores -> index(scoreRows[ComputerScoring(m_scores -> currentPlayer())], 0);
		m_highlightedRowIndex  = mi.row();
		m_scoresWidget -> setItemDelegateForRow(m_highlightedRowIndex, m_delegateHighlighted);
		QItemSelectionModel::SelectionFlags fl = QItemSelectionModel::Clear |
		                                         QItemSelectionModel::Rows  |
		                                         QItemSelectionModel::Select;
		m_scoresWidget -> selectionModel() -> select(mi, fl);
	}
}

void kiriki::endGame()
{
	const player &p = m_scores -> winner();
	m_lateral -> setEnabled(false);
	m_lateral -> endGame();
	m_hintAction -> setEnabled(false);
	if (p.isHuman())
	{
		KScoreDialog sc(KScoreDialog::Name | KScoreDialog::Score | KScoreDialog::Date, this);
		if (m_hintGiven) m_hintGiven = false;
		else
		{
			KScoreDialog::FieldInfo scoreInfo;
			scoreInfo[KScoreDialog::Score].setNum(p.grandTotal());
			const QDate date = QDate::currentDate();
			const QString datestring = date.toString(Qt::DefaultLocaleShortDate);
			scoreInfo[KScoreDialog::Date] = datestring;
			
			if (sc.addScore(scoreInfo))
			{
				sc.exec();
			}
		}
	}
	if (m_demoAction -> isChecked()) QTimer::singleShot(3000, this, SLOT(demo()));
}

void kiriki::showHighScores()
{
	KScoreDialog sc(KScoreDialog::Name | KScoreDialog::Score | KScoreDialog::Date, this);
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
	int fontSize = kirikiSettings::fontSize();
	
	KConfigDialog *configDialog = new KConfigDialog(this, "settings", kirikiSettings::self());
	configDialog->setFaceType(KConfigDialog::Plain);
	configDialog -> addPage(new configWidget(configDialog), QString(), QString());
        configDialog->setHelp(QString(),"kiriki");
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
	if (fontSize != kirikiSettings::fontSize())
	{
		m_scoresWidget -> resizeColumnToContents(0);
		m_scores->redraw();
	}
}

void kiriki::print()
{
	QPrinter printer;
	printer.setFullPage( true );
	QPrintDialog *printDialog = KdePrint::createPrintDialog(&printer, this);
	if (printDialog->exec())
	{
		QPainter painter(&printer);
		m_scores->print(painter, printer.width(), printer.height());
	}
	delete printDialog;
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
			m_hintAction -> setEnabled(false);
			QTimer::singleShot(kirikiSettings::waitTime(), this, SLOT(playComputer()));
		}
		else
		{
			m_hintAction -> setEnabled(true);
			m_lateral -> setEnabled(true);
		}
	 }
}

void kiriki::playComputer()
{
	if (!m_scores -> currentPlayer().isHuman())
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
	play(row);
	}
	else m_lateral -> setEnabled(true);
}

#include "kiriki.moc"
