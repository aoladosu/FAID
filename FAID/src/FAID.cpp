#include "FAID.h"
#include "StateData.h"
#include "StartState.h"
#include "RaceState.h"
#include "AvoidState.h"
#include <chrono>
#include <thread>
#include <iostream>


FAID::FAID(GameData gameData, HANDLE process) : gameData(gameData), process(process)
{
	// create states and set initial state
	
	startState = new StartState(&this->gameData);
	raceState = new RaceState(&this->gameData);
	avoidState = new AvoidState(&this->gameData);
	currState = startState;
	pastState = startState;
}

FAID::~FAID()
{
	delete startState;
	delete raceState;
	delete avoidState;
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
			case StateNumber::AvoidState:
				currState = avoidState;
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

bool FAID::verifyMemory()
{
	// checks that all memory can be read from and written to
	int test;
	bool noError = true;

	// read tests
	if (!ReadProcessMemory(process, (LPVOID)gameData.xAddr, &test, sizeof(test), NULL)) {
		memoryErrorPrint(true, gameData.xAddr, "x");
		noError = false;
	}
	if (!ReadProcessMemory(process, (LPVOID)gameData.yAddr, &test, sizeof(test), NULL)) {
		memoryErrorPrint(true, gameData.yAddr, "y");
		noError = false;
	}
	/*
	if (!ReadProcessMemory(gameData.process, (LPVOID)gameData.zAddr, &test, sizeof(test), NULL)) {
		memoryErrorPrint(true, gameData.zAddr, "z");
		noError = false;
	}
	if (!ReadProcessMemory(process, (LPVOID)gameData.enemyPosAddr, &test, sizeof(test), NULL)) {
		memoryErrorPrint(true, gameData.enemyPosAddr, "EnemyPosition");
		noError = false;
	}
	*/
	if (!ReadProcessMemory(process, (LPVOID)gameData.chkPtAddr, &test, sizeof(test), NULL)) {
		memoryErrorPrint(true, gameData.chkPtAddr, "Checkpoint");
		noError = false;
	}

	// write tests
	test = 0;
	if (!WriteProcessMemory(process, (LPVOID)gameData.ctrlAddr, &test, sizeof(test), NULL)) {
		memoryErrorPrint(false, gameData.ctrlAddr, "Control");
		noError = false;
	}


	if (!noError) {
		std::cout << "Try restarting the level or restarting the game.\n";
	}
	return noError;
}

void FAID::memoryErrorPrint(bool read, uintptr_t addr, std::string addrName) {
	if (read) {
		std::cout << "FAID is unable to read from " << addrName << " address: "  << std::hex << addr << "\n";
	}
	else {
		std::cout << "FAID is unable to wrtie to " << addrName << " address: " << std::hex << addr << "\n";
	}
}
