#include "better_repel.h"

C_DECL_BEGIN
GameEvent* THUMB_BRANCH_CheckRepelEvent(Field* field, GameSystem* gsys, GameData* gameData)
{
    SaveControl* SaveControl; // r0
    EncountSave* EncountSave; // r6

    SaveControl = GameData_GetSaveControl(gameData);
    EncountSave = SaveControl_GetEncountSave(SaveControl);

    if (EncountSave->RepelSteps)
        EncountSave->RepelSteps = 0xFF;
    else
        EncountSave->RepelSteps = 0x00;

    return 0;
}

#if REPEL_SHAKING_GRASS
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

                                if (GFL_RandomLCAlt(1000u) < 10 * v10 && EncountSave->RepelSteps == 0x00)
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
C_DECL_END