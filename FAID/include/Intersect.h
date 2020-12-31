#pragma once

// plucker test
bool intersectTest(int points[8], int x, int y, int dirX, int dirY);

// ray tracing functions
bool standardPavedRampIntersect(int obsX, int obsY, int angle, int x, int y, int dirX, int dirY);
bool dirtSpeedBumpIntersect(int obsX, int obsY, int angle, int x, int y, int dirX, int dirY);