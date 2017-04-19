#include "Player.h"
#include <time.h>
#include "Enemy.h"
int slot;
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
	case 'x':
	case 'X':
		clear();
		break;
	case 'R':
	case 'r':{
		mvprintw(14, 0,("Which slot?\n"));
		move(15, 0);
		echo();
		char slot_ac = getch();
		if(slot_ac == '1')
			slot = 1;
		if(slot_ac == '2')
			slot = 2;
		if(slot_ac == '3')
			slot = 3;
		if(slot_ac == '4')
			slot = 4;
		if (slot == 1)
		{
			if (slots[0].empty)
			{
				mvprintw(15,0,"This slot is empty\n");
			}else{
			slots[0].UseItem(*this);
			clear();
			}}
		if (slot == 2)
		{
			if (slots[1].empty)
			{
				mvprintw(15,0,"This slot is empty\n");
			}else{
			slots[1].UseItem(*this);
			clear();
			}}
		if (slot == 3)
		{
			if (slots[2].empty)
			{
				mvprintw(15,0,"This slot is empty\n");
			}
			else{
			slots[2].UseItem(*this);
			clear();
			}}
		if (slot == 4)
		{
			if (slots[3].empty)
			{
				mvprintw(15,0,"This slot is empty\n");
			}else{
			slots[3].UseItem(*this);
			clear();
			}
		}
		noecho();
		slot = 0;
		}
		break;
	default:
		mvprintw(15,0,"Invalid Input. try again");
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
					mvprintw(15, 0,"Lockpicking Success!");
					doors.erase(doors.begin() + i);
					return true;
				}
				else
				{
					mvprintw(15,0, "Lockpick failed...");
					return false;
				}
			}
		}
	}
	if ((map.GetTile(targetX, targetY) == '{') || (map.GetTile(targetX,targetY) == '}'))
	{
		mvprintw(15,0,"This door requires a key.");
		return false;
	}
	if (map.GetTile(targetX, targetY) == '>' || map.GetTile(targetX, targetY) == '<')
	{
		TakeDamage(20);
		mvprintw(15,0,"You get nicked by a trap.");
		return false;
	}
	// XP BOOST TEMP
	if (map.GetTile(targetX, targetY) == 'I')
	{
		InsertItem(ID_POTION_SKILL);
		mvprintw(15,0,"You picked up a potion of skill");
	}
	if (map.GetTile(targetX, targetY) == 'h')
	{
		InsertItem(ID_POTION_HEAL);
		mvprintw(15,0,"You picked up a health potion.");
	}
	if (map.GetTile(targetX, targetY) == 'w')
	{
		InsertWeapon(ID_WEAPON_SWORD);
		mvprintw(15,0,"You picked up a sword.");
	}
	if (map.GetTile(targetX, targetY) == 'k')
	{
		InsertWeapon(ID_WEAPON_KITESHIELD);
		mvprintw(15,0,"You picked up a shield.");
	}
	if (map.GetTile(targetX, targetY) == 'o')
	{
		InsertWeapon(ID_WEAPON_ROUNDSHIELD);
		mvprintw(15,0,"You picked up a round shield.");
	}
	if (map.GetTile(targetX, targetY) == 'H')
	{
		_hp = GetMaxHP();
		mvprintw(15,0,"You have been refreshed heavily.");
	}
	// Actor
	if (map.GetTile(targetX, targetY) == 'T')
	{
		for (int i = 0; i < actors.size(); i++)
		{
			if (targetX == actors[i].GetX() && targetY == actors[i].GetY())
			{
				mvprintw(14,0, "%s : %s", actors[i].getName(), actors[i].getResponse());
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
