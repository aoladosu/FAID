#include "RaceState.h"
#include "CollisionUtil.h"
#include <iostream>

RaceState::RaceState(GameData* gameData) : State(gameData){}

int RaceState::update(StateNumber& stateVal)
{
	// update car variables
	updateCarInfo();

	// get goal to drive to
	setGoal();
	adjustGoal();

	// get direction to drive towards
	int direction = getDriveDirection();

	// determine state transition
	stateVal = nextState();

	return direction;
}

void RaceState::enterState(StateData stateData)
{
	std::cout << "Entering race state\n";
	X = stateData.X;
	Y = stateData.Y;
	Z = stateData.Z;
}

StateData RaceState::exitState()
{

	std::cout << "Exiting race state\n";

	StateData stateData = StateData();
	stateData.X = X;
	stateData.Y = Y;
	stateData.Z = Z;
	stateData.goalX = goalX;
	stateData.goalY = goalY;
	stateData.dirX = dirX;
	stateData.dirY = dirY;
	stateData.valid = true;

	return stateData;
}

StateNumber RaceState::nextState()
{
	// determine state transition
	int obsLoc;
	bool isJump;
	StateNumber stateVal = StateNumber::CurrentState;

	if (obsCollision(gameData->jumps, gameData->jumpsSize, gameData->obstacles, gameData->obsSize, X, Y, dirX, dirY, obsLoc, isJump)) {
		// colliding with obstacle

		int xPos, yPos;
		if (isJump) {
			xPos = gameData->jumps[obsLoc + 1];
			yPos = gameData->jumps[obsLoc + 2];
		}
		else {
			xPos = gameData->obstacles[obsLoc + 1];
			yPos = gameData->obstacles[obsLoc + 2];
		}

		if (distSquared(X, Y, xPos, yPos) < distSquared(X, Y, goalX, goalY)) {
			// check if obstacle collision is behind goal location first
			stateVal = StateNumber::AvoidState;
		}
	}

	return stateVal;
}

void RaceState::setGoal()
{
	// set the goal of this state to the next checkpoint
	int newChkPt;
	ReadProcessMemory(gameData->process, (LPVOID)gameData->chkPtAddr, &newChkPt, sizeof(newChkPt), NULL);
	if (newChkPt != currChkPt) {
		currChkPt = newChkPt;
		int offset = (newChkPt % gameData->chkPtsSize) * 3;
		goalX = gameData->chkPts[offset];
		goalY = gameData->chkPts[offset + 1];
		goalZ = 0;
	}
}

void RaceState::adjustGoal()
{
	// adjust the goal if we are coming at it from a bad angle
	// TODO: above
}

