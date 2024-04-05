#include "level_cap.h"

C_DECL_BEGIN
//BagItem* BagSave_AddItemCore(BagSaveData* bag, u16 item_idx, u16 quantity, HeapID heapId);
//bool THUMB_BRANCH_BagSave_AddItem(BagSaveData* bag, u16 itemId, u16 quantity, HeapID heapId)
//{
//    itemId = 622;
//    return BagSave_AddItemCore(bag, itemId, quantity, heapId) != 0;
//}

int THUMB_BRANCH_s002A_WorkSet(ScriptVM* vm, FieldScriptEnv* env)
{
    u16* Var; // r4

    Var = ScriptReadVar(vm, env);
    k::Printf("WORK SET VAR PTR: %d\n", Var);
    k::Printf("WORK SET VAR OLD: %d\n", *Var);
    *Var = ScriptReadAny(vm, env);
    k::Printf("WORK SET VAR NEW: %d\n", *Var);
    return 0;
}

bool THUMB_BRANCH_PokeList_CanItemWithBattleStatsBeUsed(PartyPkm* a1, unsigned int a2, int a3, unsigned __int16 a4)
{
    ItemData* DataFile; // r4
    u32 Status; // r5
    u32 Param; // r5
    unsigned __int16 v9; // r0
    int v10; // r5
    signed __int32 v11; // r5
    signed __int32 v12; // r7
    int v13; // r0
    int v14; // r0
    int v15; // r0
    int v16; // r0
    int v17; // r0
    int v19; // [sp+0h] [bp-30h]
    int v20; // [sp+4h] [bp-2Ch]
    signed __int32 v21; // [sp+8h] [bp-28h]
    signed __int32 v22; // [sp+Ch] [bp-24h]
    signed __int32 v23; // [sp+10h] [bp-20h]
    signed __int32 v24; // [sp+14h] [bp-1Ch]
    a4 = HEAPID_POKELIST;
    DataFile = (ItemData*)PML_ItemReadDataFile(a2, 0, a4);
    if (PML_ItemGetParam(DataFile, ITSTAT_HAS_BATTLE_STATS) == 1)
    {
        Status = PokeParty_GetStatusCond(a1);
        if (PML_ItemGetParam(DataFile, ITSTAT_AILMENT_SLP) && Status == 2
            || PML_ItemGetParam(DataFile, ITSTAT_AILMENT_PSN) && Status == 5
            || PML_ItemGetParam(DataFile, ITSTAT_AILMENT_BRN) && Status == 4
            || PML_ItemGetParam(DataFile, ITSTAT_AILMENT_FRZ) && Status == 3
            || PML_ItemGetParam(DataFile, ITSTAT_AILMENT_PAR) && Status == 1)
        {
            goto LABEL_102;
        }
        Param = PokeParty_GetParam(a1, PF_NowHP, 0);
        if ((PML_ItemGetParam(DataFile, ITSTAT_BOOST_REVIVE) || PML_ItemGetParam(DataFile, ITSTAT_BOOST_SACRED_ASH))
            && !PML_ItemGetParam(DataFile, ITSTAT_BOOST_RARECANDY))
        {
            if (!Param)
            {
            LABEL_102:
                GFL_HeapFree(DataFile);
                return 1;
            }
        }
        else if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_HPREPLENISH)
            && Param
            && Param < PokeParty_GetParam(a1, PF_MaxHP, 0))
        {
            goto LABEL_102;
        }

        u32 level_cap = GetLvlCap();
        u32 level = PokeParty_GetParam(a1, PF_Level, 0);
        if (PML_ItemGetParam(DataFile, ITSTAT_BOOST_RARECANDY) && level < level_cap)
        {
            goto LABEL_102;
        }
        if ((PML_ItemGetParam(DataFile, ITSTAT_BOOST_PP1) || PML_ItemGetParam(DataFile, ITSTAT_BOOST_PPMAX))
            && PokeParty_GetParam(a1, (PkmField)(a3 + 62), 0) < 3)
        {
            v9 = PokeParty_GetParam(a1, (PkmField)(a3 + 54), 0);
            if (PML_MoveGetMaxPP(v9, 0) >= 5)
            {
                goto LABEL_102;
            }
        }
        if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_PPREPLENISH) && PokeParty_CheckPPNeedsReplenish(a1, a3) == 1)
        {
            goto LABEL_102;
        }
        if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_PPREPLENISH_FULL))
        {
            v10 = 0;
            while (PokeParty_CheckPPNeedsReplenish(a1, v10) != 1)
            {
                if (++v10 >= 4)
                {
                    goto LABEL_36;
                }
            }
            goto LABEL_102;
        }
    LABEL_36:
        v11 = PokeParty_GetParam(a1, PF_EvHP, 0);
        v24 = PokeParty_GetParam(a1, PF_EvATK, 0);
        v23 = PokeParty_GetParam(a1, PF_EvDEF, 0);
        v22 = PokeParty_GetParam(a1, PF_EvSPE, 0);
        v21 = PokeParty_GetParam(a1, PF_EvSPA, 0);
        v12 = PokeParty_GetParam(a1, PF_EvSPD, 0);
        v20 = PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_ABOVE100);
        if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_HP))
        {
            v19 = PML_ItemGetParam(DataFile, ITSTAT_EV_HP);
            if (v19 <= 0 || PokeParty_GetParam(a1, PF_Species, 0) == PK292_SHEDINJA)
            {
                if (v19 < 0 && (v11 > 0 || PokeList_CanFriendshipBeIncreased(a1, DataFile)))
                {
                    goto LABEL_102;
                }
            }
            else if ((v11 < 100 || v20) && v11 < 255 && v12 + v21 + v22 + v23 + v11 + v24 < 510)
            {
                goto LABEL_102;
            }
        }
        if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_ATK))
        {
            v13 = PML_ItemGetParam(DataFile, ITSTAT_EV_ATK);
            if (v13 <= 0)
            {
                if (v13 < 0 && (v24 > 0 || PokeList_CanFriendshipBeIncreased(a1, DataFile)))
                {
                    goto LABEL_102;
                }
            }
            else if ((v24 < 100 || v20) && v24 < 255 && v12 + v21 + v22 + v23 + v11 + v24 < 510)
            {
                goto LABEL_102;
            }
        }
        if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_DEF))
        {
            v14 = PML_ItemGetParam(DataFile, ITSTAT_EV_DEF);
            if (v14 <= 0)
            {
                if (v14 < 0 && (v23 > 0 || PokeList_CanFriendshipBeIncreased(a1, DataFile)))
                {
                    goto LABEL_102;
                }
            }
            else if ((v23 < 100 || v20) && v23 < 255 && v12 + v21 + v22 + v23 + v11 + v24 < 510)
            {
                goto LABEL_102;
            }
        }
        if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_SPE))
        {
            v15 = PML_ItemGetParam(DataFile, ITSTAT_EV_SPE);
            if (v15 <= 0)
            {
                if (v15 < 0 && (v22 > 0 || PokeList_CanFriendshipBeIncreased(a1, DataFile)))
                {
                    goto LABEL_102;
                }
            }
            else if ((v22 < 100 || v20) && v22 < 255 && v12 + v21 + v22 + v23 + v11 + v24 < 510)
            {
                goto LABEL_102;
            }
        }
        if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_SPA))
        {
            v16 = PML_ItemGetParam(DataFile, ITSTAT_EV_SPA);
            if (v16 <= 0)
            {
                if (v16 < 0 && (v21 > 0 || PokeList_CanFriendshipBeIncreased(a1, DataFile)))
                {
                    goto LABEL_102;
                }
            }
            else if ((v21 < 100 || v20) && v21 < 255 && v12 + v21 + v22 + v23 + v11 + v24 < 510)
            {
                goto LABEL_102;
            }
        }
        if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_SPD))
        {
            v17 = PML_ItemGetParam(DataFile, ITSTAT_EV_SPD);
            if (v17 <= 0)
            {
                if (v17 < 0 && (v12 > 0 || PokeList_CanFriendshipBeIncreased(a1, DataFile)))
                {
                    goto LABEL_102;
                }
            }
            else if ((v12 < 100 || v20) && v12 < 255 && v12 + v21 + v22 + v23 + v11 + v24 < 510)
            {
                goto LABEL_102;
            }
        }
    }
    GFL_HeapFree(DataFile);
    return 0;
}

