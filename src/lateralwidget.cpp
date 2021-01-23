/***************************************************************************
 *   Copyright (C) 2005-2007 by Albert Astals Cid <aacid@kde.org>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "lateralwidget.h"

#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

#include <QAction>
#include <QIcon>
#include <KStandardGameAction>
#include <KLocalizedString>

#include "diceswidget.h"

lateralWidget::lateralWidget(QWidget *parent) : QWidget(parent), m_demoMode(false)
{
	QVBoxLayout *lay = new QVBoxLayout(this);
	m_rolls = new QLabel(this);
	m_dices = new dicesWidget(this);
	m_buttons = new QStackedWidget(this);
	m_rollButton = new QPushButton(QIcon::fromTheme( QStringLiteral( "roll") ), i18n("&Roll" ), this);
	
	QAction *dummyAction = KStandardGameAction::gameNew(nullptr, nullptr, nullptr);
	m_newGameButton = new QPushButton(dummyAction->icon(), dummyAction->text(), this);
	delete dummyAction;

	m_buttons -> addWidget(m_rollButton);
	m_buttons -> addWidget(m_newGameButton);

	disableDemoMode();

	lay -> addWidget(m_rolls, 0, Qt::AlignHCenter);
	lay -> addWidget(m_dices);
	lay -> addWidget(m_buttons);
	lay -> addStretch(1);

	connect(m_rollButton, &QPushButton::clicked, this, &lateralWidget::roll);
	connect(m_rollButton, &QPushButton::clicked, this, &lateralWidget::unhighlightAllDice);
	connect(m_newGameButton, &QPushButton::clicked, this, &lateralWidget::newGame);
	
	nextTurn();
}

void lateralWidget::nextTurn()
{
	m_roll = 1;
	updateRollLabel();
	m_dices -> rollAll();
	if (!m_demoMode) m_buttons -> setCurrentWidget(m_rollButton);
}

void lateralWidget::setEnabled(bool enabled)
{
	m_rollButton -> setEnabled(enabled);
	//newGameButton enabledness is controlled internally
	//m_newGameButton -> setEnabled(enabled);
	m_dices -> setEnabled(enabled);
}

void lateralWidget::enableDemoMode()
{
	m_buttons -> setCurrentWidget(m_newGameButton);
	m_newGameButton->setEnabled(true);
	m_demoMode = true;
}

void lateralWidget::disableDemoMode()
{
	m_buttons -> setCurrentWidget(m_rollButton);
	m_newGameButton -> setEnabled(false);
	m_demoMode = false;
}

void lateralWidget::endGame()
{
	m_newGameButton -> setEnabled(true);
	m_buttons -> setCurrentWidget(m_newGameButton);
}

int lateralWidget::getDice(int dice) const
{
	return m_dices -> getDice(dice);
}

void lateralWidget::selectDice(int dice, bool selected) 
{
	m_dices -> selectDice(dice, selected);
}

void lateralWidget::highlightDice(int dice, bool selected)
{
	m_dices -> highlightDice(dice, selected);
}

void lateralWidget::unhighlightAllDice()
{
	for (int i = 0; i < 5; ++i) m_dices -> highlightDice(i, false);
}

int lateralWidget::getRolls() const
{
	return m_roll;
}

int lateralWidget::getOnes() const
{
	return m_dices -> getOnes();
}

int lateralWidget::getTwos() const
{
	return m_dices -> getTwos();
}

int lateralWidget::getThrees() const
{
	return m_dices -> getThrees();
}

int lateralWidget::getFours() const
{
	return m_dices -> getFours();
}

int lateralWidget::getFives() const
{
	return m_dices -> getFives();
}

int lateralWidget::getSixs() const
{
	return m_dices -> getSixs();
}

int lateralWidget::getThreeOfAKind() const
{
	return m_dices -> getThreeOfAKind();
}

int lateralWidget::getFourOfAKind() const
{
	return m_dices -> getFourOfAKind();
}

int lateralWidget::getFullHouse() const
{
	return m_dices -> getFullHouse();
}

int lateralWidget::getSStraight() const
{
	return m_dices -> getSStraight();
}

int lateralWidget::getLStraight() const
{
	return m_dices -> getLStraight();
}

int lateralWidget::getKiriki() const
{
	return m_dices -> getKiriki();
}

int lateralWidget::totalSum() const
{
	return m_dices -> totalSum();
}


void lateralWidget::roll()
{
	if (m_dices -> roll())
	{
		m_roll++;
		updateRollLabel();
		Q_EMIT rolled();
	}
}

void lateralWidget::newGame()
{
	disableDemoMode();
	Q_EMIT newGameClicked();
}

void lateralWidget::updateRollLabel()
{
	m_rolls -> setText(i18n("Roll %1 of 3",m_roll));
	if (!m_demoMode) setEnabled(m_roll != 3);
}


