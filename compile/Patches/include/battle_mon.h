#ifndef __BATTLE_MON_H
#define __BATTLE_MON_H

#include "defs.h"

#include "items.h"
#include "conditions.h"
#include "types.h"
#include "species.h"
#include "stat_stage.h"

#include "pml/poke_party.h"

extern "C" u32 PokeParty_GetParam(PartyPkm* pPkm, PkmField field, void* extra);

struct SWAN_ALIGNED(1) StatStageParam
{
    u8 AttackStage;
    u8 DefenseStage;
    u8 SpAttackStage;
    u8 SpDefenseStage;
    u8 SpeedStage;
    u8 AccuracyStage;
    u8 EvasionStage;
};

struct SWAN_ALIGNED(2) MoveCore
{
    u16 moveID;
    u8 currentPP;
    u8 maxPP;
    u8 PPUpCount;
    u8 usedFlag;
};

struct SWAN_ALIGNED(2) MoveSet
{
    MoveCore truth;
    MoveCore surface;
    u8 fLinked;
};

struct MoveDamageRec
{
    u16 moveID;
    u16 damage;
    u8 damageType;
    u8 moveType;
    u8 pokeID;
    u8 pokePos;
};

enum BattleMonValue
{
    VALUE_NULL = 0x0,
    VALUE_ATTACK_STAGE = 0x1,
    VALUE_DEFENSE_STAGE = 0x2,
    VALUE_SPECIAL_ATTACK_STAGE = 0x3,
    VALUE_SPECIAL_DEFENSE_STAGE = 0x4,
    VALUE_SPEED_STAGE = 0x5,
    VALUE_ACCURACY_STAGE = 0x6,
    VALUE_EVASION_STAGE = 0x7,
    VALUE_ATTACK_STAT = 0x8,
    VALUE_DEFENSE_STAT = 0x9,
    VALUE_SPECIAL_ATTACK_STAT = 0xA,
    VALUE_SPECIAL_DEFENSE_STAT = 0xB,
    VALUE_SPEED_STAT = 0xC,
    VALUE_CURRENT_HP = 0xD,
    VALUE_MAX_HP = 0xE,
    VALUE_LEVEL = 0xF,
    VALUE_ABILITY = 0x10,
    VALUE_EFFECTIVE_ABILITY = 0x11,
    VALUE_SEX = 0x12,
    VALUE_FORM = 0x13,
    VALUE_EXPERIENCE = 0x14,
    VALUE_BASE_ATTACK = 0x15,
};

struct SWAN_ALIGNED(4) BattleMon
{
    PartyPkm* partySrc;
    PartyPkm* disguiseSrc;
    u32 experience;
    u16 species;
    u16 maxHP;
    u16 currentHP;
    u16 heldItem;
    u16 usedItem;
    u16 ability;
    u8 level;
    u8 battleSlot;
    u8 baseAttack;
    u8 flags;
    ConditionData conditions[36];
    u8 moveConditionCounter[36];
    u8 confrontRecCount;
    u8 confrontRec[24];
    u8 gapE9[5];
    u16 attack;
    u16 defense;
    u16 specialAttack;
    u16 specialDefense;
    u16 speed;
    u8 Type1;
    u8 Type2;
    u8 Sex;
    u8 field_FB;
    StatStageParam statStageParam;
    u8 field_103;
    MoveSet moves[4];
    u16 currentAbility;
    u16 weight;
    u8 moveCount; // 0 to 4, amount of move slots that are not empty
    u8 form;
    u8 critStage;
    u8 usedMoveCount; // 0 to 4, amount of moves used from the 4 it has
    u8 prevMoveType;
    u8 field_145;
    u16 turnCount;
    u16 appearedTurn;
    u16 previousMove;
    u16 previousMoveID;
    u16 consecutiveMoveCounter;
    u16 field_150;
    u8 prevTargetPos;
    u8 turnFlag[2];
    u8 conditionFlag[2];
    u8 counters[5];
    MoveDamageRec damageRec[3][6];
    u8 damageRecCount[3];
    u8 damageRecTurn;
    u8 damageRecPtr;
    u8 field_1F1;
    u16 substituteHP;
    u16 comboMoveID;
    u8 comboPokeID;
    u8 field_1F7;
};

struct BattleParty
{
    BattleMon* members[6];
    u8 memberCount;
    u8 numCoverPos;
};

extern "C" u32 BattleMon_GetID(BattleMon* battleMon);
extern "C" b32 BattleMon_IsFainted(BattleMon* battleMon);
extern "C" ItemID BattleMon_GetHeldItem(BattleMon* battleMon);
extern "C" u32 BattleMon_GetValue(BattleMon* battleMon, BattleMonValue value);
extern "C" CONDITION BattleMon_GetStatus(BattleMon* battleMon);
extern "C" u32 BattleMon_GetConfrontRecCount(BattleMon* battleMon);
extern "C" u32 BattleMon_GetConfrontedPokeID(BattleMon* battleMon, u32 confronted);
extern "C" PartyPkm* BattleMon_GetSrcData(BattleMon* battleMon);
extern "C" b32 BattleMon_CheckIfMoveCondition(BattleMon* battleMon, CONDITION condition);
extern "C" b32 BattleMon_IsFullHP(BattleMon* battleMon);
extern "C" b32 BattleMon_CanBattle(BattleMon* battleMon);
extern "C" b32 BattleMon_GetConditionFlag(BattleMon* battleMon, ConditionFlag conditionFlag);
extern "C" u16 BattleMon_GetPokeType(BattleMon* battleMon);
extern "C" b32 BattleMon_HasType(BattleMon* battleMon, POKE_TYPE type);
extern "C" SPECIES BattleMon_GetSpecies(BattleMon* battleMon);
extern "C" void BattleMon_SetItem(BattleMon* battleMon, u16 itemID);
extern "C" void BattleMon_ConsumeItem(BattleMon* battleMon, u16 itemID);
extern "C" b32 BattleMon_IsStatChangeValid(BattleMon* battleMon, StatStage stat, int volume);
extern "C" ConditionData BattleMon_GetMoveCondition(BattleMon* battleMon, CONDITION condition);
extern "C" void BattleMon_CureMoveCondition(BattleMon* battleMon, CONDITION condition);
extern "C" void CureStatusCondition(BattleMon* battleMon);

extern "C" u32 DivideMaxHPZeroCheck(BattleMon* battleMon, u32 denominator);

extern "C" u32 BattleParty_GetPartyCount(BattleParty* party);
extern "C" BattleMon * BattleParty_GetMonData(BattleParty* party, u32 idx);
extern "C" BattleMon* BattleParty_GetPartyMember(BattleParty* party, u32 idx);

extern "C" u32 BattleRandom(u32 range);
extern "C" b32 CheckHPDebug(BattleMon* battleMon);

#endif // __BATTLE_MON_H
