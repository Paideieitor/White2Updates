#ifndef __SERVER_FLOW_H
#define __SERVER_FLOW_H

#include "defs.h"

#include "main_module.h"
#include "action_order.h"
#include "handler_params.h"

#define ServerCommandQueue u8
#define ArcTool u8

struct SWAN_ALIGNED(4) m_record
{
    u32 turn;
    u16 moveID;
    u8 pokeID;
    u8 effective;
};

struct MoveRecord
{
    u32 ptr;
    m_record record[120];
};

struct FaintRecordUnit
{
    u8 count;
    u8 expChecked[24];
    u8 faintPokeID[24];
};

struct FaintRecord
{
    FaintRecordUnit turnRecord[4];
};

struct SWAN_ALIGNED(4) PokeSet
{
    BattleMon* battleMon[6];
    u16 damage[6];
    u16 substituteDamage[6];
    u8 damageType[6];
    u16 sortWork[6];
    u8 count;
    u8 countMax;
    u8 getIdx;
    u8 targetPosCount;
};

struct MoveAnimCtrl
{
    u16 moveID;
    u8 attackerPos;
    u8 targetPos;
    u8 effectIndex;
    u8 flags;
    u16 subEff;
};

struct MoveParam
{
    u16 moveID;
    u16 originalMoveID;
    u16 userType;
    u8 moveType;
    u8 damageType;
    u32 category;
    u32 targetType;
    u32 flags;
};

struct HitCheckParam
{
    u8 countMax;
    u8 count;
    u8 checkEveryTime;
    u8 multiHitMove;
    u8 putAnimCmd;
    u8 multiHitEffectiveness;
};

struct SWAN_PACKED SWAN_ALIGNED(1) MoveStealParam
{
    u8 stealCount;
    u8 stealPokeID[6];
    u8 targetPos[6];
    u8 targetPokeID[6];
};

struct SWAN_PACKED SWAN_ALIGNED(1) calc_damage_record
{
    u16 damage;
    u16 pokeData; // CalcDmgPokeData
};

struct SWAN_PACKED SWAN_ALIGNED(2) CALC_DAMAGE_REC
{
    u8 real_hit_count;
    u8 substitute_hit_count;
    u8 total_hit_count;
    u8 unkn;
    calc_damage_record record[6];
};

struct POKESET_STACK_UNIT
{
    PokeSet targetOriginal;
    PokeSet target;
    PokeSet ally;
    PokeSet enemy;
    PokeSet damaged;
    PokeSet stealTarget;
    PokeSet psetTemp;
    MoveAnimCtrl moveAnimCtrl;
    MoveParam moveParam;
    MoveParam moveParamOriginal;
    HitCheckParam hitCheck;
    MoveStealParam moveStealParam;
    MoveStealParam magicCoatParam;
    CALC_DAMAGE_REC calcDamageAlly;
    CALC_DAMAGE_REC calcDamageEnemy;
    u8 defaultTargetPos;
    u8 field_28D;
    u8 field_28E;
    u8 field_28F;
};

struct PosPokeState
{
    u8 enable;
    u8 clientID;
    u8 existPokeID;
};

struct PosPoke
{
    PosPokeState state[6];
    u8 lastPos[24];
};

struct SWAN_ALIGNED(2) CalcExpWork
{
    u32 level;
    u8 isBoosted;
    u8 hp;
    u8 atk;
    u8 def;
    u8 spatk;
    u8 spdef;
    u8 spe;
};

enum ActionIgnoreReason
{
    REASON_NONE = 0x0,
    REASON_OTHER_MOVE = 0x1,
    REASON_FALL_ASLEEP = 0x2,
    REASON_CONFUSION = 0x3,
    REASON_OVERLEVELED = 0x4,
    REASON_ASLEEP = 0x5,
};

struct EffectivenessCounter
{
    u16 noEffectHits;
    u16 superEffectiveHits;
    u16 notVeryEffectiveHits;
    u16 opponentNoEffectHits;
    u16 opponentSuperEffectiveHits;
    u16 opponentNotVeryEffectiveHits;
};

