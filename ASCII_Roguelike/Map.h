#ifndef MAP_H
#define MAP_H
#include <string>
#include <vector>
#include <fstream>
#include "common.h"
#include <iostream>
#include "Door.h"
#include "GenericActor.h"
class Player;
//class Door;
class Map
{
public:
	Map();
	bool LoadMap(std::string path);
	const char GetTile(int x, int y);
	void SetTile(const char tile, int x, int y);
	void ProcessLevel(Player &player, std::vector<Door> &doors, std::vector<GenericActor> &actors);
	void printLevel();
	void SetVisited(bool visited) { _visited = visited; }
	void EnteredViaRight(bool flag) { _rightEntered = flag; }
	void EnteredViaLeft(bool flag) { _leftEntered = flag; }
private:
	bool _visited = false;
	bool _leftEntered = false;
	bool _rightEntered = false;
	std::vector<std::string> _levelData;
};

#endif