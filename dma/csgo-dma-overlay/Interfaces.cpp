#include "Interfaces.h"
#include "Utils.h"

LPSTR process_name = (LPSTR)"csgo.exe";
DWORD g_pid;

LPWSTR clientModuleName = (LPWSTR)L"client_panorama.dll";
VMMDLL_MAP_MODULEENTRY g_clientModule;

LPWSTR engineModuleName = (LPWSTR)L"engine.dll";
VMMDLL_MAP_MODULEENTRY g_engineModule;

LPWSTR eseaModuleName   = (LPWSTR)L"eseaclient.csgo.dll";
VMMDLL_MAP_MODULEENTRY g_eseaModule;
bool g_eseaPresent = false;

DWORD g_dwClientState;
DWORD g_dwEntityList;
DWORD g_dwLocalPlayer;
DWORD g_dwGlobalVars;

namespace Interfaces
{
	bool Init()
	{
		if (!VMMDLL_PidGetFromName(process_name, &g_pid))
			return false;

		printf("[+] Found %s! PID: %d\n", process_name, g_pid);

		printf("[+] Found modules:\n");

		if (!VMMDLL_ProcessMap_GetModuleFromName(g_pid, clientModuleName, &g_clientModule))
			return false;

		wprintf(L"    - %-20s at 0x%X-0x%X\n", clientModuleName, (unsigned int)g_clientModule.vaBase, (unsigned int)g_clientModule.vaBase + g_clientModule.cbImageSize);

		if (!VMMDLL_ProcessMap_GetModuleFromName(g_pid, engineModuleName, &g_engineModule))
			return false;

		wprintf(L"    - %-20s at 0x%X-0x%X\n", engineModuleName, (unsigned int)g_engineModule.vaBase, (unsigned int)g_engineModule.vaBase + g_engineModule.cbImageSize);

		if (VMMDLL_ProcessMap_GetModuleFromName(g_pid, eseaModuleName, &g_eseaModule))
		{
			g_eseaPresent = true;
			wprintf(L"    - %-20s at 0x%X-0x%X\n", eseaModuleName, (unsigned int)g_eseaModule.vaBase, (unsigned int)g_eseaModule.vaBase + g_eseaModule.cbImageSize);
		}

		printf("\n");

		wprintf(L"[*] Dumping %s\n", clientModuleName);
		ModuleDump clientModuleDump = Utils::DumpModule(g_pid, g_clientModule);

		wprintf(L"[*] Dumping %s\n", engineModuleName);
		ModuleDump engineModuleDump = Utils::DumpModule(g_pid, g_engineModule);

		printf("\n");


		g_dwClientState = Utils::FindSignature(g_pid, engineModuleDump, "A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0", 1, 0, false);
		VMMDLL_MemRead(g_pid, g_dwClientState, (PBYTE)&g_dwClientState, sizeof(DWORD));
		printf("[+] dwClientState: 0x%X\n", g_dwClientState);

		// Entity List
		g_dwEntityList = Utils::FindSignature(g_pid, clientModuleDump, "BB ? ? ? ? 83 FF 01 0F 8C ? ? ? ? 3B F8", 1, 0, false);
		printf("[+] dwEntityList: 0x%X\n", g_dwEntityList);

		

		// LocalPlayer
		g_dwLocalPlayer = Utils::FindSignature(g_pid, clientModuleDump, "8D 34 85 ? ? ? ? 89 15 ? ? ? ? 8B 41 08 8B 48 04 83 F9 FF", 3, 4);
		VMMDLL_MemRead(g_pid, (ULONG64)g_dwLocalPlayer, (PBYTE)&g_dwLocalPlayer, sizeof(DWORD));
		printf("[+] dwLocalPlayer: 0x%X\n", g_dwLocalPlayer);

		// Global Vars
		g_dwGlobalVars = Utils::FindSignature(g_pid, engineModuleDump, "68 ? ? ? ? 68 ? ? ? ? FF 50 08 85 C0", 1, 0, false);
		printf("[+] dwGlobalVars: 0x%X\n", g_dwGlobalVars);

		printf("\n");

		return true;
	}
}