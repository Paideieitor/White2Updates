#ifndef __BATTLE_EVENTS_H
#define __BATTLE_EVENTS_H

#include "defs.h"

struct ServerFlow;

enum BattleEventVar
{
	VAR_NULL = 0x0,
	VAR_1 = 0x1,
	VAR_MON_ID = 0x2,
	VAR_ATTACKING_MON = 0x3,
	VAR_DEFENDING_MON = 0x4,
	VAR_TARGET_COUNT = 0x5,
	VAR_TARGET_MON_ID = 0x6,
	VAR_7 = 0x7,
	VAR_8 = 0x8,
	VAR_9 = 0x9,
	VAR_A = 0xA,
	VAR_B = 0xB,
	VAR_ACTION = 0xC,
	VAR_POKE_POS = 0xD,
	VAR_ORIGINAL_TARGET = 0xE,
	VAR_PREVIOUS_ABILITY = 0xF,
	VAR_NEXT_ABILITY = 0x10,
	VAR_PRIORITY = 0x11,
	VAR_MOVE_ID = 0x12,
	VAR_MOVE_ID_2 = 0x13,
	VAR_ORIGINAL_MOVE = 0x14,
	VAR_POKE_TYPE = 0x15,
	VAR_MOVE_TYPE = 0x16,
	VAR_MOVE_INDEX = 0x17,
	VAR_MOVE_PRIORITY = 0x18,
	VAR_MOVE_SERIAL = 0x19,
	VAR_MOVE_CATEGORY = 0x1A,
	VAR_TARGET_TYPE = 0x1B,
	VAR_USER_TYPE = 0x1C,
	VAR_CONDITION_ID = 0x1D,
	VAR_CONDITION_ADDRESS = 0x1E,
	VAR_MOVE_EFFECT = 0x1F,
	VAR_VOLUME = 0x20,
	VAR_SEMI_INVULN_TYPE = 0x21,
	VAR_FAIL_CAUSE = 0x22,
	VAR_23 = 0x23,
	VAR_EFFECT_TURN_COUNT = 0x24,
	VAR_DEFAULT_EFFECT_CHANCE = 0x25,
	VAR_EFFECT_CHANCE = 0x26,
	VAR_ACCURACY_STAGE = 0x27,
	VAR_EVASION_STAGE = 0x28,
	VAR_MAX_HIT_COUNT = 0x29,
	VAR_HIT_COUNT = 0x2A,
	VAR_ACCURACY = 0x2B,
	VAR_CRIT_STAGE = 0x2C,
	VAR_ITEM = 0x2D,
	VAR_ITEM_REACTION = 0x2E,
	VAR_SPEED = 0x2F,
	VAR_MOVE_POWER = 0x30,
	VAR_MOVE_POWER_RATIO = 0x31,
	VAR_DAMAGE = 0x32,
	VAR_POWER = 0x33,
	VAR_GUARD = 0x34,
	VAR_RATIO = 0x35,
	VAR_36 = 0x36,
	VAR_FIXED_DAMAGE = 0x37,
	VAR_TYPE_EFFECTIVENESS = 0x38,
	VAR_WEATHER = 0x39,
	VAR_ENDURE = 0x3A,
	VAR_SWAP_POKE_ID = 0x3B,
	VAR_BATTLE_MON_STAT = 0x3C,
	VAR_BATTLE_MON_STAT_SWAP_FLAG = 0x3D,
	VAR_ENABLE_MODE = 0x3E,
	VAR_WORK_ADDRESS = 0x3F,
	VAR_NO_EFFECT_FLAG = 0x40,
	VAR_MOVE_FAIL_FLAG = 0x41,
	VAR_AVOID_FLAG = 0x42,
	VAR_43 = 0x43,
	VAR_TYPE_MATCH_FLAG = 0x44,
	VAR_CRITICAL_FLAG = 0x45,
	VAR_SUBSTITUTE_FLAG = 0x46,
	VAR_SHIELD_DUST_FLAG = 0x47,
	VAR_SHEER_FORCE_FLAG = 0x48,
	VAR_49 = 0x49,
	VAR_TRICK_ROOM_FLAG = 0x4A,
	VAR_NO_TYPE_EFFECTIVENESS = 0x4B,
	VAR_SET_TYPE_EFFECTIVENESS = 0x4C,
	VAR_DELAY_ATTACK_FLAG = 0x4D,
	VAR_MAGIC_COAT_FLAG = 0x4E,
	VAR_MESSAGE_FLAG = 0x4F,
	VAR_GENERAL_USE_FLAG = 0x51,
	VAR_SIDE = 0x52,
	VAR_SIDE_EFFECT = 0x53,
};

