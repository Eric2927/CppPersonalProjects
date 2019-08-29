#include "Move.h"

Move::Move(int moveID)
{
	if (moveID == 0)
	{
		m_moveName = "";
		m_type = None;
		m_moveType = Undefined;
		m_rawDamage = -1;
		m_powerPoint = -1;
		m_fullPowerPoint = -1;
		m_accuracy = -1;
		m_secondaryChance = -1;
		m_effect = No_Effect;
		m_criticalRatio = -1;
	}
	else
	{
		// Reads file Moves.txt
		// Check Moves.txt for more info on the format
		ifstream inMoves("Moves.txt");
		int lineNum = 1;
		while (!inMoves.eof())
		{
			if (moveID == lineNum)
			{
				inMoves.ignore(1);
				while (inMoves.peek() != 34)
				{
					char tempChar;
					inMoves >> noskipws >> tempChar;
					m_moveName += tempChar;
				}
				inMoves.ignore(1);
				int tempInt;
				inMoves >> skipws >> tempInt;
				m_type = (Type) tempInt;
				inMoves >> tempInt;
				m_moveType = (MoveType) tempInt;
				inMoves >> m_rawDamage;
				inMoves >> m_powerPoint;
				m_fullPowerPoint = m_powerPoint;
				inMoves >> m_accuracy;
				inMoves >> m_secondaryChance;
				inMoves >> tempInt;
				m_effect = (SecondaryEffect) tempInt;
				break;
			}
			inMoves.ignore(256, '\n');
			lineNum ++;
		}
		if (m_effect == HighCrit)
		{
			m_criticalRatio = 20;
		}
		else
		{
			m_criticalRatio = 10;
		}
	}
}

bool Move::hasPP()
{
	if (m_powerPoint > 0)
	{
		return true;
	}
	return false;
}
