#include "Pokemon.h"

Pokemon::Pokemon(int ID, bool customize)
{
	if (ID == 0)			// Default constructor (basically initializes everything to an invalid value)
	{
		m_name = "";
		m_primaryType = None;
		m_secondaryType = None;
		m_ability = No_Ability;
		m_HP = -1;
		m_Attack = -1;
		m_Defense = -1;
		m_SpecialAttack = -1;
		m_SpecialDefense = -1;
		m_Speed = -1;
		m_pokemonStatus = Healthy;
		m_level = -1;
	}
	else
	{
		ifstream inPokemon("Pokemon.txt");		// Reads in the file Pokemon.txt
		int pokemonID = 0;
		// This loop looks for the Pokemon with the matching "ID" (pokemon are numbered consecutively)
		while (!inPokemon.eof())
		{
			// Everytime a # is detected at the beginning of a line, it recognizes it as the next pokemon
			if (inPokemon.peek() == 35)
			{
				pokemonID ++;
			}
			// If the user input matches the ID of the current Pokemon read in the file
			if (pokemonID == ID)
			{
				inPokemon.ignore(1);		// Ignores the "#" at the beginning of the line
				getline(inPokemon, m_name);			// Reads in the next part of the line as the name of the pokemon
				int tempInt;			// An integer to dump all of the integers read from the file into

				// Reads in the next two integers as the typings
				inPokemon >> tempInt;
				m_primaryType = (Type) tempInt;
				inPokemon >> tempInt;
				m_secondaryType = (Type) tempInt;

				// Reads the next numbers in as move IDs until the "@"
				while (inPokemon.peek() != 64)
				{
					inPokemon >> tempInt;
					Move myMove(tempInt);
					m_learnableMoves.push_back(myMove);
					inPokemon.ignore(1);
				}
				inPokemon.ignore(1);				// Ignores the "@"
				bool moveSetCustomized = false;			// Boolean to represent if the moveset was customized by user
				if (customize)			// If user chose to customize the pokemon
				{
					cout << "Would you like to customize " << m_name << "'s moveset? "
							"A default moveset will be used if you choose not to." << endl;
					string userResponse;
					getline(cin, userResponse);
					if (userResponse.compare("y") == 0)			// If they chose to customize the moveset
					{
						cout << m_name << "'s Learnable Moves:" << endl;
						for (int i = 0; i < m_learnableMoves.size(); i ++)			// Lists all learnable moves
						{
							cout << i + 1 << ". " << (m_learnableMoves.at(i)).m_moveName << endl;
						}
						for (int i = 0; i < 4; i ++)			// Populates the moveset with 4 moves the user chooses from the list
						{
							while (true)
							{
								cout << "Move #" << i + 1 << ":" << endl;
								int userChoice;
								cin >> userChoice;
								cin.ignore();
								if (userChoice > m_learnableMoves.size() || userChoice <= 0)
								{
									cout << "You did not choose a valid move." << endl;
									continue;
								}
								m_moveSet[i] = m_learnableMoves.at(userChoice - 1);
								break;
							}
						}
						moveSetCustomized = true;			// moveset is customized
					}
				}
				if (!moveSetCustomized)		// if moveset is not customized, the default moveset is used (aka first 4 moves)
				{
					for (int i = 0; i < 4; i ++)
					{
						m_moveSet[i] = m_learnableMoves.at(i);
					}
				}
				bool evSpreadCustomized = false;			// Bool to represent if EV spread is customized
				// Stores the EVs of every stat (temporary, used to calculate real stats)
				int hpEVs;
				int attackEVs;
				int defenseEVs;
				int specialAttackEVs;
				int specialDefenseEVs;
				int speedEVs;
				if (customize)				// if user wanted to customize
				{
					// Asks if user wants to customize EV spread
					string userResponse;
					cout << "Would you like to customize " << m_name << "'s EV spread? "
							"A default spread will be used if you choose not to." << endl;
					getline(cin, userResponse);
					if (userResponse.compare("y") == 0)
					{
						while (true)
						{
							int totalEVs = 508;			// Max number of EVs
							cout << "You have a total of 508 EVs. Every stat's EVs max out at 252." << endl;
							// prompts user for EVs of every stat
							// read function implementation for details
							hpEVs = promptUserForEV("HP", totalEVs);
							attackEVs = promptUserForEV("Attack", totalEVs);
							defenseEVs = promptUserForEV("Defense", totalEVs);
							specialAttackEVs = promptUserForEV("Special Attack", totalEVs);
							specialDefenseEVs = promptUserForEV("Special Defense", totalEVs);
							speedEVs = promptUserForEV("Speed", totalEVs);
							if (totalEVs > 0)
							{
								cout << "You did not allocate all 508 EVs. ";	// Warns user that he/she didn't allocate all available EVs
							}
							string userInput;
							cout << "Are you sure you'd like to proceed with this spread?" << endl;
							getline(cin, userInput);
							if (userInput.compare("n") == 0)
							{
								continue;
							}
							break;
						}
						evSpreadCustomized = true;
					}
				}
				// Reads in the base stats of the pokemon
				// Reads in the default EVs if ev spread is not customized
				inPokemon >> m_HP;
				(!evSpreadCustomized) ? inPokemon >> hpEVs : inPokemon.ignore(10, '\n');
				inPokemon >> m_Attack;
				(!evSpreadCustomized) ? inPokemon >> attackEVs : inPokemon.ignore(10, '\n');
				inPokemon >> m_Defense;
				(!evSpreadCustomized) ? inPokemon >> defenseEVs : inPokemon.ignore(10, '\n');
				inPokemon >> m_SpecialAttack;
				(!evSpreadCustomized) ? inPokemon >> specialAttackEVs : inPokemon.ignore(10, '\n');
				inPokemon >> m_SpecialDefense;
				(!evSpreadCustomized) ? inPokemon >> specialDefenseEVs : inPokemon.ignore(10, '\n');
				inPokemon >> m_Speed;
				(!evSpreadCustomized) ? inPokemon >> speedEVs : inPokemon.ignore(10, '\n');
				inPokemon >> m_level;
				// Uses the base stats and EVs to calculate the actual stat used in battle
				// Read processStat() function implementation for details
				m_HP = processStat(m_HP, hpEVs, m_level, true);
				m_Attack = processStat(m_Attack, attackEVs, m_level);
				m_Defense = processStat(m_Defense, defenseEVs, m_level);
				m_SpecialAttack = processStat(m_SpecialAttack, specialAttackEVs, m_level);
				m_SpecialDefense = processStat(m_SpecialDefense, specialDefenseEVs, m_level);
				m_Speed = processStat(m_Speed, speedEVs, m_level);
				// Sets the original stats to the original stats
				m_originalStats[0] = m_HP;
				m_originalStats[1] = m_Attack;
				m_originalStats[2] = m_Defense;
				m_originalStats[3] = m_SpecialAttack;
				m_originalStats[4] = m_SpecialDefense;
				m_originalStats[5] = m_Speed;
				// Sets ability of pokemon (ID is parallel with the enum declaration)
				m_ability = (Ability) (ID - 1);
				m_pokemonStatus = Healthy;
				// Makes all stat modifications 0
				for (int i = 0; i < 5; i ++)
				{
					m_statModifications[i] = 0;
				}
				break;		// Stops looping through file (pokemon already found and all members initialized)
			}
			inPokemon.ignore(256, '\n');		// Ignores the line if pokemon ID doesn't match user input
		}
		inPokemon.close();		// Closes the file
	}
}


