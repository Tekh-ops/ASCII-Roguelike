#ifndef MAP_H
#define MAP_H
#include <string>
#include <vector>
#include <fstream>
#include "common.h"
#include <iostream>
#include "Door.h"
class Player;
//class Door;
class Map
{
public:
	Map();
	bool LoadMap(std::string path);
	const char GetTile(int x, int y);
	void SetTile(const char tile, int x, int y);
	void ProcessLevel(Player &player, std::vector<Door> &doors);
	void printLevel();
private:
	std::vector<std::string> _levelData;
};

#endif