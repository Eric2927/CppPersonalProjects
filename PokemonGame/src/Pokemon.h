#ifndef POKEMON_H_
#define POKEMON_H_

#include "General.h"
#include "Move.h"

class Pokemon
{
	string m_name;			// Stores name of the pokemon
	Type m_primaryType;		// Stores primary type of the pokemon
	Type m_secondaryType;	// Stores secondary type of the pokemon (assuming it has two typings, if not, it is set to None)
	Ability m_ability;		// Stores ability of the pokemon
	int m_HP;				// Stores HP stat of pokemon
	int m_Attack;					// Stores Attack stat of pokemon
	int m_Defense;					// Stores Defense stat of pokemon
	int m_SpecialAttack;			// Stores Special Attack stat of pokemon
	int m_SpecialDefense;			// Stores Special Defense stat of pokemon
	int m_Speed;					// Stores Speed stat of pokemon
	int m_originalStats[6];			// Stores the original (base) stats of the pokemon
	int m_statModifications[5];				// Stores the stat modifications (excluding HP)
	vector<Move> m_learnableMoves;			// Stores the learnable moves of the pokemon
	Move m_moveSet[4];						// Stores the official moveset of the pokemon
	PokemonStatus m_pokemonStatus;			// Stores the pokemon status
	PokemonStatus m_otherStatus;			// Stores the pokemon's secondary status (bizzare game mechanic, explain during presentation)
	int m_level;							// Stores level of the pokemon

public:
	Pokemon(int ID = 0, bool customize = false);		// Constructor (doubles as default constructor)
	void takeMove(Pokemon* opponent, Move &move, Weather weather, vector<FieldStatus> fieldStatus);		// Simulates the pokemon taking a move (read function implementation for more details)
	double checkEffective(Move move);				// Checks how effective the move (parameter) is against the pokemon (typing multiplier)
	void checkStatus(bool &attack);					// Checks the status of pokemon and if it renders the pokemon unable to attack for the turn
	void checkSpecialEffect(Pokemon* opponent);		// Applies the effects of certain statuses (used at end of turns)
	void applySpecialEffect(Move move, int damage, Pokemon* opponent, Weather weather);		// Applies the secondary effect of attacking moves or the effect of status moves
	int processStat(int rawStat, int EV, int level, bool HP = false);
	int promptUserForEV(string stat, int &EVsLeft);		// Prompts user for EVs (if user chooses to customize pokemon)
	void resetStatChanges();			// Resets stat changes (during switch out)
	string getName();			// Returns name of pokemon
	Move* getMoves();			// Returns pointer to the array of moves (moveset)
	int getHP();		// Returns the current HP of the pokemon
	int getFullHP();	// Returns the maximum HP of the pokemon
	PokemonStatus getPokemonStatus();		// Returns the status of the pokemon
	PokemonStatus getOtherStatus();			// Returns the other status of the pokemon
	int* getStatChanges();		// Returns a pointer to an array of integers that represents the stat changes
	void recalculateStats();	// Recalculates the stats using the stat changes array
	void displayStats();		// Displays stats of the pokemon
	bool isAttackSTAB(Move move);		// Returns true if the attack is STAB (same type attack bonus)
	friend class Battle;		// Allows battle to access the private members of this class
};


#endif /* POKEMON_H_ */
