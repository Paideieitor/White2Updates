#ifndef __HANDLER_PARAMS_H
#define __HANDLER_PARAMS_H

#include "defs.h"

struct SWAN_ALIGNED(4) HandlerParam_StrParams
{
    u16 ID;
    short flags;
    int type;
    int args[8];
};

struct HandlerParam_Header
{
    u32 flags;
};

struct HandlerParam_InterruptPoke
{
    HandlerParam_Header header;
    u8 pokeID;
    HandlerParam_StrParams exStr;
};

struct HandlerParam_SendLast
{
    HandlerParam_Header header;
    u8 pokeID;
    HandlerParam_StrParams exStr;
};

struct ServerFlow;

extern "C" b32 BattleHandler_SetString(ServerFlow* serverFlow, HandlerParam_StrParams* params);

#endif // __HANDLER_PARAMS_H
