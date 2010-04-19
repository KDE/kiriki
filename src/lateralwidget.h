/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef LATERALWIDGET_H
#define LATERALWIDGET_H

#include <QWidget>

class QLabel;
class QPushButton;
class QStackedWidget;

class dicesWidget;

class lateralWidget : public QWidget
{
Q_OBJECT
	public:
		explicit lateralWidget(QWidget *parent);

		void nextTurn();		

		void setEnabled(bool enabled);
		void setDemoMode(bool demoMode);
		void endGame();

		int getDice(int dice) const;
		void selectDice(int dice, bool select);
		int getRolls() const;
		
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
		
	signals:
		void newGameClicked();

	public slots:
		void roll();
		void newGame();
		void enableDemoMode();
	
	private:
		void updateRollLabel();
		
		QLabel *m_rolls;
		dicesWidget *m_dices;
		QStackedWidget *m_buttons;
		QPushButton *m_rollButton;
		QPushButton *m_newGameButton;
		bool m_demoMode;

		int m_roll;
};

#endif

