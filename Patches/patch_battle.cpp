#include "custom/settings.h"
#include "custom/battle_expansion.h"
#include "custom/item_expansion.h"

#include "custom/level_cap.h"

extern "C" 
{

// GROUPS
// --- DATA ---

ITEM_ID ItemsThatPreventStatusMoveUse[] = {
    IT0640_ASSAULT_VEST,
};
struct NoContactItem
{
    ITEM_ID itemID;
    MoveFlag filter;
};
NoContactItem ItemsThatPreventContact[] = {
    {IT0880_PROTECTIVE_PADS, (MoveFlag)0xFF},
    {IT1884_PUNCHING_GLOVE, FLAG_PUNCH},
};
ITEM_ID ItemsThatPreventHazardEffects[] = {
    IT1120_HEAVY_DUTY_BOOTS,
};
ITEM_ID ItemsThatGuaranteeMultiHit[] = {
    IT1886_LOADED_DICE,
};
ITEM_ID ItemsThatPreventMoldbreaker[] = {
    IT1881_ABILITY_SHIELD,
};
ITEM_ID ItemsThatWeatherEffects[] = {
    IT1123_UTILITY_UMBRELLA,
};

struct PowderMove
{
    MoveID moveID;
    bool selfTargeting;
};
PowderMove PowderMoves[] = {
    {MOVE147_SPORE, false},
    {MOVE178_COTTON_SPORE, false},
    {MOVE078_STUN_SPORE, false},
    // POWDER
    {MOVE476_RAGE_POWDER, true},
    // MAGIC_POWDER
    {MOVE077_POISON_POWDER, false},
    {MOVE079_SLEEP_POWDER, false},
};

struct ModifyItemMove
{
    MoveID moveID;
    bool onlyBerries;
};
ModifyItemMove MovesThatModifyItem[] = {
    {MOVE168_THIEF, false},
    {MOVE271_TRICK, false},
    {MOVE282_KNOCK_OFF, false},
    {MOVE343_COVET, false},
    {MOVE365_PLUCK, true},
    {MOVE415_SWITCHEROO, false},
    {MOVE450_BUG_BITE, true},
    {MOVE510_INCINERATE, true},
    {MOVE516_BESTOW, false},
    // {CORROSIVE_GAS, false},
    // {TEATIME, true},
};

MoveID SoundBasedMoves[] // MoveFlag::FLAG_SOUND
{
    MOVE045_GROWL,
    MOVE046_ROAR,
    MOVE047_SING,
    MOVE048_SUPERSONIC,
    MOVE103_SCREECH,
    MOVE173_SNORE,
    MOVE195_PERISH_SONG,
    MOVE215_HEAL_BELL,
    MOVE253_UPROAR,
    MOVE304_HYPER_VOICE,
    MOVE319_METAL_SOUND,
    MOVE320_GRASS_WHISTLE,
    MOVE336_HOWL,
    MOVE405_BUG_BUZZ,
    MOVE448_CHATTER,
    MOVE496_ROUND,
    MOVE497_ECHOED_VOICE,
    MOVE547_RELIC_SONG,
    MOVE555_SNARL,
    MOVE568_NOBLE_ROAR,
    MOVE574_DISARMING_VOICE,
    MOVE575_PARTING_SHOT,
    MOVE586_BOOMBURST,
    MOVE590_CONFIDE,
    //SPARKILING_ARIA,
    //CLANGING_SCALES,
    //CLANGOROUS_SOULBLAZE,
    //CLANGOROUS_SOUL,
    //OVERDRIVE,
    //EERIE_SPELL,
    //TORCH_SONG,
    //ALLURING_VOICE,
    //PSYCHIC_NOISE,
};

MoveID PunchingMoves[] // MoveFlag::FLAG_PUNCH
{
    MOVE004_COMET_PUNCH,
    MOVE005_MEGA_PUNCH,
    MOVE007_FIRE_PUNCH,
    MOVE008_ICE_PUNCH,
    MOVE009_THUNDER_PUNCH,
    MOVE146_DIZZY_PUNCH,
    MOVE183_MACH_PUNCH,
    MOVE223_DYNAMIC_PUNCH,
    MOVE264_FOCUS_PUNCH,
    MOVE309_METEOR_MASH,
    MOVE325_SHADOW_PUNCH,
    MOVE327_SKY_UPPERCUT,
    MOVE359_HAMMER_ARM,
    MOVE409_DRAIN_PUNCH,
    MOVE418_BULLET_PUNCH,
    MOVE612_POWER_UP_PUNCH
    // ICE_HAMMER,
    // PLASMA_FIST,
    // DOUBLE_IRON_BASH,
    // WICKED_BLOW,
    // SURGING_STRIKES,
    // HEADLONG_RUSH,
    // JET_PUNCH,
    // RAGE_FIST,
};

// --- FUNCITOMS ---

bool IsPowderMove(MoveID moveID, bool excludeSelfTarget)
{
    for (u16 i = 0; i < ARRAY_COUNT(PowderMoves); ++i)
        if (moveID == PowderMoves[i].moveID)
        {
            if (!excludeSelfTarget || !PowderMoves[i].selfTargeting)
                return true;
            else
                break;
        }
    return false;
}
bool IsModifyItemMove(MoveID moveID, ItemID heldItem)
{
    for (u16 i = 0; i < ARRAY_COUNT(MovesThatModifyItem); ++i)
        if (moveID == MovesThatModifyItem[i].moveID)
        {
            if (!MovesThatModifyItem[i].onlyBerries || PML_ItemIsBerry(heldItem))
                return true;
            else
                break;
        }
    return false;
}

bool CommonItemListCheck(ItemID itemID, ITEM_ID* itemList, int listSize)
{
    if (BattleField_CheckEffect(FLDEFF_MAGIC_ROOM))
        return false;

    for (u16 i = 0; i < listSize; ++i)
        if (itemID == itemList[i])
            return true;
    return false;
}
bool DoesItemPreventStatusMoveUse(ItemID itemID)
{
    return CommonItemListCheck(itemID, ItemsThatPreventStatusMoveUse, ARRAY_COUNT(ItemsThatPreventStatusMoveUse));
}
bool DoesItemPreventContact(ItemID itemID, MoveID moveID)
{
    if (BattleField_CheckEffect(FLDEFF_MAGIC_ROOM))
        return false;

    for (u16 i = 0; i < ARRAY_COUNT(ItemsThatPreventContact); ++i)
        if (itemID == ItemsThatPreventContact[i].itemID)
        {
            if (ItemsThatPreventContact[i].filter == 0xFF)
                return true;

            return getMoveFlag(moveID, ItemsThatPreventContact[i].filter);
        }
    return false;
}
bool DoesItemPreventHazardEffects(ItemID itemID)
{
    return CommonItemListCheck(itemID, ItemsThatPreventHazardEffects, ARRAY_COUNT(ItemsThatPreventHazardEffects));
}
bool DoesItemGuaranteeMultiHit(ItemID itemID)
{
    return CommonItemListCheck(itemID, ItemsThatGuaranteeMultiHit, ARRAY_COUNT(ItemsThatGuaranteeMultiHit));
}
bool DoesItemPreventMoldbreaker(ItemID itemID)
{
    return CommonItemListCheck(itemID, ItemsThatPreventMoldbreaker, ARRAY_COUNT(ItemsThatPreventMoldbreaker));
}
bool DoesItemPreventWeatherEffects(ItemID itemID)
{
    return CommonItemListCheck(itemID, ItemsThatWeatherEffects, ARRAY_COUNT(ItemsThatWeatherEffects));
}

void SetIntimidateFlag(bool intimidate)
{
    EventWorkSave* eventWork = GameData_GetEventWork(GAME_DATA);
    if (intimidate)
    {
        EventWork_FlagSet(eventWork, INTIMIDATE_FLAG);
    }
    else
    {
        EventWork_FlagReset(eventWork, INTIMIDATE_FLAG);
    }
}
bool GetIntimidateFlag()
{
    EventWorkSave* eventWork = GameData_GetEventWork(GAME_DATA);
    return EventWork_FlagGet(eventWork, INTIMIDATE_FLAG);
}
bool IsIntimidated(AbilID attackerAbility)
{
    if (GetIntimidateFlag() && attackerAbility == ABIL022_INTIMIDATE)
        return true;
    return false;
}

// FIELD EFFECT EXPANSION
struct BattleFieldExt
{
    int terrain;
    int terrainTurns;
    void* battleEventItems[8];
    ConditionData conditionData[8];
    u32 turnCount[8];
    u32 dependPokeID[8][6];
    u32 dependPokeCount[8];
    u32 effectEnableFlags[8];
};
BattleFieldExt battleFieldExt;
#define BATTLEFIELD_EXT (BattleField*)&battleFieldExt

int BattleField_GetTerrain()
{
    return battleFieldExt.terrain;
}
void BattleField_SetTerrain(int terrain, int turns)
{
    battleFieldExt.terrain = terrain;
    battleFieldExt.terrainTurns = turns;
}
void BattleField_EndTerrain()
{
    battleFieldExt.terrain = 0;
    battleFieldExt.terrainTurns = 0;
}
int BattleField_GetTerrainTurns()
{
    return battleFieldExt.terrainTurns;
}

int TerrainPowerMod(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, int terrain, int type)
{
    switch (terrain)
    {
    case TERRAIN_ELECTRIC:
        if (!ServerControl_CheckFloating(serverFlow, attackingMon, 1))
            if (type == TYPE_ELEC)
                return 5325;
        break;
    case TERRAIN_GRASSY:
        if (!ServerControl_CheckFloating(serverFlow, attackingMon, 1))
            if (type == TYPE_GRASS)
                return 5325;
        break;
    case TERRAIN_MISTY:
        if (!ServerControl_CheckFloating(serverFlow, defendingMon, 1)) // check the defender
            if (type == TYPE_DRAGON)
                return 2048;
        break;
    case TERRAIN_PSYCHIC:
        if (!ServerControl_CheckFloating(serverFlow, attackingMon, 1))
            if (type == TYPE_PSY)
                return 5325;
        break;
    }
    return 4096;
}

int ServerEvent_IncreaseMoveTerrainTurns(ServerFlow* serverFlow, int terrain, int attackingSlot)
{
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    BattleEventVar_SetConstValue(VAR_WEATHER, terrain);
    BattleEventVar_SetConstValue(NEW_VAR_ATTACKING_MON, attackingSlot);
    BattleEventVar_SetValue(VAR_EFFECT_TURN_COUNT, 0);
    BattleEvent_CallHandlers(serverFlow, EVENT_MOVE_TERRAIN_TURN_COUNT);
    int turnCount = BattleEventVar_GetValue(VAR_EFFECT_TURN_COUNT);
    BattleEventVar_Pop();
    return turnCount;
}
int ServerEvent_ChangeTerrain(ServerFlow* serverFlow, int terrain) 
{
    bool failFlag = 0;

    PokeSet* pokeSet = &serverFlow->pokeSet_1A68;
    j_PokeSet_SeekStart(pokeSet);
    for (BattleMon* currentMon = j_PokeSet_SeekNext(pokeSet); currentMon; currentMon = j_PokeSet_SeekNext(pokeSet))
    {
        int v4 = HEManager_PushState((unsigned int*)&serverFlow->HEManager);

        if (!BattleMon_IsFainted(currentMon))
        {
            BattleEventVar_Push();
            SET_UP_NEW_EVENT;
            int currentSlot = BattleMon_GetID(currentMon);
            BattleEventVar_SetConstValue(NEW_VAR_MON_ID, currentSlot);
            BattleEventVar_SetConstValue(VAR_WEATHER, terrain);
            BattleEventVar_SetRewriteOnceValue(VAR_MOVE_FAIL_FLAG, 0);
            BattleEvent_CallHandlers(serverFlow, EVENT_TERRAIN_CHANGE);
            failFlag = BattleEventVar_GetValue(VAR_MOVE_FAIL_FLAG);
            if (failFlag)
            {
                BattleEvent_CallHandlers(serverFlow, EVENT_TERRAIN_CHANGE_FAIL);
            }
            BattleEventVar_Pop();
        }

        HEManager_PopState((unsigned int*)&serverFlow->HEManager, v4);
    }

    return !failFlag;
}
void ServerEvent_ChangeTerrainAfter(ServerFlow* serverFlow, int terrain)
{
    PokeSet* pokeSet = &serverFlow->pokeSet_1A68;
    j_PokeSet_SeekStart(pokeSet);
    for (BattleMon* currentMon = j_PokeSet_SeekNext(pokeSet); currentMon; currentMon = j_PokeSet_SeekNext(pokeSet))
    {
        int v4 = HEManager_PushState((unsigned int*)&serverFlow->HEManager);

        if (!BattleMon_IsFainted(currentMon))
        {
            BattleEventVar_Push();
            SET_UP_NEW_EVENT;
            int currentSlot = BattleMon_GetID(currentMon);
            BattleEventVar_SetConstValue(NEW_VAR_MON_ID, currentSlot);
            BattleEventVar_SetConstValue(VAR_WEATHER, terrain);
            BattleEvent_CallHandlers(serverFlow, EVENT_AFTER_TERRAIN_CHANGE);
            BattleEventVar_Pop();
        }

        HEManager_PopState((unsigned int*)&serverFlow->HEManager, v4);
    }
}
int ServerEvent_GetTerrain(ServerFlow* serverFlow)
{
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    BattleEventVar_SetRewriteOnceValue(VAR_MOVE_FAIL_FLAG, 0);
    BattleEvent_CallHandlers(serverFlow, EVENT_TERRAIN_CHECK);
    int failFlag = BattleEventVar_GetValue(VAR_MOVE_FAIL_FLAG);
    BattleEventVar_Pop();
    if (failFlag)
        return 0;
    return BattleField_GetTerrain();
}
void ServerEvent_GroundedByGravity(ServerFlow* serverFlow, BattleMon* battleMon)
{
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    int pokemonSlot = BattleMon_GetID(battleMon);
    BattleEventVar_SetConstValue(NEW_VAR_MON_ID, pokemonSlot);
    BattleEvent_CallHandlers(serverFlow, EVENT_GROUNDED_BY_GRAVITY);
    BattleEventVar_Pop();
}

bool ServerControl_ChangeTerrainCheck(ServerFlow* serverFlow, int terrain, int turns)
{
    if (terrain > TERRAIN_PSYCHIC)
        return 0;

    return terrain != BattleField_GetTerrain() || turns == 255 && BattleField_GetTerrainTurns() != 255;
}
int ServerControl_ChangeTerrain(ServerFlow* serverFlow, int terrain, int turns)
{
    if (!ServerControl_ChangeTerrainCheck(serverFlow, terrain, turns))
        return 0;

    if (!ServerEvent_ChangeTerrain(serverFlow, terrain))
        return 0;

    BattleField_SetTerrain(terrain, turns);
    return 1;
}

BattleEventHandlerTableEntry* EventAddFieldWeather(int* a1)
{
    *a1 = 1;
    return (BattleEventHandlerTableEntry*)0x0689D7C0;
}
BattleEventHandlerTableEntry* EventAddFieldTrickRoom(int* a1)
{
    *a1 = 1;
    return (BattleEventHandlerTableEntry*)0x0689D7B8;
}
BattleEventHandlerTableEntry* EventAddFieldGravity(int* a1)
{
    *a1 = 2;
    return (BattleEventHandlerTableEntry*)0x0689D7C8;
}
BattleEventHandlerTableEntry* EventAddFieldImprison(int* a1)
{
    *a1 = 1;
    return (BattleEventHandlerTableEntry*)0x0689D7A0;
}
BattleEventHandlerTableEntry* EventAddFieldWaterSport(int* a1)
{
    *a1 = 1;
    return (BattleEventHandlerTableEntry*)0x0689D7A8;
}
BattleEventHandlerTableEntry* EventAddFieldMudSport(int* a1)
{
    *a1 = 1;
    return (BattleEventHandlerTableEntry*)0x0689D798;
}
BattleEventHandlerTableEntry* EventAddFieldWonderRoom(int* a1)
{
    *a1 = 1;
    return (BattleEventHandlerTableEntry*)0x0689D7B0;
}
BattleEventHandlerTableEntry* EventAddFieldMagicRoom(int* a1)
{
    *a1 = 1;
    return (BattleEventHandlerTableEntry*)0x0689D790;
}


// GENERAL TERRAIN
void HandlerTerrainPreventStatus(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    int terrain = BattleField_GetTerrain();
    if (terrain != TERRAIN_ELECTRIC && terrain != TERRAIN_MISTY)
        return;

    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_DEFENDING_MON));
    if (!ServerControl_CheckFloating(serverFlow, currentMon, 1))
    {
        if (terrain == TERRAIN_ELECTRIC)
        {
            if (BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_SLEEP ||
                BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_YAWN)
            {
                *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
            }
        }
        else // MISTY TERRAIN
        {
            if (BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_PARALYSIS ||
                BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_SLEEP ||
                BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_FREEZE ||
                BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_BURN ||
                BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_POISON ||
                BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_CONFUSION ||
                BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_YAWN)
            {
                *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
            }
        }
    }
}
void HandlerTerrainStatusFailMessage(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    int terrain = BattleField_GetTerrain();
    if (terrain != TERRAIN_ELECTRIC && terrain != TERRAIN_MISTY)
        return;

    if (*work)
    {
        short msgID = BATTLE_ELECTRIC_TERRAIN_STATUS_MSGID;
        if (terrain == TERRAIN_MISTY)
            msgID = BATTLE_MISTY_TERRAIN_STATUS_MSGID;

        int defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        HandlerParam_Message* v6 = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, 31);
        BattleHandler_StrSetup(&v6->str, 2u, msgID);
        BattleHandler_AddArg(&v6->str, defendingSlot);
        BattleHandler_PopWork(serverFlow, v6);
    }
}
// ELECTRIC TERRAIN
void CommonElectricTerrainCureStatus(ServerFlow* serverFlow, int pokemonSlot)
{
    HandlerParam_CureCondition* v7;

    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_SLEEP))
    {
        if (!ServerControl_CheckFloating(serverFlow, currentMon, 1))
        {
            v7 = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
            v7->sickCode = CONDITION_SLEEP;
            v7->poke_cnt = 1;
            v7->pokeID[0] = pokemonSlot;
            DPRINT("TERRAIN CURE\n");
            BattleHandler_PopWork(serverFlow, v7);
        }
    }
}
void HandlerElectricTerrainCheckSleep(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    DPRINT("HandlerElectricTerrainEndSleep\n");
    if (BattleField_GetTerrain() != TERRAIN_ELECTRIC)
        return;

    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (!ServerControl_CheckFloating(serverFlow, currentMon, 1))
        {
            BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
        }
    }
}
void HandlerElectricTerrainTerrainChange(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (IS_NOT_NEW_EVENT)
        return;

    if (BattleField_GetTerrain() != TERRAIN_ELECTRIC)
        return;

    CommonElectricTerrainCureStatus(serverFlow, BattleEventVar_GetValue(NEW_VAR_MON_ID));
}
void HandlerElectricTerrainSwitchIn(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (BattleField_GetTerrain() != TERRAIN_ELECTRIC)
        return;

    CommonElectricTerrainCureStatus(serverFlow, BattleEventVar_GetValue(VAR_MON_ID));
}
void HandlerElectricTerrainActProcEnd(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (BattleField_GetTerrain() != TERRAIN_ELECTRIC)
        return;

    CommonElectricTerrainCureStatus(serverFlow, BattleEventVar_GetValue(VAR_MON_ID));
}
void HandlerElectricTerrainFloatingChange(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (BattleField_GetTerrain() != TERRAIN_ELECTRIC)
        return;

    CommonElectricTerrainCureStatus(serverFlow, BattleEventVar_GetValue(VAR_MON_ID));
}
void HandlerElectricTerrainFloatingChangeNEW(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (IS_NOT_NEW_EVENT)
        return;

    if (BattleField_GetTerrain() != TERRAIN_ELECTRIC)
        return;

    CommonElectricTerrainCureStatus(serverFlow, BattleEventVar_GetValue(NEW_VAR_MON_ID));
}
// GRASSY TERRAIN
void HandlerGrassyTerrainHeal(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (BattleField_GetTerrain() != TERRAIN_GRASSY)
        return;

    HandlerParam_RecoverHP* v6;

    int currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
    if (!ServerControl_CheckFloating(serverFlow, currentMon, 1))
    {
        if (!BattleMon_IsFullHP(currentMon))
        {
            v6 = (HandlerParam_RecoverHP*)BattleHandler_PushWork(serverFlow, EFFECT_RECOVER_HP, currentSlot);
            v6->pokeID = currentSlot;
            v6->recoverHP = DivideMaxHPZeroCheck(currentMon, 16);
            BattleHandler_StrSetup(&v6->exStr, 2u, 914);
            BattleHandler_AddArg(&v6->exStr, currentSlot);
            int SubID = BattleEventItem_GetSubID(a1);
            BattleHandler_AddArg(&v6->exStr, SubID);
            BattleHandler_PopWork(serverFlow, v6);
        }
    }
}
void HandlerGrassyTerrainQuakeMoves(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (BattleField_GetTerrain() != TERRAIN_GRASSY)
        return;

    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON))
    {
        int moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (moveID == MOVE089_EARTHQUAKE || 
            moveID == MOVE222_MAGNITUDE || 
            moveID == MOVE523_BULLDOZE)
        {
            BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            if (!ServerControl_CheckFloating(serverFlow, defendingMon, 1))
            {
                int power = BattleEventVar_GetValue(VAR_MOVE_POWER);
                BattleEventVar_RewriteValue(VAR_MOVE_POWER, power / 2);
            }
        }
    }
}
// MISTY TERRAIN (all in general)
// PSYCHIC TERRAIN
void HandlerPsychicTerrainPreventPrio(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (BattleField_GetTerrain() != TERRAIN_PSYCHIC)
        return;

    int currentMon = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    DPRINTF("PREVENT PRIO -> TERRAIN: %d | ATTACKER: %d | DEFENDER: %d\n", BattleField_GetTerrain(), BattleEventVar_GetValue(VAR_ATTACKING_MON), currentMon);
    if (currentMon == BattleEventVar_GetValue(VAR_ATTACKING_MON))
        return;

    BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, currentMon);
    if (!ServerControl_CheckFloating(serverFlow, defendingMon, 1))
    {
        ActionOrderWork* action_order = serverFlow->actionOrderWork;
        u16 orderIdx = 0;
    
        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_ATTACKING_MON));
        for (; orderIdx < 6; ++orderIdx)
            if (action_order[orderIdx].battleMon == attackingMon)
                break;
    
        int priority = (action_order[orderIdx].field_8 >> 16) & 0x3FFFFF;
        priority -= 7;
        DPRINTF("PRIO: %d\n", priority);
        int special_priority = ((action_order[orderIdx].field_8 >> 13) & 0x7); // special priority takes into account item & ability prio boosts (1 = no added prio)
        special_priority -= 1;
        DPRINTF("SPECIAL PRIO: %d\n", special_priority);
        priority += special_priority;
    
        if (priority > 0)
            BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);
    }
}
BattleEventHandlerTableEntry FieldTerrainHandlers[] = {
    // GENERAL TERRAIN
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
    // MISTY TERRAIN (all in general)
    // PSYCHIC TERRAIN
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerPsychicTerrainPreventPrio},
};
BattleEventHandlerTableEntry* EventAddFieldTerrain(int* a1)
{
    *a1 = ARRAY_COUNT(FieldTerrainHandlers);
    return FieldTerrainHandlers;
}

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
#if ADD_NEW_ITEMS
    {(FieldEffect)FLDEFF_TERRAIN, EventAddFieldTerrain},
#endif
};

#if ADD_NEW_ITEMS

BattleEventItem* THUMB_BRANCH_j_j_FieldEffectEvent_AddItem(FieldEffect fieldEffect, int a2, ConditionData conditionData, int a4)
{
    int enventFuncAmount[5];
    enventFuncAmount[1] = a4;

    for (u16 fieldEffectIdx = 0; fieldEffectIdx < ARRAY_COUNT(FieldEffectEventAddTableExt); ++fieldEffectIdx)
    {
        if (fieldEffect == FieldEffectEventAddTableExt[fieldEffectIdx].effect)
        {
            BattleEventHandlerTableEntry* battleEventEntry = FieldEffectEventAddTableExt[fieldEffectIdx].func(enventFuncAmount);
            BattleEventItem* result = BattleEvent_AddItem(EVENTITEM_FIELD, fieldEffect, EVENTPRI_FIELD_DEFAULT, 0, a2, battleEventEntry, enventFuncAmount[0]);
            return result;
        }
    }
    return nullptr;
}

void THUMB_BRANCH_BattleField_InitCore(BattleField* battleField, Weather weather)
{
    for (int currentEffect = FLDEFF_WEATHER; currentEffect < DEFAULT_FIELD_EFFECT_AMOUNT; ++currentEffect)
    {
        BattleField_ClearFactorWork(battleField, (FieldEffect)currentEffect);
    }
    battleField->weather = weather;
    battleField->weatherTurns = 255;

    for (int currentEffect = FLDEFF_TERRAIN; currentEffect < FIELD_EFFECT_AMOUNT; ++currentEffect)
    {
        BattleField_ClearFactorWork(battleField, (FieldEffect)currentEffect);
    }
    battleFieldExt.terrain = 0; // TODO: Pre set terrain in battle, maybe... if its done in this function
    battleFieldExt.terrainTurns = 255;
}

