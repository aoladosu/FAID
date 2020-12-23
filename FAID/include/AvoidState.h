#pragma once
#include "State.h"

#include <chrono>

/*
* this state avoids stationary objects
* It handles one at a time before handing control back to the 
* state that called it
*/

class AvoidState : public State
{
public:
	AvoidState(GameData* gameData);

	int update(StateNumber& stateVal) override;
	void enterState(StateData stateData) override;
	StateData exitState() override;

private:
	int obstacleNum = -1;
	bool isJump = false;
	bool onJumpDir = false;
	int obsX = 0;
	int obsY = 0;
	int obsAngle = 0;
	// these goal positions are where another position wants to go,
	// not where this state wants to go
	int endGoalX = 0;
	int endGoalY = 0;
};

