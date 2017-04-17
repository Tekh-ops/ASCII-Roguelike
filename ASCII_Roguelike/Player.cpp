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
	case 'T':
	case 't':
	
		srand(time(NULL));
		std::cout << msgs[rand() % 4] << std::endl;
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
		std::cout << "Seems you've hit a wall. hmm" << std::endl;
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
					std::cout << "Lockpicking Success!" << std::endl;
					doors.erase(doors.begin() + i);
					return true;
				}
				else
				{
					std::cout << "Lockpick failed..." << std::endl;
					return false;
				}
			}
		}
	}
	if (map.GetTile(targetX, targetY) == '>' || map.GetTile(targetX, targetY) == '<')
	{
		TakeDamage(20);
		std::cout << "You ran into a trap... Good Job" << std::endl;
		return false;
	}
	// XP BOOST TEMP
	if (map.GetTile(targetX, targetY) == 'I')
	{
		AddXP(20);
		std::cout << "You have gained 20 xp" << std::endl;
	}
	if (map.GetTile(targetX, targetY) == 'h')
	{
		_hp += 20;
		if (_hp > _maxHP)
			_hp = _maxHP;
		std::cout << "You have been healed slightly.\n";
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
	if (map.GetTile(targetX, targetY) == 'g')
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
	if (map.GetTile(targetX, targetY) == 'G')
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