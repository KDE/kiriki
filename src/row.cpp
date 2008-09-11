/***************************************************************************
 *   Copyright (C) 2008 by Albert Astals Cid <aacid@kde.org>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "row.h"

Row::Row(Type type, const QString &text, int scoreRow, Flags flags)
{
	m_type = type;
	m_text = text;
	m_scoreRow = scoreRow;
	m_flags = flags;
}

Row::Type Row::type() const
{
	return m_type;
}

QString Row::text() const
{
	return m_text;
}

int Row::scoreRow() const
{
	if (m_type == ScoreRow) return m_scoreRow;
	else return -1;
}

Row::Flags Row::flags() const
{
	return m_flags;
}
