#include "State.h"

State::State(GameData* gameData): gameData(gameData){}

State::State(){}

int State::update(StateNumber& stateVal)
{
	return idle;
}

void State::enterState(StateData stateData) {}

StateData State::exitState(){
	return StateData();
}