enum BattleEventType
{
	EVENT_NULL = 0x0,
	EVENT_ACTION_PROCESSING_START = 0x1,
	EVENT_ACTION_PROCESSING_END = 0x2,
	EVENT_MOVE_SEQUENCE_START = 0x3,
	EVENT_MOVE_SEQUENCE_END = 0x4,
	EVENT_BYPASS_SUBSTITUTE = 0x5,
	EVENT_CHECK_DELAYED_MOVE = 0x6,
	EVENT_DECIDE_DELAYED_MOVE = 0x7,
	EVENT_MOVE_SEQUENCE_STEAL = 0x8,
	EVENT_MOVE_SEQUENCE_REFLECT = 0x9,
	EVENT_A = 0xA,
	EVENT_SKIP_RUN_CALC = 0xB,
	EVENT_PREVENT_RUN = 0xC,
	EVENT_RUN_EXIT_MESSAGE = 0xD,
	EVENT_CHECK_SLEEP = 0xE,
	EVENT_CHECK_SPECIAL_PRIORITY = 0xF,
	EVENT_GET_MOVE_PRIORITY = 0x11,
	EVENT_CHECK_FLOATING = 0x12,
	EVENT_CALC_SPEED = 0x13,
	EVENT_14 = 0x14,
	EVENT_BEFORE_ATTACKS = 0x15,
	EVENT_MOVE_REQUEST_FOR_ACTION_ORDER = 0x16,
	EVENT_MOVE_REQUEST_CHECK_FAIL = 0x17,
	EVENT_MOVE_REQUEST_PARAM = 0x18,
	EVENT_MOVE_REQUEST_MESSAGE = 0x19,
	EVENT_CHECK_MOVE_STEAL = 0x1A,
	EVENT_FLOATING_IMMUNE_TO_MOVE = 0x1B,
	EVENT_SKIP_AVOID_CHECK = 0x1C,
	EVENT_CHECK_MOVE_FAIL = 0x1D,
	EVENT_MOVE_EXECUTE_CHECK1 = 0x1E,
	EVENT_MOVE_EXECUTE_CHECK2 = 0x1F,
	EVENT_MOVE_EXECUTE_CHECK3 = 0x20,
	EVENT_MOVE_EXECUTE_FAIL = 0x21,
	EVENT_CHOOSE_MOVE = 0x22,
	EVENT_BREAK_OPPONENT_GUARD = 0x23,
	EVENT_MOVE_EXECUTE_START = 0x24,
	EVENT_MOVE_EXECUTE_EFFECTIVE = 0x25,
	EVENT_MOVE_EXECUTE_NOEFFECT = 0x26,
	EVENT_MOVE_EXECUTE_END = 0x27,
	EVENT_MOVE_PARAM = 0x28,
	EVENT_DECIDE_TARGET = 0x29,
	EVENT_REDIRECT_TARGET = 0x2A,
	EVENT_REDIRECT_TARGETEND = 0x2B,
	EVENT_NOEFFECT_CHECK = 0x2C,
	EVENT_ABILITY_CHECK_NO_EFFECT = 0x2D,
	EVENT_CHECK_PROTECT_BREAK = 0x2E,
	EVENT_AVOID_MOVE = 0x2F,
	EVENT_CHECK_DAMAGE_TO_RECOVER = 0x30,
	EVENT_APPLY_DAMAGE_TO_RECOVER = 0x31,
	EVENT_BYPASS_ACCURACY_CHECK = 0x32,
	EVENT_MOVE_ACCURACY_STAGE = 0x33,
	EVENT_MOVE_ACCURACY = 0x34,
	EVENT_MOVE_HIT_COUNT = 0x35,
	EVENT_CRITICAL_CHECK = 0x36,
	EVENT_MOVE_BASE_POWER = 0x37,
	EVENT_MOVE_POWER = 0x38,
	EVENT_BEFORE_ATTACKER_POWER = 0x39,
	EVENT_BEFORE_DEFENDER_GUARD = 0x3A,
	EVENT_ATTACKER_POWER = 0x3B,
	EVENT_DEFENDER_GUARD = 0x3C,
	EVENT_CHECK_TYPE_EFFECTIVENESS_ENABLE = 0x3D,
	EVENT_CHECK_TYPE_EFFECTIVENESS = 0x3E,
	EVENT_REWRITE_TYPE_EFFECTIVENESS = 0x3F,
	EVENT_TYPE_MATCH_CHECK = 0x40,
	EVENT_SAME_TYPE_ATTACK_BONUS = 0x41,
	EVENT_ATTACKER_TYPE = 0x42,
	EVENT_DEFENDER_TYPE = 0x43,
	EVENT_AFTER_DAMAGE_REACTION = 0x44,
	EVENT_DETERMINE_MOVE_DAMAGE = 0x45,
	EVENT_MOVE_DAMAGE_PROCESSING_1 = 0x46,
	EVENT_MOVE_DAMAGE_PROCESSING_2 = 0x47,
	EVENT_MOVE_DAMAGE_PROCESSING_END = 0x48,
	EVENT_MOVE_DAMAGE_PROCESSING_FINAL = 0x49,
	EVENT_PREVIOUS_MOVE_REACTION = 0x4A,
	EVENT_MOVE_DAMAGE_REACTION_1 = 0x4B,
	EVENT_MOVE_DAMAGE_REACTION_2 = 0x4C,
	EVENT_MOVE_DAMAGE_SIDE_AFTER = 0x4D,
	EVENT_DECREMENT_PP = 0x4E,
	EVENT_DECREMENT_PP_DONE = 0x4F,
	EVENT_CALC_RECOIL = 0x50,
	EVENT_ADD_STAT_STAGE_CHANGE_TARGET = 0x51,
	EVENT_ADD_STAT_STAGE_CHANGE_USER = 0x52,
	EVENT_SWITCH_OUT_INTERRUPT = 0x53,
	EVENT_SWITCH_OUT_END = 0x54,
	EVENT_SWITCH_IN = 0x55,
	EVENT_SWITCH_IN_PREVIOUS = 0x56,
	EVENT_AFTER_LAST_SWITCHIN = 0x57,
	EVENT_CHECK_ACTIVATION = 0x58,
	EVENT_GET_STAT_STAGE_CHANGE_VALUE = 0x59,
	EVENT_STAT_STAGE_CHANGE = 0x5A,
	EVENT_STAT_STAGE_CHANGE_LAST_CHECK = 0x5B,
	EVENT_STAT_STAGE_CHANGE_FAIL = 0x5C,
	EVENT_STAT_STAGE_CHANGE_APPLIED = 0x5D,
	EVENT_MOVE_STAT_STAGE_CHANGE_APPLIED = 0x5E,
	EVENT_MOVE_CONDITION_TURN_COUNT = 0x5F,
	EVENT_MOVE_CONDITION_SPECIAL = 0x60,
	EVENT_MOVE_CONDITION_MESSAGE = 0x61,
	EVENT_MOVE_CONDITION_PARAM = 0x62,
	EVENT_ADD_CONDITION_TYPE = 0x63,
	EVENT_ADD_CONDITION = 0x64,
	EVENT_ADD_CONDITION_CHECK_FAIL = 0x65,
	EVENT_ADD_CONDITION_APPLIED = 0x66,
	EVENT_ADD_CONDITION_FAIL = 0x67,
	EVENT_ADD_BASIC_STATUS = 0x68,
	EVENT_MOVE_CONDITION_APPLIED = 0x69,
	EVENT_ABILITY_NULLIFIED = 0x6A,
	EVENT_CONDITION_DAMAGE = 0x6B,
	EVENT_MOVE_FLINCH_CHANCE = 0x6C,
	EVENT_FLINCH_CHECK = 0x6D,
	EVENT_FLINCH_FAIL = 0x6E,
	EVENT_FLINCH_APPLIED = 0x6F,
	EVENT_OHKO_CHECK = 0x70,
	EVENT_OHKO_PREVENT = 0x71,
	EVENT_USE_ITEM = 0x72,
	EVENT_USE_ITEM_TEMP = 0x73,
	EVENT_ENDURE_CHECK = 0x74,
	EVENT_ENDURE = 0x75,
	EVENT_TURN_CHECK_BEGIN = 0x76,
	EVENT_TURN_CHECK_END = 0x77,
	EVENT_TURN_CHECK_DONE = 0x78,
	EVENT_NOTIFY_AIR_LOCK = 0x79,
	EVENT_WEATHER_CHECK = 0x7A,
	EVENT_WEIGHT_RATIO = 0x7B,
	EVENT_MOVE_WEATHER_TURN_COUNT = 0x7C,
	EVENT_WEATHER_CHANGE = 0x7D,
	EVENT_AFTER_WEATHER_CHANGE = 0x7E,
	EVENT_WEATHER_REACTION = 0x7F,
	EVENT_ENABLE_SIMPLE_DAMAGE = 0x80,
	EVENT_DAMAGE_PROCESSING_START = 0x81,
	EVENT_DAMAGE_PROCESSING_END_PREVIOUSHIT = 0x82,
	EVENT_DAMAGE_PROCESSING_END_HIT_REAL = 0x83,
	EVENT_DAMAGE_PROCESSING_END_HIT_1 = 0x84,
	EVENT_DAMAGE_PROCESSING_END_HIT_2 = 0x85,
	EVENT_DAMAGE_PROCESSING_END_HIT_3 = 0x86,
	EVENT_DAMAGE_PROCESSING_END_HIT_4 = 0x87,
	EVENT_DAMAGE_PROCESSING_END = 0x88,
	EVENT_BEFORE_ABILITY_CHANGE = 0x89,
	EVENT_AFTER_ABILITY_CHANGE = 0x8A,
	EVENT_CHECK_FORCE_SWITCH = 0x8B,
	EVENT_CALC_DRAIN = 0x8C,
	EVENT_CALC_DRAIN_END = 0x8D,
	EVENT_RECOVER_HP = 0x8F,
	EVENT_AFTER_ITEM_EQUIP = 0x90,
	EVENT_CHECK_ITEM_REACTION = 0x91,
	EVENT_ITEM_CONSUMED = 0x92,
	EVENT_CHECK_CHARGE_UP_FAIL = 0x93,
	EVENT_CHECK_CHARGE_UP_SKIP = 0x94,
	EVENT_CHARGE_UP_START = 0x95,
	EVENT_CHARGE_UP_START_DONE = 0x96,
	EVENT_CHARGE_UP_SKIP = 0x97,
	EVENT_CHARGE_UP_END = 0x98,
	EVENT_CHECK_HIDING = 0x99,
	EVENT_HELD_ITEM_CHECK = 0x9A,
	EVENT_HELD_ITEM_FAIL = 0x9B,
	EVENT_HELD_ITEM_DECIDE = 0x9C,
	EVENT_ITEM_REWRITE_DONE = 0x9D,
	EVENT_CALL_FIELD_EFFECT = 0x9E,
	EVENT_CHECK_SIDE_EFFECT_PARAM = 0x9F,
	EVENT_UNCATEGORIZED_MOVE = 0xA0,
	EVENT_UNCATEGORIZED_MOVE_NO_TARGET = 0xA1,
	EVENT_COMBINED_MOVE_CHECK = 0xA2,
	EVENT_NOTIFY_FAINTED = 0xA3,
	EVENT_AFTER_MOVE = 0xA4,
};

