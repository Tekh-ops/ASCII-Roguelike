#include <iostream>
#include <vector>
#include "Player.h"
#include <SDL2/SDL_mixer.h>
#include <curses.h>
#include <stdio.h>
#include <time.h>
#include <thread> // I should remove headers.. sigh
#include "Enemy.h"
#include <panel.h>

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
		void RunGame(Map map[], Player &player, WINDOW* window)
		{
			/*We just use this to handle pretty much everything to do with initialization*/
			bool run = GAME_ACTIVE; // Run flag (affects whether game still runs)
			bool LevelLoaded = false;
			char input;
			int currentLevel = 2; // This is the hamlet
			std::vector<Door> doors;
			std::vector<GenericActor> actors;
			std::vector<Enemy> enemies;
			char name[20];
			Mix_Chunk* moveA = Mix_LoadWAV("audio\\move.wav");
			Mix_Chunk* taunt = Mix_LoadWAV("audio\\taunt.wav");
			cbreak();
			attron(A_BOLD);
			init_pair(1, COLOR_YELLOW, COLOR_BLACK);
			attron(COLOR_PAIR(1));
			mvprintw(4, 5,"Before we proceed, your name?\n");
			move ( 5, 5 );
			scanw("%s", name);
			mvprintw(7, 5, "Perfect. Welcome %s\n", name);
			attron(A_BLINK);
			mvaddstr(10, 5, "Press Any Key To Begin The Experience :)");
			attroff(A_BLINK);
			attroff(COLOR_PAIR(1));
			getch();
			flash();
			wrefresh(window);
			curs_set(0);
			/* Main Game Loop*/
			while (run) // equ run == true
			{
				if (LevelLoaded != true){
					map[currentLevel - 1].ProcessLevel(player, doors, actors, enemies);
					// Clear the vectors before
					LevelLoaded = true;
				}
				if (player.GetHealth() > 0){
					noecho();
					mvprintw(1,4, "| %s |", map[currentLevel - 1].getName());
					map[currentLevel - 1].printLevel();
					mvprintw(18, 50, "You are level %d\n", player.GetLevel());
					mvprintw(17, 0,"Name: %s\n", name);
					player.PrintInventory();
					attron(COLOR_PAIR(2));
					mvprintw(16, 50, "Your Health is ");
					attroff(COLOR_PAIR(2));
					printw(" %d \\ %d", player.GetHealth(), player.GetMaxHP());
					attron(COLOR_PAIR(2));
					mvprintw(17, 50, "Your Defense is ");
					attroff(COLOR_PAIR(2));
					printw( "%d", player.GetDefense() );
					attron(COLOR_PAIR(2));
					mvprintw(15, 50, "XP To Next Level : ");
					attroff(COLOR_PAIR(2));
					printw(" %d\\%d", player.GetXP(), player.GetXpTilNextLevel());
					mvprintw(19, 50, "You Do %d raw damage", player.GetAttack());
					mvprintw(20, 50, "Your lockpicking skill is %d", player.GetSkill());
					mvprintw(23, 15, "Q : Quit Game R: Use Item X: Clear Screen V: Drop Item");
				//	std::cout << "  Attack Power: " << player.GetAttack() << std::endl;
				//	std::cout << "Lockpick Skill: " << player.GetSkill() << std::endl;
				//	std::cout << "XP To Next Level: " << player.GetXP() << " \\ " << player.GetXpTilNextLevel() << std::endl;
				//	std::cout << "\n" <<player.da << std::endl;
				//	std::cout << "Q to quit game  R(Use item)";
					input = getch();
					player.LevelUp();
					int prev = currentLevel;
					player.ProcessInput(input, doors, actors, enemies, map[currentLevel - 1], currentLevel);
					if (prev != currentLevel)
					{
						CleanUp::ClearVector(enemies, actors, doors);
						map[currentLevel - 1].ProcessLevel(player, doors, actors, enemies);
						Mix_PlayChannel(-1, moveA, 0);
						clear();
						wrefresh(window);
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
					wrefresh(window);
				}
				else
				{
					Mix_PlayChannel(-1, taunt, 0);
					std::cout << "\n\n\n\n\n\n\n\n\n\n";
					std::cout << "Farewell, " << name << " Your legacy will be remembered." << std::endl;

					/*Write this man's grave*/
					std::ofstream rip("your legacy.txt");
					rip << name << " Was Level " << player.GetLevel() << " Upon death" << std::endl;
					rip << "You were remembered as a hero" << std::endl;
					rip << "But was slain on his journey for the quest item or something.\n";
					rip << "He bore no noteworthy accomplishments obviously." << std::endl;
					attroff(A_BOLD);
					run = GAME_CLOSE;
				}
			}
			return;
		}
	};
};
