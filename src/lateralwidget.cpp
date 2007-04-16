/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "lateralwidget.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <klocale.h>

#include "diceswidget.h"

lateralWidget::lateralWidget(QWidget *parent) : QWidget(parent)
{
	QVBoxLayout *lay = new QVBoxLayout(this);
	m_rolls = new QLabel(this);
	m_dices = new dicesWidget(this);
	m_rollButton = new QPushButton(i18n("&Roll"), this);

	lay -> addWidget(m_rolls, 0, Qt::AlignHCenter);
	lay -> addWidget(m_dices);
	lay -> addWidget(m_rollButton);
	lay -> addStretch(1);

	connect(m_rollButton, SIGNAL(clicked(bool)), this, SLOT(roll()));
	
	nextTurn();
}

void lateralWidget::nextTurn()
{
	m_roll = 1;
	updateRollLabel();
	m_dices -> rollAll();
}

void lateralWidget::setEnabled(bool enabled)
{
	m_rollButton -> setEnabled(enabled);
	m_dices -> setEnabled(enabled);
}

int lateralWidget::getDice(int dice) const
{
	return m_dices -> getDice(dice);
}

void lateralWidget::selectDice(int dice, bool selected) 
{
	m_dices -> selectDice(dice, selected);
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
	}
}

void lateralWidget::updateRollLabel()
{
	m_rolls -> setText(i18n("Roll %1 of 3",m_roll));
	setEnabled(m_roll != 3);
}

#include "lateralwidget.moc"
