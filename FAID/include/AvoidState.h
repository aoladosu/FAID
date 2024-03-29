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

protected:
	// functions
	StateNumber nextState() override;

private:
	// variables
	int obstacleNum = -1;
	bool isJump = false;
	bool onJumpDir = false;
	int obsX = 0;
	int obsY = 0;
	int obsAngle = 0;
	int obsPiece = 0;
	// these goal positions are where another position wants to go,
	// not where this state wants to go
	int endGoalX = 0;
	int endGoalY = 0;

	void setGoal();
	
	bool biDirectionalJump(int piece);
	void setGoalDestination(int& A1x, int& A1y, int& A2x, int& A2y, int L, int heightUp, int heightDown, int widthLeft, int widthRight);

	// avoid functions, exception cases
	void dirtSpeedBumpAvoid(int &A1x, int &A1y, int &A2x, int &A2y, int L);

};

