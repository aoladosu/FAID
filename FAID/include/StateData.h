#pragma once

enum class StateNumber
{
	CurrentState, StartState, RaceState
};

class StateData 
{
public:
	StateData();

	bool valid = false;
	int goalX = 0;
	int goalY = 0;
	int dirX = 0;
	int dirY = 0;
};