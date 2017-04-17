#include "Game.h"

const int amt = 4;

std::string lvlList[amt][2] =
{
	{ "levels\\hamlet.txt", "Friendly Hamlet"},
	{"levels\\level1.txt", "Privateer's Hold"},
	{"levels\\level2.txt", "Crimson Passage"},
	{"levels\\level3.txt", "The End"}
};

int main()
{
	Map map[amt];
	Player player(15, 100, 5, 15);
	for (int i = 0; i < amt; i++){
		map[i].LoadMap(lvlList[i][0]);
		map[i].setName(lvlList[i][1]);
	}

	RunGame(map, player);
	return 0;
}