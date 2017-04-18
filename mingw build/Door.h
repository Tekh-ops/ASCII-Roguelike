#pragma once
class Player;
class Door
{
public:
	Door()
	{}
	Door(int strength, int x, int y);
	void setPosition(int x, int y);
	void setStrength(int str){ _strength = str; }
	bool attemptOpen(int skill);
	inline int getX() { return _x; }
	inline int getY() { return _y; }
private:
	int _strength;
	int _x, _y;
};

