#include "AvoidState.h"
#include "CollisionUtil.h"

#include <iostream>

AvoidState::AvoidState(GameData* gameData) : State(gameData) {}

int AvoidState::update(StateNumber& stateVal)
{

	// update variables
	int newX, newY, newZ = 0;
	ReadProcessMemory(gameData->process, (LPVOID)gameData->xAddr, &newX, sizeof(newX), NULL);
	ReadProcessMemory(gameData->process, (LPVOID)gameData->yAddr, &newY, sizeof(newY), NULL);
	if ((newY != Y) || (newX != X)) {
		dirX = newX - X;
		dirY = newY - Y;
		X = newX;
		Y = newY;
		Z = newZ;
	}

	// change state if no collision, or new object to avoid
	stateVal = nextState();

	int direction = up;
	return direction;
}

void AvoidState::enterState(StateData stateData)
{
	std::cout << "Entering avoid state\n";

	// copy information passed on from previous state
	X = stateData.X;
	Y = stateData.Y;
	Z = stateData.Z;
	endGoalX = stateData.goalX;
	endGoalY = stateData.goalY;
	dirX = stateData.dirX;
	dirY = stateData.dirY;

	// get information for what obstacle to avoid
	obsCollision(gameData->jumps, gameData->jumpsSize, gameData->obstacles, gameData->obsSize, X, Y, dirX, dirY, obstacleNum, isJump);
	if (isJump) {
		obsX = gameData->jumps[obstacleNum + 1];
		obsY = gameData->jumps[obstacleNum + 2];
		obsAngle = gameData->jumps[obstacleNum + 3];
		onJumpDir = onJumpDirection(obsX, obsY, dirX, dirY, obsAngle, X, Y);
	}
	else {
		obsX = gameData->obstacles[obstacleNum + 1];
		obsY = gameData->obstacles[obstacleNum + 2];
		obsAngle = gameData->obstacles[obstacleNum + 3];
	}
}

StateData AvoidState::exitState()
{
	std::cout << "Exiting avoid state\n";

	StateData data = StateData();
	int x, y;
	ReadProcessMemory(gameData->process, (LPVOID)gameData->xAddr, &x, sizeof(x), NULL);
	ReadProcessMemory(gameData->process, (LPVOID)gameData->yAddr, &y, sizeof(y), NULL);
	data.X = x;
	data.Y = y;
	data.Z = 0;
	return data;
}

StateNumber AvoidState::nextState()
{
	// change state if no collision, or new object to avoid
	StateNumber stateVal = StateNumber::CurrentState;
	int newObstacleNum;
	bool newIsJump;
	if (obsCollision(gameData->jumps, gameData->jumpsSize, gameData->obstacles, gameData->obsSize, X, Y, dirX, dirY, newObstacleNum, newIsJump)) {
		if ((obstacleNum != newObstacleNum) || (newIsJump != isJump)) stateVal = StateNumber::PastState;
	}
	else {
		stateVal = StateNumber::PastState;
	}

	return stateVal;
}