void THUMB_BRANCH_BattleField_ClearFactorWork(BattleField* battleField, FieldEffect fieldEffect)
{
    BattleField* currBattleField = battleField;
    FieldEffect currFieldEffect = fieldEffect;
    if (fieldEffect >= DEFAULT_FIELD_EFFECT_AMOUNT)
    {
        currBattleField = BATTLEFIELD_EXT;
        currFieldEffect = (FieldEffect)(fieldEffect - DEFAULT_FIELD_EFFECT_AMOUNT);
    }

    currBattleField->battleEventItems[currFieldEffect] = 0;

    currBattleField->conditionData[currFieldEffect] = Condition_MakeNull();
    currBattleField->turnCount[currFieldEffect] = 0;
    currBattleField->dependPokeCount[currFieldEffect] = 0;
    currBattleField->effectEnableFlags[currFieldEffect] = 0;

    int currentPoke = 0;
    while (currentPoke < 6)
    {
        currBattleField->dependPokeID[currFieldEffect][currentPoke] = 31;
        ++currentPoke;
    }
}

int THUMB_BRANCH_BattleField_AddEffectCore(BattleField* battleField, FieldEffect fieldEffect, ConditionData conditionData, int a4)
{
    BattleField* currBattleField = battleField;
    FieldEffect currFieldEffect = fieldEffect;
    if (fieldEffect >= DEFAULT_FIELD_EFFECT_AMOUNT)
    {
        currBattleField = BATTLEFIELD_EXT;
        currFieldEffect = (FieldEffect)(fieldEffect - DEFAULT_FIELD_EFFECT_AMOUNT);
    }

    if (currBattleField->effectEnableFlags[currFieldEffect])
        return 0;

    if (a4)
    {
        BattleEventItem* effectEvent = j_j_FieldEffectEvent_AddItem(fieldEffect, 0, conditionData, a4);
        currBattleField->battleEventItems[currFieldEffect] = effectEvent;
        if (!effectEvent)
            return 0;
    }

    currBattleField->effectEnableFlags[currFieldEffect] = 1;
    currBattleField->conditionData[currFieldEffect] = conditionData;
    currBattleField->turnCount[currFieldEffect] = 0;
    currBattleField->dependPokeCount[currFieldEffect] = 0;

    int currentPoke = 0;
    while (currentPoke < 6)
    {
        currBattleField->dependPokeID[currFieldEffect][currentPoke] = 31;
        ++currentPoke;
    }

    int pokemonSlot = Condition_GetMonID(conditionData);
    if (pokemonSlot != 31)
    {
        BattleField_AddDependPokeCore(battleField, fieldEffect, pokemonSlot);
    }
    return 1;
}

int THUMB_BRANCH_BattleField_RemoveEffectCore(BattleField* battleField, FieldEffect fieldEffect)
{
    if (!BattleField_CheckFieldEffectCore(battleField, fieldEffect))
        return 0;

    BattleField* currBattleField = battleField;
    FieldEffect currFieldEffect = fieldEffect;
    if (fieldEffect >= DEFAULT_FIELD_EFFECT_AMOUNT)
    {
        currBattleField = BATTLEFIELD_EXT;
        currFieldEffect = (FieldEffect)(fieldEffect - DEFAULT_FIELD_EFFECT_AMOUNT);
    }

    BattleEventItem* battleEventItems = currBattleField->battleEventItems[currFieldEffect];
    if (battleEventItems)
    {
        BattleEventItem_Remove(battleEventItems);
        currBattleField->battleEventItems[currFieldEffect] = 0;
    }
    BattleField_ClearFactorWork(battleField, fieldEffect);

    return 1;
}

int THUMB_BRANCH_BattleField_AddDependPokeCore(BattleField* battleField, FieldEffect fieldEffect, int pokemonSlot)
{
    BattleField* currBattleField = battleField;
    FieldEffect currFieldEffect = fieldEffect;
    if (fieldEffect >= DEFAULT_FIELD_EFFECT_AMOUNT)
    {
        currBattleField = BATTLEFIELD_EXT;
        currFieldEffect = (FieldEffect)(fieldEffect - DEFAULT_FIELD_EFFECT_AMOUNT);
    }

    if (BattleField_CheckFieldEffectCore(battleField, fieldEffect))
    {
        u32 pokeCount = currBattleField->dependPokeCount[currFieldEffect];
        if (pokeCount < 6)
        {
            int currentPoke = 0;
            if (!pokeCount)
            {
                currBattleField->dependPokeID[currFieldEffect][pokeCount] = pokemonSlot;
                currBattleField->dependPokeCount[currFieldEffect]++;
                return 1;
            }
            while (pokemonSlot != currBattleField->dependPokeID[currFieldEffect][currentPoke])
            {
                if (++currentPoke >= pokeCount)
                {
                    currBattleField->dependPokeID[currFieldEffect][pokeCount] = pokemonSlot;
                    currBattleField->dependPokeCount[currFieldEffect]++;
                    return 1;
                }
            }
        }
    }

    return 0;
}

void THUMB_BRANCH_BattleField_RemoveDependPokeEffectCore(BattleField* battleField, int pokemonSlot)
{
    for (int currentEffect = FLDEFF_WEATHER; currentEffect < 8; ++currentEffect)
    {
        if (BattleField_CheckFieldEffectCore(battleField, (FieldEffect)currentEffect))
        {
            if (battleField->dependPokeCount[currentEffect])
            {
                int currentPoke = 0;
                while (pokemonSlot != battleField->dependPokeID[currentEffect][currentPoke]) // find slot to remove
                {
                    if (++currentPoke >= battleField->dependPokeCount[currentEffect])
                    {
                        return;
                    }
                }

                for (; currentPoke < 5; ++currentPoke) // move back slots after removed slot by one
                {
                    battleField->dependPokeID[currentEffect][currentPoke] = battleField->dependPokeID[currentEffect][currentPoke + 1];
                }
                battleField->dependPokeID[currentEffect][currentPoke] = 31; // set last slot to null
                --battleField->dependPokeCount[currentEffect]; // remove 1 from the total count

                u32 pokeCount = battleField->dependPokeCount[currentEffect];
                if (pokeCount == 0) // remove field effect if the count is empty
                {
                    BattleEventItem* battleEventItems = battleField->battleEventItems[currentEffect];
                    if (battleEventItems)
                    {
                        BattleEventItem_Remove(battleEventItems);
                        battleField->battleEventItems[currentEffect] = 0;
                    }
                    BattleField_ClearFactorWork(battleField, (FieldEffect)currentEffect);
                }
                else
                {
                    int condPokemonSlot = Condition_GetMonID(battleField->conditionData[currentEffect]);
                    if (pokemonSlot == condPokemonSlot) // if the condition mon id is the one removed, set it to the first available mon
                    {
                        Condition_SetMonID(&battleField->conditionData[currentEffect], battleField->dependPokeID[currentEffect][0]);
                    }
                }
            }
        }
    }
}

u32 THUMB_BRANCH_BattleField_CheckFieldEffectCore(BattleField* battleField, FieldEffect fieldEffect)
{
    if (fieldEffect >= DEFAULT_FIELD_EFFECT_AMOUNT)
        return battleFieldExt.effectEnableFlags[fieldEffect - DEFAULT_FIELD_EFFECT_AMOUNT];

    return battleField->effectEnableFlags[fieldEffect];
}

bool serverCommandFlag = false;
int THUMB_BRANCH_ServerCommand_TurnCheckField(int a1)
{
    serverCommandFlag = true;
    BattleField_TurnCheckCore(*(BattleField**)(a1 + 52), 0, 0);
    serverCommandFlag = false;
    return 1;
}
void THUMB_BRANCH_BattleField_TurnCheckCore(BattleField* battleField, int(* callback)(FieldEffect, ServerFlow*), ServerFlow* serverFlow)
{
    for (int currentEffect = 1; currentEffect < FIELD_EFFECT_AMOUNT; ++currentEffect)
    {
        if (BattleField_CheckFieldEffectCore(battleField, (FieldEffect)currentEffect))
        {
            BattleField* currBattleField = battleField;
            FieldEffect currFieldEffect = (FieldEffect)currentEffect;
            if (currentEffect >= DEFAULT_FIELD_EFFECT_AMOUNT)
            {
                currBattleField = BATTLEFIELD_EXT;
                currFieldEffect = (FieldEffect)(currentEffect - DEFAULT_FIELD_EFFECT_AMOUNT);
            }

            u32 turnMax = Condition_GetTurnMax(currBattleField->conditionData[currFieldEffect]);
            if (turnMax)
            {
                u32 turnCount = currBattleField->turnCount[currFieldEffect];
                if (currentEffect < DEFAULT_FIELD_EFFECT_AMOUNT || !serverCommandFlag)
                    turnCount += 1;

                currBattleField->turnCount[currFieldEffect] = turnCount;
                if (currentEffect == FLDEFF_TERRAIN)
                {
                    if (battleFieldExt.terrainTurns == 255)
                        continue; // ignore turns if terrain is permanent

                    battleFieldExt.terrainTurns = turnCount; // update terrain turns
                }
                
                if (turnCount >= turnMax)
                {
                    if (currentEffect == FLDEFF_TERRAIN)
                    {
                        BattleField_EndTerrain(); // remove terrain data
                        ServerEvent_ChangeTerrain(serverFlow, TERRAIN_NULL);
                    }

                    BattleEventItem* events = currBattleField->battleEventItems[currFieldEffect];
                    if (events)
                    {
                        BattleEventItem_Remove(events);
                        currBattleField->battleEventItems[currFieldEffect] = 0;
                    }
                    BattleField_ClearFactorWork(battleField, (FieldEffect)currentEffect);
                    if (callback)
                    {
                        callback((FieldEffect)currentEffect, serverFlow);
                    }
                }
            }
        }
    }
}

int THUMB_BRANCH_BattleHandler_AddFieldEffect(ServerFlow* serverFlow, HandlerParam_AddFieldEffect* params)
{
    u8 prevTerrain = TERRAIN_NULL;
    u8 terrain = params->field_D;
    int pokemonSlot = params->header.flags << 19 >> 27;

    if (params->effect == FLDEFF_TERRAIN)
    {
        prevTerrain = BattleField_GetTerrain();
        u8 turns = Condition_GetTurnMax(params->cond);

        u8 extraTurns = ServerEvent_IncreaseMoveTerrainTurns(serverFlow, terrain, pokemonSlot);
        if (extraTurns)
        {
            turns += extraTurns;
            params->cond = Condition_MakeTurn(turns);
        }

        if (!ServerControl_ChangeTerrain(serverFlow, terrain, turns))
        {
            return 0;
        }
    }

    if (!prevTerrain && !ServerControl_FieldEffectCore(serverFlow, params->effect, params->cond, params->fAddDependPoke))
    {
        return 0;
    }

    if (params->effect == FLDEFF_TERRAIN)
    {
        int moveID = 0;
        switch (params->field_D)
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
        int pokePos = MainModule_PokeIDToPokePos(serverFlow->mainModule, serverFlow->pokeCon, pokemonSlot);
        ServerDisplay_AddCommon(serverFlow->serverCommandQueue, 48, pokePos, pokePos, moveID, 0, 0);
    }

    BattleHandler_SetString(serverFlow, &params->exStr);

    if (params->effect == FLDEFF_TERRAIN)
    {
        ServerEvent_ChangeTerrainAfter(serverFlow, terrain);
    }

    return 1;
}

int THUMB_BRANCH_BattleHandler_RemoveFieldEffect(ServerFlow* serverFlow, HandlerParam_RemoveFieldEffect* params)
{
    if (params->effect == FLDEFF_TERRAIN)
    {
        BattleField_EndTerrain();
    }
    if (!BattleField_RemoveEffect(params->effect))
    {
        return 0;
    }
    ServerControl_FieldEffectEnd(serverFlow, params->effect);
    return 1;
}

void THUMB_BRANCH_ServerControl_FieldEffectEnd(ServerFlow* serverFlow, BattleFieldEffect fieldEffect)
{
    int msgID = -1;
    DPRINTF("END FIELD EFFECT: %d\n", fieldEffect);
    if (fieldEffect <= FLDEFF_TERRAIN)
    {
        switch (fieldEffect)
        {
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
    DPRINTF("MSG ID: %d\n", msgID);
    if (msgID >= 0)
    {
        ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, 90, msgID, 0xFFFF0000);
    }
    ServerDisplay_AddCommon(serverFlow->serverCommandQueue, 36, fieldEffect);
    if (fieldEffect == EFFECT_MAGIC_ROOM)
    {
        ServerControl_SortBySpeed(serverFlow, serverFlow->PokeSetTemp);
        j_PokeSet_SeekStart(serverFlow->PokeSetTemp);
        for (BattleMon* currentMon = j_PokeSet_SeekNext(serverFlow->PokeSetTemp);
            currentMon;
            currentMon = j_PokeSet_SeekNext(serverFlow->PokeSetTemp))
        {
            if (BattleMon_CanBattle(currentMon))
            {
                ServerControl_CheckItemReaction(serverFlow, currentMon, 0);
            }
        }
    }
}

int THUMB_BRANCH_BattleHandler_GravityCheck(ServerFlow* serverFlow, HandlerParam_Header* params)
{
    u8 pokemonIDs[8];
    short pokePos = MainModule_PokeIDToPokePos(serverFlow->mainModule, serverFlow->pokeCon, params->flags << 19 >> 27);
    int pokeCount = Handler_ExpandPokeID(serverFlow, pokePos | 0x800, pokemonIDs);
    for (int i = 0; i < pokeCount; i = ++i)
    {
        BattleMon* battleMon = PokeCon_GetBattleMon(serverFlow->pokeCon, pokemonIDs[i]);
        bool grounded = false;
        if (BattleMon_GetConditionFlag(battleMon, CONDITIONFLAG_FLY))
        {
            ServerControl_HideTurnCancel((int)serverFlow, battleMon, 3);
            grounded = true;
        }
        if (ServerEvent_CheckFloating(serverFlow, battleMon, 1))
        {
            grounded = true;
        }
        if (BattleMon_CheckIfMoveCondition(battleMon, CONDITION_FLOATING))
        {
            ServerControl_CureCondition(serverFlow, battleMon, CONDITION_FLOATING, 0);
            grounded = true;
        }
        if (BattleMon_CheckIfMoveCondition(battleMon, CONDITION_TELEKINESIS))
        {
            ServerControl_CureCondition(serverFlow, battleMon, CONDITION_TELEKINESIS, 0);
            grounded = true;
        }
        if (grounded)
        {
            ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, 91, 1083, pokemonIDs[i], -65536);
            DPRINT("GRAVITY TEXT\n");
            ServerEvent_GroundedByGravity(serverFlow, battleMon);
        }
    }
    return 1;
}
#endif

// SIDE EFFECT EXPANSION
// Accessing the Sife Effect functions crashed the game so I rewrote the whole system
// Thats why I branch the 2 access points to create a Side Effect BattleItem (BattleHandler_AddSideEffect & ItemEffect_StatusGuard) 
// and build from there
BattleSideManager SideStatusExt;
u8 removeSideEffExtFlags;

int BattleSideStatus_GetCountFromBattleEventItem(BattleEventItem* a1, unsigned int currentSide)
{
    int effect = BattleEventItem_GetSubID(a1);
    if (currentSide >= 2)
    {
        return 0;
    }
    else
    {
        if (effect <= DEFAULT_SIDE_CONDITION_AMOUNT)
            return SideStatus.Sides[currentSide].Conditions[effect].Count;
        else
            return SideStatusExt.Sides[currentSide].Conditions[effect - DEFAULT_SIDE_CONDITION_AMOUNT].Count;
    }
}

BattleEventHandlerTableEntry* EventAddSideReflect(int* a1)
{
    *a1 = 1;
    return (BattleEventHandlerTableEntry*)0x689D878;
}
BattleEventHandlerTableEntry* EventAddSideLightScreen(int* a1)
{
    *a1 = 1;
    return (BattleEventHandlerTableEntry*)0x689D898;
}
BattleEventHandlerTableEntry* EventAddSideSafeguard(int* a1)
{
    *a1 = 2;
    return (BattleEventHandlerTableEntry*)0x689D8D0;
}
BattleEventHandlerTableEntry* EventAddSideMist(int* a1)
{
    *a1 = 2;
    return (BattleEventHandlerTableEntry*)0x689D8E0;
}
BattleEventHandlerTableEntry* EventAddSideTailwind(int* a1)
{
    *a1 = 1;
    return (BattleEventHandlerTableEntry*)0x689D8C0;
}
BattleEventHandlerTableEntry* EventAddSideLuckyChant(int* a1)
{
    *a1 = 1;
    return (BattleEventHandlerTableEntry*)0x689D8B8;
}
// Heavy-Duty Boots hazard checks
void HandlerSideSpikes(BattleEventItem* a1, ServerFlow* serverFlow, int currentSide, int* work)
{
    HandlerParam_Damage* v9;

    int currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if (currentSide == GetSideFromMonID(currentSlot) && !Handler_CheckFloating(serverFlow, currentSlot))
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
        if (DoesItemPreventHazardEffects(BattleMon_GetHeldItem(currentMon))) // Heavy-Duty Boots check
            return;

        int spikeLayers = BattleSideStatus_GetCountFromBattleEventItem(a1, currentSide); // New Definition
        int dmgPercent = 0;
        switch (spikeLayers)
        {
        case 1:
            dmgPercent = 8;
            break;
        case 2:
            dmgPercent = 6;
            break;
        case 3:
            dmgPercent = 4;
            break;
        }

        v9 = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, 31);
        v9->pokeID = currentSlot;
        v9->damage = DivideMaxHPZeroCheck(currentMon, dmgPercent);
        BattleHandler_StrSetup(&v9->exStr, 2u, 851);
        BattleHandler_AddArg(&v9->exStr, currentSlot);
        BattleHandler_PopWork(serverFlow, v9);
    }
}
BattleEventHandlerTableEntry SideSpikesHandlers[] = {
    {EVENT_SWITCH_IN, HandlerSideSpikes},
};
BattleEventHandlerTableEntry* EventAddSideSpikes(int* a1)
{
    *a1 = 1;
    return SideSpikesHandlers;
}
// Heavy-Duty Boots hazard checks
void HandlerSideToxicSpikes(BattleEventItem* a1, ServerFlow* serverFlow, int currentSide, int* work)
{
    HandlerParam_RemoveSideEffect* v7; // r1
    HandlerParam_RemoveSideEffect* v9; // r0
    HandlerParam_AddCondition* v10; // r4

    int currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if (currentSide == GetSideFromMonID(currentSlot) && !Handler_CheckFloating(serverFlow, currentSlot))
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
        if (BattleMon_HasType(currentMon, TYPE_POISON))
        {
            v7 = (HandlerParam_RemoveSideEffect*)BattleHandler_PushWork(serverFlow, EFFECT_REMOVE_SIDE_EFFECT, currentSlot);
            v7->side = currentSide;
            v7->flags[0] = 3;
            for (unsigned int i = 1; i < 3; ++i)
            {
                v9 = (HandlerParam_RemoveSideEffect*)((char*)v7 + i);
                v9->flags[0] = 0;
            }
            if (v7->flags[0] > 1u)
            {
                v7->flags[1] |= 0x80u;
            }
            BattleHandler_PopWork(serverFlow, v7);
        }
        else
        {
            if (DoesItemPreventHazardEffects(BattleMon_GetHeldItem(currentMon))) // Heavy-Duty Boots check
                return;

            v10 = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, 31);
            v10->sickID = CONDITION_POISON;
            unsigned int battleCount = BattleSideStatus_GetCountFromBattleEventItem(a1, currentSide); // New Definition
            ConditionData permanent;
            if (battleCount <= 1)
            {
                permanent = Condition_MakePermanent();
            }
            else
            {
                permanent = Condition_MakeBadlyPoisoned();
            }
            v10->sickCont = permanent;
            v10->pokeID = currentSlot;
            BattleHandler_PopWork(serverFlow, v10);
        }
    }
}
BattleEventHandlerTableEntry SideToxicSpikesHandlers[] = {
    {EVENT_SWITCH_IN, HandlerSideToxicSpikes},
};
BattleEventHandlerTableEntry* EventAddSideToxicSpikes(int* a1)
{
    *a1 = 1;
    return SideToxicSpikesHandlers;
}
// Heavy-Duty Boots hazard checks
void HandlerSideStealthRock(BattleEventItem* a1, ServerFlow* serverFlow, int currentSide, int* work)
{
    HandlerParam_Damage* v9;

    int currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if (currentSide == GetSideFromMonID(currentSlot))
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
        if (DoesItemPreventHazardEffects(BattleMon_GetHeldItem(currentMon))) // Heavy-Duty Boots check
            return;

        int pokeType = BattleMon_GetPokeType(currentMon);
        TypeEffectiveness effectiveness = (TypeEffectiveness)GetTypeEffectivenessVsMon(TYPE_ROCK, pokeType);
        unsigned int dmgPercent = 0;

        if (IsEqual(effectiveness, EFFECTIVENESS_IMMUNE))
        {
            return;
        }
        else if (IsEqual(effectiveness, EFFECTIVENESS_1_4))
        {
            dmgPercent = 32;
        }
        else if (IsEqual(effectiveness, EFFECTIVENESS_1_2))
        {
            dmgPercent = 16;
        }
        else if (IsEqual(effectiveness, EFFECTIVENESS_1))
        {
            dmgPercent = 8;
        }
        else if (IsEqual(effectiveness, EFFECTIVENESS_2))
        {
            dmgPercent = 4;
        }
        else if (IsEqual(effectiveness, EFFECTIVENESS_4))
        {
            dmgPercent = 2;
        }

        v9 = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, 31);
        v9->pokeID = currentSlot;
        v9->damage = DivideMaxHPZeroCheck(currentMon, dmgPercent);
        BattleHandler_StrSetup(&v9->exStr, 2u, 854);
        BattleHandler_AddArg(&v9->exStr, currentSlot);
        BattleHandler_PopWork(serverFlow, v9);
    }
}
BattleEventHandlerTableEntry SideStealthRockHandlers[] = {
    {EVENT_SWITCH_IN, HandlerSideStealthRock},
};
BattleEventHandlerTableEntry* EventAddSideStealthRock(int* a1)
{
    *a1 = 1;
    return SideStealthRockHandlers;
}
BattleEventHandlerTableEntry* EventAddSideWideGuard(int* a1)
{
    *a1 = 1;
    return (BattleEventHandlerTableEntry*)0x689D888;
}
BattleEventHandlerTableEntry* EventAddSideQuickGuard(int* a1)
{
    *a1 = 1;
    return (BattleEventHandlerTableEntry*)0x689D8A0;
}
BattleEventHandlerTableEntry* EventAddSideRainbow(int* a1)
{
    *a1 = 3;
    return (BattleEventHandlerTableEntry*)0x689D8F0;
}
BattleEventHandlerTableEntry* EventAddSideSeaOfFire(int* a1)
{
    *a1 = 1;
    return (BattleEventHandlerTableEntry*)0x689D880;
}
BattleEventHandlerTableEntry* EventAddSideSwamp(int* a1)
{
    *a1 = 1;
    return (BattleEventHandlerTableEntry*)0x689D8A8;
}

// STICKY WEB
// Heavy-Duty Boots hazard checks
void HandlerSideStickyWeb(BattleEventItem* a1, ServerFlow* serverFlow, int currentSide, int* work)
{
    HandlerParam_ChangeStatStage* v3;

    int currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if (currentSide == GetSideFromMonID(currentSlot))
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
        if (DoesItemPreventHazardEffects(BattleMon_GetHeldItem(currentMon))) // Heavy-Duty Boots check
            return;

        v3 = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, currentSlot);
        v3->poke_cnt = 1;
        v3->pokeID[0] = currentSlot;
        v3->rankType = STATSTAGE_SPEED;
        v3->rankVolume = -1;
        BattleHandler_PopWork(serverFlow, v3);
    }
}
BattleEventHandlerTableEntry SideStickyWebHandlers[] = {
    {EVENT_SWITCH_IN, HandlerSideStickyWeb},    // Rapid Spin implementation is in HandlerRapidSpin
};
BattleEventHandlerTableEntry* EventAddSideStickyWeb(int* a1)
{
    *a1 = 1;
    return SideStickyWebHandlers;
}

