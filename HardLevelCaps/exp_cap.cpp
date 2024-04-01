#include "level_cap.h"

#define GIVE_EVS false

C_DECL_BEGIN
void THUMB_BRANCH_AddExpAndEVs(ServerFlow* serverFlow, BattleParty* party, int DefeatedMon, _DWORD* a4)
{
    CalcExpWork* expEvs = (CalcExpWork*)a4;
    unsigned int MonsWithExpShare; // r7
    unsigned int v6; // r6
    unsigned int count; // r4
    BattleMon* MonData; // r6
    u32 v9; // r0
    int SplitExpSharedExp; // r7
    BattleMon* v12; // r6
    int ConfrontRecCount; // r7
    unsigned int MonsToGainExp; // r6
    int v15; // r4
    int ConfrontedPokeID; // r0
    BattleMon* PokeParam; // r0
    BattleMon* v18; // r4
    int ID; // r0
    unsigned int v20; // r4
    BattleMon* battleMon; // r7
    int DefeatedMonLevel; // r3
    int KeySystemLevelAdjust; // r0
    int v26; // r1
    u32 v27; // r0
    BattleMon* PartyMember; // r0
    int Level; // [sp+14h] [bp-34h]
    PartyPkm* pPkm; // [sp+18h] [bp-30h]
    void* pTrainerInfo; // [sp+1Ch] [bp-2Ch]
    int SplitBaseExp; // [sp+20h] [bp-28h]
    unsigned int NumMonsInParty; // [sp+24h] [bp-24h]
    u32 BaseExp; // [sp+28h] [bp-20h]
    BtlSetup* v42; // [sp+2Ch] [bp-1Ch]
    u32 Region; // [sp+30h] [bp-18h]

    v42 = MainModule_GetBtlSetup_0(serverFlow->mainModule);
    BaseExp = CalcBaseExpGain((BattleMon*)DefeatedMon, (char)v42->KeySystemLevelAdjust);
    NumMonsInParty = BattleParty_GetPartyCount(party);
    MonsWithExpShare = 0;
    if (BtlSetup_GetBattleType(serverFlow->mainModule) == BtlType_Trainer)
    {
        BaseExp = 15 * BaseExp / 10;
    }

    sys_memset(expEvs, 0, 12 * 6);

    count = 0;
    if (NumMonsInParty)
    {
        do
        {
            MonData = BattleParty_GetMonData(party, (unsigned __int8)count);
            if (!BattleMon_IsFainted(MonData) && BattleMon_GetHeldItem(MonData) == IT0216_EXP_SHARE)
            {
                MonsWithExpShare = (unsigned __int16)(MonsWithExpShare + 1);
            }
            count = (unsigned __int16)(count + 1);
        } while (count < NumMonsInParty);
    }
    if (MonsWithExpShare)
    {
        v9 = BaseExp >> 1;
        BaseExp = v9;
        SplitExpSharedExp = v9 / MonsWithExpShare;
        if (!SplitExpSharedExp)
        {
            SplitExpSharedExp = 1;
        }
        for (unsigned int i = 0; i < NumMonsInParty; i = (unsigned __int16)(i + 1))
        {
            v12 = BattleParty_GetMonData(party, (unsigned __int8)i);
            if (!BattleMon_IsFainted(v12) && BattleMon_GetHeldItem(v12) == IT0216_EXP_SHARE)
            {
                expEvs[i].level = SplitExpSharedExp;
            }
        }
    }
    ConfrontRecCount = BattleMon_GetConfrontRecCount((BattleMon*)DefeatedMon);
    MonsToGainExp = 0;
    v15 = 0;
    if (ConfrontRecCount > 0)
    {
        do
        {
            ConfrontedPokeID = BattleMon_GetConfrontedPokeID((BattleMon*)DefeatedMon, (unsigned __int8)v15);
            PokeParam = PokeCon_GetPokeParam(serverFlow->pokeCon, ConfrontedPokeID);
            if (!BattleMon_IsFainted(PokeParam))
            {
                MonsToGainExp = (unsigned __int8)(MonsToGainExp + 1);
            }
            v15 = (unsigned __int16)(v15 + 1);
        } while (v15 < ConfrontRecCount);
    }
    SplitBaseExp = BaseExp / MonsToGainExp;
    if (!SplitBaseExp)
    {
        SplitBaseExp = 1;
    }
    for (unsigned int j = 0; j < NumMonsInParty; j = (unsigned __int16)(j + 1))
    {
        v18 = BattleParty_GetMonData(party, (unsigned __int8)j);
        if (!BattleMon_IsFainted(v18))
        {
            ID = BattleMon_GetID(v18);
            v20 = 0;
            for (unsigned int k = ID; v20 < ConfrontRecCount; v20 = (unsigned __int8)(v20 + 1))
            {
                if (k == BattleMon_GetConfrontedPokeID((BattleMon*)DefeatedMon, v20))
                {
                    expEvs[j].level += SplitBaseExp;
                }
            }
        }
    }
    for (unsigned int m = 0; m < NumMonsInParty; m = (unsigned __int16)(m + 1))
    {
        if (expEvs[m].level)
        {
            pTrainerInfo = MainModule_GetPlayerStatus(serverFlow->mainModule);
            battleMon = BattleParty_GetMonData(party, (unsigned __int8)m);
            pPkm = BattleMon_GetSrcData(battleMon);
            Level = (unsigned __int16)BattleMon_GetValue(battleMon, VALUE_LEVEL);
            DefeatedMonLevel = (unsigned __int16)BattleMon_GetValue((BattleMon*)DefeatedMon, VALUE_LEVEL);
            KeySystemLevelAdjust = (char)v42->KeySystemLevelAdjust;
            if (KeySystemLevelAdjust < 0)
            {
                DefeatedMonLevel = (unsigned __int16)(DefeatedMonLevel - KeySystemLevelAdjust);
            }
            expEvs[m].level = ScaleExpGainedByLevel((int)battleMon, expEvs[m].level, Level, DefeatedMonLevel);
            if (!IsTrainerOT(&pPkm->base, pTrainerInfo))
            {
                Region = PokeParty_GetParam(pPkm, PF_Region, 0);
                if (Region == TrainerInfo_GetRegion(pTrainerInfo))
                {
                    v26 = 6144;
                }
                else
                {
                    v26 = 6963;
                }
                expEvs[m].level = fixed_round(expEvs[m].level, v26);
                expEvs[m].isBoosted = 1;
            }
            if (BattleMon_GetHeldItem(battleMon) == IT0231_LUCKY_EGG)
            {
                expEvs[m].level = fixed_round(expEvs[m].level, 6144);
                expEvs[m].isBoosted = 1;
            }
            v27 = PassPower_ApplyEXP(expEvs[m].level);
            expEvs[m].level = v27;
            if (v27 > 0x186A0)
            {
                expEvs[m].level = 100000;
            }
        }
    }

    u32 lvl_cap = GetCurrentLvlCap();
    for (unsigned int n = 0; n < NumMonsInParty; ++n)
    {
        BattleMon* battle_pokemon = BattleParty_GetMonData(party, n);

        u32 current_level = (u32)battle_pokemon->Level;
        if (current_level >= lvl_cap)
        {
            sys_memset(&expEvs[n], 0, 12u);
            continue;
        }

        u32 exp_to_cap = PML_UtilGetPkmLvExp(battle_pokemon->Species, battle_pokemon->Form, lvl_cap);
        exp_to_cap -= battle_pokemon->Experience;
        if (expEvs[n].level > exp_to_cap)
        {
            expEvs[n].level = exp_to_cap;
        }

        if (GIVE_EVS)
        {
            BattleMon* party_member = BattleParty_GetPartyMember(party, n);
            AddEVs(party_member, (BattleMon*)DefeatedMon, &expEvs[n].hp);
        }
    }
}
C_DECL_END