struct SWAN_ALIGNED(8) ServerFlow
{
    BtlServerWk* server;
    MainModule* mainModule;
    PokeCon* pokeCon;
    ServerCommandQueue* serverCommandQueue;
    u32 turnCount;
    u32 flowResult;
    u32 heapHandle;
    MoveRecord moveRecord;
    FaintRecord faintRecord;
    ArcTool* evolutionDataPtr;
    MoveAnimCtrl* moveAnimCtrl;
    MoveStealParam* moveStealParam;
    MoveStealParam* magicCoatParam;
    HitCheckParam* hitCheckParam;
    EscapeInfo escapeInfo;
    u32 field_4C0;
    u32 field_4C4;
    u32 field_4C8;
    u16 field_4CC;
    u16 field_4CE;
    u32 field_4D0;
    u8 field_4D4[672];
    u32 simulationCounter;
    u32 moveSerial;
    u8 cmdBuildStep;
    u8 field_77D;
    u8 turnCheckSeq;
    u8 defaultTargetPos;
    u16 heapID;
    u8 numActOrder;
    u8 numEndActOrder;
    u8 field_784;
    u8 revivedPokeCount;
    u8 field_786;
    u8 field_787;
    u8 field_788;
    u8 thruDeadMsgPokeID;
    u8 field_78A;
    u8 gap78B;
    u8 field_78C;
    u8 field_78D;
    u8 field_78E;
    u8 field_78F;
    u8 field_790;
    u8 revivePokeID[24];
    u8 pokeInFlag[24];
    u8 field_7C1[24];
    u8 switchCount[4];
    u8 field_7DD;
    u8 field_7DE;
    u8 field_7DF;
    ActionOrderWork actionOrderWork[6];
    ActionOrderWork tempActionOrderWork;
    PokeSet* setTargetOriginal;
    PokeSet* setTarget;
    PokeSet* setAlly;
    PokeSet* setEnemy;
    PokeSet* pokeSetDamaged;
    PokeSet* setStealTarget;
    PokeSet* pokeSetTemp;
    CALC_DAMAGE_REC* calcDamageAlly;
    CALC_DAMAGE_REC* calcDamageEnemy;
    POKESET_STACK_UNIT pokesetUnit[7];
    u32 pokesetStackPtr;
    PokeSet pokeSet_1A68;
    MoveParam* moveParam;
    MoveParam* moveParamOriginal;
    PosPoke posPoke;
    u8 field_1AE2;
    u8 field_1AE3;
    HandlerParam_StrParams strParam;
    CalcExpWork* levelUpInfo;
    u8 gap1B10[68];
    u32 field_1B54;
    u8 gap1B58[316];
    u8 field_1C88;
    u8 field_1C89;
    u8 field_1C8A;
    u8 field_1C97;
    u8 gap1C98[224];
    u32 HEManager;
    u8 gap1D7C[8];
    u8 field_1D84;
    u8 field_1D85;
    u8 field_1D86;
    u8 field_1D87;
    u8 field_1D88;
    u8 field_1D89;
    u8 field_1D8A;
    u8 gap1D8B[481];
    u8 field_1F6C;
    u8 gap1F6D[11];
    u16 prevUsedMove;
    u8 field_1F7A;
    u8 field_1F7B;
    ActionIgnoreReason actionIgnoreReason;
    EffectivenessCounter effectivenessCounter;
    u32 dmgAffRec;
    u8 gap1F90[8];
    u8 field_1F98;
    u8 gap1F99[83];
    u8 field_1FEC[4];
    u8 TempWork;
    u8 field_1FF1;
    u8 gap1FF2[318];
    u32 field_2130;
    u8 gap2134[8];
    u8 field_213C;
    u8 field_213D;
    u8 field_213E;
    u8 field_213F;
};

extern "C" u32 ServerFlow_ReqChangePokeForServer(ServerFlow * serverFlow, u16 * a2);
extern "C" b32 ServerFlow_IsCompetitorScenarioMode(ServerFlow* serverFlow);
extern "C" u32 ActionOrder_Proc(ServerFlow* serverFlow, ActionOrderWork* actionOrder);

extern "C" b32 ServerControl_TurnCheck(ServerFlow* serverFlow);
extern "C" b32 ServerControl_CheckMatchup(ServerFlow* serverFlow);
extern "C" void ServerControl_CheckActivation(ServerFlow* serverFlow);
extern "C" u32 ServerControl_CheckExpGet(ServerFlow* serverFlow);

extern "C" u32 ServerDisplay_IllusionSet(ServerFlow* serverFlow, u16* a2);

extern "C" BattleMon* PokeCon_GetBattleMon(PokeCon* pokeCon, u32 index);

#endif // __SERVER_FLOW_H
