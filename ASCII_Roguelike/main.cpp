#include "Game.h"

std::string lvlList[3] =
{
	"levels\\level1.txt",
	"levels\\level2.txt",
	"levels\\level3.txt"
};

int main()
{
	Map map[3];
	Player player(15, 100, 5, 15);
	for (int i = 0; i < 3; i++)
	map[i].LoadMap(lvlList[i]);

	RunGame(map, player);
	return 0;
}