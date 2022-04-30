#include <Windows.h>
#include <thread>
#include "utils/utils.h"
#include "hook/hooks.h"
int start(HMODULE hModule)
{
	utils.setupCmd();
	utils.setupSignatures();
	hooks.initialize();

	while (!GetAsyncKeyState(VK_DELETE))
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

	hooks.detach();
	utils.detachCmd();
	FreeLibraryAndExitThread(hModule, 0);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD sReason, LPVOID lpReserved)
{
	if (sReason == DLL_PROCESS_ATTACH) {
		auto pHandle = CreateThread(nullptr,0, LPTHREAD_START_ROUTINE(start), hModule, 0, nullptr);
		if (pHandle)
			CloseHandle(pHandle);
	}
	return TRUE;
}