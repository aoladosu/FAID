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
	// timer for how long to stay in this state
	std::chrono::steady_clock::time_point timer;
};