void Pokemon::takeMove(Pokemon* opponent, Move &move, Weather weather, vector<FieldStatus> fieldStatus)
{
	if (rand() % 100 < move.m_accuracy || opponent->m_ability == No_Guard)		// Checks if move hits
	{
		int damage = 0;

		// Calculates damage based on move type (physical vs special)
		switch (move.m_moveType)
		{
		case Physical:
			damage = ((2 * opponent->m_level / 5 + 2) * move.m_rawDamage * (opponent->m_Attack / m_Defense)) / 50 + 2;
			for (FieldStatus fieldState : fieldStatus)
			{
				if (fieldState == Reflect)
				{
					damage *= 0.5;
				}
			}
			break;
		case Special:
			if (move.m_effect == Psyshock)
			{
				damage = ((2 * opponent->m_level / 5 + 2) * move.m_rawDamage * (opponent->m_SpecialAttack / m_Defense)) / 50 + 2;
			}
			else
			{
				damage = ((2 * opponent->m_level / 5 + 2) * move.m_rawDamage * (opponent->m_SpecialAttack / m_SpecialDefense)) / 50 + 2;
			}
			for (FieldStatus fieldState : fieldStatus)
			{
				if (fieldState == Light_Screen)
				{
					damage *= 0.5;
				}
			}
			break;
		}

		// Checks if move is stab
		if (opponent->m_primaryType == move.m_type || opponent->m_secondaryType == move.m_type)
		{
			// If move is stab, damage is multiplied by 1.5
			damage *= 1.5;
		}
		double typeMultiplier = checkEffective(move);		// Gets damage multiplier based on type

		// Displays a message depending on how effective the move was on the pokemon
		if (typeMultiplier == 4 && move.m_moveType != Status)
		{
			cout << "It was incredibly effective!" << endl;
		}
		else if (typeMultiplier == 2 && move.m_moveType != Status)
		{
			cout << "It was super effective!" << endl;
		}
		else if (typeMultiplier == 0.5 && move.m_moveType != Status)
		{
			cout << "It was not very effective!" << endl;
		}
		else if (typeMultiplier == 0 && move.m_moveType != Status)
		{
			cout << "It had no effect!" << endl;
		}

		// Damage is multiplied by the type multiplier
		damage *= typeMultiplier;

		// Affects damage of certain moves based on weather
		switch (weather)
		{
		case Sun:
			if (move.m_type == Fire)
			{
				damage *= 1.5;
			}
			else if (move.m_type == Water)
			{
				damage *= 0.5;
			}
			break;
		case Rain:
			if (move.m_type == Water)
			{
				damage *= 1.5;
			}
			else if (move.m_type == Fire)
			{
				damage *= 0.5;
			}
			break;
		}
		// Implements abilities that affect damage
		switch (opponent->m_ability)
		{
		// Blaze boosts damage of fire type moves if the health of the move user is lower than 1/3 of the full health
		case Blaze:
			if (opponent->m_HP < opponent->m_originalStats[0] * 0.33 && move.m_type == Fire)
			{
				damage *= 1.5;
			}
			break;
		// Torrent does the same thing as blaze, except for water type moves
		case Torrent:
			if (opponent->m_HP < opponent->m_originalStats[0] * 0.33 && move.m_type == Water)
			{
				damage *= 1.5;
			}
			break;
		// Overgrow does the same thing as blaze, except for grass type moves
		case Overgrow:
			if (opponent->m_HP < opponent->m_originalStats[0] * 0.33 && move.m_type == Grass)
			{
				damage *= 1.5;
			}
			break;
		// Tinted lens increases the damage of not very effective moves
		case Tinted_Lens:
			if (typeMultiplier < 1)
			{
				damage *= 2;
			}
			break;
		// Swarm does the same thing as blaze, except for bug type moves
		case Swarm:
			if (opponent->m_HP < opponent->m_originalStats[0] * 0.33 && move.m_type == Bug)
			{
				damage *= 1.5;
			}
			break;
		}
		switch (m_ability)
		{
		// Levitate makes the pokemon with it immune to ground type moves
		case Levitate:
			if (move.m_type == Ground)
			{
				cout << m_name << " avoided the attack with levitate!" << endl;
				damage *= 0;
			}
			break;
		// Multiscale reduces the damage of an attacking move on the pokemon if the pokemon is full health
		case Multiscale:
			if (m_HP == m_originalStats[0] && move.m_moveType != Status)
			{
				cout << m_name << "'s multiscale reduced the damage taken by the attack!" << endl;
				damage *= 0.5;
			}
			break;
		// Water Absorb makes the pokemon that has it immune to water type moves.
		// Water moves used on this pokemon will restore its HP by 25% of its full HP
		case Water_Absorb:
			if (move.m_type == Water)
			{
				cout << m_name << "'s water absorb recovered its HP!" << endl;
				damage *= 0;
				m_HP += m_originalStats[0] * 0.25;
			}
			break;
		// Thick Fat reduces the damage of fire and ice type moves used on this pokemon
		case Thick_Fat:
			if (move.m_type == Fire || move.m_type == Ice)
			{
				damage *= 0.5;
			}
			break;
		// Static has a 30% chance to paralyze the opponent when they use a physical move
		case Static:
			if (rand() % 100 < 30 && opponent->m_pokemonStatus == Healthy && move.m_moveType == Physical)
			{
				cout << m_name << "'s static paralyzed " << opponent->m_name << endl;
				opponent->m_pokemonStatus = Paralysis;
				opponent->m_Speed *= 0.5;
			}
			break;
		}
		// Based on crit rate of move, calculates if move is a critical hit
		if (rand() % 100 < move.m_criticalRatio && move.m_moveType != Status)
		{
			// If it is was a critical hit, the damage is multiplied
			cout << "It was a critical hit!" << endl;
			damage *= 1.5;
		}
		damage *= (double)((rand() % 16) + 85) / 100;		// adds a random factor to every move's damage
		if (rand() % 100 < move.m_secondaryChance)		// If the move has a secondary effect and it hits
		{
			applySpecialEffect(move, damage, opponent, weather);	// Applies secondary effect
		}

		// Sturdy is an ability that allows the pokemon to survive
		// a one-hit move at full HP
		if (m_HP == m_originalStats[0] && damage >= m_HP && m_ability == Sturdy)
		{
			cout << m_name << " hang on with sturdy!" << endl;
			m_HP = 1;
		}
		else
		{
			// Reduces health by damage done by move
			m_HP -= damage;
			if (m_HP < 0)
			{
				m_HP = 0;
				m_pokemonStatus = Fainted;
			}
		}
		move.m_powerPoint --;
	}
	else
	{
		cout << m_name << " avoided the attack!" << endl;
	}
}

