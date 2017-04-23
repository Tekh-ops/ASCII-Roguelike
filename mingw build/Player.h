#ifndef PLAYER_H
#define PLAYER_H
#include "common.h"
#include <iostream>
#include <sstream>
#include "GenericActor.h"
#include <curses.h>
#include <SDL2/SDL_mixer.h>
class Map;
class Door;
class Enemy;

// Inventory will be 5 of these.

class Player
{
public:
	Player();
	Player(int defense, int health, int lockpick, int attack, int skill); // Inital
	std::string whoKilledMe(Enemy enemy, std::string str);
	void setPosition(int x, int y);
	void PrintMsg(char* d)
	{
		da = d;
	}
	void ProcessInput(char in, std::vector<Door> &doors, std::vector<GenericActor> &actors, std::vector<Enemy> &enemy, Map &map, int &lvl);
	bool ProcessMove(Map &map, std::vector<Door> &doors, std::vector<GenericActor> &actors, std::vector<Enemy> &enemy, int targetX, int targetY, int&lvl);
	void AddHealth(int value)
	{
		if (value > _maxHP)
		{
			value = _maxHP;
		}
		_hp += value;
	}
	void AddSkill(int value)
	{
		_skill += value;
	}
	bool LevelUp()
	{
		if (_xp >= _xpReq)
		{
			_def += 5;
			_maxHP += 10;
			_lockPick += 2;
			_xp = 0;
			_xpReq += 20;
			_skill += 2;
			_defaultDef += 5;
			_level++;
			attron(COLOR_PAIR(1));
			mvprintw(14,0,"Leveled Up!\n");
			attroff(COLOR_PAIR(1));
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
	inline int GetMaxHP() { return _maxHP; }
	inline int GetSkill() { return _lockPick; }
	inline int GetLevel() { return _level; }
	inline int GetAttack() { return _attack; }
	inline int GetDefAttack() { return _defaultAttack; }
	inline int GetDefDefense() { return _defaultDef; }
	inline int GetXP() { return _xp; }
	inline int GetX() { return _x; }
	inline int GetY() { return _y; }
	void TakeDamage(int val) { _hp -= val - _def % 2; }
	void ProcessUseItem(int slot);
	inline int GetDefense() {
		return _def;
	}
	struct InventorySlot
	{
		int id = -1; // Item ID
		bool empty = true;
		std::string itemName = "Empty";
		void UseItem(Player &plyer)
		{

			Mix_Chunk* heal = Mix_LoadWAV("audio\\heal.wav");
			if (empty == true)
			{
				return;
			}
			else
			{
				if(id == 0)
				{
					empty = true;
				}
				if (id == ID_POTION_HEAL)
				{
					Mix_PlayChannel(-1, heal, 0);
					plyer.AddHealth(40); // basic
					mvprintw(15,0,"You feel better from that drink");
				}
				if (id == ID_POTION_SKILL)
				{
					plyer.AddSkill(2);
					Mix_PlayChannel(-1, heal, 0);
					mvprintw(15,0,"You feel more professional");
				}

			}
			empty = true;
			itemName = "Empty";
			id = 0;

		}
		void AddToInventory(int id)
		{
			if (id == ID_POTION_HEAL)
			{
				itemName = "Weak HP Potion";
			}
			if (id == ID_POTION_SKILL)
			{
				itemName = "Potion Of Skill";
			}
			if (id == ID_KEY)
			{
				itemName = "A Key";
			}
			this->id = id;
		}
		// Refactoring Stuff
	};
	void InsertItem(int id);
	void InsertWeapon(int id, char* name);
	void RemoveItem(int slot);
	struct WeaponSlot
	{
		int id = ID_WEAPON_FISTS;

		bool empty = true;
		bool equipped = true;
		std::string itemName = "Fists";
		void AddToInventory(int id, Player & plyer, char* name)
		{
				int baseDmg = plyer.GetDefAttack();
				int baseDefense = plyer.GetDefDefense();
			if (equipped == false)
			{
				itemName = "Fists";
				plyer.SetAttack(baseDmg - 10);
			}
			if (equipped == true){
				if (id == ID_WEAPON_SWORD)
				{
					plyer.SetAttack(baseDmg + 10);
				}
				if(id == ID_WEAPON_CRUDE_SWORD)
				{
					plyer.SetAttack(baseDmg + 5);
					plyer.SetDefense(baseDefense + 15);
				}
				if(id == ID_WEAPON_CRUELAXE)
				{
					plyer.SetAttack(baseDmg + 25);
					plyer.SetDefense(baseDefense - 10);
				}
				if (id == ID_WEAPON_KITESHIELD)
				{
					plyer.SetDefense(baseDmg + 10);
				}
				if (id == ID_WEAPON_ROUNDSHIELD)
				{
					plyer.SetDefense(baseDefense + 15);
				}
				itemName = name;
			}
		
		}
		void EquipItem()
		{
			if (equipped == false)
				equipped = true;
			if (equipped == true)
				equipped = false;
		}
	};
	WeaponSlot wSlots[2]; // Two Weapon Slots
	InventorySlot slots[4]; // 4 inventory slots
	void SetAttack(int val)
	{
		_attack = val;
	}
	void SetDefense(int val)
	{
		_def = val;
	}
	void PrintInventory()
	{
		std::string condition[2] = { "Main-hand", "Off-hand" };
		for (int i = 0; i < 2; i++){
			attron(COLOR_PAIR(2));
			mvprintw(i+2, 50, "%s. ", condition[i].c_str());
			attroff(COLOR_PAIR(2));
			attron(COLOR_PAIR(1));
			printw("%s", wSlots[i].itemName.c_str());
			attroff(COLOR_PAIR(1));
		}
		printw("\n");
		for (int i = 0; i < 4; i++){
			move(19+i,0);
			attron(COLOR_PAIR(2));
			printw(" %d. ", i+1);
			attroff(COLOR_PAIR(2));
			attron(COLOR_PAIR(1));
			printw("%s", slots[i].itemName.c_str());
			attroff(COLOR_PAIR(1));
		}
		
	}
	char* da = "Nothing noteworthy to say.";
private:

	int _x, _y;
	int _xpReq = 40;
	int _def, _hp, _maxHP;
	int _xp;
	int _attack;
	int _lockPick;
	int _level = 1;
	int _skill;	
	int _defaultDef;
	int _defaultAttack;
};



#endif
