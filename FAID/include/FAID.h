#pragma once

#include "State.h"
#include "GameData.h"

#include <string>


class FAID
{
	// agent class

private:
	// variables
	State *currState;
	State *pastState;
	State *startState;
	State *raceState;
	State* avoidState;
	GameData gameData;
	HANDLE process;
	void memoryErrorPrint(bool read, uintptr_t addr, std::string addrName);

public:
	// functions
	FAID(GameData gameData, HANDLE process);
	~FAID();
	void play();
	bool verifyMemory();

	
};

