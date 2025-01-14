#include "settings.h"
#include "GameVariables.h"
#include "BattleUpgrade.h"

#include "include/server_flow.h"
#include "include/server_events.h"
#include "include/handler_params.h"

#include "include/moves.h"
#include "include/battle_events.h"
#include "include/types.h"
#include "include/abilities.h"

#if ADD_LEVEL_CAP || DISABLE_EVS

extern "C" u32 CalcBaseExpGain(BattleMon * defeatedMon, u32 keySystemLevelAdjust);
extern "C" u32 ScaleExpGainedByLevel(BattleMon * battleMon, u32 expToAdd, u32 currentLevel, u32 defeatedMonLevel);
extern "C" u32 PassPower_ApplyEXP(u32 baseExp);
extern "C" void AddEVs(BattleMon * battleMon, BattleMon * defeatedMon, CalcExpWork * calcExpWork);
extern "C" u32 PML_UtilGetPkmLvExp(u16 species, u16 form, int level);

extern "C" void THUMB_BRANCH_SAFESTACK_AddExpAndEVs(ServerFlow * serverFlow, BattleParty * party, BattleMon * defeatedMon, CalcExpWork * partyCalcExpWork) {
    BtlSetup* btlSetup = MainModule_GetBtlSetup(serverFlow->mainModule);

    u32 baseExp = CalcBaseExpGain(defeatedMon, btlSetup->keySystemLevelAdjust);
    if (MainModule_GetBattleType(serverFlow->mainModule) == BtlType_Trainer) {
        baseExp = 15 * baseExp / 10;
    }

    u32 currExpSlot = 0;
    while (currExpSlot < 6) {
        sys_memset(&partyCalcExpWork[currExpSlot], 0, 12);
        ++currExpSlot;
    }

    u32 monsInParty = BattleParty_GetPartyCount(party);
    u32 monsWithExpShare = 0;
    if (monsInParty) {
        u32 currMonInParty = 0;

        while (currMonInParty < monsInParty) {
            BattleMon* currPartyBattleMon = BattleParty_GetMonData(party, currMonInParty);

            if (!BattleMon_IsFainted(currPartyBattleMon) &&
                BattleMon_GetHeldItem(currPartyBattleMon) == IT0216_EXP_SHARE) {
                ++monsWithExpShare;
            }
            ++currMonInParty;
        }
    }

    if (monsWithExpShare) {
        u32 halfExp = baseExp / 2;
        baseExp -= halfExp;

        u32 splitSharedExp = halfExp / monsWithExpShare;
        if (!splitSharedExp) {
            splitSharedExp = 1;
        }

        for (u32 currMonInParty = 0; currMonInParty < monsInParty; ++currMonInParty) {
            BattleMon* currPartyBattleMon = BattleParty_GetMonData(party, currMonInParty);

            if (!BattleMon_IsFainted(currPartyBattleMon) &&
                BattleMon_GetHeldItem(currPartyBattleMon) == IT0216_EXP_SHARE) {
                partyCalcExpWork[currMonInParty].level = splitSharedExp;
            }
        }
    }

    u32 confrontedMons = BattleMon_GetConfrontRecCount(defeatedMon);
    u32 monsToShareExp = 0;
    if (confrontedMons > 0) {
        u32 currConfrontedMon = 0;

        while (currConfrontedMon < confrontedMons) {
            u32 confrontedSlot = BattleMon_GetConfrontedPokeID(defeatedMon, currConfrontedMon);

            BattleMon* confrontedMon = PokeCon_GetBattleMon(serverFlow->pokeCon, confrontedSlot);
            if (!BattleMon_IsFainted(confrontedMon)) {
                ++monsToShareExp;
            }
            ++currConfrontedMon;
        }
    }

    u32 splitBaseExp = baseExp / monsToShareExp;
    if (!(baseExp / monsToShareExp)) {
        splitBaseExp = 1;
    }

    for (u32 i = 0; i < monsInParty; ++i) {
        BattleMon* partyMon = BattleParty_GetMonData(party, i);

        if (!BattleMon_IsFainted(partyMon)) {
            int partySlot = BattleMon_GetID(partyMon);

            for (int currConfrontedMon = 0; currConfrontedMon < confrontedMons; ++currConfrontedMon) {
                if (partySlot == BattleMon_GetConfrontedPokeID(defeatedMon, currConfrontedMon)) {
                    partyCalcExpWork[i].level += splitBaseExp;
                }
            }
        }
    }

    for (u32 i = 0; i < monsInParty; i = ++i) {
        CalcExpWork* calcExpWork = &partyCalcExpWork[i];

        if (calcExpWork->level) {
            BattleMon* partyMon = BattleParty_GetMonData(party, i);

            u32 level = BattleMon_GetValue(partyMon, VALUE_LEVEL);
            u32 defeatedMonLevel = BattleMon_GetValue(defeatedMon, VALUE_LEVEL);
            if (btlSetup->keySystemLevelAdjust < 0) {
                defeatedMonLevel -= btlSetup->keySystemLevelAdjust;
            }
            calcExpWork->level = ScaleExpGainedByLevel(partyMon, calcExpWork->level, level, defeatedMonLevel);

            PartyPkm* partyPkm = BattleMon_GetSrcData(partyMon);
            void* trainerInfo = MainModule_GetPlayerStatus(serverFlow->mainModule);
            if (!IsTrainerOT(&partyPkm->Base, trainerInfo)) {
                u32 region = PokeParty_GetParam(partyPkm, PF_Region, 0);
                u32 expRatio = 0;

                if (region == TrainerInfo_GetRegion(trainerInfo)) {
                    expRatio = 6144;
                }
                else {
                    expRatio = 6963;
                }
                calcExpWork->level = fixed_round(calcExpWork->level, expRatio);
                calcExpWork->isBoosted = 1;
            }
            if (BattleMon_GetHeldItem(partyMon) == IT0231_LUCKY_EGG) {
                calcExpWork->level = fixed_round(calcExpWork->level, 6144);
                calcExpWork->isBoosted = 1;
            }
            u32 passPowerExp = PassPower_ApplyEXP(calcExpWork->level);
            calcExpWork->level = passPowerExp;
            if (passPowerExp > 100000) {
                calcExpWork->level = 100000;
            }

#if ADD_LEVEL_CAP
            u8 lvlCap = (u8)GetLvlCap();
            u8 currentLevel = partyMon->level;
            if (currentLevel >= lvlCap) {
                sys_memset(&partyCalcExpWork[i], 0, 12u);
            }
            else {
                u32 expToCap = PML_UtilGetPkmLvExp(partyMon->species, partyMon->form, lvlCap);
                expToCap -= partyMon->experience;

                if (partyCalcExpWork[i].level > expToCap) {
                    partyCalcExpWork[i].level = expToCap;
                }
            }
#endif
        }
    }

#if !DISABLE_EVS
    for (u32 i = 0; i < monsInParty; i = ++i) {
        if (partyCalcExpWork[i].level) {
            BattleMon* partyMon = BattleParty_GetPartyMember(party, i);
            AddEVs(partyMon, defeatedMon, &partyCalcExpWork[i]);
        }
    }
#endif
}

