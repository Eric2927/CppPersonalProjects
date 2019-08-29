#include "General.h"
#include "Battle.h"


Battle::Battle(Pokemon* userTeam, Pokemon* AITeam, int activeUserPokemonID, int activeAIPokemonID)
{
	// Initializes everything
	m_userTeam = userTeam;
	m_AITeam = AITeam;
	m_activeUserPokemon = &m_userTeam[activeUserPokemonID];
	m_activeAIPokemon = &m_AITeam[activeAIPokemonID];
	m_turn = 1;
	m_weather = Clear;

	// Accomodates for the ability intimidate, which lowers the opponent's attack by one stage when the pokemon is first sent out
	if (m_activeUserPokemon->m_ability == Intimidate)
	{
		m_activeAIPokemon->m_statModifications[0] --;
		cout << m_activeUserPokemon->m_name << "'s intimidate lowered " << m_activeAIPokemon->m_name << "'s attack!" << endl;
		m_activeAIPokemon->recalculateStats();
	}
	if (m_activeAIPokemon->m_ability == Intimidate)
	{
		m_activeUserPokemon->m_statModifications[0] --;
		cout << m_activeAIPokemon->m_name << "'s intimidate lowered " << m_activeUserPokemon->m_name << "'s attack!" << endl;
		m_activeUserPokemon->recalculateStats();
	}
}

void Battle::displayActiveMoves(bool user)
{
	if (user)
	{
		for (int i = 0; i < 4; i ++)
		{
			cout << i + 1 << ". " << m_activeUserPokemon->getMoves()[i].m_moveName << " PP: "
					<< m_activeUserPokemon->getMoves()[i].m_powerPoint << "/"
					<< m_activeUserPokemon->getMoves()[i].m_fullPowerPoint << endl;
		}
	}
	else
	{
		for (int i = 0; i < 4; i ++)
		{
			cout << i + 1 << ". " << m_activeAIPokemon->getMoves()[i].m_moveName << " PP: "
					<< m_activeAIPokemon->getMoves()[i].m_powerPoint << "/"
					<< m_activeAIPokemon->getMoves()[i].m_fullPowerPoint << endl;
		}
	}
}


void Battle::displayTeam(bool user)
{
	if (user)
	{
		for (int i = 0; i < 3; i ++)
		{
			cout << i + 1 << ". " << m_userTeam[i].getName() << endl;
		}
	}
	else
	{
		for (int i = 0; i < 3; i ++)
		{
			cout << i + 1 << ". " << m_AITeam[i].getName() << endl;
		}
	}
}



