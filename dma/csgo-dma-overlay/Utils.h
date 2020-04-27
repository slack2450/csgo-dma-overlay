#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "math.hpp"
#include <vmmdll.h>
#include "Vector.h"
#include "VMatrix.h"
#include "Interfaces.h"
#include "Entities.h"

#define INRANGE(x,a,b)   (x >= a && x <= b)
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))

#define getBits( x )		(INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte( x )		(getBits(x[0]) << 4 | getBits(x[1]))

#define GenMatrix glm::mat4(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)

typedef struct _ModuleDump {
	BYTE* dump;
	DWORD base;
	DWORD size;
} ModuleDump;


static glm::mat4 viewprojection;

class Utils
{
	static BYTE ReadByte(DWORD pid, VMMDLL_MAP_MODULEENTRY module, DWORD pCur)
	{
		BYTE byte;
		VMMDLL_MemRead(pid, pCur, &byte, sizeof(byte));
		return byte;
	}

public:

	static DWORD FindPattern(DWORD pid, ModuleDump module, const char* szSignature, int start=0)
	{

		const char* pat = szSignature;
		DWORD firstMatch = 0;

		QWORD rangeStart = 0;
		DWORD rangeEnd = module.size;

		DWORD pCur;
		for (pCur = start; pCur < rangeEnd; pCur++)
		{

			if (!*pat)
				return module.base + firstMatch;

			if (*(PBYTE)pat == '\?' || module.dump[pCur] == GET_BYTE(pat))
			{
				if (!firstMatch)
					firstMatch = pCur;

				if (!pat[2])
				{
					return module.base + firstMatch;	
				}
					

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;

				else
					pat += 2;
			}
			else
			{
				pat = szSignature;
				firstMatch = 0;
			}
		}
		return 0u;
	}

	static DWORD FindSignature(DWORD pid, ModuleDump module, const char* szSignature, int offset, int extra=0, bool eseaSigProtection=true)
	{
		DWORD result = FindPattern(pid, module, szSignature) + offset;
		if (g_eseaPresent && eseaSigProtection)
		{
			result = FindPattern(pid, module, szSignature, result - module.base) + offset;
		}
		VMMDLL_MemRead(pid, (ULONG64)(result), (PBYTE)&result, sizeof(DWORD));

		return result + extra;
	}

	static ModuleDump DumpModule(DWORD pid, VMMDLL_MAP_MODULEENTRY module)
	{
		ModuleDump moduleDump;
		moduleDump.base = (DWORD)module.vaBase;
		moduleDump.size = module.cbImageSize;
		moduleDump.dump = (BYTE*)malloc(sizeof(BYTE) * module.cbImageSize);

		VMMDLL_MemReadEx(pid, module.vaBase, (PBYTE)moduleDump.dump, sizeof(BYTE) * moduleDump.size, NULL, VMMDLL_FLAG_NOCACHE);

		return moduleDump;
	}
};