SideEffectEventAddTable ExtSideEffectEventAddTable[] = {
{SIDEEFF_REFLECT, EventAddSideReflect, 1},
{SIDEEFF_LIGHT_SCREEN, EventAddSideLightScreen, 1},
{SIDEEFF_SAFEGUARD, EventAddSideSafeguard, 1},
{SIDEEFF_MIST, EventAddSideMist, 1},
{SIDEEFF_TAILWIND, EventAddSideTailwind, 1},
{SIDEEFF_LUCKY_CHANT, EventAddSideLuckyChant, 1},
{SIDEEFF_SPIKES, EventAddSideSpikes, 3},
{SIDEEFF_TOXIC_SPIKES, EventAddSideToxicSpikes, 2},
{SIDEEFF_STEALTH_ROCK, EventAddSideStealthRock, 1},
{SIDEEFF_WIDE_GUARD, EventAddSideWideGuard, 1},
{SIDEEFF_QUICK_GUARD, EventAddSideQuickGuard, 1},
{SIDEEFF_RAINBOW, EventAddSideRainbow, 1},
{SIDEEFF_SEA_OF_FIRE, EventAddSideSeaOfFire, 1},
{SIDEEFF_SWAMP, EventAddSideSwamp, 1},
{SIDEEFF_STICKY_WEB, EventAddSideStickyWeb, 1},
};

int* SideEffectEvent_AddItem(int currentSide, SideEffect effect, ConditionData condData)
{
    __int16 effectCpy; // r5
    BattleSideCondition* sideCondition; // r4
    unsigned int Count; // r0
    unsigned int i; // r6
    BattleEventHandlerTableEntry* eventFunc; // r0
    BattleEventItem* eventItem; // r5
    ConditionData v9; // r0
    int v12[8]; // [sp+10h] [bp-28h] BYREF
    ConditionData condDataCpy; // [sp+30h] [bp-8h]

    v12[6] = currentSide;
    v12[7] = effect;
    condDataCpy = condData;
    effectCpy = effect;

    if (effect < DEFAULT_SIDE_CONDITION_AMOUNT)
        sideCondition = &SideStatus.Sides[currentSide].Conditions[effect];
    else
        sideCondition = &SideStatusExt.Sides[currentSide].Conditions[effect - DEFAULT_SIDE_CONDITION_AMOUNT];

    Count = sideCondition->Count;
    for (i = 0; i < ARRAY_COUNT(ExtSideEffectEventAddTable); ++i)
    {
        if (effect == ExtSideEffectEventAddTable[i].sideEffect)
        {
            if (!Count)
            {
                eventFunc = ExtSideEffectEventAddTable[i].func(v12);
                eventItem = BattleEvent_AddItem(EVENTITEM_SIDE, effectCpy, EVENTPRI_SIDE_DEFAULT, 0, currentSide, eventFunc, v12[0]);
                BattleEventItem_SetWorkValue(eventItem, 6, condDataCpy);
                sideCondition->Count = 1;
                v9 = condDataCpy;
                sideCondition->TurnCounter = 0;
                sideCondition->conditionData = v9;
                sideCondition->BattleEventItem = eventItem;

                return (int*)&eventItem->prev;
            }
            if (Count < ExtSideEffectEventAddTable[i].maxCount)
            {
                ++sideCondition->Count;
                return (int*)&sideCondition->BattleEventItem->prev;
            }
        }
    }
    return 0;
}
int THUMB_BRANCH_BattleHandler_AddSideEffect(ServerFlow* a1, HandlerParam_AddSideEffect* a2)
{
    int side; // r3
    ConditionData cont; // [sp+4h] [bp-14h] BYREF

    side = a2->side;
    cont = a2->cont;
    ServerEvent_CheckSideEffectParam(a1, a2->header.flags << 19 >> 27, a2->effect, side, &cont);
    if (!SideEffectEvent_AddItem(a2->side, a2->effect, cont))
    {
        return 0;
    }
    BattleHandler_SetString(a1, &a2->exStr);
    return 1;
}
int THUMB_BRANCH_ItemEffect_StatusGuard(ServerFlow* a1, BattleMon* a2)
{
    int ID; // r0
    int SideFromMonID; // r6
    ConditionData Turn; // r0
    int v6; // r0
    HandlerParam_Header* v7; // r4

    ID = BattleMon_GetID(a2);
    SideFromMonID = GetSideFromMonID(ID);
    Turn = Condition_MakeTurn(5);
    SideEffectEvent_AddItem(SideFromMonID, SIDEEFF_MIST, Turn);
    if (!v6)
    {
        return 0;
    }
    v7 = BattleHandler_PushWork(a1, EFFECT_MESSAGE, 31);
    BattleHandler_StrSetup((HandlerParam_StrParams*)&v7[1], 1u, 136);
    BattleHandler_AddArg((HandlerParam_StrParams*)&v7[1], SideFromMonID);
    BattleHandler_PopWork(a1, v7);
    return 1;
}

void THUMB_BRANCH_BattleSideStatus_Clear()
{
    sys_memset(&SideStatus, 0, 448u);
    sys_memset(&SideStatusExt, 0, 448u);
}

int THUMB_BRANCH_j_j_BattleSideStatus_GetCount(int currentSide, SideEffect effect)
{
    if (effect < DEFAULT_SIDE_CONDITION_AMOUNT)
        return SideStatus.Sides[currentSide].Conditions[effect].Count;
    else
        return SideStatusExt.Sides[currentSide].Conditions[effect - DEFAULT_SIDE_CONDITION_AMOUNT].Count;
}

bool BattleSideStatus_IsEffectActive(int currentSide, SideEffect effect)
{
    if (effect < DEFAULT_SIDE_CONDITION_AMOUNT)
        return SideStatus.Sides[currentSide].Conditions[effect].BattleEventItem != 0;
    else
        return SideStatusExt.Sides[currentSide].Conditions[effect - DEFAULT_SIDE_CONDITION_AMOUNT].BattleEventItem != 0;
}
bool THUMB_BRANCH_Handler_IsSideEffectActive(int a1, int currentSide, SideEffect effect)
{
    return BattleSideStatus_IsEffectActive(currentSide, effect);
}
bool THUMB_BRANCH_j_j_BattleSideStatus_IsEffectActive(int currentSide, SideEffect effect)
{
    return BattleSideStatus_IsEffectActive(currentSide, effect);
}
bool THUMB_BRANCH_j_j_BattleSideStatus_IsEffectActive_0(int currentSide, SideEffect effect)
{
    return BattleSideStatus_IsEffectActive(currentSide, effect);
}
bool THUMB_BRANCH_j_j_BattleSideStatus_IsEffectActive_1(int currentSide, SideEffect effect)
{
    return BattleSideStatus_IsEffectActive(currentSide, effect);
}
bool THUMB_BRANCH_j_j_BattleSideStatus_IsEffectActive_2(int currentSide, SideEffect effect)
{
    return BattleSideStatus_IsEffectActive(currentSide, effect);
}
bool THUMB_BRANCH_j_j_BattleSideStatus_IsEffectActive_3(int currentSide, SideEffect effect)
{
    return BattleSideStatus_IsEffectActive(currentSide, effect);
}

int THUMB_BRANCH_j_j_SideEvent_RemoveItem(int currentSide, SideEffect effect)
{
    BattleSideCondition* sideCondition; // r4

    if (effect < DEFAULT_SIDE_CONDITION_AMOUNT)
        sideCondition = &SideStatus.Sides[currentSide].Conditions[effect];
    else
        sideCondition = &SideStatusExt.Sides[currentSide].Conditions[effect - DEFAULT_SIDE_CONDITION_AMOUNT];

    if (!sideCondition->BattleEventItem)
    {
        return 0;
    }
    BattleEventItem_Remove(sideCondition->BattleEventItem);
    sideCondition->BattleEventItem = 0;
    sideCondition->Count = 0;

    int condDataInt = *((int*)&sideCondition->conditionData);
    condDataInt &= 0xFFFFFFF8;
    sideCondition->conditionData = *((ConditionData*)&condDataInt);

    return 1;
}

void THUMB_BRANCH_ServerControl_TurnCheckSide(ServerFlow* a1)
{
    BattleSideCondition* sideCondition;
    unsigned int turnCounter;

    for (unsigned int currentSide = 0; currentSide < 2; ++currentSide)
    {
        for (unsigned int effect = 0; effect < ARRAY_COUNT(ExtSideEffectEventAddTable); ++effect)
        {
            if (effect < DEFAULT_SIDE_CONDITION_AMOUNT)
                sideCondition = &SideStatus.Sides[currentSide].Conditions[effect];
            else
                sideCondition = &SideStatusExt.Sides[currentSide].Conditions[effect - DEFAULT_SIDE_CONDITION_AMOUNT];

            int condDataInt = *((int*)&sideCondition->conditionData);

            if (sideCondition->Count != 0 && (condDataInt & 7) == 2)
            {
                turnCounter = sideCondition->TurnCounter + 1;
                sideCondition->TurnCounter = turnCounter;
                if (turnCounter >= (condDataInt << 23) >> 26)
                {
                    sideCondition->Count = 0;

                    condDataInt &= 0xFFFFFFF8;
                    sideCondition->conditionData = *((ConditionData*)&condDataInt);

                    BattleEventItem_Remove(sideCondition->BattleEventItem);
                    sideCondition->BattleEventItem = 0;
                    ServerControl_SideEffectEndMessage(currentSide, effect, a1);
                }
            }
        }
    }
}

int THUMB_BRANCH_BattleHandler_RemoveSideEffectCore(ServerFlow* serverFlow, HandlerParam_RemoveSideEffect* params)
{
    unsigned int flagIdx;
    bool effectFlagActive;

    int removedAnEffect = 0;
    for (u16 i = 0; i <= DEFAULT_SIDE_CONDITION_AMOUNT; ++i)
    {
        flagIdx = ((i >> 3) + 1);

        effectFlagActive = flagIdx < params->flags[0] && ((1 << (i & 7)) & params->flags[flagIdx]) != 0;
        if (effectFlagActive)
        {
            if (i >= DEFAULT_SIDE_CONDITION_AMOUNT) // if the unused flags are set we check the extended flags
            {
                for (u16 j = DEFAULT_SIDE_CONDITION_AMOUNT; j < SIDE_CONDITION_AMOUNT; ++j)
                {
                    u8 flag = 1 << (j - DEFAULT_SIDE_CONDITION_AMOUNT);
                    if (removeSideEffExtFlags & flag)
                    {
                        if (j_j_SideEvent_RemoveItem(params->side, j))
                        {
                            ServerControl_SideEffectEndMessageCore(serverFlow, SIDEEFF_STEALTH_ROCK, params->side);
                            removedAnEffect = 1;
                        }
                    }
                }
            }
            else if (j_j_SideEvent_RemoveItem(params->side, i))
            {
                ServerControl_SideEffectEndMessageCore(serverFlow, i, params->side);
                removedAnEffect = 1;
            }
        }
    }

    removeSideEffExtFlags = 0;
    return removedAnEffect;
}

// ABILITY EXPANSION
// NEW
 
// New event triggered before an ability change that allows cancelling the change (called here -> BattleHandler_ChangeAbility)
u8 ServerEvent_ChangeAbilityCheckFail(ServerFlow* a1, int pokemonSlot, int prevAbility, int nextAbility)
{
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    BattleEventVar_SetConstValue(NEW_VAR_MON_ID, pokemonSlot);
    BattleEventVar_SetConstValue(VAR_PREVIOUS_ABILITY, prevAbility);
    BattleEventVar_SetConstValue(VAR_NEXT_ABILITY, nextAbility);
    BattleEventVar_SetRewriteOnceValue(VAR_MOVE_FAIL_FLAG, 0);
    BattleEvent_CallHandlers(a1, EVENT_ABILITY_CHANGE_CHECK_FAIL);
    u8 failFlag = BattleEventVar_GetValue(VAR_MOVE_FAIL_FLAG);
    BattleEventVar_Pop();

    return failFlag;
}


// OVL_167
#if ADD_NEW_ITEMS
void THUMB_BRANCH_HandlerIntimidate(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    u8* TempWork; // r4
    unsigned int v6; // r5
    HandlerParam_ChangeStatStage* v7; // r1
    unsigned int v8; // r3
    char v9; // r2
    char* v10; // r0
    __int16 ExistFrontPokePos; // [sp+0h] [bp-18h]

    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        ExistFrontPokePos = Handler_GetExistFrontPokePos(serverFlow, pokemonSlot);
        TempWork = Handler_GetTempWork(serverFlow);
        v6 = Handler_ExpandPokeID(serverFlow, ExistFrontPokePos | 0x100, TempWork);
        if (v6)
        {

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
            v7 = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
            v7->rankType = STATSTAGE_ATTACK;
            v7->rankVolume = -1;
            v7->fMoveAnimation = 1;
            v8 = 0;
            for (v7->poke_cnt = v6; v8 < v6; v10[16] = v9)
            {
                v9 = TempWork[v8];
                v10 = (char*)v7 + v8++;
            }

            SetIntimidateFlag(true);
            BattleHandler_PopWork(serverFlow, v7);
            SetIntimidateFlag(false);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}

// Protective Pads contact checks
void THUMB_BRANCH_HandlerMummy(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    HandlerParam_ChangeAbility* v8;

    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && !Handler_CheckMatchup((int)serverFlow))
    {
        u16 moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (getMoveFlag(moveID, FLAG_CONTACT))
        {
            int attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
            if (DoesItemPreventContact(BattleMon_GetHeldItem(attackingMon), (MoveID)moveID)) // Protective Pad check
                return;

            if (BattleMon_GetValue(attackingMon, VALUE_ABILITY) != 152)
            {
                v8 = (HandlerParam_ChangeAbility*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_ABILITY, pokemonSlot);
                v8->abilityID = ABIL152_MUMMY;
                v8->pokeID = attackingSlot;
                BattleHandler_StrSetup(&v8->exStr, 2u, 463);
                BattleHandler_AddArg(&v8->exStr, v8->pokeID);
                if (!MainModule_IsAllyMonID(pokemonSlot, attackingSlot))
                {
                    v8->header.flags |= 0x800000u;
                }
                BattleHandler_PopWork(serverFlow, v8);
            }
        }
    }
}

// Protective Pads contact checks (Rough Skin & Iron Barbs)
void THUMB_BRANCH_HandlerRoughSkin(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    HandlerParam_Damage* v8;

    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG))
    {
        u16 moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (getMoveFlag(moveID, FLAG_CONTACT))
        {
            int attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
            if (!BattleMon_IsFainted(attackingMon))
            {
                if (DoesItemPreventContact(BattleMon_GetHeldItem(attackingMon), (MoveID)moveID)) // Protective Pad check
                    return;

                v8 = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
                v8->header.flags |= 0x800000u;
                v8->pokeID = attackingSlot;
                v8->damage = DivideMaxHPZeroCheck(attackingMon, 8u);
                BattleHandler_StrSetup(&v8->exStr, 2u, 430);
                BattleHandler_AddArg(&v8->exStr, attackingSlot);
                BattleHandler_PopWork(serverFlow, v8);
            }
        }
    }
}

// Protective Pads contact checks
void THUMB_BRANCH_HandlerAftermath(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    HandlerParam_Damage* v9; // r4

    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON))
    {
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsFainted(defendingMon))
        {
            u16 moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
            if (getMoveFlag(moveID, FLAG_CONTACT))
            {
                int attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
                BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
                if (DoesItemPreventContact(BattleMon_GetHeldItem(attackingMon), (MoveID)moveID)) // Protective Pad check
                    return;

                v9 = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
                v9->header.flags |= 0x800000u;
                v9->pokeID = attackingSlot;
                v9->damage = DivideMaxHPZeroCheck(attackingMon, 4u);
                v9->flags = v9->flags & 0xFE | 1;
                BattleHandler_StrSetup(&v9->exStr, 2u, 402);
                BattleHandler_AddArg(&v9->exStr, attackingSlot);
                BattleHandler_PopWork(serverFlow, v9);
            }
        }
    }
}

// Utility Umbrella weather checks
void THUMB_BRANCH_HandlerChlorophyll(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) && Handler_GetWeather(serverFlow) == WEATHER_SUN)// sun
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // dont get speed boost when holding Utility Umbrella
            return;

       BattleEventVar_MulValue(VAR_RATIO, 0x2000);
    }
}

// Utility Umbrella weather checks
void THUMB_BRANCH_HandlerDrySkinWeather(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    BattleMon* currentMon; // r7
    int weather; // r0
    HandlerParam_Damage* v7; // r6
    HandlerParam_RecoverHP* v8; // r6

    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // dont get heal or take damage when holding Utility Umbrella
            return;

        weather = BattleEventVar_GetValue(VAR_WEATHER);
        if (weather == WEATHER_SUN)
        {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
            v7 = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
            v7->pokeID = pokemonSlot;
            v7->damage = DivideMaxHPZeroCheck(currentMon, 8u);
            BattleHandler_PopWork(serverFlow, v7);
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
        else if (weather == WEATHER_RAIN)
        {
            v8 = (HandlerParam_RecoverHP*)BattleHandler_PushWork(serverFlow, EFFECT_RECOVER_HP, pokemonSlot);
            v8->header.flags |= 0x800000u;
            v8->pokeID = pokemonSlot;
            v8->recoverHP = DivideMaxHPZeroCheck(currentMon, 8u);
            BattleHandler_PopWork(serverFlow, v8);
        }
    }
}


// Utility Umbrella weather checks (Flower Gift)
BattleEventHandlerTableEntry FlowerGiftHandlers[] = {
    {EVENT_AFTER_LAST_SWITCHIN, HandlerFlowerGiftSwitchIn},
    {EVENT_CHECK_ACTIVATION, HandlerFlowerGiftSwitchIn},
    {EVENT_ITEM_REWRITE_DONE, HandlerFlowerGiftSwitchIn}, // Utility Umbrella weather checks
    {EVENT_AFTER_ABILITY_CHANGE, HandlerFlowerGiftGotAbility},
    {EVENT_AFTER_WEATHER_CHANGE, HandlerFlowerGiftWeather},
    {EVENT_ABILITY_NULLIFIED, HandlerFlowerGiftAbilityOff},
    {EVENT_NOTIFY_AIR_LOCK, HandlerFlowerGiftAirLock},
    {EVENT_ACTION_PROCESSING_END, HandlerFlowerGiftWeather},
    {EVENT_TURN_CHECK_DONE, HandlerFlowerGiftWeather},
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerFlowerGiftAbilityChange},
    {EVENT_ATTACKER_POWER, HandlerFlowerGiftPower},
    {EVENT_DEFENDER_GUARD, HandlerFlowerGiftSpecialDefense},
};
BattleEventHandlerTableEntry* THUMB_BRANCH_EventAddFlowerGift(int* a1)
{
    *a1 = ARRAY_COUNT(FlowerGiftHandlers);
    return FlowerGiftHandlers;
}
void THUMB_BRANCH_CommonFlowerGiftFormChange(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int nextForm, u8 a5)
{
    HandlerParam_ChangeForm* v9;

    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    if (nextForm == 1 && DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // stay in normal form when holding Utility Umbrella
    {
        nextForm = 0;
    }

    if (nextForm != BattleMon_GetValue(currentMon, VALUE_FORM))
    {
        v9 = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
        v9->pokeID = pokemonSlot;
        v9->formNo = nextForm;
        v9->header.flags = v9->header.flags & 0xFF7FFFFF | (a5 << 23) & 0xFFFFFF;
        BattleHandler_StrSetup(&v9->exStr, 2u, 222);
        BattleHandler_AddArg(&v9->exStr, pokemonSlot);
        BattleHandler_PopWork(serverFlow, v9);
    }
}
void THUMB_BRANCH_HandlerFlowerGiftPower(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (CheckFlowerGiftEnablePokemon(serverFlow, pokemonSlot) && Handler_GetWeather(serverFlow) == WEATHER_SUN)
    {
        int attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        if (MainModule_IsAllyMonID(pokemonSlot, attackingSlot))
        {
            u16 moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
            if (PML_MoveGetCategory(moveID) == CATEGORY_PHYSICAL)
            {
                BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
                if (DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // dont boost attack when holding Utility Umbrella
                    return;

                BattleEventVar_MulValue(VAR_RATIO, 6144);
            }
        }
    }
}
void THUMB_BRANCH_HandlerFlowerGiftSpecialDefense(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (CheckFlowerGiftEnablePokemon(serverFlow, pokemonSlot) && Handler_GetWeather(serverFlow) == WEATHER_SUN)
    {
        int defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (MainModule_IsAllyMonID(pokemonSlot, defendingSlot) && BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_SPECIAL)
        {
            BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            if (DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // dont boost spdef when holding Utility Umbrella
                return;

            BattleEventVar_MulValue(VAR_RATIO, 6144);
        }
    }
}

// Utility Umbrella weather checks (Forecast)
BattleEventHandlerTableEntry ForecastHandlers[] = {
    {EVENT_AFTER_LAST_SWITCHIN, HandlerForecastSwitchIn},
    {EVENT_CHECK_ACTIVATION, HandlerForecastSwitchIn},
    {EVENT_ITEM_REWRITE_DONE, HandlerForecastSwitchIn}, // Utility Umbrella weather checks
    {EVENT_AFTER_ABILITY_CHANGE, HandlerForecastGetAbility},
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerForecastChangeAbility},
    {EVENT_ABILITY_NULLIFIED, HandlerForecastAbilityOff},
    {EVENT_NOTIFY_AIR_LOCK, HandlerForecastAirLock},
    {EVENT_ACTION_PROCESSING_END, HandlerForecastWeather},
    {EVENT_TURN_CHECK_DONE, HandlerForecastWeather},
    {EVENT_AFTER_WEATHER_CHANGE, HandlerForecastWeather},
};
BattleEventHandlerTableEntry* THUMB_BRANCH_EventAddForecast(int* a1)
{
    *a1 = ARRAY_COUNT(ForecastHandlers);
    return ForecastHandlers;
}
void THUMB_BRANCH_CommonForecastFormChange(ServerFlow* serverFlow, int pokemonSlot, unsigned int weather)
{
    HandlerParam_ChangeForm* v9;

    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    if (BattleMon_GetSpecies(currentMon) == PK351_CASTFORM)
    {
        int nextForm = 0;
        switch (weather)
        {
        case WEATHER_SUN:
            nextForm = 1;
            break;
        case WEATHER_RAIN:
            nextForm = 2;
            break;
        case WEATHER_HAIL:
            nextForm = 3;
            break;
        default:
            return;
        }
        if ((nextForm > 0 && nextForm < 3) && DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // stay in normal form when holding Utility Umbrella (unless hail)
        {
            nextForm = 0;
        }


        int currentForm = BattleMon_GetValue(currentMon, VALUE_FORM);
        if (nextForm != currentForm)
        {
            v9 = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            v9->header.flags |= 0x800000u;
            v9->pokeID = pokemonSlot;
            v9->formNo = nextForm;
            BattleHandler_StrSetup(&v9->exStr, 2u, 222);
            BattleHandler_AddArg(&v9->exStr, pokemonSlot);
            BattleHandler_PopWork(serverFlow, v9);
        }
    }
}

// Utility Umbrella weather checks
void THUMB_BRANCH_HandlerHarvest(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    HandlerParam_SetItem* v8;

    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

        int usedItem = BattleMon_GetUsedItem(currentMon);
        if (usedItem)
        {
            if (PML_ItemIsBerry(usedItem)
                && BattleMon_GetHeldItem(currentMon) == IT_NULL)
            {
                if ((Handler_GetWeather(serverFlow) == WEATHER_SUN && !DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) ||
                    AbilityEvent_RollEffectChance(serverFlow, 50u)) // dont ignore roll when holding the utility umbrella
                {
                    v8 = (HandlerParam_SetItem*)BattleHandler_PushWork(serverFlow, EFFECT_SET_HELD_ITEM, pokemonSlot);
                    v8->header.flags |= 0x800000u;
                    v8->itemID = usedItem;
                    v8->pokeID = pokemonSlot;
                    v8->fClearConsume = 1;
                    BattleHandler_StrSetup(&v8->exStr, 2u, 475);
                    BattleHandler_AddArg(&v8->exStr, pokemonSlot);
                    BattleHandler_AddArg(&v8->exStr, usedItem);
                    BattleHandler_PopWork(serverFlow, v8);
                }
            }
        }
    }
}

// Utility Umbrella weather checks
void THUMB_BRANCH_HandlerHydration(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    HandlerParam_CureCondition* v6;

    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) && Handler_GetWeather(serverFlow) == WEATHER_RAIN)
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // dont boost spdef when holding Utility Umbrella
            return;

        if (BattleMon_GetStatus(currentMon))
        {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
            v6 = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
            v6->sickCode = CONDITION_24;
            v6->pokeID[0] = pokemonSlot;
            v6->poke_cnt = 1;
            v6->header.flags |= 0x2000000u;
            BattleHandler_PopWork(serverFlow, v6);
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}

// Utility Umbrella weather checks (Leaf Guard)
void THUMB_BRANCH_HandlerLeafGuard(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) && Handler_GetWeather(serverFlow) == WEATHER_SUN)
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // dont cure status when holding Utility Umbrella
            return;

        int condition = BattleEventVar_GetValue(VAR_CONDITION_ID);
        if (IsBasicStatus((MoveCondition)condition) || condition == 14)
        {
            *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
        }
    }
}
void THUMB_BRANCH_HandlerLeafGuardYawnCheck(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) && Handler_GetWeather(serverFlow) == WEATHER_SUN)
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // dont cure status when holding Utility Umbrella
            return;

        BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
    }
}

