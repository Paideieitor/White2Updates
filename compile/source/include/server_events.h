#ifndef __SERVER_EVENTS_H
#define __SERVER_EVENTS_H

#include "defs.h"

#include "server_flow.h"

extern "C" u32 ServerEvent_CalculateSpeed(ServerFlow* serverFlow, BattleMon* battleMon, b32 IsTrickRoomEnabled);

#endif // __SERVER_EVENTS_H