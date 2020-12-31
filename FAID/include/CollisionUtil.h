#pragma once


// driving
bool reachedPoint(int x, int y, int z, int goalX, int goalY, int goalZ);

// detect collision with stationary objects
bool obsCollision(int *jumps, int jumpSize, int *obstacles, int obSize, int x, int y, int dirX, int dirY, int &object, bool &isJump);

// check if car direction is inline with jump direction
bool onJumpDirection(int jumpX, int jumpY, int dirX, int dirY, int angle, int x, int y, bool bidirectional);

// data helper functions
int distSquared(int x1, int y1, int x2, int y2);
float getAngle(int dirX1, int dirY1, int dirX2, int dirY2);

// intersect function
bool intersect(int piece, int obsX, int obsY, int angle, int x, int y, int dirX, int dirY);