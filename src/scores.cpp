/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QFont>
#include <QPalette>

#include <klocale.h>

#include "scores.h"
#include "settings.h"

scores::scores()
{
	player p;
	for (int i = 1; i <= kirikiSettings::numberOfPlayers(); i++)
	{
		if (i == 1)
		{
			p.setName(kirikiSettings::player1Name());
			p.setHuman(true);
		}
		else if (i == 2)
		{
			p.setName(kirikiSettings::player2Name());
			p.setHuman(kirikiSettings::player2IsHuman());
		}
		else if (i == 3)
		{
			p.setName(kirikiSettings::player3Name());
			p.setHuman(kirikiSettings::player3IsHuman());
		}
		else if (i == 4)
		{
			p.setName(kirikiSettings::player4Name());
			p.setHuman(kirikiSettings::player4IsHuman());
		}
		else if (i == 5)
		{
			p.setName(kirikiSettings::player5Name());
			p.setHuman(kirikiSettings::player5IsHuman());
		}
		else if (i == 6)
		{
			p.setName(kirikiSettings::player6Name());
			p.setHuman(kirikiSettings::player6IsHuman());
		}
		
		m_players.append(p);
	}
	
	m_currentPlayer = 0;
}

bool scores::allScores() const
{
	bool all;
	all = true;
	for (int i = 0; i < m_players.count(); i++) all = all && m_players.at(i).allScores();
	return all;
}

void scores::nextPlayer()
{
	m_currentPlayer++;
	m_currentPlayer = m_currentPlayer % m_players.count();
}

const player &scores::currentPlayer() const
{
	return m_players.at(m_currentPlayer);
}

const player &scores::winner() const
{
	int best = 0;
	for (int i = 1; i < m_players.count(); i++)
	{
		if (m_players.at(best).grandTotal() < m_players.at(i).grandTotal()) best = i;
	}
	return m_players.at(best);
}

int scores::rowCount(const QModelIndex &/*index*/) const
{
	return 19;
}

int scores::columnCount(const QModelIndex &/*index*/) const
{
	return m_players.size() + 1;
}

QVariant scores::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) return QVariant();
	
	int column, row;
	column = index.column();
	row = index.row();
	
	if (role == Qt::FontRole)
	{
		QFont f;
		if (row == 6 || row == 7 || row == 16 || row == 18) f.setBold(true);
		if (row == 18) f.setPointSize(f.pointSize() + 5);
		return f;
	}
	else if (role == Qt::BackgroundColorRole)
	{
		QPalette p;
		QColor c;
		if (column % 2 == 0)
		{
			c = p.alternateBase().color();
			if (row % 2) c = c.dark(105);
		}
		else
		{
			if (row % 2 == 0) c = p.base().color();
			else c = p.alternateBase().color();
		}
		
		if (row == 18) c = p.highlight().color().light();
		return c;
	}
	else if (role == Qt::TextAlignmentRole)
	{
		if (column != 0) return Qt::AlignCenter;
	}
	
	if (row == 8) return QVariant();
	if (row == 17) return QVariant();
	
	if (role != Qt::DisplayRole) return QVariant();
	
	if (column == 0)
	{
		switch (row)
		{
			case 0:
				return i18n("1s");
			break;
			
			case 1:
				return i18n("2s");
			break;
			
			case 2:
				return i18n("3s");
			break;
			
			case 3:
				return i18n("4s");
			break;
			
			case 4:
				return i18n("5s");
			break;
			
			case 5:
				return i18n("6s");
			break;
			
			case 6:
				return i18n("Bonus if > 62");
			break;
			
			case 7:
				return i18n("Upper total");
			break;
			
			case 9:
				return i18n("3 of a Kind");
			break;
			
			case 10:
				return i18n("4 of a Kind");
			break;
			
			case 11:
				return i18n("Full House");
			break;
			
			case 12:
				return i18n("Small Straight");
			break;
			
			case 13:
				return i18n("Large Straight");
			break;
			
			case 14:
				return i18n("Kiriki");
			break;
			
			case 15:
				return i18n("Chance");
			break;
			
			case 16:
				return i18n("Lower Total");
			break;
			
			case 18:
				return i18n("Grand Total");
			break;
			
			default:
				return QVariant();
			break;
		}
	}

	const player &p = m_players.at(column - 1);
	int score = -1;
	if (row < 6) score = p.score(row);
	if (row == 6) score = p.bonus();
	if (row == 7) score = p.upperTotal(true);
	if (row > 7 && row < 16) score = p.score(row - 3);
	if (row == 16) score = p.lowerTotal();
	if (row == 18) score = p.grandTotal();
	
	if (score < 0) return QVariant();
	else return QString::number(score);
}

QVariant scores::headerData(int section, Qt::Orientation /*orientation*/, int role) const
{
	if (role != Qt::DisplayRole) return QVariant();
	
	if (section == 0) return QVariant();
	return m_players.at(section - 1).name();
}

bool scores::setData(const QModelIndex &mi, const QVariant &value, int role)
{
	if (role != Qt::EditRole) return false;
	
	int row = mi.row();
	if (row == 6 || row == 7 || row == 8 || row == 16 || row == 18) return false; 
	
	if (row < 6)
	{
		if (m_players[m_currentPlayer].score(row) >= 0) return false;
		m_players[m_currentPlayer].setScore(row, value.toInt());
		emit dataChanged(mi, mi);
		emit dataChanged(index(m_currentPlayer + 1, 6), index(m_currentPlayer + 1, 7));
		emit dataChanged(index(m_currentPlayer + 1, 18), index(m_currentPlayer + 1, 18));
	}
	else
	{
		if (row == 14)
		{
			// Kiriki can be acumulated
			if (m_players[m_currentPlayer].score(11) > 0)
			{
				m_players[m_currentPlayer].setScore(11, m_players[m_currentPlayer].score(11) + value.toInt());
			}
			else if (m_players[m_currentPlayer].score(11) < 0)
			{
				m_players[m_currentPlayer].setScore(11, value.toInt());
			}
			else return false;
		}
		else
		{
			if (m_players[m_currentPlayer].score(row - 3) >= 0) return false;
			m_players[m_currentPlayer].setScore(row - 3, value.toInt());
		}
		
		emit dataChanged(mi, mi);
		emit dataChanged(index(m_currentPlayer + 1, 16), index(m_currentPlayer + 1, 18));
	}
	
	return true;
}
