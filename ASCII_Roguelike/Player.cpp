#include "Player.h"
#include <time.h>
std::string msgs[4] = { "This game sucks", "I'm bored", "I should play MGS:V", "Bored..." };
Player::Player()
{

}

Player::Player(int defense, int health, int lockpick) :
_def(defense), _hp(health), _lockPick(lockpick)
{
	_xp = 0;
}

void Player::setPosition(int x, int y)
{
	_x = x;
	_y = y;
}

void Player::ProcessInput(char in, std::vector<Door> doors , Map &map)
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
		if (ProcessMove(map, doors , targetX, targetY))
		{
			map.SetTile('.', prevX, prevY);
			map.SetTile('@', targetX, targetY);
			setPosition(targetX, targetY);
		}
	break;
	case 's':
	case'S':
		targetY += 1;
		if (ProcessMove(map, doors ,targetX, targetY))
		{
			map.SetTile('.', prevX, prevY);
			map.SetTile('@', targetX, targetY);
			setPosition(targetX, targetY);
		}
		break;
	case 'a':
	case'A':
		targetX -= 1;
		if (ProcessMove(map, doors, targetX, targetY))
		{
			map.SetTile('.', prevX, prevY);
			map.SetTile('@', targetX, targetY);
			setPosition(targetX, targetY);
		}
		break;
	case 'd':
	case'D':
		targetX += 1;
		if (ProcessMove(map, doors ,targetX, targetY))
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

bool Player::ProcessMove(Map &map, std::vector<Door> doors, int targetX, int targetY)
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
				std::cout << "Attempted Lockpicking" << std::endl;
			
				return doors[i].attemptOpen(GetSkill());
				
			}
		}
	}
	if (map.GetTile(targetX, targetY) == '>' || map.GetTile(targetX, targetY) == '<')
	{
		_hp -= 20;
		std::cout << "You ran into a trap... Good Job" << std::endl;
		return false;
	}
	return true;
}