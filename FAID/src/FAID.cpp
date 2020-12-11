#include "FAID.h"
#include "StateData.h"
#include "StartState.h"
#include "RaceState.h"
#include <chrono>
#include <thread>
#include <iostream>


FAID::FAID(GameData gameData, HANDLE process) : gameData(gameData), process(process)
{
	// create states and set initial state
	
	startState = new StartState(&this->gameData);
	raceState = new RaceState(&this->gameData);
	currState = startState;
	pastState = startState;
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
	int numChkPt = gameData.laps * gameData.chkPtsSize;
	int chkPtCount;
	bool gameOver = true;
	StateNumber nextState;
	StateData data;
	State *temp;

	currState->enterState(data);


	while (true) {

		// write button press to memory
		controls = currState->update(nextState);
		WriteProcessMemory(process, (LPVOID)gameData.ctrlAddr, &controls, sizeof(controls), NULL);
		
		// switch state and handle entering and exiting
		if (nextState != StateNumber::CurrentState) {
			data = currState->exitState();
			temp = currState;
			switch (nextState) {
			case StateNumber::StartState:
				currState = startState;
				break;
			case StateNumber::PastState:
				currState = pastState;
				break;
			case StateNumber::RaceState:
				currState = raceState;
				break;
			default:
				break;
			}
			currState->enterState(data);
			pastState = temp;
		}

		// check if race is over
		ReadProcessMemory(process, (LPVOID)gameData.chkPtAddr, &chkPtCount, sizeof(chkPtCount), NULL);
		if ((chkPtCount == numChkPt) || nextState == StateNumber::EndRace) {
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
	}
}
