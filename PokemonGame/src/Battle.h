#ifndef BATTLE_H_
#define BATTLE_H_

#include "General.h"
#include "Pokemon.h"
#include "Move.h"

class Battle
{
	int m_turn;		// Indicates turn number (not really used right now, it is here for future development)
	Weather m_weather;		// Represents current weather on the field
	Pokemon* m_userTeam;	// Pointer to array storing user's team
	Pokemon* m_AITeam;		// Pointer to array storing AI's team
	Pokemon* m_activeUserPokemon;		// Pointer to the user's pokemon currently on the field
	Pokemon* m_activeAIPokemon;			// Pointer to the AI's pokemon currently on the field
	vector<FieldStatus> m_fieldStatuses;		// Field statuses (not currently used, for later development)

public:
	Battle(Pokemon* userTeam, Pokemon* AITeam, int activeUserPokemonID, int activeAIPokemonID);		// constructor
	void displayTeam(bool user);			// Displays team (if parameter is true, displays user team. otherwise, displays AI team)
	void displayActiveMoves(bool user);		// Displays moveset of active pokemon (parameter works same way as displayTeam())
	void promptSwitch(bool user, bool fainted);		// Performs a pokemon switch either for user or AI
	void displayUI();								// Displays user interface
	void getUserChoice(bool &userFight, int &userChoice);		// Gets the user's choice on wether to fight or not
	bool isBattleOver(bool &userWin);			// Returns a boolean that represents if the battle is over (true if battle over)
	void executeTurn(bool userFight, bool AIFight, int userNum, int AINum, bool &userWin);		// Simulates a turn
	Pokemon* getActiveUserPokemon();		// Returns a pointer to the active user pokemon
	Pokemon* getActiveAIPokemon();			// Returns a pointer to the active AI pokemon
	Pokemon* getAITeam();			// Returns a pointer to the array storing the AI team
};

#endif /* BATTLE_H_ */
