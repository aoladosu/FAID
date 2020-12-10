#pragma once
#include "State.h"
#include <chrono>

class StartState : public State
{

public:
	StartState(GameData *gameData);
	
	int update(StateNumber& stateVal) override;
	void enterState(StateData stateData) override;
	StateData exitState() override;

private:
	// timer for how long to stay in this state
	std::chrono::steady_clock::time_point timer;
};

