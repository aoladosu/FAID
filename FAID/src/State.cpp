#include "State.h"
#include <cmath>

State::State(GameData* gameData): gameData(gameData){}

State::State(){}

int State::update(StateNumber& stateVal)
{
	return idle;
}

void State::enterState(StateData stateData) {}

StateData State::exitState(){
	return StateData();
}

StateNumber State::nextState() {
	return StateNumber::CurrentState;
}

void State::updateCarInfo()
{
	// update car's position variables/direction
	int newX, newY, newZ = 0;
	ReadProcessMemory(gameData->process, (LPVOID)gameData->xAddr, &newX, sizeof(newX), NULL);
	ReadProcessMemory(gameData->process, (LPVOID)gameData->yAddr, &newY, sizeof(newY), NULL);
	//ReadProcessMemory(gameData->process, (LPVOID)gameData->zAddr, &newZ, sizeof(z), NULL);

	// update variables
	if ((newY != Y) || (newX != X)) {
		dirX = newX - X;
		dirY = newY - Y;
		X = newX;
		Y = newY;
		Z = newZ;
	}
}

int State::getDriveDirection()
{
	// return the direction to go to reach the goal
	float PI = 3.14159;
	int direction = up;
	int goalDirX = goalX - X;
	int goalDirY = goalY - Y;

	// look at dot product to get angle between vectors to decide how hard to turn
	// or to turn at all
	int dot = dirX * (goalDirX)+dirY * (goalDirY);
	float lenDir = sqrt(dirX * dirX + dirY * dirY);
	float lenGoal = sqrt(goalDirX * goalDirX + goalDirY * goalDirY);
	float angle = acos(dot / (lenDir * lenGoal)) * 180 / PI;
	int angleOffset = 9;

	// if we are within 10 degrees of target, then we are fine
	if ((angle < angleOffset) && !isnan(angle)) {
		return up;
	}

	// cross product, get z component to decide turn direction
	int zComp = (goalDirX)*dirY - (goalDirY)*dirX;

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