#include "Player.h"
#include <time.h>
#include "Enemy.h"
std::string msgs[4] = { "This game sucks", "I'm bored", "I should play MGS:V", "Bored..." };
Player::Player()
{
}

Player::Player(int defense, int health, int lockpick, int attack, int skill) :
_def(defense), _hp(health), _lockPick(lockpick), _attack(attack), _skill(skill)
{
	_maxHP = _hp;
	_xp = 0;
}

void Player::setPosition(int x, int y)
{
	_x = x;
	_y = y;
}

void Player::InsertItem(int id)
{
	bool alreadyInInventory = false;
	for (int i = 0; i < 4; i++)
	{
		if (slots[i].empty == true)
		{
			if (alreadyInInventory == false){
				slots[i].AddToInventory(id);
				alreadyInInventory = true;
				slots[i].empty = false;
			}
		}
	}
}
void Player::InsertWeapon(int id)
{
	bool alreadyInInventory = false;
	for (int i = 0; i < 2; i++)
	{
		if (wSlots[i].empty == true)
		{
			if (alreadyInInventory == false){
				wSlots[i].AddToInventory(id, *this);
				alreadyInInventory = true;
				wSlots[i].empty = false;
			}
		}
	}
}

void Player::ProcessInput(char in, std::vector<Door> &doors, std::vector<GenericActor> &actors, std::vector<Enemy> &enemy , Map &map, int &lvl)
{
	int targetX, targetY, prevX, prevY;
	prevX = _x;
	prevY = _y;
	targetX = _x;
	targetY = _y;
	da = "Nothing";
	switch (in)
	{
	case 'w':
	case'W':
		targetY -= 1;
		if (ProcessMove(map, doors, actors , enemy ,targetX, targetY, lvl))
		{
			map.SetTile('.', prevX, prevY);
			map.SetTile('@', targetX, targetY);
			setPosition(targetX, targetY);
		}
	break;
	case 's':
	case'S':
		targetY += 1;
		if (ProcessMove(map, doors, actors, enemy ,targetX, targetY, lvl))
		{
			map.SetTile('.', prevX, prevY);
			map.SetTile('@', targetX, targetY);
			setPosition(targetX, targetY);
		}
		break;
	case 'a':
	case'A':
		targetX -= 1;
		if (ProcessMove(map, doors, actors, enemy ,targetX, targetY, lvl))
		{
			map.SetTile('.', prevX, prevY);
			map.SetTile('@', targetX, targetY);
			setPosition(targetX, targetY);
		}
		break;
	case 'd':
	case'D':
		targetX += 1;
		if (ProcessMove(map, doors, actors, enemy ,targetX, targetY,lvl))
		{
			map.SetTile('.', prevX, prevY);
			map.SetTile('@', targetX, targetY);
			setPosition(targetX, targetY);
		}
		break;
	case 'Q':
	case 'q':
		// should replace with something else
		// since it's not really an error
		exit(0);
		break;
	case 'T':
	case 't':
		srand(time(NULL));
		std::cout << msgs[rand() % 4] << std::endl;
		break;
	case 'R':
	case 'r':
		std::cout << "Which slot?\n";
		int slot;
		std::cin >> slot;
		if (slot == 1)
		{
			if (slots[0].empty)
			{
				PrintMsg("This slot is empty\n");
			}
			slots[0].UseItem(*this);
		}
		if (slot == 2)
		{
			if (slots[1].empty)
			{
				PrintMsg("This slot is empty\n");
			}
			slots[1].UseItem(*this);
		}
		if (slot == 3)
		{
			if (slots[2].empty)
			{
				PrintMsg("This slot is empty\n");
			}
			slots[2].UseItem(*this);
		}
		if (slot == 4)
		{
			if (slots[3].empty)
			{
				PrintMsg("This slot is empty\n");
			}
			slots[3].UseItem(*this);
		}
		break;
	default:
		std::cout << "Invalid Input. try again" << std::endl;
		break;
	}
}

