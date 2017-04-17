#include "Map.h"
#include "Enemy.h"
std::string names[] =
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
			// 'T' Actor
			if (_levelData[i][j] == 'T')
			{
				actors.push_back(GenericActor(100, 0, false, 35, j, i, j*i - j, names[a]));
				a++;
			}
			// Goblin
			if (_levelData[i][j] == 'g')
			{
				enemies.push_back(Enemy(30, 25, 20, j, i, j*i - j + 1, "Goblin", 'g', 20, 11));
			}
			if (_levelData[i][j] == 'G')
			{
				enemies.push_back(Enemy(70, 35, 250, j, i, j*i - 3, "Gorbalt The Cruel", 'G', 45, 25));
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
			std::cout << _levelData[i][j];
		}
		std::cout << "\n";
	}
}