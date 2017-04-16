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
	std::vector<GenericActor> actors;
	std::string name;
	std::cout << "Before we proceed, your name?" << std::endl;
	std::cin >> name;
	std::cout << "Perfect. Welcome " << name << std::endl;
	
	while (run)
	{
		if (LevelLoaded != true){
			map.ProcessLevel(player, doors, actors);
			LevelLoaded = true;
		}
		if (player.GetHealth() > 0){
			map.printLevel();
			std::cout << "You are Level " << player.GetLevel() << std::endl;
			std::cout << "Name: " << name;
			std::cout << "  Health: " << player.GetHealth() << " \\ " << player.GetMaxHP() ;
			std::cout << "  Defense: " << player.GetDefense();
			std::cout << "  Attack Power: " << player.GetAttack() << std::endl;
			std::cout << "Lockpick Skill: " << player.GetSkill() << std::endl;
			std::cout << "XP To Next Level: " << player.GetXP() << " \\ " << player.GetXpTilNextLevel() << std::endl;
			std::cout << "There are " << actors.size() << " NPCs " << "| There are " << doors.size() << " Doors Locked" << std::endl;
			std::cout << "\nQ to quit game\tT to taunt\n";
			std::cout << "\n\n\n";
			input = getch();
			player.LevelUp();
			player.ProcessInput(input, doors, actors ,map);
			for (int i = 0; i < actors.size(); i++)
			{
				actors[i].AI_Loop(map);
				if (actors[i].isDead(map))
					actors.erase(actors.begin() + i);
			}
		}
		else
		{
			std::cout << "\n\n\n\n\n\n\n\n\n\n";
			std::cout << "Farewell, " << name << " Your legacy will be remembered." << std::endl;
			std::ofstream rip(name + "'s legacy.txt");
			rip << name << " Was Level " << player.GetLevel() << " Upon death" << std::endl;
			rip << "You were remembered as a hero" << std::endl;
			run = false;
		}
	}
	return;
}