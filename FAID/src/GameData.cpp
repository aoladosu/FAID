#include "GameData.h"
#include <iostream>

GameData::GameData() {}

void GameData::print()
{
	// print values in game data
	std::cout << "xAddress: " << std::hex << xAddr << "\n";
	std::cout << "yAddress: " << std::hex << yAddr << "\n";
	std::cout << "zAddress: " << std::hex << zAddr << "\n";
	std::cout << "ctrlAddress: " << std::hex << ctrlAddr << "\n";
	std::cout << "chkPtAddress: " << std::hex << chkPtAddr << "\n";

}

