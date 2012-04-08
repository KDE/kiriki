/***************************************************************************
 *   Copyright (C) 2005, 2008 by Albert Astals Cid <tsdgeos@terra.es>      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "scores.h"

#include <QFont>
#include <QPainter>
#include <QPalette>

#include <kiconloader.h>
#include <klocale.h>

#include "settings.h"

scores::scores()
{
	player p;
	for (int i = 1; i <= kirikiSettings::numberOfPlayers(); i++)
	{
		if (i == 1)
		{
			p.setName(kirikiSettings::player1Name());
			p.setHuman(kirikiSettings::player1IsHuman());
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
	
	m_rows.append(Row(Row::NamesRow));
	m_rows.append(Row(Row::ScoreRow, i18n("1s"), 0));
	m_rows.append(Row(Row::ScoreRow, i18n("2s"), 1));
	m_rows.append(Row(Row::ScoreRow, i18n("3s"), 2));
	m_rows.append(Row(Row::ScoreRow, i18n("4s"), 3));
	m_rows.append(Row(Row::ScoreRow, i18n("5s"), 4));
	m_rows.append(Row(Row::ScoreRow, i18n("6s"), 5));
	m_rows.append(Row(Row::BonusRow, i18n("Bonus if > 62"), -1, Row::BoldFontFlag));
	m_rows.append(Row(Row::UpperTotalRow, i18n("Upper total"), -1, Row::BoldFontFlag));
	m_rows.append(Row(Row::EmptyRow));
	m_rows.append(Row(Row::ScoreRow, i18n("3 of a Kind"), 6));
	m_rows.append(Row(Row::ScoreRow, i18n("4 of a Kind"), 7));
	m_rows.append(Row(Row::ScoreRow, i18n("Full House"), 8));
	m_rows.append(Row(Row::ScoreRow, i18n("Small Straight"), 9));
	m_rows.append(Row(Row::ScoreRow, i18n("Large Straight"), 10));
	m_rows.append(Row(Row::ScoreRow, i18n("Kiriki"), 11));
	m_rows.append(Row(Row::ScoreRow, i18n("Chance"), 12));
	m_rows.append(Row(Row::LowerTotalRow, i18n("Lower Total"), -1, Row::BoldFontFlag));
	m_rows.append(Row(Row::EmptyRow));
	m_rows.append(Row(Row::GrandTotalRow, i18n("Grand Total"), -1, Row::BoldFontFlag | Row::BiggerFontFlag));
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

int scores::currentPlayerNumber() const
{
	return m_currentPlayer;
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

Row scores::row(int row) const
{
	return m_rows.at(row);
}

int scores::rowForScoreRow(int scoreRow) const
{
	for (int i = 0; i < m_rows.count(); ++i)
	{
		if (m_rows.at(i).scoreRow() == scoreRow) return i;
	}
	return -1;
}

int scores::rowCount(const QModelIndex &/*index*/) const
{
	return m_rows.count();
}

int scores::columnCount(const QModelIndex &/*index*/) const
{
	return m_players.size() + 1;
}

QVariant scores::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) return QVariant();
	
	int column = index.column();
	const Row row = m_rows.at(index.row());
	
	if (row.type() == Row::NamesRow)
	{
		if (column > 0)
		{
			const player &p = m_players.at(column - 1);
			if (role == Qt::DecorationRole)
			{
				QString icon;
				if (p.isHuman()) icon = "user-identity";
				else icon = "cpu";
				return KIconLoader::global()->loadIcon(icon, KIconLoader::NoGroup, KIconLoader::SizeMedium);
			}
			else if (role == Qt::DisplayRole)
			{
				return p.name();
			}
			else if (role == Qt::BackgroundColorRole)
			{
				if (column - 1 == m_currentPlayer)
				{
					QPalette pal;
					return pal.alternateBase().color().dark(110);
				}
			}
			else if (role == Qt::FontRole)
			{
				if (column - 1 == m_currentPlayer)
				{
					QFont f;
					f.setBold(true);
					return f;
				}
			}
		}
		return QVariant();
	}
	
	if (role == Qt::FontRole)
	{
		QFont f;
		f.setPointSize(kirikiSettings::fontSize());
		if (row.flags() & Row::BoldFontFlag) f.setBold(true);
		if (row.flags() & Row::BiggerFontFlag) f.setPointSize(f.pointSize() + 5);
		return f;
	}
	else if (role == Qt::BackgroundColorRole)
	{
		QPalette p;
		QColor c;
		if (column % 2 == 0)
		{
			c = p.alternateBase().color();
			if (index.row() % 2) c = c.dark(105);
		}
		else
		{
			if (index.row() % 2 == 0) c = p.base().color();
			else c = p.alternateBase().color();
		}
		
		if (row.type() == Row::GrandTotalRow) c = p.highlight().color().light();
		
		if (column - 1 == m_currentPlayer) c = c.dark(110);
		
		return c;
	}
	else if (role == Qt::TextAlignmentRole)
	{
		if (column != 0) return Qt::AlignCenter;
	}
	
	if (row.type() == Row::EmptyRow) return QVariant();

	if (role != Qt::DisplayRole) return QVariant();
	
	if (column == 0) return row.text();

	const player &p = m_players.at(column - 1);
	int score = -1;
	
	if (row.type() == Row::ScoreRow) score = p.score(row.scoreRow());
	else if (row.type() == Row::BonusRow) score = p.bonus();
	else if (row.type() == Row::UpperTotalRow) score = p.upperTotalWithBonus();
	else if (row.type() == Row::LowerTotalRow) score = p.lowerTotal();
	else if (row.type() == Row::GrandTotalRow) score = p.grandTotal();
	
	if (score < 0) return QVariant();
	else return QString::number(score);
}