struct BattleEventItem;

typedef void(*BattleEventHandler)(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work);

struct BattleEventHandlerTableEntry
{
	BattleEventType eventType;
	BattleEventHandler handler;
};

typedef b32(*BattleEventSkipCheckHandler)(BattleEventItem* item, void* work, u32 factorType, u32 eventType, u16 subID, u8 pokeID);

enum BattleEventItemType
{
	EVENTITEM_MOVE = 0x0,
	EVENTITEM_POS = 0x1,
	EVENTITEM_SIDE = 0x2,
	EVENTITEM_FIELD = 0x3,
	EVENTITEM_ABILITY = 0x4,
	EVENTITEM_ITEM = 0x5,
	EVENTITEM_ISOLATED = 0x6,
	EVENTITEM_MAX = 0x7,
};

enum BattleEventPriority
{
	EVENTPRI_MOVE_DEFAULT = 0x0,
	EVENTPRI_POS_DEFAULT = 0x1,
	EVENTPRI_SIDE_DEFAULT = 0x2,
	EVENTPRI_FIELD_DEFAULT = 0x3,
	EVENTPRI_ABILITY_POISON_TOUCH = 0x4,
	EVENTPRI_ABILITY_DEFAULT = 0x5,
	EVENTPRI_ITEM_DEFAULT = 0x6,
	EVENTPRI_ABILITY_STALL = 0x7,
	EVENTPRI_MAX = 0x8,
};