// Utility Umbrella weather checks
void THUMB_BRANCH_HandlerRainDish(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // dont heal when holding Utility Umbrella
            return;

        CommonWeatherRecoveryAbility(serverFlow, pokemonSlot, WEATHER_RAIN);
    }
}

// Utility Umbrella weather checks (Solar Power)
void THUMB_BRANCH_HandlerSolarPowerWeather(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    HandlerParam_Damage* v7;

    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) && BattleEventVar_GetValue(VAR_WEATHER) == WEATHER_SUN)
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // dont show ability pop up when holding Utility Umbrella
            return;

        u16 damage = DivideMaxHPZeroCheck(currentMon, 8u);
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
        v7 = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
        v7->pokeID = pokemonSlot;
        v7->damage = damage;
        BattleHandler_PopWork(serverFlow, v7);
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
void THUMB_BRANCH_HandlerSolarPowerPower(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) && Handler_GetWeather(serverFlow) == WEATHER_SUN)
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // dont boost spatk when holding Utility Umbrella
            return;

        u16 moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (PML_MoveGetCategory(moveID) == CATEGORY_SPECIAL)
        {
            BattleEventVar_MulValue(VAR_RATIO, 6144);
        }
    }
}

// Utility Umbrella weather checks
void THUMB_BRANCH_HandlerSwiftSwim(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) && Handler_GetWeather(serverFlow) == WEATHER_RAIN)// rain
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // dont get speed boost when holding Utility Umbrella
            return;

        BattleEventVar_MulValue(VAR_RATIO, 0x2000);
    }
}

// Protective Pads contact checks
void THUMB_BRANCH_CommonContactStatusAbility(ServerFlow* serverFlow, int pokemonSlot, MoveCondition condition, ConditionData condData, u8 effectChance)
{
    HandlerParam_AddCondition* v9;

    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG))
    {
        u16 moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (getMoveFlag(moveID, FLAG_CONTACT) && AbilityEvent_RollEffectChance(serverFlow, effectChance))
        {
            int attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
            if (DoesItemPreventContact(BattleMon_GetHeldItem(attackingMon), (MoveID)moveID)) // Protective Pad check
                return;

            v9 = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
            v9->header.flags |= 0x800000u;
            v9->sickID = condition;
            v9->sickCont = condData;
            v9->fAlmost = 0;
            v9->pokeID = attackingSlot;
            BattleHandler_PopWork(serverFlow, v9);
        }
    }
}

// Called when changing an ability (exept when using Skill Swap)
int THUMB_BRANCH_BattleHandler_ChangeAbility(ServerFlow* serverFlow, HandlerParam_ChangeAbility* params)
{
    BattleMon*  battleMon = PokeCon_GetBattleMon(serverFlow->pokeCon, params->pokeID);
    int abilityID = (unsigned __int16)BattleMon_GetValue(battleMon, VALUE_ABILITY);
    if (j_j_Tables_IsAbilityMultitype(abilityID)
        || BattleMon_IsFainted(battleMon)
        || !params->fSameAbilityEffective && params->abilityID == abilityID)
    {
        return 0;
    }
    u8 failFlag = ServerEvent_ChangeAbilityCheckFail(serverFlow, params->pokeID, abilityID, params->abilityID); // Added event to allow ability changes to fail
    if (failFlag)
    {
        return 0;
    }

    if ((params->header.flags & 0x800000) != 0)
    {
        ServerDisplay_AddCommon(serverFlow->serverCommandQueue, 87, params->header.flags << 19 >> 27);
    }
    ServerDisplay_AddCommon(serverFlow->serverCommandQueue, 73, params->pokeID, params->abilityID);
    BattleHandler_SetString(serverFlow, &params->exStr);

    unsigned int v7 = HEManager_PushState((unsigned int*)&serverFlow->HEManager);
    ServerEvent_ChangeAbilityBefore(serverFlow, params->pokeID, abilityID, params->abilityID);
    HEManager_PopState((unsigned int*)&serverFlow->HEManager, v7);

    AbilityEvent_RemoveItem(battleMon);
    BattleMon_ChangeAbility(battleMon, params->abilityID);
    ServerDisplay_AddCommon(serverFlow->serverCommandQueue, 29, params->pokeID, params->abilityID);
    AbilityEvent_AddItem(battleMon);
    ServerDisplay_AddCommon(serverFlow->serverCommandQueue, 88, params->pokeID);

    if (abilityID != params->abilityID)
    {
        unsigned int v8 = HEManager_PushState((unsigned int*)&serverFlow->HEManager);
        ServerEvent_ChangeAbilityAfter(serverFlow, params->pokeID);
        HEManager_PopState((unsigned int*)&serverFlow->HEManager, v8);
    }
    if ((params->header.flags & 0x800000) != 0)
    {
        ServerDisplay_AddCommon(serverFlow->serverCommandQueue, 88, params->header.flags << 19 >> 27);
    }
    if (!BattleMon_CheckIfMoveCondition(battleMon, CONDITION_GASTROACID))
    {
        if (abilityID == ABIL103_KLUTZ)
        {
            ServerControl_CheckItemReaction(serverFlow, battleMon, 0);
        }
        if (abilityID == ABIL127_UNNERVE)
        {
            ServerControl_UnnerveAction(serverFlow, (int)battleMon);
        }
    }
    return 1;
}

// Called when changing an ability using Skill Swap
void THUMB_BRANCH_ServerDisplay_SkillSwap(ServerFlow* serverFlow, BattleMon* attackingMon, PokeSet* targetSet)
{
    unsigned int v11;
    unsigned int v12;

    BattleMon* defendingMon = (BattleMon*)j_j_PokeSet_Get_3((int)targetSet, 0);
    int attackerAbility = BattleMon_GetValue(attackingMon, VALUE_ABILITY);
    int defenderAbility = BattleMon_GetValue(defendingMon, VALUE_ABILITY);

    if (attackerAbility == defenderAbility
        || j_CheckSkillSwapFailAbility(attackerAbility)
        || j_CheckSkillSwapFailAbility_0(defenderAbility))
    {
        ABILITY_SWAP_FAIL:
        ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, 90, 71, -65536);
    }
    else
    {
        int attackingSlot = BattleMon_GetID(attackingMon);
        int defendingSlot = BattleMon_GetID(defendingMon);

        u8 atkFailFlag = ServerEvent_ChangeAbilityCheckFail(serverFlow, attackingSlot, attackerAbility, defenderAbility); // Added event to allow ability changes to fail
        u8 defFailFlag = ServerEvent_ChangeAbilityCheckFail(serverFlow, defendingSlot, defenderAbility, attackerAbility); // Added event to allow ability changes to fail
        if (atkFailFlag || defFailFlag)
        {
            goto ABILITY_SWAP_FAIL;
        }

        MoveAnimCtrl* moveAnimCtrl = serverFlow->moveAnimCtrl;
        if ((moveAnimCtrl->flags & 1) == 0)
        {
            moveAnimCtrl->flags = moveAnimCtrl->flags & 0xFE | 1;
        }
        ServerDisplay_AddCommon(serverFlow->serverCommandQueue, 74, attackingSlot, defendingSlot, defenderAbility, attackerAbility);
        ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, 91, 508, attackingSlot, -65536);
        v11 = HEManager_PushState((unsigned int*)&serverFlow->HEManager);
        ServerEvent_ChangeAbilityBefore(serverFlow, attackingSlot, attackerAbility, defenderAbility);
        ServerEvent_ChangeAbilityBefore(serverFlow, defendingSlot, defenderAbility, attackerAbility);
        HEManager_PopState((unsigned int*)&serverFlow->HEManager, v11);
        BattleMon_ChangeAbility(attackingMon, defenderAbility);
        BattleMon_ChangeAbility(defendingMon, attackerAbility);
        AbilityEvent_Swap(attackingMon, defendingMon);
        ServerDisplay_AddCommon(serverFlow->serverCommandQueue, 88, attackingSlot);
        ServerDisplay_AddCommon(serverFlow->serverCommandQueue, 88, defendingSlot);
        if (attackerAbility != defenderAbility)
        {
            v12 = HEManager_PushState((unsigned int*)&serverFlow->HEManager);
            ServerEvent_ChangeAbilityAfter(serverFlow, attackingSlot);
            ServerEvent_ChangeAbilityAfter(serverFlow, defendingSlot);
            HEManager_PopState((unsigned int*)&serverFlow->HEManager, v12);
        }
        if (!BattleMon_CheckIfMoveCondition(attackingMon, CONDITION_GASTROACID))
        {
            if (attackerAbility == ABIL103_KLUTZ)
            {
                ServerControl_CheckItemReaction(serverFlow, attackingMon, 0);
            }
            if (attackerAbility == ABIL127_UNNERVE)
            {
                ServerControl_UnnerveAction(serverFlow, (int)attackingMon);
            }
        }
        if (!BattleMon_CheckIfMoveCondition(defendingMon, CONDITION_GASTROACID))
        {
            if (defenderAbility == ABIL103_KLUTZ)
            {
                ServerControl_CheckItemReaction(serverFlow, defendingMon, 0);
            }
            if (defenderAbility == ABIL127_UNNERVE)
            {
                ServerControl_UnnerveAction(serverFlow, (int)defendingMon);
            }
        }
    }
}

bool THUMB_BRANCH_HandlerMoldBreakerSkipCheck(BattleEventItem* a1, ServerFlow* serverFlow, int factorType, int eventType, int subID, u16 abilityID)
{
    if (factorType == 4 && j_j_IsMoldBreakerAffectedAbility(abilityID))
    {
        int defendingSlot = serverFlow->setTarget->getIdx;
        if (defendingSlot >= 0 && defendingSlot < BATTLE_MAX_SLOTS)
        {
            BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, defendingSlot);
            if (DoesItemPreventMoldbreaker(BattleMon_GetHeldItem(defendingMon)))
            {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}
#endif

// MOVE EXPANSION
// NEW
#if ADD_NEW_ITEMS
void HandlerRagePowderBaitTarget(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    BattleStyle battleStyle = Handler_GetBattleStyle((int)serverFlow);
    if (battleStyle && battleStyle != BTL_STYLE_ROTATION)
    {
        int attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (!MainModule_IsAllyMonID(pokemonSlot, attackingSlot))
        {
            MoveID moveID = (MoveID)BattleEventVar_GetValue(VAR_MOVE_ID);
            if (moveID != MOVE507_SKY_DROP)
            {
                if (!BattleMon_CheckIfMoveCondition(attackingMon, CONDITION_SKYDROP))
                {
                    if (!Handler_IsMonSwitchOutInterrupted(serverFlow))
                    {
                        if (!Handler_IsTargetInRange(serverFlow, attackingSlot, pokemonSlot, moveID))
                        {
                            if (BattleEventVar_GetValue(VAR_ITEM) == IT0650_SAFETY_GOGGLES)
                            {
                                return;
                            }
                            BattleEventVar_RewriteValue(VAR_DEFENDING_MON, pokemonSlot);
                        }
                    }
                }
            }
        }
    }
}
BattleEventHandlerTableEntry RagePowderHandlers[] = {
    {EVENT_MOVE_EXECUTE_CHECK2, HandlerFollowMeCheckFail},
    {EVENT_UNCATEGORIZED_MOVE, HandlerFollowMeTextSet},
    {EVENT_REDIRECT_TARGET, HandlerRagePowderBaitTarget},
    {EVENT_TURN_CHECK_BEGIN, HandlerFollowMeTurnCheck},
};
BattleEventHandlerTableEntry* EventAddRagePowder(int* a1)
{
    *a1 = ARRAY_COUNT(RagePowderHandlers);
    return RagePowderHandlers;
}
#endif

// --- DATA ---

// Extended list of move events
MoveEventAddTable ExtMoveEventAddTable[] = {
{MOVE160_CONVERSION, EventAddConversion},
{MOVE293_CAMOUFLAGE, EventAddCamouflage},
{MOVE114_HAZE, EventAddHaze},
{MOVE138_DREAM_EATER, EventAddDreamEater},
{MOVE161_TRI_ATTACK, EventAddTriAttack},
{MOVE290_SECRET_POWER, EventAddSecretPower},
{MOVE448_CHATTER, EventAddChatter},
{MOVE162_SUPER_FANG, EventAddSuperFang},
{MOVE082_DRAGON_RAGE, EventAddDragonRage},
{MOVE069_SEISMIC_TOSS, EventAddSeismicToss},
{MOVE101_NIGHT_SHADE, EventAddSeismicToss},
{MOVE149_PSYWAVE, EventAddPsywave},
{MOVE173_SNORE, EventAddSnore},
{MOVE387_LAST_RESORT, EventAddLastResort},
{MOVE175_FLAIL, EventAddFlail},
{MOVE179_REVERSAL, EventAddFlail},
{MOVE206_FALSE_SWIPE, EventAddFalseSwipe},
{MOVE169_SPIDER_WEB, EventAddSpiderWeb},
{MOVE212_MEAN_LOOK, EventAddSpiderWeb},
{MOVE335_BLOCK, EventAddSpiderWeb},
{MOVE203_ENDURE, EventAddEndure},
{MOVE049_SONIC_BOOM, EventAddSonicBoom},
{MOVE252_FAKE_OUT, EventAddFakeOut},
{MOVE283_ENDEAVOR, EventAddEndeavor},
{MOVE284_ERUPTION, EventAddEruption},
{MOVE323_WATER_SPOUT, EventAddEruption},
{MOVE287_REFRESH, EventAddRefresh},
{MOVE234_MORNING_SUN, EventAddMorningSun},
{MOVE236_MOONLIGHT, EventAddMorningSun},
{MOVE235_SYNTHESIS, EventAddMorningSun},
{MOVE378_WRING_OUT, EventAddCrushGrip},
{MOVE462_CRUSH_GRIP, EventAddCrushGrip},
{MOVE311_WEATHER_BALL, EventAddWeatherBall},
{MOVE182_PROTECT, EventAddProtect},
{MOVE197_DETECT, EventAddProtect},
{MOVE150_SPLASH, EventAddSplash},
{MOVE174_CURSE, EventAddCurse},
{MOVE037_THRASH, EventAddThrash},
{MOVE253_UPROAR, EventAddUproar},
{MOVE080_PETAL_DANCE, EventAddThrash},
{MOVE200_OUTRAGE, EventAddThrash},
{MOVE362_BRINE, EventAddBrine},
{MOVE358_WAKE_UP_SLAP, EventAddWakeUpSlap},
{MOVE265_SMELLING_SALTS, EventAddSmellingSalts},
{MOVE367_ACUPRESSURE, EventAddAcupressure},
{MOVE376_TRUMP_CARD, EventAddTrumpCard},
{MOVE019_FLY, EventAddFly},
{MOVE340_BOUNCE, EventAddBounce},
{MOVE291_DIVE, EventAddDive},
{MOVE091_DIG, EventAddDig},
{MOVE076_SOLAR_BEAM, EventAddSolarBeam},
{MOVE013_RAZOR_WIND, EventAddRazorWind},
{MOVE143_SKY_ATTACK, EventAddSkyAttack},
{MOVE130_SKULL_BASH, EventAddSkullBash},
{MOVE227_ENCORE, EventAddEncore},
{MOVE239_TWISTER, EventAddTwister},
{MOVE016_GUST, EventAddTwister},
{MOVE089_EARTHQUAKE, EventAddEarthquake},
{MOVE057_SURF, EventAddSurf},
{MOVE327_SKY_UPPERCUT, EventAddSkyUppercut},
{MOVE222_MAGNITUDE, EventAddMagnitude},
{MOVE364_FEINT, EventAddFeint},
{MOVE467_SHADOW_FORCE, EventAddShadowForce},
{MOVE165_STRUGGLE, EventAddStruggle},
{MOVE386_PUNISHMENT, EventAddPunishment},
{MOVE269_TAUNT, EventAddTaunt},
{MOVE445_CAPTIVATE, EventAddCaptivate},
{MOVE187_BELLY_DRUM, EventAddBellyDrum},
{MOVE194_DESTINY_BOND, EventAddDestinyBond},
{MOVE263_FACADE, EventAddFacade},
{MOVE371_PAYBACK, EventAddPayback},
{MOVE237_HIDDEN_POWER, EventAddHiddenPower},
{MOVE107_MINIMIZE, EventAddMinimize},
{MOVE111_DEFENSE_CURL, EventAddDefenseCurl},
{MOVE023_STOMP, EventAddStomp},
{MOVE171_NIGHTMARE, EventAddNightmare},
{MOVE389_SUCKER_PUNCH, EventAddSuckerPunch},
{MOVE205_ROLLOUT, EventAddRollout},
{MOVE301_ICE_BALL, EventAddRollout},
{MOVE312_AROMATHERAPY, EventAddAromatherapy},
{MOVE215_HEAL_BELL, EventAddHealBell},
{MOVE262_MEMENTO, EventAddMemento},
{MOVE180_SPITE, EventAddSpite},
{MOVE156_REST, EventAddRest},
{MOVE199_LOCK_ON, EventAddLockOn},
{MOVE170_MIND_READER, EventAddLockOn},
{MOVE115_REFLECT, EventAddReflect},
{MOVE113_LIGHT_SCREEN, EventAddLightScreen},
{MOVE219_SAFEGUARD, EventAddSafeguard},
{MOVE054_MIST, EventAddMist},
{MOVE366_TAILWIND, EventAddTailwind},
{MOVE216_RETURN, EventAddReturn},
{MOVE218_FRUSTRATION, EventAddFrustration},
{MOVE217_PRESENT, EventAddPresent},
{MOVE259_TORMENT, EventAddTorment},
{MOVE286_IMPRISON, EventAddImprison},
{MOVE356_GRAVITY, EventAddGravity},
{MOVE288_GRUDGE, EventAddGrudge},
{MOVE270_HELPING_HAND, EventAddHelpingHand},
{MOVE380_GASTRO_ACID, EventAddGastroAcid},
{MOVE272_ROLE_PLAY, EventAddRolePlay},
{MOVE191_SPIKES, EventAddSpikes},
{MOVE390_TOXIC_SPIKES, EventAddToxicSpikes},
{MOVE446_STEALTH_ROCK, EventAddStealthRock},
{MOVE355_ROOST, EventAddRoost},
{MOVE393_MAGNET_RISE, EventAddMagnetRise},
{MOVE210_FURY_CUTTER, EventAddFuryCutter},
{MOVE375_PSYCHO_SHIFT, EventAddPsychoShift},
{MOVE372_ASSURANCE, EventAddAssurance},
{MOVE176_CONVERSION_2, EventAddConversion2},
{MOVE068_COUNTER, EventAddCounter},
{MOVE243_MIRROR_COAT, EventAddMirrorCoat},
{MOVE368_METAL_BURST, EventAddMetalBurst},
{MOVE279_REVENGE, EventAddRevenge},
{MOVE419_AVALANCHE, EventAddRevenge},
{MOVE167_TRIPLE_KICK, EventAddTripleKick},
{MOVE360_GYRO_BALL, EventAddGyroBall},
{MOVE220_PAIN_SPLIT, EventAddPainSplit},
{MOVE266_FOLLOW_ME, EventAddFollowMe},
{MOVE388_WORRY_SEED, EventAddWorrySeed},
{MOVE086_THUNDER_WAVE, EventAddThunderWave},
{MOVE244_PSYCH_UP, EventAddPsychUp},
{MOVE391_HEART_SWAP, EventAddHeartSwap},
{MOVE384_POWER_SWAP, EventAddPowerSwap},
{MOVE385_GUARD_SWAP, EventAddGuardSwap},
{MOVE213_ATTRACT, EventAddAttract},
{MOVE449_JUDGMENT, EventAddJudgement},
{MOVE363_NATURAL_GIFT, EventAddNaturalGift},
{MOVE282_KNOCK_OFF, EventAddKnockOff},
{MOVE050_DISABLE, EventAddDisable},
{MOVE168_THIEF, EventAddThief},
{MOVE343_COVET, EventAddThief},
{MOVE271_TRICK, EventAddTrick},
{MOVE415_SWITCHEROO, EventAddTrick},
{MOVE102_MIMIC, EventAddMimic},
{MOVE166_SKETCH, EventAddSketch},
{MOVE026_JUMP_KICK, EventJumpKickAdd},
{MOVE136_HIGH_JUMP_KICK, EventJumpKickAdd},
{MOVE432_DEFOG, EventAddDefog},
{MOVE280_BRICK_BREAK, EventAddBrickBreak},
{MOVE433_TRICK_ROOM, EventAddTrickRoom},
{MOVE346_WATER_SPORT, EventAddWaterSport},
{MOVE300_MUD_SPORT, EventAddMudSport},
{MOVE268_CHARGE, EventAddCharge},
{MOVE195_PERISH_SONG, EventAddPerishSong},
{MOVE073_LEECH_SEED, EventAddLeechSeed},
{MOVE251_BEAT_UP, EventAddBeatUp},
{MOVE392_AQUA_RING, EventAddAquaRing},
{MOVE461_LUNAR_DANCE, EventAddLunarDance},
{MOVE361_HEALING_WISH, EventAddHealingWish},
{MOVE118_METRONOME, EventAddMetronome},
{MOVE267_NATURE_POWER, EventAddNaturePower},
{MOVE274_ASSIST, EventAddAssist},
{MOVE119_MIRROR_MOVE, EventAddMirrorMove},
{MOVE382_ME_FIRST, EventAddMeFirst},
{MOVE383_COPYCAT, EventAddCopycat},
{MOVE214_SLEEP_TALK, EventAddSleepTalk},
{MOVE067_LOW_KICK, EventAddLowKick},
{MOVE447_GRASS_KNOT, EventAddLowKick},
{MOVE264_FOCUS_PUNCH, EventAddFocusPunch},
{MOVE254_STOCKPILE, EventAddStockpile},
{MOVE255_SPIT_UP, EventAddSpitUp},
{MOVE256_SWALLOW, EventAddSwallow},
{MOVE248_FUTURE_SIGHT, EventAddFutureSight},
{MOVE353_DOOM_DESIRE, EventAddDoomDesire},
{MOVE278_RECYCLE, EventAddRecycle},
{MOVE228_PURSUIT, EventAddPursuit},
{MOVE006_PAY_DAY, EventAddPayDay},
{MOVE117_BIDE, EventAddBide},
{MOVE289_SNATCH, EventAddSnatch},
{MOVE277_MAGIC_COAT, EventAddMagicCoat},
{MOVE100_TELEPORT, EventAddTeleport},
{MOVE369_U_TURN, EventAddUturn},
{MOVE226_BATON_PASS, EventAddBatonPass},
{MOVE365_PLUCK, EventAddPluck},
{MOVE450_BUG_BITE, EventAddPluck},
{MOVE374_FLING, EventAddFling},
{MOVE035_WRAP, EventAddBind},
{MOVE020_BIND, EventAddBind},
{MOVE083_FIRE_SPIN, EventAddBind},
{MOVE128_CLAMP, EventAddBind},
{MOVE328_SAND_TOMB, EventAddBind},
{MOVE463_MAGMA_STORM, EventAddBind},
{MOVE250_WHIRLPOOL, EventAddWhirlpool},
{MOVE229_RAPID_SPIN, EventAddRapidSpin},
{MOVE018_WHIRLWIND, EventAddRapidSpin},
{MOVE379_POWER_TRICK, EventAddPowerTrick},
{MOVE144_TRANSFORM, EventAddTransform},
{MOVE153_EXPLOSION, EventAddExplosion},
{MOVE120_SELF_DESTRUCT, EventAddExplosion},
{MOVE116_FOCUS_ENERGY, EventAddFocusEnergy},
{MOVE099_RAGE, EventAddRage},
{MOVE246_ANCIENT_POWER, EventAddAncientPower},
{MOVE466_OMINOUS_WIND, EventAddAncientPower},
{MOVE318_SILVER_WIND, EventAddAncientPower},
{MOVE087_THUNDER, EventAddThunder},
{MOVE059_BLIZZARD, EventAddBlizzard},
{MOVE273_WISH, EventAddWish},
{MOVE381_LUCKY_CHANT, EventAddLuckyChant},
{MOVE193_FORESIGHT, EventAddForesight},
{MOVE316_ODOR_SLEUTH, EventAddForesight},
{MOVE357_MIRACLE_EYE, EventAddMiracleEye},
{MOVE074_GROWTH, EventAddGrowth},
{MOVE474_VENOSHOCK, EventAddVenoshock},
#if ADD_NEW_ITEMS
{MOVE476_RAGE_POWDER, EventAddRagePowder},
#else
{MOVE476_RAGE_POWDER, EventAddFollowMe},
#endif
{MOVE487_SOAK, EventAddSoak},
{MOVE493_SIMPLE_BEAM, EventAddSimpleBeam},
{MOVE494_ENTRAINMENT, EventAddEntrainment},
{MOVE499_CLEAR_SMOG, EventAddClearSmog},
{MOVE500_STORED_POWER, EventAddStoredPower},
{MOVE504_SHELL_SMASH, EventAddShellSmash},
{MOVE506_HEX, EventAddHex},
{MOVE512_ACROBATICS, EventAddAcrobatics},
{MOVE521_VOLT_SWITCH, EventAddUturn},
{MOVE469_WIDE_GUARD, EventAddWideGuard},
{MOVE513_REFLECT_TYPE, EventAddReflectType},
{MOVE471_POWER_SPLIT, EventAddPowerSplit},
{MOVE470_GUARD_SPLIT, EventAddGuardSplit},
{MOVE475_AUTOTOMIZE, EventAddAutotomize},
{MOVE484_HEAVY_SLAM, EventAddHeavySlam},
{MOVE535_HEAT_CRASH, EventAddHeavySlam},
{MOVE472_WONDER_ROOM, EventAddWonderRoom},
{MOVE478_MAGIC_ROOM, EventAddMagicRoom},
{MOVE473_PSYSHOCK, EventAddPsyshock},
{MOVE540_PSYSTRIKE, EventAddPsyshock},
{MOVE481_FLAME_BURST, EventAddFlameBurst},
{MOVE486_ELECTRO_BALL, EventAddElectroBall},
{MOVE485_SYNCHRONOISE, EventAddSynchronoise},
{MOVE498_CHIP_AWAY, EventAddChipAway},
{MOVE533_SACRED_SWORD, EventAddChipAway},
{MOVE497_ECHOED_VOICE, EventAddEchoedVoice},
{MOVE510_INCINERATE, EventAddIncinerate},
{MOVE516_BESTOW, EventAddBestow},
{MOVE509_CIRCLE_THROW, EventAddCircleThrow},
{MOVE525_DRAGON_TAIL, EventAddCircleThrow},
{MOVE514_RETALIATE, EventAddRetaliate},
{MOVE492_FOUL_PLAY, EventAddFoulPlay},
{MOVE479_SMACK_DOWN, EventAddSmackDown},
{MOVE515_FINAL_GAMBIT, EventAddFinalGambit},
{MOVE495_AFTER_YOU, EventAddAfterYou},
{MOVE511_QUASH, EventAddQuash},
{MOVE496_ROUND, EventAddRound},
{MOVE501_QUICK_GUARD, EventAddQuickGuard},
{MOVE502_ALLY_SWITCH, EventAddAllySwitch},
{MOVE477_TELEKINESIS, EventAddTelekinesis},
{MOVE507_SKY_DROP, EventAddSkyDrop},
{MOVE537_STEAMROLLER, EventAddStomp},
{MOVE542_HURRICANE, EventAddThunder},
{MOVE548_SECRET_SWORD, EventAddPsyshock},
{MOVE547_RELIC_SONG, EventAddRelicSong},
{MOVE546_TECHNO_BLAST, EventAddTechnoBlast},
{MOVE553_FREEZE_SHOCK, EventAddFreezeShock},
{MOVE554_ICE_BURN, EventAddFreezeShock},
{MOVE518_WATER_PLEDGE, EventAddWaterPledge},
{MOVE519_FIRE_PLEDGE, EventAddWaterPledge},
{MOVE520_GRASS_PLEDGE, EventAddWaterPledge},
{MOVE558_FUSION_FLARE, EventAddFusionFlare},
{MOVE559_FUSION_BOLT, EventAddFusionFlare},
};

// FUNCTIONS

// OVL_167
#if ADD_NEW_ITEMS
// Utility Umbrella weather checks
void THUMB_BRANCH_HandlerMorningSun(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        int healRatio = 2048;

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (!DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // dont change heal amount when holding Utility Umbrella
        {
            int weather = Handler_GetWeather(serverFlow);
            switch (weather)
            {
            case WEATHER_SUN:
                healRatio = 2732;
                break;
            case WEATHER_RAIN:
            case WEATHER_HAIL:
            case WEATHER_SANDSTORM:
                healRatio = 1024;
                break;
            default:
                break;
            }
        }

        BattleEventVar_RewriteValue(VAR_RATIO, healRatio);
    }
}

// Utility Umbrella weather checks (Thunder)
void THUMB_BRANCH_HandlerThunderRainCheck(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON))
    {
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_DEFENDING_MON));
        if (DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(defendingMon))) // dont skip acc check when holding Utility Umbrella
            return;

        if (Handler_GetWeather(serverFlow) == WEATHER_RAIN)
        {
            BattleEventVar_RewriteValue(VAR_GENERAL_USE_FLAG, 1);
        }
    }
}
void THUMB_BRANCH_HandlerThunderSunCheck(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON))
    {
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_DEFENDING_MON));
        if (DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(defendingMon))) // dont lower accuracy when holding Utility Umbrella
            return;

        if (Handler_GetWeather(serverFlow) == WEATHER_SUN)
        {
            BattleEventVar_RewriteValue(VAR_ACCURACY, 50);
        }
    }
}