int THUMB_BRANCH_PokeList_ApplyItemEffect(PartyPkm* a1, unsigned int a2, int a3, unsigned __int16 a4, int a5)
{
    int v6; // r7
    ItemData* DataFile; // r4
    int v8; // r6
    int v9; // r0
    int v10; // r0
    u16 v11; // r6
    u16 v12; // r0
    u32 PkmLvExp; // r0
    unsigned int v14; // r0
    int v15; // r0
    int i; // r6
    int v17; // r2
    int EVsAppliedAfterLimit; // r2
    int v19; // r2
    int v20; // r2
    int v21; // r2
    int v22; // r2
    int v23; // r2
    signed __int32 v25; // r0
    int v26; // r6
    int v27; // r0
    int v28; // r0
    int v29; // r0
    u16 v30; // [sp+8h] [bp-80h]
    int v33; // [sp+14h] [bp-74h]
    int v34; // [sp+18h] [bp-70h]
    int v35; // [sp+1Ch] [bp-6Ch]
    int v36; // [sp+20h] [bp-68h]
    int v37; // [sp+24h] [bp-64h]
    int v38; // [sp+28h] [bp-60h]
    int v39; // [sp+2Ch] [bp-5Ch]
    signed int v40; // [sp+30h] [bp-58h]
    int v41; // [sp+34h] [bp-54h]
    int v42; // [sp+38h] [bp-50h]
    int v43; // [sp+3Ch] [bp-4Ch]
    int v44; // [sp+40h] [bp-48h]
    int v45; // [sp+44h] [bp-44h]
    int v46; // [sp+48h] [bp-40h]
    int v47; // [sp+4Ch] [bp-3Ch]
    u32 data; // [sp+50h] [bp-38h]
    u32 Param; // [sp+54h] [bp-34h]
    signed __int32 v50; // [sp+58h] [bp-30h]

    a5 = HEAPID_POKELIST;

    v30 = a2;
    v6 = 0;
    DataFile = (ItemData*)PML_ItemReadDataFile(a2, 0, a5);
    if (PML_ItemGetParam(DataFile, ITSTAT_HAS_BATTLE_STATS) != 1)
    {
        goto LABEL_85;
    }
    v8 = 0;
    Param = PokeParty_GetParam(a1, PF_StatusCond, 0);
    data = Param;
    if (PML_ItemGetParam(DataFile, ITSTAT_AILMENT_SLP))
    {
        data = 0;
        v8 = 1;
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_AILMENT_PSN))
    {
        data = 0;
        v8 = 1;
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_AILMENT_BRN))
    {
        data = 0;
        v8 = 1;
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_AILMENT_FRZ))
    {
        data = 0;
        v8 = 1;
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_AILMENT_PAR))
    {
        data = 0;
        v8 = 1;
    }
    if (Param != data)
    {
        PokeParty_SetParam(a1, PF_StatusCond, data);
        v6 = 1;
    }
    v42 = PokeParty_GetParam(a1, PF_NowHP, 0);
    v40 = PokeParty_GetParam(a1, PF_MaxHP, 0);
    if ((PML_ItemGetParam(DataFile, ITSTAT_BOOST_REVIVE) || PML_ItemGetParam(DataFile, ITSTAT_BOOST_SACRED_ASH))
        && !PML_ItemGetParam(DataFile, ITSTAT_BOOST_RARECANDY))
    {
        if (!v42)
        {
            v9 = PML_ItemGetParam(DataFile, ITSTAT_HEAL_AMOUNT);
            PokeList_ApplyHeal(a1, 0, v40, v9);
            v6 = 1;
        }
    }
    else
    {
        if (!PML_ItemGetParam(DataFile, ITSTAT_FLAG_HPREPLENISH))
        {
            goto LABEL_25;
        }
        if (v42 < v40)
        {
            v10 = PML_ItemGetParam(DataFile, ITSTAT_HEAL_AMOUNT);
            PokeList_ApplyHeal(a1, v42, v40, v10);
            v6 = 1;
        }
    }
    v8 = 1;