double Pokemon::checkEffective(Move move)
{
	double damageMultiplier = 1;
	ifstream inTypeChart("typechart.txt");		// Reads the text file with typing chart
	vector<Type> superEffective;		// Stores super effective types on the pokemon
	vector<Type> notEffective;			// Stores not effective types on the pokemon
	vector<Type> noEffect;			// Stores types that have no effect on the pokemon
	int lineType = 0;			// Stores the ID of the type represented by the line being read
	while (lineType < 15)		// Loops through every type
	{
		if (m_primaryType == lineType || m_secondaryType == lineType)
		{
			inTypeChart.ignore(1);
			inTypeChart >> noskipws;
			int effectiveness = 0;
			while (inTypeChart.peek() != 64)	// Keeps looping until delimiter @
												// Check file for more info on the format
			{
				if (inTypeChart.peek() == 124)
				{
					effectiveness ++;
					inTypeChart.ignore(2);
				}
				else
				{
					int tempInt;
					inTypeChart >> tempInt;
					// Stores the typings in the respective vectors
					switch (effectiveness)
					{
					case 0:
						superEffective.push_back((Type) tempInt);
						break;
					case 1:
						notEffective.push_back((Type) tempInt);
						break;
					case 2:
						noEffect.push_back((Type) tempInt);
						break;
					}
					inTypeChart.ignore(1);
				}
			}
		}
		inTypeChart.ignore(256, '\n');
		lineType ++;
	}

	// Finds the move type in the vectors and based on the effectiveness
	// of the move, damage multiplier is changed in a specific way
	for (Type effectiveType : superEffective)
	{
		if (move.m_type == effectiveType)
		{
			damageMultiplier *= 2;
		}
	}
	for (Type notEffectiveType : notEffective)
	{
		if (move.m_type == notEffectiveType)
		{
			damageMultiplier *= 0.5;
		}
	}
	for (Type noEffectType : noEffect)
	{
		if (move.m_type == noEffectType)
		{
			damageMultiplier = 0;
		}
	}
	inTypeChart.close();		// Closes file

	return damageMultiplier;
}

