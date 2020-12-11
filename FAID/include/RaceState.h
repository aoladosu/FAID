#pragma once
#include "State.h"
class RaceState : public State
{
public:
	RaceState(GameData *gameData);

	int update(StateNumber& stateVal) override;
	void enterState(StateData stateData) override;
	StateData exitState() override;

private:
	// variables
	short int currChkPt = 0;
	bool goalReached = false;
	const double PI = 3.14159;

	// functions
	void setGoal();
	void adjustGoal();
	int getDriveDirection();
};