LABEL_25:
    v50 = PokeParty_GetParam(a1, PF_Level, 0);
    if (PML_ItemGetParam(DataFile, ITSTAT_BOOST_RARECANDY))
    {
        if (v50 < 100)
        {
            v11 = PokeParty_GetParam(a1, PF_Species, 0);
            v12 = PokeParty_GetParam(a1, PF_Forme, 0);
            PkmLvExp = PML_UtilGetPkmLvExp(v11, v12, (v50 + 1));
            PokeParty_SetParam(a1, PF_Experience, PkmLvExp);
            PokeParty_RecalcStats(a1);
            if (!v42)
            {
                v14 = PokeParty_GetParam(a1, PF_MaxHP, 0);
                PokeList_ApplyHeal(a1, 0, v14, v14 - v40);
            }
            v6 = 1;
        }
        v8 = 1;
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_BOOST_EVOSTONE))
    {
        v8 = 1;
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_BOOST_PP1))
    {
        if (PokeList_ApplyMaxPPIncrease(a1, a3, 1) == 1)
        {
            v6 = 1;
        }
    }
    else
    {
        if (!PML_ItemGetParam(DataFile, ITSTAT_BOOST_PPMAX))
        {
            goto LABEL_41;
        }
        if (PokeList_ApplyMaxPPIncrease(a1, a3, 3) == 1)
        {
            v6 = 1;
        }
    }
    v8 = 1;
