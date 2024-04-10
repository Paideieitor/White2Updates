#ifndef __LEVEL_CAP_H
#define __LEVEL_CAP_H

#include "include_all.h"

#define LVL_CAP_VAR 16415

C_DECL_BEGIN
void SetLvlCap(u16 lvl_cap)
{
    EventWorkSave* eventWork = GameData_GetEventWork(GAME_DATA);
    u16* lvl_cap_ptr = EventWork_GetWkPtr(eventWork, LVL_CAP_VAR);

#ifdef DEBUG_PRINT
    k::Printf("CHANGED LEVEL CAP: %d\n", lvl_cap);
#endif

    *lvl_cap_ptr = lvl_cap;
}

u32 GetLvlCap()
{
    EventWorkSave* eventWork = GameData_GetEventWork(GAME_DATA);
    u16* lvl_cap_ptr = EventWork_GetWkPtr(eventWork, LVL_CAP_VAR);

#ifdef DEBUG_PRINT
    k::Printf("CURRENT LEVEL CAP: %d\n", *lvl_cap_ptr);
#endif

    return *lvl_cap_ptr;
}
C_DECL_END

#endif