#include "Enemy.h"
#include "Map.h"
#include "GenericActor.h"
#include "Player.h"
#include <time.h>
Enemy::Enemy(int hp, int defense, int xpGain, int x, int y, int id, std::string name, char tile, int attack, int skill) :_hp(hp), _def(defense), _xpGain(xpGain)
	, _x(x), _y(y), _id(id), _name(name), _tile(tile), _attack(attack)
	, _skill(skill)
{
}

const char* Enemy::getName()
{
	return _name.c_str();
}

bool Enemy::MoveX(Map &map, Player&player, std::vector<GenericActor> &actor, int targetX)
{
	if (processMove(map,player,actor,targetX, _y))
	{
		map.SetTile('.', _x, _y);
		map.SetTile(_tile, targetX, _y);
		_x = targetX;
		return true;
	}
	else
	{
		return false;
	}
}
bool Enemy::MoveY(Map &map, Player&player, std::vector<GenericActor> &actor, int targetY)
{
	if (processMove(map, player ,actor,_x, targetY))
	{
		map.SetTile('.', _x, _y);
		map.SetTile(_tile, _x, targetY);
		_y = targetY;
		return true;
	}
	else
	{
		return false;
	}
}

bool Enemy::processMove(Map map, Player&player, std::vector<GenericActor> &actor, int targetX, int targetY)
{
	if (map.GetTile(targetX, targetY) == '#')
		return false;
	if (map.GetTile(targetX, targetY) == '@'){
		return false;
	}
	if (map.GetTile(targetX, targetY) == '|' || map.GetTile(targetX, targetY) == '-' || map.GetTile(targetX, targetY) == 'X')
		return false;
	if (map.GetTile(targetX, targetY) == '.')
		return true;
	if (map.GetTile(targetX, targetY) == 'T'){
		for (int i = 0; i < actor.size(); i++)
		{
			if (actor[i].GetX() == _x && actor[i].GetY() == _y)
				actor[i].TakeDamage(_attack); // Deal flat dmg
		}
		return false;
	}
	if (map.GetTile(targetX, targetY) == _tile)
		return false;
	if (map.GetTile(targetX, targetY) == '<'){
		TakeDamage(20);
		return false;
	}

		return false;
}

bool Enemy::isDead(Map&map, Player &player)
{
	if (_hp <= 0){
		map.SetTile('.', _x, _y);
		player.AddXP(_xpGain);
		std::cout << "you gained " << _xpGain << "XP.\n";
		return true;
	}

		return false;
}

bool Enemy::EngageBattle(Player &player, int targetX, int targetY)
{
	if (targetX == _x && targetY == _y)
	{
	std::mt19937 mt_rant_p(time(0) % player.GetSkill());
	std::mt19937 mt_rand(time(0) % _skill);
	auto diceRoll = std::bind(std::uniform_int_distribution<int>(1, 3), std::mt19937(mt_rand));
	auto p_diceRoll = std::bind(std::uniform_int_distribution<int>(0, 6), std::mt19937(mt_rant_p));
	if (diceRoll() > p_diceRoll())
	{
		player.TakeDamage(_attack);
		std::cout << _name << " has dealt " << _attack - player.GetDefense() % 2<< " to you. (You missed)" << std::endl;
	}
	else if (p_diceRoll() > diceRoll())
	{
		TakeDamage(player.GetAttack());
		std::cout << "You do " << player.GetAttack() - _def % 2 << " damage to " << _name << "\n";
		std::cout << _name << " has " << _hp << " health left\n";
	}
	else if (p_diceRoll() == diceRoll())
	{
		TakeDamage(player.GetAttack());
		player.TakeDamage(_attack);
		std::cout << "Both you and the " << _name << " Get hit by each other.\n";
	}
	return true;
}
	return false;
}

void Enemy::AI_Loop(Map &map, Player &player, std::vector<GenericActor>&actors, char in)
{
	int distX = abs(player.GetX() - _x);
	int distY = abs(player.GetY() - _y);
	bool aggroed = false;
	// Detects within 6 blocks.
	if (distX <= 6 && distY <= 6){
		aggroed = true;
	}
	else
	{
		aggroed = false;
	}
	if (aggroed == true){
		if (_x != player.GetX())
		{
			if (in == 'd' || in == 'D')
				MoveX(map, player, actors, _x - 1);
			if (in == 'a' || in == 'A')
				MoveX(map, player, actors, _x + 1);
			if (_y != player.GetY()){
				if (in == 'w' || in == 'W')
					MoveY(map, player, actors, _y - 1);
				if (in == 's' || in == 'S')
					MoveY(map, player, actors, _y + 1);
			}
		}
}
	else if (aggroed == false)
	{
		// Just walk randomly
		std::mt19937 mt_rand(time(0) + (_id * 2 + _id));
		auto diceRoll = std::bind(std::uniform_int_distribution<int>(0, 6), std::mt19937(mt_rand));
		if (diceRoll() == 1){

			if (!MoveX(map,player,actors,1 + _x))
				MoveX(map,player,actors,-1 + _x);
		}
		if (diceRoll() == 2){
			if (!MoveY(map,player,actors,1 + _y))
				MoveY(map,player,actors,-1 + _y);
		}
		if (diceRoll() == 4){
			if (!MoveX(map,player,actors,-1 + _x))
				MoveX(map,player,actors,1 + _x);
		}
		if (diceRoll() == 3){
			if (!MoveY(map,player,actors,-1 + _y))
				MoveY(map,player,actors, 1 + _y);
		}
	}
}