#include "Map.h"

Map::Map()
{
}

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
void Map::ProcessLevel(Player &player, std::vector<Door> &doors)
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