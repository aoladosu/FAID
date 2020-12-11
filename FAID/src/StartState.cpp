#include "StartState.h"
#include <iostream>



StartState::StartState(GameData *gameData): State(gameData){}

int StartState::update(StateNumber &stateVal)
{
	// change state once 3 seconds have passed
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed = end - timer;

	if (elapsed.count() >= 3) {
		stateVal = StateNumber::RaceState;
	}
	else {
		stateVal = StateNumber::CurrentState;
	}
	return idle;
}

void StartState::enterState(StateData stateData)
{
	std::cout << "entering start state\n";
	timer = std::chrono::steady_clock::now();
}

StateData StartState::exitState()
{
	std::cout << "exiting start state\n";

	StateData data = StateData();
	int x, y, z = 0;
	ReadProcessMemory(gameData->process, (LPVOID)gameData->xAddr, &x, sizeof(x), NULL);
	ReadProcessMemory(gameData->process, (LPVOID)gameData->yAddr, &y, sizeof(y), NULL);
	data.oldX = x;
	data.oldY = y;
	data.oldZ = 0;
	return data;
}
