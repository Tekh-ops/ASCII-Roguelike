#include "SaveWriter.h"
#include "Player.h"
SaveWriter::SaveWriter(){
}

SaveWriter::SaveWriter(char* fileName):
_fileName(fileName){
	_writeFile.open(fileName);
}

SaveWriter::~SaveWriter(){
	_writeFile.close();
}

void SaveWriter::WriteData(Player player, int currentLevel){
	_writeFile << player.name << "|" << player.GetHealth() << "|" << player.GetMaxHP() << "|" << player.GetLevel() << "|" << player.GetX() << "|" << player.GetY() << "|" << currentLevel;	
}
// Specified Data
void SaveWriter::WriteImplictData(){
	_writeFile << "tester";
}

void SaveWriter::Begin()
{
	_writeFile.open(_fileName);
}

void SaveWriter::Finish()
{
	_writeFile.close();
}
