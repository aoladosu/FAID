#pragma once

#include <Windows.h>

// stores information about the game

class GameData
{
public:
	
	// car position mem locations
	uintptr_t xAddr;
	uintptr_t yAddr;
	uintptr_t zAddr;

	// control mem location
	uintptr_t controls;

	// enemy position locations
	uintptr_t enemyPos;

	// stage obstacles
	int *chkPts;
	int *obstacles;
	int *jumps;

};

