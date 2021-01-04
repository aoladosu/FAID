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
		bool bidirectional = biDirectionalJump(obsPiece);
		onJumpDir = onJumpDirection(obsPiece, obsX, obsY, dirX, dirY, obsAngle, X, Y, bidirectional);
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

	int heightUp, heightDown, widthLeft, widthRight;
	bool defined = false;
	switch (obsPiece) {
	case 26:
		// basic paved ramp
		heightUp = 800;
		heightDown = 800;
		widthLeft = 800;
		widthRight = 800;
		break;
	case 28:
		// double sided paved ramp
		heightUp = 2500;
		heightDown = 2500;
		widthLeft = 900;
		widthRight = 900;
		break;
	case 29:
		// split paved ramp
		heightUp = 2500;
		heightDown = 2500;
		widthLeft = 800;
		widthRight = 800;
		break;
	case 30:
		// landing paved ramp
		heightUp = 1800;
		heightDown = 3400;
		widthLeft = 750;
		widthRight = 750;
		break;
	case 32:
		// small paved ramp
		heightUp = 560;
		heightDown = 520;
		widthLeft = 200;
		widthRight = 200;
		break;
	case 33:
		// dirt speed bump
		dirtSpeedBumpAvoid(A1x, A1y, A2x, A2y, L);
		defined = true;
		break;
	case 34:
		// large offroad ramp
		heightUp = 2000;
		heightDown = 2000;
		widthLeft = 1300;
		widthRight = 1300;
		break;
	case 35:
		// small offroad ramp
		heightUp = 860;
		heightDown = 1000;
		widthLeft = 650;
		widthRight = 650;
		break;
	default:
		std::cout << "An obstacle piece " << obsPiece << " was not defined in avoid state: setGoal\n";
	}

	// define two potential locations (A1, A2) = (left, right, or vice versa)
	// to drive towards to get around the object
	if (!defined) {
		setGoalDestination(A1x, A1y, A2x, A2y, L, heightUp, heightDown, widthLeft, widthRight);
	}

	// set goal to drive towards, pick the one closest to the goal
	int goalDist1 = distSquared(A1x, A1y, X, Y); //distSquared(A1x, A1y, endGoalX, endGoalY)
	int goalDist2 = distSquared(A2x, A2y, X, Y); //distSquared(A2x, A2y, endGoalX, endGoalY)
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

bool AvoidState::biDirectionalJump(int piece)
{
	// check if a jump is bidirectional
	bool biDirectional = false;

	switch (piece) {
	case 28:
	case 29:
	case 33:
	case 34:
	case 35:
		biDirectional = true;
		break;
	}


	return biDirectional;
}

void AvoidState::setGoalDestination(int& A1x, int& A1y, int& A2x, int& A2y, int L, int heightUp, int heightDown, int widthLeft, int widthRight)
{
	// A1x/y A2x/y are possible destinations to go to avoid the object on opposite sides of the obstacle
	// with an angle of 0:
	// heightUp: y offset to be behind the obstacle
	// heightDown: y offset to be infront of the obstacle
	// widthLeft: x offset to be to the left of the obstacle
	// widthRight: x offset to be to the right of the obstacle

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
			A1x = obsX - widthLeft;
			A1y = obsY;
			A2x = obsX + widthRight;
			A2y = obsY;
		}
		else {
			// define L1,L4
			A1x = obsX;
			A1y = obsY + heightUp;
			A2x = obsX;
			A2y = obsY - heightDown;
		}
	}
	else if (obsAngle == 90) {
		if ((L == 1) || (L == 4)) {
			// define L2,L3
			A1x = obsX;
			A1y = obsY - widthLeft;
			A2x = obsX;
			A2y = obsY + widthRight;
		}
		else {
			// define L1,L4
			A1x = obsX - heightUp;
			A1y = obsY;
			A2x = obsX + heightDown;
			A2y = obsY;
		}
	}
	else if (obsAngle == 180) {
		if ((L == 1) || (L == 4)) {
			// define L2,L3
			A1x = obsX + widthLeft;
			A1y = obsY;
			A2x = obsX - widthRight;
			A2y = obsY;
		}
		else {
			// define L1,L4
			A1x = obsX;
			A1y = obsY - heightUp;
			A2x = obsX;
			A2y = obsY + heightDown;
		}
	}
	else {
		if ((L == 1) || (L == 4)) {
			// define L2,L3
			A1x = obsX;
			A1y = obsY + widthLeft;
			A2x = obsX;
			A2y = obsY - widthRight;
		}
		else {
			// define L1,L4
			A1x = obsX + heightUp;
			A1y = obsY;
			A2x = obsX - heightDown;
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
