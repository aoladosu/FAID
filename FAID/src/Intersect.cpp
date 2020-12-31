#include "Intersect.h"

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
	if ( !((p1 < 0) && (p2 < 0) && (p3 < 0) && (p4 < 0))  && !((p1 > 0) && (p2 > 0) && (p3 > 0) && (p4 > 0)) ){
		intersect = true;
	}

	return intersect;
}

bool standardPavedRampIntersect(int obsX, int obsY, int angle, int x, int y, int dirX, int dirY)
{
	// code: 26
	// basic paved ramp

	// define four corners
	// approximate the ramp to be a square, so angle doesn't matter here
	int p1X, p1Y, p2X, p2Y, p3X, p3Y, p4X, p4Y;
	int size = 700;
	p1X = obsX - size;
	p1Y = obsY + size;
	p2X = obsX + size;
	p2Y = obsY + size;
	p3X = obsX + size;
	p3Y = obsY - size;
	p4X = obsX - size;
	p4Y = obsY - size;

	// gather points in array
	int points[] = { p1X, p1Y, p2X, p2Y, p3X, p3Y, p4X, p4Y };

	// intersect test to check for ray intersect
	return intersectTest(points, x, y, dirX, dirY);
}

bool dirtSpeedBumpIntersect(int obsX, int obsY, int angle, int x, int y, int dirX, int dirY)
{
	// code: 33
	// dirt speed bumb

	int p1X, p1Y, p2X, p2Y, p3X, p3Y, p4X, p4Y;
	int width = 675, height = 500;

	// 2 orientations are identical
	if ((angle == 0) || (angle == 180)) {
		p1X = obsX - width;
		p1Y = obsY + height;
		p2X = obsX + width;
		p2Y = obsY + height;
		p3X = obsX + width;
		p3Y = obsY - height;
		p4X = obsX - width;
		p4Y = obsY - height;
	}
	else {
		p1X = obsX - height;
		p1Y = obsY - width;
		p2X = obsX - height;
		p2Y = obsY + width;
		p3X = obsX + height;
		p3Y = obsY - width;
		p4X = obsX + height;
		p4Y = obsY - width;
	}

	// gather points in array
	int points[] = { p1X, p1Y, p2X, p2Y, p3X, p3Y, p4X, p4Y };

	// intersect test to check for ray intersect
	return intersectTest(points, x, y, dirX, dirY);
}
