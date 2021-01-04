#include "CollisionUtil.h"

#include <stdlib.h>
#include <iostream>

bool reachedPoint(int x, int y, int z, int goalX, int goalY, int goalZ)
{
	// check if goal is reached within some tolerance
	int tolZ = 50;
	int tolerance = 700*700;

	int dist = (goalX - x) * (goalX - x) + (goalY - y) * (goalY - y);
	int distZ = abs(goalZ - z);

	return (distZ <= tolZ) && (dist <= tolerance);
}

bool obsCollision(int *jumps, int jumpSize, int *obstacles, int obsSize, int x, int y, int dirX, int dirY, int &object, bool &isJump)
{
	// return true if a collision with a stationary object is coming
	// return number of object in object and whether that object is a jump or obstacle in isJump

	bool collision = false;
	int radDist = 4000*4000, obsDist;		// r^2: maximum distance we can see forward
	int xPos, yPos, sightAngle = 30;

	// check if object is within our sight radius and in front of us (angle checks second cond)
	for (int i = 0; i < jumpSize * 4; i+=4) {
		xPos = jumps[i + 1];
		yPos = jumps[i + 2];
		obsDist = distSquared(x, y, xPos, yPos);
		if ((obsDist < radDist) && (getAngle(dirX, dirY, xPos-x, yPos-y) < sightAngle)) {
			if (intersect(jumps[i], xPos, yPos, obstacles[i + 3], x, y, dirX, dirY)) {
				radDist = obsDist;
				object = i;
				collision = true;
				isJump = true;
			}
		}
	}
	
	// same as for jumps
	for (int i = 0; i < obsSize * 4; i+=4) {
		xPos = obstacles[i + 1];
		yPos = obstacles[i + 2];
		obsDist = distSquared(x, y, xPos, yPos);
		if ((obsDist < radDist) && (getAngle(dirX, dirY, xPos - x, yPos - y) < sightAngle)) {
			if (intersect(obstacles[i], xPos, yPos, obstacles[i+3], x, y, dirX, dirY)) {
				radDist = obsDist;
				object = i;
				collision = true;
				isJump = false;
			}
		}
	}
	
	return collision;
}

bool onJumpDirection(int piece, int jumpX, int jumpY, int dirX, int dirY, int angle, int x, int y, bool bidirectional)
{
	// check if the car is oriented such that it can jump off the ramp instead of crashing into it
	// angles increase counter clockwise

	// jump direction vector
	int jumpDirX = 0, jumpDirY = -1;
	switch (angle) {
	case 90:
		jumpDirX = 1;
		jumpDirY = 0;
		break;
	case 180:
		jumpDirX = 0;
		jumpDirY = 1;
		break;
	case 270:
		jumpDirX = -1;
		jumpDirY = 0;
		break;
	}
	if (angle < 0) {
		jumpDirX *= -1;
		jumpDirY *= -1;
	}

	int angleOffset = 45;
	bool onJump = (getAngle(x - jumpX, y - jumpY, jumpDirX, jumpDirY) <= angleOffset) && notPastJump(piece, angle, x, y, jumpX, jumpY, bidirectional);
	if (bidirectional) {
		jumpDirX *= -1;
		jumpDirY *= -1;
		onJump = onJump || getAngle(x - jumpX, y - jumpY, jumpDirX, jumpDirY) <= angleOffset;
	}
	
	return onJump;
}

bool notPastJump(int piece, int angle, int x, int y, int jumpX, int jumpY, bool bidirectional)
{
	// check that if the car is in front of the jump by 45 degrees,
	// it is not already past the jump entrance

	int heightDown;
	bool canJump = false;

	switch (piece)
	{
	case 26:
		heightDown = 700;
		break;
	case 28:
		heightDown = 2423;
		break;
	case 29:
		heightDown = 2300;
		break;
	case 30:
		heightDown = 3374;
		break;
	case 32:
		heightDown = 508;
		break;
	case 33:
		heightDown = 500;
		break;
	case 34:
		heightDown = 1900;
		break;
	case 35:
		heightDown = 957;
		break;
	default:
		std::cout << "An obstacle piece " << piece << " was not defined in collision util: notPastJump\n";
	}

	if ((angle == 0) || ((angle == 180) && (bidirectional))) {
		if (y < jumpY - heightDown) canJump = true;
	}
	if ((angle == 90) || ((angle == 270) && (bidirectional))) {
		if (x < jumpX - heightDown) canJump = true;
	}
	if ((angle == 180) || ((angle == 0) && (bidirectional))) {
		if (y > jumpY + heightDown) canJump = true;
	}
	if ((angle == 270) || ((angle == 90) && (bidirectional))) {
		if (x > jumpX + heightDown) canJump = true;
	}

	return canJump;
}

int distSquared(int x1, int y1, int x2, int y2)
{
	// get squared distance between two points
	return ((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1));
}

