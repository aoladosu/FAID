#pragma once

enum class StateNumber
{
	CurrentState, StartState, RaceState, PastState, EndRace
};

class StateData 
{
public:
	StateData();

	bool valid = false;
	int goalX = 0;
	int goalY = 0;
	int oldX = 0;
	int oldY = 0;
	int oldZ = 0;
};