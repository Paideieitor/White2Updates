#ifndef __DEFS_H
#define __DEFS_H

#ifdef __GNUC__
#define SWAN_PACKED __attribute__((__packed__))
#define SWAN_ALIGNED(x) __attribute__((aligned(x)))
#else
#define SWAN_PACKED
#define SWAN_ALIGNED(x)
#endif

#include "swantypes.h"

#define DEBUG_PRINT true
#if DEBUG_PRINT
#include "kPrint.h"
#define DPRINT(format) k::Print(format)
#define DPRINTF(format, ...) k::Printf(format, __VA_ARGS__)
#elif !DEBUG_PRINT
#define DPRINT(format)
#define DPRINTF(format, ...)
#endif

struct SWAN_ALIGNED(2) StrBuf
{
	u16 charCapacity;
	u16 charCount;
	u32 magic;
	u16 string[];
};

extern "C" void sys_memset(const void* ptr, u8 value, size_t size);

extern "C" u32 fixed_round(u32 value, u32 ratio);

extern u32 g_GameBeaconSys;
#define GAME_DATA *(GameData**)(g_GameBeaconSys + 4)

#endif // __DEFS_H
