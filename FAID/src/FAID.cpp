#include "FAID.h"
#include "StateData.h"
#include "StartState.h"
#include "RaceState.h"
#include <chrono>
#include <thread>


FAID::FAID(GameData gameData, HANDLE process) : gameData(gameData), process(process)
{
	// create states and set initial state
	
	startState = new StartState(&this->gameData);
	raceState = new RaceState(&this->gameData);
	currState = startState;
}

FAID::~FAID()
{
	delete startState;
	delete raceState;
}

void FAID::play() {
	// execute each state 
	// write control to memory
	// change state as necesary

	int controls;
	bool gameOver = false;
	StateNumber nextState;
	StateData data;

	currState->enterState(data);


	while (true) {

		// write button press to memory
		controls = currState->update(nextState);
		int h = WriteProcessMemory(process, (LPVOID)gameData.ctrlAddr, &controls, sizeof(controls), NULL);
		
		// switch state and handle entering and exiting
		if (nextState != StateNumber::CurrentState) {
			data = currState->exitState();
			switch (nextState) {
			case StateNumber::StartState:
				currState = startState;
				break;
			case StateNumber::RaceState:
				currState = raceState;
				break;
			default:
				break;
			}
			currState->enterState(data);
		}

		// check if race is over
		if (gameOver) {
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(4));
	}
}
