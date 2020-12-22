#include "AvoidState.h"

#include <iostream>

AvoidState::AvoidState(GameData* gameData) : State(gameData) {}

int AvoidState::update(StateNumber& stateVal)
{
	// change state once 3 seconds have passed
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed = end - timer;

	if (elapsed.count() >= 2) {
		stateVal = StateNumber::RaceState;
	}
	else {
		stateVal = StateNumber::CurrentState;
	}
	return up|right;
}

void AvoidState::enterState(StateData stateData)
{
	std::cout << "Entering avoid state\n";

	// copy information passed on from previous state
	X = stateData.X;
	Y = stateData.Y;
	Z = stateData.Z;
	goalX = stateData.goalX;
	goalY = stateData.goalY;


	timer = std::chrono::steady_clock::now();
}

StateData AvoidState::exitState()
{
	std::cout << "Exiting avoid state\n";

	StateData data = StateData();
	int x, y;
	ReadProcessMemory(gameData->process, (LPVOID)gameData->xAddr, &x, sizeof(x), NULL);
	ReadProcessMemory(gameData->process, (LPVOID)gameData->yAddr, &y, sizeof(y), NULL);
	data.X = x;
	data.Y = y;
	data.Z = 0;
	return data;
}
