#pragma once
#include "State.h"


/*
* this state moves the car toward each checkpoint
* it transitions to avoid state if there is an obstacle in the way
*/

class RaceState : public State
{
public:
	RaceState(GameData *gameData);

	int update(StateNumber& stateVal) override;
	void enterState(StateData stateData) override;
	StateData exitState() override;

private:
	// variables
	short int currChkPt = -1;

	// functions
	void setGoal();
	void adjustGoal();

protected:
	StateNumber nextState() override;

};

