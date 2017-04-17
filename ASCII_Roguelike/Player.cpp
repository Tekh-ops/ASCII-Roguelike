#include "Player.h"
#include <time.h>
std::string msgs[4] = { "This game sucks", "I'm bored", "I should play MGS:V", "Bored..." };
Player::Player()
{

}

Player::Player(int defense, int health, int lockpick, int attack) :
_def(defense), _hp(health), _lockPick(lockpick), _attack(attack)
{
	_maxHP = _hp;
	_xp = 0;
}

void Player::setPosition(int x, int y)
{
	_x = x;
	_y = y;
}

void Player::ProcessInput(char in, std::vector<Door> &doors, std::vector<GenericActor> actors , Map &map, int &lvl)
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
		if (ProcessMove(map, doors, actors , targetX, targetY, lvl))
		{
			map.SetTile('.', prevX, prevY);
			map.SetTile('@', targetX, targetY);
			setPosition(targetX, targetY);
		}
	break;
	case 's':
	case'S':
		targetY += 1;
		if (ProcessMove(map, doors, actors ,targetX, targetY, lvl))
		{
			map.SetTile('.', prevX, prevY);
			map.SetTile('@', targetX, targetY);
			setPosition(targetX, targetY);
		}
		break;
	case 'a':
	case'A':
		targetX -= 1;
		if (ProcessMove(map, doors, actors ,targetX, targetY, lvl))
		{
			map.SetTile('.', prevX, prevY);
			map.SetTile('@', targetX, targetY);
			setPosition(targetX, targetY);
		}
		break;
	case 'd':
	case'D':
		targetX += 1;
		if (ProcessMove(map, doors, actors ,targetX, targetY,lvl))
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

bool Player::ProcessMove(Map &map, std::vector<Door> &doors, std::vector<GenericActor> actors, int targetX, int targetY, int &lvl)
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
	// Actor
	if (map.GetTile(targetX, targetY) == 'T')
	{
		std::cout << "You bumped into a man" << std::endl;
		return false;
	}
	if (map.GetTile(targetX, targetY) == 'a')
	{
		std::cout << "Place holder for previous level" << std::endl;
		map.SetVisited(true);
		map.EnteredViaLeft(true);
		lvl--;
		return false;
	}
	if (map.GetTile(targetX, targetY) == 'd')
	{
		std::cout << "Place holder for next level" << std::endl;
		map.SetVisited(true);
		map.EnteredViaRight(true);
		lvl++;
		return false;
	}
	return true;
}