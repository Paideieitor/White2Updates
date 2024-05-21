#include "custom/item_expansion.h"

C_DECL_BEGIN
BagItem* BagSave_AddItemCore(BagSaveData* bag, u16 item_idx, u16 quantity, HeapID heapId);

// --- PREDEF ---
GameEvent* EventFieldToggleRepel_Create(Field* field, GameSystem* gsys);


// --- DATA ---

// OVL_12

// [OVL_12:0216CE24] Extended list of items that have an overworld effect while in the overworld
FieldCommonEvent ExtFieldCommonEvents[] = {
    { EventFieldToggleCycling_Create, ACTID_BIKE},
    { EventEntralinkWarpIn_CreateDefault, ACTID_WARP_SWEETSCENT },
    { EventMapChangeEscapeRope_Create, ACTID_ESCAPE_ROPE },
    { EventSweetScent_Create, ACTID_WARP_SWEETSCENT },
    { EventFieldFishing_Create, ACTID_FISHING },
    { EventFieldToggleDowsing_Create, ACTID_DOWSING_MCHN },
    { EventFieldToggleRepel_Create, ACTID_REPEL },
};


// --- FUNCTIONS ---

// NEW

// Callback for the use of the repel item from the Register menu
int EventFieldToggleRepel_Callback(GameEvent* gEvent, u32* pState, EventFieldToggle* data)
{
    SaveControl* saveControl;
    EncountSave* encountSave;

    saveControl = GameData_GetSaveControl(GAME_DATA);
    encountSave = SaveControl_GetEncountSave(saveControl);
    if (!encountSave->RepelSteps)
    {
        DPRINT("ACTIVATED INFINITE REPEL\n");
        encountSave->RepelSteps = 0xFF;
    }
    else
    {
        DPRINT("DEACTIVATED INFINITE REPEL\n");
        encountSave->RepelSteps = 0x00;
    }

    return 1;
}

// Creates the event for the use of the repel item from the Register menu
GameEvent* EventFieldToggleRepel_Create(Field* field, GameSystem* gsys)
{
    GameEvent* v4;
    _DWORD* Data;

    v4 = GameEvent_Create(gsys, 0, (GameEventCallback)EventFieldToggleRepel_Callback, 8u);
    Data = (unsigned int*)GameEvent_GetData(v4);
    Data[0] = (unsigned int)gsys;
    Data[1] = (unsigned int)field;
    return v4;
}

// Bypasses the action block function
bool IsNewItemActionBlocked(ActionID actionID)
{
    switch (actionID)
    {
    case ACTID_REPEL:
        return true;
    }
    return false;
}

// OVL_36

// Called every step the player takes to check the repel steps left (pretty sure an empty function gets the work done too)
GameEvent* THUMB_BRANCH_CheckRepelEvent(Field* field, GameSystem* gsys, GameData* gameData)
{
    SaveControl* SaveControl; // r0
    EncountSave* EncountSave; // r6

    SaveControl = GameData_GetSaveControl(gameData);
    EncountSave = SaveControl_GetEncountSave(SaveControl);

    if (EncountSave->RepelSteps)
    {
        EncountSave->RepelSteps = 0xFF;
    }
    else
    {
        EncountSave->RepelSteps = 0x00;
    }

    return 0;
}

#if REPEL_SHAKING_GRASS
// Checks whether to create a shaking spot or not (does other stuff too)
void THUMB_BRANCH_UpdatePhenomenon(EncountSystem* encSys)
{
    int v2; // r6
    _WORD* m_EffectEncountState; // r7
    MapMtxSys* MapMatrixSystem; // r4
    FieldPlayer* Player; // r0
    FieldActor* Actor; // r0
    int ZoneIDAtMatrixXZWorld; // r0
    void* TrainerCardDataBlkAddress; // r0
    EncData* m_EncData; // r1
    u32 v10; // r4
    EncountState* a1; // [sp+0h] [bp-28h]
    int PlayerStateZoneID; // [sp+4h] [bp-24h]
    VecFx32 dest; // [sp+8h] [bp-20h] BYREF

    v2 = 0;
    a1 = GameData_GetEncountState(encSys->m_GameData);
    if (EncSys_IsActive(encSys))
    {
        m_EffectEncountState = (unsigned short*)encSys->m_EffectEncountState;
        MapMatrixSystem = GetMapMatrixSystem(encSys->m_GameData);
        if (encSys->m_Field)
        {
            if (MapMatrixSystem)
            {
                PlayerStateZoneID = Field_GetPlayerStateZoneID(encSys->m_Field);
                Player = Field_GetPlayer(encSys->m_Field);
                Actor = FieldPlayer_GetActor(Player);
                CopyActorWPos(Actor, &dest);
                if (RangeCheckChunkCoordinateWorld(MapMatrixSystem, dest.x, dest.z))
                {
                    ZoneIDAtMatrixXZWorld = GetZoneIDAtMatrixXZWorld(MapMatrixSystem, dest.x, dest.z);
                    if (PlayerStateZoneID != 0xFFFF
                        && ZoneIDAtMatrixXZWorld != 0xFFFF
                        && PlayerStateZoneID == ZoneIDAtMatrixXZWorld)
                    {
                        TrainerCardDataBlkAddress = getTrainerCardDataBlkAddress(encSys->m_GameData);
                        if (isBadgeObtained(TrainerCardDataBlkAddress, 0)
                            && Field_GetResolvedControllerTypeID(encSys->m_Field) == FLD_MAPCTRL_GRID)
                        {
                            m_EncData = encSys->m_EncData;
                            if (m_EncData->UserData[2])
                            {
                                v2 = 1;
                            }
                            if (m_EncData->UserData[4] || m_EncData->UserData[6])
                            {
                                v2 |= 2u;
                            }
                            if (v2 && (a1->field_24 & 0xF00) == 0 && sub_21A24AC(a1, (int)m_EffectEncountState))
                            {
                                v10 = PassPower_ApplyExploringChance((unsigned __int16)m_EffectEncountState[733]);

                                SaveControl* SaveControl = GameData_GetSaveControl(GAME_DATA);
                                EncountSave* EncountSave = SaveControl_GetEncountSave(SaveControl);

                                if (EncountSave->RepelSteps == 0x00 && GFL_RandomLCAlt(1000u) < 10 * v10) // avoid creating a shaking spot if the repel is active
                                {
                                    positionShakingSpot(encSys, (int)m_EffectEncountState, v2);
                                    if (m_EffectEncountState[2])
                                    {
                                        sub_21A272C((void**)&encSys->m_Field, (int)m_EffectEncountState);
                                    }
                                }
                                else
                                {
                                    setShakingSpotOff(a1);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
#endif


// ARM9
// Checks if the reg item action is blocked in the current moment (Ex. bike while in a house)
int THUMB_BRANCH_PlayerActionPerms_IsActionBlocked(PlayerActionPerms* perms, ActionID actionID)
{
    if (IsNewItemActionBlocked(actionID)) // This checks if the reg item is new (not vanilla)
        return 0; // rn it bypasses the check but you could add your own blocking params i guess

    return perms->ActionsBlocked[actionID];
}


// OVL_12

// Calls the list event function of the item that has an overworld effect while in the overworld
GameEvent* THUMB_BRANCH_CallFieldCommonEventFunc(FieldCommonEventID cmnEventId, GameSystem* gsys, Field* field)
{
    return ExtFieldCommonEvents[cmnEventId].function(field, gsys);
}

C_DECL_END