void Pokemon::checkStatus(bool &attack)
{
	// Checks the pokemon's status to see if it has anything that can prevent the pokemon from moving
	switch (m_pokemonStatus)
	{
	case Paralysis:
		if (rand() % 4 < 1)
		{
			cout << m_name << " is paralyzed. It can't move!" << endl;
			attack = false;
		}
		break;
	case Confused:
		if (rand() % 3 < 1)
		{
			cout << getName() << " snapped out of confusion!" << endl;
			m_pokemonStatus = Healthy;
		}
		else
		{
			cout << m_name << " is confused!" << endl;
			if (rand() % 3 < 1)
			{
				cout << "It hit itself in confusion!" << endl;
			}
			m_HP *= 0.9;
			attack = false;
		}
		break;
	case Freeze:
		if (rand() % 5 < 1)
		{
			cout << m_name << " thawed out!" << endl;
			m_pokemonStatus = Healthy;
		}
		else
		{
			cout << m_name << " is frozen solid!" << endl;
			attack = false;
		}
		break;
	case Sleep:
		if (rand() % 4 < 1)
		{
			cout << m_name << " woke up!" << endl;
			m_pokemonStatus = Healthy;
		}
		else
		{
			cout << m_name << " is fast asleep!" << endl;
			attack = false;
		}
	}
}


