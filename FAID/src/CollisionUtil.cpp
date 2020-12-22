#include "CollisionUtil.h"

#include <stdlib.h>
#include <iostream>

// driving
bool reachedPoint(int x, int y, int z, int goalX, int goalY, int goalZ)
{
	// check if goal is reached within some tolerance
	int tolZ = 50;
	int tolerance = 700*700;

	int dist = (goalX - x) * (goalX - x) + (goalY - y) * (goalY - y);
	int distZ = abs(goalZ - z);

	return (distZ <= tolZ) && (dist <= tolerance);
}

bool obsCollision(int *jumps, int jumpSize, int *obstacles, int obsSize, int x, int y, int &object, bool &isJump)
{
	// return true if a collision with a stationary object is coming
	// return number of object in object and whether that object is a jump or obstacle in isJump

	bool collision = false;
	int radDist = 1500*1500, obsDist;		// r^2: maximum distance we can see forward
	int xPos, yPos;

	for (int i = 0; i < jumpSize * 4; i+=4) {
		xPos = jumps[i + 1];
		yPos = jumps[i + 2];
		obsDist = distSquared(x, y, xPos, yPos);
		if (obsDist < radDist) {
			// something within our radius of sight
			radDist = obsDist;
			// TODO: refine this, not true if we all ready passed it
			object = i;
			collision = true;
			isJump = true;
		}
	}

	for (int i = 0; i < obsSize * 4; i+=4) {
		xPos = obstacles[i + 1];
		yPos = obstacles[i + 2];
		obsDist = distSquared(x, y, xPos, yPos);
		if (obsDist < radDist) {
			// something within our radius of sight
			radDist = obsDist;
			// TODO: refine this, not true if we all ready passed it
			object = i;
			collision = true;
			isJump = false;
		}
	}
	
	return collision;
}

int distSquared(int x1, int y1, int x2, int y2)
{
	return ((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1));
}




