#pragma once

enum class StateNumber
{
	CurrentState, StartState, RaceState, AvoidState, PastState, EndRace
};

class StateData 
{
public:
	StateData();

	bool valid = false;
	int goalX = 0;
	int goalY = 0;
	int X = 0;
	int Y = 0;
	int Z = 0;
};