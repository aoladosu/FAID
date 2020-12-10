#include <iostream>
#include <Util.h>
#include "GameData.h"
#include "FAID.h"

int main()
{
	std::cout << "Hello World!\n";

	
	// get handle to window
	HWND wndHandle = FindWindowA(NULL, "Need for Madness");
	if (wndHandle == NULL) {
		std::cout << "Unable to find window.";
		exit(0);
	}

	// get process id and handle and set access
	DWORD pid;
	GetWindowThreadProcessId(wndHandle, &pid);
	HANDLE processHandle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
	if (processHandle == NULL) {
		std::cout << "Unable to get process handle.";
		exit(0);
	}

	// get memory locations

	// get checkpoint
	char chkPointModule[] = "dcpr.dll";
	int chkPointOffsets[] = {0x4, 0x304, 0x48, 0xD4, 0x3C};
	int size = 5;
	uintptr_t baseAddr = GetModuleBaseAddress(pid, chkPointModule);
	baseAddr = baseAddr + 0x00207AC;
	uintptr_t chkPointMem = followPointer(processHandle, baseAddr, chkPointOffsets, size);

	// get controls
	char controlModule[] = "awt.dll";
	int controlOffsets[] = { 0xC, 0x14, 0x184, 0xE4, 0xC4 };
	baseAddr = GetModuleBaseAddress(pid, controlModule);
	baseAddr = baseAddr + 0x000F94E4;
	uintptr_t controlsMem = followPointer(processHandle, baseAddr, controlOffsets, size);

	// get stage data
	int *chkPoints=0, stage=1;
	countStageData(stage, size);
	if (getStageData(stage, size, chkPoints) < 0) {
		std::cout << "Couldn't get stage data\n";
	}

	// set up game data
	GameData gameData;
	gameData.chkPts = chkPoints;
	gameData.chkPtsSize = size;
	gameData.ctrlAddr = controlsMem;
	gameData.stage = stage;

	FAID faid(gameData, processHandle);
	std::cout << "Starting agent\n";
	faid.play();

	free(chkPoints);
}






/*
key press

#include <Winuser.h>

	INPUT inputs[2];
	ZeroMemory(inputs, sizeof(inputs));

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = VK_UP;

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = VK_UP;
	inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

	UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

*/