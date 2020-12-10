#pragma once

#include <Windows.h>

// stores information about the game

class GameData
{
public:
	
	GameData();

	// car position mem locations
	uintptr_t xAddr = 0;
	uintptr_t yAddr = 0;
	uintptr_t zAddr = 0;

	// control mem location
	uintptr_t ctrlAddr = 0;

	// enemy position locations
	uintptr_t enemyPosAddr = 0;

	// stage obstacles
	short int stage = 0;
	short int chkPtsSize = 0;
	short int obsSize = 0;
	short int jumpsSize = 0;
	int *chkPts = 0;
	int *obstacles = 0;
	int *jumps = 0;

};

