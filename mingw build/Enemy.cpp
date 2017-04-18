#include "Enemy.h"
#include "Map.h"
#include "GenericActor.h"
#include "Player.h"
#include <time.h>
// I've been using nothing but init lists for this.
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
	//Very similar implementation to both Player and Actor
	if (map.GetTile(targetX, targetY) == '#')
		return false;
	// No player either
	if (map.GetTile(targetX, targetY) == '@'){
		return false;
	}
	// No doors
	if (map.GetTile(targetX, targetY) == '|' || map.GetTile(targetX, targetY) == '-' || map.GetTile(targetX, targetY) == 'X')
		return false;
	if (map.GetTile(targetX, targetY) == '.')
		return true;
	// As far as I know, an enemy will never go into contact with another actor
	// But just incase
	if (map.GetTile(targetX, targetY) == 'T'){
		for (int i = 0; i < actor.size(); i++)
		{
			if (actor[i].GetX() == _x && actor[i].GetY() == _y)
				actor[i].TakeDamage(_attack); // Deal flat dmg
		}
		return false;
	}
	// No genecide. Not that they will be marked dead.
	if (map.GetTile(targetX, targetY) == _tile)
		return false;
	// Spikes 
	if (map.GetTile(targetX, targetY) == '<' || map.GetTile(targetX,targetY) == '>'){
		TakeDamage(20);
		return false;
	}

		return false;
}

/*
Give Experience to player and check if I'm dead.
*/
bool Enemy::isDead(Map&map, Player &player)
{
	if (_hp <= 0){
		map.SetTile('.', _x, _y); // Effectively remove the enemy from the map
		player.AddXP(_xpGain); // Give XP
		std::cout << "You gained " << _xpGain << std::endl;
		return true;
	}

		return false;
}

/*
	Engage Battle with a player
	called as a result of ProcessMove
*/
bool Enemy::EngageBattle(Player &player, int targetX, int targetY)
{
	// Check if both axises are the same for both player and destined move
	if (targetX == _x && targetY == _y)
	{
	/*Get the hit chances using rng*/
	std::mt19937 mt_rant_p(time(0) % player.GetSkill());
	std::mt19937 mt_rand(time(0) % _skill);
	/*Damage dice rolls*/
	auto diceRoll = std::bind(std::uniform_int_distribution<int>(1, 3), std::mt19937(mt_rand));
	auto p_diceRoll = std::bind(std::uniform_int_distribution<int>(0, 6), std::mt19937(mt_rant_p));
	int prevHealth = _hp; // Use to print the difference in health
	int prevPlayerHP = player.GetHealth();
	std::mt19937 mt_ran(time(0) % _skill + 50);
	std::mt19937 mt_rane(time(0) % player.GetSkill() % 25);
	auto diceRoll_A = std::bind(std::uniform_int_distribution<int>(1, _attack), std::mt19937(mt_ran));
	auto diceRoll_pA = std::bind(std::uniform_int_distribution<int>(1, player.GetAttack()), std::mt19937(mt_rane));
	if (diceRoll() > p_diceRoll())
	{
		// You get hit
		player.TakeDamage(diceRoll_A());
		std::cout << _name << " has dealt " << prevPlayerHP - player.GetHealth() << " to you. (You missed)" << std::endl;
		prevPlayerHP = player.GetHealth();
	}
	else if (p_diceRoll() > diceRoll())
	{
		// You hit 
		TakeDamage(diceRoll_pA());
		std::cout << "You do " << player.GetAttack() << " to " << _name << std::endl;
		
		prevHealth = _hp;
	}
	else if (p_diceRoll() == diceRoll())
	{
		// Very hard condition to reach for whatever reason.
		TakeDamage(diceRoll_pA());
		player.TakeDamage(diceRoll_A());
		std::cout << "Both you and the enemy hit each other";
	}
	return true;
}
	return false;
}

void Enemy::AI_Loop(Map &map, Player &player, std::vector<GenericActor>&actors, char in)
{
	/*Get the absolute distance values for detection*/
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
			// Move based on player movement
			if (in == 'd' || in == 'D')
				MoveX(map, player, actors, _x + 1);
			if (in == 'a' || in == 'A')
				MoveX(map, player, actors, _x - 1);
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