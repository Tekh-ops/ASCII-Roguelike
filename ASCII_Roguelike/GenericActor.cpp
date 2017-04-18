#include "GenericActor.h"
#include "Map.h"
#include <time.h>

std::string dialogue[]
{
	"Do you have buisness with me?\n",
		"Excuse me!\n",
		"Hey!\n",
		"Hello sir! :)\n",
		"Hello adventurer!\n",
		"This town is quite boring...\n",
		"Hmmm.... Hmm...\n",
		"I think it's pretty dumb we don't have a shop.\n",
		"....\n",
		"This dungeon nearby is quite dangerous\n"
};

GenericActor::GenericActor(int hp, int defense, bool killable, int xpGain, int x, int y, int id, std::string name) :
_hp(hp), _def(defense), _Killable(killable), _xpGain(xpGain)
, _x(x), _y(y), _id(id), _name(name)
{
}

const char* GenericActor::getResponse()
{
	std::mt19937 mt_rand(time(0)+1);
	auto diceRoll = std::bind(std::uniform_int_distribution<int>(0, 7), std::mt19937(mt_rand));

	return dialogue[diceRoll()].c_str();
}

const char* GenericActor::getName()
{
	return _name.c_str();
}

bool GenericActor::MoveX(Map &map, int targetX)
{
	if (processMove(map, targetX, _y))
	{
		map.SetTile('.', _x, _y);
		map.SetTile('T', targetX, _y);
		_x = targetX;
		return true;
	}
	else
	{
		return false;
	}
}
bool GenericActor::MoveY(Map &map, int targetY)
{
	if (processMove(map, _x, targetY))
	{
		map.SetTile('.', _x, _y);
		map.SetTile('T', _x, targetY);
		_y = targetY;
		return true;
	}
	else
	{
		return false;
	}
}

bool GenericActor::processMove(Map map, int targetX, int targetY)
{
	if (map.GetTile(targetX, targetY) == '#')
		return false;
	if (map.GetTile(targetX, targetY) == '@')
		return false;
	if (map.GetTile(targetX, targetY) == '|' || map.GetTile(targetX, targetY) == '-' || map.GetTile(targetX, targetY) == 'X' || map.GetTile(targetX,targetY) == '0')
		return false;
	if (map.GetTile(targetX, targetY) == '.')
		return true;
	// Try not to commit genicide please
	if (map.GetTile(targetX, targetY) == 'T')
		return false;
	if (map.GetTile(targetX, targetY) == '<'){
		TakeDamage(20);
		return false;
}
		if (map.GetTile(targetX, targetY) == '>'){
			TakeDamage(20);
			return false;
		}
}

void GenericActor::AI_Loop(Map &map)
{
	std::mt19937 mt_rand(time(0) + (_id*2+_id));
	auto diceRoll = std::bind(std::uniform_int_distribution<int>(0, 6), std::mt19937(mt_rand));
	if (diceRoll() == 1){

		if	(!MoveX(map, 1 + _x))
			MoveX(map, -1 + _x);
}
	if (diceRoll() == 2){
		if (!MoveY(map, 1 + _y))
			MoveY(map, -1 + _y);
}
	if (diceRoll() == 4){
		if (!MoveX(map, -1 + _x))
			MoveX(map, 1 + _x);
}
	if (diceRoll() == 3){
		if (!MoveY(map, -1 + _y))
			MoveY(map, 1 + _y);
	}
}

bool GenericActor::isDead(Map &map)
{
	if (_hp == 0)
	{
		//Just finish him
		map.SetTile('.', _x, _y);
		printf("An actor has died.\n");
		return true;
	}
	return false;
}