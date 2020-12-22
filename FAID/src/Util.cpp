#include "Util.h"

#include <WinUser.h>
#include <processthreadsapi.h>
#include <tlhelp32.h>
#include <comdef.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

constexpr int JUMP = 1;
constexpr int OBS = 2;

// stage parsing
void countStageData(int stage, int &chkSize, int &jumpSize, int &obsSize)
{
	// count number of instances of things in the stage
	
	std::ifstream file("stages/" + std::to_string(stage) + ".txt");
	std::string line, chkPtStr = "chk", setStr = "set";
	int chkPtCount = 0, jumpCount = 0, obsCount = 0;
	char c;
	int piece, relPiece;

	if (!file.good()) {
		return;
	}

	while (std::getline(file, line)) {

		// find checkpoints
		if (line.find(chkPtStr) != std::string::npos) chkPtCount++;

		// find important pieces - jumps/obstacles
		if (line.find(setStr) != std::string::npos) {
			std::istringstream iss(line);
			iss >> c >> c >> c >> c >> piece;
			relPiece = relevantPiece(piece);
			if (relPiece != 0) {
				if (relPiece == JUMP) jumpCount++;
				else obsCount++;
			}
		}
	}

	chkSize = chkPtCount;
	jumpSize = jumpCount;
	obsSize = obsCount;
	file.close();
}

int getStageData(int stage, int chkSize, int *&chkPoints, int jumpSize, int *&jumps, int obsSize, int *&obstacles, int &nlaps)
{
	// read file to get stage data into malloced array

	std::ifstream file("stages/" + std::to_string(stage) + ".txt");
	std::string line, chkPtStr = "chk", lapStr = "nlaps", setStr = "set";

	if (!file.good()) {
		return -1;
	}

	// 3 because x,y,angle will be packed next to each other
	// 4 for size of int
	// +1 for edge case
	chkPoints = (int*)malloc((chkSize + 1) * 3 * 4);
	if (chkPoints == nullptr) {
		return -1;
	}

	// 4 because piece,x,y,angle will be packed next to each other
	// 4 for size of int
	// +1 for edge case
	jumps = (int*)malloc((jumpSize + 1) * 4 * 4);
	if (jumps == nullptr) {
		return -1;
	}

	// same as above
	obstacles = (int*)malloc((obsSize + 1) * 4 * 4);
	if (obstacles == nullptr) {
		return -1;
	}

	int piece, x, y, angle, relPiece;
	int	chkPos = 0, jumpPos = 0, obsPos = 0;
	char c;
	while (std::getline(file, line)) {

		// get checkpoint data
		if (line.find(chkPtStr) != std::string::npos) {
			std::istringstream iss(line);
			iss >> c >> c >> c >> c >> piece >> c >> x >> c >> y >> c >> angle;
			chkPoints[chkPos] = x;
			chkPoints[chkPos + 1] = y;
			chkPoints[chkPos + 2] = angle;
			chkPos += 3;
		}

		// get number of laps
		if (line.find(lapStr) != std::string::npos) {
			std::istringstream iss(line);
			iss >> c >> c >> c >> c  >> c >> c >> nlaps;
		}

		// get jumps/obstacle data
		if (line.find(setStr) != std::string::npos) {
			std::istringstream iss(line);
			iss >> c >> c >> c >> c >> piece >> c >> x >> c >> y >> c >> angle;
			relPiece = relevantPiece(piece);
			if (relPiece != 0) {
				if (relPiece == JUMP) {
					jumps[jumpPos] = piece;
					jumps[jumpPos + 1] = x;
					jumps[jumpPos + 2] = y;
					jumps[jumpPos + 3] = angle;
					jumpPos += 4;
				}
				else {
					obstacles[obsPos] = piece;
					obstacles[obsPos + 1] = x;
					obstacles[obsPos + 2] = y;
					obstacles[obsPos + 3] = angle;
					obsPos += 4;
				}
			}
		}
	}

	// pad with 0's
	for (int i = chkPos; i < chkPos + 3; i++) {
		chkPoints[i] = 0;
	}
	for (int i = jumpPos; i < jumpPos + 4; i++) {
		jumps[i] = 0;
	}
	for (int i = obsPos; i < obsPos + 4; i++) {
		obstacles[i] = 0;
	}

	file.close();
	return 0;
}

int relevantPiece(int piece)
{
	// return true if a piece in a stage is relevant to
	// filters out roads and such

	int pieceType = 0;

	switch (piece)
	{
	case 26:
		pieceType = JUMP;
		break;
	case 33:
		pieceType = JUMP;
		break;
	default:
		pieceType = 0;
		break;
	}

	return pieceType;
}

// memory
uintptr_t GetModuleBaseAddress(DWORD pid, char* moduleName) {
	// get the base address for the module

	uintptr_t baseAddress = 0;

	// get list of modules and find moduleName
	HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, pid);
	if (snapShot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 module;
		module.dwSize = sizeof(module);
		if (Module32First(snapShot, &module)) {
			do {
				_bstr_t str1(module.szModule);
				const char* str2 = str1;
				if (strcmp(str2, moduleName) == 0) {
					std::cout << "found module " << str2 << "\n";
					baseAddress = (uintptr_t)module.modBaseAddr;
					break;
				}
			} while (Module32Next(snapShot, &module));
		}
		CloseHandle(snapShot);
	}

	// return address
	return baseAddress;

}

uintptr_t followPointer(HANDLE processHandle, uintptr_t address, int offsets[], int size) {

	uintptr_t readAddress;
	for (int i = 0; i < size; i++) {
		ReadProcessMemory(processHandle, (LPVOID)address, &readAddress, sizeof(address), NULL);
		address = readAddress + offsets[i];
	}
	return address;
}


