/***************************************************************************
 *   Copyright (C) 2008 by Albert Astals Cid <aacid@kde.org>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef KIRIKI_ROW_H
#define KIRIKI_ROW_H
#include <QString>

class Row
{
	public:
		enum Type
		{
			NamesRow,
			EmptyRow,
			ScoreRow,
			BonusRow,
			UpperTotalRow,
			LowerTotalRow,
			GrandTotalRow
		};
		
		enum Flag
		{
			NoFlags = 0,
			BoldFontFlag = 1,
			BiggerFontFlag = 2
		};
		Q_DECLARE_FLAGS(Flags, Flag)
		
		explicit Row(Type type, const QString &text = QString(), int scoreRow = -1, Flags flags = NoFlags);
		
		Type type() const;
		QString text() const;
		int scoreRow() const;
		Flags flags() const;
	
	private:
		Type m_type;
		QString m_text;
		int m_scoreRow;
		Flags m_flags;
};


Q_DECLARE_OPERATORS_FOR_FLAGS(Row::Flags)
#endif