void Battle::displayUI()
{
	string typeString[16] = {"Normal", "Fire", "Water", "Electric", "Grass",
							"Ice", "Fighting", "Poison", "Ground", "Flying",
							"Psychic", "Bug", "Rock", "Ghost", "Dragon", ""};

	cout << "Your Pokemon: " << m_activeUserPokemon->getName() << endl;			// Displays user's pokemon's name

	// Displays user's pokemon's typing
	cout << "Type: " << "[" << typeString[m_activeUserPokemon->m_primaryType] << "]";
	if (m_activeUserPokemon->m_secondaryType != None)
	{
		cout << " [" << typeString[m_activeUserPokemon->m_secondaryType] << "]" << endl;
	}
	else
	{
		cout << endl;
	}

	// Displays health of user's pokemon out of max health
	cout << "HP: " << m_activeUserPokemon->getHP() << "/" <<
			m_activeUserPokemon->getFullHP() << " ";

	// Displays stat changes of user pokemon
	for (int i = 0; i < 5; i ++)
	{
		if (m_activeUserPokemon->getStatChanges()[i] != 0)
		{
			switch (i)
			{
			case 0:
				cout << "(Atk ";
				if (m_activeUserPokemon->getStatChanges()[i] > 0)
				{
					cout << "+" << m_activeUserPokemon->getStatChanges()[i] << ") ";
				}
				else
				{
					cout << m_activeUserPokemon->getStatChanges()[i] << ") ";
				}
				break;
			case 1:
				cout << "(Def ";
				if (m_activeUserPokemon->getStatChanges()[i] > 0)
				{
					cout << "+" << m_activeUserPokemon->getStatChanges()[i] << ") ";
				}
				else
				{
					cout << m_activeUserPokemon->getStatChanges()[i] << ") ";
				}
				break;
			case 2:
				cout << "(SpA ";
				if (m_activeUserPokemon->getStatChanges()[i] > 0)
				{
					cout << "+" << m_activeUserPokemon->getStatChanges()[i] << ") ";
				}
				else
				{
					cout << m_activeUserPokemon->getStatChanges()[i] << ") ";
				}
				break;
			case 3:
				cout << "(SpD ";
				if (m_activeUserPokemon->getStatChanges()[i] > 0)
				{
					cout << "+" << m_activeUserPokemon->getStatChanges()[i] << ") ";
				}
				else
				{
					cout << m_activeUserPokemon->getStatChanges()[i] << ") ";
				}
				break;
			case 4:
				cout << "(Spe ";
				if (m_activeUserPokemon->getStatChanges()[i] > 0)
				{
					cout << "+" << m_activeUserPokemon->getStatChanges()[i] << ") ";
				}
				else
				{
					cout << m_activeUserPokemon->getStatChanges()[i] << ") ";
				}
				break;
			}
		}
	}

	// Displays status of user pokemon
	switch (m_activeUserPokemon->getPokemonStatus())
	{
	case Paralysis:
		cout << "(Paralyzed)";
		break;
	case Burn:
		cout << "(Burned)";
		break;
	case Poisoned:
		cout << "(Poisoned)";
		break;
	case Sleep:
		cout << "(Asleep)";
		break;
	case Freeze:
		cout << "(Frozen)";
		break;
	case Confused:
		cout << "(Confused)";
		break;
	}
	cout << endl;


	// Does same thing for AI pokemon
	cout << "Opposing Pokemon: " << m_activeAIPokemon->getName() << endl;
	cout << "Type: " << "[" << typeString[m_activeAIPokemon->m_primaryType] << "]";
	if (m_activeAIPokemon->m_secondaryType != None)
	{
		cout << " [" << typeString[m_activeAIPokemon->m_secondaryType] << "]" << endl;
	}
	else
	{
		cout << endl;
	}
	cout << "HP: " << m_activeAIPokemon->getHP() << "/" <<
			m_activeAIPokemon->getFullHP() << " ";
	for (int i = 0; i < 5; i ++)
	{
		if (m_activeAIPokemon->getStatChanges()[i] != 0)
		{
			switch (i)
			{
			case 0:
				cout << "(Atk ";
				if (m_activeAIPokemon->getStatChanges()[i] > 0)
				{
					cout << "+" << m_activeAIPokemon->getStatChanges()[i] << ") ";
				}
				else
				{
					cout << m_activeAIPokemon->getStatChanges()[i] << ") ";
				}
				break;
			case 1:
				cout << "(Def ";
				if (m_activeAIPokemon->getStatChanges()[i] > 0)
				{
					cout << "+" << m_activeAIPokemon->getStatChanges()[i] << ") ";
				}
				else
				{
					cout << m_activeAIPokemon->getStatChanges()[i] << ") ";
				}
				break;
			case 2:
				cout << "(SpA ";
				if (m_activeAIPokemon->getStatChanges()[i] > 0)
				{
					cout << "+" << m_activeAIPokemon->getStatChanges()[i] << ") ";
				}
				else
				{
					cout << m_activeAIPokemon->getStatChanges()[i] << ") ";
				}
				break;
			case 3:
				cout << "(SpD ";
				if (m_activeAIPokemon->getStatChanges()[i] > 0)
				{
					cout << "+" << m_activeAIPokemon->getStatChanges()[i] << ") ";
				}
				else
				{
					cout << m_activeAIPokemon->getStatChanges()[i] << ") ";
				}
				break;
			case 4:
				cout << "(Spe ";
				if (m_activeAIPokemon->getStatChanges()[i] > 0)
				{
					cout << "+" << m_activeAIPokemon->getStatChanges()[i] << ") ";
				}
				else
				{
					cout << m_activeAIPokemon->getStatChanges()[i] << ") ";
				}
				break;
			}
		}
	}
	switch (m_activeAIPokemon->getPokemonStatus())
	{
	case Paralysis:
		cout << "(Paralyzed)";
		break;
	case Burn:
		cout << "(Burned)";
		break;
	case Poisoned:
		cout << "(Poisoned)";
		break;
	case Sleep:
		cout << "(Asleep)";
		break;
	case Freeze:
		cout << "(Frozen)";
		break;
	case Confused:
		cout << "(Confused)";
		break;
	}
	cout << endl;
}

