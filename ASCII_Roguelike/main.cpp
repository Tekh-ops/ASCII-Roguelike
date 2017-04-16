#include "Game.h"
int main()
{
	Map map;
	Player player(15, 100, 5);
	map.LoadMap("levels\\level1.txt");
	RunGame(map, player);
	return 0;
}