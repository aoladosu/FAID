#include "Util.h"

#include <WinUser.h>
#include <processthreadsapi.h>
#include <tlhelp32.h>
#include <comdef.h>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <iostream>

// stage parsing
void countStageData(int stage, int &chkSize)
{
	// count number of instances of things in the stage
	
	std::ifstream file("stages/" + std::to_string(stage) + ".txt");
	std::string line, chkPtStr = "chk";
	int chkPtCount=0;

	if (!file.good()) {
		return;
	}

	while (std::getline(file, line)) {
		if (line.find(chkPtStr) != std::string::npos) chkPtCount++;
	}

	chkSize = chkPtCount;
	file.close();
}

int getStageData(int stage, int chkSize, int *&chkPoints)
{
	// read file to get stage data into malloced array

	std::ifstream file("stages/" + std::to_string(stage) + ".txt");
	std::string line, chkPtStr = "chk";
	if (!file.good()) {
		return -1;
	}

	// 3 because x,y,z angle will be packed next to each other
	chkPoints = (int*) malloc(chkSize*3*4);
	if (chkPoints == nullptr) {
		return -1;
	}

	int piece, x, y, angle, i=0;
	char c;
	while (std::getline(file, line) && (i < chkSize*3)) {
		if (line.find(chkPtStr) != std::string::npos) {
			std::istringstream iss(line);
			iss >> c >> c >> c >> c >> piece >> c >> x >> c >> y >> c >> angle;
			chkPoints[i] = x;
			chkPoints[i+1] = y;
			chkPoints[i+2] = angle;
			i += 3;
		}
	}

	file.close();
	return 0;
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


