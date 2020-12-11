#include "RaceState.h"
#include "Util.h"
#include <cmath>
#include <iostream>

RaceState::RaceState(GameData* gameData) : State(gameData){}

int RaceState::update(StateNumber& stateVal)
{
	// check if goal has been reached
	int x, y, z=0;
	ReadProcessMemory(gameData->process, (LPVOID)gameData->xAddr, &x, sizeof(x), NULL);
	ReadProcessMemory(gameData->process, (LPVOID)gameData->yAddr, &y, sizeof(y), NULL);
	//ReadProcessMemory(gameData->process, (LPVOID)gameData->zAddr, &z, sizeof(z), NULL);
	//goalReached = goalReached || reachedPoint(x,y,z, goalX,goalY,goalZ);

	// get a new goal if goal is reached
	setGoal();
	adjustGoal();


	// update variables
	if ((y != oldY) || (x != oldX)){
		dirX = x - oldX;
		dirY = y - oldY;
		oldX = x;
		oldY = y;
		oldZ = z;
	}

	int direction = getDriveDirection();
	stateVal = StateNumber::CurrentState;
	return direction;
}

void RaceState::enterState(StateData stateData)
{
	std::cout << "entering race state\n";

	// copy information passed on from previous state
	oldX = stateData.oldX;
	oldY = stateData.oldY;
	oldZ = stateData.oldZ;

	// update current checkpoint incase another state passed through a checkpoint
	setGoal();
	adjustGoal();

}

StateData RaceState::exitState()
{

	std::cout << "exiting start state\n";

	StateData stateData = StateData();
	stateData.oldX = oldX;
	stateData.oldY = oldY;
	stateData.oldZ = oldZ;
	stateData.goalX = goalX;
	stateData.goalY = goalY;
	stateData.valid = true;

	return stateData;
}

void RaceState::setGoal()
{
	// set the goal of this state to the next checkpoint
	ReadProcessMemory(gameData->process, (LPVOID)gameData->chkPtAddr, &currChkPt, sizeof(currChkPt), NULL);
	int offset = (currChkPt % gameData->chkPtsSize) * 3;
	goalX = gameData->chkPts[offset];
	goalY = gameData->chkPts[offset+1];
	goalZ = 0;
}

void RaceState::adjustGoal()
{
	// adjust the goal if we are coming at it from a bad angle
	// TODO: above
}

int RaceState::getDriveDirection()
{
	// return the direction to go to reach the goal
	int direction = 0;
	int goalDirX = goalX - oldX;
	int goalDirY = goalY - oldY;

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

	// look at dot product to get angle between vectors to decide how hard to turn
	int dot = dirX * (goalDirX) + dirY * (goalDirY);
	double lenDir = sqrt(dirX * dirX + dirY * dirY);
	double lenGoal = sqrt(goalDirX * goalDirX + goalDirY * goalDirY);
	double angle = acos(dot / (lenDir * lenGoal)) * 180 / PI;
	if ( !((angle >= 45) && (angle <=90)) && !((angle <= -45) && (angle >= -90)) ){
		direction |= up;
	}
	
	return direction;
}