void scores::askForRedraw()
{
	emit dataChanged(index(0, 0), index(m_rows.count() - 1, m_players.count() - 1));
}

bool scores::setData(const QModelIndex &mi, const QVariant &value, int role)
{
	if (role != Qt::EditRole) return false;
	
	const Row row = m_rows.at(mi.row());
	if (row.type() != Row::ScoreRow) return false;
	
	if (row.scoreRow() == 11)
	{
		// Kiriki can be acumulated
		if (m_players[m_currentPlayer].score(11) > 0 && value.toInt() > 0)
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
		if (m_players[m_currentPlayer].score(row.scoreRow()) >= 0) return false;
		m_players[m_currentPlayer].setScore(row.scoreRow(), value.toInt());
	}
	
	emit dataChanged(mi, mi);
	
	for (int i = 0; i < m_rows.count(); ++i)
	{
		const Row::Type t = m_rows.at(i).type();
		if (t == Row::BonusRow || t == Row::UpperTotalRow || t == Row::LowerTotalRow || t == Row::GrandTotalRow)
			emit dataChanged(index(m_currentPlayer + 1, i), index(m_currentPlayer + 1, i));
	}
	
	return true;
}

void scores::print(QPainter &painter, double width, double height) const
{
	QFont f;
	QFontMetrics fm(f);
	double margin = width * 0.1;
	double widthToUse = width - 2.0 * margin;
	double fontHeight;
	double heightToUse;
	bool continueFindingFont = true;
	while (continueFindingFont)
	{
		fontHeight = fm.height();
		heightToUse = 40.0 * fontHeight;
		if ( heightToUse < height - 2.0 * margin )
		{
			continueFindingFont = false;
		}
		else
		{
			f.setPointSize(f.pointSize() - 1);
			fm = QFontMetrics(f);
		}
	}
	
	double cellWidth = widthToUse / (double)(m_players.count() + 1);
	double cellHeight = fontHeight * 2.0;
	
	painter.drawRect( QRectF( margin, margin, widthToUse, heightToUse ) );
	for (int i = 1; i <= 20; ++i)
	{
		painter.drawLine( QPointF(margin, margin + i * cellHeight), QPointF(margin + widthToUse, margin + i * cellHeight) );
	}
		
	for (int i = 1; i <= m_players.count() + 1; ++i)
	{
		painter.drawLine( QPointF(margin + i * cellWidth, margin), QPointF(margin + i * cellWidth, margin + heightToUse) );
	}
	
	// write the names
	for (int i = 1; i <= m_players.count(); ++i)
	{
		QRectF cell(margin + i * cellWidth, margin, cellWidth, cellHeight);
		painter.drawText( cell, Qt::AlignCenter, m_players[i-1].name() );
	}
	
	// write the plays
	for (int i = 1; i <= 20; ++i)
	{
		QRectF cell(margin, margin + i * cellHeight, cellWidth, cellHeight);
		painter.drawText( cell, Qt::AlignCenter, data( index(i-1, 0), Qt::DisplayRole ).toString() );
	}
	
	// write the scores
	for (int i = 1; i <= m_players.count(); ++i)
	{
		for (int j = 1; j <= 20; ++j)
		{
			QRectF cell(margin + i * cellWidth, margin + j * cellHeight, cellWidth, cellHeight);
			painter.drawText( cell, Qt::AlignCenter, data( index(j-1, i), Qt::DisplayRole ).toString() );
		}
	}
}
