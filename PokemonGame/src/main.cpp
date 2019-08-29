#include "General.h"
#include "Pokemon.h"
#include "Move.h"
#include "Battle.h"

// Author: Eric Li

// NOTE: I have not implemented many dumb-user safeties (like if they enter invalid options).
// I just specified directions clearly in the directions at the beginning.

// Generates random team for AI (can have repeats)
void generateRandomTeam(Pokemon* team);

// Calculates the AI's next move
int AIDecide(Battle* battle, bool &fight);

int main()
{
	srand(time(0));					// Seeds the random numbers
	Pokemon userTeam[3];			// Stores user team
	Pokemon AITeam[3];				// Stores AI team
	generateRandomTeam(AITeam);		// Makes random AI team and stores it in AITeam

	// Basic instructions and introduction to the game
	cout << "Welcome to Pokemon 3v3!" << endl;
	cout << "Instructions: Before we begin, please note that whenever you are asked"
			"to choose something from a numbered list, you need to enter the number next to the"
			"item you want to choose, not the name. If you are asked a yes or no question, please"
			"enter y or n, respectively (in lowercase). Please follow the directions or else this game will not work!" << endl << endl;
	cout << "Choose three of the following pokemon for your team." << endl;
	cout << "1. Charizard\n"
			"2. Blastoise\n"
			"3. Venusaur\n"
			"4. Pikachu\n"
			"5. Onix\n"
			"6. Pidgeot\n"
			"7. Gyarados\n"
			"8. Gengar\n"
			"9. Alakazam\n"
			"10. Dragonite\n"
			"11. Lapras\n"
			"12. Machamp\n"
			"13. Butterfree\n"
			"14. Beedrill\n"
			"15. Snorlax\n";
	int numPokemonChosen = 0;
	string userResponse;
	int userNumResponse;
	while (numPokemonChosen < 3)		// Keeps looping until user has chosen 3 pokemon for their team
	{
		bool customize = false;
		cout << "Pokemon #" << numPokemonChosen + 1 << ": ";		// Asks user to enter the ID of the pokemon they want
		int pokemonID;
		cin >> pokemonID;
		cin.ignore();
		if (pokemonID > 15 || pokemonID < 1)			// Checks if the pokemon ID the user entered is valid
		{
			cout << "You did not enter a valid Pokemon. Remember to enter"
					"the number next to the Pokemon, not its actual name." << endl;
			continue;
		}
		cout << "Would you like to customize this pokemon?" << endl;		// Asks if user wants to customize the pokemon
		getline(cin, userResponse);
		if (userResponse.compare("y") == 0)			// Sets customize to true if user wants to customize
		{
			customize = true;
		}
		Pokemon tempPokemon(pokemonID, customize);			// Makes the pokemon the user specified (with customizations if they wanted it)
		userTeam[numPokemonChosen] = tempPokemon;			// Adds that pokemon to the user's team
		numPokemonChosen ++;
	}
	cout << "Opponent Team:" << endl;
	for (int i = 0; i < 3; i ++)						// Displays opponent's team so user can make educated choice for their leading pokemon
	{
		cout << i + 1 << ". " << AITeam[i].getName() << endl;
	}
	cout << "Which Pokemon would you like to lead with?" << endl;		// Asks the user what pokemon they want to lead with
	for (int i = 0; i < 3; i ++)										// Displays user's team
	{
		cout << i + 1 << ". " << userTeam[i].getName() << endl;
	}
	cin >> userNumResponse;
	cin.ignore();
	int AILead = rand() % 3;			// Chooses a random pokemon on the AI team for AI to send out

	// Prints out the leading pokemon on both teams
	cout << "You sent out your " << userTeam[userNumResponse - 1].getName() << endl;
	cout << "The opponent sent out its " << AITeam[AILead].getName() << endl;
	Battle battle(userTeam, AITeam, userNumResponse - 1, AILead);
	bool win;			// stores whether or not user won (true if user won, false otherwise)
	while (true)
	{
		battle.displayUI();			// Displays UI of battle (read the Battle class for more details)
		bool userFight;				// Boolean to store whether user is going to fight or switch (true if user fights, false if they choose to switch out)
		int userChoiceID;			// Boolean to store the move number the user wants to use if they choose to fight (useless if they choose to switch out)
		battle.getUserChoice(userFight, userChoiceID);			// Prompts user to choose whether to fight or switch, and initializes the previous two booleans appropriately
		if (userFight && !(battle.getActiveUserPokemon()->getMoves()[userChoiceID - 1].hasPP()))	// Checks if user's chosen move has PP (if they chose to fight)
		{
			cout << "This move has no PP left!" << endl;
			continue;
		}
		bool AIFight;			// Boolean to store whether AI is going to fight or switch (true if AI fights, false if AI switches)
		int AIMoveID = AIDecide(&battle, AIFight);		// AI calculates whether it wants to fight or switch (check the function implementation for more details)
		battle.executeTurn(userFight, AIFight, userChoiceID - 1, AIMoveID, win);		// Executes a turn based on what the user and AI decided on
		if (battle.isBattleOver(win))			// Checks if the battle is over, if so, breaks out of game loop
		{
			break;
		}
	}
	// Checks if user won, if so, prints a congratulation message. Otherwise, prints a lose message.
	if (win)
	{
		cout << "All of the AI's pokemon have fainted. You won. Congratulations." << endl;
	}
	else
	{
		cout << "All of your pokemon have fainted. You lost. Try again next time." << endl;
	}
}


