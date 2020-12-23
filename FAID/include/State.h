#pragma once

#include "GameData.h"
#include "StateData.h"

// base state class
class State
{
public:

	State();
	State(GameData *gameData);

	// perform update actions and return the next state
	virtual int update(StateNumber &stateVal) = 0;
	// perform any actions needed upon entering state
	virtual void enterState(StateData stateData) = 0;
	// perform any actions needed upon exiting state
	virtual StateData exitState() = 0;

private:
	virtual StateNumber nextState() = 0;

protected:
	// game data
	GameData* gameData = 0;

	// arrow keys
	int idle = 0;
	int up = 0x010000;
	int down = 0x01000000;
	int left = 0x01;
	int right = 0x0100;
	int space = 0x01;

	// car info
	int X = 0;
	int Y = 0;
	int Z = 0;
	int dirX = 0;
	int dirY = 0;
	int goalX = 0;
	int goalY = 0;
	int goalZ = 0;

};

