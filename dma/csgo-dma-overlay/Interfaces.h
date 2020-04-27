#pragma once

#include <iostream>
#include <vmmdll.h>

extern DWORD g_pid;

extern VMMDLL_MAP_MODULEENTRY g_clientModule;
extern VMMDLL_MAP_MODULEENTRY g_engineModule;
extern VMMDLL_MAP_MODULEENTRY g_eseaModule;
extern bool g_eseaPresent;

extern DWORD g_dwEntityList;
extern DWORD g_dwClientState;
extern DWORD g_dwLocalPlayer;
extern DWORD g_dwGlobalVars;

namespace Interfaces
{
	// Used to initalise interfaces
	bool Init();
};