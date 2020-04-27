#pragma once

#include <vmmdll.h>

namespace offsets
{
	ULONG64 m_bDormant = 0xED;
	ULONG64 m_iHealth = 0x100;
	ULONG64 m_vecViewOffset = 0x108;
	ULONG64 m_vecOrigin = 0x138;
	ULONG64 m_angEyeAngles = 0xB36C;
	
	ULONG64 dwClientState_ViewAngles = 0x4D88;
}