#ifndef PLAYER_H
#define PLAYER_H
#include "common.h"
#include <iostream>
class Map;
class Door;
class Player
{
public:
	Player();
	Player(int defense, int health, int lockpick); // Inital
	void setPosition(int x, int y);
	void ProcessInput(char in, std::vector<Door> doors , Map &map);
	bool ProcessMove(Map &map, std::vector<Door>doors, int targetX, int targetY);
	void LevelUp()
	{
		if (_xp == _xpReq)
		{
			_def += 5;
			_hp += 15;
			_lockPick += 2;
			_xpReq += 10;
			_level++;
			std::cout << "Leveled Up!" << std::endl;
		}
		std::cout << "You are not ready young padawan" << std::endl;
	}
	void AddXP(int val)
	{
		_xp += val;
	}
	inline int GetXpTilNextLevel() { return _xpReq; }
	inline int GetHealth() { return _hp; }
	inline int GetSkill() { return _lockPick; }
	inline int GetLevel() { return _level; }
	void TakeDamage(int val) { _hp -= val - _def; }
		inline int GetDefense() {
		return _def;
	}
private:
	int _x, _y;
	int _xpReq = 40;
	int _def, _hp;
	int _xp;
	int _lockPick;
	int _level = 1;
};

#endif