void Battle::getUserChoice(bool &userFight, int &userChoice)
{
	bool madeChoice = false;

	// Allows user to choose many things, two of which are to proceed in the game, the rest of which are to help
	// the user make a decision
	while (!madeChoice)
	{
		cout << "What would you like to do?" << endl;
		cout << "1. Fight\n"
				"2. Switch out\n"
				"3. Display your Pokemon's stats\n"
				"4. Display your Opponent's stats\n"
				"5. Display your team\n"
				"6. Display AI's team" << endl;
		int userInput;
		cin >> userInput;
		cin.ignore();
		switch (userInput)
		{
		case 1:
			cout << "Which move would you like to use?" << endl;
			displayActiveMoves(true);
			cin >> userChoice;
			cin.ignore();
			userFight = true;
			madeChoice = true;
			break;
		case 2:
			userFight = false;
			madeChoice = true;
			break;
		case 3:
			m_activeUserPokemon->displayStats();
			break;
		case 4:
			m_activeAIPokemon->displayStats();
			break;
		case 5:
			displayTeam(true);
			break;
		case 6:
			displayTeam(false);
			break;
		}
	}
}

// Returns true if battle is over, and sets the reference boolean to true if user won, false if AI won
bool Battle::isBattleOver(bool &userWin)
{
	bool userLost = true;
	bool AILost = true;
	for (int i = 0; i < 3; i ++)
	{
		if (m_userTeam[i].getPokemonStatus() != Fainted)
		{
			userLost = false;
		}
		if (m_AITeam[i].getPokemonStatus() != Fainted)
		{
			AILost = false;
		}
	}
	if (userLost && !AILost)
	{
		userWin = false;
		return true;
	}
	else if (!userLost && AILost)
	{
		userWin = true;
		return true;
	}
	else
	{
		return false;
	}
}