void Pokemon::checkSpecialEffect(Pokemon* opponent)
{
	// Makes the statuses that do damage passively happen
	switch (m_pokemonStatus)
	{
	case Burn:
		m_HP -= m_originalStats[0] * 0.9;
		m_Attack *= 0.5;
		cout << m_name << " was hurt by burn" << endl;
		break;
	case Poisoned:
		m_HP -= m_originalStats[0] * 0.18;
		cout << m_name << " was hurt by poison" << endl;
		break;
	}
	if (m_otherStatus == Seeded)
	{
		m_HP -= m_originalStats[0] * 0.12;
		opponent->m_HP += m_originalStats[0] * 0.12;
		if (opponent->m_HP > opponent->m_originalStats[0])
		{
			opponent->m_HP = opponent->m_originalStats[0];
		}
		cout << m_name << "'s health was sapped by leech seed!" << endl;
	}
}

void Pokemon::applySpecialEffect(Move move, int damage, Pokemon* opponent, Weather weather)
{
	/// Applies special effect of moves (should be self-explanatory based on code)
	switch (move.m_effect)
	{
	case E_Burn:
		if (m_primaryType == Fire || m_secondaryType == Fire)
		{
			cout << m_name << " is immune to burn!" << endl;
		}
		else
		{
			m_pokemonStatus = Burn;
			cout << m_name << " was burned!" << endl;
		}
		break;
	case E_Paralyze:
		m_pokemonStatus = Paralysis;
		cout << m_name << " was paralyzed!" << endl;
		m_Speed *= 0.5;
		break;
	case E_Poison:
		if (m_primaryType == Poison || m_secondaryType == Poison)
		{
			cout << m_name << " is immune to poison!" << endl;
		}
		else
		{
			m_pokemonStatus = Poisoned;
			cout << m_name << " was poisoned!" << endl;
		}
		break;
	case E_Sleep:
		m_pokemonStatus = Sleep;
		cout << m_name << " fell asleep!" << endl;
		break;
	case E_Freeze:
		m_pokemonStatus = Freeze;
		cout << m_name << " was frozen solid!" << endl;
		break;
	case Recoil:
		opponent->m_HP -= damage / 3;
		cout << opponent->m_name << " took recoil!" << endl;
		break;
	case Drain_Half:
		opponent->m_HP += damage / 2;
		cout << opponent->m_name << " drained " << m_name << "'s HP!" << endl;
		if (opponent->m_HP > opponent->m_originalStats[0])
		{
			opponent->m_HP = opponent->m_originalStats[0];
		}
		break;
	case L_SpecialDefense1:
		opponent->m_SpecialDefense -= (opponent->m_originalStats[3]) * 0.5;
		cout << m_name << "'s special defense fell!" << endl;
		break;
	case E_Confuse:
		opponent->m_pokemonStatus = Confused;
		cout << opponent->m_name << " became confused!" << endl;
		break;
	case Leech_Seed:
		m_otherStatus = Seeded;
		cout << m_name << " was seeded!" << endl;
		break;
	case Synthesis:
		if (weather == Sun)
		{
			opponent->m_HP += opponent->m_originalStats[0] * 0.75;
		}
		else if (weather == Rain)
		{
			opponent->m_HP += opponent->m_originalStats[0] * 0.25;
		}
		else
		{
			opponent->m_HP += opponent->m_originalStats[0] * 0.5;
		}
		if (opponent->m_HP > opponent->m_originalStats[0])
		{
			opponent->m_HP = opponent->m_originalStats[0];
		}
		break;
	case Roost:
		opponent->m_HP += opponent->m_originalStats[0] * 0.5;
		if (opponent->m_HP > opponent->m_originalStats[0])
		{
			opponent->m_HP = opponent->m_originalStats[0];
		}
		break;
	case Rest:
		opponent->m_HP = opponent->m_originalStats[0];
		opponent->m_pokemonStatus = Sleep;
		break;
	case Suicide:
		cout << opponent->m_name << " has fainted!" << endl;
		opponent->m_HP = 0;
		opponent->m_pokemonStatus = Fainted;
		break;
	case R_Speed2:
		cout << opponent->m_name << "'s speed rose sharply!" << endl;
		opponent->m_statModifications[4] += 2;
		opponent->recalculateStats();
		break;
	case Dragon_Dance:
		cout << opponent->m_name << "'s attack rose!" << endl;
		cout << opponent->m_name << "'s speed rose!" << endl;
		opponent->m_statModifications[0] += 1;
		opponent->m_statModifications[4] += 1;
		opponent->recalculateStats();
		break;
	case Calm_Mind:
		cout << opponent->m_name << "'s special attack rose!" << endl;
		cout << opponent->m_name << "'s special defense rose!" << endl;
		opponent->m_statModifications[2] += 1;
		opponent->m_statModifications[3] += 1;
		opponent->recalculateStats();
		break;
	case Quiver_Dance:
		cout << opponent->m_name << "'s special attack rose!" << endl;
		cout << opponent->m_name << "'s special defense rose!" << endl;
		cout << opponent->m_name << "'s speed rose!" << endl;
		opponent->m_statModifications[2] += 1;
		opponent->m_statModifications[3] += 1;
		opponent->m_statModifications[4] += 1;
		opponent->recalculateStats();
		break;
	case Curse:
		cout << opponent->m_name << "'s attack rose!" << endl;
		cout << opponent->m_name << "'s defense rose!" << endl;
		cout << opponent->m_name << "'s speed fell!" << endl;
		opponent->m_statModifications[0] += 1;
		opponent->m_statModifications[1] += 1;
		opponent->m_statModifications[4] -= 1;
		opponent->recalculateStats();
		break;
	case Recover:
		opponent->m_HP += opponent->m_originalStats[0] * 0.5;
		if (opponent->m_HP > opponent->m_originalStats[0])
		{
			opponent->m_HP = opponent->m_originalStats[0];
		}
		break;
	case Bulk_Up:
		cout << opponent->m_name << "'s attack rose!" << endl;
		cout << opponent->m_name << "'s defense rose!" << endl;
		opponent->m_statModifications[1] += 1;
		opponent->m_statModifications[2] += 1;
		opponent->recalculateStats();
		break;
	}
}


