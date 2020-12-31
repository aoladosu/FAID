#include "AvoidState.h"
#include "CollisionUtil.h"

#include <iostream>

AvoidState::AvoidState(GameData* gameData) : State(gameData) {}

int AvoidState::update(StateNumber& stateVal)
{
	// update car variables
	updateCarInfo();

	// change state if no collision, or new object to avoid
	stateVal = nextState();

	int direction = getDriveDirection();
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
		obsPiece = gameData->jumps[obstacleNum];
		obsX = gameData->jumps[obstacleNum + 1];
		obsY = gameData->jumps[obstacleNum + 2];
		obsAngle = gameData->jumps[obstacleNum + 3];
		bool bidirectional = gameData->jumps[obstacleNum] == 33;
		onJumpDir = onJumpDirection(obsX, obsY, dirX, dirY, obsAngle, X, Y, bidirectional);
	}
	else {
		obsPiece = gameData->obstacles[obstacleNum];
		obsX = gameData->obstacles[obstacleNum + 1];
		obsY = gameData->obstacles[obstacleNum + 2];
		obsAngle = gameData->obstacles[obstacleNum + 3];
	}

	// set drive location
	setGoal();
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

void AvoidState::setGoal() {
	// find position to drive towards to avoid obstacle

	// get  vector for jump
	int jumpX = 0, jumpY = 1;
	switch (obsAngle) {
	case 90:
		jumpX = -1;
		jumpY = 0;
		break;
	case 180:
		jumpX = 0;
		jumpY = -1;
		break;
	case 270:
		jumpX = 1;
		jumpY = 0;
		break;
	}
	if (obsAngle < 0) {
		jumpX *= -1;
		jumpY *= -1;
	}


	// define vectors for dot product
	float PI = 3.14159;
	int jumpDirX = obsX + jumpX;
	int jumpDirY = obsY + jumpY;
	int carDirX = X - obsX;
	int carDirY = Y - obsX;

	// calculate angle car is approaching object from
	int dot = carDirX * (jumpDirX)+ carDirY * (jumpDirY);
	float lenDir = sqrt(carDirX * carDirX + carDirY * carDirY);
	float lenGoal = sqrt(jumpDirX * jumpDirX + jumpDirY * jumpDirY);
	float angle = acos(dot / (lenDir * lenGoal)) * 180 / PI;
	int zComp = (carDirX)*jumpDirY - (carDirY)*jumpDirX;


	int A1x, A1y, A2x, A2y, L;
	// the angle tells us in what quadrant around the obstacle we are: L1,L2,L3,L4
	if (angle <= 45) L = 1;
	else if ((angle <= 135) && (zComp < 0)) L = 2;
	else if (angle <= 135) L = 3;
	else L = 4;

	// define two potential locations (A1, A2) = (left, right, or vice versa)
	// to drive towards to get around the object
	// call function that matches piece type
	switch (obsPiece) {
	case 26:
		standardPavedRampAvoid(A1x, A1y, A2x, A2y, L);
		break;
	case 33:
		dirtSpeedBumpAvoid(A1x, A1y, A2x, A2y, L);
		break;
	default:
		std::cout << "An obstacle piece " << obsPiece << " was not defined\n";
	}

	// set goal to drive towards, pick the one closest to the goal
	int goalDist1 = distSquared(A1x, A1y, endGoalX, endGoalY) + distSquared(A1x, A1y, X, Y);
	int goalDist2 = distSquared(A2x, A2y, endGoalX, endGoalY) + distSquared(A2x, A2y, X, Y);
	if (goalDist1 < goalDist2) {
		goalX = A1x;
		goalY = A1y;
		goalZ = 0;
	}
	else {
		goalX = A2x;
		goalY = A2y;
		goalZ = 0;
	}

}

void AvoidState::standardPavedRampAvoid(int &A1x, int &A1y, int &A2x, int &A2y, int L) {
	// code: 26
	// basic paved ramp

	int a1 = 800, a2 = 500, b1 = 700, b2 = 700;

	if (onJumpDir) {
		// just go towards goal
		A1x = endGoalX;
		A2x = endGoalX;
		A1y = endGoalY;
		A2y = endGoalY;
		return;
	}

	if (obsAngle == 0) {
		if ((L == 1) || (L == 4)) {
			// define L2,L3
			A1x = obsX - b1;
			A1y = obsY;
			A2x = obsX + b2;
			A2y = obsY;
		}
		else {
			// define L1,L4
			A1x = obsX;
			A1y = obsY + a1;
			A2x = obsX;
			A2y = obsY - a2;
		}
	}
	else if (obsAngle == 90) {
		if ((L == 1) || (L == 4)) {
			// define L2,L3
			A1x = obsX;
			A1y = obsY - b1;
			A2x = obsX;
			A2y = obsY + b2;
		}
		else {
			// define L1,L4
			A1x = obsX - a1;
			A1y = obsY;
			A2x = obsX + a2;
			A2y = obsY;
		}
	}
	else if (obsAngle == 180) {
		if ((L == 1) || (L == 4)) {
			// define L2,L3
			A1x = obsX + b1;
			A1y = obsY;
			A2x = obsX - b2;
			A2y = obsY;
		}
		else {
			// define L1,L4
			A1x = obsX;
			A1y = obsY - a1;
			A2x = obsX;
			A2y = obsY + a2;
		}
	}
	else {
		if ((L == 1) || (L == 4)) {
			// define L2,L3
			A1x = obsX;
			A1y = obsY + b1;
			A2x = obsX;
			A2y = obsY - b2;
		}
		else {
			// define L1,L4
			A1x = obsX + a1;
			A1y = obsY;
			A2x = obsX - a2;
			A2y = obsY;
		}
	}

}

void AvoidState::dirtSpeedBumpAvoid(int &A1x, int &A1y, int &A2x, int &A2y, int L) {
	// code: 33
	// dirt speed bumb

	// just go towards goal
	A1x = endGoalX;
	A2x = endGoalX;
	A1y = endGoalY;
	A2y = endGoalY;
}