#pragma once

#include "State.h"
#include "GameData.h"


class FAID
{
	// agent class

private:
	// variables
	State *currState;
	State *pastState;
	State *startState;
	State *raceState;
	GameData gameData;
	HANDLE process;

public:

	// functions
	FAID(GameData gameData, HANDLE process);
	~FAID();
	void play();

	
};