struct SWAN_ALIGNED(4) BattleEventItem
{
	BattleEventItem* prev;
	BattleEventItem* next;
	const BattleEventHandlerTableEntry* handlerTable;
	BattleEventSkipCheckHandler skipCheckHandler;
	BattleEventItemType factorType;
	u32 priority;
	u32 flags;
	u32 work[7];
	u16 subID;
	u8 dependID;
	u8 pokeID;
};

extern "C" BattleEventItem * BattleEvent_AddItem(BattleEventItemType eventType, u16 subID, BattleEventPriority mainPriority, u32 subPriority, u32 dependID, const BattleEventHandlerTableEntry * handlerTable, u16 NumHandlers);
extern "C" void BattleEventItem_Remove(BattleEventItem* item);

extern "C" BattleEventItem* ItemEvent_AddItem(BattleMon* battleMon);
extern "C" void ItemEvent_RemoveItem(BattleMon* battleMon);
extern "C" BattleEventItem* ItemEvent_AddItemCore(BattleMon* battleMon, ITEM_ID itemID);
extern "C" void BattleEventItem_SetTempItemFlag(BattleEventItem* item);
extern "C" void BattleEventItem_SetWorkValue(BattleEventItem* item, u32 idx, u32 value);
extern "C" void ItemEvent_PushRun(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot);
extern "C" void BattleEventItem_AttachSkipCheckHandler(BattleEventItem* item, BattleEventSkipCheckHandler handler);

