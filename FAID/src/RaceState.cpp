#include "RaceState.h"

RaceState::RaceState(GameData* gameData) : State(gameData){}

int RaceState::update(StateNumber& stateVal)
{
	stateVal = StateNumber::CurrentState;
	return 0x10000;
}

void RaceState::enterState(StateData stateData)
{
}

StateData RaceState::exitState()
{
	return StateData();
}
