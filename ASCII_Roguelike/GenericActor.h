#pragma once
// A simple actor.
/*
	Pretty much a sample for the enemies to build of
	off
*/
#include <random>
#include <functional>
class Map;
class GenericActor
{
public:
	GenericActor(int hp, int defense, bool killable, int xpGain, int x, int y);
	bool isKillable(){ return _Killable; }
	void TakeDamage(int damage) { _hp -= damage - _def % 2; };
	bool processMove(Map map, int targetX, int targetY);
	void MoveX(Map &map, int targetX);
	void MoveY(Map &map, int targetY);
	bool isDead(Map &map);
	void AI_Loop(Map &map);
	// Do not use this function 
	int GetX() { return _x; }
	int GetY() { return _y; }
private:
	int _hp, _def, _xpGain;
	bool _Killable;
	int _x, _y;
};

