#ifndef __LEVEL_CAP_H
#define __LEVEL_CAP_H

#include "custom/include_all.h"

#define LVL_CAP_VAR 16415

C_DECL_BEGIN
void SetLvlCap(u16 lvl_cap)
{
    EventWorkSave* eventWork = GameData_GetEventWork(GAME_DATA);
    u16* lvl_cap_ptr = EventWork_GetWkPtr(eventWork, LVL_CAP_VAR);

    DPRINTF("CHANGED LEVEL CAP: %d\n", lvl_cap);

    *lvl_cap_ptr = lvl_cap;
}

u32 GetLvlCap()
{
    EventWorkSave* eventWork = GameData_GetEventWork(GAME_DATA);
    u16* lvl_cap_ptr = EventWork_GetWkPtr(eventWork, LVL_CAP_VAR);

    DPRINTF("CURRENT LEVEL CAP: %d\n", *lvl_cap_ptr);

    return *lvl_cap_ptr;
}
C_DECL_END

#endif