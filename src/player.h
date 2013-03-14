/***************************************************************************
 *   Copyright (C) 2005, 2008 by Albert Astals Cid <aacid@kde.org>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

class player
{
	public:
		player();
		
		void setName(const QString &name);
		QString name() const;

		bool isHuman() const;
		void setHuman(bool human);
		
		void initScores();
		
		// score, bonus, lowerTotal and grandTotal return < 0 when have *no* value
		
		int score(int index) const;
		void setScore(int index, int value);
		
		int bonus() const;
		int upperTotal() const;
		int upperTotalWithBonus() const;
		int lowerTotal() const;
		int grandTotal() const;

		bool allScores() const;
		bool noScores() const;
	
	private:
		int scoreRange(int begin, int end) const;
		
		bool m_human;
		QString m_name;
		int m_scores[13];
};

#endif
