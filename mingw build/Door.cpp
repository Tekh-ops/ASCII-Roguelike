#include "Door.h"
#include "Player.h"

Door::Door(int strength, int x, int y) :
_strength(strength),
_x(x),
_y(y)
{
}

void Door::setPosition(int x, int y)
{
	_x = x;
	_y = y;
}

bool Door::attemptOpen(int skill)
{
	if (skill >= _strength)
		return true;
	else
		return false;
}