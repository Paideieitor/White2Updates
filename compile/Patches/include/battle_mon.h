#ifndef __BATTLE_MON_H
#define __BATTLE_MON_H

#include "defs.h"

#include "items.h"

#include "pml/poke_party.h"

extern "C" u32 PokeParty_GetParam(PartyPkm * pPkm, PkmField field, void* extra);

struct ConditionData
{
    u32 status : 3;
    u32 turncount : 3;
    u32 field7 : 3;
    u32 param : 3;
    u32 rest : 20;
};

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

enum MoveCondition
{
    CONDITION_NONE = 0x0,
    CONDITION_PARALYSIS = 0x1,
    CONDITION_SLEEP = 0x2,
    CONDITION_FREEZE = 0x3,
    CONDITION_BURN = 0x4,
    CONDITION_POISON = 0x5,
    CONDITION_CONFUSION = 0x6,
    CONDITION_ATTRACT = 0x7,
    CONDITION_BIND = 0x8,
    CONDITION_NIGHTMARE = 0x9,
    CONDITION_CURSE = 0xA,
    CONDITION_TAUNT = 0xB,
    CONDITION_TORMENT = 0xC,
    CONDITION_DISABLE = 0xD,
    CONDITION_YAWN = 0xE,
    CONDITION_HEALBLOCK = 0xF,
    CONDITION_GASTROACID = 0x10,
    CONDITION_FORESIGHT = 0x11,
    CONDITION_LEECHSEED = 0x12,
    CONDITION_EMBARGO = 0x13,
    CONDITION_PERISHSONG = 0x14,
    CONDITION_INGRAIN = 0x15,
    CONDITION_BLOCK = 0x16,
    CONDITION_ENCORE = 0x17,
    CONDITION_ROOST = 0x18,
    CONDITION_MOVELOCK = 0x19,
    CONDITION_CHARGELOCK = 0x1A,
    CONDITION_CHOICELOCK = 0x1B,
    CONDITION_MUSTHIT = 0x1C,
    CONDITION_LOCKON = 0x1D,
    CONDITION_FLOATING = 0x1E,
    CONDITION_KNOCKEDDOWN = 0x1F,
    CONDITION_TELEKINESIS = 0x20,
    CONDITION_SKYDROP = 0x21,
    CONDITION_ACCURACYUP = 0x22,
    CONDITION_AQUARING = 0x23,
    CONDITION_24 = 0x24,
};

extern "C" u32 BattleMon_GetID(BattleMon* battleMon);
extern "C" b32 BattleMon_IsFainted(BattleMon* battleMon);
extern "C" ItemID BattleMon_GetHeldItem(BattleMon* battleMon);
extern "C" u32 BattleMon_GetValue(BattleMon* battleMon, BattleMonValue value);
extern "C" MoveCondition BattleMon_GetStatus(BattleMon* battleMon);
extern "C" u32 BattleMon_GetConfrontRecCount(BattleMon* battleMon);
extern "C" u32 BattleMon_GetConfrontedPokeID(BattleMon* battleMon, u32 confronted);
extern "C" PartyPkm* BattleMon_GetSrcData(BattleMon* battleMon);

extern "C" u32 BattleParty_GetPartyCount(BattleParty * party);
extern "C" BattleMon * BattleParty_GetMonData(BattleParty * party, u32 idx);
extern "C" BattleMon* BattleParty_GetPartyMember(BattleParty* party, u32 idx);

extern "C" u32 BattleRandom(u32 range);
extern "C" b32 CheckHPDebug(BattleMon* battleMon);

#endif // __BATTLE_MON_H
