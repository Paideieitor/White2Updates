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
#include "include/field_effects.h"
#include "include/personal.h"

#if DYNAMIC_SPEED || EXPAND_FIELD_EFFECTS

#define ACTION_ORDER_GET_PRIO(actionOrder, idx) (actionOrder[idx].speed >> 16) & 0x3FFFFF
#define ACTION_ORDER_GET_SPECIAL_PRIO(actionOrder, idx) ((actionOrder[idx].speed >> 13) & 0x7)

#endif // DYNAMIC_SPEED || EXPAND_FIELD_EFFECTS

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

extern "C" u32 j_j_FaintRecord_GetCount_1(FaintRecord * faintRecord, u32 turn);
extern "C" u32 Handler_IsPosOpenForRevivedMon(ServerFlow * serverFlow);
extern "C" void SortActionOrderBySpeed(ServerFlow * serverFlow, ActionOrderWork * actionOrder, u32 remainingActions);
extern "C" b32 ActionOrder_InterruptReserve(ServerFlow * serverFlow, u32 pokemonSlot);
extern "C" u32 ActionOrder_SendToLast(ServerFlow * serverFlow, u32 pokemonSlot);

// Records if an InterruptAction event (After You) was triggered in the current turn
// - Used in [ServerFlow_ActOrderProcMain] & [BattleHandler_InterruptAction]
u8 interruptActionFlag = 0;

// Records the battleslot of the mons that have been sent last (Quash) in the current turn
// - Used in [PokeSet_SortBySpeedDynamic] & [BattleHandler_SendLast]
u8 sendLastSlots[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

extern "C" void SwapPokemonOrder(ActionOrderWork * actionOrder, u16 * speedStats, u8 * priority, u8 * eventPriority, u8 slowIdx, u8 fastIdx) {
    BattleMon* bufferMon = actionOrder[fastIdx].battleMon;
    BattleActionParam bufferAction = actionOrder[fastIdx].action;
    u32 buffer_speed = actionOrder[fastIdx].speed;
    u8 buffer_partyID = actionOrder[fastIdx].partyID;
    u8 buffer_done = actionOrder[fastIdx].done;
    u8 buffer_field_E = actionOrder[fastIdx].field_E;
    u8 buffer_field_F = actionOrder[fastIdx].field_F;
    u16 bufferSpeed = speedStats[fastIdx];
    u8 bufferPriority = priority[fastIdx];
    u8 bufferEventPriority = eventPriority[fastIdx];

    actionOrder[fastIdx].battleMon = actionOrder[slowIdx].battleMon;
    actionOrder[slowIdx].battleMon = bufferMon;

    actionOrder[fastIdx].action = actionOrder[slowIdx].action;
    actionOrder[slowIdx].action = bufferAction;

    actionOrder[fastIdx].speed = actionOrder[slowIdx].speed;
    actionOrder[slowIdx].speed = buffer_speed;

    actionOrder[fastIdx].partyID = actionOrder[slowIdx].partyID;
    actionOrder[slowIdx].partyID = buffer_partyID;

    actionOrder[fastIdx].done = actionOrder[slowIdx].done;
    actionOrder[slowIdx].done = buffer_done;

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

extern "C" void PokeSet_SortBySpeedDynamic(ServerFlow* serverFlow, ActionOrderWork * actionOrder, u8 firstIdx, u8 turnStart) {
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

                priority[i] = ACTION_ORDER_GET_PRIO(actionOrder, i);
                // Special priority takes into account item & ability prio boosts (1 = no added prio).
                int specialPriority = ACTION_ORDER_GET_SPECIAL_PRIO(actionOrder, i);
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

extern "C" u32 THUMB_BRANCH_BattleHandler_InterruptAction(ServerFlow * serverFlow, HandlerParam_InterruptPoke * params) {
    if (!ActionOrder_InterruptReserve(serverFlow, params->pokeID))
    {
        return 0;
    }
    BattleHandler_SetString(serverFlow, &params->exStr);

    interruptActionFlag = 1;
    return 1;
}

extern "C" u32 THUMB_BRANCH_BattleHandler_SendLast(ServerFlow * serverFlow, HandlerParam_SendLast * params) {
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

#if DYNAMIC_SPEED || EXPAND_ABILITIES

#if EXPAND_ABILITIES

// Defines if a switch should be made after the turn has been processed
// The vanilla game does not have logic to switch after all actions have been performed
// like for example after receiving damage from a status condition
// - Set in [HandlerEmergencyExitSwitchEnd]
// - Reset & used in [ServerFlow_ActOrderProcMain]
u8 endTurnSwitchFlag = 0;

#endif

// Stores data of extra action generated by Dancer, Instruct...
// - Set in [HandlerDancerCheckMove]
// - Reset and used in [ServerFlow_ActOrderProcMain]
ActionOrderWork extraActionOrder[6];

// Defines if an extra action is taking place
// - Set and reset in [ServerFlow_ActOrderProcMain]
u8 extraActionFlag = 0;

extern "C" int THUMB_BRANCH_ServerFlow_ActOrderProcMain(ServerFlow * serverFlow, u32 currentActionIdx) {
    u32 procAction = 0;
    ActionOrderWork* actionOrderWork = serverFlow->actionOrderWork;

    for (u8 i = 0; i < 6; ++i) {
        sys_memset(&extraActionOrder[i], 0, sizeof(ActionOrderWork));
    }
    
    // Re-calc speed after switch-ins
    PokeSet_SortBySpeedDynamic(serverFlow, actionOrderWork, (u8)currentActionIdx, 1);
    while (currentActionIdx < serverFlow->numActOrder || extraActionOrder[0].battleMon != nullptr) {
        ActionOrderWork* currentActionOrder = &actionOrderWork[currentActionIdx];
        // Swap the current action if an extra one has been scheduled
        u8 isExtraAction = 0;
        if (extraActionOrder[0].battleMon != nullptr) {
            isExtraAction = 1;
            currentActionOrder = &extraActionOrder[0];

            Turnflag_Clear(currentActionOrder->battleMon, TURNFLAG_ACTIONSTART);
            Turnflag_Clear(currentActionOrder->battleMon, TURNFLAG_ACTIONDONE);
            Turnflag_Clear(currentActionOrder->battleMon, TURNFLAG_MOVEPROCDONE);
            Turnflag_Clear(currentActionOrder->battleMon, TURNFLAG_MOVED);
            Turnflag_Clear(currentActionOrder->battleMon, TURNFLAG_USINGFLING);

            extraActionFlag = 1;
        }

        if (!extraActionFlag) {
            // [1 = fight] [2 = item] [3 = switch] [4 = run]
            u32 action = BattleAction_GetAction(&currentActionOrder->action);
            // I think this recals speed in rotation battles
            if (procAction == 6 && action != 6) {
                ServerControl_CheckActivation(serverFlow);
                SortActionOrderBySpeed(serverFlow, currentActionOrder, (u32)serverFlow->numActOrder - currentActionIdx);
            }
        }
#if EXPAND_ABILITIES
        // Skip action if the Pokémon is invulnerable because of Commander
        b32 isCommander = (!BattleMon_CheckIfMoveCondition(currentActionOrder->battleMon, CONDITION_CHARGELOCK) &&
            BattleMon_GetHideCondition(currentActionOrder->battleMon) == CONDITIONFLAG_SHADOWFORCE);
        if (!isCommander) {
#endif
            procAction = ActionOrder_Proc(serverFlow, currentActionOrder);
#if EXPAND_ABILITIES
        }
#endif
        // Skip speed calcs if After You was used
        if (interruptActionFlag != 1) {
            // Re-calc speed
            PokeSet_SortBySpeedDynamic(serverFlow, actionOrderWork, (u8)currentActionIdx, 0);
        }
        interruptActionFlag = 0;

        // Move the next extra action to the first slot
        if (isExtraAction) {
            for (u8 i = 0; i < 5; ++i) {
                sys_memcpy(&extraActionOrder[i + 1], &extraActionOrder[i], sizeof(ActionOrderWork));
            }
            sys_memset(&extraActionOrder[5], 0, sizeof(ActionOrderWork));
           
            extraActionFlag = 0;
        }

        u32 getExp = ServerControl_CheckExpGet(serverFlow);
        b32 matchup = ServerControl_CheckMatchup(serverFlow);

        // Stop the turn if the battle ends
        if (matchup) {
            serverFlow->flowResult = 4;
            // Don't advance an action if we have an extra action
            if (isExtraAction) {
                return currentActionIdx;
            }
            else {
                return currentActionIdx + 1;
            }
        }

        // Stop the turn if a new Pokémon has to enter the battle
        if (serverFlow->flowResult == 6 || serverFlow->flowResult == 1) {
            // Don't advance an action if we have an extra action
            if (isExtraAction) {
                return currentActionIdx;
            }
            else {
                return currentActionIdx + 1;
            }
        }

        // Stop the turn if a Pokémon died but the battle is not over
        if (getExp) {
            serverFlow->flowResult = 3;
            // Don't advance an action if we have an extra action
            if (isExtraAction) {
                return currentActionIdx;
            }
            else {
                return currentActionIdx + 1;
            }
        }

        // Don't advance an action if we have an extra action
        if (!isExtraAction) {
            ++currentActionIdx;
        }
    }

    // Only TurnCheck if the turn is completely over
    if (!serverFlow->flowResult) {
        u32 turnCheck = ServerControl_TurnCheck(serverFlow);
        
        // Battle is over
        if (ServerControl_CheckMatchup(serverFlow)) {
            serverFlow->flowResult = 4;
            return serverFlow->numActOrder;
        }
        
        // A Pokémon fainted during the TurnCheck
        if (turnCheck) {
            serverFlow->flowResult = 3;
            return serverFlow->numActOrder;
        }

#if EXPAND_ABILITIES
        if (endTurnSwitchFlag) {
            endTurnSwitchFlag = 0;

            // Skip TurnCheck if a Pokémon has to enter the battle during the TurnCheck
            serverFlow->turnCheckSeq = 7;

            serverFlow->flowResult = 1;
            return serverFlow->numActOrder;
        }
#endif

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

#endif // DYNAMIC_SPEED || EXPAND_ABILITIES

#if EXPAND_FIELD_EFFECTS

typedef u32 TERRAIN;
enum Terrain
{
    TERRAIN_NULL = 0,
    TERRAIN_ELECTRIC = 1,
    TERRAIN_GRASSY = 2,
    TERRAIN_MISTY = 3,
    TERRAIN_PSYCHIC = 4,
    TERRAIN_NULLIFY = 5,
};

struct BattleFieldExt
{
    WEATHER weather;
    u32 weatherTurns;
    TERRAIN terrain;
    u32 terrainTurns;
    BattleEventItem* battleEventItems[FIELD_EFFECT_AMOUNT];
    ConditionData conditionData[FIELD_EFFECT_AMOUNT];
    u32 turnCount[FIELD_EFFECT_AMOUNT];
    u32 dependPokeID[FIELD_EFFECT_AMOUNT][6];
    u32 dependPokeCount[FIELD_EFFECT_AMOUNT];
    u32 effectEnableFlags[FIELD_EFFECT_AMOUNT];

#if EXPAND_ABILITIES
    // Stores the ID of an item being forcibly used by [BattleHandler_ForceUseHeldItem]
    // Basically stores the data in [ItemEvent_AddTempItem] so [ServerEvent_EquipTempItem] can use it
    ITEM_ID tempItem;

    // Amount of Pokemon with Aura Break
    u8 auraBreakMons;

    // Amount of Pokemon mantaining current extreme weather
    u8 extremeWeatherMons;

    // Each bit represents a battle slot, 1 means the mon has used Battle Bond and thus can't use it again
    u32 battleBondFlag;

    // Tracks which Emergency Exit Pokémon should switch after the move is over 
    // - Used in [CommonEmergencyExitCheck] & [HandlerEmergencyExitSwitch]
    u32 emergencyExitFlag;

    // Stores the Pokémon that start the battle
    // - Set in [ServerFlow_SetupBeforeFirstTurn]
    u8 firstTurnMons[6] = { BATTLE_MAX_SLOTS };

#if GEN9_INTREPID_SWORD
    // Each bit represents a battle slot, 1 means the mon has used Intrepid Sword and thus can't use it again
    u32 intrepidSwordFlag;
#endif
#if GEN9_DAUNTLESS_SHIELD
    // Each bit represents a battle slot, 1 means the mon has used Dauntless Shield and thus can't use it again
    u32 dauntlessShieldFlag;
#endif

    // Amount of Pokemon Neutralizing Gas
    u8 neutralizingGasMons;

    // Stores the 2 possible paired Dondozos battle slots for Commander
    u8 pairedDondozos[4];

    // Each bit represents a battle slot, 1 means the mon has forced the activation of Protosynthesis or 
    // Quark Drive usign a Booster Energy
    u32 paradoxAbilityFlag;

    // Each bit represents a battle slot, 1 means the mon has already used Supersweet Syrup
    u32 supersweetSyrupFlag;
#endif // EXPAND_ABILITIES
};

// The battle engine stores 2 BattleField structs
//  - One is dyanamically allocated using BattleField_Init
//  - The other one is in a hardcoded memory address [0x21DD928] I don't know when this gets 
//    allocated, this means I can't make room for the extra data this patch adds to the struct.
// This code disregards the hardcoded instance of the data
BattleFieldExt* g_BattleField;

extern "C" BattleFieldExt * BattleField_Init(u16 heapID);
extern "C" void BattleField_InitCore(BattleFieldExt * battleField, WEATHER weather);
extern "C" void BattleField_ClearFactorWork(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect);
extern "C" WEATHER BattleField_GetWeatherCore(BattleFieldExt * battleField);
extern "C" u32 BattleField_GetWeatherTurnsCore(BattleFieldExt * battleField);
extern "C" void BattleField_SetWeatherCore(BattleFieldExt * battleField, WEATHER weather, u32 turns);
extern "C" WEATHER BattleField_TurnCheckWeatherCore(BattleFieldExt * battleField);
extern "C" b32 BattleField_CheckFieldEffectCore(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect);
extern "C" b32 BattleField_AddDependPokeCore(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect, u32 pokemonSlot);
extern "C" b32 BattleField_AddEffectCore(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect, ConditionData conditionData, b32 addEvents);
extern "C" b32 BattleField_RemoveEffectCore(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect);
extern "C" void BattleField_RemoveDependPokeEffectCore(BattleFieldExt * battleField, u32 pokemonSlot);
extern "C" b32 BattleField_CheckImprisonCore(BattleFieldExt * battleField, PokeCon * pokeCon, BattleMon * battleMon, MOVE_ID moveID);
extern "C" void BattleField_TurnCheckCore(BattleFieldExt * battleField, void(*callback)(FIELD_EFFECT, ServerFlow*), ServerFlow * serverFlow);
extern "C" WEATHER BattleField_GetWeather();
extern "C" u32 BattleField_GetWeatherTurns();
extern "C" void BattleField_EndWeather(BattleFieldExt * battleField);
extern "C" void BattleField_SetWeather(WEATHER weather, u32 turns);
extern "C" b32 BattleField_CheckEffect(FIELD_EFFECT fieldEffect);

#if EXPAND_ABILITIES

extern "C" u32 BattleField_GetAuraBreakMons() {
    return g_BattleField->auraBreakMons;
}
extern "C" void BattleField_AddAuraBreakMon() {
    ++g_BattleField->auraBreakMons;
}
extern "C" void BattleField_RemoveAuraBreakMon() {
    --g_BattleField->auraBreakMons;
}

extern "C" u32 BattleField_GetExtremeWeatherMons() {
    return g_BattleField->extremeWeatherMons;
}
extern "C" void BattleField_AddExtremeWeatherMon() {
    ++g_BattleField->extremeWeatherMons;
}
extern "C" void BattleField_RemoveExtremeWeatherMon() {
    --g_BattleField->extremeWeatherMons;
}
extern "C" void BattleField_ResetExtremeWeatherMons() {
    g_BattleField->extremeWeatherMons = 0;
}

extern "C" b32 BattleField_CheckBattleBondFlag(u32 battleSlot) {
    return (g_BattleField->battleBondFlag >> battleSlot) & 1;
}
extern "C" void BattleField_SetBattleBondFlag(u32 battleSlot) {
    g_BattleField->battleBondFlag |= (1 << battleSlot);
}

extern "C" void HandlerAddExtremeWeather(ServerFlow * serverFlow, u32 pokemonSlot, WEATHER weather) {
    if (BattleField_GetExtremeWeatherMons()) {
        if (BattleField_GetWeather() == weather) {
            // If this Extreme Weather is already in the field, just add 1 to the counter
            BattleField_AddExtremeWeatherMon();
            return;
        }
        else {
            // If a different Extreme Weather is already in the field, reset the counter
            BattleField_ResetExtremeWeatherMons();
        }
    }

    HandlerParam_ChangeWeather* changeWeather;
    changeWeather = (HandlerParam_ChangeWeather*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_WEATHER, pokemonSlot);
    changeWeather->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
    changeWeather->weather = weather;
    changeWeather->turn = INFINITE_FIELD_TURNS;
    // This sets the weather in its Extreme form
    changeWeather->field_7 = 1;
    BattleHandler_PopWork(serverFlow, changeWeather);

    // Add 1 to the Extreme Weather counter
    BattleField_AddExtremeWeatherMon();
}
extern "C" void HandlerRemoveExtremeWeather(ServerFlow * serverFlow, u32 pokemonSlot, WEATHER weather) {
    WEATHER currentWeather = BattleField_GetWeather();
    if (currentWeather == weather && BattleField_GetExtremeWeatherMons()) {
        // Substract 1 to the Extreme Weather counter
        BattleField_RemoveExtremeWeatherMon();

        // If there are no more Pokémon invoking Extreme Weather remove it
        if (!BattleField_GetExtremeWeatherMons()) {
            BattleField_EndWeather(g_BattleField);
            // I set the 8th bit in the weather variable to indicate that it is Extreme Weather
            ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_WeatherEnd, weather | 0x80);
            ServerControl_ChangeWeatherAfter(serverFlow, WEATHER_NULL);
        }
    }
}
extern "C" void HandlerExtremeWeatherImmunity(ServerFlow * serverFlow, u32 pokemonSlot, WEATHER weather, POKE_TYPE type, u16 msgID) {
    if (ServerEvent_GetWeather(serverFlow) == weather && BattleField_GetExtremeWeatherMons()) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == type &&
            PML_MoveGetCategory(BattleEventVar_GetValue(VAR_MOVE_ID)) != CATEGORY_STATUS) {
            BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);

            HandlerParam_Message* message;
            message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
            BattleHandler_StrSetup(&message->str, 1u, msgID);
            BattleHandler_AddArg(&message->str, pokemonSlot);
            BattleHandler_PopWork(serverFlow, message);
        }
    }
}

extern "C" b32 CanWeatherBeChanged(ServerFlow * serverFlow, WEATHER weather, u32 turns, u8 isExtreme) {
    if (weather > WEATHER_STRONGWINDS) {
        return 0;
    }

    // Regular weather can't override Extreme weather
    if (BattleField_GetExtremeWeatherMons() && !isExtreme) {
        return 0;
    }

    // Allow the weather to change to it's extreme variant
    if (isExtreme && weather == BattleField_GetWeather()) {
        return 1;
    }

    return weather != BattleField_GetWeather() || turns == INFINITE_FIELD_TURNS && BattleField_GetWeatherTurns() != INFINITE_FIELD_TURNS;
}
extern "C" void ChangeWeather(ServerFlow * serverFlow, WEATHER weather, u32 turns, u8 isExtreme)
{
    BattleField_SetWeather(weather, turns);

    u8 serverWeather = (u8)weather;
    if (isExtreme) {
        // I set the 8th bit in the weather variable to indicate that it is Extreme Weather
        serverWeather |= 0x80;
    }
    ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_WeatherStart, serverWeather, turns);

    ServerControl_ChangeWeatherAfter(serverFlow, weather);
}

extern "C" b32 BattleField_CheckEmergencyExitFlag(u32 battleSlot) {
    return (g_BattleField->emergencyExitFlag >> battleSlot) & 1;
}
extern "C" void BattleField_SetEmergencyExitFlag(u32 battleSlot) {
    g_BattleField->emergencyExitFlag |= (1 << battleSlot);
}
extern "C" void BattleField_ResetEmergencyExitFlag(u32 battleSlot) {
    g_BattleField->emergencyExitFlag &= ~(1 << battleSlot);
}

#if GEN9_INTREPID_SWORD
extern "C" b32 BattleField_CheckIntrepidSwordFlag(u32 battleSlot) {
    return (g_BattleField->intrepidSwordFlag >> battleSlot) & 1;
}
extern "C" void BattleField_SetIntrepidSwordFlag(u32 battleSlot) {
    g_BattleField->intrepidSwordFlag |= (1 << battleSlot);
}
#endif

#if GEN9_DAUNTLESS_SHIELD
extern "C" b32 BattleField_CheckDauntlessShieldFlag(u32 battleSlot) {
    return (g_BattleField->dauntlessShieldFlag >> battleSlot) & 1;
}
extern "C" void BattleField_SetDauntlessShieldFlag(u32 battleSlot) {
    g_BattleField->dauntlessShieldFlag |= (1 << battleSlot);
}
#endif

extern "C" u32 BattleField_GetNeutralizingGasMons() {
    return g_BattleField->neutralizingGasMons;
}
extern "C" void BattleField_AddNeutralizingGasMon() {
    ++g_BattleField->neutralizingGasMons;
}
extern "C" void BattleField_RemoveNeutralizingGasMon() {
    --g_BattleField->neutralizingGasMons;
}

extern "C" void BattleField_AddPairedDondozo(u32 battleSlot, u32* index) {
    for (u32 i = 0; i < 4; ++i) {
        if (g_BattleField->pairedDondozos[i] == BATTLE_MAX_SLOTS) {
            g_BattleField->pairedDondozos[i] = battleSlot;
            *index = i;
            return;
        }
    }
}
extern "C" b32 BattleField_IsDondozoPaired(u32 battleSlot) {
    for (u32 i = 0; i < 4; ++i) {
        if (g_BattleField->pairedDondozos[i] == battleSlot) {
            return 1;
        }
    }
    return 0;
}

extern "C" b32 BattleField_CheckParadoxAbilityFlag(u32 battleSlot) {
    return (g_BattleField->paradoxAbilityFlag >> battleSlot) & 1;
}
extern "C" void BattleField_SetParadoxAbilityFlag(u32 battleSlot) {
    g_BattleField->paradoxAbilityFlag |= (1 << battleSlot);
}
extern "C" void BattleField_ResetParadoxAbilityFlag(u32 battleSlot) {
    g_BattleField->paradoxAbilityFlag &= ~(1 << battleSlot);
}

extern "C" b32 BattleField_CheckSupersweetSyrupFlag(u32 battleSlot) {
    return (g_BattleField->supersweetSyrupFlag >> battleSlot) & 1;
}
extern "C" void BattleField_SetSupersweetSyrupFlag(u32 battleSlot) {
    g_BattleField->supersweetSyrupFlag |= (1 << battleSlot);
}

#endif // EXPAND_ABILITIES

extern "C" TERRAIN BattleField_GetTerrain(BattleFieldExt * battleField) {
    return battleField->terrain;
}
extern "C" u32 BattleField_GetTerrainTurns(BattleFieldExt * battleField) {
    return battleField->terrainTurns;
}
extern "C" void BattleField_SetTerrain(BattleFieldExt * battleField, TERRAIN terrain, u32 turns) {
    battleField->terrain = terrain;
    battleField->terrainTurns = turns;
}
extern "C" void BattleField_EndTerrain(BattleFieldExt * battleField) {
    battleField->terrain = 0;
    battleField->terrainTurns = 0;
}

extern "C" u32 TerrainPowerMod(ServerFlow * serverFlow, BattleMon * attackingMon, BattleMon * defendingMon, TERRAIN terrain, POKE_TYPE type)
{
    switch (terrain) {
    case TERRAIN_ELECTRIC:
        if (type == TYPE_ELEC &&
            !ServerControl_CheckFloating(serverFlow, attackingMon, 1)) {
#if TERRAIN_GEN7_DMG
            return 6.144;
#endif
            return 5325;
        }
        break;
    case TERRAIN_GRASSY:
        if (type == TYPE_GRASS &&
            !ServerControl_CheckFloating(serverFlow, attackingMon, 1)) {
#if TERRAIN_GEN7_DMG
            return 6.144;
#endif
            return 5325;
        }
        break;
    case TERRAIN_MISTY:
        if (type == TYPE_DRAGON && !ServerControl_CheckFloating(serverFlow, defendingMon, 1)) {
            return 2048;
        }
        break;
    case TERRAIN_PSYCHIC:
        if (type == TYPE_PSY &&
            !ServerControl_CheckFloating(serverFlow, attackingMon, 1)) {
#if TERRAIN_GEN7_DMG
            return 6.144;
#endif
            return 5325;
        }
        break;
    }
    return 4096;
}

// Called when setting non-infinite terrain [BattleHandler_AddFieldEffect]
extern "C" u32 ServerEvent_IncreaseMoveTerrainTurns(ServerFlow * serverFlow, TERRAIN terrain, u32 attackingSlot) {
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    BattleEventVar_SetConstValue(NEW_VAR_ATTACKING_MON, attackingSlot);
    BattleEventVar_SetConstValue(VAR_WEATHER, terrain);
    BattleEventVar_SetValue(VAR_EFFECT_TURN_COUNT, 0);
    BattleEvent_CallHandlers(serverFlow, EVENT_MOVE_TERRAIN_TURN_COUNT);
    u32 extraTurns = BattleEventVar_GetValue(VAR_EFFECT_TURN_COUNT);
    BattleEventVar_Pop();
    return extraTurns;
}
// Called before a mon changes the terrain [ServerControl_ChangeTerrain] & [BattleField_TurnCheckCore]
extern "C" b32 ServerEvent_ChangeTerrain(ServerFlow * serverFlow, TERRAIN terrain) {
    b32 failFlag = 0;

    for (u8 i = 0; i < 24; ++i) {

        BattleMon* currentMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
        if (currentMon) {

            u32 currentSlot = BattleMon_GetID(currentMon);

            u32 currentPos = Handler_PokeIDToPokePos(serverFlow, currentSlot);
            if (currentPos != NULL_BATTLE_POS) {
                u32 HEID = HEManager_PushState(&serverFlow->HEManager);

                if (!BattleMon_IsFainted(currentMon)) {
                    BattleEventVar_Push();
                    SET_UP_NEW_EVENT;
                    u32 currentSlot = BattleMon_GetID(currentMon);
                    BattleEventVar_SetConstValue(NEW_VAR_MON_ID, currentSlot);
                    BattleEventVar_SetConstValue(VAR_WEATHER, terrain);
                    BattleEventVar_SetRewriteOnceValue(VAR_MOVE_FAIL_FLAG, 0);
                    BattleEvent_CallHandlers(serverFlow, EVENT_TERRAIN_CHANGE);
                    failFlag = BattleEventVar_GetValue(VAR_MOVE_FAIL_FLAG);
                    if (failFlag) {
                        BattleEvent_CallHandlers(serverFlow, EVENT_TERRAIN_CHANGE_FAIL);
                    }
                    BattleEventVar_Pop();
                }

                HEManager_PopState(&serverFlow->HEManager, HEID);
            }
        }
    }

    return !failFlag;
}
// Called after a mon changes the terrain [BattleHandler_AddFieldEffect]
extern "C" void ServerEvent_ChangeTerrainAfter(ServerFlow* serverFlow, TERRAIN terrain) {
    for (u8 i = 0; i < 24; ++i) {

        BattleMon* currentMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
        if (currentMon) {
            u32 currentSlot = BattleMon_GetID(currentMon);

            u32 currentPos = Handler_PokeIDToPokePos(serverFlow, currentSlot);
            if (currentPos != NULL_BATTLE_POS) {
                int HEID = HEManager_PushState(&serverFlow->HEManager);
                if (!BattleMon_IsFainted(currentMon)) {
                    BattleEventVar_Push();
                    SET_UP_NEW_EVENT;
                    BattleEventVar_SetConstValue(NEW_VAR_MON_ID, currentSlot);
                    BattleEventVar_SetConstValue(VAR_WEATHER, terrain);
                    BattleEvent_CallHandlers(serverFlow, EVENT_AFTER_TERRAIN_CHANGE);
                    BattleEventVar_Pop();
                }
                HEManager_PopState(&serverFlow->HEManager, HEID);
            }
        }
    }
}
// Called when trying to get the terrain [HandlerSurgeSurfer]
extern "C" TERRAIN ServerEvent_GetTerrain(ServerFlow* serverFlow) {
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    BattleEventVar_SetRewriteOnceValue(VAR_MOVE_FAIL_FLAG, 0);
    BattleEvent_CallHandlers(serverFlow, EVENT_TERRAIN_CHECK);
    int failFlag = BattleEventVar_GetValue(VAR_MOVE_FAIL_FLAG);
    BattleEventVar_Pop();
    if (failFlag)
        return 0;
    return BattleField_GetTerrain(g_BattleField);
}
// Called when a mon is grounded by the Gravity effect [BattleHandler_GravityCheck]
extern "C" void ServerEvent_GroundedByGravity(ServerFlow * serverFlow, BattleMon * battleMon) {
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    u32 pokemonSlot = BattleMon_GetID(battleMon);
    BattleEventVar_SetConstValue(NEW_VAR_MON_ID, pokemonSlot);
    BattleEvent_CallHandlers(serverFlow, EVENT_GROUNDED_BY_GRAVITY);
    BattleEventVar_Pop();
}

extern "C" b32 ServerControl_ChangeTerrainCheck(ServerFlow * serverFlow, TERRAIN terrain, u32 turns) {
    if (terrain > TERRAIN_PSYCHIC) {
        return 0;
    }

    return terrain != BattleField_GetTerrain(g_BattleField) ||
        turns == INFINITE_FIELD_TURNS && BattleField_GetTerrainTurns(g_BattleField) != INFINITE_FIELD_TURNS;
}
extern "C" u32 ServerControl_ChangeTerrain(ServerFlow * serverFlow, TERRAIN terrain, u32 turns) {
    if (!ServerControl_ChangeTerrainCheck(serverFlow, terrain, turns)) {
        return 0;
    }

    if (!ServerEvent_ChangeTerrain(serverFlow, terrain)) {
        return 0;
    }

    BattleField_SetTerrain(g_BattleField, terrain, turns);
    return 1;
}

extern "C" BattleEventHandlerTableEntry * EventAddFieldWeather(u32 * handlerAmount)
{
    *handlerAmount = 1;
    return (BattleEventHandlerTableEntry*)0x0689D7C0;
}
extern "C" BattleEventHandlerTableEntry * EventAddFieldTrickRoom(u32 * handlerAmount)
{
    *handlerAmount = 1;
    return (BattleEventHandlerTableEntry*)0x0689D7B8;
}
extern "C" BattleEventHandlerTableEntry * EventAddFieldGravity(u32 * handlerAmount)
{
    *handlerAmount = 2;
    return (BattleEventHandlerTableEntry*)0x0689D7C8;
}
extern "C" BattleEventHandlerTableEntry * EventAddFieldImprison(u32 * handlerAmount)
{
    *handlerAmount = 1;
    return (BattleEventHandlerTableEntry*)0x0689D7A0;
}
extern "C" BattleEventHandlerTableEntry * EventAddFieldWaterSport(u32 * handlerAmount)
{
    *handlerAmount = 1;
    return (BattleEventHandlerTableEntry*)0x0689D7A8;
}
extern "C" BattleEventHandlerTableEntry * EventAddFieldMudSport(u32 * handlerAmount)
{
    *handlerAmount = 1;
    return (BattleEventHandlerTableEntry*)0x0689D798;
}
extern "C" BattleEventHandlerTableEntry * EventAddFieldWonderRoom(u32 * handlerAmount)
{
    *handlerAmount = 1;
    return (BattleEventHandlerTableEntry*)0x0689D7B0;
}
extern "C" BattleEventHandlerTableEntry * EventAddFieldMagicRoom(u32 * handlerAmount)
{
    *handlerAmount = 1;
    return (BattleEventHandlerTableEntry*)0x0689D790;
}

// TERRAIN FIELD
// STATUS PREVENTION TERRAIN EFFECTS (ELECTRIC & MISTY)
extern "C" void HandlerTerrainPreventStatus(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    TERRAIN terrain = BattleField_GetTerrain(g_BattleField);
    if (terrain != TERRAIN_ELECTRIC && terrain != TERRAIN_MISTY) {
        return;
    }

    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_DEFENDING_MON));
    if (!ServerControl_CheckFloating(serverFlow, currentMon, 1)) {
        CONDITION condition = BattleEventVar_GetValue(VAR_CONDITION_ID);
        if (terrain == TERRAIN_ELECTRIC) {
            if (condition == CONDITION_SLEEP ||
                condition == CONDITION_YAWN) {
                *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
            }
        }
        else if (terrain == TERRAIN_MISTY) {
            if (condition == CONDITION_PARALYSIS ||
                condition == CONDITION_SLEEP ||
                condition == CONDITION_FREEZE ||
                condition == CONDITION_BURN ||
                condition == CONDITION_POISON ||
                condition == CONDITION_CONFUSION ||
                condition == CONDITION_YAWN)
            {
                *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
            }
        }
    }
}
extern "C" void HandlerTerrainStatusFailMessage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (*work) {
        TERRAIN terrain = BattleField_GetTerrain(g_BattleField);
        if (terrain != TERRAIN_ELECTRIC && terrain != TERRAIN_MISTY) {
            return;
        }

        u16 msgID = BATTLE_ELECTRIC_TERRAIN_STATUS_MSGID;
        if (terrain == TERRAIN_MISTY)
            msgID = BATTLE_MISTY_TERRAIN_STATUS_MSGID;

        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        HandlerParam_Message* message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, BATTLE_MAX_SLOTS);
        BattleHandler_StrSetup(&message->str, 2u, msgID);
        BattleHandler_AddArg(&message->str, defendingSlot);
        BattleHandler_PopWork(serverFlow, message);
    }
}
// ELECTRIC TERRAIN
extern "C" void CommonElectricTerrainCureStatus(ServerFlow * serverFlow, u32 pokemonSlot) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_SLEEP)) {
        if (!ServerControl_CheckFloating(serverFlow, currentMon, 1)) {
            HandlerParam_CureCondition* cureCondition;
            cureCondition = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
            cureCondition->condition = CONDITION_SLEEP;
            cureCondition->pokeCount = 1;
            cureCondition->pokeID[0] = pokemonSlot;
            BattleHandler_PopWork(serverFlow, cureCondition);
        }
    }
}
extern "C" void HandlerElectricTerrainCheckSleep(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (BattleField_GetTerrain(g_BattleField) != TERRAIN_ELECTRIC) {
        return;
    }

    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (!ServerControl_CheckFloating(serverFlow, currentMon, 1)) {
            BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
        }
    }
}
extern "C" void HandlerElectricTerrainTerrainChange(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;

    if (BattleField_GetTerrain(g_BattleField) != TERRAIN_ELECTRIC) {
        return;
    }

    CommonElectricTerrainCureStatus(serverFlow, BattleEventVar_GetValue(NEW_VAR_MON_ID));
}
extern "C" void HandlerElectricTerrainSwitchIn(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (BattleField_GetTerrain(g_BattleField) != TERRAIN_ELECTRIC) {
        return;
    }

    CommonElectricTerrainCureStatus(serverFlow, BattleEventVar_GetValue(VAR_MON_ID));
}
extern "C" void HandlerElectricTerrainActProcEnd(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (BattleField_GetTerrain(g_BattleField) != TERRAIN_ELECTRIC) {
        return;
    }

    CommonElectricTerrainCureStatus(serverFlow, BattleEventVar_GetValue(VAR_MON_ID));
}
extern "C" void HandlerElectricTerrainFloatingChange(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (BattleField_GetTerrain(g_BattleField) != TERRAIN_ELECTRIC) {
        return;
    }

    CommonElectricTerrainCureStatus(serverFlow, BattleEventVar_GetValue(VAR_MON_ID));
}
extern "C" void HandlerElectricTerrainFloatingChangeNEW(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;

    if (BattleField_GetTerrain(g_BattleField) != TERRAIN_ELECTRIC) {
        return;
    }

    CommonElectricTerrainCureStatus(serverFlow, BattleEventVar_GetValue(NEW_VAR_MON_ID));
}
// GRASSY TERRAIN
extern "C" void HandlerGrassyTerrainHeal(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work)
{
    if (BattleField_GetTerrain(g_BattleField) != TERRAIN_GRASSY) {
        return;
    }

    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
    if (!ServerControl_CheckFloating(serverFlow, currentMon, 1)) {
        if (!BattleMon_IsFullHP(currentMon)) {
            HandlerParam_RecoverHP* recoveHP;
            recoveHP = (HandlerParam_RecoverHP*)BattleHandler_PushWork(serverFlow, EFFECT_RECOVER_HP, currentSlot);
            recoveHP->pokeID = currentSlot;
            recoveHP->recoverHP = DivideMaxHPZeroCheck(currentMon, 16);
            BattleHandler_StrSetup(&recoveHP->exStr, 2u, 387);
            BattleHandler_AddArg(&recoveHP->exStr, currentSlot);
            BattleHandler_PopWork(serverFlow, recoveHP);
        }
    }
}
extern "C" void HandlerGrassyTerrainQuakeMoves(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (BattleField_GetTerrain(g_BattleField) != TERRAIN_GRASSY) {
        return;
    }

    MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
    if (moveID == MOVE089_EARTHQUAKE ||
        moveID == MOVE222_MAGNITUDE ||
        moveID == MOVE523_BULLDOZE) {

        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_DEFENDING_MON));
        if (!ServerControl_CheckFloating(serverFlow, defendingMon, 1)) {
            u32 power = BattleEventVar_GetValue(VAR_MOVE_POWER);
            BattleEventVar_RewriteValue(VAR_MOVE_POWER, power / 2);
        }
    }
}
// PSYCHIC TERRAIN
extern "C" void HandlerPsychicTerrainPreventPrio(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (BattleField_GetTerrain(g_BattleField) != TERRAIN_PSYCHIC) {
        return;
    }

    u32 currentSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (currentSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        return;
    }

    BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, currentSlot);
    if (!ServerControl_CheckFloating(serverFlow, defendingMon, 1)) {
        ActionOrderWork* actionOrder = serverFlow->actionOrderWork;
        u16 orderIdx = 0;

        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_ATTACKING_MON));
        for (; orderIdx < 6; ++orderIdx) {
            if (actionOrder[orderIdx].battleMon == attackingMon) {
                break;
            }
        }

        u32 priority = ACTION_ORDER_GET_PRIO(actionOrder, orderIdx);
        priority -= 7;
        // Special priority takes into account item & ability prio boosts (1 = no added prio)
        u32 special_priority = ACTION_ORDER_GET_SPECIAL_PRIO(actionOrder, orderIdx);
        special_priority -= 1;
        priority += special_priority;

        if (priority > 0) {
            BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);
        }
    }
}
BattleEventHandlerTableEntry FieldTerrainHandlers[] = {
    // STATUS PREVENTION TERRAIN EFFECTS (ELECTRIC & MISTY)
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerTerrainPreventStatus},
    {EVENT_ADD_CONDITION_FAIL, HandlerTerrainStatusFailMessage},
    // ELECTRIC TERRAIN
    {EVENT_CHECK_SLEEP, HandlerElectricTerrainCheckSleep},
    {EVENT_AFTER_TERRAIN_CHANGE, HandlerElectricTerrainTerrainChange},
    {EVENT_SWITCH_IN, HandlerElectricTerrainSwitchIn},
    {EVENT_ACTION_PROCESSING_END, HandlerElectricTerrainActProcEnd},
    {EVENT_ITEM_REWRITE_DONE, HandlerElectricTerrainFloatingChange},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerElectricTerrainFloatingChange},
    {EVENT_GROUNDED_BY_GRAVITY, HandlerElectricTerrainFloatingChangeNEW},
    // GRASSY TERRAIN
    {EVENT_TURN_CHECK_BEGIN, HandlerGrassyTerrainHeal},
    {EVENT_MOVE_BASE_POWER, HandlerGrassyTerrainQuakeMoves},
    // PSYCHIC TERRAIN
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerPsychicTerrainPreventPrio},
    // Terrain damage boost is aplied in [ServerEvent_CalcDamage]
};
BattleEventHandlerTableEntry* EventAddFieldTerrain(u32* handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(FieldTerrainHandlers);
    return FieldTerrainHandlers;
}

#if EXPAND_ABILITIES

// DARK AURA FIELD
extern "C" void HandlerFieldDarkAura(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_DARK) {
        if (BattleField_GetAuraBreakMons() == 0) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 5448);
        }
        else {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 3072);
        }
    }
}
BattleEventHandlerTableEntry FieldDarkAuraHandlers[]
{
    {EVENT_MOVE_POWER, HandlerFieldDarkAura},
};
extern "C" BattleEventHandlerTableEntry * EventAddFieldDarkAura(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(FieldDarkAuraHandlers);
    return FieldDarkAuraHandlers;
}

// FAIRY AURA FIELD
extern "C" void HandlerFieldFairyAura(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    // TODO: ADD FAIRY TYPE
    if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_NULL) {
        if (BattleField_GetAuraBreakMons() == 0) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 5448);
        }
        else {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 3072);
        }
    }
}
BattleEventHandlerTableEntry FieldFairyAuraHandlers[]
{
    {EVENT_MOVE_POWER, HandlerFieldFairyAura},
};
extern "C" BattleEventHandlerTableEntry * EventAddFieldFairyAura(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FieldFairyAuraHandlers);
    return FieldFairyAuraHandlers;
}

// VESSEL OF RUIN FIELD
extern "C" void HandlerFieldVesselOfRuin(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_SPECIAL) {

        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_ATTACKING_MON));
        if (BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY) != ABIL284_VESSEL_OF_RUIN) {
            BattleEventVar_MulValue(VAR_RATIO, 3072);
        }
    }
}
BattleEventHandlerTableEntry FieldVesselOfRuinHandlers[]
{
    {EVENT_ATTACKER_POWER, HandlerFieldVesselOfRuin},
};
extern "C" BattleEventHandlerTableEntry * EventAddFieldVesselOfRuin(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FieldVesselOfRuinHandlers);
    return FieldVesselOfRuinHandlers;
}

// SWORD OF RUIN FIELD
extern "C" void HandlerFieldSwordOfRuin(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_PHYSICAL) {

        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_DEFENDING_MON));
        if (BattleMon_GetValue(defendingMon, VALUE_EFFECTIVE_ABILITY) != ABIL285_SWORD_OF_RUIN) {
            BattleEventVar_MulValue(VAR_RATIO, 3072);
        }
    }
}
BattleEventHandlerTableEntry FieldSwordOfRuinHandlers[]
{
    {EVENT_DEFENDER_GUARD, HandlerFieldSwordOfRuin},
};
extern "C" BattleEventHandlerTableEntry * EventAddFieldSwordOfRuin(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FieldSwordOfRuinHandlers);
    return FieldSwordOfRuinHandlers;
}

// TABLETS OF RUIN FIELD
extern "C" void HandlerFieldTabletsOfRuin(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_PHYSICAL) {

        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_ATTACKING_MON));
        if (BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY) != ABIL286_TABLETS_OF_RUIN) {
            BattleEventVar_MulValue(VAR_RATIO, 3072);
        }
    }
}
BattleEventHandlerTableEntry FieldTabletsOfRuinHandlers[]
{
    {EVENT_ATTACKER_POWER, HandlerFieldTabletsOfRuin},
};
extern "C" BattleEventHandlerTableEntry * EventAddFieldTabletsOfRuin(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FieldTabletsOfRuinHandlers);
    return FieldTabletsOfRuinHandlers;
}

// BEADS OF RUIN FIELD
extern "C" void HandlerFieldBeadsOfRuin(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot != BattleEventVar_GetValue(VAR_DEFENDING_MON) && 
        BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_SPECIAL) {

        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_DEFENDING_MON));
        if (BattleMon_GetValue(defendingMon, VALUE_EFFECTIVE_ABILITY) != ABIL287_BEADS_OF_RUIN) {
            BattleEventVar_MulValue(VAR_RATIO, 3072);
        }
    }
}
BattleEventHandlerTableEntry FieldBeadsOfRuinHandlers[]
{
    {EVENT_DEFENDER_GUARD, HandlerFieldBeadsOfRuin},
};
extern "C" BattleEventHandlerTableEntry * EventAddFieldBeadsOfRuin(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FieldBeadsOfRuinHandlers);
    return FieldBeadsOfRuinHandlers;
}

#endif // EXPAND_ABILITIES

FieldEffectEventAddTable FieldEffectEventAddTableExt[] = {
    {FLDEFF_WEATHER, EventAddFieldWeather},
    {FLDEFF_TRICK_ROOM, EventAddFieldTrickRoom},
    {FLDEFF_GRAVITY, EventAddFieldGravity},
    {FLDEFF_IMPRISON, EventAddFieldImprison},
    {FLDEFF_WATER_SPORT, EventAddFieldWaterSport},
    {FLDEFF_MUD_SPORT, EventAddFieldMudSport},
    {FLDEFF_WONDER_ROOM, EventAddFieldWonderRoom},
    {FLDEFF_MAGIC_ROOM, EventAddFieldMagicRoom},
    {FLDEFF_MAGIC_ROOM, EventAddFieldMagicRoom},
    {FLDEFF_TERRAIN, EventAddFieldTerrain},
#if EXPAND_ABILITIES
    {FLDEFF_DARK_AURA, EventAddFieldDarkAura},
    {FLDEFF_FAIRY_AURA, EventAddFieldFairyAura},
    {FLDEFF_VESSEL_OF_RUIN, EventAddFieldVesselOfRuin},
    {FLDEFF_SWORD_OF_RUIN, EventAddFieldSwordOfRuin},
    {FLDEFF_TABLETS_OF_RUIN, EventAddFieldTabletsOfRuin},
    {FLDEFF_BEADS_OF_RUIN, EventAddFieldBeadsOfRuin},
#endif // EXPAND_ABILITIES
};

// Add the Field Effect battle event handlers to the event pool
extern "C" BattleEventItem * FieldEffectEvent_AddItem(FIELD_EFFECT fieldEffect, u32 pokemonSlot) {

    for (FIELD_EFFECT currentFieldEffect = 0; currentFieldEffect < FIELD_EFFECT_AMOUNT; ++currentFieldEffect) {
        if (fieldEffect == FieldEffectEventAddTableExt[currentFieldEffect].fieldEffect) {
            u32 enventFuncAmount = 0;
            BattleEventHandlerTableEntry* battleEventEntry = FieldEffectEventAddTableExt[currentFieldEffect].handlers(&enventFuncAmount);

            return BattleEvent_AddItem(EVENTITEM_FIELD, fieldEffect, EVENTPRI_FIELD_DEFAULT, 0, pokemonSlot, battleEventEntry, enventFuncAmount);
        }
    }
    return 0;
}

// Remove a specific dependent Pokémon from a specific field effect
extern "C" void BattleField_RemoveDependPokeEffectCoreFromEffect(BattleFieldExt * battleField, u32 pokemonSlot, FIELD_EFFECT fieldEffect) {
    if (BattleField_CheckFieldEffectCore(battleField, fieldEffect)) {

        u32 pokeCount = battleField->dependPokeCount[fieldEffect];
        if (pokeCount) {
            u32* dependPokes = battleField->dependPokeID[fieldEffect];

            // Find the slot to remove
            u32 removeSlot = 0;
            while (pokemonSlot != dependPokes[removeSlot]) {
                if (++removeSlot >= pokeCount) {
                    return;
                }
            }

            // Move back slots after removed slot by one
            for (; removeSlot < 5; ++removeSlot) {
                // WARNING: "BattleField_CheckFieldEffectCore" is not needed here
                // WARNING: not adding a call to another function here seems to be crashing the game
                // WARNING: debugging it seems to be creating a loop to itself in ASM
                // WARNING: it's similar to when you use a function that is declared and not defined
                BattleField_CheckFieldEffectCore(battleField, fieldEffect);
                dependPokes[removeSlot] = dependPokes[removeSlot + 1];
            }

            // Set last slot to null & remove 1 from the total count
            dependPokes[removeSlot] = BATTLE_MAX_SLOTS;
            pokeCount = --battleField->dependPokeCount[fieldEffect];

            // Remove the field effect if the count is empty
            if (pokeCount == 0) {
                BattleField_RemoveEffectCore(battleField, fieldEffect);
            }
            else {
                u32 condPokemonSlot = Condition_GetMonID(battleField->conditionData[fieldEffect]);
                // If the condition Pokémon is the one removed, set it to the first available Pokémon
                if (pokemonSlot == condPokemonSlot) {
                    Condition_SetMonID(&battleField->conditionData[fieldEffect], dependPokes[0]);
                }
            }
        }
    }
}

extern "C" void THUMB_BRANCH_BattleField_Clear(WEATHER weather) {
    BattleField_InitCore(g_BattleField, weather);
}

// Clears the data of off a field effect in the Extended BattleField struct
extern "C" void THUMB_BRANCH_BattleField_ClearFactorWork(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect) {
    battleField->battleEventItems[fieldEffect] = 0;
    battleField->conditionData[fieldEffect] = Condition_MakeNull();
    battleField->turnCount[fieldEffect] = 0;
    battleField->dependPokeCount[fieldEffect] = 0;
    battleField->effectEnableFlags[fieldEffect] = 0;

    for (u32 dependPoke = 0; dependPoke < 6; ++dependPoke) {
        battleField->dependPokeID[fieldEffect][dependPoke] = BATTLE_MAX_SLOTS;
    }
}

// Initializes the data in the Extended BattleField struct
extern "C" void THUMB_BRANCH_BattleField_InitCore(BattleFieldExt * battleField, WEATHER weather) {
    battleField->weather = weather;
    battleField->weatherTurns = INFINITE_FIELD_TURNS;
    battleField->terrain = 0;
    battleField->terrainTurns = INFINITE_FIELD_TURNS;

    for (FIELD_EFFECT currentEffect = FLDEFF_WEATHER; currentEffect < FIELD_EFFECT_AMOUNT; ++currentEffect) {
        BattleField_ClearFactorWork(battleField, currentEffect);
    }

#if EXPAND_ABILITIES
    battleField->tempItem = IT_NULL;

    battleField->auraBreakMons = 0;

    battleField->extremeWeatherMons = 0;

    battleField->battleBondFlag = 0;

    battleField->emergencyExitFlag = 0;

    sys_memset(battleField->firstTurnMons, BATTLE_MAX_SLOTS, 6);

#if GEN9_INTREPID_SWORD
    battleField->intrepidSwordFlag = 0;
#endif
#if GEN9_DAUNTLESS_SHIELD
    battleField->dauntlessShieldFlag = 0;
#endif

    battleField->neutralizingGasMons = 0;

    sys_memset(battleField->pairedDondozos, BATTLE_MAX_SLOTS, 4);

    battleField->paradoxAbilityFlag = 0;

    battleField->supersweetSyrupFlag = 0;
#endif
}

// Allocates the memory for the Extended BattleField struct
extern "C" BattleFieldExt * THUMB_BRANCH_BattleField_Init(u16 heapID) {
    if (!g_BattleField) {
        g_BattleField = (BattleFieldExt*)GFL_HeapAllocate(heapID, sizeof(BattleFieldExt), 1, "btl_field.c", 268u);
        BattleField_InitCore(g_BattleField, WEATHER_NULL);
    }
    return g_BattleField;
}

extern "C" void THUMB_BRANCH_BattleField_Free(BattleFieldExt * battleField) {
    if (g_BattleField) {
        GFL_HeapFree(g_BattleField);
        g_BattleField = nullptr;
    }
}

extern "C" WEATHER THUMB_BRANCH_BattleField_GetWeatherCore(BattleFieldExt * battleField) {
    return battleField->weather;
}

extern "C" WEATHER THUMB_BRANCH_BattleField_GetWeather() {
    return BattleField_GetWeatherCore(g_BattleField);
}

extern "C" u32 THUMB_BRANCH_BattleField_GetWeatherTurnsCore(BattleFieldExt * battleField) {
    if (battleField->weather) {
        return battleField->weatherTurns;
    }
    return 0;
}

extern "C" u32 THUMB_BRANCH_BattleField_GetWeatherTurns() {
    return BattleField_GetWeatherTurnsCore(g_BattleField);
}

extern "C" void THUMB_BRANCH_BattleField_SetWeatherCore(BattleFieldExt * battleField, WEATHER weather, u32 turns) {
    battleField->weather = weather;
    battleField->weatherTurns = turns;
}

extern "C" void THUMB_BRANCH_BattleField_SetWeather(WEATHER weather, u32 turns) {
    BattleField_SetWeatherCore(g_BattleField, weather, turns);
}

extern "C" void THUMB_BRANCH_BattleField_EndWeather(BattleFieldExt * battleField) {
    battleField->weather = WEATHER_NULL;
    battleField->weatherTurns = 0;
}

// Reduces weather turn count each turn and sets weather to NULL when it ends, returns current weather
extern "C" WEATHER THUMB_BRANCH_BattleField_TurnCheckWeatherCore(BattleFieldExt * battleField) {
    WEATHER weather = battleField->weather;
    if (!weather) {
        return WEATHER_NULL;
    }

    if (battleField->weatherTurns == 255) {
        return WEATHER_NULL;
    }

    battleField->weatherTurns -= 1;
    if (battleField->weatherTurns) {
        return WEATHER_NULL;
    }

    battleField->weather = WEATHER_NULL;
    return weather;
}

extern "C" WEATHER THUMB_BRANCH_BattleField_TurnCheckWeather() {
    return BattleField_TurnCheckWeatherCore(g_BattleField);
}

// Checks if a Field Effect is currently active
extern "C" b32 THUMB_BRANCH_BattleField_CheckFieldEffectCore(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect) {
    return battleField->effectEnableFlags[fieldEffect];
}

extern "C" b32 THUMB_BRANCH_BattleField_CheckEffect(FIELD_EFFECT fieldEffect) {
    return BattleField_CheckFieldEffectCore(g_BattleField, fieldEffect);
}

// Adds a non-active Field Effect
extern "C" b32 THUMB_BRANCH_BattleField_AddEffectCore(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect, ConditionData conditionData, b32 addEvents) {
    if (battleField->effectEnableFlags[fieldEffect]) {
        return 0;
    }

    if (addEvents) {
        BattleEventItem* effectEvent = FieldEffectEvent_AddItem(fieldEffect, 0);
        battleField->battleEventItems[fieldEffect] = effectEvent;

        if (!effectEvent) {
            return 0;
        }
    }

    battleField->effectEnableFlags[fieldEffect] = 1;
    battleField->conditionData[fieldEffect] = conditionData;
    battleField->turnCount[fieldEffect] = 0;
    battleField->dependPokeCount[fieldEffect] = 0;

    for (u32 dependPoke = 0; dependPoke < 6; ++dependPoke) {
        battleField->dependPokeID[fieldEffect][dependPoke] = BATTLE_MAX_SLOTS;
    }

    u32 pokemonSlot = Condition_GetMonID(conditionData);
    if (pokemonSlot != BATTLE_MAX_SLOTS) {
        BattleField_AddDependPokeCore(battleField, fieldEffect, pokemonSlot);
    }

    return 1;
}

extern "C" b32 THUMB_BRANCH_BattleField_AddEffect(FIELD_EFFECT fieldEffect, ConditionData conditionData) {
    return BattleField_AddEffectCore(g_BattleField, fieldEffect, conditionData, true);
}

// Removes an active Field Effect
extern "C" b32 THUMB_BRANCH_BattleField_RemoveEffectCore(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect) {
    if (!BattleField_CheckFieldEffectCore(battleField, fieldEffect)) {
        return 0;
    }

    BattleEventItem* battleEventItem = battleField->battleEventItems[fieldEffect];
    if (battleEventItem) {
        BattleEventItem_Remove(battleEventItem);
        battleField->battleEventItems[fieldEffect] = 0;
    }

    BattleField_ClearFactorWork(battleField, fieldEffect);
    return 1;
}

extern "C" b32 THUMB_BRANCH_BattleField_RemoveEffect(FIELD_EFFECT fieldEffect) {
    return BattleField_RemoveEffectCore(g_BattleField, fieldEffect);
}

// Makes a given Field Effect dependent on a Pokémon, and makes affected Pokémons dependent on it
extern "C" b32 THUMB_BRANCH_BattleField_AddDependPokeCore(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect, u32 pokemonSlot) {
    if (BattleField_CheckFieldEffectCore(battleField, fieldEffect)) {

        u32 pokeCount = battleField->dependPokeCount[fieldEffect];
        // Don't add if this effect already depends on this Pokémon
        for (u32 i = 0; i < pokeCount; ++i) {
            if (battleField->dependPokeID[fieldEffect][i] == pokemonSlot) {
                return 0;
            }
        }
        if (pokeCount < 6) {
            battleField->dependPokeID[fieldEffect][pokeCount] = pokemonSlot;
            ++battleField->dependPokeCount[fieldEffect];
            return 1;
        }
    }
    return 0;
}

extern "C" b32 THUMB_BRANCH_BattleField_AddDependPoke(FIELD_EFFECT fieldEffect, u32 pokemonSlot) {
    return BattleField_AddDependPokeCore(g_BattleField, fieldEffect, pokemonSlot);
}

// Removes Field Effect dependency
extern "C" void THUMB_BRANCH_BattleField_RemoveDependPokeEffectCore(BattleFieldExt * battleField, u32 pokemonSlot) {
    for (FIELD_EFFECT fieldEffect = FLDEFF_WEATHER; fieldEffect < FIELD_EFFECT_AMOUNT; ++fieldEffect) {
        BattleField_RemoveDependPokeEffectCoreFromEffect(battleField, pokemonSlot, fieldEffect);
    }
}

extern "C" void THUMB_BRANCH_BattleField_RemoveDependPokeEffect(u32 pokemonSlot) {
    BattleField_RemoveDependPokeEffectCore(g_BattleField, pokemonSlot);
}

// Checks if a move can be used under the effect of Imprison
extern "C" b32 THUMB_BRANCH_BattleField_CheckImprisonCore(BattleFieldExt * battleField, PokeCon * pokeCon, BattleMon * battleMon, MOVE_ID moveID) {
    u32 dependPokeCount = battleField->dependPokeCount[FLDEFF_IMPRISON];
    if (!dependPokeCount) {
        return 0;
    }

    u32 currentSlot = BattleMon_GetID(battleMon);
    for (u32 dependPoke = 0; dependPoke < dependPokeCount; ++dependPoke) {

        u32 dependSlot = battleField->dependPokeID[FLDEFF_IMPRISON][dependPoke];
        if (!MainModule_IsAllyMonID(currentSlot, dependSlot)) {

            BattleMon* dependMon = PokeCon_GetBattleMon(pokeCon, dependSlot);
            if (Move_IsUsable(dependMon, moveID)) {
                return 1;
            }
        }
    }
    return 0;
}

extern "C" b32 THUMB_BRANCH_BattleField_CheckImprison(PokeCon * pokeCon, BattleMon * battleMon, MOVE_ID moveID) {
    return BattleField_CheckImprisonCore(g_BattleField, pokeCon, battleMon, moveID);
}

// Reduces Field Effect turn count each turn and removes Field Effect when it ends
extern "C" void THUMB_BRANCH_BattleField_TurnCheckCore(BattleFieldExt * battleField, void(*callback)(FIELD_EFFECT, ServerFlow*), ServerFlow * serverFlow) {
    // Since the both BattleField structs have been merged, we ignore the client side calls
    if (!callback) {
        return;
    }
    
    for (FIELD_EFFECT fieldEffect = 1; fieldEffect < FIELD_EFFECT_AMOUNT; ++fieldEffect) {
        if (BattleField_CheckFieldEffectCore(battleField, fieldEffect)) {

            u32 turnMax = Condition_GetTurnMax(battleField->conditionData[fieldEffect]);
            if (turnMax) {
                u32 turnCount = battleField->turnCount[fieldEffect];
                ++turnCount;

                battleField->turnCount[fieldEffect] = turnCount;
                if (fieldEffect == FLDEFF_TERRAIN) {
                    // Ignore turns if terrain is permanent
                    if (battleField->terrainTurns == INFINITE_FIELD_TURNS)
                        continue;

                    // Update terrain turns
                    battleField->terrainTurns = turnCount;
                }

                if (turnCount >= turnMax) {
                    if (fieldEffect == FLDEFF_TERRAIN) {
                        // Remove terrain data
                        BattleField_EndTerrain(battleField);
                        ServerEvent_ChangeTerrain(serverFlow, TERRAIN_NULL);
                    }

                    BattleField_RemoveEffectCore(battleField, fieldEffect);
                    if (callback) {
                        callback(fieldEffect, serverFlow);
                    }
                }
            }
        }
    }
}

extern "C" void THUMB_BRANCH_BattleField_TurnCheck(void(*callback)(FIELD_EFFECT, ServerFlow*), ServerFlow * serverFlow) {
    BattleField_TurnCheckCore(g_BattleField, callback, serverFlow);
}

extern "C" b32 THUMB_BRANCH_BattleHandler_AddFieldEffect(ServerFlow * serverFlow, HandlerParam_AddFieldEffect* params) {
    u8 prevTerrain = TERRAIN_NULL;
    u8 terrain = params->field_D;
    u32 pokemonSlot = params->header.flags << 19 >> 27;
    BattleMon* currentMon = PokeCon_GetBattleMon(serverFlow->pokeCon, pokemonSlot);

    if (params->effect == FLDEFF_TERRAIN) {

        prevTerrain = BattleField_GetTerrain(g_BattleField);
        u8 turns = Condition_GetTurnMax(params->condData);

        if (turns != INFINITE_FIELD_TURNS) {
            u8 extraTurns = ServerEvent_IncreaseMoveTerrainTurns(serverFlow, terrain, pokemonSlot);
            if (extraTurns) {
                turns += extraTurns;
                params->condData = Condition_MakeTurn(turns);
            }
        }

        if (!ServerControl_ChangeTerrain(serverFlow, terrain, turns)) {
            return 0;
        }
    }

    if (!prevTerrain && !ServerControl_FieldEffectCore(serverFlow, params->effect, params->condData, params->addDependPoke))
    {
        return 0;
    }

    if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0) {
        ServerDisplay_AbilityPopupAdd(serverFlow, currentMon);
    }

    if (params->effect == FLDEFF_TERRAIN) {
        MOVE_ID moveID = 0;
        switch (terrain)
        {
        case TERRAIN_ELECTRIC:
            moveID = ELECTRIC_TERRAIN_MOVE_ANIM;
            break;
        case TERRAIN_GRASSY:
            moveID = GRASSY_TERRAIN_MOVE_ANIM;
            break;
        case TERRAIN_MISTY:
            moveID = MISTY_TERRAIN_MOVE_ANIM;
            break;
        case TERRAIN_PSYCHIC:
            moveID = PSYCHIC_TERRAIN_MOVE_ANIM;
            break;
        }
        u32 pokePos = MainModule_PokeIDToPokePos(serverFlow->mainModule, serverFlow->pokeCon, pokemonSlot);
        ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_MoveAnim, pokePos, pokePos, moveID, 0, 0);

        // ChangeBattleBackground(); TODO: CHANGE BACKGROUND WHEN THE TERRAIN STARTS AND ENDS
        // OVL_168:021E0244 in the IDB
    }

    BattleHandler_SetString(serverFlow, &params->exStr);

    if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0) {
        ServerDisplay_AbilityPopupRemove(serverFlow, currentMon);
    }

    if (params->effect == FLDEFF_TERRAIN) {
        ServerEvent_ChangeTerrainAfter(serverFlow, terrain);
    }

    return 1;
}

extern "C" void THUMB_BRANCH_ServerControl_FieldEffectEnd(ServerFlow * serverFlow, FIELD_EFFECT fieldEffect) {
    u16 msgID = 0xFFFF;
    if (fieldEffect <= FLDEFF_TERRAIN) {
        switch (fieldEffect) {
        case FLDEFF_TRICK_ROOM:
            msgID = 116;
            break;
        case FLDEFF_GRAVITY:
            msgID = 118;
            break;
        case FLDEFF_WONDER_ROOM:
            msgID = 179;
            break;
        case FLDEFF_MAGIC_ROOM:
            msgID = 181;
            break;
        case FLDEFF_TERRAIN:
            msgID = BATTLE_TERRAIN_END_MSGID;
            break;
        default:
            break;
        }
    }
    if (msgID != 0xFFFF) {
        ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, SCID_MessageStandard, msgID, 0xFFFF0000);
    }

    ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_RemoveFieldEffect, fieldEffect);
    if (fieldEffect == FLDEFF_MAGIC_ROOM) {
        ServerControl_SortBySpeed(serverFlow, serverFlow->pokeSetTemp);
        PokeSet_SeekStart(serverFlow->pokeSetTemp);
        for (BattleMon* currentMon = PokeSet_SeekNext(serverFlow->pokeSetTemp); currentMon; currentMon = PokeSet_SeekNext(serverFlow->pokeSetTemp)) {
            if (BattleMon_CanBattle(currentMon)) {
                ServerControl_CheckItemReaction(serverFlow, currentMon, 0);
            }
        }
    }
}

extern "C" u32 THUMB_BRANCH_BattleHandler_GravityCheck(ServerFlow * serverFlow, HandlerParam_Header * params) {
    u8 battleSlots[8];
    u32 pokePos = MainModule_PokeIDToPokePos(serverFlow->mainModule, serverFlow->pokeCon, params->flags << 19 >> 27);

    u32 pokeCount = Handler_ExpandPokeID(serverFlow, pokePos | 0x800, battleSlots);
    for (u32 i = 0; i < pokeCount; i = ++i) {

        BattleMon* battleMon = PokeCon_GetBattleMon(serverFlow->pokeCon, battleSlots[i]);
        b32 grounded = false;
        if (BattleMon_GetConditionFlag(battleMon, CONDITIONFLAG_FLY)) {
            ServerControl_HideTurnCancel(serverFlow, battleMon, CONDITIONFLAG_FLY);
            grounded = true;
        }
        if (ServerEvent_CheckFloating(serverFlow, battleMon, 1)) {
            grounded = true;
        }
        if (BattleMon_CheckIfMoveCondition(battleMon, CONDITION_FLOATING)) {
            ServerControl_CureCondition(serverFlow, battleMon, CONDITION_FLOATING, 0);
            grounded = true;
        }
        if (BattleMon_CheckIfMoveCondition(battleMon, CONDITION_TELEKINESIS)) {
            ServerControl_CureCondition(serverFlow, battleMon, CONDITION_TELEKINESIS, 0);
            grounded = true;
        }

        if (grounded) {
            ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, SCID_SetMessage, 1083, battleSlots[i], -65536);
            ServerEvent_GroundedByGravity(serverFlow, battleMon);
        }
    }
    return 1;
}

extern "C" void THUMB_BRANCH_HandlerRestCheckFail(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) && !BattleEventVar_GetValue(VAR_FAIL_CAUSE)) {

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_SLEEP)) {
            BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_OTHER);
        }
        else if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_HEALBLOCK)) {
            BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_HEALBLOCK);
        }
        else if (BattleMon_IsFullHP(currentMon)) {
            BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_HPFULL);
        }
        else if ((BattleField_GetTerrain(g_BattleField) == TERRAIN_ELECTRIC || BattleField_GetTerrain(g_BattleField) == TERRAIN_MISTY) &&
            !ServerControl_CheckFloating(serverFlow, currentMon, 1)) {
            // Electric Terrain & Misty Terrain fail
            BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_OTHER);
        }
        else {
            ABILITY currentAbility = BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY);
            if (currentAbility == ABIL015_INSOMNIA ||
                currentAbility == ABIL072_VITAL_SPIRIT ||
                currentAbility == ABIL175_SWEET_VEIL) {
                BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_INSOMNIA);
            }

#if EXPAND_ABILITIES
            // Flower Veil& Sweet Veil fail
            for (u8 i = 0; i < 24; ++i) {
                BattleMon* currentMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
                if (currentMon) {

                    u32 currentSlot = BattleMon_GetID(currentMon);
                    u32 currentPos = Handler_PokeIDToPokePos(serverFlow, currentSlot);
                    if (currentPos != NULL_BATTLE_POS) {
                        if (!BattleMon_IsFainted(currentMon) && MainModule_IsAllyMonID(pokemonSlot, currentSlot)) {
                            if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL175_SWEET_VEIL) {
                                BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_INSOMNIA);
                                break;
                            }
                        }
                    }
                }
            }
#endif
        }
    }
}

extern "C" u32 ServerFlow_GetBattlePedestal(ServerFlow * serverFlow) {
    return serverFlow->mainModule->btlSetup->fieldStatus.battlePedestalID;
}

extern "C" void THUMB_BRANCH_HandlerNaturePower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        MOVE_ID newMoveID = 0;

        TERRAIN terrain = BattleField_GetTerrain(g_BattleField);
        switch (terrain) {
        case TERRAIN_ELECTRIC:
            newMoveID = MOVE085_THUNDERBOLT;
            break;
        case TERRAIN_GRASSY:
            newMoveID = MOVE412_ENERGY_BALL;
            break;
        case TERRAIN_MISTY:
            newMoveID = MOVE094_PSYCHIC; // TODO: SWITCH TO MOONBLAST
            break;
        case TERRAIN_PSYCHIC:
            newMoveID = MOVE094_PSYCHIC;
            break;
        default:
        {
            u32 battleGround = ServerFlow_GetBattlePedestal(serverFlow);
            switch (battleGround) {
            case 0u:
            case 5u:
                newMoveID = MOVE402_SEED_BOMB;
                break;
            case 1u:
            case 2u:
            case 3u:
            case 8u:
            case 15u:
            case 17u:
                newMoveID = MOVE089_EARTHQUAKE;
                break;
            case 4u:
            case 14u:
            case 16u:
            case 18u:
                newMoveID = MOVE161_TRI_ATTACK;
            case 6u:
            case 11u:
            case 12u:
                newMoveID = MOVE056_HYDRO_PUMP;
                break;
            case 7u:
                newMoveID = MOVE059_BLIZZARD;
                break;
            case 9u:
                newMoveID = MOVE426_MUD_BOMB;
                break;
            case 10u:
                newMoveID = MOVE157_ROCK_SLIDE;
                break;
            case 13u:
                newMoveID = MOVE058_ICE_BEAM;
                break;
            default:
                newMoveID = MOVE161_TRI_ATTACK;
                break;
            }
        }
        break;
        }

        u32 targetPos = Handler_ReqMoveTargetAuto(serverFlow, pokemonSlot, newMoveID);
        BattleEventVar_RewriteValue(VAR_MOVE_ID, newMoveID);
        BattleEventVar_RewriteValue(VAR_POKE_POS, targetPos);
    }
}

extern "C" u32 THUMB_BRANCH_CommonSecretPowerGetParams(ServerFlow * serverFlow, u8 * effectPtr, u8 * changeAmountPtr)
{
    u8 effect;
    u8 changeAmount;
    u32 result;

    MOVE_ID newMoveID = 0;

    TERRAIN terrain = BattleField_GetTerrain(g_BattleField);
    switch (terrain)
    {
    case TERRAIN_ELECTRIC:
        effect = SCTPOWEFF_ADD_COND;
        changeAmount = CONDITION_PARALYSIS;
        result = 0;
        break;
    case TERRAIN_GRASSY:
        effect = SCTPOWEFF_ADD_COND;
        changeAmount = CONDITION_SLEEP;
        result = 7;
        break;
    case TERRAIN_MISTY:
        effect = SCTPOWEFF_LOWER_STAT;
        changeAmount = STATSTAGE_SPECIAL_ATTACK;
        result = 5;
        break;
    case TERRAIN_PSYCHIC:
        effect = SCTPOWEFF_LOWER_STAT;
        changeAmount = STATSTAGE_SPEED;
        result = 2;
        break;
    default:
    {
        u32 battleGround = ServerFlow_GetBattlePedestal(serverFlow);
        switch (battleGround) {
        case 0:
        case 5:
            effect = SCTPOWEFF_ADD_COND;
            changeAmount = CONDITION_SLEEP;
            result = 7;
            break;
        case 1:
        case 2:
        case 3:
        case 8:
        case 15:
        case 17:
            effect = SCTPOWEFF_LOWER_STAT;
            changeAmount = STATSTAGE_ACCURACY;
            result = 1;
            break;
        case 6:
        case 11:
        case 12:
            effect = SCTPOWEFF_LOWER_STAT;
            changeAmount = STATSTAGE_ATTACK;
            result = 5;
            break;
        case 7:
        case 13:
            effect = SCTPOWEFF_ADD_COND;
            changeAmount = CONDITION_FREEZE;
            if (battleGround == 7) {
                result = 3;
            }
            else {
                result = 4;
            }
            break;
        case 9:
            effect = SCTPOWEFF_LOWER_STAT;
            changeAmount = STATSTAGE_SPEED;
            result = 2;
            break;
        case 10:
        case 19:
            effect = SCTPOWEFF_FLINCH;
            changeAmount = 0;
            result = 6;
            break;
        default:
            effect = SCTPOWEFF_ADD_COND;
            changeAmount = CONDITION_PARALYSIS;
            result = 0;
            break;
        }
    }
    break;
    }

    if (effectPtr) {
        *effectPtr = effect;
    }
    if (changeAmountPtr) {
        *changeAmountPtr = changeAmount;
    }
    return result;
}

extern "C" void THUMB_BRANCH_HandlerCamouflage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        POKE_TYPE pokeType = TYPE_NORMAL;

        TERRAIN terrain = BattleField_GetTerrain(g_BattleField);
        switch (terrain) {
        case TERRAIN_ELECTRIC:
            pokeType = TYPE_ELEC;
            break;
        case TERRAIN_GRASSY:
            pokeType = TYPE_GRASS;
            break;
        case TERRAIN_MISTY:
            pokeType = TYPE_NORMAL; // TODO: SWITCH TO FAIRY
            break;
        case TERRAIN_PSYCHIC:
            pokeType = TYPE_PSY;
            break;
        default:
        {
            u32 battleGround = ServerFlow_GetBattlePedestal(serverFlow);
            switch (battleGround) {
            case 0:
            case 5:
                pokeType = TYPE_GRASS;
                break;
            case 1:
            case 2:
            case 3:
            case 8:
            case 9:
            case 15:
            case 17:
                pokeType = TYPE_GROUND;
                break;
            case 6:
            case 11:
            case 12:
                pokeType = TYPE_WATER;
                break;
            case 7:
            case 13:
                pokeType = TYPE_ICE;
                break;
            case 10:
            case 19:
                pokeType = TYPE_ROCK;
                break;
            default:
                pokeType = TYPE_NORMAL;
                break;
            }
        }
        break;
        }

        u16 monotype = PokeTypePair_MakeMonotype(pokeType);
        BattleMon* battleMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (monotype != BattleMon_GetPokeType(battleMon)) {
            HandlerParam_ChangeType* changeType;
            changeType = (HandlerParam_ChangeType*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_TYPE, pokemonSlot);
            changeType->pokeType = monotype;
            changeType->pokeID = pokemonSlot;
            BattleHandler_PopWork(serverFlow, changeType);
        }
    }
}

#if EXPAND_ABILITIES

extern "C" b32 THUMB_BRANCH_BattleHandler_ChangeWeather(ServerFlow * serverFlow, HandlerParam_ChangeWeather * params) {
    BattleMon* currentMon = PokeCon_GetBattleMon(serverFlow->pokeCon, params->header.flags << 19 >> 27);

    if (!params->airLock) {
        if (CanWeatherBeChanged(serverFlow, params->weather, params->turn, params->field_7))
        {
            if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0) {
                ServerDisplay_AbilityPopupAdd(serverFlow, currentMon);
            }

            // Changing the weather to NULL means ending the weather
            if (params->weather == WEATHER_NULL) {
                WEATHER currentWeather = BattleField_GetWeather();
                if (currentWeather != WEATHER_NULL) {
                    ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_WeatherEnd, currentWeather);
                    ServerControl_ChangeWeatherAfter(serverFlow, WEATHER_NULL);
                }
            }
            else {
                ChangeWeather(serverFlow, params->weather, params->turn, params->field_7);
                BattleHandler_SetString(serverFlow, &params->exStr);
            }

            if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0) {
                ServerDisplay_AbilityPopupRemove(serverFlow, currentMon);
            }

            return 1;
        }
    }
    else {
        if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0) {
            ServerDisplay_AbilityPopupAdd(serverFlow, currentMon);
        }

        BattleHandler_SetString(serverFlow, &params->exStr);
        u32 HEID = HEManager_PushState(&serverFlow->HEManager);
        ServerEvent_NotifyAirLock(serverFlow);
        HEManager_PopState(&serverFlow->HEManager, HEID);

        if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0) {
            ServerDisplay_AbilityPopupRemove(serverFlow, currentMon);
        }

        return 1;
    }

    return 0;
}
// This changes asume the only way to summon Extreme Weather is through BattleHandlers (abilities, items or moves)
// in order to summon Extreme Weather other ways, like at the start of a battle, look at [ServerControl_ChangeWeather]

struct WeatherGraphics {
    u16 animID;
    u16 extremeAnimID;
    u16 msgID;
    u16 extremeMsgID;
    u16 endMsgID;
    u16 endExtremeMsgID;
};
WeatherGraphics WeatherGraphicsData[WEATHER_AMOUNT]{
    {0, 0, 0, 0, 0, 0}, // NULL
    {619, DESOLATE_LAND_ANIM, 84,  BATTLE_DESOLATE_LAND_MSGID, 89, BATTLE_DESOLATE_LAND_END_MSGID}, // SUN
    {616, PRIMORDIAL_SEA_ANIM, 85,  BATTLE_PRIMORDIAL_SEA_MSGID, 90, BATTLE_PRIMORDIAL_SEA_END_MSGID}, // RAIN
    {617, 0, 87,  0, 92, 0}, // HAIL
    {618, 0, 86,  0, 91, 0}, // SANDSTORM
    {0, DELTA_STREAM_ANIM, 0, BATTLE_DELTA_STREAM_MSGID, 0, BATTLE_DELTA_STREAM_END_MSGID}, // STRONGWINDS
};
extern "C" b32 THUMB_BRANCH_ServerCommand_WeatherStart(BtlClientWk * clientWork, u32 * state, u32 * data) {
    WEATHER weather = data[0] & 0x7F;
    u8 isExtremeWeather = (data[0] & 0x80) != 0;

    if (!*state) {
        u32 turns = data[1];
        BattleField_SetWeatherCore((BattleFieldExt*)clientWork->battleField, weather, turns);
        if (IsChapterSkipMode(clientWork)) {
            return 1;
        }

        if (weather < WEATHER_AMOUNT) {
            if (!isExtremeWeather) {
                BattleViewCmd_AddAnim(WeatherGraphicsData[weather].animID);
            }
            else {
                BattleViewCmd_AddAnim(WeatherGraphicsData[weather].extremeAnimID);
            }
        }

        ++(*state);
        return 0;
    }
    if (*state == 1) {
        if (BattleViewCmd_WaitEffect()) {
            return 0;
        }

        if (!isExtremeWeather) {
            BattleViewCmd_StartMessageStandard(clientWork->btlvCore, WeatherGraphicsData[weather].msgID, 0);
        }
        else {
            BattleViewCmd_StartMessageStandard(clientWork->btlvCore, WeatherGraphicsData[weather].extremeMsgID, 0);
        }

        ++(*state);
        return 0;
    }

    return *state == 2 && BattleViewCmd_WaitMessage(clientWork->btlvCore);
}
extern "C" b32 THUMB_BRANCH_ServerCommand_WeatherEnd(BtlClientWk * clientWork, u32 * state, u32 * data) {
    WEATHER weather = data[0] & 0x7F;
    u8 isExtremeWeather = (data[0] & 0x80) != 0;

    if (*state) {
        if (*state == 1 && BattleViewCmd_WaitMessage(clientWork->btlvCore)) {
            BattleField_EndWeather((BattleFieldExt*)clientWork->battleField);
            return 1;
        }
    }
    else {
        if (weather < WEATHER_AMOUNT) {
            if (!isExtremeWeather) {
                BattleViewCmd_StartMessageStandard(clientWork->btlvCore, WeatherGraphicsData[weather].endMsgID, 0);
            }
            else {
                BattleViewCmd_StartMessageStandard(clientWork->btlvCore, WeatherGraphicsData[weather].endExtremeMsgID, 0);
            }
        }
        ++ * state;
    }
    return 0;
}

#endif // EXPAND_ABILITIES

#endif // EXPAND_FIELD_EFFECTS

#if GEN6_CRIT || EXPAND_FIELD_EFFECTS

extern "C" u32 CalcBaseDamage(u32 power, u32 attack, u32 level, u32 defense);
extern "C" u32 WeatherPowerMod(u32 weather, u32 subProcID);
extern "C" u32 TypeEffectivenessPowerMod(u32 damage, u32 typeEffectiveness);

extern "C" u32 THUMB_BRANCH_SAFESTACK_ServerEvent_CalcDamage(ServerFlow * serverFlow, BattleMon * attackingMon, BattleMon * defendingMon,
    MoveParam * moveParam, u32 typeEffectiveness, u32 targetDmgRatio, u32 critFlag, u32 battleDebugMode, u16 * destDamage) {
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
            damage += damage / 2;
#else
            damage += damage;
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

// CONTACT REWORK - Modifies contact checks to allow the battle engine to modify it from the items and abilities
#if (EXPAND_ABILITIES || EXPAND_ITEMS)
extern "C" b32 IsContact(ServerFlow* serverFlow, MOVE_ID moveID, u32 attackingSlot) {
    if (getMoveFlag(moveID, FLAG_CONTACT)) {
        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
        if (attackingMon && BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY) == ABIL203_LONG_REACH)
            return 0;
        return 1;
    }
    return 0;
}
extern "C" b32 MakesContact(ServerFlow* serverFlow, MOVE_ID moveID, u32 attackingSlot, u32 defendingSlot) {
    if (IsContact(serverFlow, moveID, attackingSlot)) {
        return 1;
    }
    return 0;
}

extern "C" void THUMB_BRANCH_SAFESTACK_CommonContactStatusAbility(ServerFlow * serverFlow, u32 currentSlot, CONDITION condition, ConditionData condData, u8 effectChance) {
    if (currentSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) && 
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot) &&
            AbilityEvent_RollEffectChance(serverFlow, effectChance)) {
            HandlerParam_AddCondition* addCondition;
            addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, currentSlot);
            addCondition->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            addCondition->condition = condition;
            addCondition->condData = condData;
            addCondition->almost = 0;
            addCondition->pokeID = BattleEventVar_GetValue(VAR_ATTACKING_MON);
            BattleHandler_PopWork(serverFlow, addCondition);
        }
    }
}
extern "C" void THUMB_BRANCH_HandlerRoughSkin(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) && 
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot))
        {
            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
            if (!BattleMon_IsFainted(attackingMon)) {
                HandlerParam_Damage* damage;
                damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
                damage->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                damage->pokeID = attackingSlot;
                damage->damage = DivideMaxHPZeroCheck(attackingMon, 8u);
                BattleHandler_StrSetup(&damage->exStr, 2u, 430);
                BattleHandler_AddArg(&damage->exStr, attackingSlot);
                BattleHandler_PopWork(serverFlow, damage);
            }
        }
    }
}
extern "C" void THUMB_BRANCH_HandlerAftermath(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (pokemonSlot == defendingSlot) {

        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsFainted(defendingMon)) {

            MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
            u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
            if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {
                BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);

                HandlerParam_Damage* damage;
                damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
                damage->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                damage->pokeID = attackingSlot;
                damage->damage = DivideMaxHPZeroCheck(attackingMon, 4u);
                damage->flags = damage->flags & 0xFE | 1;
                BattleHandler_StrSetup(&damage->exStr, 2u, 402);
                BattleHandler_AddArg(&damage->exStr, attackingSlot);
                BattleHandler_PopWork(serverFlow, damage);
            }
        }
    }
}
extern "C" void THUMB_BRANCH_HandlerPickpocket(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (HandlerCommon_CheckTargetMonID(pokemonSlot)) {

        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        if (!HandlerCommon_CheckIfCanStealPokeItem(serverFlow, pokemonSlot, attackingSlot)) {

            MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
            u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
            if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {

                BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
                if (BattleMon_GetHeldItem(currentMon) == IT_NULL) {

                    BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
                    ITEM_ID heldItem = BattleMon_GetHeldItem(attackingMon);
                    if (heldItem) {
                        HandlerParam_SwapItem* swapItem;
                        swapItem = (HandlerParam_SwapItem*)BattleHandler_PushWork(serverFlow, EFFECT_SWAP_ITEM, pokemonSlot);
                        swapItem->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                        swapItem->pokeID = attackingSlot;
                        BattleHandler_StrSetup(&swapItem->exStr, 2u, 460);
                        BattleHandler_AddArg(&swapItem->exStr, attackingSlot);
                        BattleHandler_AddArg(&swapItem->exStr, heldItem);
                        BattleHandler_PopWork(serverFlow, swapItem);
                    }
                }
            }
        }
    }
}
extern "C" void THUMB_BRANCH_HandlerPoisonTouch(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && !BattleEventVar_GetValue(VAR_SHIELD_DUST_FLAG)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot) &&
            AbilityEvent_RollEffectChance(serverFlow, 30u)) {
            HandlerParam_AddCondition* addCondition;
            addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
            addCondition->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            addCondition->pokeID = defendingSlot;
            addCondition->condition = CONDITION_POISON;
            addCondition->condData = MakeBasicStatus(CONDITION_POISON);
            BattleHandler_StrSetup(&addCondition->exStr, 2u, 472);
            BattleHandler_AddArg(&addCondition->exStr, addCondition->pokeID);
            BattleHandler_PopWork(serverFlow, addCondition);
        }
    }
}
extern "C" void THUMB_BRANCH_HandlerMummy(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && !Handler_CheckMatchup(serverFlow)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, pokemonSlot)) {

            BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            if (defendingMon) {
                ABILITY ability = BattleMon_GetValue(defendingMon, VALUE_ABILITY);

                BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
                if (attackingMon && BattleMon_GetValue(attackingMon, VALUE_ABILITY) != ability) {
                    HandlerParam_ChangeAbility* changeAbility;
                    changeAbility = (HandlerParam_ChangeAbility*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_ABILITY, pokemonSlot);
                    changeAbility->ability = ability;
                    changeAbility->pokeID = attackingSlot;
                    BattleHandler_StrSetup(&changeAbility->exStr, 2u, 463);
                    BattleHandler_AddArg(&changeAbility->exStr, changeAbility->pokeID);
                    BattleHandler_AddArg(&changeAbility->exStr, changeAbility->ability);
                    if (!MainModule_IsAllyMonID(pokemonSlot, attackingSlot)) {
                      changeAbility->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                    }
                    BattleHandler_PopWork(serverFlow, changeAbility);
                }
            }
        }
    }
}
extern "C" void THUMB_BRANCH_HandlerStickyBarbDamageReaction(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {
            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
            if (BattleMon_GetHeldItem(attackingMon) == IT_NULL) {
                HandlerParam_SwapItem* swapItem;
                swapItem = (HandlerParam_SwapItem*)BattleHandler_PushWork(serverFlow, EFFECT_SWAP_ITEM, pokemonSlot);
                swapItem->pokeID = attackingSlot;
                BattleHandler_PopWork(serverFlow, swapItem);
            }
        }
    }
}
extern "C" void THUMB_BRANCH_HandlerRockyHelmet(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {
            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
            u8 itemUseParam = CommonGetItemParam(item, ITSTAT_USE_PARAM);

            HandlerParam_Damage* damage;
            damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
            damage->pokeID = attackingSlot;
            damage->damage = DivideMaxHPZeroCheck(attackingMon, itemUseParam);
            BattleHandler_StrSetup(&damage->exStr, 2u, 424);
            BattleHandler_AddArg(&damage->exStr, damage->pokeID);
            BattleHandler_PopWork(serverFlow, damage);
        }
    }
}

#endif // (EXPAND_ABILITIES || EXPAND_ITEMS)

#if EXPAND_ABILITIES

// Called when a held item is consumed [ServerControl_ChangeHeldItem]
extern "C" void ServerEvent_ConsumeItem(ServerFlow * serverFlow, u32 currentSlot, ITEM_ID itemID) {
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    BattleEventVar_SetConstValue(NEW_VAR_MON_ID, currentSlot);
    BattleEventVar_SetValue(VAR_ITEM, itemID);
    BattleEvent_CallHandlers(serverFlow, EVENT_CONSUME_ITEM);
    BattleEventVar_Pop();
}
// Called after using a held item [ServerEvent_EquipItem]
extern "C" void ServerEvent_UseItemAfter(ServerFlow * serverFlow, u32 currentSlot, ITEM_ID itemID) {
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    BattleEventVar_SetConstValue(NEW_VAR_MON_ID, currentSlot);
    BattleEventVar_SetValue(VAR_ITEM, itemID);
    BattleEvent_CallHandlers(serverFlow, EVENT_USE_ITEM_AFTER);
    BattleEventVar_Pop();
}
// Called after using a temp item [ServerEvent_EquipTempItem]
extern "C" void ServerEvent_UseTempItemAfter(ServerFlow * serverFlow, u32 currentSlot, u32 attackingSlot, ITEM_ID itemID) {
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    BattleEventVar_SetValue(NEW_VAR_MON_ID, currentSlot);
    BattleEventVar_SetValue(NEW_VAR_ATTACKING_MON, attackingSlot);
    BattleEventVar_SetValue(VAR_ITEM, itemID);
    BattleEvent_ForceCallHandlers(serverFlow, EVENT_USE_TEMP_ITEM_AFTER);
    BattleEventVar_Pop();
}
// Called when before an ability change, it allows cancelling the change [BattleHandler_ChangeAbility]
extern "C" u8 ServerEvent_ChangeAbilityCheckFail(ServerFlow * serverFlow, u32 pokemonSlot, ABILITY prevAbility, ABILITY nextAbility) {
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    BattleEventVar_SetConstValue(NEW_VAR_MON_ID, pokemonSlot);
    BattleEventVar_SetConstValue(VAR_PREVIOUS_ABILITY, prevAbility);
    BattleEventVar_SetConstValue(VAR_NEXT_ABILITY, nextAbility);
    BattleEventVar_SetRewriteOnceValue(VAR_MOVE_FAIL_FLAG, 0);
    BattleEvent_CallHandlers(serverFlow, EVENT_ABILITY_CHANGE_CHECK_FAIL);
    u8 failFlag = BattleEventVar_GetValue(VAR_MOVE_FAIL_FLAG);
    BattleEventVar_Pop();

    return failFlag;
}
// Called when the ability of a Pokemon stops being nullifyed [ServerControl_CureCondition]
extern "C" void ServerEvent_AbilityNullifyCured(ServerFlow * serverFlow, BattleMon * battleMon) {
    BattleEventVar_Push();
    u32 currentSlot = BattleMon_GetID(battleMon);
    BattleEventVar_SetConstValue(VAR_MON_ID, currentSlot);
    // Abilities with an effect when given also activate when they stop being nullifyed
    BattleEvent_CallHandlers(serverFlow, EVENT_AFTER_ABILITY_CHANGE);
    BattleEventVar_Pop();
}
// Called after receiving simple damage (status, recoil, items...) [ServerControl_SimpleDamageCore]
extern "C" void ServerEvent_SimpleDamageReaction(ServerFlow * serverFlow, BattleMon * battleMon, u32 damage) {
    u32 HEID = HEManager_PushState(&serverFlow->HEManager);

    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    u32 currentSlot = BattleMon_GetID(battleMon);
    BattleEventVar_SetConstValue(NEW_VAR_MON_ID, currentSlot);
    BattleEventVar_SetConstValue(VAR_DAMAGE, damage);
    BattleEvent_CallHandlers(serverFlow, EVENT_SIMPLE_DAMAGE_REACTION);
    BattleEventVar_Pop();

    HEManager_PopState(&serverFlow->HEManager, HEID);
}
// Called when 2 Pokémon are swapping places [BattleHandler_SwapPoke]
extern "C" void ServerEvent_SwapPoke(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon) {
    u32 HEID = HEManager_PushState(&serverFlow->HEManager);

    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    u32 attackingSlot = BattleMon_GetID(attackingMon);
    BattleEventVar_SetConstValue(NEW_VAR_ATTACKING_MON, attackingSlot);
    u32 defendingSlot = BattleMon_GetID(defendingMon);
    BattleEventVar_SetConstValue(NEW_VAR_DEFENDING_MON, defendingSlot);
    BattleEvent_CallHandlers(serverFlow, EVENT_SWAP_POKE);
    BattleEventVar_Pop();

    HEManager_PopState(&serverFlow->HEManager, HEID);
}
// Called before using a temp item [ServerEvent_EquipTempItem]
extern "C" void ServerEvent_UseTempItemBefore(ServerFlow * serverFlow, u32 currentSlot, u32 attackingSlot, ITEM_ID itemID) {
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    BattleEventVar_SetValue(NEW_VAR_MON_ID, currentSlot);
    BattleEventVar_SetValue(NEW_VAR_ATTACKING_MON, attackingSlot);
    BattleEventVar_SetValue(VAR_ITEM, itemID);
    BattleEvent_ForceCallHandlers(serverFlow, EVENT_USE_TEMP_ITEM_BEFORE);
    BattleEventVar_Pop();
}
// Called before any other switch-in events [ServerEvent_AfterSwitchInPrevious]
extern "C" void ServerEvent_SwitchInPriority(ServerFlow* serverFlow) {
    PokeSet_SeekStart(&serverFlow->currentpokeSet);
    for (BattleMon* currentMon = PokeSet_SeekNext(&serverFlow->currentpokeSet); currentMon; currentMon = PokeSet_SeekNext(&serverFlow->currentpokeSet)) {
        u32 HEID = HEManager_PushState(&serverFlow->HEManager);

        u32 currentSlot = BattleMon_GetID(currentMon);
        BattleEventVar_Push();
        SET_UP_NEW_EVENT;
        BattleEventVar_SetConstValue(NEW_VAR_MON_ID, currentSlot);
        BattleEvent_CallHandlers(serverFlow, EVENT_SWITCH_IN_PRIO);
        BattleEventVar_Pop();

        HEManager_PopState(&serverFlow->HEManager, HEID);
    }
}

// References for the implementations in https://bulbapedia.bulbagarden.net/wiki/Ability#List_of_Abilities and https://www.youtube.com/@mupokepedia

// AROMA VEIL
extern "C" void HandlerAromaVeilPreventConditions(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) ||
        MainModule_IsAllyMonID(pokemonSlot, BattleEventVar_GetValue(VAR_DEFENDING_MON))) {
        if (BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_HEALBLOCK ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_DISABLE ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_ATTRACT ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_TAUNT ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_ENCORE ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_TORMENT) {
            *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, FORCE_FAIL_MESSAGE);
        }
    }
}
extern "C" void HandlerAromaVeilFailMessage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if ((pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) || MainModule_IsAllyMonID(pokemonSlot, BattleEventVar_GetValue(VAR_DEFENDING_MON))) &&
        *work) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
        HandlerParam_Message* message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, BATTLE_AROMA_VEIL_MSGID);
        BattleHandler_AddArg(&message->str, BattleEventVar_GetValue(VAR_DEFENDING_MON));
        BattleHandler_PopWork(serverFlow, message);
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        *work = 0;
    }
}
BattleEventHandlerTableEntry AromaVeilHandlers[]{
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerAromaVeilPreventConditions},
    {EVENT_ADD_CONDITION_FAIL, HandlerAromaVeilFailMessage},
};
extern "C" BattleEventHandlerTableEntry * EventAddAromaVeil(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(AromaVeilHandlers);
    return AromaVeilHandlers;
}

// FLOWER VEIL
extern "C" void HandlerFlowerVeilPreventStatus(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if ((pokemonSlot == defendingSlot || MainModule_IsAllyMonID(pokemonSlot, defendingSlot)) &&
        BattleMon_HasType(Handler_GetBattleMon(serverFlow, defendingSlot), TYPE_GRASS)) {
        if (BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_PARALYSIS ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_SLEEP ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_FREEZE ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_BURN ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_POISON ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_CONFUSION ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_YAWN) {
            *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, FORCE_FAIL_MESSAGE);
        }
    }
}
extern "C" void HandlerFlowerVeilStatusFailMessage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if ((pokemonSlot == defendingSlot || MainModule_IsAllyMonID(pokemonSlot, defendingSlot)) &&
        BattleMon_HasType(Handler_GetBattleMon(serverFlow, defendingSlot), TYPE_GRASS) &&
        *work) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
        HandlerParam_Message* message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, BATTLE_FLOWER_VEIL_MSGID);
        BattleHandler_AddArg(&message->str, defendingSlot);
        BattleHandler_PopWork(serverFlow, message);
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        *work = 0;
    }
}
extern "C" void HandlerFlowerVeilStatCheck(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);

    if ((pokemonSlot == currentSlot || MainModule_IsAllyMonID(pokemonSlot, currentSlot)) &&
        BattleMon_HasType(Handler_GetBattleMon(serverFlow, currentSlot), TYPE_GRASS) &&
        currentSlot != BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
        BattleEventVar_GetValue(VAR_VOLUME) <= 0) {
        *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
    }
}
extern "C" void HandlerFlowerVeilStatFailMessage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);

    if ((pokemonSlot == currentSlot || MainModule_IsAllyMonID(pokemonSlot, currentSlot)) &&
        BattleMon_HasType(Handler_GetBattleMon(serverFlow, currentSlot), TYPE_GRASS) &&
        *work) {

        u32 moveSerial = BattleEventVar_GetValue(VAR_MOVE_SERIAL);
        if (!moveSerial || work[1] != moveSerial) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            HandlerParam_Message* message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
            BattleHandler_StrSetup(&message->str, 2u, BATTLE_FLOWER_VEIL_MSGID);
            BattleHandler_AddArg(&message->str, currentSlot);
            BattleHandler_PopWork(serverFlow, message);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
            work[1] = moveSerial;
        }
        *work = 0;
    }
}
BattleEventHandlerTableEntry FlowerVeilHandlers[]{
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerFlowerVeilPreventStatus},
    {EVENT_ADD_CONDITION_FAIL, HandlerFlowerVeilStatusFailMessage},
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerFlowerVeilStatCheck},
    {EVENT_STAT_STAGE_CHANGE_FAIL, HandlerFlowerVeilStatFailMessage},
};
extern "C" BattleEventHandlerTableEntry * EventAddFlowerVeil(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(FlowerVeilHandlers);
    return FlowerVeilHandlers;
}

// CHEEK POUCH
extern "C" void HandlerCheekPouch(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        ITEM_ID itemID = BattleEventVar_GetValue(VAR_ITEM);
        if (itemID && PML_ItemIsBerry(itemID) && !BattleMon_CheckIfMoveCondition(currentMon, CONDITION_HEALBLOCK)) {
            if (!BattleMon_IsFullHP(currentMon)) {

                HandlerParam_RecoverHP* recoveHP;
                recoveHP = (HandlerParam_RecoverHP*)BattleHandler_PushWork(serverFlow, EFFECT_RECOVER_HP, pokemonSlot);
                recoveHP->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                recoveHP->pokeID = pokemonSlot;
                recoveHP->recoverHP = DivideMaxHPZeroCheck(currentMon, 3);
                BattleHandler_StrSetup(&recoveHP->exStr, 2u, 387);
                BattleHandler_AddArg(&recoveHP->exStr, pokemonSlot);
                BattleHandler_PopWork(serverFlow, recoveHP);
            }
        }
    }
}
BattleEventHandlerTableEntry CheekPouchHandlers[]{
    {EVENT_CONSUME_ITEM, HandlerCheekPouch},
    {EVENT_USE_TEMP_ITEM_AFTER, HandlerCheekPouch},
};
extern "C" BattleEventHandlerTableEntry * EventAddCheekPouch(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(CheekPouchHandlers);
    return CheekPouchHandlers;
}

// PROTEAN
extern "C" void HandlerProtean(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
#if GEN9_PROTEAN
    if (*work != 0) {
        return;
    }
#endif
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

        MoveParam params;
        ServerEvent_GetMoveParam(serverFlow, BattleEventVar_GetValue(VAR_MOVE_ID), currentMon, &params);

        if (params.moveType != NULL_TYPE &&
            params.moveType != BattleMon_GetPokeType(currentMon)) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            HandlerParam_ChangeType* changeType;
            changeType = (HandlerParam_ChangeType*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_TYPE, pokemonSlot);
            changeType->pokeType = PokeTypePair_MakeMonotype(params.moveType);
            changeType->pokeID = pokemonSlot;
            BattleHandler_PopWork(serverFlow, changeType);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
#if GEN9_PROTEAN
            *work = 1;
#endif
        }
    }
}
BattleEventHandlerTableEntry ProteanHandlers[]{
    {EVENT_MOVE_EXECUTE_CHECK2, HandlerProtean},
};
extern "C" BattleEventHandlerTableEntry * EventAddProtean(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(ProteanHandlers);
    return ProteanHandlers;
}

// FUR COAT
extern "C" void HandlerFurCoat(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (PML_MoveGetCategory(moveID) == CATEGORY_PHYSICAL) {
            BattleEventVar_MulValue(VAR_RATIO, 2048);
        }
    }
}
BattleEventHandlerTableEntry FurCoatHandlers[]{
    {EVENT_ATTACKER_POWER, HandlerFurCoat},
};
extern "C" BattleEventHandlerTableEntry * EventAddFurCoat(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(FurCoatHandlers);
    return FurCoatHandlers;
}

// MAGICIAN
extern "C" void HandlerMagician(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_GetHeldItem(currentMon) == IT_NULL && !*work) {
            u32 targetSlot = BattleEventVar_GetValue(VAR_TARGET_MON_ID);
            if (targetSlot != BATTLE_MAX_SLOTS) {
                BattleMon* targetMon = Handler_GetBattleMon(serverFlow, targetSlot);
                if (BattleMon_GetHeldItem(targetMon)) {
                    if (!HandlerCommon_CheckIfCanStealPokeItem(serverFlow, pokemonSlot, targetSlot)) {
                        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                        HandlerParam_SwapItem* swapItem;
                        swapItem = (HandlerParam_SwapItem*)BattleHandler_PushWork(serverFlow, EFFECT_SWAP_ITEM, pokemonSlot);
                        swapItem->pokeID = targetSlot;
                        BattleHandler_StrSetup(&swapItem->exStr, 2u, 1057);
                        BattleHandler_AddArg(&swapItem->exStr, pokemonSlot);
                        BattleHandler_AddArg(&swapItem->exStr, targetSlot);
                        ITEM_ID heldItem = BattleMon_GetHeldItem(targetMon);
                        BattleHandler_AddArg(&swapItem->exStr, heldItem);
                        BattleHandler_PopWork(serverFlow, swapItem);

                        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
                    }
                }
            }
        }
    }
}
BattleEventHandlerTableEntry MagicianHandlers[]{
    {EVENT_DAMAGE_PROCESSING_START, HandlerThiefStart},
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerMagician},
};
extern "C" BattleEventHandlerTableEntry * EventAddMagician(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MagicianHandlers);
    return MagicianHandlers;
}

// BULLET PROOF
MOVE_ID BulletMoves[] = {
    MOVE121_EGG_BOMB,
    MOVE140_BARRAGE,
    MOVE188_SLUDGE_BOMB,
    MOVE190_OCTAZOOKA,
    MOVE192_ZAP_CANNON,
    MOVE247_SHADOW_BALL,
    MOVE296_MIST_BALL,
    MOVE301_ICE_BALL,
    MOVE311_WEATHER_BALL,
    MOVE331_BULLET_SEED,
    MOVE350_ROCK_BLAST,
    MOVE360_GYRO_BALL,
    MOVE396_AURA_SPHERE,
    MOVE402_SEED_BOMB,
    MOVE411_FOCUS_BLAST,
    MOVE412_ENERGY_BALL,
    MOVE426_MUD_BOMB,
    MOVE439_ROCK_WRECKER,
    MOVE443_MAGNET_BOMB,
    MOVE486_ELECTRO_BALL,
    MOVE491_ACID_SPRAY,
    MOVE545_SEARING_SHOT,
#if EXPAND_MOVES
    POLLEN_PUFF,
    BEAK_BLAST,
    PYRO_BALL,
    SYRUP_BOMB,
#endif
};
extern "C" b32 IsBulletMove(MOVE_ID moveID) {
    for (u16 i = 0; i < ARRAY_COUNT(BulletMoves); ++i) {
        if (moveID == BulletMoves[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" void HandlerBulletProof(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (IsBulletMove(moveID)) {
            if (BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1)) {
                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                HandlerParam_Message* message;
                message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
                BattleHandler_StrSetup(&message->str, 2u, 210);
                BattleHandler_AddArg(&message->str, pokemonSlot);
                BattleHandler_PopWork(serverFlow, message);

                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
            }
        }
    }
}
BattleEventHandlerTableEntry BulletProofHandlers[]{
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerBulletProof},
};
extern "C" BattleEventHandlerTableEntry * EventAddBulletProof(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(BulletProofHandlers);
    return BulletProofHandlers;
}

// COMPETITIVE
extern "C" void HandlerCompetitive(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {

        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        if (!MainModule_IsAllyMonID(pokemonSlot, attackingSlot) && BattleEventVar_GetValue(VAR_VOLUME) < 0) {
            HandlerParam_ChangeStatStage* statStageChange;
            statStageChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
            statStageChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            statStageChange->stat = STATSTAGE_SPECIAL_ATTACK;
            statStageChange->volume = 2;
            statStageChange->moveAnimation = 1;
            statStageChange->pokeCount = 1;
            statStageChange->pokeID[0] = pokemonSlot;
            BattleHandler_PopWork(serverFlow, statStageChange);
        }
    }
}
BattleEventHandlerTableEntry CompetitiveHandlers[]{
    {EVENT_STAT_STAGE_CHANGE_APPLIED, HandlerCompetitive},
};
extern "C" BattleEventHandlerTableEntry * EventAddCompetitive(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(CompetitiveHandlers);
    return CompetitiveHandlers;
}

// STRONG JAW
MOVE_ID BiteMoves[] = {
    MOVE044_BITE,
    MOVE158_HYPER_FANG,
    MOVE242_CRUNCH,
    MOVE305_POISON_FANG,
    MOVE422_THUNDER_FANG,
    MOVE423_ICE_FANG,
    MOVE424_FIRE_FANG,
#if EXPAND_MOVES
    FISHIOUS_REND,
    JAW_LOCK,
    PSYCHIC_FANG,
#endif
};
extern "C" b32 IsBiteMove(MOVE_ID moveID) {
    for (u16 i = 0; i < ARRAY_COUNT(BiteMoves); ++i) {
        if (moveID == BiteMoves[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" void HandlerStrongJaw(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (IsBiteMove(moveID)) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 6144);
        }
    }
}
BattleEventHandlerTableEntry StrongJawHandlers[]{
    {EVENT_MOVE_POWER, HandlerStrongJaw},
};
extern "C" BattleEventHandlerTableEntry * EventAddStrongJaw(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(StrongJawHandlers);
    return StrongJawHandlers;
}

// REFRIGERATE
extern "C" void HandlerRefrigerateTypeChange(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_NORMAL) {
            BattleEventVar_RewriteValue(VAR_MOVE_TYPE, TYPE_ICE);
        }
    }
}
extern "C" void HandlerRefrigeratePower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_ICE) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 4915);
        }
    }
}
BattleEventHandlerTableEntry RefrigerateHandlers[]{
    {EVENT_MOVE_PARAM, HandlerRefrigerateTypeChange},
    {EVENT_MOVE_POWER, HandlerRefrigeratePower},
};
extern "C" BattleEventHandlerTableEntry * EventAddRefrigerate(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(RefrigerateHandlers);
    return RefrigerateHandlers;
}

// SWEET VEIL
extern "C" void HandlerSweetVeilPreventSleep(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) ||
        MainModule_IsAllyMonID(pokemonSlot, BattleEventVar_GetValue(VAR_DEFENDING_MON))) {
        if (BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_SLEEP) {
            *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, FORCE_FAIL_MESSAGE);
        }
    }
}
extern "C" void HandlerSweetVeilFailMessage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if ((pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) || MainModule_IsAllyMonID(pokemonSlot, BattleEventVar_GetValue(VAR_DEFENDING_MON))) &&
        *work) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, BATTLE_SWEET_VEIL_MSGID);
        BattleHandler_AddArg(&message->str, BattleEventVar_GetValue(VAR_DEFENDING_MON));
        BattleHandler_PopWork(serverFlow, message);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        *work = 0;
    }
}
BattleEventHandlerTableEntry SweetVeilHandlers[]{
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerSweetVeilPreventSleep},
    {EVENT_ADD_CONDITION_FAIL, HandlerSweetVeilFailMessage},
    // Rest prevention in [HandlerRestCheckFail]
};
extern "C" BattleEventHandlerTableEntry * EventAddSweetVeil(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SweetVeilHandlers);
    return SweetVeilHandlers;
}

// STANCE CHANGE
extern "C" void HandlerStanceChange(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    // TODO: ADD AEGISLASH
    if (BattleMon_GetSpecies(currentMon) == PK000_NULL &&
        pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 currentForm = BattleMon_GetValue(currentMon, VALUE_FORM);

        MoveCategory category = PML_MoveGetCategory(moveID);
        HandlerParam_ChangeForm* changeForm;
        if (currentForm == 0 &&
            (category == CATEGORY_PHYSICAL || category == CATEGORY_SPECIAL)) {

            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = 1;
            BattleHandler_StrSetup(&changeForm->exStr, 1u, BATTLE_BLADE_FORME_MSGID);
            BattleHandler_AddArg(&changeForm->exStr, pokemonSlot);
            BattleHandler_PopWork(serverFlow, changeForm);
        }
        // TODO: ADD KING'S SHIELD 
        else if (moveID == MOVE_NULL) {
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = 0;
            BattleHandler_StrSetup(&changeForm->exStr, 1u, BATTLE_SHIELD_FORME_MSGID);
            BattleHandler_AddArg(&changeForm->exStr, pokemonSlot);
            BattleHandler_PopWork(serverFlow, changeForm);
        }
    }
}
BattleEventHandlerTableEntry StanceChangeHandlers[]{
    {EVENT_MOVE_SEQUENCE_START, HandlerStanceChange},
};
extern "C" BattleEventHandlerTableEntry * EventAddStanceChange(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(StanceChangeHandlers);
    return StanceChangeHandlers;
}

// GALE WINGS
extern "C" void HandlerGaleWings(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
#if GEN6_GALE_WINGS
        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsFullHP(attackingMon))
#endif
        {
            MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
            if (moveID) {
                if (PML_MoveGetType(moveID) == TYPE_FLY) {
                    BattleEventVar_RewriteValue(VAR_MOVE_PRIORITY, BattleEventVar_GetValue(VAR_MOVE_PRIORITY) + 1);
                }

                // This code makes Gale Wings work with moves that turn Flying Type like Hidden Power
                /*
                MoveParam params;
                if (MoveEvent_AddItem(attackingMon, moveID, 0)) {
                    ServerEvent_GetMoveParam(serverFlow, moveID, attackingMon, &params);
                    MoveEvent_RemoveItem(attackingMon, moveID);
                }

                if (params.moveType == TYPE_FLY) {
                    BattleEventVar_RewriteValue(VAR_MOVE_PRIORITY, BattleEventVar_GetValue(VAR_MOVE_PRIORITY) + 1);
                }
                */
            }
        }
    }
}
BattleEventHandlerTableEntry GaleWingsHandlers[]{
    {EVENT_GET_MOVE_PRIORITY, HandlerGaleWings},
};
extern "C" BattleEventHandlerTableEntry * EventAddGaleWings(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(GaleWingsHandlers);
    return GaleWingsHandlers;
}

// MEGA LAUNCHER
MOVE_ID PulseMoves[] = {
    MOVE352_WATER_PULSE,
    MOVE396_AURA_SPHERE,
    MOVE399_DARK_PULSE,
    MOVE406_DRAGON_PULSE,
    MOVE505_HEAL_PULSE,
#if EXPAND_MOVES
    ORIGIN_PULSE,
    TERRAIN_PULSE,
#endif
};
extern "C" b32 IsPulseMove(MOVE_ID moveID) {
    for (u16 i = 0; i < ARRAY_COUNT(PulseMoves); ++i) {
        if (moveID == PulseMoves[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" void HandlerMegaLauncherDamageBoost(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (IsPulseMove(BattleEventVar_GetValue(VAR_MOVE_ID))) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 6144);
        }
    }
}
extern "C" void HandlerMegaLauncherHealBoost(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        if (IsPulseMove(BattleEventVar_GetValue(VAR_MOVE_ID))) {
            BattleEventVar_RewriteValue(VAR_RATIO, 3072);
        }
    }
}
BattleEventHandlerTableEntry MegaLauncherHandlers[]{
    {EVENT_MOVE_POWER, HandlerMegaLauncherDamageBoost},
    {EVENT_RECOVER_HP, HandlerMegaLauncherHealBoost},
};
extern "C" BattleEventHandlerTableEntry * EventAddMegaLauncher(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MegaLauncherHandlers);
    return MegaLauncherHandlers;
}

// GRASS PELT
extern "C" void HandlerGrassPelt(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        if (BattleField_GetTerrain(g_BattleField) == TERRAIN_GRASSY) {
            if (BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_PHYSICAL) {
                BattleEventVar_MulValue(VAR_RATIO, 6144);
            }
        }
    }
}
BattleEventHandlerTableEntry GrassPeltHandlers[]{
    {EVENT_DEFENDER_GUARD, HandlerGrassPelt},
};
extern "C" BattleEventHandlerTableEntry * EventAddGrassPelt(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(GrassPeltHandlers);
    return GrassPeltHandlers;
}

// SYMBIOSIS
ITEM_ID dmgReduceBerries[] = {
    IT0184_OCCA_BERRY,
    IT0185_PASSHO_BERRY,
    IT0186_WACAN_BERRY,
    IT0187_RINDO_BERRY,
    IT0188_YACHE_BERRY,
    IT0189_CHOPLE_BERRY,
    IT0190_KEBIA_BERRY,
    IT0191_SHUCA_BERRY,
    IT0192_COBA_BERRY,
    IT0193_PAYAPA_BERRY,
    IT0194_TANGA_BERRY,
    IT0195_CHARTI_BERRY,
    IT0196_KASIB_BERRY,
    IT0197_HABAN_BERRY,
    IT0198_COLBUR_BERRY,
    IT0199_BABIRI_BERRY,
    IT0200_CHILAN_BERRY,
#if EXPAND_ITEMS
    ROSELI_BERRY,
#endif
};
extern "C" b32 IsDmgReduceBerry(ITEM_ID itemID) {
    for (u16 i = 0; i < ARRAY_COUNT(dmgReduceBerries); ++i) {
        if (itemID == dmgReduceBerries[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" void SymbiosisChangeItem(ServerFlow* serverFlow, u32 pokemonSlot, u32 receiveSlot) {
#if SYMBIOSIS_DEBUG
    DPRINTF("SYMBIOSIS -> SLOT: %d | RECEIVE SLOT: %d\n", pokemonSlot, receiveSlot);
#endif
    if (pokemonSlot != receiveSlot && MainModule_IsAllyMonID(pokemonSlot, receiveSlot)) {
        BattleMon* receiveMon = Handler_GetBattleMon(serverFlow, receiveSlot);
        ITEM_ID allyItem = BattleMon_GetHeldItem(receiveMon);
#if SYMBIOSIS_DEBUG
        DPRINTF("SYMBIOSIS -> ALLY ITEM: %d\n", allyItem);
#endif
        if (!allyItem) {
            BattleMon* giveMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            int giveItem = BattleMon_GetHeldItem(giveMon);
#if SYMBIOSIS_DEBUG
            DPRINTF("SYMBIOSIS -> ITEM TO GIVE: %d\n", giveItem);
#endif
            if (giveItem) {
                if (!PML_ItemIsMail(giveItem)
                    && !HandlerCommon_IsUnremovableItem(BattleMon_GetSpecies(giveMon), giveItem)
                    && !HandlerCommon_IsUnremovableItem(BattleMon_GetSpecies(receiveMon), giveItem)) {
#if SYMBIOSIS_DEBUG
                    DPRINT("SYMBIOSIS -> SUCCESS\n");
#endif
                    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                    HandlerParam_SwapItem* swapItem;
                    swapItem = (HandlerParam_SwapItem*)BattleHandler_PushWork(serverFlow, EFFECT_SWAP_ITEM, pokemonSlot);
                    swapItem->pokeID = receiveSlot;
                    BattleHandler_StrSetup(&swapItem->exStr, 2u, BATTLE_SYMBIOSIS_MSGID);
                    BattleHandler_AddArg(&swapItem->exStr, pokemonSlot);
                    BattleHandler_StrSetup(&swapItem->exSubStr1, 2u, 685);
                    BattleHandler_AddArg(&swapItem->exSubStr1, receiveSlot);
                    BattleHandler_AddArg(&swapItem->exSubStr1, giveItem);
                    BattleHandler_PopWork(serverFlow, swapItem);

                    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
                }
            }
        }
    }
}
extern "C" void HandlerSymbiosis(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (IS_NOT_NEW_EVENT) return;

    u32 receiveSlot = BattleEventVar_GetValue(NEW_VAR_MON_ID);
    ITEM_ID itemID = BattleEventVar_GetValue(VAR_ITEM);

    // We initially set the delayed slot as null
    *work = BATTLE_MAX_SLOTS;

    // DELAY CHECK
    if (IsDmgReduceBerry(itemID)
#if GEN7_SYMBIOSIS_GEMS
        || IsGem(itemID)
#endif
        )
    {
        // store the delayed slot if needed
        *work = receiveSlot;
#if SYMBIOSIS_DEBUG
        DPRINTF("CHECK DELAY -> DELAY SLOT: %d\n", receiveSlot);
#endif
        return;
    }

    SymbiosisChangeItem(serverFlow, pokemonSlot, receiveSlot);
}
extern "C" void HandlerSymbiosisDelayed(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    // If a we have a delayed slot stored
    if (*work != BATTLE_MAX_SLOTS) {
#if SYMBIOSIS_DEBUG
        DPRINTF("SYMBIOSIS DELAY -> DELAY SLOT: %d\n", *work);
#endif
        // Trigger the delayed swap
        SymbiosisChangeItem(serverFlow, pokemonSlot, *work);
    }
    // Reset the event variable
    *work = BATTLE_MAX_SLOTS;
}
BattleEventHandlerTableEntry SymbiosisHandlers[]{
    {EVENT_CONSUME_ITEM, HandlerSymbiosis}, // swap item or delay if it's a damage affecting item
    {EVENT_USE_TEMP_ITEM_AFTER, HandlerSymbiosis}, // swap when consuming temp item
    {EVENT_MOVE_DAMAGE_SIDE_AFTER, HandlerSymbiosisDelayed}, // swap item if we are delaying
};
extern "C" BattleEventHandlerTableEntry * EventAddSymbiosis(u32* handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(SymbiosisHandlers);
    return SymbiosisHandlers;
}

// TOUGH CLAWS
extern "C" void HandlerToughClaws(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 5461);
        }
    }
}
BattleEventHandlerTableEntry ToughClawsHandlers[]{
    {EVENT_MOVE_POWER, HandlerToughClaws},
};
extern "C" BattleEventHandlerTableEntry * EventAddToughClaws(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(ToughClawsHandlers);
    return ToughClawsHandlers;
}

// PIXIELATE
extern "C" void HandlerPixilateTypeChange(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_NORMAL) {
            // TODO: ADD FAIRY TYPE
            BattleEventVar_RewriteValue(VAR_MOVE_TYPE, TYPE_GRASS);
        }
    }
}
extern "C" void HandlerPixilatePower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        // TODO: ADD FAIRY TYPE
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_GRASS) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 4915);
        }
    }
}
BattleEventHandlerTableEntry PixilateHandlers[]{
    {EVENT_MOVE_PARAM, HandlerPixilateTypeChange},
    {EVENT_MOVE_POWER, HandlerPixilatePower},
};
extern "C" BattleEventHandlerTableEntry * EventAddPixilate(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PixilateHandlers);
    return PixilateHandlers;
}

// GOOEY
extern "C" void HandlerGooey(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (pokemonSlot == defendingSlot &&
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {

            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
            if (BattleMon_IsStatChangeValid(attackingMon, STATSTAGE_SPEED, -1)) {
                if (!BattleMon_IsFainted(attackingMon)) {
                    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                    HandlerParam_ChangeStatStage* statChange;
                    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    statChange->pokeCount = 1;
                    statChange->pokeID[0] = attackingSlot;
                    statChange->moveAnimation = 1;
                    statChange->stat = STATSTAGE_SPEED;
                    statChange->volume = -1;
                    BattleHandler_PopWork(serverFlow, statChange);

                    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
                }
            }
        }
    }
}
BattleEventHandlerTableEntry GooeyHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerGooey},
};
extern "C" BattleEventHandlerTableEntry * EventAddGooey(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(GooeyHandlers);
    return GooeyHandlers;
}

// AERILATE
extern "C" void HandlerAerilateTypeChange(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_NORMAL) {
            BattleEventVar_RewriteValue(VAR_MOVE_TYPE, TYPE_FLY);
        }
    }
}
extern "C" void HandlerAerilatePower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_FLY) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 4915);
        }
    }
}
BattleEventHandlerTableEntry AerilateHandlers[]{
    {EVENT_MOVE_PARAM, HandlerAerilateTypeChange},
    {EVENT_MOVE_POWER, HandlerAerilatePower},
};
extern "C" BattleEventHandlerTableEntry * EventAddAerilate(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(AerilateHandlers);
    return AerilateHandlers;
}

// PARENTAL BOND
// The Parental Bond flag is set in [ServerControl_DamageRoot] whenever a move hits twice bacause of the ability
u8 parentalBondFlag = 0;
extern "C" void HandlerParentalBondPower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) && parentalBondFlag) {
        *work += 1;
        u32 power = BattleEventVar_GetValue(VAR_MOVE_POWER);

        if (*work == 2) {
            *work = 0;
#if GEN6_PARENTAL_BOND
            power /= 2;
#else
            power /= 4;
#endif
        }

        BattleEventVar_RewriteValue(VAR_MOVE_POWER, power);
    }
}
BattleEventHandlerTableEntry ParentalBondHandlers[]{
    {EVENT_MOVE_POWER, HandlerParentalBondPower},
    // Double hit implementation in [ServerControl_DamageRoot]
};
extern "C" BattleEventHandlerTableEntry * EventAddParentalBond(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ParentalBondHandlers);
    return ParentalBondHandlers;
}

// DARK AURA
extern "C" void CommonAddFieldEffectAbilityHandler(ServerFlow * serverFlow, u32 pokemonSlot, FIELD_EFFECT fieldEffect, u16 msgID) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_AddFieldEffect* addFieldEffect;
        addFieldEffect = (HandlerParam_AddFieldEffect*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_FIELD_EFFECT, pokemonSlot);
        addFieldEffect->effect = fieldEffect;
        addFieldEffect->condData = Condition_MakePoke(pokemonSlot);
        addFieldEffect->addDependPoke = 1;
        BattleHandler_StrSetup(&addFieldEffect->exStr, 2u, msgID);
        BattleHandler_AddArg(&addFieldEffect->exStr, pokemonSlot);
        BattleHandler_PopWork(serverFlow, addFieldEffect);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
extern "C" void CommonRemoveFieldEffectAbilityHandler(ServerFlow * serverFlow, u32 pokemonSlot, FIELD_EFFECT fieldEffect) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleField_RemoveDependPokeEffectCoreFromEffect(g_BattleField, pokemonSlot, fieldEffect);
    }
}
extern "C" void HandlerDarkAuraAdd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonAddFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_DARK_AURA, BATTLE_DARK_AURA_MSGID);
}
extern "C" void HandlerDarkAuraRemove(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleField_RemoveDependPokeEffectCoreFromEffect(g_BattleField, pokemonSlot, FLDEFF_DARK_AURA);
    }
}
BattleEventHandlerTableEntry DarkAuraHandlers[]{
    {EVENT_SWITCH_IN, HandlerDarkAuraAdd}, // Add fieldeffect when switching in
    {EVENT_AFTER_ABILITY_CHANGE, HandlerDarkAuraAdd}, // Add fieldeffect/dependpoke when changing ability and when ability nullify ends
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerDarkAuraRemove}, // Remove dependpoke when changing ability
    {EVENT_ABILITY_NULLIFIED, HandlerDarkAuraRemove}, // Remove dependpoke when ability is nullified
};
extern "C" BattleEventHandlerTableEntry * EventAddDarkAura(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(DarkAuraHandlers);
    return DarkAuraHandlers;
}

// FAIRY AURA
extern "C" void HandlerFairyAuraAdd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonAddFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_FAIRY_AURA, BATTLE_FAIRY_AURA_MSGID);
}
extern "C" void HandlerFairyAuraRemove(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonRemoveFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_FAIRY_AURA);
}
BattleEventHandlerTableEntry FairyAuraHandlers[]{
    {EVENT_SWITCH_IN, HandlerFairyAuraAdd}, // Add fieldeffect when switching in
    {EVENT_AFTER_ABILITY_CHANGE, HandlerDarkAuraAdd}, // Add fieldeffect/dependpoke when changing ability and when ability nullify ends
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerFairyAuraRemove}, // Remove dependpoke when changing ability
    {EVENT_ABILITY_NULLIFIED, HandlerFairyAuraRemove}, // Remove dependpoke when ability is nullified
};
extern "C" BattleEventHandlerTableEntry * EventAddFairyAura(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FairyAuraHandlers);
    return FairyAuraHandlers;
}

// AURA BREAK
extern "C" void HandlerAuraBreakAdd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleField_AddAuraBreakMon();

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, BATTLE_AURA_BREAK_MSGID);
        BattleHandler_AddArg(&message->str, pokemonSlot);
        BattleHandler_PopWork(serverFlow, message);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
extern "C" void HandlerAuraBreakRemove(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleField_RemoveAuraBreakMon();
    }
}
extern "C" void HandlerAuraBreakRemoveFainted(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        BattleMon_IsFainted(Handler_GetBattleMon(serverFlow, pokemonSlot))) {
        BattleField_RemoveAuraBreakMon();
    }
}
BattleEventHandlerTableEntry AuraBreakHandlers[]{
    {EVENT_SWITCH_IN, HandlerAuraBreakAdd},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerAuraBreakAdd},
    {EVENT_SWITCH_OUT_END, HandlerAuraBreakRemove},
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerAuraBreakRemove},
    {EVENT_ABILITY_NULLIFIED, HandlerAuraBreakRemove},
    {EVENT_NOTIFY_FAINTED, HandlerAuraBreakRemoveFainted},
};
extern "C" BattleEventHandlerTableEntry * EventAddAuraBreak(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(AuraBreakHandlers);
    return AuraBreakHandlers;
}

// PRIMORDIAL SEA
extern "C" void HandlerPrimordialSeaStart(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerAddExtremeWeather(serverFlow, pokemonSlot, WEATHER_RAIN);
    }
}
extern "C" void HandlerPrimordialSeaEnd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerRemoveExtremeWeather(serverFlow, pokemonSlot, WEATHER_RAIN);
    }
}
extern "C" void HandlerPrimordialSeaEndFainted(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        BattleMon_IsFainted(Handler_GetBattleMon(serverFlow, pokemonSlot))) {
        HandlerRemoveExtremeWeather(serverFlow, pokemonSlot, WEATHER_RAIN);
    }
}
extern "C" void HandlerPrimordialSeaImmunity(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    HandlerExtremeWeatherImmunity(serverFlow, pokemonSlot, WEATHER_RAIN, TYPE_FIRE, BATTLE_PRIMORDIAL_SEA_IMMUNE_MSGID);
}
BattleEventHandlerTableEntry PrimordialSeaHandlers[]{
    {EVENT_SWITCH_IN, HandlerPrimordialSeaStart},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerPrimordialSeaStart},
    {EVENT_SWITCH_OUT_END, HandlerPrimordialSeaEnd},
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerPrimordialSeaEnd},
    {EVENT_ABILITY_NULLIFIED, HandlerPrimordialSeaEnd},
    {EVENT_NOTIFY_FAINTED, HandlerPrimordialSeaEndFainted},
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerPrimordialSeaImmunity},
};
extern "C" BattleEventHandlerTableEntry * EventAddPrimordialSea(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PrimordialSeaHandlers);
    return PrimordialSeaHandlers;
}

// DESOLATE LAND
extern "C" void HandlerDesolateLandStart(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerAddExtremeWeather(serverFlow, pokemonSlot, WEATHER_SUN);
    }
}
extern "C" void HandlerDesolateLandEnd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerRemoveExtremeWeather(serverFlow, pokemonSlot, WEATHER_SUN);
    }
}
extern "C" void HandlerDesolateLandEndFainted(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        BattleMon_IsFainted(Handler_GetBattleMon(serverFlow, pokemonSlot))) {
        HandlerRemoveExtremeWeather(serverFlow, pokemonSlot, WEATHER_SUN);
    }
}
extern "C" void HandlerDesolateLandImmunity(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    HandlerExtremeWeatherImmunity(serverFlow, pokemonSlot, WEATHER_SUN, TYPE_WATER, BATTLE_DESOLATE_LAND_IMMUNE_MSGID);
}
BattleEventHandlerTableEntry DesolateLandHandlers[]{
    {EVENT_SWITCH_IN, HandlerDesolateLandStart},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerDesolateLandStart},
    {EVENT_SWITCH_OUT_END, HandlerPrimordialSeaEnd},
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerDesolateLandEnd},
    {EVENT_ABILITY_NULLIFIED, HandlerDesolateLandEnd},
    {EVENT_NOTIFY_FAINTED, HandlerDesolateLandEndFainted},
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerDesolateLandImmunity},
};
extern "C" BattleEventHandlerTableEntry * EventAddDesolateLand(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(DesolateLandHandlers);
    return DesolateLandHandlers;
}

// DELTA STREAM
extern "C" void HandlerDeltaStreamStart(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerAddExtremeWeather(serverFlow, pokemonSlot, WEATHER_STRONGWINDS);
    }
}
extern "C" void HandlerDeltaStreamEnd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerRemoveExtremeWeather(serverFlow, pokemonSlot, WEATHER_STRONGWINDS);
    }
}
extern "C" void HandlerDeltaStreamEndFainted(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        BattleMon_IsFainted(Handler_GetBattleMon(serverFlow, pokemonSlot))) {
        HandlerRemoveExtremeWeather(serverFlow, pokemonSlot, WEATHER_STRONGWINDS);
    }
}
extern "C" void HandlerDeltaStreamResistance(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (ServerEvent_GetWeather(serverFlow) == WEATHER_STRONGWINDS && BattleField_GetExtremeWeatherMons()) {
        if (BattleEventVar_GetValue(VAR_POKE_TYPE) == TYPE_FLY &&
            GetTypeEffectiveness(BattleEventVar_GetValue(VAR_MOVE_TYPE), TYPE_FLY) == EFFECTIVENESS_2) {

            BattleEventVar_RewriteValue(VAR_SET_TYPE_EFFECTIVENESS, EFFECTIVENESS_1);

            HandlerParam_Message* message;
            message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
            BattleHandler_StrSetup(&message->str, 1u, BATTLE_DELTA_STREAM_RESISTANCE_MSGID);
            BattleHandler_AddArg(&message->str, pokemonSlot);
            BattleHandler_PopWork(serverFlow, message);
        }
    }
}
BattleEventHandlerTableEntry DeltaStreamHandlers[]{
    {EVENT_SWITCH_IN, HandlerDeltaStreamStart},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerDeltaStreamStart},
    {EVENT_SWITCH_OUT_END, HandlerPrimordialSeaEnd},
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerDeltaStreamEnd},
    {EVENT_ABILITY_NULLIFIED, HandlerDeltaStreamEnd},
    {EVENT_NOTIFY_FAINTED, HandlerDeltaStreamEndFainted},
    {EVENT_CHECK_TYPE_EFFECTIVENESS, HandlerDeltaStreamResistance},
};
extern "C" BattleEventHandlerTableEntry * EventAddDeltaStream(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(DeltaStreamHandlers);
    return DeltaStreamHandlers;
}

// STAMINA
extern "C" void HandlerStamina(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)) {
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsStatChangeValid(defendingMon, STATSTAGE_DEFENSE, 1)) {
            if (!BattleMon_IsFainted(defendingMon)) {
                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                HandlerParam_ChangeStatStage* statChange;
                statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                statChange->pokeCount = 1;
                statChange->pokeID[0] = pokemonSlot;
                statChange->moveAnimation = 1;
                statChange->stat = STATSTAGE_DEFENSE;
                statChange->volume = 1;
                BattleHandler_PopWork(serverFlow, statChange);

                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
            }
        }
    }
}
BattleEventHandlerTableEntry StaminaHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerStamina},
};
extern "C" BattleEventHandlerTableEntry * EventAddStamina(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(StaminaHandlers);
    return StaminaHandlers;
}

// EMERGENCY EXIT

// Stores the damage a Substitute takes each action
// - Reset in [ServerControl_DamageRoot] & [CommonEmergencyExitCheck]
// - Set in [BattleMon_AddSubstituteDamage]
// - Used in [CommonEmergencyExitCheck]
u32 actionSubstituteDamage[BATTLE_MAX_SLOTS];
extern "C" void CommonEmergencyExitCheck(ServerFlow * serverFlow, u32 currentSlot) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
    u32 maxHP = BattleMon_GetValue(currentMon, VALUE_MAX_HP);

    u32 currentHP = BattleMon_GetValue(currentMon, VALUE_CURRENT_HP);
    u32 currentHPPercent = (currentHP * 100) / maxHP;

    u32 beforeDmgHP = currentHP + BattleEventVar_GetValue(VAR_DAMAGE) - actionSubstituteDamage[currentSlot];
    u32 beforeDmgHPPercent = (beforeDmgHP * 100) / maxHP;
#if EMERGENCY_EXIT_DEBUG
    DPRINTF("MAX HP: %d \n", maxHP);
    DPRINTF("CURRENT HP: %d \n", currentHP);
    DPRINTF("DAMAGE: %d \n", BattleEventVar_GetValue(VAR_DAMAGE));
    DPRINTF("SUBSTITUTE DAMAGE: %d \n", actionSubstituteDamage[currentSlot]);
    DPRINTF("BEFORE HP: %d \n", beforeDmgHP);
    DPRINTF("BEFORE HP PERCENT: %d \n", beforeDmgHPPercent);
    DPRINTF("CURRENT HP PERCENT: %d \n", currentHPPercent);
#endif
    if (beforeDmgHPPercent >= 50 &&
        currentHPPercent < 50) {
        BattleField_SetEmergencyExitFlag(currentSlot);
    }

    actionSubstituteDamage[currentSlot] = 0;
}
extern "C" void HandlerEmergencyExitDamageCheck(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 targetCount = BattleEventVar_GetValue(VAR_TARGET_COUNT);
    for (u32 target = 0; target < targetCount; ++target) {

        u32 targetSlot = BattleEventVar_GetValue((BattleEventVar)(VAR_TARGET_MON_ID + target));
        if (pokemonSlot == targetSlot) {
            CommonEmergencyExitCheck(serverFlow, pokemonSlot);

            if (BattleField_CheckEmergencyExitFlag(pokemonSlot)) {
                BattleField_ResetEmergencyExitFlag(pokemonSlot);
#if EMERGENCY_EXIT_DEBUG
                DPRINTF("EE SWITCH -> SLOT: %d\n", pokemonSlot);
#endif
                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                HandlerParam_Switch* switchOut;
                switchOut = (HandlerParam_Switch*)BattleHandler_PushWork(serverFlow, EFFECT_SWITCH, pokemonSlot);
                switchOut->pokeID = pokemonSlot;
                BattleHandler_PopWork(serverFlow, switchOut);

                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
            }
        }
    }
}
extern "C" void HandlerEmergencyExitSimpleCheck(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID)) {
#if EMERGENCY_EXIT_DEBUG
        DPRINTF("EE SIMPLE -> SLOT: %d\n", pokemonSlot);
#endif
        CommonEmergencyExitCheck(serverFlow, pokemonSlot);
    }
}
extern "C" void HandlerEmergencyExitSwitchEnd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (BattleField_CheckEmergencyExitFlag(pokemonSlot)) {
        BattleField_ResetEmergencyExitFlag(pokemonSlot);
#if EMERGENCY_EXIT_DEBUG
        DPRINTF("EE SWITCH END -> SLOT: %d\n", pokemonSlot);
#endif
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_Switch* switchOut;
        switchOut = (HandlerParam_Switch*)BattleHandler_PushWork(serverFlow, EFFECT_SWITCH, pokemonSlot);
        switchOut->pokeID = pokemonSlot;
        BattleHandler_PopWork(serverFlow, switchOut);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);

        // Set the flag to switch after a turn has ended
        endTurnSwitchFlag = 1;
    }
}
BattleEventHandlerTableEntry EmergencyExitHandlers[]{
    {EVENT_DAMAGE_PROCESSING_END_HIT_2, HandlerEmergencyExitDamageCheck},
    {EVENT_SIMPLE_DAMAGE_REACTION, HandlerEmergencyExitSimpleCheck},
    {EVENT_TURN_CHECK_END, HandlerEmergencyExitSwitchEnd},
}; 
extern "C" BattleEventHandlerTableEntry * EventAddEmergencyExit(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(EmergencyExitHandlers);
    return EmergencyExitHandlers;
}

// WATER COMPATION
extern "C" void HandlerWaterCompaction(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_WATER) {

        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsStatChangeValid(defendingMon, STATSTAGE_DEFENSE, 2)) {
            if (!BattleMon_IsFainted(defendingMon)) {
                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                HandlerParam_ChangeStatStage* statChange;
                statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                statChange->pokeCount = 1;
                statChange->pokeID[0] = pokemonSlot;
                statChange->moveAnimation = 1;
                statChange->stat = STATSTAGE_DEFENSE;
                statChange->volume = 2;
                BattleHandler_PopWork(serverFlow, statChange);

                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
            }
        }
    }
}
BattleEventHandlerTableEntry WaterCompactionHandlers[] {
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerWaterCompaction},
};
extern "C" BattleEventHandlerTableEntry* EventAddWaterCompaction(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(WaterCompactionHandlers);
    return WaterCompactionHandlers;
}

// MERCILESS
extern "C" void HandlerMerciless(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, defendingSlot);
        if (BattleMon_GetStatus(defendingMon) == CONDITION_POISON) {
            BattleEventVar_RewriteValue(VAR_CRIT_STAGE, 4);
        }
    }
}
BattleEventHandlerTableEntry MercilessHandlers[] {
    {EVENT_CRITICAL_CHECK, HandlerMerciless},
};
extern "C" BattleEventHandlerTableEntry* EventAddMerciless(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MercilessHandlers);
    return MercilessHandlers;
}

// SHIELDS DOWN
extern "C" void HandlerShieldsDown(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // TODO: ADD MINIOR 
        if (BattleMon_GetSpecies(currentMon) == PK000_NULL) {
            u32 currentHP = BattleMon_GetValue(currentMon, VALUE_CURRENT_HP);

            u8 form = 1;
            u16 msgID = BATTLE_SHIELDS_DOWN_INACTIVE_MSGID;
            if (currentHP <= DivideMaxHPZeroCheck(currentMon, 2u)) {
                form = 0;
                msgID = BATTLE_SHIELDS_DOWN_ACTIVE_MSGID;
            }

            if (form != BattleMon_GetValue(currentMon, VALUE_FORM)) {
                HandlerParam_ChangeForm* changeForm;
                changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
                changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                changeForm->pokeID = pokemonSlot;
                changeForm->newForm = form;
                BattleHandler_StrSetup(&changeForm->exStr, 1u, msgID);
                BattleHandler_PopWork(serverFlow, changeForm);
            }
        }
    }
}
BattleEventHandlerTableEntry ShieldsDownHandlers[] {
    {EVENT_TURN_CHECK_DONE, HandlerShieldsDown},
};
extern "C" BattleEventHandlerTableEntry* EventAddShieldsDown(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ShieldsDownHandlers);
    return ShieldsDownHandlers;
}

// STAKEOUT
extern "C" b32 SwitchedInThisTurn(ServerFlow* serverFlow, BattleMon* battleMon) {
    if (serverFlow->turnCount == 0) {
        for (u8 i = 0; i < 6; ++i) {
            if (g_BattleField->firstTurnMons[i] == battleMon->battleSlot) {
                // A Pokémon that started the battle doesn't count as just switched in
                return 0;
            }
        }
    }

    return !battleMon->turnCount;
}
extern "C" void HandlerStakeout(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, defendingSlot);

        if (SwitchedInThisTurn(serverFlow, defendingMon)) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 8192);
        }
    }
}
BattleEventHandlerTableEntry StakeoutHandlers[] {
    {EVENT_MOVE_POWER, HandlerStakeout},
};
extern "C" BattleEventHandlerTableEntry* EventAddStakeout(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(StakeoutHandlers);
    return StakeoutHandlers;
}

// WATER BUBBLE
extern "C" void HandlerWaterBubblePreventStatus(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_BURN) {
        *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, FORCE_FAIL_MESSAGE);
    }
}
extern "C" void CommonStatusFailMessageHandler(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (pokemonSlot == defendingSlot && *work) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 1u, 68); // no effect default message
        BattleHandler_AddArg(&message->str, defendingSlot);
        BattleHandler_PopWork(serverFlow, message);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        *work = 0;
    }
}
extern "C" void HandlerWaterBubbleHealBurn(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    if (BattleMon_GetStatus(currentMon) == CONDITION_BURN) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_CureCondition* cureCondition;
        cureCondition = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
        cureCondition->condition = CONDITION_BURN;
        cureCondition->pokeCount = 1;
        cureCondition->pokeID[0] = pokemonSlot;
        BattleHandler_PopWork(serverFlow, cureCondition);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
extern "C" void HandlerWaterBubbleResist(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_FIRE) {
        BattleEventVar_MulValue(VAR_RATIO, 2048);
    }
}
extern "C" void HandlerWaterBubblePower(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
        BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_WATER) {
        BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 8192);
    }
}
BattleEventHandlerTableEntry WaterBubbleHandlers[] {
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerWaterBubblePreventStatus},
    {EVENT_ADD_CONDITION_FAIL, CommonStatusFailMessageHandler},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerWaterBubbleHealBurn},
    {EVENT_ACTION_PROCESSING_END, HandlerWaterBubbleHealBurn},
    {EVENT_ATTACKER_POWER, HandlerWaterBubbleResist},
    {EVENT_MOVE_POWER, HandlerWaterBubblePower},
};
extern "C" BattleEventHandlerTableEntry* EventAddWaterBubble(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(WaterBubbleHandlers);
    return WaterBubbleHandlers;
}

// STEELWORKER
extern "C" void HandlerSteelworker(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
        BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_STEEL) {
        BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 6138);
    }
}
BattleEventHandlerTableEntry SteelworkerHandlers[] {
    {EVENT_MOVE_POWER, HandlerSteelworker},
};
extern "C" BattleEventHandlerTableEntry* EventAddSteelworker(u32* handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(SteelworkerHandlers);
    return SteelworkerHandlers;
}

// BERSERK
extern "C" void HandlerBerserk(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 targetCount = BattleEventVar_GetValue(VAR_TARGET_COUNT);
    for (u8 i = 0; i < targetCount; ++i) {
        u32 targetSlot = BattleEventVar_GetValue((BattleEventVar)(VAR_TARGET_MON_ID + i));
        if (pokemonSlot == targetSlot) {
            BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            u32 maxHP = BattleMon_GetValue(currentMon, VALUE_MAX_HP);

            u32 currentHP = BattleMon_GetValue(currentMon, VALUE_CURRENT_HP);
            u32 currentHPPercent = (currentHP * 100) / maxHP;

            u32 beforeDmgHP = currentHP + BattleEventVar_GetValue(VAR_DAMAGE) - actionSubstituteDamage[pokemonSlot];
            u32 beforeDmgHPPercent = (beforeDmgHP * 100) / maxHP;
#if BERSERK_DEBUG
            DPRINTF("MAX HP: %d\n", maxHP);
            DPRINTF("CURRENT HP: %d\n", currentHP);
            DPRINTF("DAMAGE: %d\n", BattleEventVar_GetValue(VAR_DAMAGE));
            DPRINTF("SUBSTITUTE DAMAGE: %d\n", actionSubstituteDamage[pokemonSlot]);
            DPRINTF("BEFORE HP: %d\n", beforeDmgHP);
            DPRINTF("BEFORE HP PERCENT: %d\n", beforeDmgHPPercent);
            DPRINTF("CURRENT HP PERCENT: %d\n", currentHPPercent);
#endif
            if (beforeDmgHPPercent >= 50)
            {
                if (currentHPPercent < 50)
                {
                    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                    HandlerParam_ChangeStatStage* stageChange;
                    stageChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    stageChange->pokeCount = 1;
                    stageChange->pokeID[0] = pokemonSlot;
                    stageChange->moveAnimation = 1;
                    stageChange->stat = STATSTAGE_SPECIAL_ATTACK;
                    stageChange->volume = 1;
                    BattleHandler_PopWork(serverFlow, stageChange);

                    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
                }
            }

            actionSubstituteDamage[pokemonSlot] = 0;
        }
    }
}
BattleEventHandlerTableEntry BerserkHandlers[] {
    {EVENT_DAMAGE_PROCESSING_END_HIT_2, HandlerBerserk},
};
extern "C" BattleEventHandlerTableEntry* EventAddBerserk(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(BerserkHandlers);
    return BerserkHandlers;
}

// SLUSH RUSH
extern "C" void HandlerSlushRush(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        ServerEvent_GetWeather(serverFlow) == WEATHER_HAIL) {
        BattleEventVar_MulValue(VAR_RATIO, 8192);
    }
}
BattleEventHandlerTableEntry SlushRushHandlers[] {
    {EVENT_CALC_SPEED, HandlerSlushRush},
};
extern "C" BattleEventHandlerTableEntry* EventAddSlushRush(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SlushRushHandlers);
    return SlushRushHandlers;
}

// LIQUID VOICE
extern "C" void HandlerLiquidVoice(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        getMoveFlag(BattleEventVar_GetValue(VAR_MOVE_ID), FLAG_SOUND)) {
        BattleEventVar_RewriteValue(VAR_MOVE_TYPE, TYPE_WATER);
    }
}
BattleEventHandlerTableEntry LiquidVoiceHandlers[] {
    {EVENT_MOVE_PARAM, HandlerLiquidVoice},
};
extern "C" BattleEventHandlerTableEntry* EventAddLiquidVoice(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(LiquidVoiceHandlers);
    return LiquidVoiceHandlers;
}

// TRIAGE
MOVE_ID healingMoves[] = {
    MOVE071_ABSORB,
    MOVE072_MEGA_DRAIN,
    MOVE105_RECOVER,
    MOVE135_SOFT_BOILED,
    MOVE138_DREAM_EATER,
    MOVE141_LEECH_LIFE,
    MOVE156_REST,
    MOVE202_GIGA_DRAIN,
    MOVE208_MILK_DRINK,
    MOVE234_MORNING_SUN,
    MOVE235_SYNTHESIS,
    MOVE236_MOONLIGHT,
    MOVE256_SWALLOW,
    MOVE273_WISH,
    MOVE303_SLACK_OFF,
    MOVE355_ROOST,
    MOVE361_HEALING_WISH,
    MOVE409_DRAIN_PUNCH,
    MOVE456_HEAL_ORDER,
    MOVE461_LUNAR_DANCE,
    MOVE505_HEAL_PULSE,
    MOVE532_HORN_LEECH,
#if EXPAND_MOVES
    STRENGTH_SAP,
    SHORE_UP,
    REVIVAL_BLESSING,
    PURIFY,
    PARABOLIC_CHARGE,
    OBLIVION_WING,
    MATCHA_GOTCHA,
    LUNAR_BLESSING,
    BITTER_BLADE,
    DRAINING_KISS,
    FLORAL_HEALING,
#endif

};
extern "C" b32 IsHealingMove(MOVE_ID moveID) {
    for (u16 i = 0; i < ARRAY_COUNT(healingMoves); ++i) {
        if (moveID == healingMoves[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" void HandlerTriage(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        int moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (IsHealingMove(moveID)) {
            BattleEventVar_RewriteValue(VAR_MOVE_PRIORITY, BattleEventVar_GetValue(VAR_MOVE_PRIORITY) + 3);
        }
    }
}
BattleEventHandlerTableEntry TriageHandlers[] {
    {EVENT_GET_MOVE_PRIORITY, HandlerTriage},
};
extern "C" BattleEventHandlerTableEntry* EventAddTriage(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(TriageHandlers);
    return TriageHandlers;
}

// GALVANIZE
extern "C" void HandlerGalvanizeTypeChange(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_NORMAL) {
            BattleEventVar_RewriteValue(VAR_MOVE_TYPE, TYPE_ELEC);
        }
    }
}
extern "C" void HandlerGalvanizePower(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_ELEC) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 4915);
        }
    }
}
BattleEventHandlerTableEntry GalvanizeHandlers[] {
    {EVENT_MOVE_PARAM, HandlerGalvanizeTypeChange},
    {EVENT_MOVE_POWER, HandlerGalvanizePower},
};
extern "C" BattleEventHandlerTableEntry* EventAddGalvanize(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(GalvanizeHandlers);
    return GalvanizeHandlers;
}

// SURGE SURFER
extern "C" void HandlerSurgeSurfer(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        ServerEvent_GetTerrain(serverFlow) == TERRAIN_ELECTRIC) {
        BattleEventVar_MulValue(VAR_RATIO, 8192);
    }
}
BattleEventHandlerTableEntry SurgeSurferHandlers[] {
    {EVENT_CALC_SPEED, HandlerSurgeSurfer},
};
extern "C" BattleEventHandlerTableEntry* EventAddSurgeSurfer(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SurgeSurferHandlers);
    return SurgeSurferHandlers;
}

// SCHOOLING
extern "C" void HandlerSchooling(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // TODO: ADD WISHIWASHI
        if (BattleMon_GetSpecies(currentMon) == PK000_NULL &&
            currentMon->level >= 20) {
            u32 currentHP = BattleMon_GetValue(currentMon, VALUE_CURRENT_HP);

            u8 form = 0;
            u16 msgID = BATTLE_SCHOOLING_INACTIVE_MSGID;
            if (currentHP > DivideMaxHPZeroCheck(currentMon, 4u)) {
                form = 1;
                msgID = BATTLE_SCHOOLING_ACTIVE_MSGID;
            }

            if (form != BattleMon_GetValue(currentMon, VALUE_FORM)) {
                HandlerParam_ChangeForm* changeForm;
                changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
                changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                changeForm->pokeID = pokemonSlot;
                changeForm->newForm = form;
                BattleHandler_StrSetup(&changeForm->exStr, 2u, msgID);
                BattleHandler_PopWork(serverFlow, changeForm);
            }
        }
    }
}
BattleEventHandlerTableEntry SchoolingHandlers[] {
    {EVENT_SWITCH_IN, HandlerSchooling},
    {EVENT_TURN_CHECK_DONE, HandlerSchooling},
};
extern "C" BattleEventHandlerTableEntry* EventAddSchooling(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SchoolingHandlers);
    return SchoolingHandlers;
}

// DISGUISE
extern "C" void HandlerDisguisePreventDamage(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    // TODO: ADD MIMIKYU
    if (BattleMon_GetSpecies(currentMon) == PK351_CASTFORM &&//PK000_NULL &&
        pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        if (BattleMon_GetValue(currentMon, VALUE_FORM) == 0) {
            // General Flag set to 2 [ServerControl_ApplyDamageToRecover]
            BattleEventVar_RewriteValue(VAR_GENERAL_USE_FLAG, 2);

            HandlerParam_ChangeForm* changeForm;
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = 1;
            BattleHandler_StrSetup(&changeForm->exStr, 2u, BATTLE_DISGUISE_MSGID);
            BattleHandler_PopWork(serverFlow, changeForm);

            HandlerParam_Damage* damage;
            damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
            damage->pokeID = pokemonSlot;
            damage->damage = DivideMaxHPZeroCheck(currentMon, 8u);
            BattleHandler_PopWork(serverFlow, damage);
        }
    }
}
BattleEventHandlerTableEntry DisguiseHandlers[] {
    {EVENT_CHECK_DAMAGE_TO_RECOVER, HandlerDisguisePreventDamage},
};
extern "C" BattleEventHandlerTableEntry* EventAddDisguise(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(DisguiseHandlers);
    return DisguiseHandlers;
}

// BATTLE BOND
extern "C" void HandlerBattleBond(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    // TODO: ADD GRENINJA
    if (BattleMon_GetSpecies(currentMon) == PK000_NULL &&
        pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
        !BattleField_CheckBattleBondFlag(pokemonSlot)) {

        u32 targetCount = BattleEventVar_GetValue(VAR_TARGET_COUNT);
        for (u8 i = 0; i < targetCount; ++i) {

            u32 targetSlot = BattleEventVar_GetValue((BattleEventVar)(VAR_TARGET_MON_ID + i));
            BattleMon* targetMon = Handler_GetBattleMon(serverFlow, targetSlot);
            if (BattleMon_IsFainted(targetMon) &&
                BattleMon_GetValue(currentMon, VALUE_FORM) != 1) {
                BattleField_SetBattleBondFlag(pokemonSlot);
#if GEN9_BATTLE_BOND
                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                HandlerParam_ChangeStatStage* statChangeAttack;
                statChangeAttack = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                statChangeAttack->pokeCount = 1;
                statChangeAttack->pokeID[0] = pokemonSlot;
                statChangeAttack->stat = STATSTAGE_ATTACK;
                statChangeAttack->volume = 1;
                BattleHandler_PopWork(serverFlow, statChangeAttack);

                HandlerParam_ChangeStatStage* statChangeSpAttack;
                statChangeSpAttack = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                statChangeSpAttack->pokeCount = 1;
                statChangeSpAttack->pokeID[0] = pokemonSlot;
                statChangeSpAttack->stat = STATSTAGE_SPECIAL_ATTACK;
                statChangeSpAttack->volume = 1;
                BattleHandler_PopWork(serverFlow, statChangeSpAttack);

                HandlerParam_ChangeStatStage* statChangeSpeed;
                statChangeSpeed = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                statChangeSpeed->pokeCount = 1;
                statChangeSpeed->pokeID[0] = pokemonSlot;
                statChangeSpeed->stat = STATSTAGE_SPEED;
                statChangeSpeed->volume = 1;
                BattleHandler_PopWork(serverFlow, statChangeSpeed);

                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);

                HandlerParam_Message* message;
                message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
                BattleHandler_StrSetup(&message->str, 2u, BATTLE_BATTLE_BOND_MSGID);
                BattleHandler_AddArg(&message->str, pokemonSlot);
                BattleHandler_PopWork(serverFlow, message);
#else
                HandlerParam_ChangeForm* changeForm;
                changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
                changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                changeForm->pokeID = pokemonSlot;
                changeForm->newForm = 1;
                BattleHandler_StrSetup(&changeForm->exStr, 2u, BATTLE_BATTLE_BOND_MSGID);
                BattleHandler_PopWork(serverFlow, changeForm);
#endif
            }
        }
    }
}
BattleEventHandlerTableEntry BattleBondHandlers[] {
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerBattleBond},
};
extern "C" BattleEventHandlerTableEntry* EventAddBattleBond(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(BattleBondHandlers);
    return BattleBondHandlers;
}

// POWER CONSTRUCT
extern "C" void HandlerPowerConstruct(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // TODO: ADD ZYGARDE
        if (BattleMon_GetSpecies(currentMon) == PK351_CASTFORM &&
            BattleMon_GetValue(currentMon, VALUE_FORM) != 2) {
            u32 currentHP = BattleMon_GetValue(currentMon, VALUE_CURRENT_HP);

            if (currentHP <= DivideMaxHPZeroCheck(currentMon, 2u)) {
                HandlerParam_ChangeForm* changeForm;
                changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
                changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                changeForm->pokeID = pokemonSlot;
                changeForm->newForm = 2;
                BattleHandler_StrSetup(&changeForm->exStr, 1u, BATTLE_POWER_CONSTRUCT_MSGID);
                BattleHandler_PopWork(serverFlow, changeForm);
            }
        }
    }
}
BattleEventHandlerTableEntry PowerConstructHandlers[] {
    {EVENT_TURN_CHECK_DONE, HandlerPowerConstruct},
};
extern "C" BattleEventHandlerTableEntry* EventAddPowerConstruct(u32* handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(PowerConstructHandlers);
    return PowerConstructHandlers;
}

// CORROSION -> Implemented in [ServerControl_AddConditionCheckFail]

// COMATOSE -> Implemented in [ServerControl_AddConditionCheckFail], [BattleMon_CheckIfMoveCondition], [HandlerHex] & [CommonStatusReaction]

// QUEENLY MAGESTY
extern "C" void HandlerQueenlyMagesty(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if ((pokemonSlot == defendingSlot || MainModule_IsAllyMonID(pokemonSlot, defendingSlot)) &&
        defendingSlot != BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        ActionOrderWork* action_order = serverFlow->actionOrderWork;
        u16 orderIdx = 0;

        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_ATTACKING_MON));
        for (; orderIdx < 6; ++orderIdx)
            if (action_order[orderIdx].battleMon == attackingMon)
                break;

        u32 priority = ACTION_ORDER_GET_PRIO(action_order, orderIdx);
        priority -= 7;
        // Special priority takes into account item & ability prio boosts (1 = no added prio)
        u32 special_priority = ACTION_ORDER_GET_SPECIAL_PRIO(action_order, orderIdx);
        special_priority -= 1;
        priority += special_priority;

        if (priority > 0) {
            BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);
        }
    }
}
BattleEventHandlerTableEntry QueenlyMagestyHandlers[] {
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerQueenlyMagesty},
};
extern "C" BattleEventHandlerTableEntry* EventAddQueenlyMagesty(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(QueenlyMagestyHandlers);
    return QueenlyMagestyHandlers;
}

// INNARDS OUT
extern "C" void HandlerInnardsOut(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsFainted(defendingMon)) {
            u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
            if (Handler_PokeIDToPokePos(serverFlow, attackingSlot) != NULL_BATTLE_POS) {

                u32 currentHP = BattleMon_GetValue(defendingMon, VALUE_CURRENT_HP);
                u32 beforeDmgHP = currentHP + BattleEventVar_GetValue(VAR_DAMAGE);

                HandlerParam_Damage* damage;
                damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
                damage->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                damage->pokeID = attackingSlot;
                damage->damage = beforeDmgHP;
                damage->flags = damage->flags & 0xFE | 1;
                BattleHandler_StrSetup(&damage->exStr, 2u, 402u);
                BattleHandler_AddArg(&damage->exStr, attackingSlot);
                BattleHandler_PopWork(serverFlow, damage);
            }
        }
    }
}
BattleEventHandlerTableEntry InnardsOutHandlers[] {
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerInnardsOut},
};
extern "C" BattleEventHandlerTableEntry* EventAddInnardsOut(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(InnardsOutHandlers);
    return InnardsOutHandlers;
}

// DANCER
MOVE_ID danceMoves[] = {
    MOVE014_SWORDS_DANCE,
    MOVE080_PETAL_DANCE,
    MOVE297_FEATHER_DANCE,
    MOVE298_TEETER_DANCE,
    MOVE349_DRAGON_DANCE,
    MOVE461_LUNAR_DANCE,
    MOVE483_QUIVER_DANCE,
    MOVE552_FIERY_DANCE,
#if EXPAND_MOVES
    CLANGOROUS_SOUL,
    REVELATION_DANCE,
    VICTORY_DANCE,
    AQUA_STEP,
#endif
};
extern "C" b32 IsDanceMove(MOVE_ID moveID) {
    for (u16 i = 0; i < ARRAY_COUNT(danceMoves); ++i) {
        if (moveID == danceMoves[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" u32 CommonGetAllyPos(ServerFlow* serverFlow, u32 battlePos) {
    BattleStyle battleStyle = BtlSetup_GetBattleStyle(serverFlow->mainModule);
    if (battleStyle != BTL_STYLE_DOUBLE && battleStyle != BTL_STYLE_TRIPLE) {
        return 6;
    }

    u8 isEnemy = battlePos & 1;
    if (isEnemy) {
        battlePos -= 1;
    }

    u32 allyPos = 0;
    if (battleStyle != BTL_STYLE_TRIPLE) {
        if (battlePos == 0) {
            allyPos = 2;
        }
        else {
            allyPos = 0;
        }
    }
    else {
        if (IsCenterInTripleBattle(battlePos)) {
            allyPos = BattleRandom(2) * 4;
        }
        else {
            allyPos = 2;
        }
    }

    return allyPos + isEnemy;
}
extern "C" void HandlerDancerCheckMove(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (!extraActionFlag) {
        u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (pokemonSlot != currentSlot && IsDanceMove(moveID)) {

            BattleMon* dancerMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            // Dancer fails if the Dancer Pokémon is in a semi-invulnerable state
            if (BattleMon_CheckIfMoveCondition(dancerMon, CONDITION_SKYDROP) ||
                BattleMon_CheckIfMoveCondition(dancerMon, CONDITION_CHARGELOCK)) {
                return;
            }

            // The slowest Dancer Pokémon must move first, 
            // so we move all extra actions back and add the newest one to the start
            for (u8 i = 5; i != 0; --i) {
                sys_memcpy(&extraActionOrder[i - 1], &extraActionOrder[i], sizeof(ActionOrderWork));
            }
            sys_memset(&extraActionOrder[0], 0, sizeof(ActionOrderWork));

            extraActionOrder[0].battleMon = dancerMon;

            BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
            for (u8 orderIdx = 0; orderIdx < 6; ++orderIdx) {
                if (serverFlow->actionOrderWork[orderIdx].battleMon == currentMon) {
                    extraActionOrder[0].action = serverFlow->actionOrderWork[orderIdx].action;
                    
                    BattleAction_Fight* fight = (BattleAction_Fight*)&extraActionOrder[0].action;
                    
                    MoveTarget moveTarget = (MoveTarget)PML_MoveGetParam((MOVE_ID)fight->moveID, MVDATA_TARGET);
                    // The target position varies depending on the move and if the Pokémon is an ally or not
                    switch (moveTarget)
                    {
                    case TARGET_OTHER_SELECT:
                    case TARGET_ENEMY_SELECT:
                    case TARGET_ENEMY_RANDOM:
                        if (!MainModule_IsAllyMonID(pokemonSlot, currentSlot)) {
                            fight->targetPos = Handler_PokeIDToPokePos(serverFlow, currentSlot);
                        }
                        break;
                    case TARGET_ALLY_USER_SELECT:
                        fight->targetPos = Handler_PokeIDToPokePos(serverFlow, pokemonSlot);
                        break;
                    case TARGET_ALLY_SELECT:
                        fight->targetPos = CommonGetAllyPos(serverFlow, Handler_PokeIDToPokePos(serverFlow, pokemonSlot));
                        break;
                    default:
                        fight->targetPos = 6;
                    }
                    
                    extraActionOrder[0].speed = serverFlow->actionOrderWork[orderIdx].speed;
                    extraActionOrder[0].partyID = serverFlow->actionOrderWork[orderIdx].partyID;
                    break;
                }
            }

            extraActionOrder[0].field_E = 0;
            extraActionOrder[0].field_F = 0;
        }
    }
}
extern "C" void HandlerDancerPopUp(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (extraActionFlag && pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
extern "C" void HandlerDancerMoveFail(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if (extraActionFlag && pokemonSlot == currentSlot) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);

        // If the Dancer Pokémon is encored it tries to copy the move,
        // but if it's not the encored move it fails 
        if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_ENCORE)) {
            u8 actionIdx = 0;
            for (; actionIdx < 6; ++actionIdx) {
                if (extraActionOrder[actionIdx].battleMon == currentMon) {
                    break;
                }
            }
            BattleAction_Fight* fight = (BattleAction_Fight*)&extraActionOrder[actionIdx].action;

            if (fight->moveID != moveID) {  
                BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_MOVELOCK);
                return;
            }
        }

        // If the Dancer Pokémon is locked to a move by a choice item or by a move 
        // like Petal Dance it tries to copy the move but it fails
        if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_MOVELOCK) ||
            BattleMon_CheckIfMoveCondition(currentMon, CONDITION_CHOICELOCK)) {
            BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_MOVELOCK);
            return;
        }
    }
}
BattleEventHandlerTableEntry DancerHandlers[]{
    {EVENT_MOVE_EXECUTE_EFFECTIVE, HandlerDancerCheckMove},
    {EVENT_MOVE_EXECUTE_NOEFFECT, HandlerDancerCheckMove},
    {EVENT_MOVE_SEQUENCE_START, HandlerDancerPopUp},
    {EVENT_MOVE_EXECUTE_CHECK1, HandlerDancerMoveFail}
};
extern "C" BattleEventHandlerTableEntry* EventAddDancer(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(DancerHandlers);
    return DancerHandlers;
}

// BATTERY
extern "C" void HandlerBattery(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
    if (pokemonSlot != attackingSlot && MainModule_IsAllyMonID(pokemonSlot, attackingSlot)) {
        if (BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_SPECIAL) {
            BattleEventVar_MulValue(VAR_RATIO, 5325);
        }
    }
}
BattleEventHandlerTableEntry BatteryHandlers[]{
    {EVENT_ATTACKER_POWER, HandlerBattery},
};
extern "C" BattleEventHandlerTableEntry* EventAddBattery(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(BatteryHandlers);
    return BatteryHandlers;
}

// FLUFFY
extern "C" void HandlerFluffy(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        u32 ratio = 4096;

        POKE_TYPE moveType = BattleEventVar_GetValue(VAR_MOVE_TYPE);
        if (moveType == TYPE_FIRE) {
            ratio *= 2;
        }

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, pokemonSlot)) {
            ratio /= 2;
        }

        BattleEventVar_MulValue(VAR_RATIO, ratio);
    }
}
BattleEventHandlerTableEntry FluffyHandlers[]{
    {EVENT_MOVE_DAMAGE_PROCESSING_2, HandlerFluffy},
};
extern "C" BattleEventHandlerTableEntry* EventAddFluffy(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FluffyHandlers);
    return FluffyHandlers;
}

// SOUL-HEART
extern "C" void HandlerSoulHeart(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if (pokemonSlot != currentSlot) {

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
        if (BattleMon_IsFainted(currentMon)) {
            HandlerParam_ChangeStatStage* statChange;
            statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
            statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            statChange->pokeCount = 1;
            statChange->pokeID[0] = pokemonSlot;
            statChange->stat = STATSTAGE_SPECIAL_ATTACK;
            statChange->volume = 1;
            BattleHandler_PopWork(serverFlow, statChange);
        }
    }
}
BattleEventHandlerTableEntry SoulHeartHandlers[]{
    {EVENT_NOTIFY_FAINTED, HandlerSoulHeart},
};
extern "C" BattleEventHandlerTableEntry* EventAddSoulHeart(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SoulHeartHandlers);
    return SoulHeartHandlers;
}

// RECEIVER
extern "C" void HandlerReceiver(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if (pokemonSlot != currentSlot &&
        MainModule_IsAllyMonID(pokemonSlot, currentSlot) &&
        BattleMon_IsFainted(Handler_GetBattleMon(serverFlow, currentSlot))) {

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
        ABILITY ability = BattleMon_GetValue(currentMon, VALUE_ABILITY);
        if (!j_j_IsRolePlayFailAbility(ability)) {
            HandlerParam_ChangeAbility* changeAbility;
            changeAbility = (HandlerParam_ChangeAbility*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_ABILITY, pokemonSlot);
            changeAbility->pokeID = pokemonSlot;
            changeAbility->ability = ability;
            BattleHandler_StrSetup(&changeAbility->exStr, 2u, 619);
            BattleHandler_AddArg(&changeAbility->exStr, pokemonSlot);
            BattleHandler_AddArg(&changeAbility->exStr, currentSlot);
            BattleHandler_AddArg(&changeAbility->exStr, changeAbility->ability);
            BattleHandler_PopWork(serverFlow, changeAbility);
        }
    }
}
BattleEventHandlerTableEntry ReceiverHandlers[]{
    {EVENT_NOTIFY_FAINTED, HandlerReceiver},
};
extern "C" BattleEventHandlerTableEntry* EventAddReceiver(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ReceiverHandlers);
    return ReceiverHandlers;
}

// BEAST BOOST
extern "C" StatStage GetHighestStat(BattleMon* battleMon) {
    u16 highest = battleMon->attack;
    StatStage output = STATSTAGE_ATTACK;

    if (highest < battleMon->defense) {
        highest = battleMon->defense;
        output = STATSTAGE_DEFENSE;
    }
    if (highest < battleMon->specialAttack) {
        highest = battleMon->specialAttack;
        output = STATSTAGE_SPECIAL_ATTACK;
    }
    if (highest < battleMon->specialDefense) {
        highest = battleMon->specialDefense;
        output = STATSTAGE_SPECIAL_DEFENSE;
    }
    if (highest < battleMon->speed) {
        highest = battleMon->speed;
        output = STATSTAGE_SPEED;
    }

    return output;
}
extern "C" void HandlerBeastBoost(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        u32 targetCount = BattleEventVar_GetValue(VAR_TARGET_COUNT);
        for (u32 target = 0; target < targetCount; ++target) {

            u32 targetSlot = BattleEventVar_GetValue((BattleEventVar)(VAR_TARGET_MON_ID + target));
            BattleMon* targetMon = Handler_GetBattleMon(serverFlow, targetSlot);
            if (BattleMon_IsFainted(targetMon)) {
                BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

                HandlerParam_ChangeStatStage* statChange;
                statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                statChange->pokeCount = 1;
                statChange->pokeID[0] = pokemonSlot;
                statChange->stat = GetHighestStat(attackingMon);
                statChange->volume = 1;
                BattleHandler_PopWork(serverFlow, statChange);
            }
        }
    }
}
BattleEventHandlerTableEntry BeastBoostHandlers[]{
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerBeastBoost},
};
extern "C" BattleEventHandlerTableEntry* EventAddBeastBoost(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(BeastBoostHandlers);
    return BeastBoostHandlers;
}

// ELECTRIC SURGE
void CommonTerrainChangeAbility(ServerFlow* serverFlow, u32 pokemonSlot, TERRAIN terrain, u16 msgID) {
    if (BattleField_GetTerrain(g_BattleField) != terrain) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_AddFieldEffect* addFieldEffect;
        addFieldEffect = (HandlerParam_AddFieldEffect*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_FIELD_EFFECT, pokemonSlot);
        addFieldEffect->effect = FLDEFF_TERRAIN;
        addFieldEffect->field_D = terrain;
        addFieldEffect->condData = Condition_MakeTurn(TERRAIN_ABILITY_TURNS);
        BattleHandler_StrSetup(&addFieldEffect->exStr, 1u, msgID);
        BattleHandler_AddArg(&addFieldEffect->exStr, pokemonSlot);
        BattleHandler_PopWork(serverFlow, addFieldEffect);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
extern "C" void HandlerElectricSurge(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        CommonTerrainChangeAbility(serverFlow, pokemonSlot, TERRAIN_ELECTRIC, BATTLE_ELECTRIC_TERRAIN_MSGID);
    }
}
BattleEventHandlerTableEntry ElectricSurgeHandlers[]{
    {EVENT_SWITCH_IN, HandlerElectricSurge},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerElectricSurge},
};
extern "C" BattleEventHandlerTableEntry* EventAddElectricSurge(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ElectricSurgeHandlers);
    return ElectricSurgeHandlers;
}

// PSYCHIC SURGE
extern "C" void HandlerPsychicSurge(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        CommonTerrainChangeAbility(serverFlow, pokemonSlot, TERRAIN_PSYCHIC, BATTLE_PSYCHIC_TERRAIN_MSGID);
    }
}
BattleEventHandlerTableEntry PsychicSurgeHandlers[]{
    {EVENT_SWITCH_IN, HandlerPsychicSurge},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerPsychicSurge},
};
extern "C" BattleEventHandlerTableEntry* EventAddPsychicSurge(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PsychicSurgeHandlers);
    return PsychicSurgeHandlers;
}

// MISTY SURGE
extern "C" void HandlerMistySurge(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        CommonTerrainChangeAbility(serverFlow, pokemonSlot, TERRAIN_MISTY, BATTLE_MISTY_TERRAIN_MSGID);
    }
}
BattleEventHandlerTableEntry MistySurgeHandlers[]{
    {EVENT_SWITCH_IN, HandlerMistySurge},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerMistySurge},
};
extern "C" BattleEventHandlerTableEntry* EventAddMistySurge(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MistySurgeHandlers);
    return MistySurgeHandlers;
}

// GRASSY SURGE
extern "C" void HandlerGrassySurge(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        CommonTerrainChangeAbility(serverFlow, pokemonSlot, TERRAIN_GRASSY, BATTLE_GRASSY_TERRAIN_MSGID);
    }
}
BattleEventHandlerTableEntry GrassySurgeHandlers[]{
    {EVENT_SWITCH_IN, HandlerGrassySurge},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerGrassySurge},
};
extern "C" BattleEventHandlerTableEntry* EventAddGrassySurge(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(GrassySurgeHandlers);
    return GrassySurgeHandlers;
}

// PRISM ARMOR
extern "C" void HandlerPrismArmor(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        u32 effectiveness = BattleEventVar_GetValue(VAR_TYPE_EFFECTIVENESS);
        if (effectiveness > EFFECTIVENESS_1) {
            BattleEventVar_MulValue(VAR_RATIO, 3072);
        }
    }
}
BattleEventHandlerTableEntry PrismArmorHandlers[]{
    {EVENT_MOVE_DAMAGE_PROCESSING_2, HandlerPrismArmor},
};
extern "C" BattleEventHandlerTableEntry* EventAddPrismArmor(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PrismArmorHandlers);
    return PrismArmorHandlers;
}

// NEUROFORCE
extern "C" void HandlerNeuroforce(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        u32 effectiveness = BattleEventVar_GetValue(VAR_TYPE_EFFECTIVENESS);
        if (effectiveness > EFFECTIVENESS_1) {
            BattleEventVar_MulValue(VAR_RATIO, 5120);
        }
    }
}
BattleEventHandlerTableEntry NeuroforceHandlers[]{
    {EVENT_MOVE_DAMAGE_PROCESSING_2, HandlerNeuroforce},
};
extern "C" BattleEventHandlerTableEntry* EventAddNeuroforce(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(NeuroforceHandlers);
    return NeuroforceHandlers;
}

// INTREPID SWORD
extern "C" void HandlerIntrepidSword(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
#if GEN9_INTREPID_SWORD
        if (BattleField_CheckIntrepidSwordFlag(pokemonSlot)) {
            return;
        }
        BattleField_SetIntrepidSwordFlag(pokemonSlot);
#endif
        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        statChange->stat = STATSTAGE_ATTACK;
        statChange->volume = 1;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}
BattleEventHandlerTableEntry IntrepidSwordHandlers[]{
    {EVENT_SWITCH_IN, HandlerIntrepidSword},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerIntrepidSword},
};
extern "C" BattleEventHandlerTableEntry* EventAddIntrepidSword(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(IntrepidSwordHandlers);
    return IntrepidSwordHandlers;
}

// DAUNTLESS SHIELD
extern "C" void HandlerDauntlessShield(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
#if GEN9_DAUNTLESS_SHIELD
        if (BattleField_CheckDauntlessShieldFlag(pokemonSlot)) {
            return;
        }
        BattleField_SetDauntlessShieldFlag(pokemonSlot);
#endif
        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        statChange->stat = STATSTAGE_DEFENSE;
        statChange->volume = 1;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}
BattleEventHandlerTableEntry DauntlessShieldHandlers[]{
    {EVENT_SWITCH_IN, HandlerDauntlessShield},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerDauntlessShield},
};
extern "C" BattleEventHandlerTableEntry* EventAddDauntlessShield(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(DauntlessShieldHandlers);
    return DauntlessShieldHandlers;
}

// BALL FETCH
extern "C" void HandlerBallFetch(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // Has no item
        if (BattleMon_GetHeldItem(currentMon) == IT_NULL) {
        ActionOrderWork* actionOrderWork = serverFlow->actionOrderWork;
        for (u8 i = 0; i < 6; ++i) {
            u32 action = BattleAction_GetAction(&actionOrderWork[i].action);
            // Action is an item used by the player
            if (action == 2) {

                BattleAction_Item* itemAction = (BattleAction_Item*)&actionOrderWork[i].action;
                ITEM_ID itemID = itemAction->itemID;
                // Item is a PokéBall
                if (ItemGetParam(itemID, ITSTAT_CLASS) == 4) {
                    HandlerParam_SetItem* setItem;
                    setItem = (HandlerParam_SetItem*)BattleHandler_PushWork(serverFlow, EFFECT_SET_HELD_ITEM, pokemonSlot);
                    setItem->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                    setItem->itemID = itemID;
                    setItem->pokeID = pokemonSlot;
                    setItem->clearConsume = 1;
                    BattleHandler_StrSetup(&setItem->exStr, 2u, BATTLE_BALL_FETCH_MSGID);
                    BattleHandler_AddArg(&setItem->exStr, pokemonSlot);
                    BattleHandler_AddArg(&setItem->exStr, itemID);
                    BattleHandler_PopWork(serverFlow, setItem);
                }
            }
        }
        }
    }
}
BattleEventHandlerTableEntry HandlerBallHandlers[]{
    {EVENT_TURN_CHECK_DONE, HandlerBallFetch},
};
extern "C" BattleEventHandlerTableEntry* EventAddBallFetch(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(HandlerBallHandlers);
    return HandlerBallHandlers;
}

// COTTON DOWN
extern "C" void HandlerCottonDown(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {

        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        statChange->pokeCount = 0;

        for (u8 i = 0; i < 24; ++i) {
            BattleMon* affectedMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
            if (affectedMon) {

                u32 affectedSlot = BattleMon_GetID(affectedMon);
                if (affectedMon->battleSlot != pokemonSlot) {

                    u32 affectedPos = Handler_PokeIDToPokePos(serverFlow, affectedSlot);
                    // If it's currently in the battlefield
                    if (affectedPos != NULL_BATTLE_POS) {
                        if (!BattleMon_IsFainted(affectedMon)) {
                            statChange->pokeID[statChange->pokeCount] = affectedSlot;
                            ++statChange->pokeCount;
                        }
                    }
                }
            }
        }

        statChange->moveAnimation = 1;
        statChange->stat = STATSTAGE_SPEED;
        statChange->volume = -1;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}
BattleEventHandlerTableEntry CottonDownHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerCottonDown},
};
extern "C" BattleEventHandlerTableEntry* EventAddCottonDown(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(CottonDownHandlers);
    return CottonDownHandlers;
}

// PROPELLER TAIL
extern "C" void HandlerPropellerTail(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (IS_NOT_NEW_EVENT) return;

    // Got through all the actions this turn
    for (u8 currentActionIdx = 0; currentActionIdx < 6; ++currentActionIdx) {
        ActionOrderWork* currentActionOrder = &serverFlow->actionOrderWork[currentActionIdx];

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // If the action is performed by a Propeller Tail or Stalwart Pokémon
        if (currentActionOrder->battleMon) {
            if (currentActionOrder->battleMon == currentMon) {
                u32 action = BattleAction_GetAction(&currentActionOrder->action);
                // And the action is a fight move
                if (action == 1) {
                    BattleAction_Fight* fight = (BattleAction_Fight*)&currentActionOrder->action;
                    if (fight->targetPos != NULL_BATTLE_POS) {

                        u32 swapPos1 = Handler_PokeIDToPokePos(serverFlow, BattleEventVar_GetValue(NEW_VAR_ATTACKING_MON));
                        u32 swapPos2 = Handler_PokeIDToPokePos(serverFlow, BattleEventVar_GetValue(NEW_VAR_DEFENDING_MON));
                        // Swap the target so it remains as it was originally selected
                        if (fight->targetPos == swapPos1) {
                            fight->targetPos = swapPos2;
                        }
                        else if (fight->targetPos == swapPos2) {
                            fight->targetPos = swapPos1;
                        }

                    }
                }
            }
        }
    }
}
BattleEventHandlerTableEntry PropellerTailHandlers[]{
    {EVENT_SWAP_POKE, HandlerPropellerTail},
};
extern "C" BattleEventHandlerTableEntry* EventAddPropellerTail(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PropellerTailHandlers);
    return PropellerTailHandlers;
}

// MIRROR ARMOR
extern "C" void HandlerMirrorArmor(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
    int volume = BattleEventVar_GetValue(VAR_VOLUME);
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)
        && pokemonSlot != attackingSlot
        && volume < 0
        && !BattleEventVar_GetValue(VAR_MIRROR_ARMOR_FLAG)) {
        BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);

        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = attackingSlot;
        statChange->stat = (StatStage)BattleEventVar_GetValue(VAR_MOVE_EFFECT);
        statChange->volume = volume;
        statChange->pad = STAT_CHANGE_MIRROR_ARMOR_FLAG;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}
BattleEventHandlerTableEntry MirrorArmorHandlers[]{
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerMirrorArmor},
};
extern "C" BattleEventHandlerTableEntry* EventAddMirrorArmor(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MirrorArmorHandlers);
    return MirrorArmorHandlers;
}

// GULP MISSILE
extern "C" void HandlerGulpMissileCatch(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
    if (pokemonSlot == currentSlot &&
        (moveID == MOVE057_SURF || moveID == MOVE291_DIVE)) {

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
        // TODO: ADD CRAMORANT
        if (BattleMon_GetSpecies(currentMon) == PK351_CASTFORM &&
            BattleMon_GetValue(currentMon, VALUE_FORM) == 0) {

            HandlerParam_ChangeForm* changeForm;
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->pokeID = pokemonSlot;

            u32 currentHP = BattleMon_GetValue(currentMon, VALUE_CURRENT_HP);
            if (currentHP >= DivideMaxHPZeroCheck(currentMon, 2u)) {
                changeForm->newForm = 1;
            }
            else {
                changeForm->newForm = 2;
            }

            BattleHandler_PopWork(serverFlow, changeForm);
        }
    }
}
extern "C" void HandlerGulpMissileDamage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (pokemonSlot == defendingSlot) {
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, defendingSlot);
        u32 prevForm = BattleMon_GetValue(defendingMon, VALUE_FORM);
        // TODO: ADD CRAMORANT
        if (BattleMon_GetSpecies(defendingMon) == PK351_CASTFORM &&
            prevForm != 0) {
            HandlerParam_ChangeForm* changeForm;
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = 0;
            BattleHandler_PopWork(serverFlow, changeForm);

            u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);

            HandlerParam_Damage* damage;
            damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
            damage->pokeID = attackingSlot;
            damage->damage = DivideMaxHPZeroCheck(attackingMon, 4u);
            BattleHandler_PopWork(serverFlow, damage);

            if (prevForm == 1) {
                HandlerParam_ChangeStatStage* statChange;
                statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                statChange->pokeCount = 1;
                statChange->pokeID[0] = attackingSlot;
                statChange->stat = STATSTAGE_DEFENSE;
                statChange->volume = -1;
                BattleHandler_PopWork(serverFlow, statChange);
            }
            else if (prevForm == 2) {
                HandlerParam_AddCondition* addCondition;
                addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
                addCondition->condition = CONDITION_PARALYSIS;
                addCondition->condData = MakeBasicStatus(CONDITION_PARALYSIS);
                addCondition->almost = 0;
                addCondition->pokeID = attackingSlot;
                BattleHandler_PopWork(serverFlow, addCondition);
            }
        }
    }
}
BattleEventHandlerTableEntry GulpMissileHandlers[]{
    {EVENT_MOVE_EXECUTE_START, HandlerGulpMissileCatch},
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerGulpMissileDamage},
};
extern "C" BattleEventHandlerTableEntry* EventAddGulpMissile(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(GulpMissileHandlers);
    return GulpMissileHandlers;
}

// STEAM ENGINE
extern "C" void HandlerSteamEngine(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_WATER || BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_FIRE)) {

        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        statChange->stat = STATSTAGE_SPEED;
        statChange->volume = 6;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}
BattleEventHandlerTableEntry SteamEngineHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerSteamEngine},
};
extern "C" BattleEventHandlerTableEntry* EventAddSteamEngine(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SteamEngineHandlers);
    return SteamEngineHandlers;
}

// PUNK ROCK
extern "C" void HandlerPunkRockPower(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (getMoveFlag(BattleEventVar_GetValue(VAR_MOVE_ID), FLAG_SOUND)) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 5461);
        }
    }
}
extern "C" void HandlerPunkRockDamage(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (getMoveFlag(BattleEventVar_GetValue(VAR_MOVE_ID), FLAG_SOUND)) {
            BattleEventVar_MulValue(VAR_RATIO, 2048);
        }
    }
}
BattleEventHandlerTableEntry PunkRockHandlers[]{
    {EVENT_MOVE_POWER, HandlerPunkRockPower},
    {EVENT_MOVE_DAMAGE_PROCESSING_2, HandlerPunkRockDamage},
};
extern "C" BattleEventHandlerTableEntry* EventAddPunkRock(u32* handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(PunkRockHandlers);
    return PunkRockHandlers;
}

// SAND SPIT
extern "C" void HandlerSandSpit(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        HandlerParam_ChangeWeather* changeWeather;
        changeWeather = (HandlerParam_ChangeWeather*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_WEATHER, pokemonSlot);
        changeWeather->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        changeWeather->weather = WEATHER_SANDSTORM;
        changeWeather->turn = INFINITE_FIELD_TURNS;
        BattleHandler_PopWork(serverFlow, changeWeather);
    }
}
BattleEventHandlerTableEntry SandSpitHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerSandSpit},
};
extern "C" BattleEventHandlerTableEntry* EventAddSandSpit(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SandSpitHandlers);
    return SandSpitHandlers;
}

// ICE SCALES
extern "C" void HandlerIceScales(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_SPECIAL) {
            BattleEventVar_MulValue(VAR_RATIO, 2048);
        }
    }
}
BattleEventHandlerTableEntry IceScalesHandlers[]{
    {EVENT_MOVE_DAMAGE_PROCESSING_2, HandlerIceScales},
};
extern "C" BattleEventHandlerTableEntry* EventAddIceScales(u32* handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(IceScalesHandlers);
    return IceScalesHandlers;
}

// RIPEN
extern "C" void HandlerRipen(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        ITEM_ID itemID = BattleEventVar_GetValue(VAR_ITEM);
        if (PML_ItemIsBerry(itemID)) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}
extern "C" void HandlerRipenNEW(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (IS_NOT_NEW_EVENT) return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID)) {
        ITEM_ID itemID = BattleEventVar_GetValue(VAR_ITEM);
        if (PML_ItemIsBerry(itemID)) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}
BattleEventHandlerTableEntry RipenHandlers[]{
    {EVENT_USE_ITEM, HandlerRipen},
    {EVENT_USE_TEMP_ITEM_BEFORE, HandlerRipenNEW},
};
extern "C" BattleEventHandlerTableEntry* EventAddRipen(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(RipenHandlers);
    return RipenHandlers;
}

// ICE FACE
extern "C" void IceFaceRevertForm(ServerFlow * serverFlow, u32 pokemonSlot) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    // TODO: ADD EISCUE
    if (BattleMon_GetSpecies(currentMon) == PK351_CASTFORM &&
        BattleMon_GetValue(currentMon, VALUE_FORM) == 1) {

        if (ServerEvent_GetWeather(serverFlow) == WEATHER_HAIL) {
            HandlerParam_ChangeForm* changeForm;
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = 0;
            BattleHandler_StrSetup(&changeForm->exStr, 2u, BATTLE_DISGUISE_MSGID);
            BattleHandler_PopWork(serverFlow, changeForm);
        }
    }
}
extern "C" void HandlerIceFacePreventDamage(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    // TODO: ADD EISCUE
    if (BattleMon_GetSpecies(currentMon) == PK351_CASTFORM &&
        pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_PHYSICAL) {
        if (BattleMon_GetValue(currentMon, VALUE_FORM) == 0) {
            // General Flag set to 2 [ServerControl_ApplyDamageToRecover]
            BattleEventVar_RewriteValue(VAR_GENERAL_USE_FLAG, 2);

            HandlerParam_ChangeForm* changeForm;
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = 1;
            BattleHandler_StrSetup(&changeForm->exStr, 2u, BATTLE_DISGUISE_MSGID);
            BattleHandler_PopWork(serverFlow, changeForm);
        }
    }
}
extern "C" void HandlerIceFaceHailCheck(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    IceFaceRevertForm(serverFlow, pokemonSlot);
}
extern "C" void HandlerIceFaceSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        IceFaceRevertForm(serverFlow, pokemonSlot);
    }
}
BattleEventHandlerTableEntry IceFaceHandlers[]{
    {EVENT_CHECK_DAMAGE_TO_RECOVER, HandlerIceFacePreventDamage},
    {EVENT_AFTER_WEATHER_CHANGE, HandlerIceFaceHailCheck},
    {EVENT_SWITCH_IN, HandlerIceFaceSwitchIn},
};
extern "C" BattleEventHandlerTableEntry* EventAddIceFace(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(IceFaceHandlers);
    return IceFaceHandlers;
}

// POWER SPOT
extern "C" void HandlerPowerSpot(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
    if (pokemonSlot != attackingSlot&&
        MainModule_IsAllyMonID(pokemonSlot, attackingSlot)) {

        BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 5461);
    }
}
BattleEventHandlerTableEntry PowerSpotHandlers[]{
    {EVENT_MOVE_POWER, HandlerPowerSpot},
};
extern "C" BattleEventHandlerTableEntry* EventAddPowerSpot(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PowerSpotHandlers);
    return PowerSpotHandlers;
}

// MIMICRY
extern "C" u16 GetSpeciesTypePair(u16 species, u16 form) {
    POKE_TYPE type1 = PML_PersonalGetParamSingle(species, form, Personal_Type1);
    POKE_TYPE type2 = PML_PersonalGetParamSingle(species, form, Personal_Type2);

    return PokeTypePair_Make((u16)type1, (u16)type2);
}
extern "C" void MimicryTypeChange(ServerFlow* serverFlow, u32 pokemonSlot, b32 revert) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    u16 newType = GetSpeciesTypePair(currentMon->species, currentMon->form);

    if (!revert) {
        TERRAIN terrain = ServerEvent_GetTerrain(serverFlow);
        switch (terrain) {
        case TERRAIN_NULL:
            break;
        case TERRAIN_ELECTRIC:
            newType = PokeTypePair_MakeMonotype(TYPE_ELEC);
            break;
        case TERRAIN_GRASSY:
            newType = PokeTypePair_MakeMonotype(TYPE_GRASS);
            break;
        case TERRAIN_MISTY:
            // TODO: ADD FAIRY TYPE
            newType = PokeTypePair_MakeMonotype(TYPE_PSY);
            break;
        case TERRAIN_PSYCHIC:
            newType = PokeTypePair_MakeMonotype(TYPE_PSY);
            break;
        }
    }

    if (newType != BattleMon_GetPokeType(currentMon)) {
        HandlerParam_ChangeType* changeType;
        changeType = (HandlerParam_ChangeType*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_TYPE, pokemonSlot);
        changeType->pokeType = newType;
        changeType->pokeID = pokemonSlot;
        BattleHandler_PopWork(serverFlow, changeType);
    }
}
extern "C" void HandlerMimicry(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        MimicryTypeChange(serverFlow, pokemonSlot, 0);
    }
}
extern "C" void HandlerMimicryTerrainChange(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;
    
    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID)) {
        MimicryTypeChange(serverFlow, pokemonSlot, 0);
    }
}
extern "C" void HandlerMimicryNullified(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        MimicryTypeChange(serverFlow, pokemonSlot, 1);
    }
}
BattleEventHandlerTableEntry MimicryHandlers[]{
    {EVENT_SWITCH_IN, HandlerMimicry},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerMimicry},
    {EVENT_AFTER_TERRAIN_CHANGE, HandlerMimicryTerrainChange},
    {EVENT_ABILITY_NULLIFIED, HandlerMimicryNullified},
};
extern "C" BattleEventHandlerTableEntry* EventAddMimicry(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MimicryHandlers);
    return MimicryHandlers;
}

// SCREEN CLEANER
extern "C" void HandlerScreenCleaner(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_RemoveSideEffect* removeAllySideEffect;
        removeAllySideEffect = (HandlerParam_RemoveSideEffect*)BattleHandler_PushWork(serverFlow, EFFECT_REMOVE_SIDE_EFFECT, pokemonSlot);
        removeAllySideEffect->side = 0;
        removeAllySideEffect->flags[0] = 3;
        removeAllySideEffect->flags[1] |= 1u;
        removeAllySideEffect->flags[1] |= 2u;
        BattleHandler_PopWork(serverFlow, removeAllySideEffect);

        HandlerParam_RemoveSideEffect* removeEnemySideEffect;
        removeEnemySideEffect = (HandlerParam_RemoveSideEffect*)BattleHandler_PushWork(serverFlow, EFFECT_REMOVE_SIDE_EFFECT, pokemonSlot);
        removeEnemySideEffect->side = 1;
        removeEnemySideEffect->flags[0] = 3;
        removeEnemySideEffect->flags[1] |= 1u;
        removeEnemySideEffect->flags[1] |= 2u;
        BattleHandler_PopWork(serverFlow, removeEnemySideEffect);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
BattleEventHandlerTableEntry ScreenCleanerHandlers[]{
    {EVENT_SWITCH_IN, HandlerScreenCleaner},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerScreenCleaner},
};
extern "C" BattleEventHandlerTableEntry* EventAddScreenCleaner(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ScreenCleanerHandlers);
    return ScreenCleanerHandlers;
}

// STEELY SPIRIT
extern "C" void HandlerSteelySpirit(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
    if (pokemonSlot == attackingSlot || MainModule_IsAllyMonID(pokemonSlot, attackingSlot)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_STEEL) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 6144);
        }
    }
}
BattleEventHandlerTableEntry SteelySpiritHandlers[]{
    {EVENT_MOVE_POWER, HandlerSteelySpirit},
};
extern "C" BattleEventHandlerTableEntry* EventAddSteelySpirit(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SteelySpiritHandlers);
    return SteelySpiritHandlers;
}

// PERISH BODY
extern "C" void HandlerPerishBody(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (pokemonSlot == defendingSlot &&
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            HandlerParam_AddCondition* addConditionAtk;
            addConditionAtk = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
            addConditionAtk->pokeID = attackingSlot;
            addConditionAtk->condition = CONDITION_PERISHSONG;
            addConditionAtk->condData = Condition_MakeTurn(4);
            BattleHandler_PopWork(serverFlow, addConditionAtk);
            
            HandlerParam_AddCondition* addConditionDef;
            addConditionDef = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
            addConditionDef->pokeID = defendingSlot;
            addConditionDef->condition = CONDITION_PERISHSONG;
            addConditionDef->condData = Condition_MakeTurn(4);
            BattleHandler_PopWork(serverFlow, addConditionDef);

            HandlerParam_Message* message;
            message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
            BattleHandler_StrSetup(&message->str, 1u, BATTLE_PERISH_BODY_MSGID);
            BattleHandler_PopWork(serverFlow, message);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}
BattleEventHandlerTableEntry PerishBodyHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerPerishBody},
};
extern "C" BattleEventHandlerTableEntry* EventAddPerishBody(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PerishBodyHandlers);
    return PerishBodyHandlers;
}

// WANDERING SPIRIT
extern "C" void HandlerWanderingSpirit(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (pokemonSlot == defendingSlot &&
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {
            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
            BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, defendingSlot);

            PokeSet targetSet;
            targetSet.battleMon[0] = defendingMon;
            targetSet.count = 1;
            ServerDisplay_SkillSwap(serverFlow, attackingMon, &targetSet);
        }
    }
}
BattleEventHandlerTableEntry WanderingSpiritHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerWanderingSpirit},
};
extern "C" BattleEventHandlerTableEntry* EventAddWanderingSpirit(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(WanderingSpiritHandlers);
    return WanderingSpiritHandlers;
}

// GORILLA TACTICS
extern "C" void HandlerChoiceItemCommonMoveLock(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work);
extern "C" void HandlerChoiceBandPower(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work);
extern "C" void HandlerChoiceItemCommonItemChange(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work);
BattleEventHandlerTableEntry GorillaTacticsHandlers[]{
    {EVENT_CHOOSE_MOVE, HandlerChoiceItemCommonMoveLock},
    {EVENT_ATTACKER_POWER, HandlerChoiceBandPower},
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerChoiceItemCommonItemChange},
    {EVENT_ABILITY_NULLIFIED, HandlerChoiceItemCommonItemChange},
};
extern "C" BattleEventHandlerTableEntry* EventAddGorillaTactics(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(GorillaTacticsHandlers);
    return GorillaTacticsHandlers;
}

// NEUTRALIZING GAS
ABILITY nonNeutralizableAbilities[] = {
    ABIL121_MULTITYPE,
    ABIL161_ZEN_MODE,
    ABIL176_STANCE_CHANGE,
    ABIL197_SHIELDS_DOWN,
    ABIL208_SCHOOLING,
    ABIL209_DISGUISE,
    ABIL210_BATTLE_BOND,
    ABIL213_COMATOSE,
    ABIL225_RKS_SYSTEM,
    ABIL241_GULP_MISSILE,
    ABIL248_ICE_FACE,
    ABIL256_NEUTRALIZING_GAS,
    ABIL266_AS_ONE,
    ABIL267_AS_ONE,
#if NEUTRALIZE_PARADOX_ABILITIES
    ABIL281_PROTOSYNTHESIS,
    ABIL282_QUARK_DRIVE,
#endif
    ABIL307_TERA_SHIFT,
};
extern "C" b32 AbilityCanBeNeutralized(ABILITY ability) {
    for (u16 i = 0; i < ARRAY_COUNT(nonNeutralizableAbilities); ++i) {
        if (nonNeutralizableAbilities[i] == ability) {
            return 0;
        }
    }
    return 1;
}
extern "C" void NeutralizingGasEnd(ServerFlow* serverFlow, u32 pokemonSlot) {
    BattleField_RemoveNeutralizingGasMon();
    
    // Only trigger the nullify cured events if there are no more Neutralizing Gas Pokémon
    if (BattleField_GetNeutralizingGasMons() == 0) {
        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 1u, BATTLE_NEUTRALIZING_GAS_END_MSGID);
        BattleHandler_PopWork(serverFlow, message);

        for (u8 i = 0; i < 24; ++i) {
            BattleMon* affectedMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
            if (affectedMon) {

                u32 affectedPos = Handler_PokeIDToPokePos(serverFlow, BattleMon_GetID(affectedMon));
                // If it's currently in the battlefield
                if (affectedPos != NULL_BATTLE_POS &&
                    !BattleMon_IsFainted(affectedMon)) {

                    ServerEvent_AbilityNullifyCured(serverFlow, affectedMon);
                }
            }
        }
    }
}
extern "C" void HandlerNeutralizingGasStart(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID)) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 1u, BATTLE_NEUTRALIZING_GAS_START_MSGID);
        BattleHandler_PopWork(serverFlow, message);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);

        // Only trigger the nullify events if Neutralizing Gas is not already active
        if (BattleField_GetNeutralizingGasMons() == 0) {
            for (u8 i = 0; i < 24; ++i) {
                BattleMon* affectedMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
                if (affectedMon) {

                    u32 affectedPos = Handler_PokeIDToPokePos(serverFlow, BattleMon_GetID(affectedMon));
                    // If it's currently in the battlefield
                    if (affectedPos != NULL_BATTLE_POS &&
                        !BattleMon_IsFainted(affectedMon)) {

                        // Neutralizing Gas can't be neutralized by another Nutralizing Gas
                        // but since it can be neutralized by Gastro Acid it has a EVENT_ABILITY_NULLIFIED event
                        // and thus we have to make an special case here
                        if (BattleMon_GetValue(affectedMon, VALUE_ABILITY) != ABIL256_NEUTRALIZING_GAS) {
                            ServerEvent_AbilityNullified(serverFlow, affectedMon);
                        }
                    }
                }
            }
        }

        BattleField_AddNeutralizingGasMon();
    }
}
extern "C" void HandlerNeutralizingGasEnd(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        NeutralizingGasEnd(serverFlow, pokemonSlot);
    }
}
extern "C" void HandlerNeutralizingGasEndFainted(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        BattleMon_IsFainted(Handler_GetBattleMon(serverFlow, pokemonSlot))) {
        NeutralizingGasEnd(serverFlow, pokemonSlot);
    }
}
BattleEventHandlerTableEntry NeutralizingGasHandlers[]{
    {EVENT_SWITCH_IN_PRIO, HandlerNeutralizingGasStart},
    {EVENT_SWITCH_OUT_END, HandlerNeutralizingGasEnd},
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerNeutralizingGasEnd},
    {EVENT_ABILITY_NULLIFIED, HandlerNeutralizingGasEnd},
    {EVENT_NOTIFY_FAINTED, HandlerNeutralizingGasEndFainted},
};
extern "C" BattleEventHandlerTableEntry* EventAddNeutralizingGas(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(NeutralizingGasHandlers);
    return NeutralizingGasHandlers;
}

// PASTEL VEIL
extern "C" void HandlerPastelVeilPreventPoison(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (pokemonSlot == defendingSlot || MainModule_IsAllyMonID(pokemonSlot, defendingSlot)) {
        if (BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_POISON) {
            *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, FORCE_FAIL_MESSAGE);
        }
    }
}
extern "C" void HandlerPastelVeilFailMessage(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if ((pokemonSlot == defendingSlot || MainModule_IsAllyMonID(pokemonSlot, defendingSlot)) &&
        *work) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_Message* message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, 210u);
        BattleHandler_AddArg(&message->str, defendingSlot);
        BattleHandler_PopWork(serverFlow, message);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        *work = 0;
    }
}
extern "C" void HandlerPastelVeilCurePoisonAlly(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if (pokemonSlot == currentSlot) {
        for (u8 i = 0; i < 24; ++i) {
            BattleMon* affectedMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
            if (affectedMon) {

                u32 affectedSlot = BattleMon_GetID(affectedMon);
                if (MainModule_IsAllyMonID(pokemonSlot, affectedSlot)) {

                    u32 affectedPos = Handler_PokeIDToPokePos(serverFlow, affectedSlot);
                    // If it's currently in the battlefield
                    if (affectedPos != NULL_BATTLE_POS &&
                        !BattleMon_IsFainted(affectedMon)) {

                        if (BattleMon_CheckIfMoveCondition(affectedMon, CONDITION_POISON)) {
                            HandlerParam_CureCondition* cureCondition;
                            cureCondition = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
                            cureCondition->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                            cureCondition->condition = CONDITION_POISON;
                            cureCondition->pokeID[0] = affectedSlot;
                            cureCondition->pokeCount = 1;
                            BattleHandler_PopWork(serverFlow, cureCondition);
                        }
                    }
                }
            }
        }
    }
}
extern "C" void HandlerPastelVeilCurePoisonSelf(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_POISON)) {
        HandlerParam_CureCondition* cureCondition;
        cureCondition = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
        cureCondition->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        cureCondition->condition = CONDITION_POISON;
        cureCondition->pokeID[0] = pokemonSlot;
        cureCondition->pokeCount = 1;
        BattleHandler_PopWork(serverFlow, cureCondition);
    }
}
BattleEventHandlerTableEntry PastelVeilHandlers[]{
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerPastelVeilPreventPoison},
    {EVENT_ADD_CONDITION_FAIL, HandlerPastelVeilFailMessage},
    {EVENT_SWITCH_IN, HandlerPastelVeilCurePoisonAlly},
    {EVENT_ACTION_PROCESSING_END, HandlerPastelVeilCurePoisonSelf},
};
extern "C" BattleEventHandlerTableEntry* EventAddPastelVeil(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PastelVeilHandlers);
    return PastelVeilHandlers;
}

// HUNGER SWITCH
extern "C" void HandlerHungerSwitch(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // TODO: ADD MORPEKO
        if (BattleMon_GetSpecies(currentMon) == PK351_CASTFORM) {
            u32 form = BattleMon_GetValue(currentMon, VALUE_FORM);

            if (form == 0) {
                form = 1;
            }
            else {
                form = 0;
            }

            HandlerParam_ChangeForm* changeForm;
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = form;
            BattleHandler_PopWork(serverFlow, changeForm);
        }
    }
}
BattleEventHandlerTableEntry HungerSwitchHandlers[]{
    {EVENT_TURN_CHECK_DONE, HandlerHungerSwitch},
};
extern "C" BattleEventHandlerTableEntry* EventAddHungerSwitch(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(HungerSwitchHandlers);
    return HungerSwitchHandlers;
}

// QUICK DRAW
extern "C" void HandlerQuickDraw(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        if (AbilityEvent_RollEffectChance(serverFlow, 30) && 
            BattleEventVar_RewriteValue(VAR_PRIORITY, 2)) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            HandlerParam_Message* message;
            message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
            BattleHandler_StrSetup(&message->str, 2u, BATTLE_QUICK_DRAW_MSGID);
            BattleHandler_AddArg(&message->str, pokemonSlot);
            BattleHandler_AddArg(&message->str, item->subID);
            BattleHandler_PopWork(serverFlow, message);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}
BattleEventHandlerTableEntry QuickDrawHandlers[]{
    {EVENT_CHECK_SPECIAL_PRIORITY, HandlerQuickDraw},
};
extern "C" BattleEventHandlerTableEntry* EventAddQuickDraw(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(QuickDrawHandlers);
    return QuickDrawHandlers;
}

// UNSEEN FIST
extern "C" void HandlerUnseenFistBreakProtect(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (IsContact(serverFlow, moveID, pokemonSlot)) {
            BattleEventVar_RewriteValue(VAR_GENERAL_USE_FLAG, 1);
        }
    }
}
// TODO: ADD UNSEEN FIST IGNORING WIDE GUARD AND QUICK GUARD
BattleEventHandlerTableEntry UnseenFistHandlers[]{
    {EVENT_CHECK_PROTECT_BREAK, HandlerUnseenFistBreakProtect},
};
extern "C" BattleEventHandlerTableEntry* EventAddUnseenFist(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(UnseenFistHandlers);
    return UnseenFistHandlers;
}

// CURIOUS MEDICINE
extern "C" void ResetNegativeStatStages(BattleMon* battleMon) {

    if (battleMon->statStageParam.AttackStage < 6) {
        battleMon->statStageParam.AttackStage = 6;
    }
    if (battleMon->statStageParam.DefenseStage < 6) {
        battleMon->statStageParam.DefenseStage = 6;
    }
    if (battleMon->statStageParam.SpeedStage < 6) {
        battleMon->statStageParam.SpeedStage = 6;
    }
    if (battleMon->statStageParam.SpAttackStage < 6) {
        battleMon->statStageParam.SpAttackStage = 6;
    }
    if (battleMon->statStageParam.SpDefenseStage < 6) {
        battleMon->statStageParam.SpDefenseStage = 6;
    }
    if (battleMon->statStageParam.AccuracyStage < 6) {
        battleMon->statStageParam.AccuracyStage = 6;
    }
    if (battleMon->statStageParam.EvasionStage < 6) {
        battleMon->statStageParam.EvasionStage = 6;
    }
}
extern "C" void HandlerCuriousMedicine(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if (pokemonSlot == currentSlot) {
        for (u8 i = 0; i < 24; ++i) {
            BattleMon* affectedMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
            if (affectedMon) {

                u32 affectedSlot = BattleMon_GetID(affectedMon);
                if (MainModule_IsAllyMonID(pokemonSlot, affectedSlot)) {

                    u32 affectedPos = Handler_PokeIDToPokePos(serverFlow, affectedSlot);
                    // If it's currently in the battlefield
                    if (affectedPos != NULL_BATTLE_POS &&
                        !BattleMon_IsFainted(affectedMon)) {

                        ResetNegativeStatStages(affectedMon);
                    }
                }
            }
        }
    }

    HandlerParam_Message* message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
    BattleHandler_StrSetup(&message->str, 1u, BATTLE_CURIOUS_MEDICINE_MSGID);
    BattleHandler_PopWork(serverFlow, message);

    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
}
BattleEventHandlerTableEntry CuriousMedicineHandlers[]{
    {EVENT_SWITCH_IN, HandlerCuriousMedicine},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerCuriousMedicine},
};
extern "C" BattleEventHandlerTableEntry* EventAddCuriousMedicine(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(CuriousMedicineHandlers);
    return CuriousMedicineHandlers;
}

// TRANSISTOR
extern "C" void HandlerTransistor(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_ELEC) {
            BattleEventVar_MulValue(VAR_RATIO, 6144);
        }
    }
}
BattleEventHandlerTableEntry TransistorHandlers[]{
    {EVENT_ATTACKER_POWER, HandlerTransistor},
};
extern "C" BattleEventHandlerTableEntry * EventAddTransistor(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(TransistorHandlers);
    return TransistorHandlers;
}

// DRAGON'S MAW
extern "C" void HandlerDragonsMaw(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_DRAGON) {
            BattleEventVar_MulValue(VAR_RATIO, 6144);
        }
    }
}
BattleEventHandlerTableEntry DragonsMawHandlers[]{
    {EVENT_ATTACKER_POWER, HandlerDragonsMaw},
};
extern "C" BattleEventHandlerTableEntry * EventAddDragonsMaw(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(DragonsMawHandlers);
    return DragonsMawHandlers;
}

// When a Pokémon has the ability As One this represents the ability that the pop up should display
// First Ability (Unnerve) -> 1000 0000
// Second Ability (Chilling/Grim Neigh) -> 0100 0000
u8 asOneAbilityPopUpFlags = 0;
extern "C" void NeighBoost(ServerFlow * serverFlow, u32 pokemonSlot, StatStage stat) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        u32 targetCount = BattleEventVar_GetValue(VAR_TARGET_COUNT);
        for (u32 target = 0; target < targetCount; ++target) {

            u32 targetSlot = BattleEventVar_GetValue((BattleEventVar)(VAR_TARGET_MON_ID + target));
            BattleMon* targetMon = Handler_GetBattleMon(serverFlow, targetSlot);
            if (BattleMon_IsFainted(targetMon)) {
                BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
                if (BattleMon_IsStatChangeValid(currentMon, stat, 1)) {

                    HandlerParam_Header* header = BattleHandler_PushWork(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
                    // This lets the handler know what ability should be
                    asOneAbilityPopUpFlags = AS_ONE_SECOND_ABILITY_FLAG;
                    BattleHandler_PopWork(serverFlow, header);

                    HandlerParam_ChangeStatStage* statChange;
                    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    statChange->pokeCount = 1;
                    statChange->pokeID[0] = pokemonSlot;
                    statChange->stat = stat;
                    statChange->volume = 1;
                    BattleHandler_PopWork(serverFlow, statChange);

                    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
                }
            }
        }
    }
}

// CHILLING NEIGH
extern "C" void HandlerChillingNeigh(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    NeighBoost(serverFlow, pokemonSlot, STATSTAGE_ATTACK);
}
BattleEventHandlerTableEntry ChillingNeighHandlers[]{
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerChillingNeigh},
};
extern "C" BattleEventHandlerTableEntry* EventAddChillingNeigh(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ChillingNeighHandlers);
    return ChillingNeighHandlers;
}

// GRIM NEIGH
extern "C" void HandlerGrimNeigh(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    NeighBoost(serverFlow, pokemonSlot, STATSTAGE_SPECIAL_ATTACK);
}
BattleEventHandlerTableEntry GrimNeighHandlers[]{
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerGrimNeigh},
};
extern "C" BattleEventHandlerTableEntry* EventAddGrimNeigh(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(GrimNeighHandlers);
    return GrimNeighHandlers;
}

// AS ONE
extern "C" void HandlerAsOneUnnerve(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (!*work) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, BATTLE_AS_ONE_MSGID);
        BattleHandler_AddArg(&message->str, pokemonSlot);
        BattleHandler_PopWork(serverFlow, message);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);

        HandlerParam_Header* header = BattleHandler_PushWork(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
        // This lets the handler know what ability should be
        asOneAbilityPopUpFlags = AS_ONE_FIRST_ABILITY_FLAG;
        BattleHandler_PopWork(serverFlow, header);

        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 1u, 176u);
        BattleHandler_AddArg(&message->str, GetSideFromMonID(pokemonSlot) == 0);
        BattleHandler_PopWork(serverFlow, message);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);

        BattleEventItem_AttachSkipCheckHandler(item, HandlerUnnerveSkipCheck);
        *work = 1;
    }
}
extern "C" void HandlerAsOneUnnerveSwitchIn(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerAsOneUnnerve(item, serverFlow, pokemonSlot, work);
    }
}
extern "C" void HandlerAsOneNeigh(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        ABILITY ability = BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY);
        if (ability == ABIL266_AS_ONE) {
            NeighBoost(serverFlow, pokemonSlot, STATSTAGE_ATTACK);
        }
        else if (ability == ABIL267_AS_ONE) {
            NeighBoost(serverFlow, pokemonSlot, STATSTAGE_SPECIAL_ATTACK);
        }
    }
}
BattleEventHandlerTableEntry AsOneHandlers[]{
    {EVENT_SWITCH_IN, HandlerAsOneUnnerveSwitchIn},
    {EVENT_CHECK_ACTIVATION, HandlerAsOneUnnerve},  
    {EVENT_SWITCH_IN_PREVIOUS, HandlerAsOneUnnerve},
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerAsOneNeigh},
};
extern "C" BattleEventHandlerTableEntry* EventAddAsOne(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(AsOneHandlers);
    return AsOneHandlers;
}

// SEED SOWER
extern "C" void HandlerSeedSower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        CommonTerrainChangeAbility(serverFlow, pokemonSlot, TERRAIN_GRASSY, BATTLE_GRASSY_TERRAIN_MSGID);
    }
}
BattleEventHandlerTableEntry SeedSowerHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerSeedSower},
};
extern "C" BattleEventHandlerTableEntry * EventAddSeedSower(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SeedSowerHandlers);
    return SeedSowerHandlers;
}

// THERMAL EXCHANGE
extern "C" void HandlerThermalExchange(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) && 
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG) && 
        BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_FIRE) {
        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        statChange->stat = STATSTAGE_ATTACK;
        statChange->volume = 1;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}
BattleEventHandlerTableEntry ThermalExchangeHandlers[]{
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerWaterBubblePreventStatus},
    {EVENT_ADD_CONDITION_FAIL, CommonStatusFailMessageHandler},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerWaterBubbleHealBurn},
    {EVENT_ACTION_PROCESSING_END, HandlerWaterBubbleHealBurn},
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerThermalExchange},
};
extern "C" BattleEventHandlerTableEntry * EventAddThermalExchange(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ThermalExchangeHandlers);
    return ThermalExchangeHandlers;
}

// ANGER SHELL
extern "C" void HandlerAngerShell(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 targetCount = BattleEventVar_GetValue(VAR_TARGET_COUNT);
    for (u8 i = 0; i < targetCount; ++i) {
        u32 targetSlot = BattleEventVar_GetValue((BattleEventVar)(VAR_TARGET_MON_ID + i));
        if (pokemonSlot == targetSlot) {
            BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            u32 maxHP = BattleMon_GetValue(currentMon, VALUE_MAX_HP);

            u32 currentHP = BattleMon_GetValue(currentMon, VALUE_CURRENT_HP);
            u32 currentHPPercent = (currentHP * 100) / maxHP;

            u32 beforeDmgHP = currentHP + BattleEventVar_GetValue(VAR_DAMAGE) - actionSubstituteDamage[pokemonSlot];
            u32 beforeDmgHPPercent = (beforeDmgHP * 100) / maxHP;
#if ANGER_SHELL_DEBUG
            DPRINTF("MAX HP: %d\n", maxHP);
            DPRINTF("CURRENT HP: %d\n", currentHP);
            DPRINTF("DAMAGE: %d\n", BattleEventVar_GetValue(VAR_DAMAGE));
            DPRINTF("SUBSTITUTE DAMAGE: %d\n", actionSubstituteDamage[pokemonSlot]);
            DPRINTF("BEFORE HP: %d\n", beforeDmgHP);
            DPRINTF("BEFORE HP PERCENT: %d\n", beforeDmgHPPercent);
            DPRINTF("CURRENT HP PERCENT: %d\n", currentHPPercent);
#endif
            if (beforeDmgHPPercent >= 50)
            {
                if (currentHPPercent < 50)
                {
                    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                    HandlerParam_ChangeStatStage* statChange;
                    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    statChange->stat = STATSTAGE_DEFENSE;
                    statChange->volume = -1;
                    statChange->pokeCount = 1;
                    statChange->pokeID[0] = pokemonSlot;
                    BattleHandler_PopWork(serverFlow, statChange);
                    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    statChange->stat = STATSTAGE_SPECIAL_DEFENSE;
                    statChange->volume = -1;
                    statChange->pokeCount = 1;
                    statChange->pokeID[0] = pokemonSlot;
                    BattleHandler_PopWork(serverFlow, statChange);
                    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    statChange->stat = STATSTAGE_ATTACK;
                    statChange->volume = 2;
                    statChange->pokeCount = 1;
                    statChange->pokeID[0] = pokemonSlot;
                    BattleHandler_PopWork(serverFlow, statChange);
                    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    statChange->stat = STATSTAGE_SPECIAL_ATTACK;
                    statChange->volume = 2;
                    statChange->pokeCount = 1;
                    statChange->pokeID[0] = pokemonSlot;
                    BattleHandler_PopWork(serverFlow, statChange);
                    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    statChange->stat = STATSTAGE_SPEED;
                    statChange->volume = 2;
                    statChange->pokeCount = 1;
                    statChange->pokeID[0] = pokemonSlot;
                    BattleHandler_PopWork(serverFlow, statChange);

                    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
                }
            }

            actionSubstituteDamage[pokemonSlot] = 0;
        }
    }
}
BattleEventHandlerTableEntry AngerShellHandlers[]{
    {EVENT_DAMAGE_PROCESSING_END_HIT_2, HandlerAngerShell},
};
extern "C" BattleEventHandlerTableEntry * EventAddAngerShell(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(AngerShellHandlers);
    return AngerShellHandlers;
}

// PURIFYING SALT
extern "C" void HandlerPurifyingSaltPreventStatus(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        CONDITION condition = BattleEventVar_GetValue(VAR_CONDITION_ID);
        if (condition == CONDITION_PARALYSIS ||
            condition == CONDITION_SLEEP ||
            condition == CONDITION_FREEZE ||
            condition == CONDITION_BURN ||
            condition == CONDITION_POISON ||
            condition == CONDITION_CONFUSION ||
            condition == CONDITION_YAWN) {
            *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
        }
    }
}
extern "C" void HandlerPurifyingSaltGhostDamage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_GHOST) {
            BattleEventVar_MulValue(VAR_RATIO, 2048);
        }
    }
}
BattleEventHandlerTableEntry PurifyingSaltHandlers[]{
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerPurifyingSaltPreventStatus},
    {EVENT_ADD_CONDITION_FAIL, CommonStatusFailMessageHandler},
    {EVENT_ATTACKER_POWER, HandlerPurifyingSaltGhostDamage},
};
extern "C" BattleEventHandlerTableEntry * EventAddPurifyingSalt(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PurifyingSaltHandlers);
    return PurifyingSaltHandlers;
}

// WELL-BAKED BODY
extern "C" void HandlerWellBakedBody(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (CommonDamageRecoverCheck(serverFlow, pokemonSlot, TYPE_FIRE)) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->stat = STATSTAGE_DEFENSE;
        statChange->volume = 2;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        BattleHandler_PopWork(serverFlow, statChange);
        
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
BattleEventHandlerTableEntry WellBakedBodyHandlers[]{
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerWellBakedBody},
};
extern "C" BattleEventHandlerTableEntry * EventAddWellBakedBody(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(WellBakedBodyHandlers);
    return WellBakedBodyHandlers;
}

// WIND RIDER
MOVE_ID WindMoves[] = {
    MOVE016_GUST,
    MOVE018_WHIRLWIND,
    MOVE059_BLIZZARD,
    MOVE177_AEROBLAST,
    MOVE196_ICY_WIND,
    MOVE239_TWISTER,
    MOVE257_HEAT_WAVE,
    MOVE314_AIR_CUTTER,
    MOVE542_HURRICANE,
#if EXPAND_MOVES
    FAIRY_WIND,
    PETAL_BLIZZARD,
    BLEAKWIND_STORM,
    WILDBOLT_STORM,
    SANDSEAR_STORM,
    SPRINGTIDE_STORM,
#endif
};
extern "C" b32 IsWindMove(MOVE_ID moveID) {
    for (u16 i = 0; i < ARRAY_COUNT(WindMoves); ++i) {
        if (moveID == WindMoves[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" void WindRiderBoost(ServerFlow* serverFlow, u32 pokemonSlot) {
    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

    HandlerParam_ChangeStatStage* statChange;
    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
    statChange->stat = STATSTAGE_ATTACK;
    statChange->volume = 1;
    statChange->pokeCount = 1;
    statChange->pokeID[0] = pokemonSlot;
    BattleHandler_PopWork(serverFlow, statChange);

    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
}
extern "C" void HandlerWindRider(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && pokemonSlot != BattleEventVar_GetValue(VAR_ATTACKING_MON)
        && IsWindMove(moveID)
        && BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1)) {
        WindRiderBoost(serverFlow, pokemonSlot);
    }
}
extern "C" void HandlerWindRiderTailwind(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if ((pokemonSlot == currentSlot || MainModule_IsAllyMonID(pokemonSlot, currentSlot)) &&
        BattleEventVar_GetValue(VAR_SIDE_EFFECT) == SIDEEFF_TAILWIND) {
        WindRiderBoost(serverFlow, pokemonSlot);
    }
}
extern "C" void HandlerWindRiderSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        Handler_IsSideEffectActive(serverFlow, GetSideFromMonID(pokemonSlot), SIDEEFF_TAILWIND)) {
        WindRiderBoost(serverFlow, pokemonSlot);
    }
}
BattleEventHandlerTableEntry WindRiderHandlers[]{
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerWindRider},
    {EVENT_CHECK_SIDE_EFFECT_PARAM, HandlerWindRiderTailwind},
    {EVENT_SWITCH_IN, HandlerWindRiderSwitchIn},
};
extern "C" BattleEventHandlerTableEntry * EventAddWindRider(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(WindRiderHandlers);
    return WindRiderHandlers;
}

// GUARD DOG
extern "C" void HandlerGuardDog(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)
        && BattleEventVar_GetValue(VAR_INTIMIDATE_FLAG)) {
        BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);

        WindRiderBoost(serverFlow, pokemonSlot);
    }
}
extern "C" void HandlerSuctionCups(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work);
BattleEventHandlerTableEntry GuardDogHandlers[]{
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerGuardDog},
    {EVENT_CHECK_FORCE_SWITCH, HandlerSuctionCups},
};
extern "C" BattleEventHandlerTableEntry * EventAddGuardDog(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(GuardDogHandlers);
    return GuardDogHandlers;
}

// ROCKY PAYLOAD
extern "C" void HandlerRockyPayload(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_ROCK) {
            BattleEventVar_MulValue(VAR_RATIO, 6144);
        }
    }
}
BattleEventHandlerTableEntry RockyPayloadHandlers[]{
    {EVENT_ATTACKER_POWER, HandlerRockyPayload},
};
extern "C" BattleEventHandlerTableEntry * EventAddRockyPayload(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(RockyPayloadHandlers);
    return RockyPayloadHandlers;
}

// WIND POWER
extern "C" void CommonChargeHandler(ServerFlow * serverFlow, u32 pokemonSlot, u32* work, MOVE_ID moveID) {
    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

    HandlerParam_Message* message;
    message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
    BattleHandler_StrSetup(&message->str, 1u, BATTLE_WIND_POWER_MSGID);
    BattleHandler_AddArg(&message->str, moveID);
    BattleHandler_AddArg(&message->str, pokemonSlot);
    BattleHandler_PopWork(serverFlow, message);
    
    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    
    *work = 1;
}
extern "C" void CommonChargePowerHandler(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (*work > 0
        && pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)
        && BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_ELEC) {
        BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 8192);
        *work = 0;
    }
}
extern "C" void HandlerWindPower(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && pokemonSlot != BattleEventVar_GetValue(VAR_ATTACKING_MON)
        && IsWindMove(moveID)) {
        CommonChargeHandler(serverFlow, pokemonSlot, work, moveID);
    }
}
extern "C" void HandlerWindPowerTailwind(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if ((pokemonSlot == currentSlot || MainModule_IsAllyMonID(pokemonSlot, currentSlot)) &&
        BattleEventVar_GetValue(VAR_SIDE_EFFECT) == SIDEEFF_TAILWIND) {
        CommonChargeHandler(serverFlow, pokemonSlot, work, MOVE366_TAILWIND);
    }
}
BattleEventHandlerTableEntry WindPowerHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerWindPower},
    {EVENT_CHECK_SIDE_EFFECT_PARAM, HandlerWindPowerTailwind},
    {EVENT_MOVE_POWER, CommonChargePowerHandler},
};
extern "C" BattleEventHandlerTableEntry * EventAddWindPower(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(WindPowerHandlers);
    return WindPowerHandlers;
}

// ZERO TO HERO
extern "C" void HandlerZeroToHero(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_TARGET_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // TODO: ADD PALAFIN
        if (BattleMon_GetSpecies(currentMon) == PK351_CASTFORM &&
            BattleMon_GetValue(currentMon, VALUE_FORM) == 0) {
            HandlerParam_ChangeForm* changeForm;
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = 1;
            BattleHandler_PopWork(serverFlow, changeForm);
        }
    }
}
BattleEventHandlerTableEntry ZeroToHeroHandlers[]{
    {EVENT_SWITCH_OUT_INTERRUPT, HandlerZeroToHero},
};
extern "C" BattleEventHandlerTableEntry * EventAddZeroToHero(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ZeroToHeroHandlers);
    return ZeroToHeroHandlers;
}

//COMMANDER
// work[0] -> the battle slot of the Dondozo
// work[1] -> the index where the Dondozo slot is stored in the "pairedDondozos" array, plus one
extern "C" u32 FindCommanderAlly(ServerFlow* serverFlow, u32 pokemonSlot) {
    for (u8 i = 0; i < 24; ++i) {
        BattleMon* affectedMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
        if (affectedMon) {

            u32 affectedSlot = BattleMon_GetID(affectedMon);
            if (affectedSlot != pokemonSlot &&
                MainModule_IsAllyMonID(pokemonSlot, affectedSlot)) {

                u32 affectedPos = Handler_PokeIDToPokePos(serverFlow, affectedSlot);
                // If it's currently in the battlefield
                // TODO: ADD DONDOZO
                if (affectedPos != NULL_BATTLE_POS &&
                    !BattleMon_IsFainted(affectedMon) &&
                    affectedMon->species == PK061_POLIWHIRL &&
                    // Check that this Dondozo is not already paired
                    !BattleField_IsDondozoPaired(affectedSlot)) {
                    return affectedSlot;
                }
            }
        }
    }

    return BATTLE_MAX_SLOTS;
}
extern "C" void CommanderStart(ServerFlow* serverFlow, u32 pokemonSlot, u32* work, u32 currentSlot) {
    u32 dondozoSlot = BATTLE_MAX_SLOTS;
    if (pokemonSlot != currentSlot) {
        dondozoSlot = currentSlot;
    }
    else {
        dondozoSlot = FindCommanderAlly(serverFlow, pokemonSlot);
    }

    BattleMon* tatsugiriMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    BattleMon* dondozoMon = Handler_GetBattleMon(serverFlow, dondozoSlot);
    // TODO: ADD TATSUGIRI AND DONDOZO
    if (!tatsugiriMon || !dondozoMon ||
        tatsugiriMon->species != PK649_GENESECT ||
        dondozoMon->species != PK061_POLIWHIRL) {
        return;
    }

    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

    // Remove Tatsugiris Substitute
    tatsugiriMon->substituteHP = 0;

    HandlerParam_SetConditionFlag* setConditionFlag;
    // Remove Tatsugiris other hide states if they are set
    CONDITION_FLAG hideFlag = BattleMon_GetHideCondition(tatsugiriMon);
    if (hideFlag != 16) {
        setConditionFlag = (HandlerParam_SetConditionFlag*)BattleHandler_PushWork(serverFlow, EFFECT_RESET_CONDITION_FLAG, pokemonSlot);
        setConditionFlag->pokeID = pokemonSlot;
        setConditionFlag->flag = hideFlag;
        BattleHandler_PopWork(serverFlow, setConditionFlag);
    }

    // Remove Tatsugiris charge move state if it is set
    if (BattleMon_CheckIfMoveCondition(tatsugiriMon, CONDITION_CHARGELOCK)) {
        HandlerParam_CureCondition* cureCondition;
        cureCondition = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
        cureCondition->condition = CONDITION_CHARGELOCK;
        cureCondition->pokeID[0] = pokemonSlot;
        cureCondition->pokeCount = 1;
        cureCondition->msgDisable = 1;
        BattleHandler_PopWork(serverFlow, cureCondition);
    }

    // Make Tatsugiri semi-invulnerable
    setConditionFlag = (HandlerParam_SetConditionFlag*)BattleHandler_PushWork(serverFlow, EFFECT_SET_CONDITION_FLAG, pokemonSlot);
    setConditionFlag->pokeID = pokemonSlot;
    setConditionFlag->flag = CONDITIONFLAG_SHADOWFORCE;
    BattleHandler_PopWork(serverFlow, setConditionFlag);

    ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_Hide, pokemonSlot, 1);

    // Make Tatsugiri skip the action selection
    setConditionFlag = (HandlerParam_SetConditionFlag*)BattleHandler_PushWork(serverFlow, EFFECT_SET_CONDITION_FLAG, pokemonSlot);
    setConditionFlag->pokeID = pokemonSlot;
    setConditionFlag->flag = CONDITIONFLAG_NOACTION;
    BattleHandler_PopWork(serverFlow, setConditionFlag);

    // Make Tatsugiri unable to switch
    HandlerParam_AddCondition* addCondition;
    addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
    addCondition->pokeID = pokemonSlot;
    addCondition->condition = CONDITION_BLOCK;
    addCondition->condData = ConditionData_MakePoke(dondozoSlot);
    BattleHandler_PopWork(serverFlow, addCondition);

    // Apply omni-boost to Dondozo
    for (u32 stat = STATSTAGE_ATTACK; stat <= STATSTAGE_SPEED; ++stat) {
        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->stat = (StatStage)stat;
        statChange->volume = 2;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = dondozoSlot;
        BattleHandler_PopWork(serverFlow, statChange);
    }

    // Make Dondozo unable to switch
    addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
    addCondition->pokeID = dondozoSlot;
    addCondition->condition = CONDITION_BLOCK;
    addCondition->condData = Condition_MakePermanent();
    BattleHandler_PopWork(serverFlow, addCondition);
    // Force switch prevention handled in [ServerEvent_CheckForceSwitch]

    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);

    work[0] = dondozoSlot;
    BattleField_AddPairedDondozo(dondozoSlot, &work[1]);
    ++work[1];

#if COMMANDER_DEBUG
    DPRINTF("TATSU %d \n", pokemonSlot);
    DPRINTF("   HIDE %d \n", BattleMon_GetHideCondition(tatsugiriMon));
    DPRINTF("   CHARGE %d \n", BattleMon_CheckIfMoveCondition(tatsugiriMon, CONDITION_CHARGELOCK));
    DPRINTF("   NO ACT %d \n", BattleMon_GetConditionFlag(tatsugiriMon, CONDITIONFLAG_NOACTION));
    DPRINTF("   DOZO %d \n", dondozoSlot);
    DPRINTF("   PAIR %d \n", work[1]);
#endif
}
extern "C" void HandlerCommanderSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    
    if (work[1] == 0) {
        u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
        if (pokemonSlot == currentSlot || MainModule_IsAllyMonID(pokemonSlot, currentSlot)) {

            CommanderStart(serverFlow, pokemonSlot, work, currentSlot);
        }
    }
}
extern "C" void HandlerCommanderSwitchInStart(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (work[1] == 0) {
        CommanderStart(serverFlow, pokemonSlot, work, pokemonSlot);
    }
}
extern "C" void HandlerCommanderFainted(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (work[1] > 0) {
        u32 pairIndex = work[1] - 1;

        u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
        if (currentSlot == g_BattleField->pairedDondozos[pairIndex]) {

            // Remove Tatsugiris hide state
            HandlerParam_SetConditionFlag* setConditionFlag;
            setConditionFlag = (HandlerParam_SetConditionFlag*)BattleHandler_PushWork(serverFlow, EFFECT_RESET_CONDITION_FLAG, pokemonSlot);
            setConditionFlag->pokeID = pokemonSlot;
            setConditionFlag->flag = CONDITIONFLAG_SHADOWFORCE;
            BattleHandler_PopWork(serverFlow, setConditionFlag);

            // Remove Tatsugiris No Action state
            setConditionFlag = (HandlerParam_SetConditionFlag*)BattleHandler_PushWork(serverFlow, EFFECT_RESET_CONDITION_FLAG, pokemonSlot);
            setConditionFlag->pokeID = pokemonSlot;
            setConditionFlag->flag = CONDITIONFLAG_NOACTION;
            BattleHandler_PopWork(serverFlow, setConditionFlag);

            ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_Hide, pokemonSlot, 0);

            work[0] = BATTLE_MAX_SLOTS;
            work[1] = 0;
        }
    }
}
BattleEventHandlerTableEntry CommanderHandlers[]{
    {EVENT_SWITCH_IN, HandlerCommanderSwitchIn},
    {EVENT_AFTER_LAST_SWITCHIN, HandlerCommanderSwitchInStart},
    {EVENT_NOTIFY_FAINTED, HandlerCommanderFainted},
};
extern "C" BattleEventHandlerTableEntry * EventAddCommander(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(CommanderHandlers);
    return CommanderHandlers;
}

// ELECTROMORPHOSIS
extern "C" void HandlerElectromorphosis(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && pokemonSlot != BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        CommonChargeHandler(serverFlow, pokemonSlot, work, moveID);
    }
}
BattleEventHandlerTableEntry ElectromorphosisHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerElectromorphosis},
    {EVENT_MOVE_POWER, CommonChargePowerHandler},
};
extern "C" BattleEventHandlerTableEntry * EventAddElectromorphosis(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ElectromorphosisHandlers);
    return ElectromorphosisHandlers;
}

// PROTOSYNTHESIS
// work[0] -> the stat it is boosting
// work[1] -> flag that prevents the ability to activate on switch in in the first turn
extern "C" StatStage GetHighestStatWithBoosts(BattleMon * battleMon) {
    u16 highest = BattleMon_GetValue(battleMon, VALUE_ATTACK_STAT);
    StatStage output = STATSTAGE_ATTACK;

    if (u16 defense = BattleMon_GetValue(battleMon, VALUE_DEFENSE_STAT) > highest) {
        highest = defense;
        output = STATSTAGE_DEFENSE;
    }
    if (u16 specialAttack = BattleMon_GetValue(battleMon, VALUE_SPECIAL_ATTACK_STAGE) > highest) {
        highest = specialAttack;
        output = STATSTAGE_SPECIAL_ATTACK;
    }
    if (u16 specialDefense = BattleMon_GetValue(battleMon, VALUE_SPECIAL_DEFENSE_STAGE) > highest) {
        highest = specialDefense;
        output = STATSTAGE_SPECIAL_DEFENSE;
    }
    if (u16 speed = BattleMon_GetValue(battleMon, VALUE_SPEED_STAT) > highest) {
        highest = speed;
        output = STATSTAGE_SPEED;
    }

    return output;
}
extern "C" void CommonParadoxActivationHandler(ServerFlow * serverFlow, u32 pokemonSlot, u32* work, b32 fieldCondition) {
    if (work[0] == STATSTAGE_NULL) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

        if (BattleField_CheckParadoxAbilityFlag(pokemonSlot) || fieldCondition) {
            work[0] = GetHighestStatWithBoosts(currentMon);

            u16 msgID = 0;
            // TODO: ADD CUSTOM MESSAGES, I'M LAZY RN
            switch (work[0])
            {
            case STATSTAGE_ATTACK:
                msgID = 27;
                break;
            case STATSTAGE_DEFENSE:
                msgID = 30;
                break;
            case STATSTAGE_SPECIAL_ATTACK:
                msgID = 33;
                break;
            case STATSTAGE_SPECIAL_DEFENSE:
                msgID = 36;
                break;
            case STATSTAGE_SPEED:
                msgID = 39;
                break;
            }

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            // Activate the Energy Booster message and consumition
            if (BattleField_CheckParadoxAbilityFlag(pokemonSlot)) {
                ServerControl_CheckItemReaction(serverFlow, currentMon, 0);
            }

            HandlerParam_Message* message;
            message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
            BattleHandler_StrSetup(&message->str, 2u, msgID);
            BattleHandler_AddArg(&message->str, pokemonSlot);
            BattleHandler_PopWork(serverFlow, message);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
        else {
            work[0] = STATSTAGE_NULL;
        }
    }
}
extern "C" void CommonRemoveForceParadoxHandler(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleField_ResetParadoxAbilityFlag(pokemonSlot);
    }
}
extern "C" void CommonParadoxAttackHandler(ServerFlow * serverFlow, u32 pokemonSlot, StatStage stat, b32 fieldCondition) {
    if (BattleField_CheckParadoxAbilityFlag(pokemonSlot) || fieldCondition) {

        if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
            (stat == STATSTAGE_ATTACK || stat == STATSTAGE_SPECIAL_ATTACK)) {

            MoveCategory category = CATEGORY_PHYSICAL;
            if (stat == STATSTAGE_SPECIAL_ATTACK) {
                MoveCategory category = CATEGORY_SPECIAL;
            }
            if (category == BattleEventVar_GetValue(VAR_MOVE_CATEGORY)) {
                BattleEventVar_MulValue(VAR_RATIO, 5324);
            }
        }
    }
}
extern "C" void CommonParadoxDefenseHandler(ServerFlow * serverFlow, u32 pokemonSlot, StatStage stat, b32 fieldCondition) {
    if (BattleField_CheckParadoxAbilityFlag(pokemonSlot) || fieldCondition) {

        if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
            (stat == STATSTAGE_DEFENSE || stat == STATSTAGE_SPECIAL_DEFENSE)) {
            MoveCategory category = CATEGORY_PHYSICAL;
            if (stat == STATSTAGE_SPECIAL_DEFENSE) {
                MoveCategory category = CATEGORY_SPECIAL;
            }

            if (category == BattleEventVar_GetValue(VAR_MOVE_CATEGORY)) {
                BattleEventVar_MulValue(VAR_RATIO, 5324);
            }
        }
    }
}
extern "C" void CommonParadoxSpeedHandler(ServerFlow * serverFlow, u32 pokemonSlot, StatStage stat, b32 fieldCondition) {
    if (BattleField_CheckParadoxAbilityFlag(pokemonSlot) || fieldCondition) {

        if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
            stat == STATSTAGE_SPEED) {
            BattleEventVar_MulValue(VAR_RATIO, 6144);
        }
    }
}
extern "C" void HandlerProtosynthesisAfterLastSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    // Check the weather directly to avoid Cloud Nine or Air Lock
    CommonParadoxActivationHandler(serverFlow, pokemonSlot, work, BattleField_GetWeatherCore(g_BattleField) == WEATHER_SUN);
    work[1] = 1;
}
extern "C" void HandlerProtosynthesisSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (work[1] && pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        // Check the weather directly to avoid Cloud Nine or Air Lock
        CommonParadoxActivationHandler(serverFlow, pokemonSlot, work, BattleField_GetWeatherCore(g_BattleField) == WEATHER_SUN);
    }
}
extern "C" void HandlerProtosynthesisAttack(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonParadoxAttackHandler(serverFlow, pokemonSlot, (StatStage)work[0], ServerEvent_GetWeather(serverFlow) == WEATHER_SUN);
}
extern "C" void HandlerProtosynthesisDefense(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonParadoxDefenseHandler(serverFlow, pokemonSlot, (StatStage)work[0], ServerEvent_GetWeather(serverFlow) == WEATHER_SUN);
}
extern "C" void HandlerProtosynthesisSpeed(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonParadoxSpeedHandler(serverFlow, pokemonSlot, (StatStage)work[0], ServerEvent_GetWeather(serverFlow) == WEATHER_SUN);
}
BattleEventHandlerTableEntry ProtosynthesisHandlers[]{
    {EVENT_AFTER_LAST_SWITCHIN, HandlerProtosynthesisAfterLastSwitchIn},
    {EVENT_AFTER_WEATHER_CHANGE, HandlerProtosynthesisAfterLastSwitchIn},
    {EVENT_SWITCH_IN, HandlerProtosynthesisSwitchIn},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerProtosynthesisSwitchIn},
    {EVENT_SWITCH_OUT_END, CommonRemoveForceParadoxHandler},
    {EVENT_ATTACKER_POWER, HandlerProtosynthesisAttack},
    {EVENT_DEFENDER_GUARD, HandlerProtosynthesisDefense},
    {EVENT_CALC_SPEED, HandlerProtosynthesisSpeed},
};
extern "C" BattleEventHandlerTableEntry* EventAddProtosynthesis(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ProtosynthesisHandlers);
    return ProtosynthesisHandlers;
}

// QUARK DRIVE QuarkDrive
// work[0] -> the stat it is boosting
// work[1] -> flag that prevents the ability to activate on switch in in the first turn
extern "C" void HandlerQuarkDriveAfterLastSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    // Check the terrain directly to avoid Teraform Zero
    CommonParadoxActivationHandler(serverFlow, pokemonSlot, work, BattleField_GetTerrain(g_BattleField) == TERRAIN_ELECTRIC);
    work[1] = 1;
}
extern "C" void HandlerQuarkDriveTerrainChange(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;
    
    HandlerQuarkDriveAfterLastSwitchIn(item, serverFlow, pokemonSlot, work);
}
extern "C" void HandlerQuarkDriveSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (work[1] && pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        // Check the terrain directly to avoid Teraform Zero
        CommonParadoxActivationHandler(serverFlow, pokemonSlot, work, BattleField_GetTerrain(g_BattleField) == TERRAIN_ELECTRIC);
    }
}
extern "C" void HandlerQuarkDriveAttack(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonParadoxAttackHandler(serverFlow, pokemonSlot, (StatStage)work[0], ServerEvent_GetTerrain(serverFlow) == TERRAIN_ELECTRIC);
}
extern "C" void HandlerQuarkDriveDefense(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonParadoxDefenseHandler(serverFlow, pokemonSlot, (StatStage)work[0], ServerEvent_GetTerrain(serverFlow) == TERRAIN_ELECTRIC);
}
extern "C" void HandlerQuarkDriveSpeed(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonParadoxSpeedHandler(serverFlow, pokemonSlot, (StatStage)work[0], ServerEvent_GetTerrain(serverFlow) == TERRAIN_ELECTRIC);
}
BattleEventHandlerTableEntry QuarkDriveHandlers[]{
    {EVENT_AFTER_LAST_SWITCHIN, HandlerQuarkDriveAfterLastSwitchIn},
    {EVENT_AFTER_TERRAIN_CHANGE, HandlerQuarkDriveTerrainChange},
    {EVENT_SWITCH_IN, HandlerQuarkDriveSwitchIn},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerQuarkDriveSwitchIn},
    {EVENT_SWITCH_OUT_END, CommonRemoveForceParadoxHandler},
    {EVENT_ATTACKER_POWER, HandlerQuarkDriveAttack},
    {EVENT_DEFENDER_GUARD, HandlerQuarkDriveDefense},
    {EVENT_CALC_SPEED, HandlerQuarkDriveSpeed},
};
extern "C" BattleEventHandlerTableEntry * EventAddQuarkDrive(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(QuarkDriveHandlers);
    return QuarkDriveHandlers;
}

// GOOD AS GOLD
extern "C" void HandlerGoodAsGold(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot != BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
        pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (PML_MoveGetCategory(moveID) == CATEGORY_STATUS) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}
BattleEventHandlerTableEntry GoodAsGoldHandlers[]{
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerGoodAsGold},
};
extern "C" BattleEventHandlerTableEntry * EventAddGoodAsGold(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(GoodAsGoldHandlers);
    return GoodAsGoldHandlers;
}

// VESSEL OF RUIN
extern "C" void HandlerVesselOfRuinAdd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonAddFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_VESSEL_OF_RUIN, BATTLE_VESSEL_OF_RUIN_MSGID);
}
extern "C" void HandlerVesselOfRuinRemove(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonRemoveFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_VESSEL_OF_RUIN);
}
BattleEventHandlerTableEntry VesselOfRuinHandlers[]{
    {EVENT_SWITCH_IN, HandlerVesselOfRuinAdd}, // Add fieldeffect when switching in
    {EVENT_AFTER_ABILITY_CHANGE, HandlerVesselOfRuinAdd}, // Add fieldeffect/dependpoke when changing ability and when ability nullify ends
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerVesselOfRuinRemove}, // Remove dependpoke when changing ability
    {EVENT_ABILITY_NULLIFIED, HandlerVesselOfRuinRemove}, // Remove dependpoke when ability is nullified
};
extern "C" BattleEventHandlerTableEntry * EventAddVesselOfRuin(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(VesselOfRuinHandlers);
    return VesselOfRuinHandlers;
}

// SWORD OF RUIN
extern "C" void HandlerSwordOfRuinAdd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonAddFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_SWORD_OF_RUIN, BATTLE_SWORD_OF_RUIN_MSGID);
}
extern "C" void HandlerSwordOfRuinRemove(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonRemoveFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_SWORD_OF_RUIN);
}
BattleEventHandlerTableEntry SwordOfRuinHandlers[]{
    {EVENT_SWITCH_IN, HandlerSwordOfRuinAdd}, // Add fieldeffect when switching in
    {EVENT_AFTER_ABILITY_CHANGE, HandlerSwordOfRuinAdd}, // Add fieldeffect/dependpoke when changing ability and when ability nullify ends
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerSwordOfRuinRemove}, // Remove dependpoke when changing ability
    {EVENT_ABILITY_NULLIFIED, HandlerSwordOfRuinRemove}, // Remove dependpoke when ability is nullified
};
extern "C" BattleEventHandlerTableEntry * EventAddSwordOfRuin(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SwordOfRuinHandlers);
    return SwordOfRuinHandlers;
}

// TABLETS OF RUIN
extern "C" void HandlerTabletsOfRuinAdd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonAddFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_TABLETS_OF_RUIN, BATTLE_TABLETS_OF_RUIN_MSGID);
}
extern "C" void HandlerTabletsOfRuinRemove(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonRemoveFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_TABLETS_OF_RUIN);
}
BattleEventHandlerTableEntry TabletsOfRuinHandlers[]{
    {EVENT_SWITCH_IN, HandlerTabletsOfRuinAdd}, // Add fieldeffect when switching in
    {EVENT_AFTER_ABILITY_CHANGE, HandlerTabletsOfRuinAdd}, // Add fieldeffect/dependpoke when changing ability and when ability nullify ends
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerTabletsOfRuinRemove}, // Remove dependpoke when changing ability
    {EVENT_ABILITY_NULLIFIED, HandlerTabletsOfRuinRemove}, // Remove dependpoke when ability is nullified
};
extern "C" BattleEventHandlerTableEntry * EventAddTabletsOfRuin(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(TabletsOfRuinHandlers);
    return TabletsOfRuinHandlers;
}

// BEADS OF RUIN
extern "C" void HandlerBeadsOfRuinAdd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonAddFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_BEADS_OF_RUIN, BATTLE_BEADS_OF_RUIN_MSGID);
}
extern "C" void HandlerBeadsOfRuinRemove(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonRemoveFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_BEADS_OF_RUIN);
}
BattleEventHandlerTableEntry BeadsOfRuinHandlers[]{
    {EVENT_SWITCH_IN, HandlerBeadsOfRuinAdd}, // Add fieldeffect when switching in
    {EVENT_AFTER_ABILITY_CHANGE, HandlerBeadsOfRuinAdd}, // Add fieldeffect/dependpoke when changing ability and when ability nullify ends
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerBeadsOfRuinRemove}, // Remove dependpoke when changing ability
    {EVENT_ABILITY_NULLIFIED, HandlerBeadsOfRuinRemove}, // Remove dependpoke when ability is nullified
};
extern "C" BattleEventHandlerTableEntry * EventAddBeadsOfRuin(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(BeadsOfRuinHandlers);
    return BeadsOfRuinHandlers;
}

// ORICHALCUM PULSE
extern "C" void HandlerOrichalcumPulseSun(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_ChangeWeather* changeWeather;
        changeWeather = (HandlerParam_ChangeWeather*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_WEATHER, pokemonSlot);
        changeWeather->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        changeWeather->weather = WEATHER_SUN;
        changeWeather->turn = WEATHER_ABILITY_TURNS;
        BattleHandler_PopWork(serverFlow, changeWeather);

        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, BATTLE_ORICHALCUM_PULSE_MSGID);
        BattleHandler_AddArg(&message->str, pokemonSlot);
        BattleHandler_PopWork(serverFlow, message);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
extern "C" void HandlerOrichalcumPulseAttack(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
        BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_PHYSICAL &&
        ServerEvent_GetWeather(serverFlow) == WEATHER_SUN) {
        
        BattleEventVar_MulValue(VAR_RATIO, 5461);
    }
}
BattleEventHandlerTableEntry OrichalcumPulseHandlers[]{
    {EVENT_SWITCH_IN, HandlerOrichalcumPulseSun},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerOrichalcumPulseSun},
    {EVENT_ATTACKER_POWER, HandlerOrichalcumPulseAttack},
};
extern "C" BattleEventHandlerTableEntry * EventAddOrichalcumPulse(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(OrichalcumPulseHandlers);
    return OrichalcumPulseHandlers;
}

// HADRON ENGINE
extern "C" void HandlerHadronEngineTerrain(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
        
        HandlerParam_AddFieldEffect* addFieldEffect;
        addFieldEffect = (HandlerParam_AddFieldEffect*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_FIELD_EFFECT, pokemonSlot);
        addFieldEffect->effect = FLDEFF_TERRAIN;
        addFieldEffect->field_D = TERRAIN_ELECTRIC;
        addFieldEffect->condData = Condition_MakeTurn(TERRAIN_ABILITY_TURNS);
        BattleHandler_StrSetup(&addFieldEffect->exStr, 2u, BATTLE_HADRON_ENGINE_MSGID);
        BattleHandler_AddArg(&addFieldEffect->exStr, pokemonSlot);
        BattleHandler_PopWork(serverFlow, addFieldEffect);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
extern "C" void HandlerHadronEngineAttack(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
        BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_SPECIAL &&
        ServerEvent_GetTerrain(serverFlow) == TERRAIN_ELECTRIC) {

        BattleEventVar_MulValue(VAR_RATIO, 5461);
    }
}
BattleEventHandlerTableEntry HadronEngineHandlers[]{
    {EVENT_SWITCH_IN, HandlerHadronEngineTerrain},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerHadronEngineTerrain},
    {EVENT_ATTACKER_POWER, HandlerHadronEngineAttack},
};
extern "C" BattleEventHandlerTableEntry * EventAddHadronEngine(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(HadronEngineHandlers);
    return HadronEngineHandlers;
}

// OPPORTUNIST
// work[0] -> flag to apply the stat change
extern "C" void HandlerOpportunistCheck(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    int volume = BattleEventVar_GetValue(VAR_VOLUME);
    if (!MainModule_IsAllyMonID(pokemonSlot, BattleEventVar_GetValue(VAR_MON_ID))
        && volume > 0
        && !BattleEventVar_GetValue(VAR_OPPORTUNIST_FLAG)) {

        work[0] = 1;
    }
}
extern "C" void HandlerOpportunist(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (work[0]) {
        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        statChange->stat = (StatStage)BattleEventVar_GetValue(VAR_MOVE_EFFECT);
        statChange->volume = BattleEventVar_GetValue(VAR_VOLUME);
        statChange->pad = STAT_CHANGE_OPPORTUNIST_FLAG;
        BattleHandler_PopWork(serverFlow, statChange);

        work[0] = 0;
    }
    
}
BattleEventHandlerTableEntry OpportunistHandlers[]{
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerOpportunistCheck},
    {EVENT_STAT_STAGE_CHANGE_APPLIED, HandlerOpportunist},
};
extern "C" BattleEventHandlerTableEntry * EventAddOpportunist(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(OpportunistHandlers);
    return OpportunistHandlers;
}

// CUD CHEW
// work[0] -> berry that has been consumed
// work[1] -> turn count until consuming the berry again
extern "C" void HandlerCudChew(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        ITEM_ID itemID = BattleEventVar_GetValue(VAR_ITEM);
        if (itemID && PML_ItemIsBerry(itemID)) {
    
            work[0] = itemID;
            work[1] = 0;
        }
    }
}
extern "C" void HandlerCudChewConsume(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (work[0] && pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        ++work[1];

        if (work[1] == 2) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            HandlerParam_ActivateItem* forceUseItem;
            forceUseItem = (HandlerParam_ActivateItem*)BattleHandler_PushWork(serverFlow, EFFECT_FORCE_USE_HELD_ITEM, pokemonSlot);
            forceUseItem->header.flags |= 0x1000000u;
            forceUseItem->itemID = work[0];
            forceUseItem->pokeID = pokemonSlot;
            BattleHandler_PopWork(serverFlow, forceUseItem);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);

            work[0] = 0;
            work[1] = 0;
        }
    }
}
BattleEventHandlerTableEntry CudChewHandlers[]{
    {EVENT_CONSUME_ITEM, HandlerCudChew},
    {EVENT_USE_TEMP_ITEM_AFTER, HandlerCudChew},
    {EVENT_TURN_CHECK_END, HandlerCudChewConsume},
};
extern "C" BattleEventHandlerTableEntry * EventAddCudChew(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(CudChewHandlers);
    return CudChewHandlers;
}

// SHARPNESS
MOVE_ID sharpMoves[]{
    MOVE015_CUT,
    MOVE075_RAZOR_LEAF,
    MOVE163_SLASH,
    MOVE210_FURY_CUTTER,
    MOVE314_AIR_CUTTER,
    MOVE332_AERIAL_ACE,
    MOVE348_LEAF_BLADE,
    MOVE400_NIGHT_SLASH,
    MOVE403_AIR_SLASH,
    MOVE404_X_SCISSOR,
    MOVE427_PSYCHO_CUT,
    MOVE440_CROSS_POISON,
    MOVE533_SACRED_SWORD,
    MOVE534_RAZOR_SHELL,
    MOVE548_SECRET_SWORD,
#if EXPAND_MOVES
    AQUA_CUTTER,
    CEASELESS_EDGE,
    SOLAR_BLADE,
    STONE_AXE,
    BEHEMOTH_BLADE,
    BITTER_BLADE,
    KOWTOW_CLEAVE,
    MIGHTY_CLEAVE,
    POPULATION_BOMB,
    PSYBLADE,
    TACHYON_CUTTER,
#endif
};
extern "C" b32 IsSharpMove(MOVE_ID moveID) {
    for (u16 i = 0; i < ARRAY_COUNT(sharpMoves); ++i) {
        if (moveID == sharpMoves[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" void HandlerSharpness(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (IsSharpMove(moveID)) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 6144);
        }
    }
}
BattleEventHandlerTableEntry SharpnessHandlers[]{
    {EVENT_MOVE_POWER, HandlerSharpness},
};
extern "C" BattleEventHandlerTableEntry * EventAddSharpness(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(SharpnessHandlers);
    return SharpnessHandlers;
}

// SUPREME OVERLORD
// work[0] -> the amount to boost power
extern "C" u32 GetFaintedAlliesCount(ServerFlow * serverFlow, u32 pokemonSlot) {
    u32 count = 0;
    for (u8 i = 0; i < 24; ++i) {
        BattleMon* affectedMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
        if (affectedMon) {

            u32 affectedSlot = BattleMon_GetID(affectedMon);
            if (affectedSlot != pokemonSlot && MainModule_IsAllyMonID(pokemonSlot, affectedSlot)) {

                if (BattleMon_IsFainted(affectedMon)) {
                    // Add one to the count for each fainted ally
                    ++count;
                }
            }
        }
    }
    return count;
}
extern "C" void HandlerSupremeOverlordSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        u32 faintedAllies = GetFaintedAlliesCount(serverFlow, pokemonSlot);
        u32 ratio = 0;
        for (u32 i = 0; i < faintedAllies; ++i) {
            ratio += 409;
        }
        work[0] = ratio;
    }
}
extern "C" void HandlerSupremeOverlordPower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) && work[0]) {
        BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 4096 + work[0]);
    }
}
BattleEventHandlerTableEntry SupremeOverlordHandlers[]{
    {EVENT_SWITCH_IN, HandlerSupremeOverlordSwitchIn},
    {EVENT_MOVE_POWER, HandlerSupremeOverlordPower},
};
extern "C" BattleEventHandlerTableEntry * EventAddSupremeOverlord(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(SupremeOverlordHandlers);
    return SupremeOverlordHandlers;
}

// COSTAR
extern "C" void GetStatChanges(s8* output, StatStageParam* statChanges) {
    output[STATSTAGE_ATTACK - 1] = statChanges->AttackStage - 6;
    output[STATSTAGE_DEFENSE - 1] = statChanges->DefenseStage - 6;
    output[STATSTAGE_SPECIAL_ATTACK - 1] = statChanges->SpAttackStage - 6;
    output[STATSTAGE_SPECIAL_DEFENSE - 1] = statChanges->SpDefenseStage - 6;
    output[STATSTAGE_SPEED - 1] = statChanges->SpeedStage - 6;
    output[STATSTAGE_ACCURACY - 1] = statChanges->AccuracyStage - 6;
    output[STATSTAGE_EVASION - 1] = statChanges->EvasionStage - 6;
}
extern "C" b32 HasStatChanges(s8 * statChanges) {
    for (u32 stat = STATSTAGE_ATTACK; stat <= STATSTAGE_EVASION; ++stat) {
        if (statChanges[stat - 1] != 6) {
            return 1;
        }
    }
    return 0;
}
extern "C" void HandlerCostar(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        u32 allyPos = CommonGetAllyPos(serverFlow, Handler_PokeIDToPokePos(serverFlow, pokemonSlot));
        if (allyPos == NULL_BATTLE_POS) {
            return;
        }

        u32 allySlot = Handler_PokePosToPokeID(serverFlow, allyPos);
        BattleMon* allyMon = Handler_GetBattleMon(serverFlow, allySlot);
        s8 statChanges[7];
        GetStatChanges(statChanges, &allyMon->statStageParam);

        b32 hasFocusEnergy = BattleMon_GetConditionFlag(allyMon, CONDITIONFLAG_FOCUSENERGY);
        if (HasStatChanges(statChanges) || hasFocusEnergy) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            for (u32 stat = STATSTAGE_ATTACK; stat <= STATSTAGE_EVASION; ++stat) {
                s8 volume = statChanges[stat - 1];
                if (volume != 0) {
                    HandlerParam_ChangeStatStage* statChange;
                    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    statChange->pokeCount = 1;
                    statChange->pokeID[0] = pokemonSlot;
                    statChange->stat = (StatStage)stat;
                    statChange->volume = volume;
                    BattleHandler_PopWork(serverFlow, statChange);
                }
            }

            if (hasFocusEnergy) {
                HandlerParam_SetConditionFlag* setConditionFlag;
                setConditionFlag = (HandlerParam_SetConditionFlag*)BattleHandler_PushWork(serverFlow, EFFECT_SET_CONDITION_FLAG, pokemonSlot);
                setConditionFlag->pokeID = pokemonSlot;
                setConditionFlag->flag = CONDITIONFLAG_FOCUSENERGY;
                BattleHandler_PopWork(serverFlow, setConditionFlag);

                HandlerParam_Message* message;
                message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
                BattleHandler_StrSetup(&message->str, 2u, 1041u);
                BattleHandler_AddArg(&message->str, pokemonSlot);
                BattleHandler_PopWork(serverFlow, message);
            }

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}
BattleEventHandlerTableEntry CostarHandlers[]{
    {EVENT_SWITCH_IN, HandlerCostar},
};
extern "C" BattleEventHandlerTableEntry * EventAddCostar(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(CostarHandlers);
    return CostarHandlers;
}

// TOXIC DEBRIS
extern "C" void CommonCreateSideEffect(BattleEventItem* item, ServerFlow* serverFlow, u32 currentSlot, u32* work, u8 opposingSide, SideEffect effect, ConditionData condData, u16 msgID);
extern "C" void HandlerToxicDebris(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG) &&
        BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_PHYSICAL) {
        ConditionData condData = Condition_MakePermanent();
        u8 opposingSide = GetSideFromOpposingMonID(pokemonSlot);
        
        HandlerParam_AddSideEffect* addSideEffect;
        addSideEffect = (HandlerParam_AddSideEffect*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_SIDE_EFFECT, pokemonSlot);
        addSideEffect->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        addSideEffect->sideEffect = SIDEEFF_TOXIC_SPIKES;
        addSideEffect->side = opposingSide;
        addSideEffect->condData = condData;
        BattleHandler_StrSetup(&addSideEffect->exStr, 1u, 152u);
        BattleHandler_AddArg(&addSideEffect->exStr, opposingSide);
        BattleHandler_PopWork(serverFlow, addSideEffect);
    }
}
BattleEventHandlerTableEntry ToxicDebrisHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerToxicDebris},
};
extern "C" BattleEventHandlerTableEntry * EventAddToxicDebris(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ToxicDebrisHandlers);
    return ToxicDebrisHandlers;
}

// EARTH EATER
extern "C" void HandlerEarthEater(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (CommonDamageRecoverCheck(serverFlow, pokemonSlot, TYPE_GROUND)) {
        CommonTypeRecoverHP(serverFlow, pokemonSlot, 4u);
    }
}
BattleEventHandlerTableEntry EarthEaterHandlers[]{
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerEarthEater},
};
extern "C" BattleEventHandlerTableEntry * EventAddEarthEater(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(EarthEaterHandlers);
    return EarthEaterHandlers;
}

// MYCELIUM MIGHT 
extern "C" void HandlerMoldBreakerStart(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work);
extern "C" void HandlerMoldBreakerEnd(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work);
extern "C" void HandlerMyceliumMightPriority(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (PML_MoveGetCategory(moveID) == CATEGORY_STATUS) {
            BattleEventVar_RewriteValue(VAR_PRIORITY, 0);
        }
    }
}
extern "C" void HandlerMyceliumMightMoldBreaker(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) && !work[0]) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (PML_MoveGetCategory(moveID) == CATEGORY_STATUS) {
            HandlerMoldBreakerStart(item, serverFlow, pokemonSlot, work);
        }
    }
}
BattleEventHandlerTableEntry MyceliumMightHandlers[]{
    {EVENT_CHECK_SPECIAL_PRIORITY, HandlerMyceliumMightPriority},
    {EVENT_MOVE_SEQUENCE_START, HandlerMyceliumMightMoldBreaker},
    {EVENT_MOVE_SEQUENCE_END, HandlerMoldBreakerEnd},
};
extern "C" BattleEventHandlerTableEntry * EventAddMyceliumMight(u32 * handlerAmount) {
    *handlerAmount = GetNumHandlersWithPriority(EVENTPRI_ABILITY_STALL, ARRAY_COUNT(MyceliumMightHandlers));
    return MyceliumMightHandlers;
}

// HOSPITALITY
extern "C" void HandlerHospitality(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        u32 allyPos = CommonGetAllyPos(serverFlow, Handler_PokeIDToPokePos(serverFlow, pokemonSlot));
        if (allyPos == NULL_BATTLE_POS) {
            return;
        }

        u32 allySlot = Handler_PokePosToPokeID(serverFlow, allyPos);
        BattleMon* allyMon = Handler_GetBattleMon(serverFlow, allySlot);

        if (!BattleMon_CheckIfMoveCondition(allyMon, CONDITION_HEALBLOCK) &&
            !BattleMon_IsFullHP(allyMon)) {
            HandlerParam_RecoverHP* recoveHP;
            recoveHP = (HandlerParam_RecoverHP*)BattleHandler_PushWork(serverFlow, EFFECT_RECOVER_HP, pokemonSlot);
            recoveHP->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            recoveHP->pokeID = allySlot;
            recoveHP->recoverHP = DivideMaxHPZeroCheck(allyMon, 4u);
            BattleHandler_StrSetup(&recoveHP->exStr, 2u, 387u);
            BattleHandler_AddArg(&recoveHP->exStr, allySlot);
            BattleHandler_PopWork(serverFlow, recoveHP);
        }
    }
}
BattleEventHandlerTableEntry HospitalityHandlers[]{
    {EVENT_SWITCH_IN, HandlerHospitality},
};
extern "C" BattleEventHandlerTableEntry * EventAddHospitality(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(HospitalityHandlers);
    return HospitalityHandlers;
}

// MIND'S EYE
extern "C" void CommonIgnoreEvasionHandler(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        BattleEventVar_RewriteValue(VAR_ACCURACY_STAGE, 6);
    }
}
extern "C" void HandlerScrappy(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerKeenEyeCheck(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerKeenEyeGuard(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
BattleEventHandlerTableEntry MindsEyeHandlers[]{
    {EVENT_CHECK_TYPE_EFFECTIVENESS, HandlerScrappy},
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerKeenEyeCheck},
    {EVENT_STAT_STAGE_CHANGE_FAIL, HandlerKeenEyeGuard},
    {EVENT_MOVE_ACCURACY_STAGE, CommonIgnoreEvasionHandler},
};
extern "C" BattleEventHandlerTableEntry * EventAddMindsEye(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MindsEyeHandlers);
    return MindsEyeHandlers;
}

// EMBODY ASPECT TEAL
extern "C" void CommonEmbodyAspectHandler(ServerFlow * serverFlow, u32 pokemonSlot, StatStage stat) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        
        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        statChange->stat = stat;
        statChange->volume = 1;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}
extern "C" void HandlerEmbodyAspectTeal(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonEmbodyAspectHandler(serverFlow, pokemonSlot, STATSTAGE_SPEED);
}
BattleEventHandlerTableEntry EmbodyAspectTealHandlers[]{
    {EVENT_AFTER_ABILITY_CHANGE, HandlerEmbodyAspectTeal},
};
extern "C" BattleEventHandlerTableEntry * EventAddEmbodyAspectTeal(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(EmbodyAspectTealHandlers);
    return EmbodyAspectTealHandlers;
}

// EMBODY ASPECT HEARTHFLAME
extern "C" void HandlerEmbodyAspectHearthflame(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonEmbodyAspectHandler(serverFlow, pokemonSlot, STATSTAGE_ATTACK);
}
BattleEventHandlerTableEntry EmbodyAspectHearthflameHandlers[]{
    {EVENT_AFTER_ABILITY_CHANGE, HandlerEmbodyAspectHearthflame},
};
extern "C" BattleEventHandlerTableEntry * EventAddEmbodyAspectHearthflame(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(EmbodyAspectHearthflameHandlers);
    return EmbodyAspectHearthflameHandlers;
}

// EMBODY ASPECT WELLSPRING
extern "C" void HandlerEmbodyAspectWellspring(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonEmbodyAspectHandler(serverFlow, pokemonSlot, STATSTAGE_SPECIAL_DEFENSE);
}
BattleEventHandlerTableEntry EmbodyAspectWellspringHandlers[]{
    {EVENT_AFTER_ABILITY_CHANGE, HandlerEmbodyAspectWellspring},
};
extern "C" BattleEventHandlerTableEntry * EventAddEmbodyAspectWellspring(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(EmbodyAspectWellspringHandlers);
    return EmbodyAspectWellspringHandlers;
}

// EMBODY ASPECT CORNERSTONE
extern "C" void HandlerEmbodyAspectCornerstorne(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonEmbodyAspectHandler(serverFlow, pokemonSlot, STATSTAGE_DEFENSE);
}
BattleEventHandlerTableEntry EmbodyAspectCornerstorneHandlers[]{
    {EVENT_AFTER_ABILITY_CHANGE, HandlerEmbodyAspectCornerstorne},
};
extern "C" BattleEventHandlerTableEntry * EventAddEmbodyAspectCornerstorne(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(EmbodyAspectCornerstorneHandlers);
    return EmbodyAspectCornerstorneHandlers;
}

// TOXIC CHAIN
extern "C" void HandlerToxicChain(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && !BattleEventVar_GetValue(VAR_SHIELD_DUST_FLAG)) {

        if (AbilityEvent_RollEffectChance(serverFlow, 30u)) {
            HandlerParam_AddCondition* addCondition;
            addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
            addCondition->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            addCondition->pokeID = BattleEventVar_GetValue(VAR_DEFENDING_MON);
            addCondition->condition = CONDITION_POISON;
            addCondition->condData = Condition_MakeBadlyPoisoned();
            BattleHandler_PopWork(serverFlow, addCondition);
        }
    }
}
BattleEventHandlerTableEntry ToxicChainHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerToxicChain},
};
extern "C" BattleEventHandlerTableEntry * EventAddToxicChain(u32 * handlerAmount) {
    *handlerAmount = GetNumHandlersWithPriority(EVENTPRI_ABILITY_POISON_TOUCH, ARRAY_COUNT(ToxicChainHandlers));
    return ToxicChainHandlers;
}

// SUPERSWEET SYRUP 
extern "C" void HandlerSupersweetSyrup(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) && 
        !BattleField_CheckSupersweetSyrupFlag(pokemonSlot)) {
        BattleField_SetSupersweetSyrupFlag(pokemonSlot);

        u16 frontPokeExist = Handler_GetExistFrontPokePos(serverFlow, pokemonSlot);
        u8* frontSlots = Handler_GetTempWork(serverFlow);
        u32 frontCount = Handler_ExpandPokeID(serverFlow, frontPokeExist | 0x100, frontSlots);
        if (frontCount) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            HandlerParam_ChangeStatStage* statChange;
            statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
            statChange->stat = STATSTAGE_EVASION;
            statChange->volume = -1;
            statChange->moveAnimation = 1;
            statChange->pokeCount = frontCount;
            for (u8 frontCurrent = 0; frontCurrent < frontCount; ++frontCurrent) {
                statChange->pokeID[frontCurrent] = frontSlots[frontCurrent];
            }
            BattleHandler_PopWork(serverFlow, statChange);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}
BattleEventHandlerTableEntry SupersweetSyrupHandlers[]{
    {EVENT_SWITCH_IN, HandlerSupersweetSyrup},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerSupersweetSyrup},
};
extern "C" BattleEventHandlerTableEntry * EventAddSupersweetSyrup(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SupersweetSyrupHandlers);
    return SupersweetSyrupHandlers;
}

// TERA SHIFT
extern "C" void HandlerTeraShift(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // TODO: ADD TERAPAGOS
        if (BattleMon_GetSpecies(currentMon) == PK351_CASTFORM &&
            BattleMon_GetValue(currentMon, VALUE_FORM) == 0) {
            HandlerParam_ChangeForm* changeForm;
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = 1;
            BattleHandler_PopWork(serverFlow, changeForm);
        }
    }
}
BattleEventHandlerTableEntry TeraShiftHandlers[]{
    {EVENT_SWITCH_IN, HandlerTeraShift},
};
extern "C" BattleEventHandlerTableEntry * EventAddTeraShift(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(TeraShiftHandlers);
    return TeraShiftHandlers;
}

// TERA SHELL
extern "C" void HandlerTeraShell(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsFullHP(currentMon)) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            BattleEventVar_RewriteValue(VAR_SET_TYPE_EFFECTIVENESS, 2);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}
BattleEventHandlerTableEntry TeraShellHandlers[]{
    {EVENT_CHECK_TYPE_EFFECTIVENESS, HandlerTeraShell},
};
extern "C" BattleEventHandlerTableEntry * EventAddTeraShell(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(TeraShellHandlers);
    return TeraShellHandlers;
}

// TERAFORM ZERO
extern "C" void HandlerTeraformZero(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_ChangeWeather* changeWeather;
        changeWeather = (HandlerParam_ChangeWeather*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_WEATHER, pokemonSlot);
        changeWeather->weather = WEATHER_NULL;
        BattleHandler_PopWork(serverFlow, changeWeather);

        HandlerParam_AddFieldEffect* addFieldEffect;
        addFieldEffect = (HandlerParam_AddFieldEffect*)BattleHandler_PushWork(serverFlow, EFFECT_REMOVE_FIELD_EFFECT, pokemonSlot);
        addFieldEffect->effect = FLDEFF_TERRAIN;
        BattleHandler_PopWork(serverFlow, addFieldEffect);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
BattleEventHandlerTableEntry TeraformZeroHandlers[]{
    {EVENT_AFTER_ABILITY_CHANGE, HandlerTeraformZero},
};
extern "C" BattleEventHandlerTableEntry * EventAddTeraformZero(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(TeraformZeroHandlers);
    return TeraformZeroHandlers;
}

// POISON PUPPETEER
extern "C" void HandlerPoisonPuppeteer(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)
        && BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_POISON) {
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);

        HandlerParam_AddCondition* addCondition;
        addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
        addCondition->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        addCondition->condition = CONDITION_CONFUSION;
        addCondition->pokeID = defendingSlot;
        MakeCondition(addCondition->condition, Handler_GetBattleMon(serverFlow, addCondition->pokeID), &addCondition->condData);
        BattleHandler_PopWork(serverFlow, addCondition);
    }
}
BattleEventHandlerTableEntry PoisonPuppeteerHandlers[]{
    {EVENT_ADD_BASIC_STATUS, HandlerPoisonPuppeteer},
};
extern "C" BattleEventHandlerTableEntry * EventAddPoisonPuppeteer(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PoisonPuppeteerHandlers);
    return PoisonPuppeteerHandlers;
}

AbilityEventAddTable AbilityEventAddTableExt[]{
    { ABIL022_INTIMIDATE, EventAddIntimidate },
    { ABIL029_CLEAR_BODY, EventAddClearBody },
    { ABIL073_WHITE_SMOKE, EventAddClearBody },
    { ABIL039_INNER_FOCUS, EventAddInnerFocus },
    { ABIL080_STEADFAST, EventAddSteadfast },
    { ABIL047_THICK_FAT, EventAddThickFat },
    { ABIL052_HYPER_CUTTER, EventAddHyperCutter },
    { ABIL037_HUGE_POWER, EventAddHugePower },
    { ABIL074_PURE_POWER, EventAddHugePower },
    { ABIL110_TINTED_LENS, EventAddTintedLens },
    { ABIL003_SPEED_BOOST, EventAddSpeedBoost },
    { ABIL066_BLAZE, EventAddBlaze },
    { ABIL067_TORRENT, EventAddTorrent },
    { ABIL065_OVERGROW, EventAddOvergrow },
    { ABIL068_SWARM, EventAddSwarm },
    { ABIL062_GUTS, EventAddGuts },
    { ABIL092_SKILL_LINK, EventAddSkillLink },
    { ABIL051_KEEN_EYE, EventAddKeenEye },
    { ABIL086_SIMPLE, EventAddSimple },
    { ABIL116_SOLID_ROCK, EventAddSolidRock },
    { ABIL111_FILTER, EventAddSolidRock },
    { ABIL063_MARVEL_SCALE, EventAddMarvelScale },
    { ABIL079_RIVALRY, EventAddRivalry },
    { ABIL102_LEAF_GUARD, EventAddLeafGuard },
    { ABIL002_DRIZZLE, EventAddDrizzle },
    { ABIL070_DROUGHT, EventAddDrought },
    { ABIL045_SAND_STREAM, EventAddSandStream },
    { ABIL117_SNOW_WARNING, EventAddSnowWarning },
    { ABIL076_AIR_LOCK, EventAddAirLock },
    { ABIL013_CLOUD_NINE, EventAddAirLock },
    { ABIL101_TECHNICIAN, EventAddTechnician },
    { ABIL012_OBLIVIOUS, EventAddOblivious },
    { ABIL093_HYDRATION, EventAddHydration },
    { ABIL090_POISON_HEAL, EventAddPoisonHeal },
    { ABIL115_ICE_BODY, EventAddIceBody },
    { ABIL044_RAIN_DISH, EventAddRainDish },
    { ABIL019_SHIELD_DUST, EventAddShieldDust },
    { ABIL091_ADAPTABILITY, EventAddAdaptability },
    { ABIL032_SERENE_GRACE, EventAddSereneGrace },
    { ABIL094_SOLAR_POWER, EventAddSolarPower },
    { ABIL033_SWIFT_SWIM, EventAddSwiftSwim },
    { ABIL034_CHLOROPHYLL, EventAddChlorophyll },
    { ABIL061_SHED_SKIN, EventAddShedSkin },
    { ABIL077_TANGLED_FEET, EventAddTangledFeet },
    { ABIL095_QUICK_FEET, EventAddQuickFeet },
    { ABIL055_HUSTLE, EventAddHustle },
    { ABIL004_BATTLE_ARMOR, EventAddBattleArmor },
    { ABIL075_SHELL_ARMOR, EventAddBattleArmor },
    { ABIL105_SUPER_LUCK, EventAddSuperLuck },
    { ABIL083_ANGER_POINT, EventAddAngerPoint },
    { ABIL097_SNIPER, EventAddSniper },
    { ABIL089_IRON_FIST, EventAddIronFist },
    { ABIL014_COMPOUND_EYES, EventAddCompoundEyes },
    { ABIL069_ROCK_HEAD, EventAddRockHead },
    { ABIL120_RECKLESS, EventAddReckless },
    { ABIL009_STATIC, EventAddStatic },
    { ABIL038_POISON_POINT, EventAddPoisonPoint },
    { ABIL049_FLAME_BODY, EventAddFlameBody },
    { ABIL027_EFFECT_SPORE, EventAddEffectSpore },
    { ABIL057_PLUS, EventAddPlusMinus },
    { ABIL058_MINUS, EventAddPlusMinus },
    { ABIL056_CUTE_CHARM, EventAddCuteCharm },
    { ABIL008_SAND_VEIL, EventAddSandVeil },
    { ABIL081_SNOW_CLOAK, EventAddSnowCloak },
    { ABIL036_TRACE, EventAddTrace },
    { ABIL096_NORMALIZE, EventAddNormalize },
    { ABIL024_ROUGH_SKIN, EventAddRoughSkin },
    { ABIL030_NATURAL_CURE, EventAddNaturalCure },
    { ABIL028_SYNCHRONIZE, EventAddSynchronize },
    { ABIL088_DOWNLOAD, EventAddDownload },
    { ABIL005_STURDY, EventAddSturdy },
    { ABIL085_HEATPROOF, EventAddHeatproof },
    { ABIL109_UNAWARE, EventAddUnaware },
    { ABIL087_DRY_SKIN, EventAddDrySkin },
    { ABIL010_VOLT_ABSORB, EventAddVoltAbsorb },
    { ABIL011_WATER_ABSORB, EventAddWaterAbsorb },
    { ABIL078_MOTOR_DRIVE, EventAddMotorDrive },
    { ABIL007_LIMBER, EventAddLimber },
    { ABIL015_INSOMNIA, EventAddInsomnia },
    { ABIL072_VITAL_SPIRIT, EventAddInsomnia },
    { ABIL020_OWN_TEMPO, EventAddOwnTempo },
    { ABIL040_MAGMA_ARMOR, EventAddMagmaArmor },
    { ABIL041_WATER_VEIL, EventAddWaterVeil },
    { ABIL017_IMMUNITY, EventAddImmunity },
    { ABIL113_SCRAPPY, EventAddScrappy },
    { ABIL043_SOUNDPROOF, EventAddSoundproof },
    { ABIL026_LEVITATE, EventAddLevitate },
    { ABIL122_FLOWER_GIFT, EventAddFlowerGift },
    { ABIL018_FLASH_FIRE, EventAddFlashFire },
    { ABIL108_FOREWARN, EventAddForewarn },
    { ABIL107_ANTICIPATION, EventAddAnticipation },
    { ABIL119_FRISK, EventAddFrisk },
    { ABIL106_AFTERMATH, EventAddAftermath },
    { ABIL050_RUN_AWAY, EventAddRunAway },
    { ABIL016_COLOR_CHANGE, EventAddColorChange },
    { ABIL104_MOLD_BREAKER, EventAddMoldBreaker },
    { ABIL054_TRUANT, EventAddTruant },
    { ABIL031_LIGHTNING_ROD, EventAddLightningRod },
    { ABIL114_STORM_DRAIN, EventAddStormDrain },
    { ABIL112_SLOW_START, EventAddSlowStart },
    { ABIL006_DAMP, EventAddDamp },
    { ABIL025_WONDER_GUARD, EventAddWonderGuard },
    { ABIL100_STALL, EventAddStall },
    { ABIL059_FORECAST, EventAddForecast },
    { ABIL021_SUCTION_CUPS, EventAddSuctionCups },
    { ABIL064_LIQUID_OOZE, EventAddLiquidOoze },
    { ABIL103_KLUTZ, EventAddKlutz },
    { ABIL060_STICKY_HOLD, EventAddStickyHold },
    { ABIL046_PRESSURE, EventAddPressure },
    { ABIL098_MAGIC_GUARD, EventAddMagicGuard },
    { ABIL123_BAD_DREAMS, EventAddBadDreams },
    { ABIL053_PICKUP, EventAddPickup },
    { ABIL084_UNBURDEN, EventAddUnburden },
    { ABIL001_STENCH, EventAddStench },
    { ABIL023_SHADOW_TAG, EventAddShadowTag },
    { ABIL071_ARENA_TRAP, EventAddArenaTrap },
    { ABIL042_MAGNET_PULL, EventAddMagnetPull },
    { ABIL124_PICKPOCKET, EventAddPickpocket },
    { ABIL125_SHEER_FORCE, EventAddSheerForce },
    { ABIL128_DEFIANT, EventAddDefiant },
    { ABIL129_DEFEATIST, EventAddDefeatist },
    { ABIL136_MULTISCALE, EventAddMultiscale },
    { ABIL134_HEAVY_METAL, EventAddHeavyMetal },
    { ABIL135_LIGHT_METAL, EventAddLightMetal },
    { ABIL126_CONTRARY, EventAddContrary },
    { ABIL127_UNNERVE, EventAddUnnerve },
    { ABIL130_CURSED_BODY, EventAddCursedBody },
    { ABIL131_HEALER, EventAddHealer },
    { ABIL132_FRIEND_GUARD, EventAddFriendGuard },
    { ABIL133_WEAK_ARMOR, EventAddWeakArmor },
    { ABIL137_TOXIC_BOOST, EventAddToxicBoost },
    { ABIL138_FLARE_BOOST, EventAddFlareBoost },
    { ABIL139_HARVEST, EventAddHarvest },
    { ABIL140_TELEPATHY, EventAddTelepathy },
    { ABIL141_MOODY, EventAddMoody },
    { ABIL142_OVERCOAT, EventAddOvercoat },
    { ABIL143_POISON_TOUCH, EventAddPoisonTouch },
    { ABIL144_REGENERATOR, EventAddRegenerator },
    { ABIL145_BIG_PECKS, EventAddBigPecks },
    { ABIL146_SAND_RUSH, EventAddSandRush },
    { ABIL147_WONDER_SKIN, EventAddWonderSkin },
    { ABIL148_ANALYTIC, EventAddAnalytic },
    { ABIL149_ILLUSION, EventAddIllusion },
    { ABIL150_IMPOSTER, EventAddImposter },
    { ABIL151_INFILTRATOR, EventAddInfiltrator },
    { ABIL152_MUMMY, EventAddMummy },
    { ABIL153_MOXIE, EventAddMoxie },
    { ABIL154_JUSTIFIED, EventAddJustified },
    { ABIL155_RATTLED, EventAddRattled },
    { ABIL156_MAGIC_BOUNCE, EventAddMagicBounce },
    { ABIL157_SAP_SIPPER, EventAddSapSipper },
    { ABIL158_PRANKSTER, EventAddPrankster },
    { ABIL159_SAND_FORCE, EventAddSandForce },
    { ABIL160_IRON_BARBS, EventAddRoughSkin },
    { ABIL161_ZEN_MODE, EventAddZenMode },
    { ABIL162_VICTORY_STAR, EventAddVictoryStar },
    { ABIL163_TURBOBLAZE, EventAddMoldBreaker },
    { ABIL164_TERAVOLT, EventAddMoldBreaker },
    { ABIL165_AROMA_VEIL, EventAddAromaVeil },
    { ABIL166_FLOWER_VEIL, EventAddFlowerVeil },
    { ABIL167_CHEEK_POUCH, EventAddCheekPouch },
    { ABIL168_PROTEAN, EventAddProtean },
    { ABIL169_FUR_COAT, EventAddFurCoat },
    { ABIL170_MAGICIAN, EventAddMagician },
    { ABIL171_BULLETPROOF, EventAddBulletProof },
    { ABIL172_COMPETITIVE, EventAddCompetitive },
    { ABIL173_STRONG_JAW, EventAddStrongJaw },
    { ABIL174_REFRIGERATE, EventAddRefrigerate },
    { ABIL175_SWEET_VEIL, EventAddSweetVeil },
    { ABIL176_STANCE_CHANGE, EventAddStanceChange },
    { ABIL177_GALE_WINGS, EventAddGaleWings },
    { ABIL178_MEGA_LAUNCHER, EventAddMegaLauncher },
    { ABIL179_GRASS_PELT, EventAddGrassPelt },
    { ABIL180_SYMBIOSIS, EventAddSymbiosis },
    { ABIL181_TOUGH_CLAWS, EventAddToughClaws },
    { ABIL182_PIXILATE, EventAddPixilate },
    { ABIL183_GOOEY, EventAddGooey },
    { ABIL184_AERILATE , EventAddAerilate },
    { ABIL185_PARENTAL_BOND, EventAddParentalBond },
    { ABIL186_DARK_AURA, EventAddDarkAura },
    { ABIL187_FAIRY_AURA, EventAddFairyAura },
    { ABIL188_AURA_BREAK, EventAddAuraBreak },
    { ABIL189_PRIMORDIAL_SEA, EventAddPrimordialSea },
    { ABIL190_DESOLATE_LAND, EventAddDesolateLand },
    { ABIL191_DELTA_STREAM, EventAddDeltaStream },
    { ABIL192_STAMINA, EventAddStamina },
    { ABIL193_WIMP_OUT, EventAddEmergencyExit },
    { ABIL194_EMERGENCY_EXIT, EventAddEmergencyExit },
    { ABIL195_WATER_COMPACTION, EventAddWaterCompaction },
    { ABIL196_MERCILESS, EventAddMerciless },
    { ABIL197_SHIELDS_DOWN, EventAddShieldsDown },
    { ABIL198_STAKEOUT, EventAddStakeout },
    { ABIL199_WATER_BUBBLE, EventAddWaterBubble },
    { ABIL200_STEELWORKER, EventAddSteelworker },
    { ABIL201_BERSERK, EventAddBerserk },
    { ABIL202_SLUSH_RUSH, EventAddSlushRush },
    { ABIL204_LIQUID_VOICE, EventAddLiquidVoice },
    { ABIL205_TRIAGE, EventAddTriage },
    { ABIL206_GALVANIZE, EventAddGalvanize },
    { ABIL207_SURGE_SURFER, EventAddSurgeSurfer },
    { ABIL208_SCHOOLING, EventAddShieldsDown },
    { ABIL209_DISGUISE, EventAddDisguise },
    { ABIL210_BATTLE_BOND, EventAddBattleBond },
    { ABIL211_POWER_CONSTRUCT, EventAddPowerConstruct },
    { ABIL214_QUEENLY_MAGESTY, EventAddQueenlyMagesty },
    { ABIL215_INNARDS_OUT, EventAddInnardsOut },
    { ABIL216_DANCER, EventAddDancer },
    { ABIL217_BATTERY, EventAddBattery },
    { ABIL218_FLUFFY, EventAddFluffy },
    { ABIL219_DAZZLING, EventAddQueenlyMagesty },
    { ABIL220_SOUL_HEART, EventAddSoulHeart },
    { ABIL221_TANGLING_HAIR, EventAddGooey },
    { ABIL222_RECEIVER, EventAddReceiver },
    { ABIL223_POWER_OF_ALCHEMY, EventAddReceiver },
    { ABIL224_BEAST_BOOST, EventAddBeastBoost },
    { ABIL226_ELECTRIC_SURGE, EventAddElectricSurge },
    { ABIL227_PSYCHIC_SURGE, EventAddPsychicSurge },
    { ABIL228_MISTY_SURGE, EventAddMistySurge },
    { ABIL229_GRASSY_SURGE, EventAddGrassySurge },
    { ABIL230_FULL_METAL_BODY, EventAddClearBody },
    { ABIL231_SHADOW_SHIELD, EventAddMultiscale },
    { ABIL232_PRISM_ARMOR, EventAddPrismArmor },
    { ABIL233_NEUROFORCE, EventAddNeuroforce },
    { ABIL234_INTREPID_SWORD, EventAddIntrepidSword },
    { ABIL235_DAUNTLESS_SHIELD, EventAddDauntlessShield },
    { ABIL236_LIBERO, EventAddProtean },
    { ABIL237_BALL_FETCH, EventAddBallFetch },
    { ABIL238_COTTON_DOWN, EventAddCottonDown },
    { ABIL239_PROPELLER_TAIL, EventAddPropellerTail },
    { ABIL240_MIRROR_ARMOR, EventAddMirrorArmor },
    { ABIL241_GULP_MISSILE, EventAddGulpMissile },
    { ABIL242_STALWART, EventAddPropellerTail },
    { ABIL243_STEAM_ENGINE, EventAddSteamEngine },
    { ABIL244_PUNK_ROCK, EventAddPunkRock },
    { ABIL245_SAND_SPIT, EventAddSandSpit },
    { ABIL246_ICE_SCALES, EventAddIceScales },
    { ABIL247_RIPEN, EventAddRipen },
    { ABIL248_ICE_FACE, EventAddIceFace },
    { ABIL249_POWER_SPOT, EventAddPowerSpot },
    { ABIL250_MIMICRY, EventAddMimicry },
    { ABIL251_SCREEN_CLEANER, EventAddScreenCleaner },
    { ABIL252_STEELY_SPIRIT, EventAddSteelySpirit },
    { ABIL253_PERISH_BODY, EventAddPerishBody },
    { ABIL254_WANDERING_SPIRIT, EventAddWanderingSpirit },
    { ABIL255_GORILLA_TACTICS, EventAddGorillaTactics },
    { ABIL256_NEUTRALIZING_GAS, EventAddNeutralizingGas },
    { ABIL257_PASTEL_VEIL, EventAddPastelVeil },
    { ABIL258_HUNGER_SWITCH, EventAddHungerSwitch },
    { ABIL259_QUICK_DRAW, EventAddQuickDraw },
    { ABIL260_UNSEEN_FIST, EventAddUnseenFist },
    { ABIL261_CURIOUS_MEDICINE, EventAddCuriousMedicine },
    { ABIL262_TRANSISTOR, EventAddTransistor },
    { ABIL263_DRAGON_S_MAW, EventAddDragonsMaw },
    { ABIL264_CHILLING_NEIGH, EventAddChillingNeigh },
    { ABIL265_GRIM_NEIGH, EventAddGrimNeigh },
    { ABIL266_AS_ONE, EventAddAsOne },
    { ABIL267_AS_ONE, EventAddAsOne },
    { ABIL268_LINGERING_AROMA, EventAddMummy },
    { ABIL269_SEED_SOWER, EventAddSeedSower },
    { ABIL270_THERMAL_EXCHANGE, EventAddThermalExchange },
    { ABIL271_ANGER_SHELL, EventAddAngerShell },
    { ABIL272_PURIFYING_SALT, EventAddPurifyingSalt },
    { ABIL273_WELL_BAKED_BODY, EventAddWellBakedBody },
    { ABIL274_WIND_RIDER, EventAddWindRider },
    { ABIL275_GUARD_DOG, EventAddGuardDog },
    { ABIL276_ROCKY_PAYLOAD, EventAddRockyPayload },
    { ABIL277_WIND_POWER, EventAddWindPower },
    { ABIL278_ZERO_TO_HERO, EventAddZeroToHero },
    { ABIL279_COMMANDER, EventAddCommander },
    { ABIL280_ELECTROMORPHOSIS, EventAddElectromorphosis },
    { ABIL281_PROTOSYNTHESIS, EventAddProtosynthesis },
    { ABIL282_QUARK_DRIVE, EventAddQuarkDrive },
    { ABIL283_GOOD_AS_GOLD, EventAddGoodAsGold },
    { ABIL284_VESSEL_OF_RUIN, EventAddVesselOfRuin },
    { ABIL285_SWORD_OF_RUIN, EventAddSwordOfRuin },
    { ABIL286_TABLETS_OF_RUIN, EventAddTabletsOfRuin },
    { ABIL287_BEADS_OF_RUIN, EventAddBeadsOfRuin },
    { ABIL288_ORICHALCUM_PULSE, EventAddOrichalcumPulse },
    { ABIL289_HADRON_ENGINE, EventAddHadronEngine },
    { ABIL290_OPPORTUNIST , EventAddOpportunist },
    { ABIL291_CUD_CHEW, EventAddCudChew },
    { ABIL292_SHARPNESS, EventAddSharpness },
    { ABIL293_SUPREME_OVERLORD, EventAddSupremeOverlord },
    { ABIL294_COSTAR, EventAddCostar },
    { ABIL295_TOXIC_DEBRIS, EventAddToxicDebris },
    { ABIL296_ARMOR_TAIL, EventAddQueenlyMagesty },
    { ABIL297_EARTH_EATER, EventAddEarthEater },
    { ABIL298_MYCELIUM_MIGHT, EventAddMyceliumMight },
    { ABIL299_HOSPITALITY, EventAddHospitality },
    { ABIL300_MIND_S_EYE, EventAddMindsEye },
    { ABIL301_EMBODY_ASPECT_TEAL , EventAddEmbodyAspectTeal },
    { ABIL302_EMBODY_ASPECT_HEARTHFLAME , EventAddEmbodyAspectHearthflame },
    { ABIL303_EMBODY_ASPECT_WELLSPRING , EventAddEmbodyAspectWellspring },
    { ABIL304_EMBODY_ASPECT_CORNERSTONE , EventAddEmbodyAspectCornerstorne },
    { ABIL305_TOXIC_CHAIN, EventAddToxicChain },
    { ABIL306_SUPERSWEET_SYRUP, EventAddSupersweetSyrup },
    { ABIL307_TERA_SHIFT, EventAddTeraShift },
    { ABIL308_TERA_SHELL, EventAddTeraShell },
    { ABIL309_TERAFORM_ZERO, EventAddTeraformZero },
    { ABIL310_POISON_PUPPETEE, EventAddPoisonPuppeteer },
};

extern "C" BattleEventItem* THUMB_BRANCH_AbilityEvent_AddItem(BattleMon* battleMon) {
    ABILITY ability = BattleMon_GetValue(battleMon, VALUE_ABILITY);

    for (u32 abilityEvent = 0; abilityEvent < ARRAY_COUNT(AbilityEventAddTableExt); ++abilityEvent) {
        if (ability == AbilityEventAddTableExt[abilityEvent].ability) {
            u32 handlerAmount;
            BattleEventHandlerTableEntry* battleHandlerItems = AbilityEventAddTableExt[abilityEvent].func(&handlerAmount);

            BattleEventPriority mainPrio = GetHandlerMainPriority(&handlerAmount);
            u32 subPrio = AbilityEvent_GetSubPriority(battleMon);
            u32 pokemonSlot = BattleMon_GetID(battleMon);
            return BattleEvent_AddItem(EVENTITEM_ABILITY, ability, mainPrio, subPrio, pokemonSlot, battleHandlerItems, handlerAmount);
        }
    }
    return 0;
}

ABILITY MoldBreakerAffectedAbilitiesExt[]{
    ABIL004_BATTLE_ARMOR,
    ABIL005_STURDY,
    ABIL006_DAMP,
    ABIL007_LIMBER,
    ABIL008_SAND_VEIL,
    ABIL010_VOLT_ABSORB,
    ABIL011_WATER_ABSORB,
    ABIL012_OBLIVIOUS,
    ABIL015_INSOMNIA,
    ABIL017_IMMUNITY,
    ABIL018_FLASH_FIRE,
    ABIL019_SHIELD_DUST,
    ABIL020_OWN_TEMPO,
    ABIL021_SUCTION_CUPS,
    ABIL025_WONDER_GUARD,
    ABIL026_LEVITATE,
    ABIL029_CLEAR_BODY,
    ABIL031_LIGHTNING_ROD,
    ABIL035_ILLUMINATE,
    ABIL039_INNER_FOCUS,
    ABIL040_MAGMA_ARMOR,
    ABIL041_WATER_VEIL,
    ABIL043_SOUNDPROOF,
    ABIL047_THICK_FAT,
    ABIL051_KEEN_EYE,
    ABIL052_HYPER_CUTTER,
    ABIL060_STICKY_HOLD,
    ABIL063_MARVEL_SCALE,
    ABIL072_VITAL_SPIRIT,
    ABIL073_WHITE_SMOKE,
    ABIL075_SHELL_ARMOR,
    ABIL077_TANGLED_FEET,
    ABIL078_MOTOR_DRIVE,
    ABIL081_SNOW_CLOAK,
    ABIL085_HEATPROOF,
    ABIL086_SIMPLE,
    ABIL087_DRY_SKIN,
    ABIL102_LEAF_GUARD,
    ABIL109_UNAWARE,
    ABIL111_FILTER,
    ABIL114_STORM_DRAIN,
    ABIL116_SOLID_ROCK,
    ABIL122_FLOWER_GIFT,
    ABIL126_CONTRARY,
    ABIL132_FRIEND_GUARD,
    ABIL134_HEAVY_METAL,
    ABIL135_LIGHT_METAL,
    ABIL136_MULTISCALE,
    ABIL140_TELEPATHY,
    ABIL142_OVERCOAT,
    ABIL145_BIG_PECKS,
    ABIL147_WONDER_SKIN,
    ABIL156_MAGIC_BOUNCE,
    ABIL157_SAP_SIPPER,
    ABIL165_AROMA_VEIL,
    ABIL166_FLOWER_VEIL,
    ABIL169_FUR_COAT,
    ABIL171_BULLETPROOF,
    ABIL175_SWEET_VEIL,
    ABIL188_AURA_BREAK,
    ABIL199_WATER_BUBBLE,
    ABIL209_DISGUISE,
    ABIL214_QUEENLY_MAGESTY,
    ABIL218_FLUFFY,
    ABIL219_DAZZLING,
    ABIL240_MIRROR_ARMOR,
    ABIL244_PUNK_ROCK,
    ABIL246_ICE_SCALES,
    ABIL248_ICE_FACE,
    ABIL257_PASTEL_VEIL,
    ABIL270_THERMAL_EXCHANGE,
    ABIL272_PURIFYING_SALT,
    ABIL273_WELL_BAKED_BODY,
    ABIL274_WIND_RIDER,
    ABIL275_GUARD_DOG,
    ABIL283_GOOD_AS_GOLD,
    ABIL284_VESSEL_OF_RUIN,
    ABIL286_TABLETS_OF_RUIN,
    ABIL296_ARMOR_TAIL,
    ABIL297_EARTH_EATER,
    ABIL300_MIND_S_EYE,
    ABIL308_TERA_SHELL,
};
extern "C" b32 THUMB_BRANCH_IsMoldBreakerAffectedAbility(ABILITY ability) {
    for (u16 i = 0; i < ARRAY_COUNT(MoldBreakerAffectedAbilitiesExt); ++i) {
        if (ability == MoldBreakerAffectedAbilitiesExt[i]) {
            return 1;
        }
    }

    return 0;
}

extern "C" BattleEventItem * THUMB_BRANCH_ItemEvent_AddTempItem(BattleMon * battleMon, ITEM_ID itemID)
{
    if (!itemID) {
        return 0;
    }

    BattleEventItem* itemEvents = ItemEvent_AddItemCore(battleMon, itemID);
    if (!itemEvents) {
        return 0;
    }

    BattleEventItem_SetTempItemFlag(itemEvents);
    BattleEventItem_SetWorkValue(itemEvents, 6, 1);

    g_BattleField->tempItem = itemID; // store the temp item ID
    return itemEvents;
}

extern "C" void THUMB_BRANCH_ServerControl_ChangeHeldItem(ServerFlow * serverFlow, BattleMon * battleMon, ITEM_ID itemID, b32 consumeItem)
{
    u32 battleSlot = BattleMon_GetID(battleMon);
    ITEM_ID usedItem = BattleMon_GetHeldItem(battleMon);

    u32 HEID = HEManager_PushState(&serverFlow->HEManager);
    ServerEvent_ItemSetDecide(serverFlow, battleMon, itemID);
    HEManager_PopState(&serverFlow->HEManager, HEID);

    if (!itemID) {
        ServerDisplay_SetConditionFlag(serverFlow, battleMon, CONDITIONFLAG_NULL);
    }
    ItemEvent_RemoveItem(battleMon);
    ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_SetItem, battleSlot, itemID);
    BattleMon_SetItem(battleMon, itemID);
    if (itemID) {
        ItemEvent_AddItem(battleMon);
    }

    HEID = HEManager_PushState(&serverFlow->HEManager);
    ServerEvent_ItemRewriteDone(serverFlow, battleMon);
    HEManager_PopState(&serverFlow->HEManager, HEID);

    if (consumeItem) {
        BattleMon_ConsumeItem(battleMon, usedItem);
        ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_ConsumeItem, battleSlot, usedItem);
        ServerDisplay_SetTurnFlag(serverFlow, battleMon, TURNFLAG_ITEMCONSUMED);

        HEID = HEManager_PushState(&serverFlow->HEManager);
        ServerEvent_ConsumeItem(serverFlow, BattleMon_GetID(battleMon), usedItem); // Added consume item event
        HEManager_PopState(&serverFlow->HEManager, HEID);
    }
}

extern "C" void THUMB_BRANCH_ServerEvent_EquipItem(ServerFlow * serverFlow, BattleMon * currentMon) {
    ITEM_ID itemID = BattleMon_GetHeldItem(currentMon);

    BattleEventVar_Push();
    u32 currentSlot = BattleMon_GetID(currentMon);
    BattleEventVar_SetConstValue(VAR_MON_ID, currentSlot);
    BattleEventVar_SetValue(VAR_ITEM, itemID);
    BattleEvent_CallHandlers(serverFlow, EVENT_USE_ITEM);
    BattleEventVar_Pop();

    // Call the use item after event after using the item
    ServerEvent_UseItemAfter(serverFlow, currentSlot, itemID);
}
extern "C" void THUMB_BRANCH_ServerEvent_EquipTempItem(ServerFlow * serverFlow, BattleMon * currentMon, u32 attackingSlot)
{
    ITEM_ID itemID = g_BattleField->tempItem;
    g_BattleField->tempItem = IT_NULL;

    u32 currentSlot = BattleMon_GetID(currentMon);

    // Call the use item event before using the item
    ServerEvent_UseTempItemBefore(serverFlow, currentSlot, attackingSlot, itemID);

    BattleEventVar_Push();
    BattleEventVar_SetValue(VAR_MON_ID, currentSlot);
    BattleEventVar_SetValue(VAR_ATTACKING_MON, attackingSlot);
    BattleEventVar_SetValue(VAR_ITEM, itemID);
    BattleEvent_ForceCallHandlers(serverFlow, EVENT_USE_ITEM_TEMP);
    BattleEventVar_Pop();

    // Call the use item event after using the item
    ServerEvent_UseTempItemAfter(serverFlow, currentSlot, attackingSlot, itemID);
}

MOVE_ID movesThatIgnoreParentalBond[]{
    MOVE374_FLING,
    MOVE120_SELF_DESTRUCT,
    MOVE153_EXPLOSION,
    MOVE515_FINAL_GAMBIT,
    MOVE253_UPROAR,
    MOVE205_ROLLOUT,
    MOVE301_ICE_BALL,
    MOVE283_ENDEAVOR,
};
b32 DoesMoveIgnoreParentalBond(MOVE_ID moveID) {
    for (u16 i = 0; i < ARRAY_COUNT(movesThatIgnoreParentalBond); ++i) {
        if (moveID == movesThatIgnoreParentalBond[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" b32 ParentalBondCheck(ServerFlow* serverFlow, MOVE_ID moveID, BattleMon* attackingMon, PokeSet* targetSet) {
    if (BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY) != ABIL185_PARENTAL_BOND) {
        return 0;
    }

    u32 targetCount = PokeSet_GetCount(targetSet);
    // Only when targeting 1 Pokémon will Parental Bond work
    if (targetCount && targetCount == 1) {

        MoveParam* moveParam;
        ServerEvent_GetMoveParam(serverFlow, moveID, attackingMon, moveParam);
        // Self-targeting and side moves don't work
        if (moveParam->targetType == TARGET_USER ||
            moveParam->targetType == TARGET_FIELD ||
            moveParam->targetType == TARGET_SIDE_ENEMY ||
            moveParam->targetType == TARGET_SIDE_ALLY) {
            return 0;
        }

        // Charge up moves never work
        if (getMoveFlag(moveParam->moveID, FLAG_REQUIRES_CHARGE)) {
            return 0;
        }

        // There are specific moves that don't work
        if (DoesMoveIgnoreParentalBond(moveParam->moveID)) {
            return 0;
        }

        return 1;
    }
    return 0;
}
extern "C" void THUMB_BRANCH_LINK_ServerControl_DamageRoot_0x36(ServerFlow* serverFlow, BattleMon* attackingMon, MOVE_ID moveID, HitCheckParam* hitCheckParam) {
    ServerEvent_CheckMultihitHits(serverFlow, attackingMon, moveID, hitCheckParam);
    
    // Reset any substitute damage recorded
    sys_memset(actionSubstituteDamage, 0, BATTLE_MAX_SLOTS * sizeof(u32));

    // Reset Parental Bond flag
    parentalBondFlag = 0;
    if (!serverFlow->hitCheckParam->multiHitMove && ParentalBondCheck(serverFlow, moveID, attackingMon, serverFlow->setTargetOriginal)) {
        // I use [serverFlow->setTargetOriginal] so that Parental Bond takes into account 
        // all original targets even if they had Immunities of if the move missed
        serverFlow->hitCheckParam->countMax = 2;
        serverFlow->hitCheckParam->checkEveryTime = 0;
        serverFlow->hitCheckParam->multiHitMove = 1;

        // Set the Parental Bond flag so that the second hit has to deal less damage, used in [HandlerParentalBondPower]
        parentalBondFlag = 1;
    }
}

// Generic Ability Change ability check
ABILITY AbilityChangeGenericFailAbilities[] = {
    ABIL121_MULTITYPE,
    ABIL161_ZEN_MODE,
    ABIL176_STANCE_CHANGE,
    ABIL197_SHIELDS_DOWN,
    ABIL208_SCHOOLING,
    ABIL209_DISGUISE,
    ABIL210_BATTLE_BOND,
    ABIL213_COMATOSE,
    ABIL225_RKS_SYSTEM,
    ABIL241_GULP_MISSILE,
    ABIL248_ICE_FACE,
    ABIL278_ZERO_TO_HERO,
    ABIL279_COMMANDER,
    ABIL307_TERA_SHIFT,
};
extern "C" b32 IsAbilityChangeGenericFailAbility(ABILITY ability) {
    for (u16 i = 0; i < ARRAY_COUNT(AbilityChangeGenericFailAbilities); ++i) {
        if (ability == AbilityChangeGenericFailAbilities[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" b32 THUMB_BRANCH_j_j_Tables_IsAbilityMultitype(ABILITY ability) {
    return IsAbilityChangeGenericFailAbility(ability);
}

// Trace opposing ability check
ABILITY AbilityChangeFailAbilities[] = {
    ABIL036_TRACE,
    ABIL059_FORECAST,
    ABIL121_MULTITYPE,
    ABIL122_FLOWER_GIFT,
    ABIL161_ZEN_MODE,
    ABIL149_ILLUSION,
    ABIL150_IMPOSTER,
    ABIL025_WONDER_GUARD,
    ABIL176_STANCE_CHANGE,
    ABIL197_SHIELDS_DOWN,
    ABIL208_SCHOOLING,
    ABIL209_DISGUISE,
    ABIL210_BATTLE_BOND,
    ABIL211_POWER_CONSTRUCT,
    ABIL213_COMATOSE,
    ABIL222_RECEIVER,
    ABIL223_POWER_OF_ALCHEMY,
    ABIL225_RKS_SYSTEM,
    ABIL241_GULP_MISSILE,
    ABIL248_ICE_FACE,
    ABIL256_NEUTRALIZING_GAS,
    ABIL258_HUNGER_SWITCH,
    ABIL278_ZERO_TO_HERO,
    ABIL279_COMMANDER,
    ABIL281_PROTOSYNTHESIS,
    ABIL282_QUARK_DRIVE,
    ABIL288_ORICHALCUM_PULSE,
    ABIL289_HADRON_ENGINE,
    ABIL307_TERA_SHIFT,
    ABIL308_TERA_SHELL,
    ABIL309_TERAFORM_ZERO,
};
extern "C" b32 IsAbilityChangeFailAbility(ABILITY ability) {
    for (u16 i = 0; i < ARRAY_COUNT(AbilityChangeFailAbilities); ++i) {
        if (ability == AbilityChangeFailAbilities[i]) {
            return 1;
        }
    }
    return 0;
}
// Trace ability checks
extern "C" b32 THUMB_BRANCH_j_j_IsTraceFailAbility(ABILITY ability) {
    if (ability == ABIL025_WONDER_GUARD ||
        ability == ABIL288_ORICHALCUM_PULSE ||
        ability == ABIL289_HADRON_ENGINE) {
        return 0;
    }
    else {
        return IsAbilityChangeFailAbility(ability);
    }
}
extern "C" b32 THUMB_BRANCH_j_j_IsTraceFailAbility0(ABILITY ability) {
    if (ability == ABIL025_WONDER_GUARD ||
        ability == ABIL288_ORICHALCUM_PULSE ||
        ability == ABIL289_HADRON_ENGINE) {
        return 0;
    }
    else {
        return IsAbilityChangeFailAbility(ability);
    }
}
// Entrainment opposing ability check
extern "C" b32 THUMB_BRANCH_j_j_j_IsTraceFailAbility1(ABILITY ability) {
    // Entrainment can override Wonder Guard
    if (ability == ABIL025_WONDER_GUARD) {
        return 0;
    }

    return IsAbilityChangeGenericFailAbility(ability);
}
// Role Play opposing ability check
extern "C" b32 THUMB_BRANCH_j_j_IsRolePlayFailAbility(ABILITY ability) {
    return IsAbilityChangeFailAbility(ability);
}

// Skill Swap current ability check
ABILITY AbilitySwapFailAbilities[] = {
    ABIL025_WONDER_GUARD,
    ABIL121_MULTITYPE,
    ABIL149_ILLUSION,
    ABIL161_ZEN_MODE,
    ABIL176_STANCE_CHANGE,
    ABIL197_SHIELDS_DOWN,
    ABIL208_SCHOOLING,
    ABIL209_DISGUISE,
    ABIL210_BATTLE_BOND,
    ABIL213_COMATOSE,
    ABIL222_RECEIVER,
    ABIL223_POWER_OF_ALCHEMY,
    ABIL225_RKS_SYSTEM,
    ABIL241_GULP_MISSILE,
    ABIL248_ICE_FACE,
    ABIL256_NEUTRALIZING_GAS,
    ABIL258_HUNGER_SWITCH,
    ABIL278_ZERO_TO_HERO,
    ABIL279_COMMANDER,
    ABIL281_PROTOSYNTHESIS,
    ABIL282_QUARK_DRIVE,
    ABIL288_ORICHALCUM_PULSE,
    ABIL289_HADRON_ENGINE,
    ABIL307_TERA_SHIFT,
    ABIL308_TERA_SHELL,
    ABIL309_TERAFORM_ZERO,
};
extern "C" b32 IsAbilitySwapFailAbility(ABILITY ability) {
    for (u16 i = 0; i < ARRAY_COUNT(AbilitySwapFailAbilities); ++i) {
        if (ability == AbilitySwapFailAbilities[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" b32 THUMB_BRANCH_j_CheckSkillSwapFailAbility(ABILITY ability) {
    return IsAbilitySwapFailAbility(ability);
}
// Skill Swap opposing ability check
extern "C" b32 THUMB_BRANCH_j_CheckSkillSwapFailAbility0(ABILITY ability) {
    return IsAbilitySwapFailAbility(ability);
}

extern "C" u32 MultiplyValueByRatio(u32 value, u32 ratio);

extern "C" u32 THUMB_BRANCH_ServerEvent_CalcMoveHealAmount(ServerFlow* serverFlow, MOVE_ID moveID, BattleMon* currentMon)
{
    u32 finalHeal = 0;

    u32 healParam = PML_MoveGetParam(moveID, MVDATA_HEAL);
    BattleEventVar_Push();
    u32 currentSlot = BattleMon_GetID(currentMon);
    BattleEventVar_SetConstValue(VAR_MON_ID, currentSlot);
    BattleEventVar_SetValue(VAR_RATIO, 0);
    // Added move ID
    BattleEventVar_SetConstValue(VAR_MOVE_ID, moveID);
    BattleEvent_CallHandlers(serverFlow, EVENT_RECOVER_HP);
    u32 ratio = BattleEventVar_GetValue(VAR_RATIO);
    BattleEventVar_Pop();
    u32 healAmount = BattleMon_GetValue(currentMon, VALUE_MAX_HP);

    if (ratio)
        finalHeal = fixed_round(healAmount, ratio);
    else
        finalHeal = MultiplyValueByRatio(healAmount, healParam);

    if (!finalHeal)
        return 1;
    if (finalHeal > healAmount)
        return healAmount;
    return finalHeal;
}

extern "C" void THUMB_BRANCH_ServerControl_CureCondition(ServerFlow * serverFlow, BattleMon * battleMon, CONDITION condition, ConditionData * prevCondition) {
    if (condition) {
        u32 pokemonSlot = BattleMon_GetID(battleMon);
        if (prevCondition) {
            *prevCondition = BattleMon_GetMoveCondition(battleMon, condition);
        }
        if (condition >= CONDITION_CONFUSION) {
            BattleMon_CureMoveCondition(battleMon, condition);
            if (condition == CONDITION_GASTROACID) {
                // Call server event when ability nullify is over
                ServerEvent_AbilityNullifyCured(serverFlow, battleMon);
            }
            ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_CureMoveCondition, pokemonSlot, condition);
        }
        else {
            CureStatusCondition(battleMon);
            ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_CureStatusCondition, pokemonSlot);
            if (PosPoke_GetPokeExistPos(&serverFlow->posPoke, pokemonSlot) != NULL_BATTLE_POS) {
                ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_StatusIcon, pokemonSlot, 0);
            }
        }
    }
}

extern "C" b32 THUMB_BRANCH_ServerControl_SimpleDamageCore(ServerFlow * serverFlow, BattleMon * battleMon, int damage, HandlerParam_StrParams * str) {
    int damageToDeal = -damage;
    if (!damageToDeal) {
        return 0;
    }

    ServerDisplay_SimpleHP(serverFlow, battleMon, damageToDeal, 1);
    TurnFlag_Set(battleMon, TURNFLAG_DAMAGED);
    if (str) {
        BattleHandler_SetString(serverFlow, str);
        BattleHandler_StrClear(str);
    }

    ServerControl_CheckItemReaction(serverFlow, battleMon, 1);
    // Call to new event
    ServerEvent_SimpleDamageReaction(serverFlow, battleMon, damage);
    if (ServerControl_CheckFainted(serverFlow, battleMon)) {
        ServerControl_CheckMatchup(serverFlow);
    }

    return 1;
}

extern "C" b32 THUMB_BRANCH_BattleMon_AddSubstituteDamage(BattleMon * battleMon, u32 * damage) {
    b32 result;

    u32 substituteHP = battleMon->substituteHP;
    if (substituteHP > *damage) {
        battleMon->substituteHP = substituteHP - *damage;
        result = 0;
    }
    else {
        *damage = substituteHP;
        battleMon->substituteHP = 0;
        result = 1;
    }

    actionSubstituteDamage[battleMon->battleSlot] += *damage;
    return result;
}

extern "C" void THUMB_BRANCH_LINK_ServerFlow_SetupBeforeFirstTurn_0x6E(ServerFlow* serverFlow, u32 clientID, u32 switchInSlot, u32 switchOutSlot) {
    ServerControl_SwitchInCore(serverFlow, clientID, switchInSlot, switchOutSlot);

    // Store the starting Pokémon in the firstTurnMons array
    BattleParty* party = PokeCon_GetBattleParty(serverFlow->pokeCon, clientID);
    BattleMon* switchInMon = BattleParty_GetPartyMember(party, switchInSlot);
    for (u8 i = 0; i < 6; ++i) {
        if (g_BattleField->firstTurnMons[i] == BATTLE_MAX_SLOTS) {
            g_BattleField->firstTurnMons[i] = switchInMon->battleSlot;
            break;
        }
    }
}

extern "C" void THUMB_BRANCH_ServerEvent_GetMoveParam(ServerFlow* serverFlow, MOVE_ID moveID, BattleMon* battleMon, MoveParam* moveParam) {
    BattleEventVar_Push();

    u32 currentSlot = BattleMon_GetID(battleMon);
    POKE_TYPE moveType = PML_MoveGetType(moveID);
    POKE_TYPE pokeType = BattleMon_GetPokeType(battleMon);
    MoveCategory category = PML_MoveGetCategory(moveID);
    u32 targetType = PML_MoveGetParam(moveID, MVDATA_TARGET);

    BattleEventVar_SetConstValue(VAR_MON_ID, currentSlot);
    BattleEventVar_SetConstValue(VAR_MOVE_ID, moveID);
    BattleEventVar_SetRewriteOnceValue(VAR_MOVE_TYPE, moveType);
    BattleEventVar_SetValue(VAR_USER_TYPE, pokeType);
    BattleEventVar_SetValue(VAR_MOVE_CATEGORY, category);
    BattleEventVar_SetValue(VAR_TARGET_TYPE, targetType);
    BattleEventVar_SetRewriteOnceValue(VAR_NO_TYPE_EFFECTIVENESS, 0);

    BattleEvent_CallHandlers(serverFlow, EVENT_MOVE_PARAM);

    moveParam->moveID = moveID;
    moveParam->originalMoveID = moveID;
    moveParam->moveType = BattleEventVar_GetValue(VAR_MOVE_TYPE);
    moveParam->userType = BattleEventVar_GetValue(VAR_USER_TYPE);
    moveParam->category = BattleEventVar_GetValue(VAR_MOVE_CATEGORY);
    moveParam->targetType = BattleEventVar_GetValue(VAR_TARGET_TYPE);
    moveParam->flags = 0;
    if (BattleEventVar_GetValue(VAR_NO_TYPE_EFFECTIVENESS)) {
        moveParam->moveType = TYPE_NULL;
    }

    BattleEventVar_Pop();
}

extern "C" void THUMB_BRANCH_ServerControl_ApplyDamageToRecover(ServerFlow* serverFlow, BattleMon* attackingMon, MoveParam* moveParam, PokeSet* targetSet) {
    PokeSet_SeekStart(targetSet);
    for (BattleMon* currentMon = PokeSet_SeekNext(targetSet); currentMon; currentMon = PokeSet_SeekNext(targetSet)) {
        
        u32 checkDamageFlag = ServerEvent_CheckDamageToRecover(serverFlow, attackingMon, currentMon, moveParam);
        if (checkDamageFlag) {
            MoveAnimCtrl* moveAnimCtrl = serverFlow->moveAnimCtrl;
            if ((moveAnimCtrl->flags & 1) == 0) {
                moveAnimCtrl->flags = moveAnimCtrl->flags & 0xFE | 1;
            }

            u32 HEID = HEManager_PushState(&serverFlow->HEManager);
            ServerEvent_ApplyDamageToRecover(serverFlow, attackingMon, currentMon);
            HEManager_PopState(&serverFlow->HEManager, HEID);

            switch (checkDamageFlag)
            {
            case 1:
                PokeSet_Remove(targetSet, currentMon);
                break;
            case 2:
                --serverFlow->hitCheckParam->countMax;
                if (!serverFlow->hitCheckParam->countMax) {
                    PokeSet_Remove(targetSet, currentMon);
                }
                break;
            }
        }
    }
}

ABILITY AbilitiesImmuneToGastroAcid[] = {
    ABIL121_MULTITYPE,
    ABIL161_ZEN_MODE,
    ABIL176_STANCE_CHANGE,
    ABIL197_SHIELDS_DOWN,
    ABIL208_SCHOOLING,
    ABIL209_DISGUISE,
    ABIL210_BATTLE_BOND,
    ABIL211_POWER_CONSTRUCT,
    ABIL213_COMATOSE,
    ABIL225_RKS_SYSTEM,
    ABIL241_GULP_MISSILE,
    ABIL248_ICE_FACE,
    ABIL266_AS_ONE,
    ABIL267_AS_ONE,
    ABIL278_ZERO_TO_HERO,
    ABIL279_COMMANDER,
    ABIL281_PROTOSYNTHESIS,
    ABIL282_QUARK_DRIVE,
    ABIL307_TERA_SHIFT,
};
extern "C" b32 IsAbilityImmuneToGastroAcid(ABILITY ability) {
    for (u16 i = 0; i < ARRAY_COUNT(AbilitiesImmuneToGastroAcid); ++i) {
        if (AbilitiesImmuneToGastroAcid[i] == ability) {
            return 1;
        }
    }
    return 0;
}
extern "C" u32 THUMB_BRANCH_SAFESTACK_ServerControl_AddConditionCheckFail(ServerFlow* serverFlow, BattleMon* defendingMon, BattleMon* attackingMon, CONDITION condition, ConditionData condData, u8 overrideMode, u32 almost) {
    u32 failStatus = AddConditionCheckFailOverwrite(serverFlow, defendingMon, condition, condData, overrideMode);
    
    // Ignore poison immunity if the Pokémon has Corrotion
    if (condition == CONDITION_POISON &&
        failStatus == 2 &&
        BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY) == ABIL212_CORROSION) {
        failStatus = 0;
    }
    // Make any status fail if the Pokémon has Comatose
    if ((condition < CONDITION_CONFUSION || condition == CONDITION_YAWN) &&
        defendingMon->currentAbility == ABIL213_COMATOSE) {
        failStatus = 3;
    }
    
    if (condition == CONDITION_GASTROACID && 
        (defendingMon->conditions[condition].status) == 0) {

        switch (failStatus) {
        case 0:
            // Gastro Acid fails agains certain abilities
            if (IsAbilityImmuneToGastroAcid(BattleMon_GetValue(defendingMon, VALUE_ABILITY))) {
                failStatus = 3;
            }
            break;
        case 1:
            // Gastro Acid should not fail if the condition is already detected 
            // only because the Pokémon is affected by Neutralizing Gas
            failStatus = 0;
            break;
        }
    }

    if (failStatus) {
        if (almost) {
            AddConditionCheckFailStandard(serverFlow, defendingMon, failStatus, condition);
        }
        return 1;
    }
    else {
        u32 HEID = HEManager_PushState(&serverFlow->HEManager);
        u32 failFlag = ServerEvent_MoveConditionCheckFail(serverFlow, attackingMon, defendingMon, condition);
        // Added new state to force the fail
        if ((failFlag && almost) || failFlag == FORCE_FAIL_MESSAGE) {
            ServerEvent_AddConditionFailed(serverFlow, defendingMon, attackingMon, condition);
            // This disables the default fail message
            serverFlow->field_78A |= 0x10u;
        }
        HEManager_PopState(&serverFlow->HEManager, HEID);
        return failFlag;
    }
}

extern "C" b32 THUMB_BRANCH_BattleMon_CheckIfMoveCondition(BattleMon* battleMon, CONDITION condition) {
    if (condition == CONDITION_SLEEP && BattleMon_GetValue(battleMon, VALUE_EFFECTIVE_ABILITY) == ABIL213_COMATOSE) {
        return 1;
    }

    if (condition == CONDITION_GASTROACID && 
        BattleField_GetNeutralizingGasMons() &&
        AbilityCanBeNeutralized(BattleMon_GetValue(battleMon, VALUE_ABILITY))) {
        return 1;
    }

    return (battleMon->conditions[condition].status) != 0;
}

extern "C" void THUMB_BRANCH_HandlerHex(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, defendingSlot);
        if (BattleMon_CheckIfMoveCondition(defendingMon, CONDITION_PARALYSIS) ||
            BattleMon_CheckIfMoveCondition(defendingMon, CONDITION_SLEEP) ||
            BattleMon_CheckIfMoveCondition(defendingMon, CONDITION_FREEZE) ||
            BattleMon_CheckIfMoveCondition(defendingMon, CONDITION_BURN) ||
            BattleMon_CheckIfMoveCondition(defendingMon, CONDITION_POISON) ||
            BattleMon_CheckIfMoveCondition(defendingMon, CONDITION_PARALYSIS)) {

            u32 power = BattleEventVar_GetValue(VAR_MOVE_POWER);
            BattleEventVar_RewriteValue(VAR_MOVE_POWER, power * 2);
        }
    }
}

extern "C" void THUMB_BRANCH_CommonStatusReaction(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, CONDITION condition) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    if (currentMon->currentAbility == ABIL213_COMATOSE) {
        return;
    }
    
    CONDITION conditionCpy = condition;
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)
        && (!BattleEventVar_GetValueIfExist(VAR_ITEM_REACTION, &conditionCpy) || conditionCpy == CONDITION_FREEZE || conditionCpy == CONDITION_NONE)
        && CommonConditionCodeMatch(serverFlow, pokemonSlot, condition)) {
        ItemEvent_PushRun(item, serverFlow, pokemonSlot);
    }
}

extern "C" void THUMB_BRANCH_HandlerThrash(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // Ignore this effect if used by Dancer
        if (extraActionFlag) {
            MoveEvent_ForceRemoveItemFromBattleMon(currentMon, item->subID);
            return;
        }
        if (!BattleMon_CheckIfMoveCondition(currentMon, CONDITION_MOVELOCK) && !work[6]) {
            u32 maxTurns = (BattleRandom(2u) + 2);
        
            HandlerParam_AddCondition* addCondition;
            addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
            addCondition->condition = CONDITION_MOVELOCK;
            addCondition->condData = Condition_MakeTurnParam(maxTurns, item->subID);
            addCondition->almost = 0;
            addCondition->pokeID = pokemonSlot;
            BattleHandler_PopWork(serverFlow, addCondition);
            work[6] = 1;
            *work = maxTurns;
        }
    }
}

extern "C" void THUMB_BRANCH_HandlerThrashEnd(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {

    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) && work[6]) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        b32 finished = 0;
        // Don't count Dancer uses of the move
        if (*work && !extraActionFlag) {
            --*work;
        }
        if (!BattleEventVar_GetValue(VAR_GENERAL_USE_FLAG)) {
            HandlerParam_CureCondition* cureCondition;
            cureCondition = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
            cureCondition->condition = CONDITION_MOVELOCK;
            cureCondition->pokeCount = 1;
            cureCondition->pokeID[0] = pokemonSlot;
            BattleHandler_PopWork(serverFlow, cureCondition);
        
            finished = 1;
        }
        if (!*work)
        {
            HandlerParam_AddCondition* addCondition;
            addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
            addCondition->condition = CONDITION_CONFUSION;
            MakeCondition(CONDITION_CONFUSION, currentMon, &addCondition->condData);
            addCondition->reserved = 1;
            addCondition->pokeID = pokemonSlot;
            BattleHandler_StrSetup(&addCondition->exStr, 2u, 360u);
            BattleHandler_AddArg(&addCondition->exStr, pokemonSlot);
            BattleHandler_PopWork(serverFlow, addCondition);

            finished = 1;
        }
        if (finished) {
            MoveEvent_ForceRemoveItemFromBattleMon(currentMon, item->subID);
        }
    }
}

ABILITY IgnoreBaitAbilities[] = {
    ABIL239_PROPELLER_TAIL,
    ABIL242_STALWART,
};
extern "C" b32 AbilityIgnoresBait(ABILITY ability) {
    for (u16 i = 0; i < ARRAY_COUNT(IgnoreBaitAbilities); ++i) {
        if (IgnoreBaitAbilities[i] == ability) {
            return 1;
        }
    }
    return 0;
}
extern "C" void THUMB_BRANCH_HandlerFollowMeBaitTarget(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    BattleStyle battleStyle = BtlSetup_GetBattleStyle(serverFlow->mainModule);
    if (battleStyle != BTL_STYLE_SINGLE &&
        battleStyle != BTL_STYLE_ROTATION) {

        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
        // Propeller Tail & Stallwart ignore redirection
        if (AbilityIgnoresBait(BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY))) {
            return;
        }

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (!MainModule_IsAllyMonID(pokemonSlot, attackingSlot)
            && BattleEventVar_GetValue(VAR_MOVE_ID) != MOVE507_SKY_DROP
            && !BattleMon_CheckIfMoveCondition(currentMon, CONDITION_SKYDROP)
            && !Handler_IsMonSwitchOutInterrupted(serverFlow)) {
        
            u16 moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
            if (!Handler_IsTargetInRange(serverFlow, attackingSlot, pokemonSlot, moveID)) {
                BattleEventVar_RewriteValue(VAR_DEFENDING_MON, pokemonSlot);
            }
        }
    }
}

extern "C" b32 THUMB_BRANCH_BattleHandler_SwapPoke(ServerFlow* serverFlow, HandlerParam_SwapPoke* params) {
    if (params->pokeID1 == params->pokeID2) {
        return 0;
    }
    u32 clientID1 = MainModule_PokeIDToClientID(params->pokeID1);
    if (clientID1 != MainModule_PokeIDToClientID(params->pokeID2)) {
        return 0;
    }

    BattleMon* battleMon1 = PokeCon_GetBattleMon(serverFlow->pokeCon, params->pokeID1);
    BattleMon* battleMon2 = PokeCon_GetBattleMon(serverFlow->pokeCon, params->pokeID2);
    if (BattleMon_IsFainted(battleMon1) || BattleMon_IsFainted(battleMon2)) {
        return 0;
    }

    BattleParty* battleParty = PokeCon_GetBattleParty(serverFlow->pokeCon, clientID1);
    int partyMon1 = BattleParty_FindPartyMon(battleParty, battleMon1);
    int partyMon2 = BattleParty_FindPartyMon(battleParty, battleMon2);
    if (partyMon1 < 0 || partyMon2 < 0) {
        return 0;
    }
    ServerControl_MoveCore(serverFlow, clientID1, partyMon1, partyMon2, 0);
    BattleHandler_SetString(serverFlow, &params->exStr);
    ServerControl_AfterMove(serverFlow, clientID1, partyMon1, partyMon2);

    // Added an event
    ServerEvent_SwapPoke(serverFlow, battleMon1, battleMon2);
    return 1;
}

extern "C" b32 THUMB_BRANCH_CommonRedirectMove(ServerFlow* serverFlow, u32 pokemonSlot, u32* work, POKE_TYPE pokeType) {
    u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
    if (attackingSlot != pokemonSlot && pokeType == BattleEventVar_GetValue(VAR_MOVE_TYPE)) {

        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
        if (AbilityIgnoresBait(BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY))) {
            return 0;
        }

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (!Handler_IsTargetInRange(serverFlow, attackingSlot, pokemonSlot, moveID) && !IsComboMove(moveID)) {

            u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
            if (BattleEventVar_RewriteValue(VAR_DEFENDING_MON, pokemonSlot) && defendingSlot != pokemonSlot)
            {
                return 1;
            }
        }
    }
    return 0;
}

extern "C" u32 CommonLeppaBerryLastMoveID(ServerFlow* serverFlow, u32 pokemonSlot);
extern "C" u32 CommonLeppaBerryEnableMoveID(ServerFlow* serverFlow, u32 pokemonSlot);

extern "C" void THUMB_BRANCH_HandlerLeppaBerryCommon(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work, b32 isTemp) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

    u32 moveSlot = CommonLeppaBerryLastMoveID(serverFlow, pokemonSlot);
    if (moveSlot == 4) {

        moveSlot = CommonLeppaBerryEnableMoveID(serverFlow, pokemonSlot);
        if (moveSlot == 4 && isTemp) {
            moveSlot = 0;

            u32 moveCount = BattleMon_GetMoveCount(currentMon);
            if (moveCount) {
                while (!Move_GetMissingPP(currentMon, moveSlot)) {
                    if (++moveSlot >= moveCount) {
                        return;
                    }
                }
            }
        }
    }

    if (moveSlot < 4)
    {
        HandlerParam_PP* pp;
        pp = (HandlerParam_PP*)BattleHandler_PushWork(serverFlow, EFFECT_RESTORE_PP, pokemonSlot);
        pp->volume = 10;
        // Double the PP gained if Ripen is active
        if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL247_RIPEN) {
            pp->volume *= 2;
        }
        pp->pokeID = pokemonSlot;
        pp->moveSlot = moveSlot;
        pp->flags = pp->flags & 0xFE | 1;
        BattleHandler_StrSetup(&pp->exStr, 2u, 911u);
        BattleHandler_AddArg(&pp->exStr, pokemonSlot);
        BattleHandler_AddArg(&pp->exStr, item->subID);
        MOVE_ID moveID = Move_GetID(currentMon, moveSlot);
        BattleHandler_AddArg(&pp->exStr, moveID);
        BattleHandler_PopWork(serverFlow, pp);
    }
}

extern "C" void THUMB_BRANCH_HandlerOranBerryUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerParam_RecoverHP* recoverHP;
        recoverHP = (HandlerParam_RecoverHP*)BattleHandler_PushWork(serverFlow, EFFECT_RECOVER_HP, pokemonSlot);
        recoverHP->pokeID = pokemonSlot;
        recoverHP->recoverHP = CommonGetItemParam(item, ITSTAT_USE_PARAM);
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // Double the HP gained if Ripen is active
        if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL247_RIPEN) {
            recoverHP->recoverHP *= 2;
        }
        BattleHandler_StrSetup(&recoverHP->exStr, 2u, 908u);
        BattleHandler_AddArg(&recoverHP->exStr, pokemonSlot);
        BattleHandler_AddArg(&recoverHP->exStr, item->subID);
        BattleHandler_PopWork(serverFlow, recoverHP);
    }
}

extern "C" void THUMB_BRANCH_HandlerSitrusBerryUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        u32 maxHP = BattleMon_GetValue(currentMon, VALUE_MAX_HP);
        u32 healPercent = CommonGetItemParam(item, ITSTAT_USE_PARAM);
        // Double the HP gained if Ripen is active
        if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL247_RIPEN) {
            healPercent *= 2;
        }

        HandlerParam_RecoverHP* recoverHP;
        recoverHP = (HandlerParam_RecoverHP*)BattleHandler_PushWork(serverFlow, EFFECT_RECOVER_HP, pokemonSlot);
        recoverHP->pokeID = pokemonSlot;
        recoverHP->recoverHP = maxHP * healPercent / 100u;
        BattleHandler_StrSetup(&recoverHP->exStr, 2u, 908u);
        BattleHandler_AddArg(&recoverHP->exStr, pokemonSlot);
        BattleHandler_AddArg(&recoverHP->exStr, item->subID);
        BattleHandler_PopWork(serverFlow, recoverHP);
    }
}

extern "C" s8 doesNatureAffectStat(PartyPkm* pPkm, u32 stat);
extern "C" void THUMB_BRANCH_SAFESTACK_CommonPinchBerry(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work, u32 natureType) {
    if (!*work && pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        u32 healPercent = CommonGetItemParam(item, ITSTAT_USE_PARAM);
        
        HandlerParam_RecoverHP* recoverHP;
        recoverHP = (HandlerParam_RecoverHP*)BattleHandler_PushWork(serverFlow, EFFECT_RECOVER_HP, pokemonSlot);
        recoverHP->pokeID = pokemonSlot;
        recoverHP->recoverHP = DivideMaxHPZeroCheck(currentMon, healPercent);
        // Double the HP gained if Ripen is active
        if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL247_RIPEN) {
            recoverHP->recoverHP *= 2;
        }
        BattleHandler_StrSetup(&recoverHP->exStr, 2u, 908u);
        BattleHandler_AddArg(&recoverHP->exStr, pokemonSlot);
        BattleHandler_AddArg(&recoverHP->exStr, item->subID);
        BattleHandler_PopWork(serverFlow, recoverHP);

        PartyPkm* partyPkm = BattleMon_GetSrcData(currentMon);
        if (doesNatureAffectStat(partyPkm, natureType) == -1)
        {
            HandlerParam_AddCondition* addCondition;
            addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
            addCondition->pokeID = pokemonSlot;
            addCondition->condition = CONDITION_CONFUSION;
            MakeCondition(CONDITION_CONFUSION, currentMon, &addCondition->condData);
            BattleHandler_PopWork(serverFlow, addCondition);
        }
        *work = 1;
    }
}

extern "C" void THUMB_BRANCH_CommonStatBoostBerry(ServerFlow* serverFlow, u32 pokemonSlot, StatStage stat, u8 volume) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->stat = stat;
        statChange->volume = volume;
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // Double the stat boost if Ripen is active
        if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL247_RIPEN) {
            statChange->volume *= 2;
        }
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}

extern "C" void THUMB_BRANCH_HandlerEnigmaBerryUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        u32 healPercent = CommonGetItemParam(item, ITSTAT_USE_PARAM);
        // Double the HP gained if Ripen is active
        if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL247_RIPEN) {
            healPercent *= 2;
        }
        
        HandlerParam_RecoverHP* recoverHP; 
        recoverHP = (HandlerParam_RecoverHP*)BattleHandler_PushWork(serverFlow, EFFECT_RECOVER_HP, pokemonSlot);
        recoverHP->pokeID = pokemonSlot;
        recoverHP->recoverHP = DivideMaxHPZeroCheck(currentMon, healPercent);
        BattleHandler_PopWork(serverFlow, recoverHP);
    }
}

extern "C" void THUMB_BRANCH_SAFESTACK_CommonResistBerry(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work, u8 pokeType, b32 skipEffectivenessCheck) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && pokeType == BattleEventVar_GetValue(VAR_MOVE_TYPE)
        && (skipEffectivenessCheck || BattleEventVar_GetValue(VAR_TYPE_EFFECTIVENESS) > 3)) {

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (!BattleMon_IsSubstituteActive(currentMon)) {

            u32 ratio = 2048;
            // Double the resistence if Ripen is active
            if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL247_RIPEN) {
                ratio /= 2;
            }

            BattleEventVar_MulValue(VAR_RATIO, ratio);
            if (!Handler_IsSimulationMode(serverFlow)) {
                *work = 1;
            }
        }
    }
}

extern "C" void THUMB_BRANCH_HandlerJabocaRowapBerryUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) && *work) {
        u32 attackingSlot = *work - 1;
        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
        if (!BattleMon_IsFainted(attackingMon)) {
            u32 damagePercent = CommonGetItemParam(item, ITSTAT_USE_PARAM);
            BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            // Double the damage if Ripen is active
            if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL247_RIPEN) {
                damagePercent /= 2;
            }
        
            HandlerParam_Damage* damage;
            damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
            damage->pokeID = attackingSlot;
            damage->damage = DivideMaxHPZeroCheck(attackingMon, damagePercent);
            BattleHandler_StrSetup(&damage->exStr, 2u, 1031u);
            BattleHandler_AddArg(&damage->exStr, damage->pokeID);
            BattleHandler_AddArg(&damage->exStr, pokemonSlot);
            BattleHandler_AddArg(&damage->exStr, item->subID);
            BattleHandler_PopWork(serverFlow, damage);
        }
    }
}

extern "C" u32 THUMB_BRANCH_ServerEvent_CheckDamageToRecover(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, MoveParam* moveParam) {
    BattleEventVar_Push();
    u32 attackingSlot = BattleMon_GetID(attackingMon);
    BattleEventVar_SetConstValue(VAR_ATTACKING_MON, attackingSlot);
    u32 defendingSlot = BattleMon_GetID(defendingMon);
    BattleEventVar_SetConstValue(VAR_DEFENDING_MON, defendingSlot);
    BattleEventVar_SetConstValue(VAR_MOVE_TYPE, moveParam->moveType);
    // Add move category
    BattleEventVar_SetConstValue(VAR_MOVE_CATEGORY, moveParam->category);
    BattleEventVar_SetRewriteOnceValue(VAR_GENERAL_USE_FLAG, 0);
    BattleEvent_CallHandlers(serverFlow, EVENT_CHECK_DAMAGE_TO_RECOVER);
    u32 generalFlag = BattleEventVar_GetValue(VAR_GENERAL_USE_FLAG);
    BattleEventVar_Pop();
    return generalFlag;
}

extern "C" void THUMB_BRANCH_ServerEvent_AfterSwitchInPrevious(ServerFlow* serverFlow) {
    // Add an event that triggers before every other switch in event
    ServerEvent_SwitchInPriority(serverFlow);

    BattleEventVar_Push();
    BattleEvent_CallHandlers(serverFlow, EVENT_SWITCH_IN_PREVIOUS);
    BattleEventVar_Pop();
}

extern "C" b32 ServerEvent_CheckProtectBreak(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, MOVE_ID moveID)
{
    BattleEventVar_Push();
    u32 attackingSlot = BattleMon_GetID(attackingMon);
    BattleEventVar_SetValue(VAR_ATTACKING_MON, attackingSlot);
    // Added the defending slot for fun, it's used in Unseen Fist but it's not needed
    u32 defendingSlot = BattleMon_GetID(defendingMon);
    BattleEventVar_SetValue(VAR_DEFENDING_MON, defendingSlot);
    // Added the Move ID so that Unseen Fist can check if it is contact
    BattleEventVar_SetValue(VAR_MOVE_ID, moveID);
    BattleEventVar_SetValue(VAR_GENERAL_USE_FLAG, 0);
    BattleEvent_CallHandlers(serverFlow, EVENT_CHECK_PROTECT_BREAK);
    b32 protectBroken = BattleEventVar_GetValue(VAR_GENERAL_USE_FLAG);
    BattleEventVar_Pop();
    return protectBroken;
}
extern "C" void THUMB_BRANCH_SAFESTACK_flowsub_CheckNoEffect_Protect(ServerFlow* serverFlow, u16* moveID, BattleMon* attackingMon, PokeSet* targetSet, int dmgAffRec) {
    PokeSet_SeekStart(targetSet);
    for (BattleMon* targetMon = PokeSet_SeekNext(targetSet); targetMon; targetMon = PokeSet_SeekNext(targetSet)) {
        if (!ServerControl_IsGuaranteedHit(serverFlow, attackingMon, targetMon)
            && ServerControl_CheckNoEffectCore(serverFlow, moveID, attackingMon, targetMon, dmgAffRec, EVENT_REDIRECT_TARGETEND)) {
            PokeSet_Remove(targetSet, targetMon);
        }
    }
    PokeSet_SeekStart(targetSet);
    for (BattleMon* targetMon = PokeSet_SeekNext(targetSet); targetMon; targetMon = PokeSet_SeekNext(targetSet)) {
        if (ServerControl_CheckNoEffectCore(serverFlow, moveID, attackingMon, targetMon, dmgAffRec, EVENT_NOEFFECT_CHECK)) {
            PokeSet_Remove(targetSet, targetMon);
        }
    }
    if (getMoveFlag(*moveID, FLAG_BLOCKED_BY_PROTECT)) {
        PokeSet_SeekStart(targetSet);
        for (BattleMon* targetMon = PokeSet_SeekNext(targetSet); targetMon; targetMon = PokeSet_SeekNext(targetSet)) {
            // Added the modified ServerEvent_CheckProtectBreak
            if (BattleMon_GetTurnFlag(targetMon, TURNFLAG_PROTECT)
                && !ServerEvent_CheckProtectBreak(serverFlow, attackingMon, targetMon, *moveID)) {
                PokeSet_Remove(targetSet, targetMon);
                u32 targetSlot = BattleMon_GetID(targetMon);
                ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, SCID_SetMessage, 523, targetSlot, 0xFFFF0000);
            }
        }
    }
    PokeSet_SeekStart(targetSet);
    for (BattleMon* targetMon = PokeSet_SeekNext(targetSet); targetMon; targetMon = PokeSet_SeekNext(targetSet)) {
        if (ServerControl_CheckNoEffectCore(serverFlow, moveID, attackingMon, targetMon, dmgAffRec, EVENT_ABILITY_CHECK_NO_EFFECT)) {
            PokeSet_Remove(targetSet, targetMon);
        }
    }
}

extern "C" b32 THUMB_BRANCH_BattleHandler_AbilityPopupAdd(ServerFlow* serverFlow, HandlerParam_Header* params) {
    u32 battleSlot = params->flags << 19 >> 27;
    if (!DoesBattleMonExist(&serverFlow->posPoke, battleSlot)) {
        return 0;
    }

    // Add the forced ability to the server call
    ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_AbilityPopupIn, battleSlot | asOneAbilityPopUpFlags);
    // Clear the variable for the client
    asOneAbilityPopUpFlags = 0;
    return 1;
}

extern "C" u32 THUMB_BRANCH_ServerCommand_AbilityPopupIn(BtlClientWk* client, u32* state, u32* data) {
    u32 battleSlot = *data & 0x1F;
    u32 battlePos = MainModule_PokeIDToPokePos(client->mainModule, client->pokeCon, battleSlot);
    if (*state) {
        if (*state != 1) {
            return 1;
        }

        if (!BattleViewCmd_AbilityPopup_DisplayWait(client->btlvCore, battlePos)) {
            return 0;
        }
    }
    else {
        if (IsChapterSkipMode(client)) {
            return 1;
        }

        // Set the forced ability for the server
        asOneAbilityPopUpFlags = *data & 0xE0;
        BattleViewCmd_AbilityPopup_DisplayStart(client->btlvCore, battlePos, 1);
    }
    ++(*state);
    return 0;
}

extern "C" u32 THUMB_BRANCH_LINK_AbilityPopup_DisplayStart_0x18(BattleMon* battleMon, BattleMonValue value) {
    ABILITY ability = BattleMon_GetValue(battleMon, value);

    if (asOneAbilityPopUpFlags) {
        if (asOneAbilityPopUpFlags == AS_ONE_FIRST_ABILITY_FLAG) {
            ability = ABIL127_UNNERVE;
        } 
        else if (asOneAbilityPopUpFlags == AS_ONE_SECOND_ABILITY_FLAG) {
            if (ability == ABIL266_AS_ONE) {
                ability = ABIL264_CHILLING_NEIGH;
            }
            else if (ability == ABIL267_AS_ONE) {
                ability = ABIL265_GRIM_NEIGH;
            }
        }

        // Clear the variable for the server
        asOneAbilityPopUpFlags = 0;
    }

    return ability;
}

extern "C" void THUMB_BRANCH_ServerControl_UnnerveAction(ServerFlow* serverFlow, BattleMon* battleMon) {
    ABILITY ability = BattleMon_GetValue(battleMon, VALUE_EFFECTIVE_ABILITY);
    if (ability == ABIL127_UNNERVE ||
        ability == ABIL266_AS_ONE ||
        ability == ABIL267_AS_ONE) {

        FRONT_POKE_SEEK_WORK frontSet[4];
        FRONT_POKE_SEEK_InitWork(frontSet, serverFlow);

        u32 battleSlot = BattleMon_GetID(battleMon);
        BattleMon* frontMon;
        while (FRONT_POKE_SEEK_GetNext(frontSet, serverFlow, &frontMon)) {
            u32 frontSlot = BattleMon_GetID(frontMon);
            if (!MainModule_IsAllyMonID(battleSlot, frontSlot)) {
                ServerControl_CheckItemReaction(serverFlow, frontMon, 0);
            }
        }
    }
}

ABILITY DontChangeFormOnSwitchOutAbilities[] = {
    ABIL210_BATTLE_BOND,
    ABIL209_DISGUISE,
    ABIL248_ICE_FACE,
    ABIL211_POWER_CONSTRUCT,
    ABIL278_ZERO_TO_HERO,
    ABIL307_TERA_SHIFT,
    ABIL308_TERA_SHELL,
    ABIL309_TERAFORM_ZERO,
};
extern "C" b32 AbilityDontChangeFormOnSwitchOut(ABILITY ability) {
    for (u16 i = 0; i < ARRAY_COUNT(DontChangeFormOnSwitchOutAbilities); ++i) {
        if (DontChangeFormOnSwitchOutAbilities[i] == ability) {
            return 1;
        }
    }
    return 0;
}
extern "C" void THUMB_BRANCH_BattleMon_ClearForSwitchOut(BattleMon* battleMon) {
    sys_memset(battleMon->turnFlag, 0, 2u);
    BattleMon_ClearFormChange(battleMon);
    MoveWork_ClearSurface(battleMon);
    BattleMon_ClearUsedMoveFlag(battleMon);
    ClearCounter(battleMon);
    BattleMon_ClearComboMoveData(battleMon);
    BattleMon_IllusionBreak(battleMon);
    if (!BattleMon_GetConditionFlag(battleMon, CONDITIONFLAG_BATONPASS))
    {
        BattleMon_RemoveSubstitute(battleMon);
        ClearMoveStatusWork(battleMon, 0);
        ResetStatStages(&battleMon->statStageParam);
        sys_memset(battleMon->conditionFlag, 0, 2u);
    }
    if (!AbilityDontChangeFormOnSwitchOut(battleMon->currentAbility)) {
        battleMon->form = battleMon->flags & 0x1F;
    }
    battleMon->currentAbility = battleMon->ability;
    PokeParty_SetParam(battleMon->partySrc, PF_Forme, battleMon->form);
    PokeParty_RecalcStats(battleMon->partySrc);
}

extern "C" void THUMB_BRANCH_ServerFlow_CheckPokeHideAvoid(ServerFlow* serverFlow, u16* moveID, BattleMon* attackingMon, PokeSet* targetSet) {
    PokeSet_SeekStart(targetSet);
    for (BattleMon* currentMon = PokeSet_SeekNext(targetSet); currentMon; currentMon = PokeSet_SeekNext(targetSet)) {

        // Always miss if the Pokémon is invulnerable because of Commander
        b32 isCommander = (!BattleMon_CheckIfMoveCondition(currentMon, CONDITION_CHARGELOCK) &&
            BattleMon_GetHideCondition(currentMon) == CONDITIONFLAG_SHADOWFORCE);
        if (isCommander) {
            PokeSet_Remove(targetSet, currentMon);

            // Skip message when a non-targeted move is avoided
            MoveTarget moveTarget = (MoveTarget)PML_MoveGetParam((MOVE_ID)*moveID, MVDATA_TARGET);
            if (moveTarget <= TARGET_ENEMY_SELECT || !isCommander) {
                ServerDisplay_MoveAvoid(serverFlow, currentMon);
            }

            continue;
        }
        

        if (!ServerControl_IsGuaranteedHit(serverFlow, attackingMon, currentMon)) {
            if (ServerEvent_CheckHiding(serverFlow, attackingMon, currentMon)) {
                PokeSet_Remove(targetSet, currentMon);
                ServerDisplay_MoveAvoid(serverFlow, currentMon);
            }
        }
    }
}

extern "C" void THUMB_BRANCH_LINK_ServerFlow_ClearPokeNoActionFlag_0x34(ServerFlow* serverFlow, BattleMon* battleMon, CONDITION_FLAG conditionFlag) {
    b32 isCommander = (!BattleMon_CheckIfMoveCondition(battleMon, CONDITION_CHARGELOCK) &&
        BattleMon_GetHideCondition(battleMon) == CONDITIONFLAG_SHADOWFORCE);
    if (!isCommander) {
        ServerDisplay_ResetConditionFlag(serverFlow, battleMon, conditionFlag);
    }
}

extern "C" u32 THUMB_BRANCH_ServerEvent_CheckForceSwitch(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon) {
    BattleEventVar_Push();
    BattleEventVar_SetRewriteOnceValue(VAR_MOVE_FAIL_FLAG, 0);
    u32 defendingSlot = BattleMon_GetID(defendingMon);
    BattleEventVar_SetConstValue(VAR_DEFENDING_MON, defendingSlot);
    BattleEvent_CallHandlers(serverFlow, EVENT_CHECK_FORCE_SWITCH);
    Condition_CheckForceSwitchFail(serverFlow, defendingMon);
    u32 failFlag = BattleEventVar_GetValue(VAR_MOVE_FAIL_FLAG);
    BattleEventVar_Pop();
    // Fail to force a switch if the Pokémon is a Dondozo paired by Commander
    if (BattleField_IsDondozoPaired(defendingMon->battleSlot)) {
        failFlag = 1;
    }
    return failFlag;
}

extern "C" b32 SideEvent_AddItem(u32 side, SIDE_EFFECT sideEffect, ConditionData condData);
extern "C" b32 THUMB_BRANCH_BattleHandler_AddSideEffect(ServerFlow* serverFlow, HandlerParam_AddSideEffect* params) {
    u8 side = params->side;
    ConditionData condData = params->condData;
    u32 pokemonSlot = params->header.flags << 19 >> 27;

    ServerEvent_CheckSideEffectParam(serverFlow, pokemonSlot, params->sideEffect, side, &condData);
    if (!SideEvent_AddItem(params->side, params->sideEffect, condData)) {
        return 0;
    }
    BattleMon* battleMon = PokeCon_GetBattleMon(serverFlow->pokeCon, params->header.flags << 19 >> 27);
    if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0) {
        ServerDisplay_AbilityPopupAdd(serverFlow, battleMon);

        u32 pokePos = MainModule_PokeIDToPokePos(serverFlow->mainModule, serverFlow->pokeCon, pokemonSlot);
        switch (params->sideEffect) {
        case SIDEEFF_TOXIC_SPIKES:
            ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_MoveAnim, pokePos, 0, MOVE390_TOXIC_SPIKES, 0, 0);
            break;
        }
    }
    BattleHandler_SetString(serverFlow, &params->exStr);
    if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0) {
        ServerDisplay_AbilityPopupRemove(serverFlow, battleMon);
    }
    return 1;
}

#endif // EXPAND_ABILITIES

#if EXPAND_ABILITIES || EXPAND_ITEMS

// Stat boost extra flags, allows to distinguish if a stat boost was caused by Intimidate or Mirror Armor
extern "C" b32 THUMB_BRANCH_SAFESTACK_ServerEvent_CheckStatStageChangeSuccess(ServerFlow * serverFlow, BattleMon * affectedMon, StatStage statStage, u32 attackingSlot, int volume, u32 moveSerial) {
    BattleEventVar_Push();
    u32 affectedSlot = BattleMon_GetID(affectedMon);
    BattleEventVar_SetConstValue(VAR_MON_ID, affectedSlot);
    BattleEventVar_SetConstValue(VAR_ATTACKING_MON, attackingSlot);
    BattleEventVar_SetConstValue(VAR_MOVE_EFFECT, statStage);
    BattleEventVar_SetConstValue(VAR_VOLUME, volume);
    BattleEventVar_SetConstValue(VAR_INTIMIDATE_FLAG, (moveSerial & STAT_CHANGE_INTIMIDATE_FLAG) != 0); // Intimidate Flag
    BattleEventVar_SetConstValue(VAR_MIRROR_ARMOR_FLAG, (moveSerial & STAT_CHANGE_MIRROR_ARMOR_FLAG) != 0); // Mirror Armor Flag
    BattleEventVar_SetConstValue(VAR_OPPORTUNIST_FLAG, (moveSerial & STAT_CHANGE_OPPORTUNIST_FLAG) != 0); // Opportunist Flag
    BattleEventVar_SetConstValue(VAR_MOVE_SERIAL, (moveSerial & 0x0FFFFFFF));
    BattleEventVar_SetRewriteOnceValue(VAR_MOVE_FAIL_FLAG, 0);
    BattleEvent_CallHandlers(serverFlow, EVENT_STAT_STAGE_CHANGE_LAST_CHECK);
    u32 failFlag = BattleEventVar_GetValue(VAR_MOVE_FAIL_FLAG);
    BattleEventVar_Pop();

    if (!failFlag) {
        return 1;
    }
    return 0;
}

extern "C" void THUMB_BRANCH_ServerEvent_StatStageChangeFail(ServerFlow * serverFlow, BattleMon * currentMon, u32 moveSerial) {
    BattleEventVar_Push();
    u32 currentSlot = BattleMon_GetID(currentMon);
    BattleEventVar_SetConstValue(VAR_MON_ID, currentSlot);
    BattleEventVar_SetConstValue(VAR_INTIMIDATE_FLAG, (moveSerial & STAT_CHANGE_INTIMIDATE_FLAG) != 0); // Intimidate Flag
    BattleEventVar_SetConstValue(VAR_MIRROR_ARMOR_FLAG, (moveSerial & STAT_CHANGE_MIRROR_ARMOR_FLAG) != 0); // Mirror Armor Flag
    BattleEventVar_SetConstValue(VAR_OPPORTUNIST_FLAG, (moveSerial & STAT_CHANGE_OPPORTUNIST_FLAG) != 0); // Opportunist Flag
    BattleEventVar_SetConstValue(VAR_MOVE_SERIAL, moveSerial & 0x0FFFFFFF);
    BattleEvent_CallHandlers(serverFlow, EVENT_STAT_STAGE_CHANGE_FAIL);
    BattleEventVar_Pop();
}

extern "C" void THUMB_BRANCH_HandlerIntimidate(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        u16 frontPokeExist = Handler_GetExistFrontPokePos(serverFlow, pokemonSlot);
        u8* frontSlots = Handler_GetTempWork(serverFlow);
        u32 frontCount = Handler_ExpandPokeID(serverFlow, frontPokeExist | 0x100, frontSlots);
        if (frontCount) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            HandlerParam_ChangeStatStage* statChange;
            statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
            statChange->stat = STATSTAGE_ATTACK;
            statChange->volume = -1;
            statChange->moveAnimation = 1;
            statChange->pokeCount = frontCount;
            for (u8 frontCurrent = 0; frontCurrent < frontCount; ++frontCurrent) {
                statChange->pokeID[frontCurrent] = frontSlots[frontCurrent];
            }
            // Set the intimidate flag
            statChange->pad = STAT_CHANGE_INTIMIDATE_FLAG;
            BattleHandler_PopWork(serverFlow, statChange);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}

#endif

#if EXPAND_ABILITIES || EXPAND_MOVES

extern "C" TypeEffectiveness THUMB_BRANCH_SAFESTACK_ServerEvent_CheckDamageEffectiveness(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, PokeType moveType, u8 pokemonType) {
    BattleEventVar_Push();
    u32 attackingSlot = BattleMon_GetID(attackingMon);
    BattleEventVar_SetConstValue(VAR_ATTACKING_MON, attackingSlot);
    u32 defendingSlot = BattleMon_GetID(defendingMon);
    BattleEventVar_SetConstValue(VAR_DEFENDING_MON, defendingSlot);
    BattleEventVar_SetConstValue(VAR_POKE_TYPE, pokemonType);
    BattleEventVar_SetConstValue(VAR_MOVE_TYPE, moveType);
    BattleEventVar_SetRewriteOnceValue(VAR_NO_TYPE_EFFECTIVENESS, 0);
    BattleEventVar_SetRewriteOnceValue(VAR_SET_TYPE_EFFECTIVENESS, 0);
    Condition_CheckUnaffectedByType(serverFlow, defendingMon);
    BattleEvent_CallHandlers(serverFlow, EVENT_CHECK_TYPE_EFFECTIVENESS);
    b32 overrideImmunity = BattleEventVar_GetValue(VAR_NO_TYPE_EFFECTIVENESS);
    u32 overrideEffectiveness = BattleEventVar_GetValue(VAR_SET_TYPE_EFFECTIVENESS);
    BattleEventVar_Pop();
        
    TypeEffectiveness effectiveness = GetTypeEffectiveness(moveType, pokemonType);
    if (effectiveness != EFFECTIVENESS_IMMUNE) {
        switch (overrideEffectiveness) {
        case 1:
            return EFFECTIVENESS_1;
        case 2:
            return EFFECTIVENESS_1_2;
        case 3:
            return EFFECTIVENESS_2;
        }
    }
    else if (overrideImmunity) {
        return EFFECTIVENESS_1;
    }
    return effectiveness;
}

#endif // EXPAND_ABILITIES || EXPAND_MOVES

// WARNING BattleHandler_ChangeAbility is used in BattleUpgrade.S