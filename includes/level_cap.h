#ifndef __LEVEL_CAP_H
#define __LEVEL_CAP_H

#include "include_all.h"

#define LVL_CAP_VAR 16462

C_DECL_BEGIN
void SetLvlCap(u16 lvl_cap)
{
    EventWorkSave* eventWork = GameData_GetEventWork(*(GameData**)(g_GameBeaconSys + 4));
    u16* lvl_cap_ptr = EventWork_GetWkPtr(eventWork, LVL_CAP_VAR);

#ifdef DEBUG_PRINT
    k::Printf("CHANGED LEVEL CAP: %d\n", lvl_cap);
#endif

    *lvl_cap_ptr = lvl_cap;
}

u32 GetLvlCap()
{
    EventWorkSave* eventWork = GameData_GetEventWork(*(GameData**)(g_GameBeaconSys + 4));
    u16* lvl_cap_ptr = EventWork_GetWkPtr(eventWork, LVL_CAP_VAR);

#ifdef DEBUG_PRINT
    k::Printf("GAME BEACON SYS: %d\n", g_GameBeaconSys);
    k::Printf("LVL CAP VAR: %d\n", LVL_CAP_VAR);
    k::Printf("LEVEL CAP VAR PTR: %d\n", lvl_cap_ptr);
    k::Printf("CURRENT LEVEL CAP: %d\n", *lvl_cap_ptr);
#endif

    return *lvl_cap_ptr;
}
C_DECL_END

#endif