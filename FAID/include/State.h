#pragma once

#include "GameData.h"
#include "StateData.h"

// base state class
class State
{
public:

	State();
	State(GameData *gameData);

	// game data
	GameData *gameData=0;

	// perform update actions and return the next state
	virtual int update(StateNumber &stateVal) = 0;
	// perform any actions needed upon entering state
	virtual void enterState(StateData stateData) = 0;
	// perform any actions needed upon exiting state
	virtual StateData exitState() = 0;

};