LABEL_41:
    if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_PPREPLENISH))
    {
        v15 = PML_ItemGetParam(DataFile, ITSTAT_PP_GAIN);
        if (PokeParty_AddPP(a1, a3, v15) == 1)
        {
            v6 = 1;
        }
    }
    else
    {
        if (!PML_ItemGetParam(DataFile, ITSTAT_FLAG_PPREPLENISH_FULL))
        {
            goto LABEL_51;
        }
        for (i = 0; i < 4; ++i)
        {
            v17 = PML_ItemGetParam(DataFile, ITSTAT_PP_GAIN);
            if (PokeParty_AddPP(a1, i, v17) == 1)
            {
                v6 = 1;
            }
        }
    }
    v8 = 1;
LABEL_51:
    v43 = PokeParty_GetParam(a1, PF_EvHP, 0);
    v41 = PokeParty_GetParam(a1, PF_EvATK, 0);
    v39 = PokeParty_GetParam(a1, PF_EvDEF, 0);
    v38 = PokeParty_GetParam(a1, PF_EvSPE, 0);
    v47 = PokeParty_GetParam(a1, PF_EvSPA, 0);
    v46 = PokeParty_GetParam(a1, PF_EvSPD, 0);
    v45 = PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_ABOVE100);
    if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_HP))
    {
        v44 = PML_ItemGetParam(DataFile, ITSTAT_EV_HP);
        if (v44 > 0 && PokeParty_GetParam(a1, PF_Species, 0) == 292)
        {
        LABEL_85:
            GFL_HeapFree(DataFile);
            return 0;
        }
        EVsAppliedAfterLimit = PokeList_GetEVsAppliedAfterLimit(v43, v46 + v47 + v38 + v41 + v39, v44, v45);
        if (EVsAppliedAfterLimit != -1)
        {
            v43 = EVsAppliedAfterLimit;
            PokeParty_SetParam(a1, PF_EvHP, EVsAppliedAfterLimit);
            PokeParty_RecalcStats(a1);
            v6 = 1;
        }
        if (v44 > 0)
        {
            v8 = 1;
        }
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_ATK))
    {
        v33 = PML_ItemGetParam(DataFile, ITSTAT_EV_ATK);
        v19 = PokeList_GetEVsAppliedAfterLimit(v41, v46 + v47 + v38 + v43 + v39, v33, v45);
        if (v19 != -1)
        {
            v41 = v19;
            PokeParty_SetParam(a1, PF_EvATK, v19);
            PokeParty_RecalcStats(a1);
            v6 = 1;
        }
        if (v33 > 0)
        {
            v8 = 1;
        }
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_DEF))
    {
        v34 = PML_ItemGetParam(DataFile, ITSTAT_EV_DEF);
        v20 = PokeList_GetEVsAppliedAfterLimit(v39, v46 + v47 + v38 + v43 + v41, v34, v45);
        if (v20 != -1)
        {
            v39 = v20;
            PokeParty_SetParam(a1, PF_EvDEF, v20);
            PokeParty_RecalcStats(a1);
            v6 = 1;
        }
        if (v34 > 0)
        {
            v8 = 1;
        }
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_SPE))
    {
        v35 = PML_ItemGetParam(DataFile, ITSTAT_EV_SPE);
        v21 = PokeList_GetEVsAppliedAfterLimit(v38, v46 + v47 + v39 + v43 + v41, v35, v45);
        if (v21 != -1)
        {
            v38 = v21;
            PokeParty_SetParam(a1, PF_EvSPE, v21);
            PokeParty_RecalcStats(a1);
            v6 = 1;
        }
        if (v35 > 0)
        {
            v8 = 1;
        }
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_SPA))
    {
        v36 = PML_ItemGetParam(DataFile, ITSTAT_EV_SPA);
        v22 = PokeList_GetEVsAppliedAfterLimit(v47, v46 + v38 + v39 + v43 + v41, v36, v45);
        if (v22 != -1)
        {
            v47 = v22;
            PokeParty_SetParam(a1, PF_EvSPA, v22);
            PokeParty_RecalcStats(a1);
            v6 = 1;
        }
        if (v36 > 0)
        {
            v8 = 1;
        }
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_SPD))
    {
        v37 = PML_ItemGetParam(DataFile, ITSTAT_EV_SPD);
        v23 = PokeList_GetEVsAppliedAfterLimit(v46, v47 + v38 + v39 + v43 + v41, v37, v45);
        if (v23 != -1)
        {
            PokeParty_SetParam(a1, PF_EvSPD, v23);
            PokeParty_RecalcStats(a1);
            v6 = 1;
        }
        if (v37 > 0)
        {
            v8 = 1;
        }
    }
    if (!v6 && v8 == 1)
    {
        goto LABEL_85;
    }
    v25 = PokeParty_GetParam(a1, PF_Happiness, 0);
    v26 = v25;
    if (v25 >= 100)
    {
        if (v25 >= 200)
        {
            if (v25 <= 255 && PML_ItemGetParam(DataFile, ITSTAT_FLAG_FRIENDSHIP_ADD3))
            {
                v29 = PML_ItemGetParam(DataFile, ITSTAT_FRIENDSHIP3);
                PokeList_ApplyFriendship(a1, v26, v29, v30, a4, a5);
                GFL_HeapFree(DataFile);
                return v6;
            }
        }
        else if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_FRIENDSHIP_ADD2))
        {
            v28 = PML_ItemGetParam(DataFile, ITSTAT_FRIENDSHIP2);
            PokeList_ApplyFriendship(a1, v26, v28, v30, a4, a5);
            GFL_HeapFree(DataFile);
            return v6;
        }
    }
    else if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_FRIENDSHIP_ADD1))
    {
        v27 = PML_ItemGetParam(DataFile, ITSTAT_FRIENDSHIP1);
        PokeList_ApplyFriendship(a1, v26, v27, v30, a4, a5);
        GFL_HeapFree(DataFile);
        return v6;
    }
    GFL_HeapFree(DataFile);
    return v6;
}
C_DECL_END