float getAngle(int dirX1, int dirY1, int dirX2, int dirY2) {
	// get angle between two vectors

	float PI = 3.14159;
	int dot = dirX1 * (dirX2) + dirY1 * (dirY2);
	float lenDir = sqrt(dirX1 * dirX1 + dirY1 * dirY1);
	float lenGoal = sqrt(dirX2 * dirX2 + dirY2 * dirY2);
	float angle = acos(dot / (lenDir * lenGoal)) * 180 / PI;
	return angle;
}

bool intersect(int piece, int obsX, int obsY, int angle, int x, int y, int dirX, int dirY)
{
	int points[8], heightUp, heightDown, widthLeft, widthRight;
	switch (piece) {
	case 26:
		// basic paved ramp
		widthLeft = 700;
		widthRight = 700;
		heightUp = 700;
		heightDown = 700;
		break;
	case 28:
		// double sided paved ramp
		widthLeft = 710;
		widthRight = 710;
		heightUp = 2440;
		heightDown = 2440;
		break;
	case 29:
		// split paved ramp
		widthLeft = 750;
		widthRight = 750;
		heightUp = 2350;
		heightDown = 2350;
		break;
	case 30:
		// landing paved ramp
		widthLeft = 750;
		widthRight = 750;
		heightUp = 1700;
		heightDown = 3374;
		break;
	case 32:
		// small paved ramp
		widthLeft = 170;
		widthRight = 170;
		heightUp = 540;
		heightDown = 540;
		break;
	case 33:
		// dirt speed bumb
		widthLeft = 675;
		widthRight = 675;
		heightUp = 500;
		heightDown = 500;
		break;
	case 34:
		// large offroad ramp
		widthLeft = 1300;
		widthRight = 1300;
		heightUp = 2000;
		heightDown = 2000;
		break;
	case 35:
		// small offroad ramp
		widthLeft = 650;
		widthRight = 650;
		heightUp = 960;
		heightDown = 960;
		break;
	default:
		std::cout << "An obstacle piece " << piece << " was not defined in collision util: intersect\n";
	}

	// test intersect
	definePoints(points, heightUp, heightDown, widthLeft, widthRight, obsX, obsY, angle);
	return intersectTest(points, x, y, dirX, dirY);
}


bool intersectTest(int points[8], int x, int y, int dirX, int dirY)
{
	bool intersect = false;
	int p1, p2, p3, p4;

	if (dirX != 0) {
		// define b for y=mx+b
		int b = y - (dirY / dirX) * x;

		// calculate intersect
		// deltaX*Y - deltaY*X - deltaX*b = 0
		p1 = dirX * points[1] - dirY * points[0] - dirX * b;
		p2 = dirX * points[3] - dirY * points[2] - dirX * b;
		p3 = dirX * points[5] - dirY * points[4] - dirX * b;
		p4 = dirX * points[7] - dirY * points[6] - dirX * b;
	}
	else {
		p1 = points[0];
		p2 = points[2];
		p3 = points[4];
		p4 = points[6];
	}

	// if all of them have the same sign, there is no intersect else there is
	if (!((p1 < 0) && (p2 < 0) && (p3 < 0) && (p4 < 0)) && !((p1 > 0) && (p2 > 0) && (p3 > 0) && (p4 > 0))) {
		intersect = true;
	}

	return intersect;
}

void definePoints(int points[8], int heightUp, int heightDown, int widthLeft, int widthRight, int obsX, int obsY, int angle)
{
	// define four corners of object
	if (angle == 0) {
		points[0] = obsX - widthLeft;
		points[1] = obsY + heightUp;
		points[2] = obsX + widthRight;
		points[3] = obsY + heightUp;
		points[4] = obsX + widthRight;
		points[5] = obsY - heightDown;
		points[6] = obsX - widthLeft;
		points[7] = obsY - heightDown;
	}
	else if (angle == 90) {
		points[0] = obsX - heightUp;
		points[1] = obsY - widthLeft;
		points[2] = obsX - heightUp;
		points[3] = obsY + widthRight;
		points[4] = obsX + heightDown;
		points[5] = obsY + widthRight;
		points[6] = obsX + heightDown;
		points[7] = obsY - widthLeft;
	}
	else if (angle == 180) {
		points[0] = obsX - widthRight;
		points[1] = obsY + heightDown;
		points[2] = obsX - widthRight;
		points[3] = obsY - heightUp;
		points[4] = obsX + widthLeft;
		points[5] = obsY + heightDown;
		points[6] = obsX + widthLeft;
		points[7] = obsY - heightUp;
	}
	else {
		points[0] = obsX - heightDown;
		points[1] = obsY - widthRight;
		points[2] = obsX - heightDown;
		points[3] = obsY + widthLeft;
		points[4] = obsX + heightUp;
		points[5] = obsY + widthLeft;
		points[6] = obsX + heightUp;
		points[7] = obsY - widthRight;
	}
}

