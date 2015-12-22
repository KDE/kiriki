/***************************************************************************
 *   Copyright (C) 2005, 2008 by Albert Astals Cid <aacid@kde.org>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

// Using lots of code from

/*
 * Gyatzee: Gnomified Yahtzee game.
 * (C) 1998 the Free Software Foundation
 *
 * File:   computer.c
 *
 * Author: Scott Heavner
 *
 *   Window manager independent yahtzee routines for computer opponents.
 *
 *   Variables are exported in yahtzee.h
 *
 *   This program is based on based on orest zborowski's curses based
 *   yahtze (c)1992.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "player.h"

static const int NUM_UPPER = 6;
static const int NUM_FIELDS = 13;
static const int H_3 = 6;
static const int H_4 = 7;
static const int H_FH = 8;
static const int H_SS = 9;
static const int H_LS = 10;
static const int H_YA = 11;
static const int H_CH = 12;

typedef struct {
  int val;
  int sel;
} DiceInfo;

DiceInfo DiceValues[5] = { {1,0}, {2,0}, {3,0}, {4,0}, {5,0} };

void setComputerDiceValue(int dice, int value)
{
	DiceValues[dice].val = value;
	DiceValues[dice].sel = false;
}

bool getComputerDiceSelected(int dice)
{
	return DiceValues[dice].sel;
}

bool computerDiceSelected()
{
	bool selected = false;
	int i = 0;
	while (!selected && i < 5)
	{
		selected = DiceValues[i].sel;
		i++;
	}
	return selected;
}

int numberOfRolls;

/*
**	questions are:
**		0:	none
**		1:	"what dice to roll again?"
**		2:	"where do you want to put that?"
*/

typedef struct
{
	int value;
	char rerolls[5];
} DiceRank;

static DiceRank bc_table[NUM_FIELDS];

static void MarkAllRerolls(DiceRank *t, int val)
{
  int i;
  for (i=0; i<5; i++)
    t->rerolls[i] = val;
}

static inline void TagReroll(DiceRank *t, int i)
{
  t->rerolls[i] = 1;
}

static inline void ClearReroll(DiceRank *t, int i)
{
  t->rerolls[i] = 0;
}

static inline void ClearRerolls(DiceRank *t)
{
  MarkAllRerolls(t,0);
}

static inline void TagRerolls(DiceRank *t)
{
  MarkAllRerolls(t,1);
}

/*static inline int NeedsReroll(DiceRank *t)
{
        int i, j=0;
        for (i=0; i<5; i++)
                if (t->rerolls[i])
                        j++;
        return j;
}*/

int 
add_dice(void)
{
        int i;
        int val;

        val = 0;

        for (i = 0; i < 5; ++i)
                val += DiceValues[i].val;

        return (val);
}

int 
count(int val)
{
        int i;
        int num;

        num = 0;

        for (i = 0; i < 5; ++i)
                if (DiceValues[i].val == val)
                        ++num;

        return (num);
}

int 
find_n_of_a_kind(int n, int but_not)
{
        int i;

        for (i = 0; i < 5; ++i)
        {
                if (DiceValues[i].val == but_not)
                        continue;

                if (count(DiceValues[i].val) >= n)
                        return (DiceValues[i].val);
        }

        return (0);
}

int 
find_straight(int run, int notstart, int notrun)
{
        int i;
        int j;

        for (i = 1; i < 7; ++i)
        {
                if (i >= notstart && i < notstart + notrun)
                        continue;

                for (j = 0; j < run; ++j)
                        if (!count(i + j))
                                break;

                if (j == run)
                        return (i);
        }

        return (0);
}

/*static char *RerollString(DiceRank *t)
{
        static char dice_string[11];
        int i;

        sprintf(dice_string,"          ");

        for (i=0; i<5; i++) {
                if (t->rerolls[i])
                        dice_string[i*2] = '1'+i;
                else
                        dice_string[i*2] = ' ';
        }

        return dice_string;
}*/

