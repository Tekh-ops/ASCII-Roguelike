#pragma once
#include <string>
#include <random>
#include <vector>
#include <functional>
// I could've and should've used
// Inheritance to make this class
// Since it is going to be similar to the actor
// I'll do that later though
class Map;
class Player;
class GenericActor;
class Enemy
{
public:
	Enemy(int hp, int defense, int xpGain, int x, int y, int id, std::string name, char tile, int attack, int skill);
	void TakeDamage(int damage) { _hp -= damage - _def % 2; }
	bool processMove(Map map, Player&player, std::vector<GenericActor> &actor, int targetX, int targetY);
	bool MoveX(Map &map, Player&player, std::vector<GenericActor> &actor, int targetX);
	bool MoveY(Map &map, Player&player, std::vector<GenericActor> &actor, int targetY);
	const char* getName();
	int GetX() { return _x; }
	int GetY() { return _y; }
	bool isDead(Map &map, Player &player);
	bool EngageBattle(Player & player, int targetX, int targetY);
	void AI_Loop(Map &map,Player &player, std::vector<GenericActor> &actors, char in);
private:
	int _hp, _def, _xpGain;
	int _attack;
	std::string _name;
	char _tile;
	int _skill;
	int _x, _y;
	int _id; // RNG Purposes
};