void generateRandomTeam(Pokemon* team)
{
	for (int i = 0; i < 3; i ++)
	{
		Pokemon aiPokemon(rand() % 15 + 1);		// Generates random number between 1 and 15, inclusive, as pokemon ID
		team[i] = aiPokemon;					// Adds that pokemon to AI team
	}
}

/*
 * Takes in a battle pointer parameter and a reference to a boolean called fight.
 * The battle pointer allows the AI to adequately assess the situation.
 * The boolean reference is set based on if the user wants to use a move or switch out.
 * Returns an integer representing the move number (ID) that the AI wants to use, given
 * that it decides to fight.
 * Returns -2 if AI doesn't fight. Returns -1 if AI's pokemon has to struggle. Otherwise, returns move ID.
 */
int AIDecide(Battle* battle, bool &fight)
{
	fight = true;			// Initializes fight to true

	// Checks if the other pokemon has STAB (Same Type Attack Bonus) moves that are super effective (2x or more damage)
	// If so, AI will choose to switch out (fight = false)
	// Code for AI's decision on what to switch out to is in the Battle class, promptSwitch() function
	for (int i = 0; i < 4; i ++)
	{
		if ((battle->getActiveAIPokemon()->checkEffective(battle->getActiveUserPokemon()->getMoves()[i]) > 1) && (battle->getActiveUserPokemon()->isAttackSTAB(battle->getActiveUserPokemon()->getMoves()[i])))
		{
			fight = false;
		}
	}

	// If AI decides to switch out, this checks if the AI has anything to switch out to (if everything else is fainted or not)
	// If AI has nothing to switch out to, AI will fight (fight = true)
	if (!fight)
	{
		int faintCounter = 0;
		for (int i = 0; i < 3; i ++)
		{
			if (battle->getAITeam()->getPokemonStatus() == Fainted)
			{
				faintCounter ++;
			}
		}
		if (faintCounter == 2)
		{
			fight = true;
		}
	}

	// Code for if AI does end up deciding to fight
	if (fight)
	{
		int bestMoveID = 0;

		// Checks if all of the AI's pokemon's moves are out of PP (can't use any moves)
		// If so, fight is set to false and -1 is returned
		while (!battle->getActiveAIPokemon()->getMoves()[bestMoveID].hasPP())
		{
			bestMoveID ++;
			if (bestMoveID > 3)
			{
				fight = false;
				return -1;
			}
		}
		bestMoveID = 0;
		bool hasStatusMove = false;			// boolean that represents if AI's active pokemon's movepool has a status move (true if so, false if not)

		// Loops through the AI's pokemon's moves. If any one of them is a status move, hasStatusMove is set to true
		for (int i = 0; i < 4; i ++)
		{
			if (battle->getActiveAIPokemon()->getMoves()[i].m_moveType == Status)
			{
				hasStatusMove = true;
			}
		}

		// Prioritizes healing if AI's pokemon is below half HP
		// Given that AI's pokemon has status moves and its HP is below half of its maximum HP,
		// this code loops through all of the moves and if a move is a status move, it checks if
		// the effect is a self-heal. If so, that move ID is returned (aka AI will use it)
		if ((battle->getActiveAIPokemon()->getHP() < battle->getActiveAIPokemon()->getFullHP() * 0.5) && hasStatusMove)
		{
			for (int i = 0; i < 4; i ++)
			{
				if (battle->getActiveAIPokemon()->getMoves()[i].m_moveType == Status)
				{
					switch (battle->getActiveAIPokemon()->getMoves()[i].m_effect)
					{
					case Synthesis:
						return i;
					case Roost:
						return i;
					case Recover:
						return i;
					}
				}
			}
		}

		// Second priority is to hinder the other pokemon's health
		// Given that AI's pokemon has a status move and the other pokemon is healthy,
		// this code loops through all of the moves and if a move is a status move, it checks if
		// the effect will give a status to the opponent. If so, that move ID is returned
		if (hasStatusMove && battle->getActiveUserPokemon()->getPokemonStatus() == Healthy)
		{
			for (int i = 0; i < 4; i ++)
			{
				if (battle->getActiveAIPokemon()->getMoves()[i].m_moveType == Status)
				{
					switch (battle->getActiveAIPokemon()->getMoves()[i].m_effect)
					{
					case E_Burn:
						return i;
					case E_Paralyze:
						return i;
					case E_Poison:
						return i;
					case E_Sleep:
						return i;
					case E_Freeze:
						return i;
					case E_Confuse:
						return i;
					case Leech_Seed:
						if (battle->getActiveUserPokemon()->getOtherStatus() != Seeded)
						{
							return i;
						}
						break;
					}
				}
			}
		}
		bool hasStatBoosts = false;				// Boolean representing if AI's pokemon have stat boosts

		// Loops throught the stat modifications array of the AI's pokemon
		// If any one of them is positive (aka has a stat boost), then hasStatBoosts is set to true
		for (int i = 0; i < 5; i ++)
		{
			if (battle->getActiveAIPokemon()->getStatChanges()[i] > 0)
			{
				hasStatBoosts = true;
			}
		}

		// Third priority is to self-boost stats
		// Given that AI's pokemon doesn't have stat boosts and has a status move,
		// this code loops through every move and if the move is a status move, it
		// checks if the effect is a self stat boost. If so, the move ID is returned
		if (!hasStatBoosts && hasStatusMove)
		{
			for (int i = 0; i < 4; i ++)
			{
				if (battle->getActiveAIPokemon()->getMoves()[i].m_moveType == Status)
				{
					switch (battle->getActiveAIPokemon()->getMoves()[i].m_effect)
					{
					case R_Speed2:
						return i;
					case Dragon_Dance:
						return i;
					case Calm_Mind:
						return i;
					case Quiver_Dance:
						return i;
					case Curse:
						return i;
					case Bulk_Up:
						return i;
					}
				}
			}
		}

		// Last priority is to actually damage opponent (I know, weird, but it's how I would play it if I was an AI)
		// Loops through every move (ignoring the first move, which is initially stored in bestMoveID) and compares it
		// to the move stored at bestMoveID.
		for (int i = bestMoveID + 1; i < 4; i ++)
		{
			// First checks if the move has PP. If not, then the move is not considered
			if (battle->getActiveAIPokemon()->getMoves()[i].hasPP())
			{
				// Then compares how effective the move at index i is against the opponent versus the move at index "bestMoveID"
				// If the move at index i is more effective than the move at index bestMoveID, then bestMoveID is set to i
				// For more details, check the function implementation in the Pokemon class for "checkEffective()"
				if (battle->getActiveUserPokemon()->checkEffective(battle->getActiveAIPokemon()->getMoves()[i]) > battle->getActiveUserPokemon()->checkEffective(battle->getActiveAIPokemon()->getMoves()[bestMoveID]))
				{
					bestMoveID = i;
				}
				// Checks to see if both moves are equally effective
				else if (battle->getActiveUserPokemon()->checkEffective(battle->getActiveAIPokemon()->getMoves()[i]) == battle->getActiveUserPokemon()->checkEffective(battle->getActiveAIPokemon()->getMoves()[bestMoveID]))
				{
					// If the move at i is a STAB move and the move at bestMoveID is not, then bestMoveID is set to i
					if ((battle->getActiveAIPokemon()->isAttackSTAB(battle->getActiveAIPokemon()->getMoves()[i])) && !(battle->getActiveAIPokemon()->isAttackSTAB(battle->getActiveAIPokemon()->getMoves()[bestMoveID])))
					{
						bestMoveID = i;
					}
					// If the move at i and the move at bestMoveID are both STAB or are both not STAB
					else if ((battle->getActiveAIPokemon()->isAttackSTAB(battle->getActiveAIPokemon()->getMoves()[i])) == (battle->getActiveAIPokemon()->isAttackSTAB(battle->getActiveAIPokemon()->getMoves()[bestMoveID])))
					{
						// Compares the raw damage of the moves. If the move at i does more damage than the mvoe at bestMoveID,
						// then bestMoveID is replaced with i
						if (battle->getActiveAIPokemon()->getMoves()[i].m_rawDamage > battle->getActiveAIPokemon()->getMoves()[bestMoveID].m_rawDamage)
						{
							bestMoveID = i;
						}
					}
				}
			}
		}
		// Returns bestMoveID
		return bestMoveID;
	}
	return -2;

}


