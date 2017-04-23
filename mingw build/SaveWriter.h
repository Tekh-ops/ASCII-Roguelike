#include <fstream>

class Player;
class SaveWriter
{
	public:
	// Blank Constructor
	SaveWriter();
	SaveWriter(char* fileName);
	~SaveWriter();
	void WriteData(Player player, int currentLevel);
	void Finish();
	void Begin();
	void WriteImplictData();
	private:
	std::ofstream _writeFile;
	char* _fileName;
};