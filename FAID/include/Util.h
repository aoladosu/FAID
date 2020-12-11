#pragma once

#include <Windows.h>

// stage parsing
void countStageData(int stage, int &chkSize);
int getStageData(int stage, int chkSize, int *&chkPoints, int &nlaps);

// memory
uintptr_t GetModuleBaseAddress(DWORD dwProcID, char* szModuleName);
uintptr_t followPointer(HANDLE processHandle, uintptr_t address, int offsets[], int size);

// driving
bool reachedPoint(int x, int y, int z, int goalX, int goalY, int goalZ);
