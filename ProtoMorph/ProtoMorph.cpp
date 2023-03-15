// https://www.youtube.com/watch?v=hlioPJ_uB7M
#include <windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "headers/stdafx.hpp"

Init init;
Menu menu;

//Next two lines are used for SwapBuffers
typedef BOOL(__stdcall* twglSwapBuffers) (HDC hDc);
twglSwapBuffers wglSwapBuffersGateway;

//SwapBuffers is called every frame
BOOL __stdcall hkwglSwapBuffers(HDC hDc)
{
	Offsets::entList = *(EntList**)Offsets::entityListAddr; //Resets entity list incase of server change

	if (GetAsyncKeyState(VK_INSERT) & 1) //Checks if we want to open menu
	{
		Config::bMenu = !Config::bMenu;
	}

	menu.Render(); //Renders menu
	init.DoHacks(); //Does hacks

	return wglSwapBuffersGateway(hDc);
}

//Nothing much to be said about here
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, Thread, hModule, NULL, NULL);
		DisableThreadLibraryCalls(hModule);

    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}



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

	Hook SwapBuffersHook("wglSwapBuffers", "opengl32.dll", (BYTE*)hkwglSwapBuffers, (BYTE*)&wglSwapBuffersGateway, 5);
	SwapBuffersHook.Enable(); //Enables our hook
	
	// Create hackloop and get key input
	while (true) {
		// End process
		if (GetAsyncKeyState(VK_END) & 1) {
			break;
		}

		// Toggle godmode
		if (GetAsyncKeyState(VK_HOME) & 1) {
			bHealth = !bHealth;
		}

		// Toggle inf ammo
		if (GetAsyncKeyState(VK_DELETE) & 1) {
			bAmmo = !bAmmo;
		}

		// Toggle no recoil
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
			// Idk if 1337 is the new value or a addy
			*(int*)(*localPlayerPtr + 0xf8) = 1337;
		}
		if (bAmmo) {
			uintptr_t ammoAddr = mem::FindDMAAddy(moduleBase + 0x10f4f4f4, { 0x374, 0x14, 0x0 });
			int* ammo = (int*)ammoAddr;
			// Idk if this is the new value or a addy
			*ammo = 1337;
		}
	}
	Sleep(5);

	// Cleanup & eject
	SwapBuffersHook.Disable(); //Disables our hook
	menu.Shutdown(); //Unloads our menu
	MessageBeep(MB_OK); //Lets user know that hack was closed
	Sleep(100); //Gives menu time to hurry up and close properly

	FreeLibraryAndExitThread((HMODULE)param, 0); //Finally we unload the DLL
	
	// May not be needed see if error is thrown
	fclose(f);
	FreeConsole();

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