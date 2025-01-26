#ifndef __SERVER_EVENTS_H
#define __SERVER_EVENTS_H

#include "defs.h"

#include "server_flow.h"
#include "types.h"
#include "moves.h"

extern "C" u32 ServerEvent_CalculateSpeed(ServerFlow* serverFlow, BattleMon* battleMon, b32 IsTrickRoomEnabled);
extern "C" u32 ServerEvent_GetMovePower(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, MoveParam* moveParam);
extern "C" u32 ServerEvent_GetAttackPower(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, MoveParam* moveParam, u32 critFlag);
extern "C" u32 ServerEvent_GetTargetDefenses(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* targetMon, MoveParam* moveParam, u32 critFlag);
extern "C" u32 ServerEvent_GetWeather(ServerFlow* serverFlow);
extern "C" u32 ServerEvent_SameTypeAttackBonus(ServerFlow* serverFlow, BattleMon* attackingMon, PokeType moveType);
extern "C" b32 ServerEvent_CheckFloating(ServerFlow* serverFlow, BattleMon* battleMon, b32 checkFlyingType);
extern "C" void ServerEvent_GetMoveParam(ServerFlow* serverFlow, MOVE_ID moveID, BattleMon* battleMon, MoveParam* moveParam);
extern "C" void ServerEvent_ItemSetDecide(ServerFlow* serverFlow, BattleMon* battleMon, ITEM_ID itemID);
extern "C" void ServerEvent_ItemRewriteDone(ServerFlow* serverFlow, BattleMon* battleMon);
extern "C" void ServerEvent_CheckMultihitHits(ServerFlow* serverFlow, BattleMon* attackingMon, MOVE_ID moveID, HitCheckParam* params);
extern "C" void ServerEvent_NotifyAirLock(ServerFlow* serverFlow);

#endif // __SERVER_EVENTS_H