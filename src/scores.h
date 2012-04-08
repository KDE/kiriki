/***************************************************************************
 *   Copyright (C) 2005, 2008 by Albert Astals Cid <tsdgeos@terra.es>      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef SCORES_H
#define SCORES_H

#include <QAbstractTableModel>

#include "player.h"
#include "row.h"

class QPainter;

class scores : public QAbstractTableModel
{
	public:
		scores();

		bool allScores() const;

		void nextPlayer();
		const player &currentPlayer() const;
		int currentPlayerNumber() const;
		const player &winner() const;
		Row row(int row) const;
		int rowForScoreRow(int scoreRow) const;
		
		int rowCount(const QModelIndex &index) const;
		int columnCount(const QModelIndex &index) const;
		QVariant data(const QModelIndex &index, int role) const;
		bool setData(const QModelIndex &mi, const QVariant &value, int role);
		
		void redraw();
		void print(QPainter &painter, double width, double height) const;
	
	private:
		QList<player> m_players;
		QList<Row> m_rows;
		int m_currentPlayer;
};

#endif
