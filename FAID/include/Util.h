#pragma once

#include <Windows.h>

// stage parsing
void countStageData(int stage, int &chkSize, int &jumpSize, int &obsSize);
int getStageData(int stage, int chkSize, int *&chkPoints, int jumpSize, int *&jumps, int obsSize, int *&obstacles, int &nlaps);
int relevantPiece(int piece);
int modifyAngle(int angle);

// memory
uintptr_t GetModuleBaseAddress(DWORD dwProcID, char* szModuleName);
uintptr_t followPointer(HANDLE processHandle, uintptr_t address, int offsets[], int size);
