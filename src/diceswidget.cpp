/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <stdlib.h>

#include <QPainter>
#include <QMouseEvent>

#include <kpixmap.h>
#include <kpixmapeffect.h>
#include <krandom.h>
#include <kstandarddirs.h>

#include "diceswidget.h"

dicesWidget::dicesWidget(QWidget *parent) : QWidget(parent)
{
	setMinimumSize(90, 450);
	
	m_images[0] = QPixmap(locate("appdata", "images/dice-none.png"));
	m_images[1] = QPixmap(locate("appdata", "images/dice-1.png"));
	m_images[2] = QPixmap(locate("appdata", "images/dice-2.png"));
	m_images[3] = QPixmap(locate("appdata", "images/dice-3.png"));
	m_images[4] = QPixmap(locate("appdata", "images/dice-4.png"));
	m_images[5] = QPixmap(locate("appdata", "images/dice-5.png"));
	m_images[6] = QPixmap(locate("appdata", "images/dice-6.png"));
	
	rollAll();
}

void dicesWidget::setEnabled(bool enabled)
{
	m_enabled = enabled;
	update();
}

void dicesWidget::rollAll()
{
	for (int i = 0; i < 5; i++) m_rollDice[i] = true;
	roll();
}

bool dicesWidget::roll()
{
	bool b;
	b = generateDices();
	if (b) update();
	return b;
}

int dicesWidget::getDice(int dice) const
{
	return m_dice[dice];
}

void dicesWidget::selectDice(int dice, bool select)
{
	m_rollDice[dice] = select;
}

int dicesWidget::getOnes() const
{
	int n = getSimilar(1);
	return n;
}

int dicesWidget::getTwos() const
{
	int n = getSimilar(2);
	return n * 2;
}

int dicesWidget::getThrees() const
{
	int n = getSimilar(3);
	return n * 3;
}

int dicesWidget::getFours() const
{
	int n = getSimilar(4);
	return n * 4;
}

int dicesWidget::getFives() const
{
	int n = getSimilar(5);
	return n * 5;
}

int dicesWidget::getSixs() const
{
	int n = getSimilar(6);
	return n * 6;
}

int dicesWidget::getThreeOfAKind() const
{
	return getNOfKind(3);
}

int dicesWidget::getFourOfAKind() const
{
	return getNOfKind(4);
}

int dicesWidget::getFullHouse() const
{
	int i;
	bool three, two;
	three = false;
	i = 0;
	while (!three && i <= 6)
	{
		if (getSimilar(i) == 3) three = true;
		else i++;
	}
	if (three)
	{
		two = false;
		i = 0;
		while (!two && i <= 6)
		{
			if (getSimilar(i) == 2) two = true;
			else i++;
		}
		if (two) return 25;
	}
	return 0;
}

int dicesWidget::getSStraight() const
{
	if (getSimilar(1) && getSimilar(2) && getSimilar(3) && getSimilar(4)) return 30;
	if (getSimilar(2) && getSimilar(3) && getSimilar(4) && getSimilar(5)) return 30;
	if (getSimilar(3) && getSimilar(4) && getSimilar(5) && getSimilar(6)) return 30;
	return 0;
}

int dicesWidget::getLStraight() const
{
	if (getSimilar(1) && getSimilar(2) && getSimilar(3) && getSimilar(4) && getSimilar(5)) return 40;
	if (getSimilar(2) && getSimilar(3) && getSimilar(4) && getSimilar(5) && getSimilar(6)) return 40;
	return 0;
}

int dicesWidget::getKiriki() const
{
	if (getNOfKind(5) != 0) return 50;
	else return 0;
}

int dicesWidget::totalSum() const
{
	return m_dice[0] + m_dice[1] + m_dice[2] + m_dice[3] + m_dice[4]; 
}

void dicesWidget::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	for (int i = 0; i < 5; i++)
	{
		KPixmap pixmap(m_rollDice[i] ? m_images[0] : m_images[m_dice[i]]);
		// TODO need suggestions
//		if (!m_enabled) pixmap = KPixmapEffect::toGray(pixmap, false);
		
		p.drawPixmap(5, 10 + (10 + 80) * i, pixmap);
	}
}

void dicesWidget::mousePressEvent(QMouseEvent *e)
{
	if (!m_enabled) return;

	int x = e -> x();
	int y = e -> y();
	
	if (x > 5 && x < 85 && y > 10)
	{
		y -= 10;
		y = y / 90;
		m_rollDice[y] = !m_rollDice[y];
		update();
	}
}

bool dicesWidget::generateDices()
{
	bool any = false;
	for (int i = 0; i < 5; i++)
	{
		if (m_rollDice[i])
		{
			m_dice[i] = 1 + (int)(6.0 * KRandom::random() / (RAND_MAX + 1.0));
			m_rollDice[i] = false;
			any = true;
		}
	}
	return any;
}

int dicesWidget::getSimilar(int number) const
{
	int n = 0;
	for (int i = 0; i < 6; i++)
	{
		if (m_dice[i] == number) n++;
	}
	return n;
}

int dicesWidget::getNOfKind(int number) const
{
	int i = 1;
	bool have = false;
	while (!have && i <= 6)
	{
		if (getSimilar(i) >= number) have = true;
		i++;
	}
	if (have) return totalSum(); 
	else return 0;
}
