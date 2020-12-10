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
	return 0;
}

void StartState::enterState(StateData stateData)
{
	timer = std::chrono::steady_clock::now();
}

StateData StartState::exitState()
{
	return StateData();
}
