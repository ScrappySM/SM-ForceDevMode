/// How this mod works:
/// This mod is extremely basic. It simply changes the flag that controls the dev mode in the game's memory.
/// 
/// How to update the mod (assumes IDA)
/// Use the strings window and search for "mismatching developer mode", double click it and XREF to it's usage
/// You should be at an if statement, the `byte_14xxxxxxx` is the flag we want to change, copy the `xxxxxxx` part
/// Now replace the `DevModeFlag` value im this code with that value prefixed with `0x` (to tell C++ it's a hex number)
/// You're done! Compile the mod and inject it into the game

#include <Windows.h>
#include <iostream>

static constexpr uintptr_t DevModeFlag = 0x1267537;
static char oldFlag = 0;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hModule);

		// Get the address of the flag
		uintptr_t flagAddr = (uintptr_t)GetModuleHandle(NULL) + DevModeFlag;
		std::cout << "Flag address: " << std::hex << flagAddr << std::endl;

		// Read the old flag
		oldFlag = *(char*)flagAddr;
		std::cout << "Old flag: " << std::hex << (int)oldFlag << std::endl;

		// Force enable dev mode
		*(char*)flagAddr = 1;
	}

	if (dwReason == DLL_PROCESS_DETACH) {
		uintptr_t flagAddr = (uintptr_t)GetModuleHandle(NULL) + DevModeFlag;
		*(char*)flagAddr = oldFlag; // Restore the old flag
	}

	return TRUE;
}