int Pokemon::processStat(int rawStat, int EV, int level, bool HP)
{
	// Calculation based on math of official pokemon games
	if (HP)
	{
		return (int)((2 * rawStat + 31 + (int)(EV / 4)) * level / 100) + level + 10;
	}
	return (int)((2 * rawStat + 31 + (int)(EV / 4)) * level / 100) + 5;
}

int Pokemon::promptUserForEV(string stat, int &EVsLeft)
{
	while (true)
	{
		cout << "You have " << EVsLeft << " left." << endl;
		cout << "Enter the number of EVs for " << m_name << "'s " << stat << " stat:" << endl;
		int EVs;
		cin >> EVs;
		cout << EVs << endl;
		cin.ignore();
		if (EVs > 252 || EVs > EVsLeft)
		{
			cout << "The number of EVs you alloted is too high." << endl;
			continue;
		}
		EVsLeft -= EVs;
		return EVs;
	}
}

void Pokemon::resetStatChanges()
{
	// Makes all stat modifications to 0
	for (int i = 0; i < 5; i ++)
	{
		m_statModifications[i] = 0;
	}
	recalculateStats();
}

string Pokemon::getName()
{
	return m_name;
}

Move* Pokemon::getMoves()
{
	return m_moveSet;
}

int Pokemon::getHP()
{
	return m_HP;
}

