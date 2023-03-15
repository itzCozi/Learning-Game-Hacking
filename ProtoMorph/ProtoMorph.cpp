// https://www.youtube.com/watch?v=hlioPJ_uB7M
#include "stdafx.h"
#include "main.h"
#include <windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "mem.h"
#include "proc.h"

DWORD WINAPI HackThread(HMODULE hModule) {
	// Create a console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "Test Test 124\n";

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");

	bool bHealth = false;
	bool bAmmo = false;
	bool bRecoil = false;


	// Create hackloop and get key input
	while (true) {
		if (GetAsyncKeyState(VK_END) & 1) {
			break;
		}

		if (GetAsyncKeyState(VK_HOME) & 1) {
			bHealth = !bHealth;
		}

		if (GetAsyncKeyState(VK_DELETE) & 1) {
			bAmmo = !bAmmo;
		}

		if (GetAsyncKeyState(VK_INSERT) & 1) {
			bRecoil = !bRecoil;

			if (bRecoil == true) {
				mem::Nop((BYTE*)(moduleBase + 0x63786), 10);
			}
			else {
				mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10);
			}
		}


	}

	// Continus write
	uintptr_t* localPlayerPtr = (uintptr_t*)(moduleBase + 0x10f4f4);

	if (localPlayerPtr) {
		if (bHealth) {
			*(int*)(*localPlayerPtr + 0xf8) = 1337;
		}
		if (bAmmo) {
			uintptr_t ammoAddr = mem::FindDMAAddy(moduleBase + 0x10f4f4f4, { 0x374, 0x14, 0x0 });
			int* ammo = (int*)ammoAddr;
			*ammo = 1337;
		}
	}
	Sleep(5);

	// Cleanup & eject
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}