// Utility Umbrella weather checks (Solar Beam)
void THUMB_BRANCH_HandlerSolarBeamSunCheck(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON))
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // dont skip charge turn when holding Utility Umbrella
        {
            return;
        }

        if (Handler_GetWeather(serverFlow) == WEATHER_SUN)
        {
            BattleEventVar_RewriteValue(VAR_GENERAL_USE_FLAG, 1);
        }
    }
}
//void THUMB_BRANCH_HandlerSolarBeamPower(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
//{
//    int weather = Handler_GetWeather(serverFlow);
//    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)
//        && (weather >= WEATHER_RAIN && weather <= WEATHER_SANDSTORM))
//    {
//        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_DEFENDING_MON));
//        if (weather == WEATHER_RAIN && DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(defendingMon))) // dont lower power when holding Utility Umbrella (only in rain)
//            return;
//
//        BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 2048);
//    }
//}

// Utility Umbrella weather checks
void THUMB_BRANCH_HandlerGrowth(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) && Handler_GetWeather(serverFlow) == WEATHER_SUN)
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // dont boost more when holding Utility Umbrella
        {
            return;
        }

        int volume = BattleEventVar_GetValue(VAR_VOLUME);
        if (volume == 1)
        {
            volume = 2;
        }
        BattleEventVar_RewriteValue(VAR_VOLUME, volume);
    }
}

// Utility Umbrella weather checks (Weather Ball)
void THUMB_BRANCH_HandlerWeatherBallType(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // dont change type when holding Utility Umbrella
            return;


        int weather = Handler_GetWeather(serverFlow);
        int moveType = BattleEventVar_GetValue(VAR_MOVE_TYPE);
        u8 effectIndex = 0;
        switch (weather)
        {
        case WEATHER_SUN:
            moveType = TYPE_FIRE;
            effectIndex = 1;
            break;
        case WEATHER_RAIN:
            moveType = TYPE_WATER;
            effectIndex = 4;
            break;
        case WEATHER_HAIL:
            moveType = TYPE_ICE;
            effectIndex = 2;
            break;
        case WEATHER_SANDSTORM:
            moveType = TYPE_ROCK;
            effectIndex = 3;
            break;
        default:
            return;
        }
        BattleEventVar_RewriteValue(VAR_MOVE_TYPE, moveType);

        Handler_SetMoveEffectIndex(serverFlow, effectIndex);
    }
}
void THUMB_BRANCH_HandlerWeatherBallPower(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) && Handler_GetWeather(serverFlow))
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(currentMon))) // dont boost damage when holding Utility Umbrella
            return;

        int movePower = BattleEventVar_GetValue(VAR_MOVE_POWER);
        BattleEventVar_RewriteValue(VAR_MOVE_POWER, 2 * movePower);
    }
}
#endif

// Sticy Web removal
void THUMB_BRANCH_HandlerRapidSpin(BattleEventItem* a1, ServerFlow* serverFlow, int currentSlot, int* work)
{
    HandlerParam_CureCondition* v6;
    HandlerParam_CureCondition* v7;
    HandlerParam_RemoveSideEffect* v10;

    if (currentSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON))
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
        if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_LEECHSEED))
        {
            v6 = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, currentSlot);
            v6->poke_cnt = 1;
            v6->pokeID[0] = currentSlot;
            v6->sickCode = CONDITION_LEECHSEED;
            BattleHandler_PopWork(serverFlow, v6);
        }
        if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_BIND))
        {
            v7 = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, currentSlot);
            v7->poke_cnt = 1;
            v7->pokeID[0] = currentSlot;
            v7->sickCode = CONDITION_BIND;
            BattleHandler_PopWork(serverFlow, v7);
        }
        int currentSlotSide = GetSideFromMonID(currentSlot);

        bool isSpikesActive = BattleSideStatus_IsEffectActive(currentSlotSide, SIDEEFF_SPIKES);
        bool isToxSpikesActive = BattleSideStatus_IsEffectActive(currentSlotSide, SIDEEFF_TOXIC_SPIKES);
        bool isStealthRocksActive = BattleSideStatus_IsEffectActive(currentSlotSide, SIDEEFF_STEALTH_ROCK);
        bool isStickyWebActive = BattleSideStatus_IsEffectActive(currentSlotSide, (SideEffect)SIDEEFF_STICKY_WEB);
        if (isSpikesActive || isToxSpikesActive || isStealthRocksActive || isStickyWebActive)
        {
            v10 = (HandlerParam_RemoveSideEffect*)BattleHandler_PushWork(serverFlow, EFFECT_REMOVE_SIDE_EFFECT, currentSlot);
            v10->side = currentSlotSide;
            v10->flags[0] = 3;

            if (isSpikesActive && v10->flags[0] > 1u)
            {
                v10->flags[1] |= 0b01000000;
            }
            if (isToxSpikesActive && v10->flags[0] > 1u)
            {
                v10->flags[1] |= 0b10000000;
            }
            if (isStealthRocksActive && v10->flags[0] > 2u)
            {
                v10->flags[2] |= 0b00000001;
            }
            if (isStickyWebActive && v10->flags[0] > 2u)
            {
                v10->flags[2] |= 0b11000000; // we set the unused bits to indicate the use of the extended flags
                removeSideEffExtFlags |= 1;
            }

            BattleHandler_PopWork(serverFlow, v10);
        }
    }
}

// Sticy Web removal
void THUMB_BRANCH_HandlerDefog(BattleEventItem* a1, ServerFlow* serverFlow, int currentSlot, int* work)
{
    HandlerParam_ChangeStatStage* v7;
    HandlerParam_RemoveSideEffect* v8;

    if (currentSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON))
    {
        int targetSlot = BattleEventVar_GetValue(VAR_TARGET_MON_ID);
        BattleMon* targetMon = Handler_GetBattleMon(serverFlow, targetSlot);
        if (!BattleMon_IsSubstituteActive(targetMon))
        {
            v7 = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, currentSlot);
            v7->rankType = STATSTAGE_EVASION;
            v7->rankVolume = -1;
            v7->fMoveAnimation = 1;
            v7->poke_cnt = 1;
            v7->pokeID[0] = targetSlot;
            BattleHandler_PopWork(serverFlow, v7);
        }
        v8 = (HandlerParam_RemoveSideEffect*)BattleHandler_PushWork(serverFlow, EFFECT_REMOVE_SIDE_EFFECT, currentSlot);
        v8->side = GetSideFromMonID(targetSlot);
        v8->flags[0] = 3;

        if (v8->flags[0] > 1u)
        {
            v8->flags[1] |= 1u;
        }
        if (v8->flags[0] > 1u)
        {
            v8->flags[1] |= 2u;
        }
        if (v8->flags[0] > 1u)
        {
            v8->flags[1] |= 8u;
        }
        if (v8->flags[0] > 1u)
        {
            v8->flags[1] |= 4u;
        }
        if (v8->flags[0] > 1u)
        {
            v8->flags[1] |= 0x40u;
        }
        if (v8->flags[0] > 1u)
        {
            v8->flags[1] |= 0x80u;
        }
        if (v8->flags[0] > 2u)
        {
            v8->flags[2] |= 1u;
        }

        if (v8->flags[0] > 2u)
        {
            v8->flags[2] |= 0b11000000; // we set the unused bits to indicate the use of the extended flags
            removeSideEffExtFlags |= 1;
        }
        BattleHandler_PopWork(serverFlow, v8);
    }
}

#if ADD_NEW_ITEMS
// Electric & Misty Terrain fail
void THUMB_BRANCH_HandlerRestCheckFail(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) && !BattleEventVar_GetValue(VAR_FAIL_CAUSE))
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_SLEEP))
        {
            BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_OTHER);
        }
        else if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_HEALBLOCK))
        {
            BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_HEALBLOCK);
        }
        else if (BattleMon_IsFullHP(currentMon))
        {
            BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_HPFULL);
        }
        else if ((BattleField_GetTerrain() == TERRAIN_ELECTRIC || BattleField_GetTerrain() == TERRAIN_MISTY) &&
            !ServerControl_CheckFloating(serverFlow, currentMon, 1))
        {
            BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_OTHER); // TODO: Create custo move fail, maybe?
        }
        else
        {
            int currentAbility = BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY);
            if (currentAbility == ABIL015_INSOMNIA || currentAbility == ABIL072_VITAL_SPIRIT)
            {
                BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_INSOMNIA);
            }
        }
    }
}

// Terrain afected moves
void THUMB_BRANCH_HandlerNaturePower(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        int newMoveID = 0;

        int terrain = BattleField_GetTerrain();
        switch (terrain)
        {
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
            int battleGround = Handler_GetBattleTerrain(serverFlow);
            switch (battleGround)
            {
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

        int v7 = Handler_ReqMoveTargetAuto(serverFlow, pokemonSlot, newMoveID);
        BattleEventVar_RewriteValue(VAR_MOVE_ID, newMoveID);
        BattleEventVar_RewriteValue(VAR_POKE_POS, v7);
    }
}
int THUMB_BRANCH_CommonSecretPowerGetParams(ServerFlow* serverFlow, u8* effectPtr, u8* changeAmountPtr)
{
    u8 effect;
    u8 changeAmount;
    int result;

    int newMoveID = 0;

    int terrain = BattleField_GetTerrain();
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
        int battleGround = Handler_GetBattleTerrain(serverFlow);
        if (IsEqual(battleGround, 0) ||
            IsEqual(battleGround, 5))
        {
            effect = SCTPOWEFF_ADD_COND;
            changeAmount = CONDITION_SLEEP;
            result = 7;
        }
        else if (IsEqual(battleGround, 1) ||
            IsEqual(battleGround, 2) ||
            IsEqual(battleGround, 3) ||
            IsEqual(battleGround, 8) ||
            IsEqual(battleGround, 15) ||
            IsEqual(battleGround, 17))
        {
            effect = SCTPOWEFF_LOWER_STAT;
            changeAmount = STATSTAGE_ACCURACY;
            result = 1;
        }
        else if (IsEqual(battleGround, 6) ||
            IsEqual(battleGround, 11) ||
            IsEqual(battleGround, 12))
        {
            effect = SCTPOWEFF_LOWER_STAT;
            changeAmount = STATSTAGE_ATTACK;
            result = 5;
        }
        else if (IsEqual(battleGround, 7) ||
            IsEqual(battleGround, 13))
        {
            effect = SCTPOWEFF_ADD_COND;
            changeAmount = CONDITION_FREEZE;
            if (battleGround == 7)
            {
                result = 3;
            }
            else
            {
                result = 4;
            }
        }
        else if (IsEqual(battleGround, 9))
        {
            effect = SCTPOWEFF_LOWER_STAT;
            changeAmount = STATSTAGE_SPEED;
            result = 2;
        }
        else if (IsEqual(battleGround, 10) ||
            IsEqual(battleGround, 19))
        {
            effect = SCTPOWEFF_FLINCH;
            changeAmount = 0;
            result = 6;
        }
        else
        {
            effect = SCTPOWEFF_ADD_COND;
            changeAmount = CONDITION_PARALYSIS;
            result = 0;
        }
    }
    break;
    }

    if (effectPtr)
    {
        *effectPtr = effect;
    }
    if (changeAmountPtr)
    {
        *changeAmountPtr = changeAmount;
    }
    return result;
}
void THUMB_BRANCH_HandlerCamouflage(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    HandlerParam_ChangeType* v9;

    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON))
    {
        u16 pokeType = TYPE_NORMAL;

        int terrain = BattleField_GetTerrain();
        switch (terrain)
        {
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
            int battleGround = Handler_GetBattleTerrain(serverFlow);
            if (IsEqual(battleGround, 0) || 
                IsEqual(battleGround, 5))
            {
                pokeType = TYPE_GRASS;
            }
            else if (IsEqual(battleGround, 1) ||
                IsEqual(battleGround, 2) || 
                IsEqual(battleGround, 3) ||
                IsEqual(battleGround, 8) || 
                IsEqual(battleGround, 9) ||
                IsEqual(battleGround, 15) || 
                IsEqual(battleGround, 17))
            {
                pokeType = TYPE_GROUND;
            }
            else if (IsEqual(battleGround, 6) ||
                IsEqual(battleGround, 11) ||
                IsEqual(battleGround, 12))
            {
                pokeType = TYPE_WATER;
            }
            else if (IsEqual(battleGround, 7) ||
                IsEqual(battleGround, 13))
            {
                pokeType = TYPE_ICE;
            }
            else if (IsEqual(battleGround, 10) ||
                IsEqual(battleGround, 19))
            {
                pokeType = TYPE_ROCK;
            }
            else
            {
                pokeType = TYPE_NORMAL;
            }
        }
        break;
        }

        int monotype = PokeTypePair_MakeMonotype(pokeType);
        BattleMon* battleMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (monotype != BattleMon_GetPokeType(battleMon))
        {
            v9 = (HandlerParam_ChangeType*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_TYPE, pokemonSlot);
            v9->next_type = monotype;
            v9->pokeID = pokemonSlot;
            BattleHandler_PopWork(serverFlow, v9);
        }
    }
}
#endif

// Function that adds events when moves have them
int THUMB_BRANCH_MoveEvent_AddItem(BattleMon* battleMon, int moveID, int subPriority)
{
    unsigned char v10[4];
    v10[0] = 0;

    u16 moveEventIdx;
    for (u16 i = 0; i < ARRAY_COUNT(ExtMoveEventAddTable); ++i)
    {
        if (moveID == ExtMoveEventAddTable[i].move)
        {
            moveEventIdx = i;
            break;
        }
    }

    int pokemonSlot = BattleMon_GetID(battleMon);
    if (!MoveEvent_CanEffectBeRegistered(pokemonSlot, moveID, v10))
    {
        return 0;
    }

    int handlersSize = 0;
    const BattleEventHandlerTableEntry* handlers = ExtMoveEventAddTable[moveEventIdx].func(&handlersSize);
    if (!handlers)
    {
        return 0;
    }

    BattleEvent_AddItem(EVENTITEM_MOVE, moveID, EVENTPRI_MOVE_DEFAULT, subPriority, pokemonSlot, handlers, handlersSize);
    return 1;
}

#if ADD_NEW_ITEMS
// Called whenever an attack redirects (calls all EVENT_REDIRECT_TARGET handlers)
int THUMB_BRANCH_ServerEvent_RedirectTarget(ServerFlow* a1, BattleMon* a2, unsigned __int16* a3, int a4)
{
    int Value;

    BattleEventVar_Push();
    BattleEventVar_SetConstValue(VAR_ATTACKING_MON, BattleMon_GetID(a2));
    BattleEventVar_SetConstValue(VAR_MOVE_TYPE, *(a3 + 6));
    BattleEventVar_SetConstValue(VAR_MOVE_ID, *a3);
    BattleEventVar_SetConstValue(VAR_ITEM, BattleMon_GetHeldItem(a2)); // add the item value so it can be used in the handlers
    BattleEventVar_SetRewriteOnceValue(VAR_DEFENDING_MON, a4);
    BattleEvent_CallHandlers(a1, EVENT_REDIRECT_TARGET);
    Value = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    BattleEventVar_Pop();
    if (Value == a4)
    {
        return 31;
    }
    return Value;
}

// Called to check if there is anything that makes the item skip it's events
int THUMB_BRANCH_IsUnselectableMove(BtlClientWk* client, BattleMon* pokemon, int move, Btlv_StringParam* strparam)
{
    ConditionData moveCondition;
    int HeldItem;
    ConditionData moveCondition1;
    int ID;
    int v12;
    int v13;
    int v14;
    int v15;
    int v16;
    int v17;
    int choicedMove;
    int previousMove;

    if (move != MOVE165_STRUGGLE)
    {
        if (CanMonUseHeldItem(client, pokemon) && BattleMon_CheckIfMoveCondition(pokemon, CONDITION_CHOICELOCK))
        {
            moveCondition = BattleMon_GetMoveCondition(pokemon, CONDITION_CHOICELOCK);
            choicedMove = Condition_GetParam(moveCondition);
            if (Move_IsUsable(pokemon, choicedMove))
            {
                if (choicedMove != move)
                {
                    if (strparam)
                    {
                        Btlv_StringParam_Setup(strparam, 1, 99);
                        HeldItem = BattleMon_GetHeldItem(pokemon);
                        Btlv_StringParam_AddArg(strparam, HeldItem);
                        Btlv_StringParam_AddArg(strparam, choicedMove);
                    }
                    return 1;
                }
            }
        }
        if (BattleMon_CheckIfMoveCondition(pokemon, CONDITION_ENCORE))
        {
            moveCondition1 = BattleMon_GetMoveCondition(pokemon, CONDITION_ENCORE);
            previousMove = Condition_GetParam(moveCondition1);
            if (move != previousMove)
            {
                if (strparam)
                {
                    Btlv_StringParam_Setup(strparam, 1, 100);
                    ID = BattleMon_GetID(pokemon);
                    Btlv_StringParam_AddArg(strparam, ID);
                    Btlv_StringParam_AddArg(strparam, previousMove);
                }
                return 1;
            }
        }
        // Check for items that block the use of status moves (Ex. Assault Vest)
        if ((BattleMon_CheckIfMoveCondition(pokemon, CONDITION_TAUNT) || DoesItemPreventStatusMoveUse(BattleMon_GetHeldItem(pokemon))) &&
            !PML_MoveIsDamaging(move))
        {
            if (strparam)
            {
                // Output a different message when the item is the cause
                if (DoesItemPreventStatusMoveUse(BattleMon_GetHeldItem(pokemon)))
                {
                    Btlv_StringParam_Setup(strparam, 1, BATTLE_ASSAULTVEST_MSGID);
                    int pokemonID = BattleMon_GetID(pokemon);
                    int itemID = BattleMon_GetHeldItem(pokemon);
                    Btlv_StringParam_AddArg(strparam, pokemonID);
                    Btlv_StringParam_AddArg(strparam, move);
                    Btlv_StringParam_AddArg(strparam, itemID);
                }
                else
                {
                    Btlv_StringParam_Setup(strparam, 2, 571);
                    v12 = BattleMon_GetID(pokemon);
                    Btlv_StringParam_AddArg(strparam, v12);
                    Btlv_StringParam_AddArg(strparam, move);
                }
            }
            return 1;
        }
        if (BattleMon_CheckIfMoveCondition(pokemon, CONDITION_TORMENT) && move == BattleMon_GetPreviousMove(pokemon))
        {
            if (strparam)
            {
                Btlv_StringParam_Setup(strparam, 2, 580);
                v13 = BattleMon_GetID(pokemon);
                Btlv_StringParam_AddArg(strparam, v13);
                Btlv_StringParam_AddArg(strparam, move);
            }
            return 1;
        }
        if (BattleMon_CheckIfMoveCondition(pokemon, CONDITION_DISABLE)
            && move == BattleMon_GetConditionAffectedMove(pokemon, CONDITION_DISABLE)
            && move != MOVE165_STRUGGLE)
        {
            if (strparam)
            {
                Btlv_StringParam_Setup(strparam, 2, 595);
                v14 = BattleMon_GetID(pokemon);
                Btlv_StringParam_AddArg(strparam, v14);
                Btlv_StringParam_AddArg(strparam, move);
            }
            return 1;
        }
        if (BattleMon_CheckIfMoveCondition(pokemon, CONDITION_HEALBLOCK) && getMoveFlag(move, FLAG_HEALING))
        {
            if (strparam)
            {
                Btlv_StringParam_Setup(strparam, 2, 890);
                v15 = BattleMon_GetID(pokemon);
                Btlv_StringParam_AddArg(strparam, v15);
                Btlv_StringParam_AddArg(strparam, move);
            }
            return 1;
        }
        if (BattleField_CheckFieldEffectCore(client->battleField, FLDEFF_IMPRISON)
            && BattleField_CheckImprisonCore(client->battleField, client->pokeCon, pokemon, move))
        {
            if (strparam)
            {
                Btlv_StringParam_Setup(strparam, 2, 589);
                v16 = BattleMon_GetID(pokemon);
                Btlv_StringParam_AddArg(strparam, v16);
                Btlv_StringParam_AddArg(strparam, move);
            }
            return 1;
        }
        if (BattleField_CheckFieldEffectCore(client->battleField, FLDEFF_GRAVITY)
            && getMoveFlag(move, FLAG_GROUNDED_BY_GRAVITY))
        {
            if (strparam)
            {
                Btlv_StringParam_Setup(strparam, 2, 1086);
                v17 = BattleMon_GetID(pokemon);
                Btlv_StringParam_AddArg(strparam, v17);
                Btlv_StringParam_AddArg(strparam, move);
            }
            return 1;
        }
    }
    return 0;
}