extern "C" void BattleEventVar_Push();
extern "C" void BattleEventVar_Pop();
extern "C" void BattleEventVar_SetValue(BattleEventVar eventVar, int value);
extern "C" void BattleEventVar_SetConstValue(BattleEventVar eventVar, int value);
extern "C" void BattleEventVar_SetMulValue(BattleEventVar eventVar, int value1, int value2, int value3);
extern "C" u32 BattleEventVar_RewriteValue(BattleEventVar battleEvent, int value);
extern "C" void BattleEventVar_SetRewriteOnceValue(BattleEventVar battleEvent, int value);
extern "C" void BattleEventVar_MulValue(BattleEventVar battleEvent, int value);
extern "C" int BattleEventVar_GetValue(BattleEventVar eventVar);
extern "C" b32 BattleEventVar_GetValueIfExist(BattleEventVar battleEvent, u32* a2);

extern "C" void BattleEvent_CallHandlers(ServerFlow* serverFlow, BattleEventType event);
extern "C" void BattleEvent_ForceCallHandlers(ServerFlow* serverFlow, BattleEventType event);

extern "C" b32 HandlerUnnerveSkipCheck(BattleEventItem* item, void* work, u32 factorType, u32 eventType, u16 subID, u8 pokeID);

#endif // __BATTLE_EVENTS_H

