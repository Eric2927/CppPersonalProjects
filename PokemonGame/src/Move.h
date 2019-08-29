#ifndef MOVE_H_
#define MOVE_H_

#include "General.h"

struct Move
{
	string m_moveName;		// Name of move
	Type m_type;		// Type of move (typical typings)
	MoveType m_moveType;		// Type of move (physical, special, or status)
	int m_rawDamage;		// Damage of move
	int m_powerPoint;		// Current PP of move
	int m_fullPowerPoint;		// Max PP of move
	int m_accuracy;			// Accuracy of move (out of 100)
	int m_secondaryChance;		// Chance of secondary effect (out of 100)
	SecondaryEffect m_effect;		// Secondary effect (primary effect if status move)
	double m_criticalRatio;			// Chance to get critical hit
	Move(int moveID = 0);		// Constructor
	bool hasPP();			// Returns true if there is PP left
};


#endif /* MOVE_H_ */