// Called to check the amount of multiple his a move will do
int THUMB_BRANCH_ServerEvent_CheckMultihitHits(ServerFlow* serverFlow, BattleMon* battleMon, int moveID, HitCheckParam* params)
{
    int result;

    params->count = 0;
    params->MultiHitEffectiveness = 3;

    int hitMax = PML_MoveGetParam(moveID, MVDATA_HIT_MAX);
    if (hitMax <= 1)
    {
        params->countMax = 1;
        params->fCheckEveryTime = 0;
        params->fMultiHitMove = 0;

        result = 0;
    }
    else
    {
        int hitCount = RollMultiHitHits(hitMax);
        int attackingSlot = BattleMon_GetID(battleMon);

        BattleEventVar_Push();

        BattleEventVar_SetConstValue(VAR_ATTACKING_MON, attackingSlot);
        BattleEventVar_SetConstValue(VAR_MAX_HIT_COUNT, hitMax);

        BattleEventVar_SetRewriteOnceValue(VAR_HIT_COUNT, hitCount); // changed hitCount to be rewritable from the handlers
        BattleEventVar_SetRewriteOnceValue(VAR_AVOID_FLAG, 0);
        BattleEventVar_SetRewriteOnceValue(VAR_GENERAL_USE_FLAG, 0);
        BattleEvent_CallHandlers(serverFlow, EVENT_MOVE_HIT_COUNT);

        u8 avoidFlag = 0;
        if (hitMax <= 5 && BattleEventVar_GetValue(VAR_GENERAL_USE_FLAG))
        {
            params->countMax = hitMax;
            avoidFlag = 0;
        }
        else
        {
            params->countMax = BattleEventVar_GetValue(VAR_HIT_COUNT);
            avoidFlag = BattleEventVar_GetValue(VAR_AVOID_FLAG);
        }
        if (DoesItemGuaranteeMultiHit(BattleMon_GetHeldItem(battleMon)))
            avoidFlag = 0;
        params->fCheckEveryTime = avoidFlag;
        params->fMultiHitMove = 1;

        BattleEventVar_Pop();

        result = 1;
    }
    return result;
}

// Called to check if the accuracy test should be skipped or not
int THUMB_BRANCH_ServerEvent_BypassAccuracyCheck(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, MoveID moveID)
{
    int isAlwaysHit = PML_MoveIsAlwaysHit(moveID);
    if (!isAlwaysHit)
    {
        BattleEventVar_Push();
        int attackingSlot = BattleMon_GetID(attackingMon);
        BattleEventVar_SetConstValue(VAR_ATTACKING_MON, attackingSlot);
        int defendingSlot = BattleMon_GetID(defendingMon);
        BattleEventVar_SetConstValue(VAR_DEFENDING_MON, defendingSlot); // added the defending slot to the handler (to allow thunder & hurricane miss with utility umbrella)
        BattleEventVar_SetRewriteOnceValue(VAR_GENERAL_USE_FLAG, 0);
        BattleEvent_CallHandlers(serverFlow, EVENT_BYPASS_ACCURACY_CHECK);
        isAlwaysHit = BattleEventVar_GetValue(VAR_GENERAL_USE_FLAG);
        BattleEventVar_Pop();
    }
    return isAlwaysHit;
}
#endif

#if ADD_NEW_ITEMS
// ITEM EXPANSION
// NEW

// WEAKNESS POLICY
void HandlerWeaknessPolicyDamageReaction(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    TypeEffectiveness typeEffectiveness = (TypeEffectiveness)BattleEventVar_GetValue(VAR_TYPE_EFFECTIVENESS);
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG) 
        && typeEffectiveness >= EFFECTIVENESS_2)
    {
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

        if (IsModifyItemMove((MoveID)BattleEventVar_GetValue(VAR_MOVE_ID), BattleMon_GetHeldItem(defendingMon))) // CHECK ITEM IS NOT KNOCKED OFF OR CHANGED
            return;

        if (BattleMon_IsStatChangeValid(defendingMon, STATSTAGE_ATTACK, 2) ||
            BattleMon_IsStatChangeValid(defendingMon, STATSTAGE_SPECIAL_ATTACK, 2))
        {
            ItemEvent_PushRun(battleEventItem, serverFlow, pokemonSlot);
        }
    }
}
void HandlerWeaknessPolicyUse(BattleEventItem* battleEventItem, ServerFlow* a2, int pokemonSlot, int* work)
{
    HandlerParam_ChangeStatStage* attackBoost;
    HandlerParam_ChangeStatStage* spAttackBoost;

    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        attackBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(a2, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        attackBoost->poke_cnt = 1;
        attackBoost->pokeID[0] = (u8)pokemonSlot;
        attackBoost->rankType = STATSTAGE_ATTACK;
        attackBoost->rankVolume = 2;
        BattleHandler_PopWork(a2, attackBoost);

        spAttackBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(a2, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        spAttackBoost->poke_cnt = 1;
        spAttackBoost->pokeID[0] = (u8)pokemonSlot;
        spAttackBoost->rankType = STATSTAGE_SPECIAL_ATTACK;
        spAttackBoost->rankVolume = 2;
        BattleHandler_PopWork(a2, spAttackBoost);
    }
}
BattleEventHandlerTableEntry WeaknessPolicyHandlers[] = {
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerWeaknessPolicyDamageReaction},
    {EVENT_USE_ITEM, HandlerWeaknessPolicyUse},
};
BattleEventHandlerTableEntry* EventAddWeaknessPolicy(int* a1)
{
    *a1 = ARRAY_COUNT(WeaknessPolicyHandlers);
    return WeaknessPolicyHandlers;
}

// ASSAULT VEST
void HandlerAssaultVestDefense(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON))
    {
        u16 moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (PML_MoveGetCategory(moveID) == CATEGORY_SPECIAL)
        {
            BattleEventVar_MulValue(VAR_RATIO, 6144);
        }
    }
}
BattleEventHandlerTableEntry AssaultVestHandlers[] = {
    {EVENT_DEFENDER_GUARD, HandlerAssaultVestDefense}, // not using status moves effect is here -> IsUnselectableMove
};
BattleEventHandlerTableEntry* EventAddAssaultVest(int* a1)
{
    *a1 = ARRAY_COUNT(AssaultVestHandlers);
    return AssaultVestHandlers;
}

// PIXIE PLATE
void HandlerFairyFeather(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    CommonTypeBoostingItem(battleEventItem, (int)serverFlow, pokemonSlot, TYPE_NULL);
}
BattleEventHandlerTableEntry PixiePlateHandlers[] = {
    {EVENT_MOVE_POWER, HandlerFairyFeather},
};
BattleEventHandlerTableEntry* EventAddPixiePlate(int* a1)
{
    *a1 = ARRAY_COUNT(PixiePlateHandlers);
    return PixiePlateHandlers;
}

// LUMINOUS MOSS
void HandlerLuminousMossDamageReaction(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_WATER)
    {
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

        if (IsModifyItemMove((MoveID)BattleEventVar_GetValue(VAR_MOVE_ID), BattleMon_GetHeldItem(defendingMon))) // CHECK ITEM IS NOT KNOCKED OFF OR CHANGED
            return;

        if (BattleMon_IsStatChangeValid(defendingMon, STATSTAGE_SPECIAL_DEFENSE, 1))
        {
            ItemEvent_PushRun(battleEventItem, serverFlow, pokemonSlot);
        }
    }
}
void HandlerLuminousMossUse(BattleEventItem* battleEventItem, ServerFlow* a2, int pokemonSlot, int* work)
{
    HandlerParam_ChangeStatStage* spDefenseBoost;

    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        spDefenseBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(a2, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        spDefenseBoost->poke_cnt = 1;
        spDefenseBoost->pokeID[0] = (u8)pokemonSlot;
        spDefenseBoost->rankType = STATSTAGE_SPECIAL_DEFENSE;
        spDefenseBoost->rankVolume = 1;
        BattleHandler_PopWork(a2, spDefenseBoost);
    }
}
BattleEventHandlerTableEntry LuminousMossHandlers[] = {
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerLuminousMossDamageReaction},
    {EVENT_USE_ITEM, HandlerLuminousMossUse},
};
BattleEventHandlerTableEntry* EventAddLuminousMoss(int* a1)
{
    *a1 = ARRAY_COUNT(LuminousMossHandlers);
    return LuminousMossHandlers;
}

// SNOWBALL
void HandlerSnowballDamageReaction(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_ICE)
    {
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

        if (IsModifyItemMove((MoveID)BattleEventVar_GetValue(VAR_MOVE_ID), BattleMon_GetHeldItem(defendingMon))) // CHECK ITEM IS NOT KNOCKED OFF OR CHANGED
            return;

        if (BattleMon_IsStatChangeValid(defendingMon, STATSTAGE_ATTACK, 1))
        {
            ItemEvent_PushRun(battleEventItem, serverFlow, pokemonSlot);
        }
    }
}
void HandlerSnowballUse(BattleEventItem* battleEventItem, ServerFlow* a2, int pokemonSlot, int* work)
{
    HandlerParam_ChangeStatStage* attackBoost;

    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        attackBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(a2, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        attackBoost->poke_cnt = 1;
        attackBoost->pokeID[0] = (u8)pokemonSlot;
        attackBoost->rankType = STATSTAGE_ATTACK;
        attackBoost->rankVolume = 1;
        BattleHandler_PopWork(a2, attackBoost);
    }
}
BattleEventHandlerTableEntry SnowballHandlers[] = {
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerSnowballDamageReaction},
    {EVENT_USE_ITEM, HandlerSnowballUse},
};
BattleEventHandlerTableEntry* EventAddSnowball(int* a1)
{
    *a1 = ARRAY_COUNT(SnowballHandlers);
    return SnowballHandlers;
}

// SAFETY GOGGLES
void HandlerSafetyGogglesWeather(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    int currentID = BattleEventVar_GetValue(VAR_MON_ID);
    if (pokemonSlot == currentID)
    {
        int damage = BattleEventVar_GetValue(VAR_DAMAGE);
        if (damage > 0)
        {
            BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
        }
    }
}
void HandlerSafetyGogglesPowderMoves(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON))
    {
        MoveID moveID = (MoveID)BattleEventVar_GetValue(VAR_MOVE_ID);
        if (IsPowderMove(moveID, true))
        {
            BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);
        }
    }
}
BattleEventHandlerTableEntry SafetyGogglesHandlers[] = {
    {EVENT_WEATHER_REACTION, HandlerSafetyGogglesWeather},
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerSafetyGogglesPowderMoves}, // rage powder not working is here -> HandlerRagePowderBaitTarget
};
BattleEventHandlerTableEntry* EventAddSafetyGoggles(int* a1)
{
    *a1 = ARRAY_COUNT(SafetyGogglesHandlers);
    return SafetyGogglesHandlers;
}

// ROSELI BERRY
void HandlerRoseliBerry(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    CommonResistBerry(battleEventItem, serverFlow, pokemonSlot, work, TYPE_NULL, 0);
}
BattleEventHandlerTableEntry RoseliBerryHandlers[] = {
{EVENT_MOVE_DAMAGE_PROCESSING_2, HandlerRoseliBerry},
{EVENT_AFTER_DAMAGE_REACTION, HandlerCommonResistBerryDamageAfter},
};
BattleEventHandlerTableEntry* EventAddRoseliBerry(int* a1)
{
    *a1 = ARRAY_COUNT(RoseliBerryHandlers);
    return RoseliBerryHandlers;
}

// KEE BERRY
void HandlerKeeBerryDamageReaction(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_PHYSICAL)
    {
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

        if (IsModifyItemMove((MoveID)BattleEventVar_GetValue(VAR_MOVE_ID), BattleMon_GetHeldItem(defendingMon))) // CHECK ITEM IS NOT KNOCKED OFF OR CHANGED
            return;

        if (BattleMon_IsStatChangeValid(defendingMon, STATSTAGE_DEFENSE, 1))
        {
            ItemEvent_PushRun(battleEventItem, serverFlow, pokemonSlot);
        }
    }
}
void HandlerKeeBerryUse(BattleEventItem* battleEventItem, ServerFlow* a2, int pokemonSlot, int* work)
{
    HandlerParam_ChangeStatStage* defenseBoost;

    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        defenseBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(a2, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        defenseBoost->poke_cnt = 1;
        defenseBoost->pokeID[0] = (u8)pokemonSlot;
        defenseBoost->rankType = STATSTAGE_DEFENSE;
        defenseBoost->rankVolume = 1;
        BattleHandler_PopWork(a2, defenseBoost);
    }
}
BattleEventHandlerTableEntry KeeBerryHandlers[] = {
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerKeeBerryDamageReaction},
    {EVENT_USE_ITEM, HandlerKeeBerryUse},
    {EVENT_USE_ITEM_TEMP, HandlerKeeBerryUse},
};
BattleEventHandlerTableEntry* EventAddKeeBerry(int* a1)
{
    *a1 = ARRAY_COUNT(KeeBerryHandlers);
    return KeeBerryHandlers;
}

// MARANGA BERRY
void HandlerMarangaBerryDamageReaction(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_SPECIAL)
    {
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

        if (IsModifyItemMove((MoveID)BattleEventVar_GetValue(VAR_MOVE_ID), BattleMon_GetHeldItem(defendingMon))) // CHECK ITEM IS NOT KNOCKED OFF OR CHANGED
            return;

        if (BattleMon_IsStatChangeValid(defendingMon, STATSTAGE_SPECIAL_DEFENSE, 1))
        {
            ItemEvent_PushRun(battleEventItem, serverFlow, pokemonSlot);
        }
    }
}
void HandlerMarangaBerryUse(BattleEventItem* battleEventItem, ServerFlow* a2, int pokemonSlot, int* work)
{
    HandlerParam_ChangeStatStage* spDefenseBoost;

    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        spDefenseBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(a2, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        spDefenseBoost->poke_cnt = 1;
        spDefenseBoost->pokeID[0] = (u8)pokemonSlot;
        spDefenseBoost->rankType = STATSTAGE_SPECIAL_DEFENSE;
        spDefenseBoost->rankVolume = 1;
        BattleHandler_PopWork(a2, spDefenseBoost);
    }
}
BattleEventHandlerTableEntry MarangaBerryHandlers[] = {
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerMarangaBerryDamageReaction},
    {EVENT_USE_ITEM, HandlerMarangaBerryUse},
    {EVENT_USE_ITEM_TEMP, HandlerMarangaBerryUse},
};
BattleEventHandlerTableEntry* EventAddMarangaBerry(int* a1)
{
    *a1 = ARRAY_COUNT(MarangaBerryHandlers);
    return MarangaBerryHandlers;
}

// ADRENALINE ORB
void HandlerAdrenalineOrbIntimReaction(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_ATTACKING_MON));
        if (IsIntimidated((AbilID)attackingMon->CurrentAbility))
        {
            BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            if (BattleMon_IsStatChangeValid(currentMon, STATSTAGE_SPEED, 1))
            {
                SetIntimidateFlag(false); // make sure the boost from the item does not get treated as a Intim trigger when used by a mon with Intimidate
                ItemEvent_PushRun(battleEventItem, serverFlow, pokemonSlot);
                SetIntimidateFlag(true);
            }
        }
    }
}
void HandlerAdrenalineOrbUse(BattleEventItem* battleEventItem, ServerFlow* a2, int pokemonSlot, int* work)
{
    HandlerParam_ChangeStatStage* speedBoost;

    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        speedBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(a2, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        speedBoost->poke_cnt = 1;
        speedBoost->pokeID[0] = (u8)pokemonSlot;
        speedBoost->rankType = STATSTAGE_SPEED;
        speedBoost->rankVolume = 1;
        BattleHandler_PopWork(a2, speedBoost);
    }
}
BattleEventHandlerTableEntry AdrenalineOrbHandlers[] = {
    {EVENT_STAT_STAGE_CHANGE_APPLIED, HandlerAdrenalineOrbIntimReaction},
    {EVENT_USE_ITEM, HandlerAdrenalineOrbUse},
};
BattleEventHandlerTableEntry* EventAddAdrenalineOrb(int* a1)
{
    *a1 = ARRAY_COUNT(AdrenalineOrbHandlers);
    return AdrenalineOrbHandlers;
}

// PROTECTIVE PADS
// Implemented adding checks to contact situations, mostly ability handlers

// THROAT SPRAY
void HandlerThroatSpraySoundMoveCheck(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        int moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (getMoveFlag((MoveID)moveID, FLAG_SOUND))
        {
            BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            if (BattleMon_IsStatChangeValid(currentMon, STATSTAGE_SPECIAL_ATTACK, 1))
            {
                ItemEvent_PushRun(battleEventItem, serverFlow, pokemonSlot);
            }
        }
    }
}
void HandlerThroatSprayUse(BattleEventItem* battleEventItem, ServerFlow* a2, int pokemonSlot, int* work)
{
    HandlerParam_ChangeStatStage* spAttackBoost;
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        spAttackBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(a2, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        spAttackBoost->poke_cnt = 1;
        spAttackBoost->pokeID[0] = (u8)pokemonSlot;
        spAttackBoost->rankType = STATSTAGE_SPECIAL_ATTACK;
        spAttackBoost->rankVolume = 1;
        BattleHandler_PopWork(a2, spAttackBoost);
    }
}
BattleEventHandlerTableEntry ThroatSprayHandlers[] = {
    {EVENT_MOVE_EXECUTE_END, HandlerThroatSpraySoundMoveCheck},
    {EVENT_USE_ITEM, HandlerThroatSprayUse},
};
BattleEventHandlerTableEntry* EventAddThroatSpray(int* a1)
{
    *a1 = ARRAY_COUNT(ThroatSprayHandlers);
    return ThroatSprayHandlers;
}

// EJECT PACK
u8 ejectPackStatLowered[BATTLE_MAX_SLOTS];
void HandlerEjectPackStatCheck(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        if (BattleEventVar_GetValue(VAR_VOLUME) < 0)
        {
            ejectPackStatLowered[pokemonSlot] = 1;
        }
    }
}
void HandlerEjectPackActionEnd(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (ejectPackStatLowered[pokemonSlot])
    {
        ejectPackStatLowered[pokemonSlot] = 0;
        ItemEvent_PushRun(a1, serverFlow, pokemonSlot);
    }
}
void HandlerEjectPackUse(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    HandlerParam_Switch* v5;
    HandlerParam_AddAnimation* v6;

    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)
        && Handler_GetFightEnableBenchPokeNum(serverFlow, pokemonSlot)
        && Handler_CheckReservedMemberChangeAction(serverFlow))
    {
        unsigned int clientID = j_MainModule_PokeIDToClientID(pokemonSlot);
        v5 = (HandlerParam_Switch*)BattleHandler_PushWork(serverFlow, EFFECT_SWITCH, pokemonSlot);
        v5->pokeID = pokemonSlot;
        BattleHandler_StrSetup(&v5->preStr, 2u, 770);
        BattleHandler_AddArg(&v5->preStr, pokemonSlot);
        BattleHandler_AddArg(&v5->preStr, clientID);
        BattleHandler_PopWork(serverFlow, v5);
        v6 = (HandlerParam_AddAnimation*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_ANIMATION, pokemonSlot);
        u16 animation;
        if (BattleEventItem_GetSubID(a1) == MOVE369_U_TURN)
        {
            animation = 643;
        }
        else
        {
            animation = 644;
        }
        v6->animNo = animation;
        v6->pos_from = Handler_PokeIDToPokePos((int)serverFlow, pokemonSlot);
        v6->pos_to = 6;
        v6->header.flags |= 0x1000000u;
        BattleHandler_PopWork(serverFlow, v6);
    }
}
BattleEventHandlerTableEntry EjectPackHandlers[] = {
    {EVENT_STAT_STAGE_CHANGE_APPLIED, HandlerEjectPackStatCheck},
    {EVENT_ACTION_PROCESSING_END, HandlerEjectPackActionEnd},
    {EVENT_USE_ITEM, HandlerEjectPackUse},
};
BattleEventHandlerTableEntry* EventAddEjectPack(int* a1)
{
    *a1 = ARRAY_COUNT(EjectPackHandlers);
    return EjectPackHandlers;
}

// HEAVY-DUTY BOOTS
// "Implemented by adding checks to the entry hazards handlers"

// BLUNDER POLICY
void HandlerBlunderPolicyCheckMiss(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (IS_NOT_NEW_EVENT)
        return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_ATTACKING_MON))
    {
        BattleMon* battleMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsStatChangeValid(battleMon, STATSTAGE_SPEED, 2))
        {
            ItemEvent_PushRun(battleEventItem, serverFlow, pokemonSlot);
        }
    }
}
void HandlerBlunderPolicyUse(BattleEventItem* battleEventItem, ServerFlow* a2, int pokemonSlot, int* work)
{
    HandlerParam_ChangeStatStage* speedBoost;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_ATTACKING_MON))
    {
        speedBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(a2, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        speedBoost->poke_cnt = 1;
        speedBoost->pokeID[0] = (u8)pokemonSlot;
        speedBoost->rankType = STATSTAGE_SPEED;
        speedBoost->rankVolume = 2;
        BattleHandler_PopWork(a2, speedBoost);
    }
}
BattleEventHandlerTableEntry BlunderPolicyHandlers[] = {
    {EVENT_ACCURACY_MISS, HandlerBlunderPolicyCheckMiss},
    {EVENT_USE_ITEM, HandlerBlunderPolicyUse},
};
BattleEventHandlerTableEntry* EventAddBlunderPolicy(int* a1)
{
    *a1 = ARRAY_COUNT(BlunderPolicyHandlers);
    return BlunderPolicyHandlers;
}

// ROOM SERVICE
void HandlerRoomSeviceFieldCheck(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (BattleField_CheckEffect(FLDEFF_TRICK_ROOM))
    {
        BattleMon* battleMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsStatChangeValid(battleMon, STATSTAGE_SPEED, -1))
        {
            ItemEvent_PushRun(battleEventItem, serverFlow, pokemonSlot);
        }
    }
}
void HandlerRoomSeviceUse(BattleEventItem* battleEventItem, ServerFlow* a2, int pokemonSlot, int* work)
{
    HandlerParam_ChangeStatStage* speedBoost;

    if (BattleField_CheckEffect(FLDEFF_TRICK_ROOM))
    {
        speedBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(a2, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        speedBoost->poke_cnt = 1;
        speedBoost->pokeID[0] = (u8)pokemonSlot;
        speedBoost->rankType = STATSTAGE_SPEED;
        speedBoost->rankVolume = -1;
        BattleHandler_PopWork(a2, speedBoost);
    }
}
BattleEventHandlerTableEntry RoomSeviceHandlers[] = {
    {EVENT_MOVE_EXECUTE_END, HandlerRoomSeviceFieldCheck},
    {EVENT_SWITCH_IN, HandlerRoomSeviceFieldCheck},
    {EVENT_USE_ITEM, HandlerRoomSeviceUse},
};
BattleEventHandlerTableEntry* EventAddRoomSevice(int* a1)
{
    *a1 = ARRAY_COUNT(RoomSeviceHandlers);
    return RoomSeviceHandlers;
}

// UTILITY UMBRELLA
// Implemented adding checks to weather situations, mostly ability and move handlers

// ABILITY SHIELD
void HandlerAbilityShieldAbilityChangeFail(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (IS_NOT_NEW_EVENT)
        return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID))
    {
        BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
    }
}
BattleEventHandlerTableEntry AbilityShieldHandlers[] = {
    {EVENT_ABILITY_CHANGE_CHECK_FAIL, HandlerAbilityShieldAbilityChangeFail},
};
BattleEventHandlerTableEntry* EventAddAbilityShield(int* a1)
{
    *a1 = ARRAY_COUNT(AbilityShieldHandlers);
    return AbilityShieldHandlers;
}

// CLEAR AMULET
void HandlerClearAmuletCheck(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    CommonStatDropGuardCheck(serverFlow, pokemonSlot, work, 8);
}
void HandlerClearAmuletMessage(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (*work)
    {
        HandlerParam_Message* v6 = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&v6->str, 2u, BATTLE_CLEARAMULET_MSGID);
        BattleHandler_AddArg(&v6->str, pokemonSlot);
        BattleHandler_PopWork(serverFlow, v6);
    }
    *work = 0;
}
BattleEventHandlerTableEntry ClearAmuletHandlers[] = {
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerClearAmuletCheck},
    {EVENT_STAT_STAGE_CHANGE_FAIL, HandlerClearAmuletMessage},
};
BattleEventHandlerTableEntry* EventAddClearAmulet(int* a1)
{
    *a1 = ARRAY_COUNT(ClearAmuletHandlers);
    return ClearAmuletHandlers;
}