#endif // ADD_LEVEL_CAP || GIVE_EVS

#if DYNAMIC_SPEED

#define ACTION_ORDER_GET_PRIO(actionOrder) (actionOrder[i].field_8 >> 16) & 0x3FFFFF
#define ACTION_ORDER_GET_SPECIAL_PRIO(actionOrder) ((actionOrder[i].field_8 >> 13) & 0x7)

extern "C" u32 j_j_FaintRecord_GetCount_1(FaintRecord* faintRecord, u32 turn);
extern "C" u32 Handler_IsPosOpenForRevivedMon(ServerFlow* serverFlow);
extern "C" void SortActionOrderBySpeed(ServerFlow* serverFlow, ActionOrderWork* actionOrder, u32 remainingActions);
extern "C" b32 ActionOrder_InterruptReserve(ServerFlow * serverFlow, u32 pokemonSlot);
extern "C" u32 ActionOrder_SendToLast(ServerFlow * serverFlow, u32 pokemonSlot);

// Records if an InterruptAction event (After You) was triggered in the current turn
// - Used in [ServerFlow_ActOrderProcMain] & [BattleHandler_InterruptAction]
u8 interruptActionFlag = 0;

// Records the battleslot of the mons that have been sent last (Quash) in the current turn
// - Used in [PokeSet_SortBySpeedDynamic] & [BattleHandler_SendLast]
u8 sendLastSlots[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

extern "C" void SwapPokemonOrder(ActionOrderWork* actionOrder, u16* speedStats, u8* priority, u8* eventPriority, u8 slowIdx, u8 fastIdx) {
    BattleMon* bufferMon = actionOrder[fastIdx].battleMon;
    BattleActionParam bufferAction = actionOrder[fastIdx].action;
    u32 buffer_field_8 = actionOrder[fastIdx].field_8;
    u8 buffer_field_C = actionOrder[fastIdx].field_C;
    u8 buffer_fDone = actionOrder[fastIdx].fDone;
    u8 buffer_field_E = actionOrder[fastIdx].field_E;
    u8 buffer_field_F = actionOrder[fastIdx].field_F;
    u16 bufferSpeed = speedStats[fastIdx];
    u8 bufferPriority = priority[fastIdx];
    u8 bufferEventPriority = eventPriority[fastIdx];

    actionOrder[fastIdx].battleMon = actionOrder[slowIdx].battleMon;
    actionOrder[slowIdx].battleMon = bufferMon;

    actionOrder[fastIdx].action = actionOrder[slowIdx].action;
    actionOrder[slowIdx].action = bufferAction;

    actionOrder[fastIdx].field_8 = actionOrder[slowIdx].field_8;
    actionOrder[slowIdx].field_8 = buffer_field_8;

    actionOrder[fastIdx].field_C = actionOrder[slowIdx].field_C;
    actionOrder[slowIdx].field_C = buffer_field_C;

    actionOrder[fastIdx].fDone = actionOrder[slowIdx].fDone;
    actionOrder[slowIdx].fDone = buffer_fDone;

    actionOrder[fastIdx].field_E = actionOrder[slowIdx].field_E;
    actionOrder[slowIdx].field_E = buffer_field_E;

    actionOrder[fastIdx].field_F = actionOrder[slowIdx].field_F;
    actionOrder[slowIdx].field_F = buffer_field_F;

    speedStats[fastIdx] = speedStats[slowIdx];
    speedStats[slowIdx] = bufferSpeed;

    priority[fastIdx] = priority[slowIdx];
    priority[slowIdx] = bufferPriority;

    eventPriority[fastIdx] = eventPriority[slowIdx];
    eventPriority[slowIdx] = bufferEventPriority;
}

extern "C" void PokeSet_SortBySpeedDynamic(ServerFlow* serverFlow, ActionOrderWork* actionOrder, u8 firstIdx, u8 turnStart) {
    // Skip the Pokémons that have already moved.
    u8 startIdx = firstIdx;
    if (!turnStart)
        startIdx += 1;

    // "firstIdx" is the latest Pokémon to move.
    u8 pokeAmount = serverFlow->numActOrder - startIdx;

    // Reset the send last slots at the start of the turn.
    if (startIdx == 0)
        sys_memset(sendLastSlots, 0xFF, 6);

    if (pokeAmount > 1) {
        u16 speedStats[6];
        sys_memset(speedStats, 0, 12);

        u8 priority[6];
        sys_memset(priority, 0, 6);

        // 7 is the default, 6-0 are reserved for quash and 8 for escaping battle.
        u8 eventPriority[6];
        sys_memset(eventPriority, 7, 6);

        for (u8 i = startIdx; i < serverFlow->numActOrder; ++i) {
            if (!BattleMon_IsFainted(actionOrder[i].battleMon)) {
                speedStats[i] = (u16)ServerEvent_CalculateSpeed(serverFlow, actionOrder[i].battleMon, 1);

                priority[i] = ACTION_ORDER_GET_PRIO(actionOrder);
                // Special priority takes into account item & ability prio boosts (1 = no added prio).
                int specialPriority = ACTION_ORDER_GET_SPECIAL_PRIO(actionOrder);
                priority[i] += (specialPriority - 1);

                for (u8 j = 0; j < 6; ++j) {
                    if (sendLastSlots[j] == 0xFF)
                        break;

                    // If the mon has been sent last we give them a
                    // "quash speed" from 6 - 0 (non-quashed mons have 7)
                    if (sendLastSlots[j] == actionOrder[i].battleMon->battleSlot) 
                        eventPriority[i] = 6 - j;
                    // We don't break after finding a match, a mon can be quashed
                    // multiple times and the only one that matters is the last one
                }

                // This means the mon is making an escape attempt.
                // This makes it first, and overrides any other move priority.
                if (BattleAction_GetAction(&actionOrder[i].action) == 4) 
                    eventPriority[i] = 8; 
            }
            else {
                priority[i] = 0xFF;
            }
        }

        // We randomize the initial order to resolve speed ties.
        for (u8 i = startIdx; i < serverFlow->numActOrder; ++i) {
            u8 randomSpot = startIdx + (u8)BattleRandom(pokeAmount);

            if (randomSpot != i)
                SwapPokemonOrder(actionOrder, speedStats, priority, eventPriority, i, randomSpot);
        }

        u8 changes = 1;
        while (changes != 0) {
            changes = 0;

            for (u8 i = startIdx; i < serverFlow->numActOrder; ++i) {
                // Check if the first Pokémon is fainted.
                if (priority[i] != 0xFF) {
                    if (i + 1 < serverFlow->numActOrder) {
                        for (u8 j = i + 1; j < serverFlow->numActOrder; ++j) {
                            // Check if the second Pokémon is fainted.
                            if (priority[j] != 0xFF) {
                                u8 poke1EventPriority = eventPriority[i];
                                u8 poke2EventPriority = eventPriority[j];

                                if (poke2EventPriority > poke1EventPriority) {
                                    SwapPokemonOrder(actionOrder, speedStats, priority, eventPriority, i, j);
                                    ++changes;
                                }
                                else if (poke2EventPriority == poke1EventPriority) {
                                    u8 poke1Priority = priority[i];
                                    u8 poke2Priority = priority[j];

                                    if (poke2Priority > poke1Priority) {
                                        SwapPokemonOrder(actionOrder, speedStats, priority, eventPriority, i, j);
                                        ++changes;
                                    }
                                    else if (poke2Priority == poke1Priority) {
                                        u16 poke1Speed = speedStats[i];
                                        u16 poke2Speed = speedStats[j];

                                        if (poke2Speed > poke1Speed) {
                                            SwapPokemonOrder(actionOrder, speedStats, priority, eventPriority, i, j);
                                            ++changes;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

#if DYNAMIC_SPEED_DEBUG
        DPRINTF("ORDER(%d):", startIdx);
        for (u8 i = startIdx; i < serverFlow->numActOrder; ++i)
            DPRINTF("%d.", actionOrder[i].battleMon->battleSlot);
        DPRINTF("\nSPEED(%d):", startIdx);
        for (u8 i = startIdx; i < serverFlow->numActOrder; ++i)
            DPRINTF("%d.", speedStats[i]);
        DPRINTF("\nPRIORITY(%d):", startIdx);
        for (u8 i = startIdx; i < serverFlow->numActOrder; ++i)
            DPRINTF("%d.", priority[i]);
        DPRINTF("\nEVENT(%d):", startIdx);
        for (u8 i = startIdx; i < serverFlow->numActOrder; ++i)
            DPRINTF("%d.", eventPriority[i]);
        DPRINT("\n\n");
#endif
    }
}

extern "C" int THUMB_BRANCH_ServerFlow_ActOrderProcMain(ServerFlow* serverFlow, u32 currentActionIdx) {
    u32 procAction = 0;
    if (currentActionIdx >= (u32)serverFlow->numActOrder) {
    END_ACTION_ORDER:
        if (!serverFlow->flowResult) {
            u32 turnCheck = ServerControl_TurnCheck(serverFlow);

            if (ServerControl_CheckMatchup(serverFlow)) {
                serverFlow->flowResult = 4;
                return serverFlow->numActOrder;
            }
            if (turnCheck) {
                serverFlow->flowResult = 3;
                return serverFlow->numActOrder;
            }

            u32 faintedCount = j_j_FaintRecord_GetCount_1(&serverFlow->faintRecord, 0);
            if (Handler_IsPosOpenForRevivedMon(serverFlow) || faintedCount) {
                ServerFlow_ReqChangePokeForServer(serverFlow, &serverFlow->field_4CE);
                ServerDisplay_IllusionSet(serverFlow, &serverFlow->field_4CE);
                serverFlow->flowResult = 2;
                return serverFlow->numActOrder;
            }
            serverFlow->flowResult = 0;
        }
        return serverFlow->numActOrder;
    }
    else
    {
        ActionOrderWork* actionOrderWork = serverFlow->actionOrderWork;

        PokeSet_SortBySpeedDynamic(serverFlow, actionOrderWork, (u8)currentActionIdx, 1); // pre-calc speed to take into account switch-in speed modifiers (weather...)

        while (1) {
            // [1 = fight] [2 = item] [3 = switch] [4 = run]
            u32 action = BattleAction_GetAction(&actionOrderWork[currentActionIdx].action);
            if (procAction == 6 && action != 6) {
                ServerControl_CheckActivation(serverFlow);
                SortActionOrderBySpeed(serverFlow, &actionOrderWork[currentActionIdx], (u32)serverFlow->numActOrder - currentActionIdx);
            }

            procAction = ActionOrder_Proc(serverFlow, &actionOrderWork[currentActionIdx]);

            if (interruptActionFlag != 1) // skip speed calcs if after you was used
                PokeSet_SortBySpeedDynamic(serverFlow, actionOrderWork, (u8)currentActionIdx, 0); // re-calc speed
            interruptActionFlag = 0;

            u32 getExp = ServerControl_CheckExpGet(serverFlow);
            b32 matchup = ServerControl_CheckMatchup(serverFlow);

            if (matchup) {
                serverFlow->flowResult = 4;
                return currentActionIdx + 1;
            }
            if (serverFlow->flowResult == 6 || serverFlow->flowResult == 1) {
                return currentActionIdx + 1;
            }
            if (getExp) {
                break;
            }
            if (++currentActionIdx >= (u32)serverFlow->numActOrder) {
                goto END_ACTION_ORDER;
            }
        }

        serverFlow->flowResult = 3;
        return currentActionIdx + 1;
    }
}

extern "C" u32 THUMB_BRANCH_BattleHandler_InterruptAction(ServerFlow* serverFlow, HandlerParam_InterruptPoke* params) {
    if (!ActionOrder_InterruptReserve(serverFlow, params->pokeID))
    {
        return 0;
    }
    BattleHandler_SetString(serverFlow, &params->exStr);

    interruptActionFlag = 1;
    return 1;
}

extern "C" u32 THUMB_BRANCH_BattleHandler_SendLast(ServerFlow* serverFlow, HandlerParam_SendLast* params) {
    if (!ActionOrder_SendToLast(serverFlow, params->pokeID)) {
        return 0;
    }
    BattleHandler_SetString(serverFlow, &params->exStr);

    // Add the Pokémon slot to the send last slots.
    for (u8 i = 0; i < 6; ++i) {
        if (sendLastSlots[i] == 0xFF) {
            sendLastSlots[i] = params->pokeID;
            break;
        }
    }

    return 1;
}

#endif // DYNAMIC_SPEED

#if GEN6_CRIT

u8 CRIT_STAGES[5] = { 16, 8, 2, 1, 1 };

extern "C" b32 THUMB_BRANCH_RollCritical(u32 critStage)
{
    return BattleRandom(CRIT_STAGES[critStage]) == 0;
}

#endif // GEN6_CRIT

#if GEN6_CRIT || EXPAND_FIELD_EFFECTS

extern "C" u32 CalcBaseDamage(u32 power, u32 attack, u32 level, u32 defense);
extern "C" u32 WeatherPowerMod(u32 weather, u32 subProcID);
extern "C" u32 TypeEffectivenessPowerMod(u32 damage, u32 typeEffectiveness);

extern "C" u32 THUMB_BRANCH_SAFESTACK_ServerEvent_CalcDamage(ServerFlow * serverFlow, BattleMon * attackingMon, BattleMon * defendingMon,
    MoveParam* moveParam, u32 typeEffectiveness, u32 targetDmgRatio, u32 critFlag, u32 battleDebugMode, u16* destDamage) {
    u32 category = PML_MoveGetCategory(moveParam->moveID);
    u32 isFixedDamage = 0;
    BattleEventVar_Push();
    BattleEventVar_SetConstValue(VAR_TYPE_EFFECTIVENESS, typeEffectiveness);
    u32 attackingSlot = BattleMon_GetID(attackingMon);
    BattleEventVar_SetConstValue(VAR_ATTACKING_MON, attackingSlot);
    u32 defendingSlot = BattleMon_GetID(defendingMon);
    BattleEventVar_SetConstValue(VAR_DEFENDING_MON, defendingSlot);
    BattleEventVar_SetConstValue(VAR_CRITICAL_FLAG, critFlag);
    BattleEventVar_SetConstValue(VAR_MOVE_TYPE, moveParam->moveType);
    BattleEventVar_SetConstValue(VAR_MOVE_ID, moveParam->moveID);
    BattleEventVar_SetConstValue(VAR_MOVE_CATEGORY, category);
    BattleEventVar_SetValue(VAR_FIXED_DAMAGE, 0);
    BattleEvent_CallHandlers(serverFlow, EVENT_MOVE_DAMAGE_PROCESSING_1);

    u32 finalDamage = BattleEventVar_GetValue(VAR_FIXED_DAMAGE);
    if (finalDamage) {
        isFixedDamage = 1;
    }
    else {
        u32 power = ServerEvent_GetMovePower(serverFlow, attackingMon, defendingMon, moveParam);
        u32 attack = ServerEvent_GetAttackPower(serverFlow, attackingMon, defendingMon, moveParam, critFlag);
        u32 defense = ServerEvent_GetTargetDefenses(serverFlow, attackingMon, defendingMon, moveParam, critFlag);
        u8 level = BattleMon_GetValue(attackingMon, VALUE_LEVEL);
        u32 baseDamage = CalcBaseDamage(power, attack, level, defense);
        u32 damage = baseDamage;
        if (targetDmgRatio != 4096) {
            damage = fixed_round(baseDamage, targetDmgRatio);
        }

        u32 weather = ServerEvent_GetWeather(serverFlow);
        u32 weatherDmgRatio = WeatherPowerMod(weather, moveParam->moveType);
        if (weatherDmgRatio != 4096) {
            damage = fixed_round(damage, weatherDmgRatio);
        }

#if EXPAND_FIELD_EFFECTS
        u32 terrain = ServerEvent_GetTerrain(serverFlow);
        u32 terrainDmgRatio = TerrainPowerMod(serverFlow, attackingMon, defendingMon, terrain, moveParam->moveType);
        if (terrainDmgRatio != 4096) {
            damage = fixed_round(damage, terrainDmgRatio);
        }
#endif 
        if (critFlag) {
#if GEN6_CRIT
            damage = (u32)((float)damage * 1.5f);
#else
            damage *= 2;
#endif
        }

        if (!MainModule_GetDebugFlag() && ServerFlow_IsCompetitorScenarioMode(serverFlow)) {
            u32 damageRoll = 85;
            if (!battleDebugMode) {
                damageRoll = (100 - BattleRandom(16u));
            }
            damage = damageRoll * damage / 100;
        }

        PokeType moveType = (PokeType)moveParam->moveType;
        if (moveType != TYPE_NULL) {
            u32 stab = ServerEvent_SameTypeAttackBonus(serverFlow, attackingMon, moveType);
            damage = fixed_round(damage, stab);
        }

        u32 damageAfterType = TypeEffectivenessPowerMod(damage, typeEffectiveness);
        if (category == CATEGORY_PHYSICAL
            && BattleMon_GetStatus(attackingMon) == CONDITION_BURN
            && BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY) != ABIL062_GUTS) {
            damageAfterType = 50 * damageAfterType / 100u;
        }
        if (!damageAfterType) {
            damageAfterType = 1;
        }

        BattleEventVar_SetMulValue(VAR_RATIO, 4096, 41, 0x20000);
        BattleEventVar_SetValue(VAR_DAMAGE, damageAfterType);
        BattleEvent_CallHandlers(serverFlow, EVENT_MOVE_DAMAGE_PROCESSING_2);
        int damageRatio = BattleEventVar_GetValue(VAR_RATIO);
        int damageAfterProc2 = BattleEventVar_GetValue(VAR_DAMAGE);
        finalDamage = fixed_round(damageAfterProc2, damageRatio);
    }
    BattleEvent_CallHandlers(serverFlow, EVENT_MOVE_DAMAGE_PROCESSING_END);
    BattleEventVar_Pop();
    *destDamage = (u16)finalDamage;

    return isFixedDamage;
}
#endif // EXPAND_FIELD_EFFECTS || GEN6_CRIT
