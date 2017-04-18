#include "Map.h"
#include "Enemy.h"
#include <curses.h>

std::string names[8] =
{
	"Harax Loren",
	"Bill Smithenson",
	"Gilligan Herk",
	"Johnny Lucks",
	"Oswald",
	"Harres Aswed",
	"Trenton Lern",
	"Tory Asmith"
};

Map::Map()
{
}
int a = 0; // Counter
// Load the map
bool Map::LoadMap(std::string path)
{
	std::ifstream _input(path);
	std::string tempLine;
	// Was std::getline(_input, tempLine)
	while (_input >> tempLine)
	{
		// Add the lines to the level
			_levelData.push_back(tempLine);
	}
	_input.close();
	return true;
}

// Really more of something that sets positions
void Map::ProcessLevel(Player &player, std::vector<Door> &doors, std::vector<GenericActor> &actors, std::vector<Enemy> &enemies)
{
	a = 0;
	for (int i = 0; i < _levelData.size(); i++)
	{
		for (int j = 0; j < _levelData[i].size(); j++)
		{
			if (_levelData[i][j] == '@')
			{
					player.setPosition(j, i);
			}
			// Low level door
			if (_levelData[i][j] == '|' || _levelData[i][j] == '-')
			{
				doors.push_back(Door(5, j, i));
			}
			// High level door
			if (_levelData[i][j] == 'X')
			{
				doors.push_back(Door(15, j, i));
			}
			// IMPOSSIBLE DOOR (Unless Key)
			if (_levelData[i][j] == '{' || _levelData[i][j] == '}')
			{
				doors.push_back(Door(10000, j, i));
			}
			// 'T' Actor
			if (_levelData[i][j] == 'T')
			{
				actors.push_back(GenericActor(100, 0, false, 35, j, i, j*i - j, names[a]));
				a++;
			}
			/*
			Enemies are structured like this
			lowercase = normal
			uppercase = named boss
			*/

			// Goblin
			if (_levelData[i][j] == 'g')
			{
				enemies.push_back(Enemy(30, 25, 20, j, i, j*i - j + 1, "Goblin", 'g', 20, 11));
			}
			if (_levelData[i][j] == 'G')
			{
				enemies.push_back(Enemy(70, 35, 250, j, i, j*i - 3, "Gorbalt The Cruel", 'G', 30, 25));
			}
			// Rat
			if (_levelData[i][j] == 'r')
			{
				enemies.push_back(Enemy(15, 2, 5, j, i, j*i - j + 2, "Rat", 'r', 15, 5));
			}
			if (_levelData[i][j] == 'R')
			{
				enemies.push_back(Enemy(55, 15, 200, j, i, j*i - 2, "King Of Rats", 'R', 25, 30));
			}
			// Snake
			if (_levelData[i][j] == 's')
			{
				enemies.push_back(Enemy(15, 15, 10, j, i, j*i - j + 3, "Viper", 's', 30, 10));
			}
			if (_levelData[i][j] == 'S')
			{
				enemies.push_back(Enemy(60, 35, 300, j, i, j*i - 4, "Lord Of Schemes", 'S', 30, 30));
			}
		}
	}
}
		

// Recieve a tile from the map
const char Map::GetTile(int x, int y)
{
	return _levelData[y][x];
}
// Tile = "Symbol"
void Map::SetTile(const char tile, int x, int y)
{
	_levelData[y][x] = tile;
}

void Map::printLevel()
{
	// YAY LEVEL PRINTS SUCCESSFULLY
	for (int i = 0; i < _levelData.size(); i++)
	{
		for (int j = 0; j < _levelData[i].size(); j++)
		{
			mvaddch(i+4,j,_levelData[i][j]);
		}
		std::cout << "\n";
	}
}
