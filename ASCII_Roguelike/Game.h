#include <iostream>
#include <vector>
#include "Player.h"
#include <conio.h>
#include "Enemy.h"

// Macros to define flags (More descriptive but otherwise no purpose)
#define GAME_ACTIVE true
#define GAME_CLOSE false
#define ALIVE_ACTOR false
#define DEAD_ACTOR true
namespace Game{
	namespace CleanUp{
		void ClearVector(std::vector<Enemy> &vec, std::vector<GenericActor> &actors, std::vector<Door> &doors)
		{
			if (vec.size() > 0)
			vec.clear();
			if (doors.size() > 0)
			doors.clear();
			if (actors.size() > 0)
			actors.clear();
		}

		void EmptyVector(Player&player,Map&map,std::vector<Enemy> &vec, std::vector<GenericActor> &actors, std::vector<Door> &doors)
		{
			int i = 0;
			for (i = 0; i < vec.size(); i++){
				if (vec[i].isDead(map, player) == DEAD_ACTOR)
					vec.erase(vec.begin() + i);
			}
			for (i = 0; i < doors.size(); i++)
				doors.erase(doors.begin() + i);
			for (i = 0; i < actors.size(); i++){
				if (actors[i].isDead(map) == DEAD_ACTOR)
					actors.erase(actors.begin() + i);
			}
		}
	}
	namespace System{
		void RunGame(Map map[], Player &player)
		{
			/*We just use this to handle pretty much everything to do with initialization*/
			bool run = GAME_ACTIVE; // Run flag (affects whether game still runs)
			bool LevelLoaded = false;
			char input;
			int currentLevel = 2; // This is the hamlet
			std::vector<Door> doors;
			std::vector<GenericActor> actors;
			std::vector<Enemy> enemies;
			std::string name;
			std::cout << "Before we proceed, your name?" << std::endl;
			std::cin >> name;
			std::cout << "Perfect. Welcome " << name << std::endl;

			/* Main Game Loop*/
			while (run) // equ run == true
			{
				if (LevelLoaded != true){
					map[currentLevel - 1].ProcessLevel(player, doors, actors, enemies);
					// Clear the vectors before
					LevelLoaded = true;
				}
				if (player.GetHealth() > 0){
					std::cout << "\n\n";
					std::cout << "| " << map[currentLevel - 1].getName() << " | " << std::endl;
					std::cout << "------------------" << std::endl;
					map[currentLevel - 1].printLevel();
					std::cout << "You are Level " << player.GetLevel() << std::endl;
					std::cout << "Name: " << name;
					std::cout << "  Health: " << player.GetHealth() << " \\ " << player.GetMaxHP();
					std::cout << "  Defense: " << player.GetDefense();
					std::cout << "  Attack Power: " << player.GetAttack() << std::endl;
					std::cout << "Lockpick Skill: " << player.GetSkill() << std::endl;
					std::cout << "XP To Next Level: " << player.GetXP() << " \\ " << player.GetXpTilNextLevel() << std::endl;
					std::cout << "\nQ to quit game\tT to taunt\n";
					input = getch();
					player.LevelUp();
					int prev = currentLevel;
					player.ProcessInput(input, doors, actors, enemies, map[currentLevel - 1], currentLevel);
					if (prev != currentLevel)
					{
						CleanUp::ClearVector(enemies, actors, doors);
						map[currentLevel - 1].ProcessLevel(player, doors, actors, enemies);
					}
					for (int i = 0; i < actors.size(); i++)
					{
						actors[i].AI_Loop(map[currentLevel - 1]);
					}
					for (int i = 0; i < enemies.size(); i++)
					{
						enemies[i].AI_Loop(map[currentLevel - 1], player, actors, input);
					}
					CleanUp::EmptyVector(player, map[currentLevel-1], enemies, actors, doors);
				}
				else
				{
					std::cout << "\n\n\n\n\n\n\n\n\n\n";
					std::cout << "Farewell, " << name << " Your legacy will be remembered." << std::endl;

					/*Write this man's grave*/
					std::ofstream rip(name + "'s legacy.txt");
					rip << name << " Was Level " << player.GetLevel() << " Upon death" << std::endl;
					rip << "You were remembered as a hero" << std::endl;
					rip << "But was slain on his journey for the quest item or something.\n";
					rip << "He bore no noteworthy accomplishments obviously." << std::endl;
					run = GAME_CLOSE;
				}
			}
			return;
		}
	};
};