#ifndef PLAYER_H
#define PLAYER_H
#include "common.h"
#include <iostream>
#include "GenericActor.h"
class Map;
class Door;
class Player
{
public:
	Player();
	Player(int defense, int health, int lockpick, int attack); // Inital
	void setPosition(int x, int y);
	void ProcessInput(char in, std::vector<Door> &doors, std::vector<GenericActor> actors , Map &map, int &lvl);
	bool ProcessMove(Map &map, std::vector<Door> &doors, std::vector<GenericActor> actors, int targetX, int targetY, int&lvl);
	bool LevelUp()
	{
		if (_xp == _xpReq)
		{
			_def += 5;
			_maxHP += 30;
			_hp = _maxHP;
			_lockPick += 2;
			_xp = 0;
			_xpReq += 20;
			_level++;
			std::cout << "Leveled Up!" << std::endl;
			return true;
		}
		return false;
	}
	void AddXP(int val)
	{
		_xp += val;
	}
	inline int GetXpTilNextLevel() { return _xpReq; }
	inline int GetHealth() { return _hp; }
	inline int GetMaxHP() { return _maxHP;  }
	inline int GetSkill() { return _lockPick; }
	inline int GetLevel() { return _level; }
	inline int GetAttack() { return _attack;  }
	inline int GetXP() { return _xp; }
	void TakeDamage(int val) { _hp -= val - _def % 2; }
		inline int GetDefense() {
		return _def;
	}
private:
	int _x, _y;
	int _xpReq = 40;
	int _def, _hp, _maxHP;
	int _xp;
	int _attack;
	int _lockPick;
	int _level = 1;
};

#endif