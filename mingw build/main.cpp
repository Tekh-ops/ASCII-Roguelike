#include "Game.h"

const int amt = 7;
// Map names and file names
std::string lvlList[amt][2] =
{
	{"levels\\forest.txt","Forest Of Wild"},
	{ "levels\\hamlet.txt", "Friendly Hamlet"},
	{"levels\\level1.txt", "Privateer's Hold"},
	{"levels\\level2.txt", "Crimson Passage"},
	{"levels\\level3.txt", "Chamber of Misfortune"},
	{ "levels\\level4.txt", "Chamber of Everlasting Schemes" },
	{"levels\\storehouse.txt", "Friendly Storehouse"}
};

int main()
{
	Map map[amt];
	Player player(15, 100, 5, 15, 12);
	WINDOW* window;
	initscr();
	window = newwin(0,0,0,0);
	// Load Maps
	start_color();
	for (int i = 0; i < amt; i++){
		map[i].LoadMap(lvlList[i][0]);
		map[i].setName(lvlList[i][1]);
	}
	Game::System::RunGame(map, player, &window);
	delwin(window);
	endwin();
	return 0;
}