int Pokemon::getFullHP()
{
	return m_originalStats[0];
}

PokemonStatus Pokemon::getPokemonStatus()
{
	return m_pokemonStatus;
}

void Pokemon::recalculateStats()
{
	// Stat modifications work just like official pokemon games
	// Based on integer stages
	m_Attack = (m_statModifications[0] >= 1) ? (m_originalStats[1] * (2 + m_statModifications[0]) / 2) : (m_originalStats[1] * 2 / (2 - m_statModifications[0]));
	m_Defense = (m_statModifications[1] >= 1) ? (m_originalStats[2] * (2 + m_statModifications[1]) / 2) : (m_originalStats[2] * 2 / (2 - m_statModifications[1]));
	m_SpecialAttack = (m_statModifications[2] >= 1) ? (m_originalStats[3] * (2 + m_statModifications[2]) / 2) : (m_originalStats[3] * 2 / (2 - m_statModifications[2]));
	m_SpecialDefense = (m_statModifications[3] >= 1) ? (m_originalStats[4] * (2 + m_statModifications[3]) / 2) : (m_originalStats[4] * 2 / (2 - m_statModifications[3]));
	m_Speed = (m_statModifications[4] >= 1) ? (m_originalStats[5] * (2 + m_statModifications[4]) / 2) : (m_originalStats[5] * 2 / (2 - m_statModifications[4]));
}

int* Pokemon::getStatChanges()
{
	return m_statModifications;
}

void Pokemon::displayStats()
{
	cout << "Attack: " << m_Attack << endl;
	cout << "Defense: " << m_Defense << endl;
	cout << "Special Attack: " << m_SpecialAttack << endl;
	cout << "Special Defense: " << m_SpecialDefense << endl;
	cout << "Speed: " << m_Speed << endl;
}


bool Pokemon::isAttackSTAB(Move move)
{
	if ((move.m_type == m_primaryType || move.m_type == m_secondaryType) && move.m_moveType != Status)
	{
		return true;
	}
	return false;
}

PokemonStatus Pokemon::getOtherStatus()
{
	return m_otherStatus;
}