/*
**	depending on the dice rolls, we fill in the choice table.  if
**	for a particular choice the rolls are good, we put an appropriate
**	value in the table.  we also give which die need to be rerolled.
**
**	basically, the value system is based off the highest score possible
**	rolling the dice - all 6 (6 x 5 = 30).  so the best any of the upper
**	ranks can get is 30.
*/

static int throwaway[NUM_FIELDS] =
{
	4,      /* 1 */
	3,      /* 2 */
	2,      /* 3 */
	2,      /* 4 */
	1,      /* 5 */
	1,      /* 6 */
	3,      /* 3 of a kind */
	5,      /* 4 of a kind */
	4,      /* Full House */
	3,      /* Small straight */
	5,      /* Large straight */
	4,      /* Yahtzee */
	0,      /* Chance */
};

static void
BuildTable(const player &p, int NumberOfRolls)
{
	int i;
	int j;
	int k;
	int d;
	int d2 = -1;
	int overflow;

	for (i = 0; i < NUM_FIELDS; ++i)
	{
		if (p.score(i) >= 0)
			bc_table[i].value = -99;
		else
			bc_table[i].value = throwaway[i];

		TagRerolls(&bc_table[i]);
	}

/*
**	HANDLING UPPER SLOTS
*/

/*
**	first, we calculate the overflow of the upper ranks. that is, we
**	count all the points we have left over from our 3-of-all rule
**	if we get 3 of all in the upper ranks, we get a bonus, so if
**	we get 4 of something, that means a lower roll may be acceptable,
**	as long as we remain in the running for a bonus.  overflow can
**	be negative as well, in which case throwaway rolls are not
**	encouraged, and a high roll will get a nice boost
*/
	overflow = 0;

	for (i = 0; i < NUM_UPPER; ++i)
	{
		if (p.score(i) >= 0)
			continue;

		overflow += (count(i+1) - 3) * (i+1);
	}

	for (i = 0; i < NUM_UPPER; ++i)
	{
		if (p.score(i) >= 0)
			continue;

		ClearRerolls(&bc_table[i]);

		for (d = 0; d < 5; ++d) {
                        if (DiceValues[d].val != i + 1)
                                TagReroll(&bc_table[i],d);
		}

/*
**	ok. now we set a base value on the roll based on its count and
**	how much it is worth to us.
*/
		bc_table[i].value = count(i+1) * 8;

/*
**	we have to play games with the bonus.
**	if we already have a bonus, then all free slots are candidates for
**		throwing away - we only do this when there are no more rolls
**	if this would get us a bonus, we make it very attractive
**
**	if we get 3 of everything on the top, we get a bonus. so...
**	if we have more than 3, we make the choice more attractive.
**	if less than 3, we make it less attractive.
**	if our overflow (any that are more than 3, summed up) covers up
**		our lack (if we only have 2, and there were 4 6's), we
**		do not penalize ourselves as much (since we're still in the
**		running for a bonus)
*/

		if (p.upperTotal() >= 63)
		{
			if (NumberOfRolls > 2)
				bc_table[i].value += 10;
		}

		else if (p.upperTotal() + count(i+1) * (i+1) >= 63)
		{
			bc_table[i].value += 35;
		}

		if (count(i+1) < 3)
		{
			if (overflow < (3 - count(i+1)) * (i+1))
				bc_table[i].value -= (3 - count(i+1)) * 2;
		}

		else if (count(i+1) > 3)
		{
			bc_table[i].value += (count(i+1) - 3) * 2;
		}
	}

/*
**	HANDLING LOWER SLOTS
*/

/*
**	first, we look for potential.  these values will be larger than the
**	single rolls but less than the made rolls (or those with higher value)
**
**	we also do such thinking only if we're not supposed to be looking just
**	for the best combinations...
*/
	if (NumberOfRolls < 3)
	{
/*
**	searching for large straight... here we chicken out and only look at
**	runs which might have possibilities
*/
		if (p.score(H_LS) < 0)
		{
			for (i = 4; i > 0; --i)
			{
				d2 = find_straight(i, 0, 0 /* SDH 0 or 1? */);

				if (d2 == 0)
					continue;

				bc_table[H_LS].value = (40 * i) / 5;

				ClearRerolls(&bc_table[H_LS]);

				for (d = 1; d < 7; ++d)
				{
					if (count(d) > 0)
					{
						if (d < d2 || d >= d2 + i)
							k = count(d);

						else
							k = count(d) - 1;

						if (k < 1)
							continue;

						for (j = 0; j < 5; ++j)
						{
							if (DiceValues[j].val!=d)
								continue;

                                                        TagReroll(&bc_table[H_LS],j);

							if (!--k)
								break;
						}
					}
				}

				break;
			}
		}
/*
**	searching for small straight... here we chicken out and only look at
**	runs which might have possibilities
*/
		if (p.score(H_SS) < 0)
		{
			for (i = 3; i > 0; --i)
			{
				d2 = find_straight(i, 0, 0 /* SDH 0 or 1? */);

				if (d2 == 0)
					continue;

				bc_table[H_SS].value = (30 * i) / 4;

				ClearRerolls(&bc_table[H_SS]);

				for (d = 1; d < 7; ++d)
				{
					if (count(d) > 0)
					{
						if (d < d2 || d >= d2 + i)
							k = count(d);

						else
							k = count(d) - 1;

						if (k < 1)
							continue;

						for (j = 0; j < 5; ++j)
						{
							if (DiceValues[j].val!=d)
								continue;

                                                        TagReroll(&bc_table[H_SS],j);

							if (!--k)
								break;
						}
					}
				}

				break;
			}
		}
/*
**	searching for 3 of a kind
*/
		if (p.score(H_3) < 0)
		{
			for (i = 2; i > 0; --i)
			{
				for (d = 6; d > 0; --d)
				{
					if (count(d) >= i)
						break;
				}

				if (d == 0)
					continue;

				ClearRerolls(&bc_table[H_3]);

				bc_table[H_3].value = (d * i);

				for (j = 0; j < 5; ++j)
					if (DiceValues[j].val != d) {
                                                TagReroll(&bc_table[H_3],j);
					}

				break;
			}
		}
/*
**	searching for 4 of a kind
*/
		if (p.score(H_4) < 0)
		{
			for (i = 3; i > 0; --i)
			{
				for (d = 6; d > 0; --d)
				{
					if (count(d) >= i)
						break;
				}

				if (d == 0)
					continue;

				bc_table[H_4].value = (d * i);

				ClearRerolls(&bc_table[H_4]);

				for (j = 0; j < 5; ++j)
					if (DiceValues[j].val != d) {
                                                TagReroll(&bc_table[H_4],j);
					}

				break;
			}
		}

/*
**	searching for yahtzee... we can't set the potential value too high
**	because if we fail, the value will be no better than 4 of a kind (or so)
**	so, we make scoring the same as for 3-4 of a kind (this is 5 of a kind!)
*/
		if (p.score(H_YA) < 0)
		{
			for (i = 4; i > 0; --i)
			{
				for (d = 6; d > 0; --d)
				{
					if (count(d) >= i)
						break;
				}

				if (d == 0)
					continue;

				bc_table[H_YA].value = (d * i);

				ClearRerolls(&bc_table[H_YA]);

				for (j = 0; j < 5; ++j)
					if (DiceValues[j].val != d)
                                                TagReroll(&bc_table[H_YA],j);

				break;
			}
		}

/*
**	searching for full house
*/
		if (p.score(H_FH) < 0)
		{
			for (i = 4; i > 0; --i)
			{
				d = find_n_of_a_kind(i, 0);

				if (d == 0)
					continue;

				for (j = i; j > 0; --j)
				{
					d2 = find_n_of_a_kind(j, d);

					if (d2 > 0)
						break;
				}

				if (j == 0)
					continue;

				ClearRerolls(&bc_table[H_FH]);

				bc_table[H_FH].value = (i * 24 + j * 36) / 6;

				for (i = 0; i < 5; ++i)
				{
					if (DiceValues[i].val != d &&
					  DiceValues[i].val != d2)
                                                TagReroll(&bc_table[H_FH],i);
				}

				break;
			}
		}
	}

/*
**	now we look hard at what we got
*/
	if (p.score(H_SS) < 0 && find_straight(4, 0, 0))
	{
		d = find_straight(4, 0, 0);

		for (i = 0; i < 5; ++i)
			if (count(DiceValues[i].val) > 1 ||
                            DiceValues[i].val < d || DiceValues[i].val > d + 3) {
                                TagReroll(&bc_table[H_SS],i);
				break;
			}

		bc_table[H_SS].value = 30;
	}

	if (p.score(H_LS) < 0 && find_straight(5, 0, 0))
	{
		bc_table[H_LS].value = 40;

		ClearRerolls(&bc_table[H_LS]);
	}

	if (p.score(H_CH) < 0 && NumberOfRolls > 2)
	{
		bc_table[H_CH].value = add_dice();

		if (bc_table[H_CH].value < 20)
			bc_table[H_CH].value /= 2;

		ClearRerolls(&bc_table[H_CH]);

		for (i = 0; i < 5; ++i)
			if (DiceValues[i].val < 4)
                                TagReroll(&bc_table[H_CH],i);
	}

	if (p.score(H_FH) < 0)
	{
		d = find_n_of_a_kind(3, 0);

		if (d != 0)
		{
			if (find_n_of_a_kind(2, d))
			{
				bc_table[H_FH].value = 25;

				ClearRerolls(&bc_table[H_FH]);
			}
		}
	}

	if (p.score(H_3) < 0)
	{
		d = find_n_of_a_kind(3, 0);

		if (d != 0)
		{
			bc_table[H_3].value = add_dice();

			ClearRerolls(&bc_table[H_3]);

			for (i = 0; i < 5; ++i)
				if (d != DiceValues[i].val)
                                        TagReroll(&bc_table[H_3],i);
		}
	}

	if (p.score(H_4) < 0)
	{
		d = find_n_of_a_kind(4, 0);

		if (d != 0)
		{
/*
**	there will be a tie between 3 of a kind and 4 of a kind. we add 1
**	to break the tie in favor of 4 of a kind
*/
			bc_table[H_4].value = add_dice() + 1;
			
			ClearRerolls(&bc_table[H_4]);

			for (i = 0; i < 5; ++i)
				if (d != DiceValues[i].val)
                                        TagReroll(&bc_table[H_4],i);
		}
	}

	if (find_n_of_a_kind(5, 0))
	{

		if (p.score(H_YA) == 0)
			bc_table[H_YA].value = -99;	/* scratch */

		else
			bc_table[H_YA].value = 150;	/* so he will use it! */
		
	}
}

void
ComputerRolling(const player &p, int NumberOfRolls)
{
	int i;
	int best;
	int bestv;

	BuildTable(p, NumberOfRolls);
        
	best = 0;
        
	bestv = -99;
        
	for (i = NUM_FIELDS-1; i >= 0; --i)
		if (bc_table[i].value >= bestv) {
                        best = i;
                        
                        bestv = bc_table[i].value;
                }

        for (i=0; i<5; ++i)
                DiceValues[i].sel = bc_table[best].rerolls[i];
}

/*
**	what we do here is generate a table of all the choices then choose
**	the highest value one.  in case of a tie, we go for the lower choice
**	cause higher choices tend to be easier to better
*/

int ComputerScoring(const player &p)
{
	int i;
	int best;
	int bestv;

	BuildTable(p, 2);

	best = 0;

	bestv = -99;

	for (i = NUM_FIELDS-1; i >= 0; --i) {
			if (bc_table[i].value > bestv)
			{
				best = i;

				bestv = bc_table[i].value;
			}
        }

	return best;
}

