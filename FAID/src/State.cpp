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

StateNumber State::nextState() {
	return StateNumber::CurrentState;
}

void State::updateCarInfo()
{
	// update car's position variables/direction
	int newX, newY, newZ = 0;
	ReadProcessMemory(gameData->process, (LPVOID)gameData->xAddr, &newX, sizeof(newX), NULL);
	ReadProcessMemory(gameData->process, (LPVOID)gameData->yAddr, &newY, sizeof(newY), NULL);
	//ReadProcessMemory(gameData->process, (LPVOID)gameData->zAddr, &newZ, sizeof(z), NULL);

	// update variables
	if ((newY != Y) || (newX != X)) {
		dirX = newX - X;
		dirY = newY - Y;
		X = newX;
		Y = newY;
		Z = newZ;
	}
}
