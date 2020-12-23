#include "StartState.h"
#include <iostream>



StartState::StartState(GameData *gameData): State(gameData){}

int StartState::update(StateNumber &stateVal)
{
	stateVal = nextState();
	return idle;
}

void StartState::enterState(StateData stateData)
{
	std::cout << "Entering start state\n";
	timer = std::chrono::steady_clock::now();
}

StateData StartState::exitState()
{
	std::cout << "Exiting start state\n";

	StateData data = StateData();
	int x, y;
	ReadProcessMemory(gameData->process, (LPVOID)gameData->xAddr, &x, sizeof(x), NULL);
	ReadProcessMemory(gameData->process, (LPVOID)gameData->yAddr, &y, sizeof(y), NULL);
	data.X = x;
	data.Y = y;
	data.Z = 0;
	return data;
}

StateNumber StartState::nextState()
{
	// change state once 3 seconds have passed
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed = end - timer;
	StateNumber stateVal = StateNumber::CurrentState;

	if (elapsed.count() >= 3) {
		stateVal = StateNumber::RaceState;
	}

	return stateVal;
}
