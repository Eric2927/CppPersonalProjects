#ifndef GENERAL_H_
#define GENERAL_H_

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <initializer_list>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <array>
#include <map>
using namespace std;

// This represents the possible pokemon typings (I have left out Fairy, Steel, and Dark for simplicity)
enum Type {Normal, Fire, Water, Electric, Grass, Ice, Fighting, Poison, Ground,
	Flying, Psychic, Bug, Rock, Ghost, Dragon, None};

// This represents the possible pokemon statuses
enum PokemonStatus {Paralysis, Burn, Poisoned, Sleep, Freeze, Confused, Healthy, Fainted, Seeded};

// This represents the types of moves
enum MoveType {Physical, Special, Status, Undefined};

// This represents the abilities that pokemon may have
enum Ability {Blaze, Torrent, Overgrow, Static, Sturdy, Keen_Eye, Intimidate,
	Levitate, Synchronise, Multiscale, Water_Absorb, No_Guard, Tinted_Lens,
	Swarm, Thick_Fat, No_Ability};

// This represents all of the possible effects of moves other than damage
enum SecondaryEffect {FirstPriority, E_Burn, E_Paralyze, E_Poison, E_Sleep,
	E_Freeze, Recoil, Drain_Half, L_SpecialDefense1, Flinch, E_Confuse, F_Sun,
	Leech_Seed, Synthesis, Roost, F_Reflect, F_LightScreen, Rest, Suicide,
	HighCrit, R_Speed2, Facade, Dragon_Dance, Ice_Fang, Psyshock, Calm_Mind,
	Quiver_Dance, Brick_Break, Curse, Recover, Bulk_Up, No_Effect};

// This represents all of the possible items that the pokemon may be holding
enum Item {ExpertBelt, Leftovers, Choice_Band, Choice_Specs, Choice_Scarf, Light_Ball};

// Represents weather during battle (there is nothing implemented currently that would trigger rain or hail,
// but they are there so that I can add on to this game in the future, which I am planning to do)
enum Weather {Clear, Sun, Rain};

// Represents other things that the battle field has (currently not functioning properly unfortunately)
enum FieldStatus {Reflect, Light_Screen};

#endif /* GENERAL_H_ */