/*
PokeList_Party* THUMB_BRANCH_sub_219B30C(PokeList* a1)
{
    k::Print("THUMB_BRANCH_sub_219B30C START\n");
    PokeList_Party* v2; // r2
    PokeList_Party* result; // r0
    unsigned int ItemUseType; // r1
    ItemRestoreType v5; // r6
    int v6; // r0
    int ItemID; // r1
    int KyuremUnfuseCase; // r0
    u32 Param; // r0
    unsigned __int16 v10; // r6
    MailData* v11; // r6

    v2 = a1->field_28C;
    if (v2->field_73 == 1)
    {
        result = (PokeList_Party*)19;
        a1->field_C = 19;
        return result;
    }
    ItemUseType = v2->ItemUseType;
    if (ItemUseType > 0x1B)
    {
    LABEL_65:
        sub_219CF2C(a1);
        return (PokeList_Party*)sub_219B8F0(a1);
    }
    switch (*(u8*)(0x0219B33A + ItemUseType))
    {
    case 0:
    case 1:
    case 0x12:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x19:
        sub_219CF2C(a1);
        GFL_BGSysLoadScr(G2D_BG0A);
        return (PokeList_Party*)sub_219B8F0(a1);
    case 3:
    case 0x1B:
        a1->field_C = 19;
        v2->SelectedSlot = a1->SelectedSlot;
        result = a1->field_28C;
        result->field_50 = 0;
        return result;
    case 5:
        if (PokeList_IsPPRestoringItem(a1, v2->ItemID))
        {
            sub_219CF2C(a1);
            return (PokeList_Party*)sub_219B8F0(a1);
        }
        if (PokeList_CanItemWithBattleStatsBeUsed(a1->SelectedPkm, a1->field_28C->ItemID, 0, a1->heapID))
        {
            v5 = PokeList_PrintItemRecoverMessage(a1, 0);
            v6 = PokeList_ApplyItemEffect(a1->SelectedPkm, a1->field_28C->ItemID, 0, a1->field_28C->field_40, a1->heapID);
            if (v5 == RESTORETYPE_RARE_CANDY)
            {
                if (v6)
                {
                    sub_2038BF4(9);
                }
                sub_219F290(a1, a1->pokeList_Plate[a1->SelectedSlot], a1->SelectedPkm, 0);
            }
            else
            {
                if (v5 == RESTORETYPE_EV_HP_DECREASE || v5 == RESTORETYPE_EV_HP)
                {
                    sub_219F290(a1, a1->pokeList_Plate[a1->SelectedSlot], a1->SelectedPkm, 0);
                }
                else
                {
                    sub_219F350(a1, a1->pokeList_Plate[a1->SelectedSlot]);
                }
                GFL_SndSEPlay(SEQ_SE_RECOVERY);
            }
            return (PokeList_Party*)PokeList_SubItem(a1, a1->field_28C->ItemID);
        }
        if (a1->field_28C->ItemID == IT0466_GRACIDEA && PokeList_DoesShayminNeedFormeChange(a1))
        {
            PokeList_ChangeShayminToSkyForme(a1, a1->SelectedPkm);
            a1->field_28C->field_50 = 10;
            a1->field_C = 14;
            result = (PokeList_Party*)3;
            a1->field_274 = 3;
            return result;
        }
        if (a1->field_28C->ItemID == IT0638_REVEAL_GLASS
            && PokeList_IsRevealGlassMon((int)a1, a1->SelectedPkm)
            && isOneShotDRObtained((void*)a1->field_28C->field_24, 6, (void*)a1->field_28C->field_28))
        {
            PokeList_ChangeRevealGlassMonForme(a1, a1->SelectedPkm);
            a1->field_28C->field_50 = 10;
            result = (PokeList_Party*)14;
            a1->field_C = 14;
            a1->field_274 = 6;
            return result;
        }
        ItemID = a1->field_28C->ItemID;
        if (ItemID != IT0628_DNA_SPLICERS)
        {
            if (ItemID == IT0629_DNA_SPLICERS)
            {
                KyuremUnfuseCase = PokeList_GetKyuremUnfuseCase(a1);
                if (KyuremUnfuseCase == 5)
                {
                    sub_21A03EC(a1, 2);
                    return result;
                }
                if (KyuremUnfuseCase != 2 && KyuremUnfuseCase != 4)
                {
                    a1->TargetedSlot = a1->SelectedSlot;
                    a1->field_28C->field_50 = 10;
                    a1->field_C = 14;
                    a1->field_274 = 5;
                    result = (PokeList_Party*)2;
                    a1->field_27B = 2;
                    return result;
                }
            }
        LABEL_29:
            sub_21A03DC(a1);
            return result;
        }
        result = (PokeList_Party*)PokeList_GetKyuremFuseCase(a1);
        if (result == (PokeList_Party*)2 || result == (PokeList_Party*)4)
        {
            goto LABEL_29;
        }
        if (result == (PokeList_Party*)3)
        {
            sub_21A03EC(a1, 1);
        }
        else if (!result)
        {
            sub_219CE18(a1, a1->SelectedSlot);
            sub_219CE74(a1, a1->SelectedSlot);
            a1->TargetedSlot = a1->SelectedSlot;
            a1->field_28C->field_50 = 10;
            a1->field_C = 23;
            a1->field_14 = 0;
            sub_219F7AC(a1, a1->pokeList_Message, 0);
            sub_219F880(a1, a1->pokeList_Message, 18);
            sub_219BBA8(a1);
        }
        return result;
    case 6:
        result = (PokeList_Party*)PokeList_GetLearnMoveCase(a1, a1->SelectedPkm, v2->SelectedSlot);
        if (result <= (PokeList_Party*)3)
        {
            result = *(PokeList_Party**)(0x0219B7A6 + result);
            switch ((unsigned int)result)
            {
            case 0u:
                a1->field_28C->field_50 = 10;
                PokeListMessage_CreateWordSetSystem(a1, a1->pokeList_Message);
                PokeListMessage_LoadPokemonNicknameToStrBuf(a1, a1->pokeList_Message, 0, a1->SelectedPkm);
                PokeListMessage_LoadMoveNameToStrBuf(a1, a1->pokeList_Message, 1u, a1->field_28C->MoveToLearn);
                PokeList_PrintMessage(a1, 42, 1, (int)sub_219DE50);
                PokeListMessage_ClearWordSetSystem(a1, a1->pokeList_Message);
                sub_219E5C8(a1, a1->SelectedPkm);
                result = (PokeList_Party*)sub_219E620(a1, (int)a1->SelectedPkm);
                break;
            case 1u:
                PokeListMessage_CreateWordSetSystem(a1, a1->pokeList_Message);
                PokeListMessage_LoadPokemonNicknameToStrBuf(a1, a1->pokeList_Message, 0, a1->SelectedPkm);
                PokeListMessage_LoadMoveNameToStrBuf(a1, a1->pokeList_Message, 1u, a1->field_28C->MoveToLearn);
                PokeList_PrintMessage(a1, 33, 0, (int)sub_219DEC4);
                PokeListMessage_ClearWordSetSystem(a1, a1->pokeList_Message);
                break;
            case 2u:
                a1->field_28C->field_50 = 10;
                PokeListMessage_CreateWordSetSystem(a1, a1->pokeList_Message);
                PokeListMessage_LoadPokemonNicknameToStrBuf(a1, a1->pokeList_Message, 0, a1->SelectedPkm);
                PokeListMessage_LoadMoveNameToStrBuf(a1, a1->pokeList_Message, 1u, a1->field_28C->MoveToLearn);
                PokeList_PrintMessage(a1, 43, 1, (int)sub_219DE50);
                PokeListMessage_ClearWordSetSystem(a1, a1->pokeList_Message);
                break;
            case 3u:
                a1->field_28C->field_50 = 10;
                PokeListMessage_CreateWordSetSystem(a1, a1->pokeList_Message);
                PokeListMessage_LoadPokemonNicknameToStrBuf(a1, a1->pokeList_Message, 0, a1->SelectedPkm);
                PokeListMessage_LoadMoveNameToStrBuf(a1, a1->pokeList_Message, 1u, a1->field_28C->MoveToLearn);
                PokeList_PrintMessage(a1, 44, 1, (int)sub_219DE50);
                PokeListMessage_ClearWordSetSystem(a1, a1->pokeList_Message);
                break;
            default:
                return result;
            }
        }
        return result;
    case 9:
        Param = PokeParty_GetParam(a1->SelectedPkm, PF_Item, 0);
        v10 = Param;
        if (Param)
        {
            if (PML_ItemIsMail(Param) == 1)
            {
                a1->field_28C->field_50 = 10;
                PokeList_PrintMessage(a1, 58, 1, (int)sub_219DE50);
            }
            else
            {
                PokeListMessage_CreateWordSetSystem(a1, a1->pokeList_Message);
                PokeListMessage_LoadPokemonNicknameToStrBuf(a1, a1->pokeList_Message, 0, a1->SelectedPkm);
                sub_219FA58(a1, a1->pokeList_Message, 1, v10);
                PokeList_PrintMessage(a1, 59, 0, (int)sub_219E0AC);
                PokeListMessage_ClearWordSetSystem(a1, a1->pokeList_Message);
            }
        }
        else if (PML_ItemIsMail(a1->field_28C->ItemID) == 1)
        {
            a1->field_C = 19;
            a1->field_28C->SelectedSlot = a1->SelectedSlot;
            result = a1->field_28C;
            result->field_50 = 6;
        }
        else
        {
            a1->field_28C->field_50 = 10;
            PokeList_ChangeHeldItem(a1, a1->SelectedPkm, a1->field_28C->ItemID);
            sub_219F350(a1, a1->pokeList_Plate[a1->SelectedSlot]);
            PokeListMessage_CreateWordSetSystem(a1, a1->pokeList_Message);
            PokeListMessage_LoadPokemonNicknameToStrBuf(a1, a1->pokeList_Message, 0, a1->SelectedPkm);
            PokeListMessage_LoadItemNameToStrBuf(a1, a1->pokeList_Message, 1, a1->field_28C->ItemID);
            PokeList_ChangeArceusForme(a1, a1->SelectedPkm, a1->field_28C->ItemID);
            PokeList_ChangeGenesectForme(a1, a1->SelectedPkm, a1->field_28C->ItemID);
            if (PokeList_DoesGiratinaNeedFormeChange(a1))
            {
                PokeList_ChangeGiratinaToOriginForme(a1, a1->SelectedPkm);
                PokeList_PrintMessage(a1, 92, 1, (int)sub_219E4F8);
                a1->field_274 = 1;
            }
            else
            {
                PokeList_PrintMessage(a1, 92, 1, (int)sub_219DE50);
            }
            PokeListMessage_ClearWordSetSystem(a1, a1->pokeList_Message);
        }
        return result;
    case 0xE:
        if (PokeParty_GetParam(a1->SelectedPkm, PF_Item, 0))
        {
            PokeList_PrintMessage(a1, 109, 1, (int)sub_219DE50);
        }
        else
        {
            v11 = sub_20097F4(a1->field_28C->field_8, 0, a1->field_28C->field_6C, a1->heapID);
            PokeParty_SetParam(a1->SelectedPkm, PF_Mail, (u32)v11);
            GFL_HeapFree(v11);
            PokeParty_SetParam(a1->SelectedPkm, PF_Item, a1->field_28C->ItemID);
            sub_20097D0(a1->field_28C->field_8, 0, a1->field_28C->field_6C);
            PokeList_PrintMessage(a1, 108, 1, (int)sub_219DE50);
        }
        return result;
    case 0x10:
        if (PokeList_CheckEvolveSpecies(a1, (int)a1->SelectedPkm, v2->ItemID))
        {
            PokeList_SubItem(a1, a1->field_28C->ItemID);
            a1->field_C = 19;
            a1->field_28C->SelectedSlot = a1->SelectedSlot;
            result = a1->field_28C;
            result->field_50 = 8;
        }
        else
        {
            PokeList_PrintMessage(a1, 82, 1, (int)sub_219DE50);
        }
        return result;
    default:
        goto LABEL_65;
    }
}
*/