// MIRROR HERB
struct MirrorHerbStats
{
    int copiedSlot = -1;
    u8 boosts[8] = { 0,0,0,0,0,0,0,0 };
};
MirrorHerbStats mirrorHerbStats[BATTLE_MAX_SLOTS];
void HandlerMirrorHerbStatChange(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    int currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if (pokemonSlot != currentSlot && GetSideFromMonID(pokemonSlot) != GetSideFromMonID(currentSlot))
    {
        StatStage stat = (StatStage)BattleEventVar_GetValue(VAR_MOVE_EFFECT);
        int volume = BattleEventVar_GetValue(VAR_VOLUME);
        if (volume > 0)
        {
            BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            if (BattleMon_IsStatChangeValid(currentMon, stat, volume))
            {
                if (mirrorHerbStats[pokemonSlot].copiedSlot < 0) // Set the mon we are copying from if this is the first boost
                {
                    mirrorHerbStats[pokemonSlot].copiedSlot = currentSlot;
                }
                else
                {
                    if (mirrorHerbStats[pokemonSlot].copiedSlot != currentSlot) // Make sure we only copy the boost of 1 mon
                        return;
                }

                mirrorHerbStats[pokemonSlot].boosts[stat] += volume;
            }
        }
    }
}
void HandlerMirrorHerbActionEnd(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (mirrorHerbStats[pokemonSlot].copiedSlot > 0)
    {
        ItemEvent_PushRun(battleEventItem, serverFlow, pokemonSlot);
    }
}
void HandlerMirrorHerbUse(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    HandlerParam_ChangeStatStage* statBoost;

    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    if (BattleMon_GetHeldItem(currentMon) == IT1883_MIRROR_HERB)
    {
        for (u16 stat = 0; stat <= STATSTAGE_EVASION; ++stat)
        {
            int volume = mirrorHerbStats[pokemonSlot].boosts[stat];
            if (volume > 0)
            {
                statBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                statBoost->poke_cnt = 1;
                statBoost->pokeID[0] = (u8)pokemonSlot;
                statBoost->rankType = (StatStage)stat;
                statBoost->rankVolume = volume;
                BattleHandler_PopWork(serverFlow, statBoost);
            }
        }
        // Reset the values after using the item
        mirrorHerbStats[pokemonSlot].copiedSlot = -1;
        for (u16 i = 0; i <= STATSTAGE_EVASION; ++i)
        {
            mirrorHerbStats[pokemonSlot].boosts[i] = 0;
        }
    }
}
BattleEventHandlerTableEntry MirrorHerbHandlers[] = {
    {EVENT_STAT_STAGE_CHANGE_APPLIED, HandlerMirrorHerbStatChange},
    {EVENT_ACTION_PROCESSING_END, HandlerMirrorHerbActionEnd},
    {EVENT_USE_ITEM, HandlerMirrorHerbUse},
};
BattleEventHandlerTableEntry* EventAddMirrorHerb(int* a1)
{
    *a1 = ARRAY_COUNT(MirrorHerbHandlers);
    return MirrorHerbHandlers;
}

// LOADED DICE
void HandlerLoadedDice(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON))
    {
        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        ItemID heldItem = BattleMon_GetHeldItem(attackingMon);
        if (DoesItemGuaranteeMultiHit(heldItem))
        {
            int hitCount = BattleEventVar_GetValue(VAR_MAX_HIT_COUNT);
            if (hitCount == 5)
            {
                if (BattleRandom(100u) < 50)
                    hitCount = 4;
                else
                    hitCount = 5;
            }

            BattleEventVar_RewriteValue(VAR_HIT_COUNT, hitCount);
            BattleEventVar_RewriteValue(VAR_AVOID_FLAG, 0);
        }
    }
}
BattleEventHandlerTableEntry LoadedDiceHandlers[] = {
    {EVENT_MOVE_HIT_COUNT, HandlerLoadedDice}, // single acc check for non 5 hit moves -> ServerEvent_CheckMultihitHits
};
BattleEventHandlerTableEntry* EventAddLoadedDice(int* a1)
{
    *a1 = ARRAY_COUNT(LoadedDiceHandlers);
    return LoadedDiceHandlers;
}

// PUNCHING GLOVES
BattleEventHandlerTableEntry PunchingGloveHandlers[] = {
    {EVENT_MOVE_POWER, HandlerIronFist}, // checks to contact situations like protective pads (multiple ability handlers)
};
BattleEventHandlerTableEntry* EventAddPunchingGlove(int* a1)
{
    *a1 = ARRAY_COUNT(PunchingGloveHandlers);
    return PunchingGloveHandlers;
}

// COVERT CLOAK
void HandlerCovertCloakConditionCheck(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot != BattleEventVar_GetValue(VAR_ATTACKING_MON) && pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON))
    {
        int moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (IsAffectedBySheerForce(moveID))
        {
            BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
        }
    }
}
void HandlerCovertCloakFlinchCheck(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON))
    {
        BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
    }
}
BattleEventHandlerTableEntry CovertCloakHandlers[] = {
    {EVENT_ADD_CONDITION, HandlerCovertCloakConditionCheck},
    {EVENT_ADD_STAT_STAGE_CHANGE_TARGET, HandlerCovertCloakConditionCheck},
    {EVENT_MOVE_FLINCH_CHANCE, HandlerCovertCloakFlinchCheck},
};
BattleEventHandlerTableEntry* EventAddCovertCloak(int* a1)
{
    *a1 = ARRAY_COUNT(CovertCloakHandlers);
    return CovertCloakHandlers;
}

// FAIRY FEATHER
BattleEventHandlerTableEntry FairyFeatherHandlers[] = {
    {EVENT_MOVE_POWER, HandlerFairyFeather},
};
BattleEventHandlerTableEntry* EventAddFairyFeather(int* a1)
{
    *a1 = ARRAY_COUNT(FairyFeatherHandlers);
    return FairyFeatherHandlers;
}
#endif

// --- DATA ---

ItemEventAddTable ExtItemEventAddTable[] = { // ITEM_EVENT_ADD_TABLE
{IT0149_CHERI_BERRY, EventAddCheriBerry},
{IT0150_CHESTO_BERRY, EventAddChestoBerry},
{IT0152_RAWST_BERRY, EventAddRawstBerry},
{IT0153_ASPEAR_BERRY, EventAddAspearBerry},
{IT0156_PERSIM_BERRY, EventAddPersimBerry},
{IT0151_PECHA_BERRY, EventAddPechaBerry},
{IT0157_LUM_BERRY, EventAddLumBerry},
{IT0154_LEPPA_BERRY, EventAddLeppaBerry},
{IT0155_ORAN_BERRY, EventAddOranBerry},
{IT0043_BERRY_JUICE, EventAddBerryJuice},
{IT0158_SITRUS_BERRY, EventAddSitrusBerry},
{IT0159_FIGY_BERRY, EventAddFigyBerry},
{IT0160_WIKI_BERRY, EventAddWikiBerry},
{IT0161_MAGO_BERRY, EventAddMagoBerry},
{IT0162_AGUAV_BERRY, EventAddAguavBerry},
{IT0163_IAPAPA_BERRY, EventAddIapapaBerry},
{IT0201_LIECHI_BERRY, EventAddLiechiBerry},
{IT0202_GANLON_BERRY, EventAddGanlonBerry},
{IT0203_SALAC_BERRY, EventAddSalacBerry},
{IT0204_PETAYA_BERRY, EventAddPetayaBerry},
{IT0205_APICOT_BERRY, EventAddApicotBerry},
{IT0206_LANSAT_BERRY, EventAddLansatBerry},
{IT0207_STARF_BERRY, EventAddStarfBerry},
{IT0208_ENIGMA_BERRY, EventAddEnigmaBerry},
{IT0184_OCCA_BERRY, EventAddOccaBerry},
{IT0185_PASSHO_BERRY, EventAddPasshoBerry},
{IT0186_WACAN_BERRY, EventAddWacanBerry},
{IT0187_RINDO_BERRY, EventAddRindoBerry},
{IT0188_YACHE_BERRY, EventAddYacheBerry},
{IT0189_CHOPLE_BERRY, EventAddChopleBerry},
{IT0190_KEBIA_BERRY, EventAddKebiaBerry},
{IT0191_SHUCA_BERRY, EventAddShucaBerry},
{IT0192_COBA_BERRY, EventAddCobaBerry},
{IT0193_PAYAPA_BERRY, EventAddPayapaBerry},
{IT0194_TANGA_BERRY, EventAddTangaBerry},
{IT0195_CHARTI_BERRY, EventAddChartiBerry},
{IT0196_KASIB_BERRY, EventAddKasibBerry},
{IT0197_HABAN_BERRY, EventAddHabanBerry},
{IT0198_COLBUR_BERRY, EventAddColburBerry},
{IT0199_BABIRI_BERRY, EventAddBabiriBerry},
{IT0200_CHILAN_BERRY, EventAddChilanBerry},
{IT0210_CUSTAP_BERRY, EventAddCustapBerry},
{IT0209_MICLE_BERRY, EventAddMicleBerry},
{IT0211_JABOCA_BERRY, EventAddJabocaBerry},
{IT0212_ROWAP_BERRY, EventAddRowapBerry},
{IT0214_WHITE_HERB, EventAddWhiteHerb},
{IT0219_MENTAL_HERB, EventAddMentalHerb},
{IT0213_BRIGHT_POWDER, EventAddBrightPowder},
{IT0215_MACHO_BRACE, EventAddMachoBrace},
{IT0217_QUICK_CLAW, EventAddQuickClaw},
{IT0279_LAGGING_TAIL, EventAddLaggingTail},
{IT0316_FULL_INCENSE, EventAddLaggingTail},
{IT0221_KING_S_ROCK, EventAddKingsRock},
{IT0326_RAZOR_CLAW, EventAddScopeLens},
{IT0265_WIDE_LENS, EventAddWideLens},
{IT0232_SCOPE_LENS, EventAddScopeLens},
{IT0276_ZOOM_LENS, EventAddZoomLens},
{IT0255_LAX_INCENSE, EventAddLaxIncense},
{IT0266_MUSCLE_BAND, EventAddMuscleBand},
{IT0267_WISE_GLASSES, EventAddWiseGlasses},
{IT0226_DEEP_SEA_TOOTH, EventAddDeepSeaTooth},
{IT0227_DEEP_SEA_SCALE, EventAddDeepSeaScale},
{IT0257_METAL_POWDER, EventAddMetalPowder},
{IT0274_QUICK_POWDER, EventAddQuickPowder},
{IT0236_LIGHT_BALL, EventAddLightBall},
{IT0256_LUCKY_PUNCH, EventAddLuckyPunch},
{IT0259_STICK, EventAddStick},
{IT0225_SOUL_DEW, EventAddSoulDew},
{IT0258_THICK_CLUB, EventAddThickClub},
{IT0220_CHOICE_BAND, EventAddChoiceBand},
{IT0281_BLACK_SLUDGE, EventAddBlackSludge},
{IT0297_CHOICE_SPECS, EventAddChoiceSpecs},
{IT0287_CHOICE_SCARF, EventAddChoiceScarf},
{IT0222_SILVER_POWDER, EventAddSilverPowder},
{IT0237_SOFT_SAND, EventAddSoftSand},
{IT0238_HARD_STONE, EventAddHardStone},
{IT0239_MIRACLE_SEED, EventAddMiracleSeed},
{IT0240_BLACK_GLASSES, EventAddBlackGlasses},
{IT0241_BLACK_BELT, EventAddBlackBelt},
{IT0242_MAGNET, EventAddMagnet},
{IT0233_METAL_COAT, EventAddMetalCoat},
{IT0243_MYSTIC_WATER, EventAddMysticWater},
{IT0244_SHARP_BEAK, EventAddSharpBeak},
{IT0327_RAZOR_FANG, EventAddRazorFang},
{IT0245_POISON_BARB, EventAddPoisonBarb},
{IT0246_NEVER_MELT_ICE, EventAddNeverMeltIce},
{IT0247_SPELL_TAG, EventAddSpellTag},
{IT0248_TWISTED_SPOON, EventAddTwistedSpoon},
{IT0249_CHARCOAL, EventAddCharcoal},
{IT0250_DRAGON_FANG, EventAddDragonFang},
{IT0251_SILK_SCARF, EventAddSilkScarf},
{IT0314_ODD_INCENSE, EventAddOddIncense},
{IT0315_ROCK_INCENSE, EventAddRockIncense},
{IT0317_WAVE_INCENSE, EventAddWaveIncense},
{IT0254_SEA_INCENSE, EventAddSeaIncense},
{IT0318_ROSE_INCENSE, EventAddRoseIncense},
{IT0275_FOCUS_SASH, EventAddFocusSash},
{IT0230_FOCUS_BAND, EventAddFocusBand},
{IT0268_EXPERT_BELT, EventAddExpertBelt},
{IT0270_LIFE_ORB, EventAddLifeOrb},
{IT0277_METRONOME, EventAddMetronomeItem},
{IT0286_GRIP_CLAW, EventAddGripClaw},
{IT0253_SHELL_BELL, EventAddShellBell},
{IT0269_LIGHT_CLAY, EventAddLightClay},
{IT0271_POWER_HERB, EventAddPowerHerb},
{IT0234_LEFTOVERS, EventAddLeftovers},
{IT0272_TOXIC_ORB, EventAddToxicOrb},
{IT0273_FLAME_ORB, EventAddFlameOrb},
{IT0136_LUSTROUS_ORB, EventAddLustrousOrb},
{IT0135_ADAMANT_ORB, EventAddAdamantOrb},
{IT0278_IRON_BALL, EventAddIronBall},
{IT0280_DESTINY_KNOT, EventAddDestinyKnot},
{IT0282_ICY_ROCK, EventAddIcyRock},
{IT0283_SMOOTH_ROCK, EventAddSmoothRock},
{IT0284_HEAT_ROCK, EventAddHeatRock},
{IT0285_DAMP_ROCK, EventAddDampRock},
{IT0288_STICKY_BARB, EventAddStickyBarb},
{IT0289_POWER_BRACER, EventAddPowerBracer},
{IT0290_POWER_BELT, EventAddPowerBelt},
{IT0291_POWER_LENS, EventAddPowerLens},
{IT0292_POWER_BAND, EventAddPowerBand},
{IT0293_POWER_ANKLET, EventAddPowerAnklet},
{IT0294_POWER_WEIGHT, EventAddPowerWeight},
{IT0298_FLAME_PLATE, EventAddFlamePlate},
{IT0299_SPLASH_PLATE, EventAddSplashPlate},
{IT0300_ZAP_PLATE, EventAddZapPlate},
{IT0301_MEADOW_PLATE, EventAddMeadowPlate},
{IT0302_ICICLE_PLATE, EventAddIciclePlate},
{IT0303_FIST_PLATE, EventAddFistPlate},
{IT0304_TOXIC_PLATE, EventAddToxicPlate},
{IT0305_EARTH_PLATE, EventAddEarthPlate},
{IT0306_SKY_PLATE, EventAddSkyPlate},
{IT0307_MIND_PLATE, EventAddMindPlate},
{IT0308_INSECT_PLATE, EventAddInsectPlate},
{IT0309_STONE_PLATE, EventAddStonePlate},
{IT0310_SPOOKY_PLATE, EventAddSpookyPlate},
{IT0311_DRACO_PLATE, EventAddDracoPlate},
{IT0312_DREAD_PLATE, EventAddDreadPlate},
{IT0313_IRON_PLATE, EventAddIronPlate},
{IT0296_BIG_ROOT, EventAddBigRoot},
{IT0228_SMOKE_BALL, EventAddSmokeBall},
{IT0223_AMULET_COIN, EventAddAmuletCoin},
{IT0319_LUCK_INCENSE, EventAddAmuletCoin},
{IT0112_GRISEOUS_ORB, EventAddGriseousOrb},
{IT0539_FLOAT_STONE, EventAddFloatStone},
{IT0538_EVIOLITE, EventAddEviolite},
{IT0540_ROCKY_HELMET, EventAddRockyHelmet},
{IT0541_AIR_BALLOON, EventAddAirBalloon},
{IT0542_RED_CARD, EventAddRedCard},
{IT0543_RING_TARGET, EventAddRingTarget},
{IT0544_BINDING_BAND, EventAddBindingBand},
{IT0545_ABSORB_BULB, EventAddAbsorbBulb},
{IT0546_CELL_BATTERY, EventAddCellBattery},
{IT0547_EJECT_BUTTON, EventAddEjectButton},
{IT0548_FIRE_GEM, EventAddFireGem},
{IT0549_WATER_GEM, EventAddWaterGem},
{IT0550_ELECTRIC_GEM, EventAddElectricGem},
{IT0551_GRASS_GEM, EventAddGrassGem},
{IT0552_ICE_GEM, EventAddIceGem},
{IT0553_FIGHTING_GEM, EventAddFightingGem},
{IT0554_POISON_GEM, EventAddPoisonGem},
{IT0555_GROUND_GEM, EventAddGroundGem},
{IT0556_FLYING_GEM, EventAddFlyingGem},
{IT0557_PSYCHIC_GEM, EventAddPsychicGem},
{IT0558_BUG_GEM, EventAddBugGem},
{IT0559_ROCK_GEM, EventAddRockGem},
{IT0560_GHOST_GEM, EventAddGhostGem},
{IT0561_DRAGON_GEM, EventAddDragonGem},
{IT0562_DARK_GEM, EventAddDarkGem},
{IT0563_STEEL_GEM, EventAddSteelGem},
{IT0564_NORMAL_GEM, EventAddNormalGem},
#if ADD_NEW_ITEMS
/*Vanilla End*/
{IT0639_WEAKNESS_POLICY, EventAddWeaknessPolicy},
{IT0640_ASSAULT_VEST, EventAddAssaultVest},
{IT0644_PIXIE_PLATE, EventAddPixiePlate},
{IT0648_LUMINOUS_MOSS, EventAddLuminousMoss},
{IT0649_SNOWBALL, EventAddSnowball},
{IT0650_SAFETY_GOGGLES, EventAddSafetyGoggles},
{IT0686_ROSELI_BERRY, EventAddRoseliBerry },
{IT0687_KEE_BERRY, EventAddKeeBerry},
{IT0688_MARANGA_BERRY, EventAddMarangaBerry},
{IT0846_ADRENALINE_ORB, EventAddAdrenalineOrb},
{IT1118_THROAT_SPRAY, EventAddThroatSpray},
{IT1119_EJECT_PACK, EventAddEjectPack},
{IT1121_BLUNDER_POLICY, EventAddBlunderPolicy},
{IT1122_ROOM_SERVICE, EventAddRoomSevice},
{IT1881_ABILITY_SHIELD, EventAddAbilityShield},
{IT1882_CLEAR_AMULET, EventAddClearAmulet},
{IT1886_LOADED_DICE, EventAddLoadedDice},
{IT1883_MIRROR_HERB, EventAddMirrorHerb},
{IT1884_PUNCHING_GLOVE, EventAddPunchingGlove},
{IT1885_COVERT_CLOAK, EventAddCovertCloak},
{IT2401_FAIRY_FEATHER, EventAddFairyFeather},
#endif
}; 

// --- FUNCTIONS ---

// NEW

int ServerEvent_AccuracyMiss(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, MoveID moveID)
{
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    int attackingSlot = BattleMon_GetID(attackingMon);
    BattleEventVar_SetConstValue(NEW_VAR_ATTACKING_MON, attackingSlot);
    int defendingSlot = BattleMon_GetID(defendingMon);
    BattleEventVar_SetConstValue(NEW_VAR_DEFENDING_MON, defendingSlot);
    BattleEventVar_SetConstValue(VAR_MOVE_ID, moveID);
    BattleEventVar_SetRewriteOnceValue(VAR_GENERAL_USE_FLAG, 0);

    BattleEvent_CallHandlers(serverFlow, EVENT_ACCURACY_MISS);
    int forceHit = BattleEventVar_GetValue(VAR_GENERAL_USE_FLAG);
    BattleEventVar_Pop();
    return forceHit;
}


// OVL 167
#if ADD_NEW_ITEMS
// Function that creates the pokemons in battle from the pokemons in the party & trainers
BattleMon* THUMB_BRANCH_BattleMon_Create(PartyPkm* a1, BattlePartySlot battleSlot, HeapID a3)
{
    BattleMon* battleMon; // r4
    signed __int32 StatusCond; // r0
    signed __int32 v7; // r5
    battleMon = (BattleMon*)GFL_HeapAllocate(a3, 0x1F8u, 1, "btl_pokeparam.c", 0xE3u);
    battleMon->battleSlot = battleSlot;
    battleMon->Species = PokeParty_GetParam(a1, PF_Species, 0);
    battleMon->partySrc = a1;
    battleMon->CurrentHP = PokeParty_GetParam(a1, PF_NowHP, 0);
    battleMon->MaxHP = PokeParty_GetParam(a1, PF_MaxHP, 0);
    battleMon->HeldItem = PokeParty_GetParam(a1, PF_Item, 0);
    if (battleMon->HeldItem >= IT_ITEM_AMOUNT) // removed limiter
    {
        battleMon->HeldItem = 0;
    }
    battleMon->UsedItem = 0;
    battleMon->flags &= ~0x20u;
    battleMon->disguiseSrc = 0;
    battleMon->flags &= ~0x40u;
    battleMon->flags &= ~0x80u;
    battleMon->flags = PokeParty_GetParam(a1, PF_Forme, 0) & 0x1F | battleMon->flags & 0xE0;
    battleMon->Ability = PokeParty_GetParam(a1, PF_Ability, 0);
    battleMon->Level = PokeParty_GetParam(a1, PF_Level, 0);
    battleMon->BaseAttack = PML_PersonalGetParamSingle(battleMon->Species, battleMon->flags & 0x1F, Personal_ATK);
    BattleMon_SetupBySrcData(battleMon, a1, 1, 1);
    battleMon->MoveCount = BattleMon_GetNumMoves(battleMon, a1, 1);
    battleMon->UsedMoveCount = 0;
    j_ResetStatStages(&battleMon->statStageParam);
    ClearMoveStatusWork(battleMon, 1);
    StatusCond = GetStatusCond(a1);
    v7 = StatusCond;
    if (StatusCond)
    {
        battleMon->Conditions[StatusCond] = MakeBasicStatus((MoveCondition)StatusCond);
        battleMon->MoveConditionCounter[v7] = 0;
    }
    battleMon->AppearedTurn = 10000;
    battleMon->TurnCount = 0;
    battleMon->SubstituteHP = 0;
    battleMon->PreviousMove = 0;
    battleMon->PreviousMoveID = 0;
    battleMon->PrevMoveType = 17;
    battleMon->ConsecutiveMoveCounter = 0;
    battleMon->comboMoveID = 0;
    battleMon->comboPokeID = 31;
    battleMon->CritStage = 0;
    sys_memset(battleMon->TurnFlag, 0, 2u);
    sys_memset(battleMon->ConditionFlag, 0, 2u);
    MoveDamageRec_Init(battleMon);
    BattleMon_ClearConfrontRecCount(battleMon);
    return battleMon;
}

// Function that adds the events of the battle items
BattleEventItem* THUMB_BRANCH_ItemEvent_AddItemCore(BattleMon* a1, ItemID a2)
{
    __int16 v2;
    unsigned int i;
    unsigned int v5;
    int RealStat;
    int ID;
    const BattleEventHandlerTableEntry* v8;
    int v10[7];

    v2 = a2;
    for (i = 0; i < ARRAY_COUNT(ExtItemEventAddTable); ++i) // Use expanded list to find item events
    {
        v5 = i;
        if (a2 == ExtItemEventAddTable[i].itemID)
        {
            RealStat = BattleMon_GetRealStat(a1, VALUE_SPEED_STAT);
            ID = BattleMon_GetID(a1);
            v8 = ExtItemEventAddTable[v5].func(v10);
            return BattleEvent_AddItem(EVENTITEM_ITEM, v2, EVENTPRI_ITEM_DEFAULT, RealStat, ID, v8, v10[0]);
        }
    }
    return 0;
}

// Protective Pads contact checks
void THUMB_BRANCH_HandlerRockyHelmet(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work)
{
    HandlerParam_Damage* v7;

    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG))
    {
        u16 moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (getMoveFlag(moveID, FLAG_CONTACT))
        {
            int attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
            if (DoesItemPreventContact(BattleMon_GetHeldItem(attackingMon), (MoveID)moveID)) // Protective Pad check
                return;

            u8 itemDamage = CommonGetItemParam(a1, ITSTAT_USE_PARAM);

            v7 = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
            v7->pokeID = attackingSlot;
            v7->damage = DivideMaxHPZeroCheck(attackingMon, itemDamage);
            BattleHandler_StrSetup(&v7->exStr, 2u, 424);
            BattleHandler_AddArg(&v7->exStr, v7->pokeID);
            BattleHandler_PopWork(serverFlow, v7);
        }
    }
}

