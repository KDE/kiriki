/***************************************************************************
 *   Copyright (C) 2013 by Ian Simpson <ian.kdedev@yahoo.com>              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QItemDelegate>

class itemDelegate : public QItemDelegate
{
Q_OBJECT
	public:
		explicit itemDelegate(int height, QObject *parent = nullptr);
		QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

		void setHeight(int height);

	private:
		QVector<int> m_emptyRows;

		int m_height;
		const int m_grandTotalRow;
};

#endif // ITEMDELEGATE_H
