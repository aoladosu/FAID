#include "RaceState.h"
#include "CollisionUtil.h"
#include <cmath>
#include <iostream>

RaceState::RaceState(GameData* gameData) : State(gameData){}

int RaceState::update(StateNumber& stateVal)
{
	// update position variables
	int newX, newY, newZ=0;
	ReadProcessMemory(gameData->process, (LPVOID)gameData->xAddr, &newX, sizeof(newX), NULL);
	ReadProcessMemory(gameData->process, (LPVOID)gameData->yAddr, &newY, sizeof(newY), NULL);
	//ReadProcessMemory(gameData->process, (LPVOID)gameData->zAddr, &newZ, sizeof(z), NULL);

	// get goal to drive to
	setGoal();
	adjustGoal();


	// update variables
	if ((newY != Y) || (newX != X)){
		dirX = newX - X;
		dirY = newY - Y;
		X = newX;
		Y = newY;
		Z = newZ;
	}

	// get direction to drive towards
	int direction = getDriveDirection();

	// determine state transition
	int obsLoc;
	bool isJump;
	stateVal = StateNumber::CurrentState;

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
	
	return direction;
}

void RaceState::enterState(StateData stateData)
{
	std::cout << "Entering race state\n";

	// copy information passed on from previous state
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

int RaceState::getDriveDirection()
{
	// return the direction to go to reach the goal
	int direction = up;
	int goalDirX = goalX - X;
	int goalDirY = goalY - Y;

	// look at dot product to get angle between vectors to decide how hard to turn
	// or to turn at all
	int dot = dirX * (goalDirX) + dirY * (goalDirY);
	float lenDir = sqrt(dirX * dirX + dirY * dirY);
	float lenGoal = sqrt(goalDirX * goalDirX + goalDirY * goalDirY);
	float angle = acos(dot / (lenDir * lenGoal)) * 180 / PI;
	int angleOffset = 9;

	// if we are within 10 degrees of target, then we are fine
	if ((angle < angleOffset) && !isnan(angle)) {
		return up;
	}

	// cross product, get z component to decide turn direction
	int zComp = (goalDirX) * dirY - (goalDirY) * dirX;

	// check if we are on the line
	if (zComp == 0) {
		// if z=0, we are either heading in the right direction or opposite direction
		int diffX = goalDirX;
		int diffY = goalDirY;
		if ((((diffX >= 0) && (dirX >= 0)) || ((diffX <= 0) && (dirX <= 0))) && ((diffY >= 0) && (dirY >= 0)) || ((diffY <= 0) && (dirY <= 0))) {
			direction |= up;
		}
		else {
			direction |= left;
		}
	}
	else if (zComp > 0) {
		direction |= right;
	}
	else {
		direction |= left;
	}

	return direction;
}
