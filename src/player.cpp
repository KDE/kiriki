/***************************************************************************
 *   Copyright (C) 2005, 2008 by Albert Astals Cid <aacid@kde.org>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "player.h"

player::player()
{
	initScores();
}

void player::setName(const QString &name)
{
	m_name = name;
}

QString player::name() const
{
	return m_name;
}

bool player::isHuman() const
{
	return m_human;
}

void player::setHuman(bool human)
{
	m_human = human;
}

void player::initScores()
{
	for (int i = 0; i < 13; i++) m_scores[i] = -1;
}

int player::score(int index) const
{
	return m_scores[index];
}

void player::setScore(int index, int value)
{
	m_scores[index] = value;
}

int player::bonus() const
{
	if (scoreRange(0, 5) > 62) return 35;
	return -1;
}

int player::upperTotal() const
{
	return scoreRange(0, 5);
}

int player::upperTotalWithBonus() const
{
	int score = qMax(bonus(), 0);
	score += upperTotal();
	return score;
}

int player::lowerTotal() const
{
	return scoreRange(6, 12);
}

int player::grandTotal() const
{
	int up, down;
	up = upperTotalWithBonus();
	down = lowerTotal();
	
	if (up >= 0)
	{
		if (down >= 0) return up + down;
		else return up;
	}
	else
	{
		if (down >= 0) return down;
		else return -1;
	}
}

bool player::allScores() const
{
	int i = 0;
	bool all = true;
	while (all && i < 13)
	{
		all = m_scores[i] != -1;
		i++;
	}
	return all;
}


bool player::noScores() const
{
	bool none = true;
	for(int i = 0; none && i < 13; ++i) none = m_scores[i] == -1;
	return none;
}

int player::scoreRange(int begin, int end) const
{
	int score = 0;
	bool any = false;
	for (int i = begin; i <= end; i++)
	{
		if (m_scores[i] >= 0)
		{
			score += m_scores[i];
			any = true;
		}
	}
	if (any) return score;
	else return -1;
}
