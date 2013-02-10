/***************************************************************************
 *   Copyright (C) 2013 by Ian Simpson <ian.kdedev@yahoo.com>              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "itemdelegate.h"

itemDelegate::itemDelegate(int height, QObject *parent) :
	QItemDelegate(parent),
	m_height(height),
	m_grandTotalRow(19)
{
	m_emptyRows << 9 << 18;
}

QSize itemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.row() == 0 || index.row() == m_grandTotalRow || m_emptyRows.contains(index.row()))
	{
		return QItemDelegate::sizeHint(option, index);
	}
	else
	{
		// Set the height to that of the font set at pointSize m_height
		QSize size;
		QFont font(index.data(Qt::FontRole).value<QFont>());
		font.setPointSize(m_height);
		QFontMetrics fm(font);
		size.setHeight(fm.height());
		size.setWidth(QItemDelegate::sizeHint(option, index).width());
		return size;
	}
}

void itemDelegate::setHeight(int height)
{
	m_height = height;
}
