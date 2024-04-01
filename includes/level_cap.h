#ifndef __LEVEL_CAP_H
#define __LEVEL_CAP_H

#include "include_all.h"

#define LVL_CAP_VAR 16415

C_DECL_BEGIN
u32 GetCurrentLvlCap()
{
    EventWorkSave* eventWork = GameData_GetEventWork(*(GameData**)(g_GameBeaconSys + 4));
    u16 lvl_cap = *EventWork_GetWkPtr(eventWork, LVL_CAP_VAR);

#ifdef DEBUG_PRINT
    k::Printf("CURRENT LEVEL CAP: %d\n", lvl_cap);
#endif

    return lvl_cap;
}
C_DECL_END

#endif