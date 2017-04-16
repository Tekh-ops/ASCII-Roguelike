#include <iostream>
#include <vector>
#include "Player.h"
#include <conio.h>

void RunGame(Map map, Player &player)
{
	bool run = true; 
	bool LevelLoaded = false;
	char input;
	std::vector<Door> doors;
	std::string name;
	std::cout << "Before we proceed, your name?" << std::endl;
	std::cin >> name;
	std::cout << "Perfect. Welcome " << name << std::endl;
	
	while (run)
	{
		if (LevelLoaded != true){
			map.ProcessLevel(player, doors);
			LevelLoaded = true;
		}
		if (player.GetHealth() > 0){
			map.printLevel();
			std::cout << "You are Level " << player.GetLevel() << std::endl;
			std::cout << "Name: " << name << std::endl;
			std::cout << "Health: " << player.GetHealth() << std::endl;
			std::cout << "Defense: " << player.GetDefense() << std::endl;
			std::cout << "Lockpick Skill : " << player.GetSkill() << std::endl;
			std::cout << "Amount of Doors: " << doors.size() << std::endl;
			std::cout << "XP To Next Level: " << player.GetXpTilNextLevel() << std::endl;
			std::cout << "\n\n";
			input = getch();
			player.ProcessInput(input, doors, map);
		}
		else
		{
			std::cout << "\n\n\n\n\n\n\n\n\n\n";
			std::cout << "Farewell, " << name << " Your legacy will be remembered." << std::endl;
			std::ofstream rip(name + "'s legacy.txt");
			rip << name << "Was Level " << player.GetLevel() << " Upon death" << std::endl;
			rip << "You were remembered as a hero" << std::endl;
			run = false;
		}
	}
	return;
}