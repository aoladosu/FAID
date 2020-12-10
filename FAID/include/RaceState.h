#pragma once
#include "State.h"
class RaceState : public State
{
public:
	RaceState(GameData *gameData);

	int update(StateNumber& stateVal) override;
	void enterState(StateData stateData) override;
	StateData exitState() override;
};

