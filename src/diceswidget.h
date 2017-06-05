/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <aacid@kde.org>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef DICESWIDGET_H
#define DICESWIDGET_H

#include <QPixmap>
#include <QWidget>

class dicesWidget : public QWidget
{
	public:
		explicit dicesWidget(QWidget *parent);
		
		void setEnabled(bool enabled);
		
		bool roll();
		void rollAll();

		int getDice(int dice) const;
		void selectDice(int dice, bool select);
		void highlightDice(int dice, bool highlight);
		
		int getOnes() const;
		int getTwos() const;
		int getThrees() const;
		int getFours() const;
		int getFives() const;
		int getSixs() const;
		int getThreeOfAKind() const;
		int getFourOfAKind() const;
		int getFullHouse() const;
		int getSStraight() const;
		int getLStraight() const;
		int getKiriki() const;
		int totalSum() const;
	
	protected:
		void paintEvent(QPaintEvent *p) override;
		void mousePressEvent(QMouseEvent *e) override;
	
	private:
		bool generateDices();
		int getSimilar(int number) const;
		int getNOfKind(int number) const;
		
		bool m_enabled;
		bool m_rollDice[5];
		bool m_highlightDice[5];
		int m_dice[5];
		QPixmap m_images[7];
};

#endif
