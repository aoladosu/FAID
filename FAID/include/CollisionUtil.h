#pragma once


// driving
bool reachedPoint(int x, int y, int z, int goalX, int goalY, int goalZ);

// detect collision with stationary objects
bool obsCollision(int *jumps, int jumpSize, int *obstacles, int obSize, int x, int y, int &object, bool &isJump);
int distSquared(int x1, int y1, int x2, int y2);