// Called when checking if a move should miss from accuracy or not
void THUMB_BRANCH_flowsub_CheckWazaAvoid(ServerFlow* serverFlow, MoveParam* moveParam, BattleMon* attackerMon, PokeSet* targetSet)
{
    int setCount = j_PokeSet_GetCountMax_4(targetSet);
    if (setCount != 1 || j_PokeSet_Get_8(targetSet, 0) != attackerMon)
    {
        j_PokeSet_SeekStart_20(targetSet);

        BattleMon* battleMon = j_PokeSet_SeekNext_41(targetSet);
        while (battleMon)
        {
            if (!ServerEvent_SkipAvoidCheck(serverFlow, attackerMon, battleMon, (u16*)moveParam)
                && !ServerEvent_CheckHit(serverFlow, attackerMon, battleMon, moveParam))
            {
                ServerDisplay_MoveAvoid(serverFlow, battleMon);

                if (!ServerEvent_AccuracyMiss(serverFlow, attackerMon, battleMon, (MoveID)moveParam->MoveID)) // Added new event when a move misses
                    j_PokeSet_Remove_10(targetSet, battleMon); // This event can revert accuracy moves after they happen
            }
            
            battleMon = j_PokeSet_SeekNext_41(targetSet);
        }
    }
}

// Called when a pokemon switches in at the start of a battle
int THUMB_BRANCH_ServerControl_AfterSwitchIn(ServerFlow* serverFlow)
{
    int result;
    int hEManagerState;
    BattleMon* currentMon;

    FRONT_POKE_SEEK_WORK seekWork[6];
    PokeSet* pokeSet = &serverFlow->pokeSet_1A68;

    result = 0;
    j_j_PokeSet_Clear(pokeSet);
    FRONT_POKE_SEEK_InitWork(seekWork, serverFlow);
    while (FRONT_POKE_SEEK_GetNext(seekWork, serverFlow, &currentMon))
    {
        char* v2 = (char*)serverFlow + BattleMon_GetID(currentMon);
        if (v2[1985])
        {
            j_j_PokeSet_Add(pokeSet, currentMon);
            v2[1985] = 0;
        }
    }
    j_j_PokeSet_SortBySpeed_0(pokeSet, serverFlow);

    hEManagerState = HEManager_PushState((unsigned int*)&serverFlow->HEManager);
    ServerEvent_AfterSwitchInPrevious(serverFlow);
    if (BattleHandler_Result(serverFlow))
        result = 1;
    HEManager_PopState((unsigned int*)&serverFlow->HEManager, hEManagerState);

    j_j_PokeSet_SeekStart_21(pokeSet);
    currentMon = j_j_PokeSet_SeekNext_43(pokeSet);
    while (currentMon)
    {
        hEManagerState = HEManager_PushState((unsigned int*)&serverFlow->HEManager);
        ServerEvent_SwitchIn(serverFlow, currentMon);
        ServerEvent_ActProcEnd(serverFlow, currentMon, 0); // Added a action end to the first switch ins
        if (BattleHandler_Result(serverFlow))
        {
            result = 1;
        }
        HEManager_PopState((unsigned int*)&serverFlow->HEManager, hEManagerState);

        currentMon = j_j_PokeSet_SeekNext_42(pokeSet);
    }

    hEManagerState = HEManager_PushState((unsigned int*)&serverFlow->HEManager);
    ServerEvent_AfterLastSwitchIn(serverFlow);
    if (BattleHandler_Result(serverFlow))
    {
        result = 1;
    }
    HEManager_PopState((unsigned int*)&serverFlow->HEManager, hEManagerState);

    return result;
}
#endif

// GENERAL PURPOSE
// Prevents weather boost when holding Utility Umbrella
// Modifies crit damage boost
// Calculates terrain damage boost
int THUMB_BRANCH_SAFESTACK_ServerEvent_CalcDamage(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, MoveParam* moveParam,
    int typeEffectiveness, int targetDmgRatio, int criticalFlag, int battleDebugMode, _WORD* destDamage)
{
    int category = PML_MoveGetCategory(moveParam->MoveID);
    int isFixedDamage = 0;
    BattleEventVar_Push();
    BattleEventVar_SetConstValue(VAR_TYPE_EFFECTIVENESS, typeEffectiveness);
    int attackingSlot = BattleMon_GetID(attackingMon);
    BattleEventVar_SetConstValue(VAR_ATTACKING_MON, attackingSlot);
    int defendingSlot = BattleMon_GetID(defendingMon);
    BattleEventVar_SetConstValue(VAR_DEFENDING_MON, defendingSlot);
    BattleEventVar_SetConstValue(VAR_CRITICAL_FLAG, criticalFlag);
    BattleEventVar_SetConstValue(VAR_MOVE_TYPE, moveParam->moveType);
    BattleEventVar_SetConstValue(VAR_MOVE_ID, moveParam->MoveID);
    BattleEventVar_SetConstValue(VAR_MOVE_CATEGORY, category);
    BattleEventVar_SetValue(VAR_FIXED_DAMAGE, 0);
    BattleEvent_CallHandlers(serverFlow, EVENT_MOVE_DAMAGE_PROCESSING_1);
    u32 finalDamage = BattleEventVar_GetValue(VAR_FIXED_DAMAGE);
    if (finalDamage)
    {
        isFixedDamage = 1;
    }
    else
    {
        int power = ServerEvent_GetMovePower(serverFlow, attackingMon, defendingMon, moveParam);
        int attack = ServerEvent_GetAttackPower(serverFlow, attackingMon, defendingMon, moveParam, criticalFlag);
        int defense = ServerEvent_GetTargetDefenses(serverFlow, attackingMon, defendingMon, moveParam, criticalFlag);
        u8 level = BattleMon_GetValue(attackingMon, VALUE_LEVEL);
        int baseDamage = CalcBaseDamage(power, attack, level, defense);
        int damage = baseDamage;
        if (targetDmgRatio != 4096)
        {
            damage = fixed_round(baseDamage, targetDmgRatio);
        }

        int weather = ServerEvent_GetWeather(serverFlow);
        int weatherDmgRatio = WeatherPowerMod(weather, moveParam->moveType);
        if (weatherDmgRatio != 4096)
        {
#if ADD_NEW_ITEMS
            if (!DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(defendingMon))) // dont change damage when holding Utility Umbrella
                damage = fixed_round(damage, weatherDmgRatio);
#else
            damage = fixed_round(damage, weatherDmgRatio);
#endif 
        }

        int terrain = ServerEvent_GetTerrain(serverFlow);
        int terrainDmgRatio = TerrainPowerMod(serverFlow, attackingMon, defendingMon, terrain, moveParam->moveType);
        if (terrainDmgRatio != 4096)
        {
            damage = fixed_round(damage, terrainDmgRatio);
        }

        if (criticalFlag)
        {
#if ADD_CRIT_CHANGES
            damage = (int)((float)damage * CRIT_DMG_BOOST);
#else
            damage *= 2;
#endif
        }
        if (!MainModule_GetDebugFlag() && sub_21AE34C(serverFlow))
        {
            int damageRoll = 85;
            if (!battleDebugMode)
            {
                damageRoll = (100 - BattleRandom(16u));
            }
            damage = damageRoll * damage / 100;
        }
        PokeType moveType = (PokeType)moveParam->moveType;
        if (moveType != TYPE_NULL)
        {
            int stab = ServerEvent_SameTypeAttackBonus(serverFlow, attackingMon, moveType);
            damage = fixed_round(damage, stab);
        }
        int damageAfterType = TypeEffectivenessPowerMod(damage, typeEffectiveness);
        if (category == CATEGORY_PHYSICAL
            && BattleMon_GetStatus(attackingMon) == CONDITION_BURN
            && BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY) != ABIL062_GUTS)
        {
            damageAfterType = 50 * damageAfterType / 100u;
        }
        if (!damageAfterType)
        {
            damageAfterType = 1;
        }
        BattleEventVar_SetMulValue(VAR_RATIO, 4096, 41, 0x20000);
        BattleEventVar_SetValue(VAR_DAMAGE, damageAfterType);
        BattleEvent_CallHandlers(serverFlow, EVENT_MOVE_DAMAGE_PROCESSING_2);
        int damageRatio = BattleEventVar_GetValue(VAR_RATIO);
        int damageAfterProc2 = BattleEventVar_GetValue(VAR_DAMAGE);
        LOWORD(finalDamage) = fixed_round(damageAfterProc2, damageRatio);
    }
    BattleEvent_CallHandlers(serverFlow, EVENT_MOVE_DAMAGE_PROCESSING_END);
    BattleEventVar_Pop();
    *destDamage = finalDamage;
    return isFixedDamage;
}

// Called to check if a status condition should fail
int THUMB_BRANCH_AddConditionCheckFailOverwrite(ServerFlow* serverFlow, BattleMon* battleMon, MoveCondition condition, int a4, char a5)
{
    int pokemonTypePair = BattleMon_GetPokeType(battleMon);

    if (BattleMon_CheckIfMoveCondition(battleMon, condition) && a5 != 2)
    {
        return 1;
    }
    if (condition < CONDITION_CONFUSION && BattleMon_GetStatus(battleMon) && !a5)
    {
        return 3;
    }
    if (ServerEvent_GetWeather(serverFlow) == WEATHER_SUN && condition == CONDITION_FREEZE)
    {
#if ADD_NEW_ITEMS
        if (!DoesItemPreventWeatherEffects(BattleMon_GetHeldItem(battleMon))) // dont block freeze when holding Utility Umbrella
        {
            return 3;
        }
#else
        return 3;
#endif
    }
    if (condition == CONDITION_POISON)
    {
        if (PokeTypePair_HasType(pokemonTypePair, TYPE_STEEL) || PokeTypePair_HasType(pokemonTypePair, TYPE_POISON))
        {
            return 2;
        }
    }
    if (condition == CONDITION_BURN)
    {
        if (PokeTypePair_HasType(pokemonTypePair, TYPE_FIRE))
        {
            return 2;
        }
    }
    if (condition == CONDITION_FREEZE)
    {
        if (PokeTypePair_HasType(pokemonTypePair, TYPE_ICE))
        {
            return 2;
        }
    }
    if (condition == CONDITION_LEECHSEED)
    {
        if (PokeTypePair_HasType(pokemonTypePair, TYPE_GRASS))
        {
            return 2;
        }
    }
    if (condition == CONDITION_YAWN && BattleMon_GetStatus(battleMon))
    {
        return 3;
    }
    if (condition == CONDITION_GASTROACID && BattleMon_GetValue(battleMon, VALUE_ABILITY) == ABIL121_MULTITYPE)
    {
        return 3;
    }
    return 0;
}

// DYNAMIC SPEED
#if ADD_DYNAMIC_SPEED
enum OrderAlteringMoves
{
    OAM_NONE = 0,
    OAM_AFTER_YOU = 1,
    OAM_QUASH = 2
};
u16 GetOrderAlteringMoveVar()
{
    EventWorkSave* eventWork = GameData_GetEventWork(GAME_DATA);
    u16 order_altering_move = *EventWork_GetWkPtr(eventWork, ORDER_ALTERING_MOVE_VAR);

#ifdef DEBUG_PRINT
    switch (order_altering_move)
    {
    case OAM_AFTER_YOU:
        DPRINT("USED AFTER YOU\n");
        break;
    case OAM_QUASH:
        DPRINT("USED QUASH\n");
        break;
    }
#endif

    return order_altering_move;
}
void SetOrderAlteringMoveVar(u16 order_altering_move_var)
{
    EventWorkSave* eventWork = GameData_GetEventWork(GAME_DATA);
    u16* order_altering_move = EventWork_GetWkPtr(eventWork, ORDER_ALTERING_MOVE_VAR);

    *order_altering_move = order_altering_move_var;
}

int THUMB_BRANCH_BattleHandler_InterruptAction(ServerFlow* a1, HandlerParam_InterruptPoke* a2)
{
    if (!ActionOrder_InterruptReserve(a1, a2->pokeID))
    {
        return 0;
    }
    BattleHandler_SetString(a1, &a2->exStr);

    SetOrderAlteringMoveVar(OAM_AFTER_YOU);
    return 1;
}

int THUMB_BRANCH_BattleHandler_SendLast(ServerFlow* a1, HandlerParam_SendLast* a2)
{
    if (!ActionOrder_SendToLast(a1, a2->pokeID))
    {
        return 0;
    }
    BattleHandler_SetString(a1, &a2->exStr);

    SetOrderAlteringMoveVar(OAM_QUASH);
    return 1;
}

void SwapPokemonOrder(ActionOrderWork* action_order, u32* speed_stats, u32* priority, u16 slow_idx, u16 fast_idx)
{
    BattleMon* buffer_mon = action_order[fast_idx].battleMon;
    BattleActionParam* buffer_action = action_order[fast_idx].Action;
    u32 buffer_field_8 = action_order[fast_idx].field_8;
    char buffer_field_C = action_order[fast_idx].field_C;
    char buffer_fDone = action_order[fast_idx].fDone;
    char buffer_field_E = action_order[fast_idx].field_E;
    char buffer_field_F = action_order[fast_idx].field_F;
    u32 buffer_speed = speed_stats[fast_idx];
    u32 buffer_priority = priority[fast_idx];

    action_order[fast_idx].battleMon = action_order[slow_idx].battleMon;
    action_order[slow_idx].battleMon = buffer_mon;

    action_order[fast_idx].Action = action_order[slow_idx].Action;
    action_order[slow_idx].Action = buffer_action;

    action_order[fast_idx].field_8 = action_order[slow_idx].field_8;
    action_order[slow_idx].field_8 = buffer_field_8;

    action_order[fast_idx].field_C = action_order[slow_idx].field_C;
    action_order[slow_idx].field_C = buffer_field_C;

    action_order[fast_idx].fDone = action_order[slow_idx].fDone;
    action_order[slow_idx].fDone = buffer_fDone;

    action_order[fast_idx].field_E = action_order[slow_idx].field_E;
    action_order[slow_idx].field_E = buffer_field_E;

    action_order[fast_idx].field_F = action_order[slow_idx].field_F;
    action_order[slow_idx].field_F = buffer_field_F;

    speed_stats[fast_idx] = speed_stats[slow_idx];
    speed_stats[slow_idx] = buffer_speed;

    priority[fast_idx] = priority[slow_idx];
    priority[slow_idx] = buffer_priority;
}

// Make all pokemos faster or tied with the speed tie loser 1 speed point faster
void BreakSpeedTie(ActionOrderWork* action_order, u32* speed_stats, u32 loser_idx)
{
    for (u32 i = 0; i < 6; ++i)
        if (action_order[i].battleMon->battleSlot != action_order[loser_idx].battleMon->battleSlot)
            if (speed_stats[i] >= speed_stats[loser_idx])
                speed_stats[i] = speed_stats[i] + 1;
}

u32 Dynamic_PokeSet_SortBySpeed(ServerFlow* server_flow, ActionOrderWork* action_order, u32 first_idx, u8 current_idx, short* quash_order, bool turn_start)
{
    u32 poke_amount = server_flow->numActOrder - first_idx; // first_idx is used for "half" turns. Ex. when a mon faints the turn ends regardles of the mons left to move,
    if (poke_amount > 1)                                    // so to keep going, the next "turn" starts with the mon that should have moved next
    {
        u16 fainted_amount = 0;

        u32 isTrickRoom = BattleField_CheckEffect(FLDEFF_TRICK_ROOM);
        u32 speed_stats[6];
        u32 priority[6];
        for (u32 i = first_idx; i < first_idx + poke_amount; ++i)
        {
            if (!BattleMon_IsFainted(action_order[i].battleMon))
            {
                speed_stats[i] = ServerEvent_CalculateSpeed(server_flow, action_order[i].battleMon, isTrickRoom);

                priority[i] = (action_order[i].field_8 >> 16) & 0x3FFFFF;
                int special_priority = ((action_order[i].field_8 >> 13) & 0x7); // special priority takes into account item & ability prio boosts (1 = no added prio)
                priority[i] += (special_priority - 1);
            }
            else
            {
                priority[i] = 0xFFFFFFFF;
                ++fainted_amount;
            }
        }

        u32 start_idx = current_idx + 1; // Skip the mons that have already moved
        u16 changes = 1;
        while (changes != 0)
        {
            changes = 0;
            for (u16 i = first_idx + start_idx; i < first_idx + poke_amount; ++i)
            {
                if (priority[i] != 0xFFFFFFFF) // check if the first mon is fainted
                {
                    if (i + 1 < first_idx + poke_amount)
                    {
                        for (u16 j = i + 1; j < first_idx + poke_amount; ++j)
                        {
                            if (priority[j] != 0xFFFFFFFF) // check if the second mon is fainted
                            {
                                u32 poke1_priority = priority[i];
                                u32 poke2_priority = priority[j];

                                if (poke2_priority > poke1_priority)
                                {
                                    SwapPokemonOrder(action_order, speed_stats, priority, i, j);
                                    ++changes;
                                }
                                else if (poke2_priority == poke1_priority)
                                {
                                    u32 poke1_speed = speed_stats[i];
                                    u32 poke2_speed = speed_stats[j];
                                    if (poke2_speed > poke1_speed || poke2_speed == poke1_speed && BattleRandom(2u))
                                    {
                                        SwapPokemonOrder(action_order, speed_stats, priority, i, j);
                                        ++changes;

                                        if (poke2_speed == poke1_speed) // completly break speed tie to avoid an infinite loop
                                            BreakSpeedTie(action_order, speed_stats, i);
                                    }
                                    if (speed_stats[j] == speed_stats[i]) // also break speed tie if the second lost
                                        BreakSpeedTie(action_order, speed_stats, j);
                                }
                            }
                        }
                    }
                }
            }
        }

        u16 quash_count = 0;
        for (u16 i = 0; i < 6; ++i)
        {
            if (quash_order[i] < 0)
                break;

            for (u16 j = first_idx + start_idx; j < first_idx + poke_amount - 1; ++j) // loop from the "current" to the one before the last one, the last can't be slower :(
                if (action_order[j].battleMon->battleSlot == quash_order[i])
                    SwapPokemonOrder(action_order, speed_stats, priority, j, j + 1);

            ++quash_count;
        }

        if (turn_start || quash_count) // push fainted mons to the end of the array at the start of the turn
            for (u16 i = 0; i < fainted_amount; ++i) // I only rearrange this when quash is used because it looks better in the debug message, not really necesary
                for (u32 j = first_idx; j < first_idx + poke_amount - 1; ++j)
                    if (BattleMon_IsFainted(action_order[j].battleMon))
                        SwapPokemonOrder(action_order, speed_stats, priority, j, j + 1);

        //DPRINT("DYNAMIC SPEED:\n");
        //for (u32 i = first_idx; i < first_idx + poke_amount; ++i)
        //{
        //    if (!BattleMon_IsFainted(action_order[i].battleMon))
        //    {
        //        DPRINTF("[ ID: %d | SPECIES: %d | PRIORITY: %d | SPEED: %d ]\n", action_order[i].battleMon->battleSlot, action_order[i].battleMon->Species, priority[i], speed_stats[i]);
        //    }
        //    else
        //    {
        //        DPRINTF("[ ID:% d | SPECIES : % d | IS FAINTED ]\n", action_order[i].battleMon->battleSlot, action_order[i].battleMon->Species);
        //    }
        //}
        //DPRINT("---\n");
    }

    return poke_amount;
}

unsigned int THUMB_BRANCH_ServerFlow_ActOrderProcMain(ServerFlow* a1, unsigned int a2)
{
    unsigned int current_idx; // r4
    ActionOrderWork* action_order_work; // r7
    int v5; // r6
    int Action; // r0
    BattleMon* PokeParam; // r0
    bool v8; // r6
    BattleMon* v9; // r0
    int flowResult; // r0
    BattleMon* v12; // r0
    int v13; // r4
    int Count_1; // r4
    int v15; // [sp+0h] [bp-28h]
    int v16; // [sp+4h] [bp-24h]
    BattleActionParam* v17; // [sp+8h] [bp-20h]
    char* p_numActOrder; // [sp+Ch] [bp-1Ch]

    v15 = 0;
    current_idx = a2;
    if (a2 >= (unsigned __int8)a1->numActOrder)
    {
    LABEL_14:
        if (!a1->flowResult)
        {
            v13 = (unsigned __int8)ServerControl_TurnCheck(a1);
            if (ServerControl_CheckMatchup(a1))
            {
                a1->flowResult = 4;
                return (unsigned __int8)a1->numActOrder;
            }
            if (v13)
            {
                a1->flowResult = 3;
                return (unsigned __int8)a1->numActOrder;
            }
            Count_1 = j_j_DeadRec_GetCount_1((int)&a1->faintRecord, 0);
            if (Handler_IsPosOpenForRevivedMon(a1) || Count_1)
            {
                ServerFlow_ReqChangePokeForServer(a1, (unsigned char*)&a1->field_4CE);
                ServerDisplay_IllusionSet(a1, (unsigned __int8*)&a1->field_4CE);
                a1->flowResult = 2;

                return (unsigned __int8)a1->numActOrder;
            }
            a1->flowResult = 0;
        }
        return (unsigned __int8)a1->numActOrder;
    }
    else
    {
        action_order_work = a1->actionOrderWork;
        p_numActOrder = &a1->numActOrder;

        short quash_order[6];
        for (u16 i = 0; i < 6; ++i)
            quash_order[i] = -1;

        Dynamic_PokeSet_SortBySpeed(a1, action_order_work, a2, current_idx, quash_order, true); // pre-calc speed to take into account switch-in speed modifiers (weather...)

        while (1)
        {
            v5 = 4 * current_idx;
            BattleMon* battle_mon = action_order_work[current_idx].battleMon;
            if (!BattleMon_IsFainted(battle_mon))
            {
                BattleActionParam* battle_action = action_order_work[current_idx].Action;

                Action = BattleAction_GetAction(battle_action);
                if (v15 == 6 && Action != 6)
                {
                    ServerControl_CheckActivation(a1);
                    sub_219FB7C(a1, (int)&action_order_work[current_idx], (unsigned __int8)*p_numActOrder - current_idx);
                }
                v15 = ActionOrder_Proc(a1, (int)&action_order_work[current_idx]);

                u16 order_altering_move = GetOrderAlteringMoveVar();
                if (order_altering_move == OAM_QUASH) // Quash: this move overrides the last pokemon on the list, so we save it to always keep it there
                {
                    u16 free_slot = 0;
                    for (free_slot; free_slot < 6; ++free_slot)
                        if (quash_order[free_slot] == -1)
                            break;

                    BattleMon* last_mon = action_order_work[*p_numActOrder - 1].battleMon;
                    if (!BattleMon_IsFainted(last_mon))
                    {
                        quash_order[free_slot] = last_mon->battleSlot;
                    }
                }

                if (order_altering_move != OAM_AFTER_YOU) // After You: this move overrides the next pokemon to move, we don't need to calc speed this iteration
                    Dynamic_PokeSet_SortBySpeed(a1, action_order_work, a2, current_idx, quash_order, false); // re-calc speed
                else
                {
                    DPRINT("AFTER YOU ORDER:\n");
                    for (u32 i = 0; i < *p_numActOrder; ++i)
                    {
                        if (!BattleMon_IsFainted(action_order_work[i].battleMon))
                        {
                            DPRINTF("[ ID: %d | SPECIES: %d ]\n", action_order_work[i].battleMon->battleSlot, action_order_work[i].battleMon->Species);
                        }
                        else
                        {
                            DPRINTF("[ ID:% d | SPECIES : % d | IS FAINTED ]\n", action_order_work[i].battleMon->battleSlot, action_order_work[i].battleMon->Species);
                        }
                    }
                    DPRINT("---\n");
                }
                
                SetOrderAlteringMoveVar(0);
                v16 = (unsigned __int8)ServerControl_CheckExpGet(a1);
                PokeParam = PokeCon_GetBattleMon(a1->pokeCon, 0);
                CheckHPDebug(PokeParam);
                v8 = ServerControl_CheckMatchup(a1);
                v9 = PokeCon_GetBattleMon(a1->pokeCon, 0);
                CheckHPDebug(v9);
            }

            if (v8)
            {
                a1->flowResult = 4;
                return current_idx + 1;
            }
            flowResult = a1->flowResult;
            if (flowResult == 6 || flowResult == 1)
            {
                return current_idx + 1;
            }
            if (v16)
            {
                break;
            }
            if (++current_idx >= (unsigned __int8)*p_numActOrder)
            {
                goto LABEL_14;
            }
        }
        v12 = PokeCon_GetBattleMon(a1->pokeCon, 0);
        CheckHPDebug(v12);
        a1->flowResult = 3;
        return current_idx + 1;
    }
}
#endif

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
    if (MainModule_GetBattleType(serverFlow->mainModule) == BtlType_Trainer)
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
            PokeParam = PokeCon_GetBattleMon(serverFlow->pokeCon, ConfrontedPokeID);
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

            if (!IsTrainerOT(&pPkm->Base, pTrainerInfo))
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

#if ADD_LEVEL_CAP
    u32 lvl_cap = GetLvlCap();
#else
    u32 lvl_cap = 100;
#endif
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

}