bool Player::ProcessMove(Map &map, std::vector<Door> &doors, std::vector<GenericActor> &actors, std::vector<Enemy> &enemy ,int targetX, int targetY, int &lvl)
{
	if ((map.GetTile(targetX, targetY) == '#'))
    {
		PrintMsg("Seems you've hit a wall. hmm");
		return false;
	}
	if ((map.GetTile(targetX, targetY) == '|') || (map.GetTile(targetX,targetY) == '-') || map.GetTile(targetX, targetY) == 'X')
	{
		for (int i = 0; i < doors.size(); i++)
		{
			if (doors[i].getX() == targetX && doors[i].getY() == targetY)
			{
				if (doors[i].attemptOpen(GetSkill()) == true)
				{
					PrintMsg("Lockpicking Success!");
					doors.erase(doors.begin() + i);
					return true;
				}
				else
				{
					PrintMsg("Lockpick failed...");
					return false;
				}
			}
		}
	}
	if ((map.GetTile(targetX, targetY) == '{') || (map.GetTile(targetX,targetY) == '}'))
	{
		PrintMsg("This door requires a key.");
		return false;
	}
	if (map.GetTile(targetX, targetY) == '>' || map.GetTile(targetX, targetY) == '<')
	{
		TakeDamage(20);
		PrintMsg("You get nicked by a trap.");
		return false;
	}
	// XP BOOST TEMP
	if (map.GetTile(targetX, targetY) == 'I')
	{
		InsertItem(ID_POTION_SKILL);
		PrintMsg("You picked up a potion of skill");
	}
	if (map.GetTile(targetX, targetY) == 'h')
	{
		InsertItem(ID_POTION_HEAL);
		PrintMsg("You picked up a health potion.");
	}
	if (map.GetTile(targetX, targetY) == 'w')
	{
		InsertWeapon(ID_WEAPON_SWORD);
		PrintMsg("You picked up a sword.");
	}
	if (map.GetTile(targetX, targetY) == 'k')
	{
		InsertWeapon(ID_WEAPON_KITESHIELD);
		PrintMsg("You picked up a shield.");
	}
	if (map.GetTile(targetX, targetY) == 'o')
	{
		InsertWeapon(ID_WEAPON_ROUNDSHIELD);
		PrintMsg("You picked up a round shield.");
	}
	if (map.GetTile(targetX, targetY) == 'H')
	{
		_hp = GetMaxHP();
		PrintMsg("You have been refreshed heavily.");
	}
	// Actor
	if (map.GetTile(targetX, targetY) == 'T')
	{
		for (int i = 0; i < actors.size(); i++)
		{
			if (targetX == actors[i].GetX() && targetY == actors[i].GetY())
			{
				std::cout<< actors[i].getName() << " : " <<actors[i].getResponse();
			}
		}
		return false;
	}
	if (map.GetTile(targetX, targetY) == 'a')
	{
		map.SetVisited(true);
		map.EnteredViaLeft(true);
		lvl--;
		return false;
	}
	if (map.GetTile(targetX, targetY) == 'd')
	{
		map.SetVisited(true);
		map.EnteredViaRight(true);
		lvl++;
		return false;
	}
	if (map.GetTile(targetX, targetY) == '(')
	{
		map.SetVisited(true);
		lvl = 2;
		return false;
	}
	if (map.GetTile(targetX, targetY) == '0')
	{
		map.SetVisited(true);
		lvl = 7;
		return false;
	}
	if (map.GetTile(targetX, targetY) == 'g' || map.GetTile(targetX, targetY) == 'G' || map.GetTile(targetX, targetY) == 's' || map.GetTile(targetX, targetY) == 'S' || map.GetTile(targetX, targetY) == 'r' || map.GetTile(targetX, targetY) == 'R')
	{
		for (int i = 0; i < enemy.size(); i++)
		{
			if (enemy[i].GetX() == targetX && enemy[i].GetY() == targetY)
			{
				enemy[i].EngageBattle(*this, targetX, targetY);
			}
		}
		return false;
	}
	return true;
}

std::string Player::whoKilledMe(Enemy enemy, std::string str)
{
	str = enemy.getName();
	return str;
}