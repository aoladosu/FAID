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
	char dcprModule[] = "dcpr.dll";
	int chkPointOffsets[] = {0x4, 0x304, 0x48, 0xD4, 0x3C};
	int size = 5;
	uintptr_t dcprbaseAddr = GetModuleBaseAddress(pid, dcprModule);
	uintptr_t baseAddr = dcprbaseAddr + 0x00207AC;
	uintptr_t chkPointMem = followPointer(processHandle, baseAddr, chkPointOffsets, size);

	// get controls
	char awtModule[] = "awt.dll";
	int controlOffsets[] = { 0xC, 0x14, 0x184, 0xE4, 0xC4 };
	uintptr_t awtbaseAddr = GetModuleBaseAddress(pid, awtModule);
	baseAddr = awtbaseAddr + 0x000F94E4;
	uintptr_t controlsMem = followPointer(processHandle, baseAddr, controlOffsets, size);

	// get x address
	int xOffsets[] = { 0xC, 0x14, 0x180, 0x2EC, 0xC };
	uintptr_t xMem = followPointer(processHandle, baseAddr, xOffsets, size);

	// get y address
	int yOffsets[] = { 0x4, 0x304, 0x48, 0xE8, 0xC };
	baseAddr = dcprbaseAddr + 0x00207AC;
	uintptr_t yMem = followPointer(processHandle, baseAddr, yOffsets, size);

	// get stage data
	int stage=1;
	int chkPtSize, obsSize, jumpSize, nlaps;
	int *jumps = 0, *chkPoints = 0, *obstacles = 0;
	countStageData(stage, chkPtSize, jumpSize, obsSize);
	if (getStageData(stage, chkPtSize, chkPoints, jumpSize, jumps, obsSize, obstacles, nlaps) < 0) {
		std::cout << "Couldn't get stage data\n";
	}

	// set up game data
	GameData gameData;
	gameData.process = processHandle;
	gameData.ctrlAddr = controlsMem;
	gameData.chkPtAddr = chkPointMem;
	gameData.xAddr = xMem;
	gameData.yAddr = yMem;
	gameData.stage = stage;
	gameData.laps = nlaps;
	gameData.chkPts = chkPoints;
	gameData.chkPtsSize = chkPtSize;
	gameData.jumpsSize = jumpSize;
	gameData.jumps = jumps;
	gameData.obsSize = obsSize;
	gameData.obstacles = obstacles;

	FAID faid(gameData, processHandle);
	std::cout << "Starting agent\n";
	if (!faid.verifyMemory()) {
		exit(0);
	}
	faid.play();

	free(chkPoints);
	free(jumps);
	free(obstacles);

	return 0;
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