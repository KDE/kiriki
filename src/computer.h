/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <aacid@kde.org>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef COMPUTER_H
#define COMPUTER_H

class player;

void setComputerDiceValue(int dice, int value);
bool getComputerDiceSelected(int dice);

void ComputerRolling(const player &p, int NumberOfRolls);
int ComputerScoring(const player &p);
bool computerDiceSelected();

#endif
