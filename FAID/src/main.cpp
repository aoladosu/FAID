#include <iostream>
#include <Util.h>

int main()
{
	std::cout << "Hello World!\n";

	/*
	// get handle to window
	HWND wndHandle = FindWindowA(NULL, "Need for Madness");
	if (wndHandle == NULL) {
		std::cout << "Unable to find window.";
		exit(0);
	}

	// get process id and handle
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


	// print memory we are reading
	std::cout << std::hex << chkPointMem << "-7\n";
	uintptr_t chkPointOld = 0, chkPointNew;
	uintptr_t up = 0x10000;
	while (true) {
		// read checkpoint number
		ReadProcessMemory(processHandle, (LPVOID)chkPointMem, &chkPointNew, sizeof(chkPointNew), NULL);
		if (chkPointOld != chkPointNew) {
			std::cout << "checkpoint: " << std::dec << chkPointNew << "\n";
		}
		chkPointOld = chkPointNew;

		//write
		WriteProcessMemory(processHandle, (LPVOID)controlsMem, &up, sizeof(up), NULL); 
	}
	*/

	int size, *chkPoints=0;
	countStageData(2, size);
	if (getStageData(2, size, chkPoints) < 0) {
		std::cout << "Couldn't get stage data\n";
	}

	for (int i = 0; i < size*3; i+=3) {
		std::cout << "x: " << chkPoints[i];
		std::cout << " y: " << chkPoints[i+1];
		std::cout << " angle: " << chkPoints[i+2];
		std::cout << "\n";
	}

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


time
#include <chrono>
#include <thread>

	auto start = std::chrono::steady_clock::now();
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed = end - start;

	while (elapsed.count() < 6) {

		test.press();
		auto end = std::chrono::steady_clock::now();
		elapsed = end - start;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}


*/