void Battle::executeTurn(bool userFight, bool AIFight, int userNum, int AINum, bool &userWin)
{
	bool userPriority = false;
	bool AIPriority = false;
	bool userGoFirst;
	if (userFight && AIFight)		// If both user and AI want to fight (use a move)
	{
		// Calculates who goes first
		if (m_activeUserPokemon->getMoves()[userNum].m_effect == FirstPriority)
		{
			userPriority = true;
		}
		if (m_activeAIPokemon->getMoves()[AINum].m_effect == FirstPriority)
		{
			AIPriority = true;
		}
		if (!userPriority && AIPriority)
		{
			userGoFirst = false;
		}
		else if (userPriority && !AIPriority)
		{
			userGoFirst = true;
		}
		else
		{
			if (m_activeUserPokemon->m_Speed > m_activeAIPokemon->m_Speed)
			{
				userGoFirst = true;
			}
			else if (m_activeUserPokemon->m_Speed < m_activeAIPokemon->m_Speed)
			{
				userGoFirst = false;
			}
			else
			{
				userGoFirst = rand() % 2;
			}
		}

		// If user goes first
		if (userGoFirst)
		{
			bool userAttack = true;
			m_activeUserPokemon->checkStatus(userAttack);		// Checks if user pokemon can attack (based on status)
			if (userAttack)
			{
				cout << "Your " << m_activeUserPokemon->getName() << " used " << m_activeUserPokemon->getMoves()[userNum].m_moveName << endl;
				m_activeAIPokemon->takeMove(m_activeUserPokemon, (m_activeUserPokemon->getMoves())[userNum], m_weather, m_fieldStatuses);
			}
			if (m_activeAIPokemon->getPokemonStatus() == Fainted)
			{
				cout << "The opposing " << m_activeAIPokemon->getName() << " has fainted!" << endl;
				if (!isBattleOver(userWin))
				{
					promptSwitch(false, true);
				}
				return;
			}
			bool AIAttack = true;
			if (rand() % 100 < m_activeUserPokemon->getMoves()[userNum].m_secondaryChance)
			{
				switch (m_activeUserPokemon->getMoves()[userNum].m_effect)
				{
				case Flinch:
					cout << "The opposing " << m_activeAIPokemon->getName() << " flinched!" << endl;
					AIAttack = false;
					break;
				case F_Sun:
					cout << "The sunlight became harsh!" << endl;
					m_weather = Sun;
					break;
				}
			}
			m_activeAIPokemon->checkStatus(AIAttack);
			if (AIAttack)
			{
				cout << "The opposing " << m_activeAIPokemon->getName() << " used " << m_activeAIPokemon->getMoves()[AINum].m_moveName << endl;
				m_activeUserPokemon->takeMove(m_activeAIPokemon, m_activeAIPokemon->getMoves()[AINum], m_weather, m_fieldStatuses);
			}
		}
		else
		{
			bool AIAttack = true;
			m_activeAIPokemon->checkStatus(AIAttack);
			if (AIAttack)
			{
				cout << "The opposing " << m_activeAIPokemon->getName() << " used " << m_activeAIPokemon->getMoves()[AINum].m_moveName << endl;
				m_activeUserPokemon->takeMove(m_activeAIPokemon, m_activeAIPokemon->getMoves()[AINum], m_weather, m_fieldStatuses);
			}
			if (m_activeUserPokemon->getPokemonStatus() == Fainted)
			{
				cout << "Your " << m_activeUserPokemon->getName() << " has fainted!" << endl;
				if (!isBattleOver(userWin))
				{
					promptSwitch(true, true);
				}
				return;
			}
			bool userAttack = true;
			if (rand() % 100 < m_activeAIPokemon->getMoves()[userNum].m_secondaryChance)
			{
				switch (m_activeAIPokemon->getMoves()[userNum].m_effect)
				{
				case Flinch:
					cout << "Your " << m_activeUserPokemon->getName() << " flinched!" << endl;
					userAttack = false;
					break;
				case F_Sun:
					cout << "The sunlight became harsh!" << endl;
					m_weather = Sun;
					break;
				/*case F_Reflect:
					m_fieldStatuses.push_back(Reflect);
					break;
				case F_LightScreen:
					m_fieldStatuses.push_back(Light_Screen);
					break; */
				}
			}
			m_activeUserPokemon->checkStatus(userAttack);
			if (userAttack)
			{
				cout << "Your " << m_activeUserPokemon->getName() << " used " << m_activeUserPokemon->getMoves()[userNum].m_moveName << endl;
				m_activeAIPokemon->takeMove(m_activeUserPokemon, m_activeUserPokemon->getMoves()[userNum], m_weather, m_fieldStatuses);
			}
		}
	}
	else if (userFight && !AIFight)
	{
		if (AINum == -1)
		{
			cout << m_activeAIPokemon->m_name << " has no moves left! It struggled!" << endl;
			m_activeAIPokemon->m_HP -= m_activeAIPokemon->m_originalStats[0] * 0.2;
			if (m_activeAIPokemon->m_HP <= 0)
			{
				m_activeAIPokemon->m_HP = 0;
				m_activeAIPokemon->m_pokemonStatus = Fainted;
				promptSwitch(false, true);
			}
		}
		else
		{
			promptSwitch(false, false);
		}
		bool userAttack = true;
		m_activeUserPokemon->checkStatus(userAttack);
		if (userAttack)
		{
			cout << "Your " << m_activeUserPokemon->getName() << " used " << m_activeUserPokemon->getMoves()[userNum].m_moveName << endl;
			m_activeAIPokemon->takeMove(m_activeUserPokemon, m_activeUserPokemon->getMoves()[userNum], m_weather, m_fieldStatuses);
		}
	}
	else if (!userFight && AIFight)
	{
		promptSwitch(true, false);
		bool AIAttack = true;
		m_activeAIPokemon->checkStatus(AIAttack);
		if (AIAttack)
		{
			cout << "The opposing " << m_activeAIPokemon->getName() << " used " << m_activeAIPokemon->getMoves()[AINum].m_moveName << endl;
			m_activeUserPokemon->takeMove(m_activeAIPokemon, m_activeAIPokemon->getMoves()[AINum], m_weather, m_fieldStatuses);
		}
	}
	else
	{
		promptSwitch(true, false);
		if (AINum == -1)
		{
			cout << m_activeAIPokemon->m_name << " has no moves left! It struggled!" << endl;
			m_activeAIPokemon->m_HP -= m_activeAIPokemon->m_originalStats[0] * 0.2;
			if (m_activeAIPokemon->m_HP <= 0)
			{
				m_activeAIPokemon->m_HP = 0;
				m_activeAIPokemon->m_pokemonStatus = Fainted;
				promptSwitch(false, true);
			}
		}
		else
		{
			promptSwitch(false, false);
		}
	}
	if (m_activeAIPokemon->getPokemonStatus() == Fainted)
	{
		cout << "The opposing " << m_activeAIPokemon->getName() << " has fainted!" << endl;
		if (!isBattleOver(userWin))
		{
			promptSwitch(false, true);
		}
	}
	if (m_activeUserPokemon->getPokemonStatus() == Fainted)
	{
		cout << "Your " << m_activeUserPokemon->getName() << " has fainted!" << endl;
		if (!isBattleOver(userWin))
		{
			promptSwitch(true, true);
		}
	}
	m_activeUserPokemon->checkSpecialEffect(m_activeAIPokemon);
	m_activeAIPokemon->checkSpecialEffect(m_activeUserPokemon);
}


