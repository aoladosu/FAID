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
	int radDist = 3000*3000, obsDist;		// r^2: maximum distance we can see forward
	int xPos, yPos, sightAngle = 30;

	// TODO: ray tracing


	// check if object is within our sight radius and in front of us (angle checks second cond)
	for (int i = 0; i < jumpSize * 4; i+=4) {
		xPos = jumps[i + 1];
		yPos = jumps[i + 2];
		obsDist = distSquared(x, y, xPos, yPos);
		if ((obsDist < radDist) && (getAngle(dirX, dirY, xPos-x, yPos-y) < sightAngle)) {
			radDist = obsDist;
			object = i;
			collision = true;
			isJump = true;
		}
	}
	
	// same as for jumps
	for (int i = 0; i < obsSize * 4; i+=4) {
		xPos = obstacles[i + 1];
		yPos = obstacles[i + 2];
		obsDist = distSquared(x, y, xPos, yPos);
		if ((obsDist < radDist) && (getAngle(dirX, dirY, xPos - x, yPos - y) < sightAngle)) {
			radDist = obsDist;
			object = i;
			collision = true;
			isJump = false;
		}
	}
	
	return collision;
}

bool onJumpDirection(int jumpX, int jumpY, int dirX, int dirY, int angle, int x, int y, bool bidirectional)
{
	// check if the car is oriented such that it can jump off the ramp instead of crashing into it
	// angles increase counter clockwise

	// jump direction vector
	int jumpDirX = 0, jumpDirY = 1;
	switch (angle) {
	case 90:
		jumpDirX = -1;
		jumpDirY = 0;
		break;
	case 180:
		jumpDirX = 0;
		jumpDirY = -1;
		break;
	case 270:
		jumpDirX = 1;
		jumpDirY = 0;
		break;
	}
	if (angle < 0) {
		jumpDirX *= -1;
		jumpDirY *= -1;
	}

	int angleOffset = 45;
	bool onJump = getAngle(dirX, dirY, jumpDirX+x, jumpDirY+y) <= angleOffset;
	if (bidirectional) {
		jumpDirX *= -1;
		jumpDirY *= -1;
		onJump = onJump || getAngle(dirX, dirY, jumpDirX + x, jumpDirY + y) <= angleOffset;
	}
	
	return false;
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



