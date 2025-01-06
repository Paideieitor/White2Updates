#ifndef __SERVER_EVENTS_H
#define __SERVER_EVENTS_H

#include "defs.h"

#include "server_flow.h"
#include "types.h"

extern "C" u32 ServerEvent_CalculateSpeed(ServerFlow* serverFlow, BattleMon* battleMon, b32 IsTrickRoomEnabled);
extern "C" u32 ServerEvent_GetMovePower(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, MoveParam* moveParam);
extern "C" u32 ServerEvent_GetAttackPower(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, MoveParam* moveParam, u32 critFlag);
extern "C" u32 ServerEvent_GetTargetDefenses(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* targetMon, MoveParam* moveParam, u32 critFlag);
extern "C" u32 ServerEvent_GetWeather(ServerFlow* serverFlow);
extern "C" u32 ServerEvent_SameTypeAttackBonus(ServerFlow* serverFlow, BattleMon* attackingMon, PokeType moveType);

#endif // __SERVER_EVENTS_H