void Battle::promptSwitch(bool user, bool fainted)
{
	if (user)
	{
		while (true)
		{
			cout << "What would you like to switch out to?" << endl;
			displayTeam(true);
			int userChoice;
			cin >> userChoice;
			cin.ignore();
			m_activeUserPokemon->resetStatChanges();
			m_activeUserPokemon->recalculateStats();
			if (m_userTeam[userChoice - 1].getPokemonStatus() == Fainted)
			{
				cout << "Your " << m_userTeam[userChoice - 1].getName() << " is fainted!" << endl;
				continue;
			}
			if (m_userTeam + userChoice - 1 == m_activeUserPokemon)
			{
				cout << "That pokemon is already on the field!" << endl;
				continue;
			}
			if (!fainted)
			{
				cout << "You withdrew your " << m_activeUserPokemon->getName() << endl;
				m_activeUserPokemon->resetStatChanges();
				m_activeUserPokemon->recalculateStats();
			}
			m_activeUserPokemon = &m_userTeam[userChoice - 1];
			cout << "You sent out " << m_activeUserPokemon->getName() << endl;
			if (m_activeUserPokemon->m_ability == Intimidate)
			{
				m_activeAIPokemon->m_statModifications[0] --;
				m_activeAIPokemon->recalculateStats();
				cout << m_activeUserPokemon->m_name << "'s intimidate lowered " << m_activeAIPokemon->m_name << "'s attack!" << endl;
			}
			break;
		}
	}
	else
	{
		m_activeAIPokemon->resetStatChanges();
		m_activeAIPokemon->recalculateStats();
		if (!fainted)
		{
			cout << "The AI withdrew its " << m_activeAIPokemon->m_name << endl;
		}
		int bestSwitchID = 0;

		// Finds an unfainted pokemon that can best take hits from the user's pokemon
		while (m_AITeam[bestSwitchID].m_pokemonStatus == Fainted)
		{
			bestSwitchID ++;
		}
		for (int i = bestSwitchID + 1; i < 3; i ++)
		{
			if (m_AITeam[i].getPokemonStatus() != Fainted)
			{
				for (int i = 0; i < 4; i ++)
				{
					if (m_activeUserPokemon->isAttackSTAB(m_activeUserPokemon->m_moveSet[i]))
					{
						if (m_AITeam[i].checkEffective(m_activeUserPokemon->m_moveSet[i]) < m_AITeam[bestSwitchID].checkEffective(m_activeUserPokemon->m_moveSet[i]))
						{
							bestSwitchID = i;
						}
					}
				}
			}
		}
		m_activeAIPokemon = &m_AITeam[bestSwitchID];
		cout << "The AI sends out " << m_activeAIPokemon->m_name << endl;
		if (m_activeAIPokemon->m_ability == Intimidate)
		{
			m_activeUserPokemon->m_statModifications[0] --;
			m_activeUserPokemon->recalculateStats();
		}
	}
}


Pokemon* Battle::getActiveUserPokemon()
{
	return m_activeUserPokemon;
}


Pokemon* Battle::getActiveAIPokemon()
{
	return m_activeAIPokemon;
}

Pokemon* Battle::getAITeam()
{
	return m_AITeam;
}
