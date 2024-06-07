#ifndef __INCLUDE_ALL_H
#define __INCLUDE_ALL_H

#define _BYTE unsigned char
#define _WORD unsigned short
#define _DWORD unsigned int
#define _QWORD unsigned long

#ifdef __GNUC__
#define SWAN_PACKED __attribute__((__packed__))
#define SWAN_ALIGNED(x) __attribute__((aligned(x)))
#endif

#ifdef _MSC_VER //unsupported on MSVC
#define SWAN_PACKED
#define SWAN_ALIGNED(x)
#endif

#define DEBUG_PRINT true
#if DEBUG_PRINT
#include "kernel/kPrint.h"
#define DPRINT(format) k::Print(format)
#define DPRINTF(format, ...) k::Printf(format, __VA_ARGS__)
#elif !DEBUG_PRINT
#define DPRINT(format)
#define DPRINTF(format, ...)
#endif

#define ARRAY_COUNT(arr) sizeof(arr) / sizeof(arr[0])

#include "swantypes.h"

#include "battle/btl_result.h"
#include "field/fieldmap.h"
#include "field/field_mmodel.h"
#include "gfl/core/gfl_heap.h"
#include "math/angle.h"
#include "math/vector.h"
#include "pml/poke_party.h"
#include "save/save_control.h"
#include "data/heapid_def.h"


C_DECL_BEGIN
enum BattleStyle
{
    BTL_STYLE_SINGLE = 0x0,
    BTL_STYLE_DOUBLE = 0x1,
    BTL_STYLE_TRIPLE = 0x2,
    BTL_STYLE_ROTATION = 0x3,
};

struct SWAN_ALIGNED(2) BattleFieldStatus
{
    u32 BattleBGID;
    _DWORD BattlePedestalID;
    _BYTE BtlWeather;
    u8 Season;
    u16 ZoneID;
    u8 Hour;
    u8 Minute;
    char field_E;
    char field_F;
};

struct SWAN_ALIGNED(2) StrBuf
{
    u16 CharCapacity;
    u16 CharCount;
    u32 Magic;
    __int16 String[];
};

struct TrainerBattleSetup
{
    _DWORD TrID;
    _DWORD TrClass;
    _DWORD TrAIBits;
    u16 TrItems[4];
    StrBuf* TrainerName;
    __int16 field_18[4];
    __int16 field_20[4];
};

struct TrainerData
{
    u8 Flags;
    u8 Class;
    u8 BattleType;
    u8 PokemonCount;
    u16 Items[4];
    u32 AI;
    u8 Healer;
    u8 Money;
    u16 ItemAfterBattle;
};

struct SWAN_ALIGNED(4) BtlSetup
{
    u32 BtlType;
    BattleStyle BtlStyle;
    BattleFieldStatus m_FieldStatus;
    u16 BattleBGMID;
    __int16 WinBGMID;
    int field_1C;
    char commMode;
    char field_21;
    char field_22;
    char field_23;
    PokeParty* Parties[4];
    int m_PlayerInfo;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    TrainerBattleSetup* TrainerSetups[4];
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    TrainerData* m_TrainerData;
    int m_ItemWork;
    int field_7C;
    int m_Dex;
    int m_RecordData;
    int field_88;
    int field_8C;
    int field_90;
    char field_94;
    char field_95;
    char field_96;
    char field_97;
    char BadgeCount;
    char field_99;
    char field_9A;
    char field_9B;
    int field_9C;
    __int16 field_A0;
    __int16 field_A2;
    int EarnedMoney;
    BattleResult battleResult;
    char CapturedPkmIdx;
    char field_AD;
    __int16 field_AE;
    void* field_B0;
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
    int field_C4;
    int field_C8;
    int field_CC;
    u16 HPSum;
    u8 CurrentTurn;
    u8 SwitchCount;
    u8 NoEffectHits;
    u8 SuperEffectiveHits;
    u8 NotVeryEffectiveHits;
    u8 OpponentNoEffectHits;
    u8 OpponentNotVeryEffectiveHits;
    u8 BattlesWon;
    u8 OpponentFainted;
    u8 AllyFainted;
    u8 UniqueMovesUsed;
    char field_DD;
    char field_DE;
    char field_DF;
    int field_E0;
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
    int field_10C;
    char field_110;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    int field_124;
    char field_128;
    int field_12C;
    int field_130;
    int field_134;
    __int16 field_138;
    __int16 field_13A;
    u8 KeySystemLevelAdjust;
    char field_13D;
    char field_13E;
    char field_13F;
    int field_140;
    int field_144;
    int field_148;
};

struct ConditionData
{
    u32 status : 3;
    u32 turncount : 3;
    u32 field7 : 3;
    u32 param : 3;
    u32 rest : 20;
};


struct StatStageParam
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
    u16 MoveID;
    u8 CurrentPP;
    u8 MaxPP;
    u8 PPUpCount;
    char UsedFlag;
};

struct MoveSet
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

enum BattlePartySlot : u8
{
    PLAYER_SLOT_0 = 0,
    PLAYER_SLOT_1 = 1,
    PLAYER_SLOT_2 = 2,
    PLAYER_SLOT_3 = 3,
    PLAYER_SLOT_4 = 4,
    PLAYER_SLOT_5 = 5,
    ALLY_TAG_SLOT_0 = 6,
    ALLY_TAG_SLOT_1 = 7,
    ALLY_TAG_SLOT_2 = 8,
    ALLY_TAG_SLOT_3 = 9,
    ALLY_TAG_SLOT_4 = 10,
    ALLY_TAG_SLOT_5 = 11,
    ENEMY_SLOT_0 = 12,
    ENEMY_SLOT_1 = 13,
    ENEMY_SLOT_2 = 14,
    ENEMY_SLOT_3 = 15,
    ENEMY_SLOT_4 = 16,
    ENEMY_SLOT_5 = 17,
    ENEMY_TAG_SLOT_0 = 18,
    ENEMY_TAG_SLOT_1 = 19,
    ENEMY_TAG_SLOT_2 = 20,
    ENEMY_TAG_SLOT_3 = 21,
    ENEMY_TAG_SLOT_4 = 22,
    ENEMY_TAG_SLOT_5 = 23,
    BATTLE_MAX_SLOTS = 24,
};

struct SWAN_ALIGNED(4) BattleMon
{
    PartyPkm* partySrc;
    PartyPkm* disguiseSrc;
    int Experience;
    __int16 Species;
    __int16 MaxHP;
    __int16 CurrentHP;
    __int16 HeldItem;
    __int16 UsedItem;
    __int16 Ability;
    char Level;
    BattlePartySlot battleSlot;
    char BaseAttack;
    char flags;
    ConditionData Conditions[36];
    u8 MoveConditionCounter[36];
    char ConfrontRecCount;
    u8 ConfrontRec[24];
    _BYTE gapE9[5];
    __int16 Attack;
    __int16 Defense;
    __int16 SpecialAttack;
    __int16 SpecialDefense;
    __int16 Speed;
    char Type1;
    char Type2;
    char Sex;
    char field_FB;
    StatStageParam statStageParam;
    char field_103;
    MoveSet Moves[4];
    __int16 CurrentAbility;
    __int16 Weight;
    char MoveCount; // 0 to 4, amount of move slots that are not empty
    char Form;
    char CritStage;
    char UsedMoveCount; // 0 to 4, amount of moves used from the 4 it has
    char PrevMoveType;
    char field_145;
    __int16 TurnCount;
    __int16 AppearedTurn;
    __int16 PreviousMove;
    __int16 PreviousMoveID;
    __int16 ConsecutiveMoveCounter;
    __int16 field_150;
    char prevTargetPos;
    u8 TurnFlag[2];
    u8 ConditionFlag[2];
    u8 Counters[5];
    MoveDamageRec DamageRec[3][6];
    unsigned __int8 DamageRecCount[3];
    char DamageRecTurn;
    char DamageRecPtr;
    char field_1F1;
    __int16 SubstituteHP;
    __int16 comboMoveID;
    char comboPokeID;
    char field_1F7;
};

struct BattleParty
{
    BattleMon* pMember[6];
    u8 memberCount;
    u8 numCoverPos;
};

struct MainModule;
struct PokeCon
{
    MainModule* mainModule;
    BattleParty party[4];
    PokeParty* srcParty[4];
    BattleMon* ActiveBattleMon[24];
    int fForServer;
};

struct BattleAction_Fight
{
    unsigned __int32 cmd : 4;
    unsigned __int32 targetPos : 4;
    unsigned __int32 moveID : 16;
    unsigned __int32 pad : 8;
};

struct BattleAction_Item
{
    unsigned __int32 cmd : 4;
    unsigned __int32 targetIdx : 3;
    unsigned __int32 itemID : 16;
    unsigned __int32 param : 8;
    unsigned __int32 pad : 1;
};

struct BattleAction_Switch
{
    unsigned __int32 cmd : 4;
    unsigned __int32 posIdx : 3;
    unsigned __int32 memberIdx : 3;
    unsigned __int32 depleteFlag : 1;
    unsigned __int32 pad : 21;
};

struct BattleAction_Run
{
    unsigned __int32 cmd : 4;
    unsigned __int32 pad : 28;
};

struct BattleAction_Default
{
    unsigned __int32 cmd : 4;
    unsigned __int32 param : 28;
};

union BattleActionParam
{
    BattleAction_Fight baFight;
    BattleAction_Item baItem;
    BattleAction_Switch baSwitch;
    BattleAction_Run baRun;
    BattleAction_Default baDefault;
};

struct SWAN_ALIGNED(4) BtlRecWk
{
    u8 field_0;
    char field_1;
    _BYTE gap2[3078];
    _DWORD dwordC08;
};

struct EscapeInfo
{
    u32 count;
    u8 clientID[4];
};

struct BtlEventHandlerTable;
struct SWAN_ALIGNED(4) BattleEventItem
{
    BattleEventItem* prev;
    BattleEventItem* next;
    const BtlEventHandlerTable* handlerTable; // ERROR struct in IDA
    u32* skipCheckHandler; //  bool(*BtlEventSkipCheckHandler)(void* myHandle, void* wk, u32 factorType, u32 eventType, u16 subID, u8 pokeID);
    u32 factorType;
    int currentStackPtr;
    int flags;
    int work[7];
    __int16 subID;
    char dependID;
    char pokeID;
};


struct BattleField
{
    int Weather;
    int WeatherTurns;
    void* BattleEventItems[8];
    ConditionData conditionData[8];
    u32 TurnCount[8];
    u32 DependPokeID[8][6];
    u32 DependPokeCount[8];
    u32 EffectEnableFlags[8];
};

struct SEND_DATA_BUFFER
{
    u32 serialNumber;
    u32 cmd;
    u32 serverSeq;
    u8 flags;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u16 size;
    u8 data[3000];
};

struct SendData
{
    SEND_DATA_BUFFER buffer;
};

struct Adapter
{
    SendData sendData;
    SendData returnData;
    int processingCmd;
    u8 myID;
    u8 myState;
    u8 flags;
};

enum GFLFontLoadType
{
    FONT_LOAD_STREAM = 0x0,
    FONT_LOAD_MEMORY = 0x1,
};

struct GlyphInfo
{
    u8 GlyphOffset;
    u8 GlyphWidth;
    u8 CellWidth;
    u8 CellHeight;
};

struct NNSG2DResFontInfo
{
    char field_8;
    u8 Height;
    u16 NullCharIndex;
    char field_C;
    u8 Width;
    u8 DefaultCharWidth;
    u8 Encoding;
    int CGLPOffset;
    int CWDHOffset;
    int CMAPOffset;
};

struct NNSG2DResFontWidth
{
    char GlyphOffset;
    char GlyphWidth;
    char CellWidth;
};

struct NNSG2DResFontGlyph
{
    NNSG2DResFontWidth Width;
    char Bitmap[];
};

struct NNSG2DResFontGlyphs
{
    u8 CellWidthPx;
    u8 CellHeightPx;
    u16 GlyphSizeBytes;
    u16 field_4;
    u8 BitDepth;
    u8 RotateMode;
    NNSG2DResFontGlyph* glyphs;
};

struct NNSG2DResFontWidthTable
{
    u8 MostUsedWidths[3];
    char field_3;
    int HeaderSize;
    int SimpleWidthsOffset;
    int KVMapsOffset;
    NNSG2DResFontWidth Content[];
};

struct SWAN_ALIGNED(4) GFLFont
{
    GFLFontLoadType LoadType;
    u32* glyphGetFunc; // void(*GlyphGetFunc)(GFLFont*, u32, void*, GlyphInfo*);
    u32* glyphBitmapCopyFunc; // void(*GlyphBitmapCopyFunc)(char*, u8, void*);
    int CachedGlyphData;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
    ArcTool* m_ArcTool;
    _DWORD DatID;
    NNSG2DResFontInfo FINF;
    u32* WidthGetterFunc; //  int(*WidthGetterFunc)(GFLFont * font, u32 character);
    u16 UndefinedCharIndex;
    char FixedWidth;
    char field_73;
    NNSG2DResFontGlyphs CGLP;
    u16 RightTileBitCount;
    u8 ByteWidthCeil;
    u8 ByteHeightCeil;
    u32 NARCDataOffset;
    NNSG2DResFontWidthTable* CWDH;
    void* CMAP;
    int GlyphRawDataOffset;
    NNSG2DResFontGlyph* GlyphBuffer;
};

struct TCBManagerEx;
struct SWAN_ALIGNED(4) TCBEx
{
    TCBManagerEx* Manager;
    TCBEx* PrevTCB;
    TCBEx* NextTCB;
    _DWORD Priority;
    _DWORD Data;
    void* CallbackFunc;
};

struct SWAN_ALIGNED(4) TCBManagerEx
{
    unsigned int TCBLimit;
    _DWORD ExtraSize;
    HeapID WorkHeapID;
    _DWORD dwordC;
    _DWORD IsTCBRequestEnd;
    TCBEx* TCBs;
    TCBEx Head;
    TCBEx* NowExecTCB;
    TCBEx* ReadyTaskHead;
};

struct SWAN_ALIGNED(4) BmpWin
{
    _WORD word0;
    u8 BGIndex;
    u8 PosX;
    u8 PosY;
    u8 BmpWidth1;
    u8 BmpHeight1;
    u8 BmpWidth2;
    char BmpHeight2;
    u8 PaletteIndex;
    u16 CharOffset;
    GFLBitmap* m_Bitmap;
};

struct SWAN_PACKED SWAN_ALIGNED(1) BtlvScuDataSingles
{
    BattleMon* battleMon;
    u8 BattlePos;
    u8 ViewPos;
    u8 MonID;
    u8 ClientID;
    char AllyClientID;
};

struct BtlvScu_Field154
{
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    char field_14;
    char field_15;
    char field_16;
    char field_17;
};

struct BtlvCore;
struct BtlvScu
{
    BmpWin* BmpWin1;
    GFLBitmap* Bitmap1;
    BmpWin* BmpWin2;
    GFLBitmap* Bitmap2;
    int field_10;
    int field_14;
    _DWORD printsys;
    _DWORD dword1C;
    _BYTE byte20;
    _BYTE gap21[3];
    GFLFont* font1;
    GFLFont* font2;
    TCBManagerEx* callbackMgr;
    _DWORD dword30;
    StrBuf* strbuf1;
    StrBuf* strbuf2;
    StrBuf* strbuf3;
    int field_40;
    int field_44;
    int field_48[18];
    int field_90;
    GFLBitmap* field_94[9];
    _BYTE gapB8[32];
    _DWORD dwordD8;
    int field_DC;
    u32* field_E0; // int(*field_E0)(int*, BtlvScu*);
    u32* field_E4; // int(*field_E4)(int*, BtlvScu*);
    BtlvScu* field_E8;
    int field_EC;
    BtlvScuDataSingles field_F0;
    _BYTE gapF9[6];
    char field_FF;
    char field_100;
    char field_101;
    _BYTE gap102[3];
    char field_105;
    char ClientID;
    _BYTE gap107[41];
    BtlvCore* viewCore;
    MainModule* mainModule;
    PokeCon* pokeCon;
    HeapID heapID;
    char field_13E;
    char field_13F;
    u8 myID;
    char field_141;
    char field_142;
    char field_143;
    int field_144;
    __int16 field_148;
    __int16 field_14A;
    __int16 field_14C;
    _BYTE byte14E;
    _BYTE field_14F;
    char field_150;
    BtlvScu_Field154 field_154;
    int field_16C;
    int field_170;
    int field_174;
    int field_178;
    int field_17C;
};

struct TargetSelectData
{
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
};

enum SubwayBattleStyle
{
    SUBWAY_BTL_STYLE_SINGLE = 0x0,
    SUBWAY_BTL_STYLE_DOUBLE = 0x1,
    SUBWAY_BTL_STYLE_MULTI_AI = 0x2,
    SUBWAY_BTL_STYLE_MULTI_NET = 0x3,
    SUBWAY_BTL_STYLE_WIFI = 0x4,
    SUBWAY_BTL_STYLE_SUPER_SINGLE = 0x5,
    SUBWAY_BTL_STYLE_SUPER_DOUBLE = 0x6,
    SUBWAY_BTL_STYLE_SUPER_MULTI_AI = 0x7,
    SUBWAY_BTL_STYLE_SUPER_MULTI_NET = 0x8,
};

struct TrialHouseWk
{
    u32 word0;
    u16 OpposingTrainerClass;
    __int16 field_6;
    __int16 OpposingTrainerName[9];
    u16 MsgID;
    __int16 field_1C;
    SWAN_PACKED SWAN_ALIGNED(1) int field_1E;
    SWAN_PACKED SWAN_ALIGNED(1) int field_22;
    SWAN_PACKED SWAN_ALIGNED(1) int field_26;
    SWAN_PACKED SWAN_ALIGNED(1) int field_2A;
    __int16 field_2E;
    char OpponentPokeParams[240];
    HeapID m_HeapID;
    __int16 field_122;
    u32 PartyMax;
    PokeParty* Party;
    SubwayBattleStyle BattleStyle;
    int field_130;
    u16 TurnsPassed;
    u16 SwitchCount;
    u16 NoEffectHits;
    u16 SuperEffectiveHits;
    u16 NotVeryEffectiveHits;
    u16 OpponentNoEffectHits;
    u16 OpponentNotVeryEffectiveHits;
    u16 BattlesWon;
    u16 OpponentFainted;
    u16 AllyFainted;
    u16 HPPercentMissing;
    u16 UniqueMovesUsed;
    _DWORD dword14C;
    u32 field_150;
};

enum BtlType
{
    BtlType_Wild = 0x0,
    BtlType_Trainer = 0x1,
    BtlType_Facility = 0x2,
    BtlType_Online = 0x3,
    BtlType_Demo = 0x4,
    BtlType_Max = 0x5,
};

struct MsgFileHeader
{
    u16 SectionCount;
    u16 LineCount;
    int DataBottomOfs;
    int Reserved;
};

struct MsgFileHandle
{
    MsgFileHeader Header;
    int DataTopOffsets[1];
};

struct MsgFileEntry
{
    int Offset;
    u16 CharCount;
    u16 Padding;
};

struct SWAN_ALIGNED(4) MsgData
{
    MsgFileHandle* FileHandle;
    int RawData;
    MsgFileEntry CurrentEntry;
    int MsgOfsInArc;
    ArcTool* MsgArc;
    __int16 HeapID;
    u8 LanguageId;
    u8 IsAllPreload;
};

struct ClAct_0x18;
struct ClAct_228
{
    ClAct_228* next;
    ClAct_228* prev;
    ClAct_0x18* clAct_0x18;
    __int16 field_C;
    __int16 field_E;
    __int16 field_10;
    __int16 field_12;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    _BYTE gap2C[20];
    int field_40;
    _BYTE gap44[16];
    int field_54;
    __int16 field_58;
    __int16 field_5A;
    __int16 field_5C;
    __int16 field_5E;
    int Flags;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
    int field_C4;
    int field_C8;
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
};

struct ClAct_0x18
{
    ClAct_228* ClAct228Array;
    ClAct_228* field_4;
    int field_8;
    __int16 ArraySize;
    char field_E;
    char field_F;
    ClAct_0x18* next;
    ClAct_0x18* prev;
};

struct SWAN_PACKED SWAN_ALIGNED(2) BtlvFingerCursorHandle
{
    ClAct_0x18* dword0;
    ClAct_228* field_4;
    TCB* dword8;
    _DWORD dwordC;
    _DWORD dword10;
    _DWORD dword14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    _WORD HeapID;
};

struct BtlvInput
{
    TCBManager* pTCBManager[2];
    TCB* TCBArray[8];
    u8 field_28;
    _BYTE gap29[31];
    int UIGraphicsNarcPtr;
    GameData* gameData;
    BattleStyle gattleStyle;
    BtlType gattleType;
    int field_58;
    MsgData* msgData;
    BtlvFingerCursorHandle* FingerCursor;
    int field_64;
    int Flags;
    int Flags2;
    int field_70;
    _BYTE gap74[144];
    int field_104;
    int field_108;
    int field_10C;
    int field_110;
    int field_114;
    ClAct_0x18* field_118;
    ClAct_228* field_11C[6];
    _BYTE gap134[24];
    ClAct_228* field_14C[6];
    _BYTE gap164[24];
    ClAct_0x18* field_17C;
    _BYTE gap180[60];
    int field_1BC;
    int field_1C0;
    ClAct_0x18* field_1C4;
    int field_1C8[6];
    _BYTE gap1E0[16];
    int field_1F0;
    int field_1F4;
    ClAct_0x18* field_1F8;
    int field_1FC;
    _BYTE gap200[12];
    ClAct_0x18* field_20C;
    int field_210;
    int field_214;
    ClAct_0x18* field_218;
    ClAct_228* clAct_228;
    int field_220;
    _BYTE gap224[2];
    char field_226;
    _BYTE gap227[5];
    ClAct_0x18* field_22C;
    ClAct_228* field_230[22];
    int field_288;
    int field_28C;
    int field_290;
    int field_294;
    int field_298;
    int field_29C;
    int field_2A0;
    int field_2A4;
    int field_2A8;
    BmpWin* bmpWin1;
    GFLBitmap* bitmap1;
    BmpWin* bmpWin2;
    GFLBitmap* bitmap2;
    TCB* field_2BC;
    int field_2C0;
    __int16 heapID;
    char field_2C6;
    u8 field_2C7[16];
    SWAN_PACKED SWAN_ALIGNED(1) int field_2D7;
    char field_2DB;
    char field_2DC;
    char field_2DD;
    u8 field_2DE[8];
    u8 field_2E6[4];
    u8 field_2EA[2];
    int field_2EC;
    int field_2F0;
    int field_2F4;
    int field_2F8;
    int field_2FC;
    int field_300;
    int field_304;
    char field_308;
    _BYTE gap309[31];
    char field_328;
    char IsPokestarBattle;
    char field_32A;
    char field_32B;
    int field_32C;
    BattleMon* field_330[3];
    _BYTE gap33C[47];
    char field_36B;
};

struct BtlClientWk;
struct BtlvScd
{
    int field_0;
    int field_4;
    int font;
    StrBuf* strBuf;
    BtlClientWk* client;
    int printsys;
    int field_18;
    int field_1C;
    int field_20;
    _BYTE gap24[60];
    int field_60;
    int field_64;
    int field_68;
    BattleMon* ActiveMon;
    BattleActionParam* ActionParam;
    int field_74;
    int field_78;
    int field_7C;
    TargetSelectData targetSelectData;
    int field_84;
    char field_88;
    char field_89;
    char MoveTargetData;
    char field_8B;
    char field_8C;
    char field_8D;
    char field_8E;
    char field_8F;
    u8 field_90;
    char field_91;
    char field_92;
    char field_93;
    char field_94;
    char field_95;
    char field_96;
    char field_97;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int viewCore;
    MainModule* mainModule;
    PokeCon* pokeCon;
    BattleParty* battleParty;
    __int16 heapID;
    __int16 field_BA;
    BtlvInput* btlvInput;
};

struct SWAN_PACKED SWAN_ALIGNED(2) BtlvCore
{
    MainModule* mainModule;
    BtlClientWk* client;
    PokeCon* pokeCon;
    unsigned __int8 myID;
    _BYTE gapD[3];
    _DWORD dword10;
    _DWORD dword14;
    _DWORD dword18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    char field_2C;
    _BYTE gap2D[127];
    StrBuf* strBuf;
    GFLFont* Font1;
    GFLFont* Font2;
    BattleActionParam* ActionParam;
    BattleMon* ActiveMon;
    int ActiveMonID;
    int field_C4;
    int field_C8;
    _BYTE gapCC[212];
    int field_1A0;
    int field_1A4;
    int field_1A8;
    int field_1AC;
    int field_1B0;
    _BYTE byte1B4;
    char field_1B5;
    char field_1B6;
    char field_1B7;
    char field_1B8;
    char field_1B9;
    char field_1BA;
    char field_1BB;
    TCBManagerEx* CallbackMgr;
    BtlvScu* btlvScu;
    BtlvScd* btlvScd;
    HeapID heapID;
};

struct Btlv_StringParam
{
    u16 strID;
    u8 wait;
    u8 strTypeAndArgCount;
    int args[8];
};

struct ScriptPlugin
{
    void* CommandSet;
    u32 CommandSetSize;
    u32 CommandNoOffset;
};

struct ScriptVMSetup
{
    u16 StackCapacity;
    u16 VMVarCount;
    void* CommandSet;
    u32 CommandSetSize;
    ScriptPlugin Plugin;
};

struct ScriptVM
{
    ScriptVMSetup Setup;
    u8 StackIndex;
    u8 State;
    u8 CPSR;
    u8 _padCPSR;
    void* NativeFunc;
    u8* PC;
    u32* Stack;
    u32* VMVars;
    void* Env;
    void* CallbackVerifier;
    GameSystem* m_GameSystem;
    u8* ExecFile;
};

struct PokeSelectParam
{
    BattleParty* party;
    u8 numSelect;
    u8 prohibit[6];
    u8 bDisabledPutPosSequence;
};

struct PokeSelectResult
{
    u8 selIdx[3];
    u8 outPokeIdx[3];
    u8 count;
    char field_7;
    char fCancel;
};

struct RecPlayerControl
{
    char field_0;
    char field_1;
    char field_2;
    char field_3;
    __int16 field_4;
    u16 field_6;
    u16 field_8;
    u16 field_A;
    u16 field_C;
};

struct SWAN_PACKED SWAN_ALIGNED(4) BtlClientWk
{
    MainModule* mainModule;
    PokeCon* pokeCon;
    BattleMon* procPoke;
    BattleActionParam* procAction;
    BtlRecWk* recData;
    int battleRecReader;
    RecPlayerControl recPlayer;
    SWAN_ALIGNED(4) _DWORD dword28;
    EscapeInfo escapeInfo;
    BattleField* battleField;
    _BYTE gap38[24];
    Adapter* adapter;
    BtlvCore* btlvCore;
    Btlv_StringParam strParam;
    int field_7C;
    Btlv_StringParam field_80;
    _BYTE gapA4[4];
    _DWORD cmdCheckServer;
    BattleMon* Party[6];
    int field_C4;
    int field_C8;
    char field_CC;
    int(*field_D0)(BtlClientWk*, int*) SWAN_ALIGNED(4);
    int field_D4;
    int(*actionSelectProc)(BtlClientWk * a1, unsigned int* a2);
    unsigned int ActionSelectSeq;
    BattleActionParam* returnDataActionSelect;
    int returnDataActionSelectCount;
    int field_E8;
    _WORD cmdLimitTime;
    _WORD wordEE;
    unsigned __int16 AIBagItems[4];
    ScriptVM* VM;
    _QWORD rand1;
    _QWORD rand2;
    _QWORD rand3;
    u8 field_114[6];
    u8 field_11A[4];
    SWAN_ALIGNED(4) BattleParty* actPokeParty;
    unsigned __int8 myCoverPosNum;
    unsigned __int8 procPokeIdx;
    char prevPokeIdx;
    u8 firstPokeIdx;
    char fStdMsgChanged;
    char field_129;
    char field_12A;
    SWAN_ALIGNED(2) BattleActionParam actionParam[3];
    u8 field_138[4];
    int cmdQueue;
    int ServerCmdArgs;
    _BYTE gap144[60];
    int serverCmd;
    int ServerCmd;
    int ServerCmdSeq;
    PokeSelectParam pokeSelectParam;
    PokeSelectResult pokeSelectResult;
    char field_1A1;
    HeapID TrainerMsgHeapID;
    char field_1A4;
    SWAN_ALIGNED(2) u16 field_1A6;
    _WORD SelItemWork[3];
    u8 myID;
    char field_1AF;
    _BYTE gap1B0;
    char commWaitInfoOn;
    char field_1B2;
    char field_1B3;
    _BYTE field_1B4;
    u8 forceQuitActionSelect;
    _BYTE cmdCheckTimingCode;
    char currentActionCount;
    char moveInfoPokeIdx;
    char moveInfoMoveIdx;
    char actSelectFlags;
    char myChangePokeCount;
    char field_1BC;
    u8 field_1BD;
    unsigned __int8 myChangePokePos[6];
    char field_1C4;
    char field_1C5;
    char field_1C6;
    char field_1C7;
    char field_1C8;
    char field_1C9;
    char field_1CA;
    SWAN_ALIGNED(2) int field_1CC;
    int field_1D0;
    _BYTE gap1D4[20];
    __int16 field_1E8;
    __int16 field_1EA;
    int field_1EC;
    int field_1F0;
    _BYTE gap1F4[16];
    int field_204;
    _BYTE gap208[12];
    int field_214;
    _BYTE gap218[4];
    _WORD word21C;
    SWAN_ALIGNED(4) int dword220;
    u16 field_224[22];
    char field_250;
};

struct BtlServerWk;
struct SWAN_ALIGNED(32) MainModule
{
    BtlSetup* btlSetup;
    int field_4;
    BtlServerWk* server;
    SWAN_ALIGNED(8) BtlClientWk* clients[4];
    int field_20;
    int field_24;
    TrainerData trainerData[4];
    _BYTE gap78[55];
    char field_AF;
    int field_B0;
    int field_B4;
    int field_B8;
    int field_BC;
    int playerInfo;
    u8 field_C4[4];
    PokeCon pokeconClient;
    PokeCon pokeconServer;
    PokeParty* srcParty[4];
    PokeParty* srcPartyForServer[4];
    PokeParty* tempParty;
    int field_2BC;
    PartyPkm* TempPartyPkm;
    int field_2C4;
    int field_2C8;
    int field_2CC;
    int field_2D0;
    int field_2D4;
    _BYTE gap2D8[260];
    int field_3DC;
    int chatotChatterBlk;
    int field_3E4;
    int field_3E8;
    int field_3EC;
    int randomSeed;
    int field_3F4;
    int field_3F8;
    int field_3FC;
    int field_400;
    int field_404;
    int field_408;
    int field_40C;
    int field_410;
    int field_414;
    int field_418;
    int field_41C;
    __int16 field_420;
    SWAN_PACKED SWAN_ALIGNED(1) int field_422;
    char field_426;
    char field_427;
    unsigned __int8 field_428[6];
    char field_42E;
    char field_42F;
    int winMoney;
    int bonusMoney;
    int loseMoney;
    int field_43C;
    __int16 field_440;
    __int16 field_442;
    int serverResult;
    EscapeInfo escapeInfo;
    int subproc;
    int field_454;
    int field_458;
    int field_45C;
    int subseq;
    int field_464;
    HeapID heapID;
    char field_46A;
    char field_46B;
    char playerClientID;
    char field_46D;
    char field_46E;
    char field_46F;
    char field_470;
    char field_471;
    char field_472;
    char field_473;
    int pokestarScriptNarcPtr;
    int pokestarScriptHeapPtr;
    int field_47C;
};

struct SVCL_WORK
{
    int adapter;
    BattleParty* party;
    u8 byte8;
    u8 byte9;
    u8 byteA;
    u8 id;
};

struct ServerFlow;
struct SWAN_ALIGNED(4) BtlServerWk
{
    int field_0;
    int field_4;
    char field_8;
    char field_9;
    char field_A;
    char field_B;
    MainModule* mainModule;
    int pokeCon;
    SVCL_WORK client[4];
    ServerFlow* flowWork;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    char field_68;
    char gap69;
    _BYTE gap6A[62];
    __int16 field_A8;
    __int16 field_AA;
    int field_AC;
    int escapeInfo;
    int* clientAction;
    _BYTE gapB8[40];
    int field_E0;
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    _BYTE gapF4[3004];
    int field_CB0;
    char field_CB4;
    char field_CB5;
    char field_CB6;
    char field_CB7;
    char field_CB8;
    char field_CB9;
    char field_CBA;
    char field_CBB;
    char field_CBC;
    char field_CBD;
    char field_CBE;
    char field_CBF;
    char field_CC0;
    char field_CC1;
    char field_CC2;
    char field_CC3;
};

struct ServerCommandQueue
{
    u32 writePtr;
    u32 readPtr;
    u8 buffer[3000];
};

struct SWAN_ALIGNED(4) m_record
{
    u32 Turn;
    __int16 MoveID;
    u8 PokeID;
    u8 fEffective;
};

struct MoveRecord
{
    u32 ptr;
    m_record record[120];
};

struct FaintRecordUnit
{
    u8 count;
    u8 fExpChecked[24];
    u8 FaintPokeID[24];
};

struct FaintRecord
{
    FaintRecordUnit turnRecord[4];
};

struct MoveAnimCtrl
{
    unsigned __int16 MoveID;
    unsigned __int8 attackerPos;
    unsigned __int8 targetPos;
    unsigned __int8 effectIndex;
    unsigned __int8 flags;
    unsigned __int16 subEff;
};

struct SWAN_PACKED SWAN_ALIGNED(1) MoveStealParam
{
    char stealCount;
    char stealPokeID[6];
    char targetPos[6];
    char targetPokeID[6];
};

struct HITCHECK_PARAM
{
    char countMax;
    char count;
    char fCheckEveryTime;
    char fMultiHitMove;
    char fPutAnimCmd;
    char MultiHitEffectiveness;
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

struct SWAN_PACKED SWAN_ALIGNED(1) calc_damage_record
{
    u8 unk0;
    u16 damage;
    u8 pokeID;
};

struct SWAN_PACKED SWAN_ALIGNED(2) CALC_DAMAGE_REC
{
    char real_hit_count;
    char substitute_hit_count;
    char total_hit_count;
    calc_damage_record record[6];
};

struct MoveParam
{
    unsigned __int16 MoveID;
    unsigned __int16 OriginalMoveID;
    unsigned __int16 userType;
    unsigned __int8 moveType;
    char damageType;
    int category;
    int targetType;
    int flags;
};

struct POKESET_STACK_UNIT
{
    PokeSet TargetOriginal;
    PokeSet Target;
    PokeSet Ally;
    PokeSet Enemy;
    PokeSet Damaged;
    PokeSet StealTarget;
    PokeSet psetTemp;
    MoveAnimCtrl moveAnimCtrl;
    MoveParam moveParam;
    MoveParam moveParamOriginal;
    HITCHECK_PARAM hitCheck;
    MoveStealParam moveStealParam;
    MoveStealParam magicCoatParam;
    CALC_DAMAGE_REC calcDamageAlly;
    CALC_DAMAGE_REC calcDamageEnemy;
    char defaultTargetPos;
    char field_28D;
    char field_28E;
    char field_28F;
};

struct PosPokeState
{
    u8 fEnable;
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

struct EffectivenessCounter
{
    u16 NoEffectHits;
    u16 SuperEffectiveHits;
    u16 NotVeryEffectiveHits;
    u16 OpponentNoEffectHits;
    u16 OpponentSuperEffectiveHits;
    u16 OpponentNotVeryEffectiveHits;
};

struct SWAN_ALIGNED(4) ActionOrderWork
{
    BattleMon* battleMon;
    BattleActionParam* Action;
    u32 field_8;
    char field_C;
    char fDone;
    char field_E;
    char field_F;
};

struct HandlerParam_StrParams
{
    u16 ID;
    int type;
    int args[8];
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

struct HitCheckParam
{
    char countMax;
    char count;
    char fCheckEveryTime;
    char fMultiHitMove;
    char fPutAnimCmd;
    char MultiHitEffectiveness;
};

struct SWAN_ALIGNED(8) ServerFlow
{
    BtlServerWk* server;
    MainModule* mainModule;
    PokeCon* pokeCon;
    ServerCommandQueue* serverCommandQueue;
    int turnCount;
    int flowResult;
    int heapHandle;
    MoveRecord moveRecord;
    FaintRecord faintRecord;
    ArcTool* evolutionDataPtr;
    MoveAnimCtrl* moveAnimCtrl;
    MoveStealParam* moveStealParam;
    MoveStealParam* magicCoatParam;
    HitCheckParam* hitCheckParam;
    EscapeInfo escapeInfo;
    int field_4C0;
    int field_4C4;
    int field_4C8;
    __int16 field_4CC;
    __int16 field_4CE;
    int field_4D0;
    u8 field_4D4[672];
    int SimulationCounter;
    int moveSerial;
    char cmdBuildStep;
    char field_77D;
    char TurnCheckSeq;
    char defaultTargetPos;
    __int16 heapID;
    char numActOrder;
    char numEndActOrder;
    _BYTE field_784;
    char revivedPokeCount;
    char field_786;
    char field_787;
    char field_788;
    char thruDeadMsgPokeID;
    char field_78A;
    char gap78B;
    char field_78C;
    char field_78D;
    char field_78E;
    char field_78F;
    char field_790;
    u8 revivePokeID[24];
    char pokeInFlag[24];
    char field_7C1[24];
    char switchCount[4];
    char field_7DD;
    char field_7DE;
    char field_7DF;
    ActionOrderWork actionOrderWork[6];
    ActionOrderWork tempActionOrderWork;
    PokeSet* setTargetOriginal;
    PokeSet* setTarget;
    PokeSet* setAlly;
    PokeSet* setEnemy;
    PokeSet* PokeSetDamaged;
    PokeSet* setStealTarget;
    PokeSet* PokeSetTemp;
    CALC_DAMAGE_REC* calcDamageAlly;
    CALC_DAMAGE_REC* calcDamageEnemy;
    POKESET_STACK_UNIT pokesetUnit[7];
    int pokesetStackPtr;
    PokeSet pokeSet_1A68;
    MoveParam* moveParam;
    MoveParam* moveParamOriginal;
    PosPoke posPoke;
    char field_1AE2;
    char field_1AE3;
    HandlerParam_StrParams strParam;
    CalcExpWork* levelUpInfo;
    _BYTE gap1B10[68];
    int field_1B54;
    _BYTE gap1B58[316];
    char field_1C88;
    char field_1C89;
    char field_1C8A;
    char field_1C97;
    _BYTE gap1C98[224];
    int HEManager;
    _BYTE gap1D7C[8];
    char field_1D84;
    char field_1D85;
    char field_1D86;
    char field_1D87;
    char field_1D88;
    char field_1D89;
    char field_1D8A;
    _BYTE gap1D8B[481];
    char field_1F6C;
    _BYTE gap1F6D[11];
    __int16 PrevUsedMove;
    char field_1F7A;
    char field_1F7B;
    ActionIgnoreReason actionIgnoreReason;
    EffectivenessCounter effectivenessCounter;
    int dmgAffRec;
    _BYTE gap1F90[8];
    char field_1F98;
    _BYTE gap1F99[83];
    u8 field_1FEC[4];
    u8 TempWork;
    char field_1FF1;
    _BYTE gap1FF2[318];
    int field_2130;
    _BYTE gap2134[8];
    char field_213C;
    char field_213D;
    char field_213E;
    char field_213F;
};

enum ItemID
{
    IT_NULL = 0x0,
    IT0001_MASTER_BALL = 0x1,
    IT0002_ULTRA_BALL = 0x2,
    IT0003_GREAT_BALL = 0x3,
    IT0004_POKE_BALL = 0x4,
    IT0005_SAFARI_BALL = 0x5,
    IT0006_NET_BALL = 0x6,
    IT0007_DIVE_BALL = 0x7,
    IT0008_NEST_BALL = 0x8,
    IT0009_REPEAT_BALL = 0x9,
    IT0010_TIMER_BALL = 0xA,
    IT0011_LUXURY_BALL = 0xB,
    IT0012_PREMIER_BALL = 0xC,
    IT0013_DUSK_BALL = 0xD,
    IT0014_HEAL_BALL = 0xE,
    IT0015_QUICK_BALL = 0xF,
    IT0016_CHERISH_BALL = 0x10,
    IT0017_POTION = 0x11,
    IT0018_ANTIDOTE = 0x12,
    IT0019_BURN_HEAL = 0x13,
    IT0020_ICE_HEAL = 0x14,
    IT0021_AWAKENING = 0x15,
    IT0022_PARALYZE_HEAL = 0x16,
    IT0023_FULL_RESTORE = 0x17,
    IT0024_MAX_POTION = 0x18,
    IT0025_HYPER_POTION = 0x19,
    IT0026_SUPER_POTION = 0x1A,
    IT0027_FULL_HEAL = 0x1B,
    IT0028_REVIVE = 0x1C,
    IT0029_MAX_REVIVE = 0x1D,
    IT0030_FRESH_WATER = 0x1E,
    IT0031_SODA_POP = 0x1F,
    IT0032_LEMONADE = 0x20,
    IT0033_MOOMOO_MILK = 0x21,
    IT0034_ENERGY_POWDER = 0x22,
    IT0035_ENERGY_ROOT = 0x23,
    IT0036_HEAL_POWDER = 0x24,
    IT0037_REVIVAL_HERB = 0x25,
    IT0038_ETHER = 0x26,
    IT0039_MAX_ETHER = 0x27,
    IT0040_ELIXIR = 0x28,
    IT0041_MAX_ELIXIR = 0x29,
    IT0042_LAVA_COOKIE = 0x2A,
    IT0043_BERRY_JUICE = 0x2B,
    IT0044_SACRED_ASH = 0x2C,
    IT0045_HP_UP = 0x2D,
    IT0046_PROTEIN = 0x2E,
    IT0047_IRON = 0x2F,
    IT0048_CARBOS = 0x30,
    IT0049_CALCIUM = 0x31,
    IT0050_RARE_CANDY = 0x32,
    IT0051_PP_UP = 0x33,
    IT0052_ZINC = 0x34,
    IT0053_PP_MAX = 0x35,
    IT0054_OLD_GATEAU = 0x36,
    IT0055_GUARD_SPEC = 0x37,
    IT0056_DIRE_HIT = 0x38,
    IT0057_X_ATTACK = 0x39,
    IT0058_X_DEFENSE = 0x3A,
    IT0059_X_SPEED = 0x3B,
    IT0060_X_ACCURACY = 0x3C,
    IT0061_X_SP_ATK = 0x3D,
    IT0062_X_SP_DEF = 0x3E,
    IT0063_POKE_DOLL = 0x3F,
    IT0064_FLUFFY_TAIL = 0x40,
    IT0065_BLUE_FLUTE = 0x41,
    IT0066_YELLOW_FLUTE = 0x42,
    IT0067_RED_FLUTE = 0x43,
    IT0068_BLACK_FLUTE = 0x44,
    IT0069_WHITE_FLUTE = 0x45,
    IT0070_SHOAL_SALT = 0x46,
    IT0071_SHOAL_SHELL = 0x47,
    IT0072_RED_SHARD = 0x48,
    IT0073_BLUE_SHARD = 0x49,
    IT0074_YELLOW_SHARD = 0x4A,
    IT0075_GREEN_SHARD = 0x4B,
    IT0076_SUPER_REPEL = 0x4C,
    IT0077_MAX_REPEL = 0x4D,
    IT0078_ESCAPE_ROPE = 0x4E,
    IT0079_REPEL = 0x4F,
    IT0080_SUN_STONE = 0x50,
    IT0081_MOON_STONE = 0x51,
    IT0082_FIRE_STONE = 0x52,
    IT0083_THUNDER_STONE = 0x53,
    IT0084_WATER_STONE = 0x54,
    IT0085_LEAF_STONE = 0x55,
    IT0086_TINY_MUSHROOM = 0x56,
    IT0087_BIG_MUSHROOM = 0x57,
    IT0088_PEARL = 0x58,
    IT0089_BIG_PEARL = 0x59,
    IT0090_STARDUST = 0x5A,
    IT0091_STAR_PIECE = 0x5B,
    IT0092_NUGGET = 0x5C,
    IT0093_HEART_SCALE = 0x5D,
    IT0094_HONEY = 0x5E,
    IT0095_GROWTH_MULCH = 0x5F,
    IT0096_DAMP_MULCH = 0x60,
    IT0097_STABLE_MULCH = 0x61,
    IT0098_GOOEY_MULCH = 0x62,
    IT0099_ROOT_FOSSIL = 0x63,
    IT0100_CLAW_FOSSIL = 0x64,
    IT0101_HELIX_FOSSIL = 0x65,
    IT0102_DOME_FOSSIL = 0x66,
    IT0103_OLD_AMBER = 0x67,
    IT0104_ARMOR_FOSSIL = 0x68,
    IT0105_SKULL_FOSSIL = 0x69,
    IT0106_RARE_BONE = 0x6A,
    IT0107_SHINY_STONE = 0x6B,
    IT0108_DUSK_STONE = 0x6C,
    IT0109_DAWN_STONE = 0x6D,
    IT0110_OVAL_STONE = 0x6E,
    IT0111_ODD_KEYSTONE = 0x6F,
    IT0112_GRISEOUS_ORB = 0x70,
    IT0113 = 0x71,
    IT0114 = 0x72,
    IT0115 = 0x73,
    IT0116_DOUSE_DRIVE = 0x74,
    IT0117_SHOCK_DRIVE = 0x75,
    IT0118_BURN_DRIVE = 0x76,
    IT0119_CHILL_DRIVE = 0x77,
    IT0120 = 0x78,
    IT0121 = 0x79,
    IT0122 = 0x7A,
    IT0123 = 0x7B,
    IT0124 = 0x7C,
    IT0125 = 0x7D,
    IT0126 = 0x7E,
    IT0127 = 0x7F,
    IT0128 = 0x80,
    IT0129 = 0x81,
    IT0130 = 0x82,
    IT0131 = 0x83,
    IT0132 = 0x84,
    IT0133 = 0x85,
    IT0134_SWEET_HEART = 0x86,
    IT0135_ADAMANT_ORB = 0x87,
    IT0136_LUSTROUS_ORB = 0x88,
    IT0137_GREET_MAIL = 0x89,
    IT0138_FAVORED_MAIL = 0x8A,
    IT0139_RSVP_MAIL = 0x8B,
    IT0140_THANKS_MAIL = 0x8C,
    IT0141_INQUIRY_MAIL = 0x8D,
    IT0142_LIKE_MAIL = 0x8E,
    IT0143_REPLY_MAIL = 0x8F,
    IT0144_BRIDGE_MAIL_S = 0x90,
    IT0145_BRIDGE_MAIL_D = 0x91,
    IT0146_BRIDGE_MAIL_T = 0x92,
    IT0147_BRIDGE_MAIL_V = 0x93,
    IT0148_BRIDGE_MAIL_M = 0x94,
    IT0149_CHERI_BERRY = 0x95,
    IT0150_CHESTO_BERRY = 0x96,
    IT0151_PECHA_BERRY = 0x97,
    IT0152_RAWST_BERRY = 0x98,
    IT0153_ASPEAR_BERRY = 0x99,
    IT0154_LEPPA_BERRY = 0x9A,
    IT0155_ORAN_BERRY = 0x9B,
    IT0156_PERSIM_BERRY = 0x9C,
    IT0157_LUM_BERRY = 0x9D,
    IT0158_SITRUS_BERRY = 0x9E,
    IT0159_FIGY_BERRY = 0x9F,
    IT0160_WIKI_BERRY = 0xA0,
    IT0161_MAGO_BERRY = 0xA1,
    IT0162_AGUAV_BERRY = 0xA2,
    IT0163_IAPAPA_BERRY = 0xA3,
    IT0164_RAZZ_BERRY = 0xA4,
    IT0165_BLUK_BERRY = 0xA5,
    IT0166_NANAB_BERRY = 0xA6,
    IT0167_WEPEAR_BERRY = 0xA7,
    IT0168_PINAP_BERRY = 0xA8,
    IT0169_POMEG_BERRY = 0xA9,
    IT0170_KELPSY_BERRY = 0xAA,
    IT0171_QUALOT_BERRY = 0xAB,
    IT0172_HONDEW_BERRY = 0xAC,
    IT0173_GREPA_BERRY = 0xAD,
    IT0174_TAMATO_BERRY = 0xAE,
    IT0175_CORNN_BERRY = 0xAF,
    IT0176_MAGOST_BERRY = 0xB0,
    IT0177_RABUTA_BERRY = 0xB1,
    IT0178_NOMEL_BERRY = 0xB2,
    IT0179_SPELON_BERRY = 0xB3,
    IT0180_PAMTRE_BERRY = 0xB4,
    IT0181_WATMEL_BERRY = 0xB5,
    IT0182_DURIN_BERRY = 0xB6,
    IT0183_BELUE_BERRY = 0xB7,
    IT0184_OCCA_BERRY = 0xB8,
    IT0185_PASSHO_BERRY = 0xB9,
    IT0186_WACAN_BERRY = 0xBA,
    IT0187_RINDO_BERRY = 0xBB,
    IT0188_YACHE_BERRY = 0xBC,
    IT0189_CHOPLE_BERRY = 0xBD,
    IT0190_KEBIA_BERRY = 0xBE,
    IT0191_SHUCA_BERRY = 0xBF,
    IT0192_COBA_BERRY = 0xC0,
    IT0193_PAYAPA_BERRY = 0xC1,
    IT0194_TANGA_BERRY = 0xC2,
    IT0195_CHARTI_BERRY = 0xC3,
    IT0196_KASIB_BERRY = 0xC4,
    IT0197_HABAN_BERRY = 0xC5,
    IT0198_COLBUR_BERRY = 0xC6,
    IT0199_BABIRI_BERRY = 0xC7,
    IT0200_CHILAN_BERRY = 0xC8,
    IT0201_LIECHI_BERRY = 0xC9,
    IT0202_GANLON_BERRY = 0xCA,
    IT0203_SALAC_BERRY = 0xCB,
    IT0204_PETAYA_BERRY = 0xCC,
    IT0205_APICOT_BERRY = 0xCD,
    IT0206_LANSAT_BERRY = 0xCE,
    IT0207_STARF_BERRY = 0xCF,
    IT0208_ENIGMA_BERRY = 0xD0,
    IT0209_MICLE_BERRY = 0xD1,
    IT0210_CUSTAP_BERRY = 0xD2,
    IT0211_JABOCA_BERRY = 0xD3,
    IT0212_ROWAP_BERRY = 0xD4,
    IT0213_BRIGHT_POWDER = 0xD5,
    IT0214_WHITE_HERB = 0xD6,
    IT0215_MACHO_BRACE = 0xD7,
    IT0216_EXP_SHARE = 0xD8,
    IT0217_QUICK_CLAW = 0xD9,
    IT0218_SOOTHE_BELL = 0xDA,
    IT0219_MENTAL_HERB = 0xDB,
    IT0220_CHOICE_BAND = 0xDC,
    IT0221_KING_S_ROCK = 0xDD,
    IT0222_SILVER_POWDER = 0xDE,
    IT0223_AMULET_COIN = 0xDF,
    IT0224_CLEANSE_TAG = 0xE0,
    IT0225_SOUL_DEW = 0xE1,
    IT0226_DEEP_SEA_TOOTH = 0xE2,
    IT0227_DEEP_SEA_SCALE = 0xE3,
    IT0228_SMOKE_BALL = 0xE4,
    IT0229_EVERSTONE = 0xE5,
    IT0230_FOCUS_BAND = 0xE6,
    IT0231_LUCKY_EGG = 0xE7,
    IT0232_SCOPE_LENS = 0xE8,
    IT0233_METAL_COAT = 0xE9,
    IT0234_LEFTOVERS = 0xEA,
    IT0235_DRAGON_SCALE = 0xEB,
    IT0236_LIGHT_BALL = 0xEC,
    IT0237_SOFT_SAND = 0xED,
    IT0238_HARD_STONE = 0xEE,
    IT0239_MIRACLE_SEED = 0xEF,
    IT0240_BLACK_GLASSES = 0xF0,
    IT0241_BLACK_BELT = 0xF1,
    IT0242_MAGNET = 0xF2,
    IT0243_MYSTIC_WATER = 0xF3,
    IT0244_SHARP_BEAK = 0xF4,
    IT0245_POISON_BARB = 0xF5,
    IT0246_NEVER_MELT_ICE = 0xF6,
    IT0247_SPELL_TAG = 0xF7,
    IT0248_TWISTED_SPOON = 0xF8,
    IT0249_CHARCOAL = 0xF9,
    IT0250_DRAGON_FANG = 0xFA,
    IT0251_SILK_SCARF = 0xFB,
    IT0252_UP_GRADE = 0xFC,
    IT0253_SHELL_BELL = 0xFD,
    IT0254_SEA_INCENSE = 0xFE,
    IT0255_LAX_INCENSE = 0xFF,
    IT0256_LUCKY_PUNCH = 0x100,
    IT0257_METAL_POWDER = 0x101,
    IT0258_THICK_CLUB = 0x102,
    IT0259_STICK = 0x103,
    IT0260_RED_SCARF = 0x104,
    IT0261_BLUE_SCARF = 0x105,
    IT0262_PINK_SCARF = 0x106,
    IT0263_GREEN_SCARF = 0x107,
    IT0264_YELLOW_SCARF = 0x108,
    IT0265_WIDE_LENS = 0x109,
    IT0266_MUSCLE_BAND = 0x10A,
    IT0267_WISE_GLASSES = 0x10B,
    IT0268_EXPERT_BELT = 0x10C,
    IT0269_LIGHT_CLAY = 0x10D,
    IT0270_LIFE_ORB = 0x10E,
    IT0271_POWER_HERB = 0x10F,
    IT0272_TOXIC_ORB = 0x110,
    IT0273_FLAME_ORB = 0x111,
    IT0274_QUICK_POWDER = 0x112,
    IT0275_FOCUS_SASH = 0x113,
    IT0276_ZOOM_LENS = 0x114,
    IT0277_METRONOME = 0x115,
    IT0278_IRON_BALL = 0x116,
    IT0279_LAGGING_TAIL = 0x117,
    IT0280_DESTINY_KNOT = 0x118,
    IT0281_BLACK_SLUDGE = 0x119,
    IT0282_ICY_ROCK = 0x11A,
    IT0283_SMOOTH_ROCK = 0x11B,
    IT0284_HEAT_ROCK = 0x11C,
    IT0285_DAMP_ROCK = 0x11D,
    IT0286_GRIP_CLAW = 0x11E,
    IT0287_CHOICE_SCARF = 0x11F,
    IT0288_STICKY_BARB = 0x120,
    IT0289_POWER_BRACER = 0x121,
    IT0290_POWER_BELT = 0x122,
    IT0291_POWER_LENS = 0x123,
    IT0292_POWER_BAND = 0x124,
    IT0293_POWER_ANKLET = 0x125,
    IT0294_POWER_WEIGHT = 0x126,
    IT0295_SHED_SHELL = 0x127,
    IT0296_BIG_ROOT = 0x128,
    IT0297_CHOICE_SPECS = 0x129,
    IT0298_FLAME_PLATE = 0x12A,
    IT0299_SPLASH_PLATE = 0x12B,
    IT0300_ZAP_PLATE = 0x12C,
    IT0301_MEADOW_PLATE = 0x12D,
    IT0302_ICICLE_PLATE = 0x12E,
    IT0303_FIST_PLATE = 0x12F,
    IT0304_TOXIC_PLATE = 0x130,
    IT0305_EARTH_PLATE = 0x131,
    IT0306_SKY_PLATE = 0x132,
    IT0307_MIND_PLATE = 0x133,
    IT0308_INSECT_PLATE = 0x134,
    IT0309_STONE_PLATE = 0x135,
    IT0310_SPOOKY_PLATE = 0x136,
    IT0311_DRACO_PLATE = 0x137,
    IT0312_DREAD_PLATE = 0x138,
    IT0313_IRON_PLATE = 0x139,
    IT0314_ODD_INCENSE = 0x13A,
    IT0315_ROCK_INCENSE = 0x13B,
    IT0316_FULL_INCENSE = 0x13C,
    IT0317_WAVE_INCENSE = 0x13D,
    IT0318_ROSE_INCENSE = 0x13E,
    IT0319_LUCK_INCENSE = 0x13F,
    IT0320_PURE_INCENSE = 0x140,
    IT0321_PROTECTOR = 0x141,
    IT0322_ELECTIRIZER = 0x142,
    IT0323_MAGMARIZER = 0x143,
    IT0324_DUBIOUS_DISC = 0x144,
    IT0325_REAPER_CLOTH = 0x145,
    IT0326_RAZOR_CLAW = 0x146,
    IT0327_RAZOR_FANG = 0x147,
    IT0328_TM01 = 0x148,
    IT0329_TM02 = 0x149,
    IT0330_TM03 = 0x14A,
    IT0331_TM04 = 0x14B,
    IT0332_TM05 = 0x14C,
    IT0333_TM06 = 0x14D,
    IT0334_TM07 = 0x14E,
    IT0335_TM08 = 0x14F,
    IT0336_TM09 = 0x150,
    IT0337_TM10 = 0x151,
    IT0338_TM11 = 0x152,
    IT0339_TM12 = 0x153,
    IT0340_TM13 = 0x154,
    IT0341_TM14 = 0x155,
    IT0342_TM15 = 0x156,
    IT0343_TM16 = 0x157,
    IT0344_TM17 = 0x158,
    IT0345_TM18 = 0x159,
    IT0346_TM19 = 0x15A,
    IT0347_TM20 = 0x15B,
    IT0348_TM21 = 0x15C,
    IT0349_TM22 = 0x15D,
    IT0350_TM23 = 0x15E,
    IT0351_TM24 = 0x15F,
    IT0352_TM25 = 0x160,
    IT0353_TM26 = 0x161,
    IT0354_TM27 = 0x162,
    IT0355_TM28 = 0x163,
    IT0356_TM29 = 0x164,
    IT0357_TM30 = 0x165,
    IT0358_TM31 = 0x166,
    IT0359_TM32 = 0x167,
    IT0360_TM33 = 0x168,
    IT0361_TM34 = 0x169,
    IT0362_TM35 = 0x16A,
    IT0363_TM36 = 0x16B,
    IT0364_TM37 = 0x16C,
    IT0365_TM38 = 0x16D,
    IT0366_TM39 = 0x16E,
    IT0367_TM40 = 0x16F,
    IT0368_TM41 = 0x170,
    IT0369_TM42 = 0x171,
    IT0370_TM43 = 0x172,
    IT0371_TM44 = 0x173,
    IT0372_TM45 = 0x174,
    IT0373_TM46 = 0x175,
    IT0374_TM47 = 0x176,
    IT0375_TM48 = 0x177,
    IT0376_TM49 = 0x178,
    IT0377_TM50 = 0x179,
    IT0378_TM51 = 0x17A,
    IT0379_TM52 = 0x17B,
    IT0380_TM53 = 0x17C,
    IT0381_TM54 = 0x17D,
    IT0382_TM55 = 0x17E,
    IT0383_TM56 = 0x17F,
    IT0384_TM57 = 0x180,
    IT0385_TM58 = 0x181,
    IT0386_TM59 = 0x182,
    IT0387_TM60 = 0x183,
    IT0388_TM61 = 0x184,
    IT0389_TM62 = 0x185,
    IT0390_TM63 = 0x186,
    IT0391_TM64 = 0x187,
    IT0392_TM65 = 0x188,
    IT0393_TM66 = 0x189,
    IT0394_TM67 = 0x18A,
    IT0395_TM68 = 0x18B,
    IT0396_TM69 = 0x18C,
    IT0397_TM70 = 0x18D,
    IT0398_TM71 = 0x18E,
    IT0399_TM72 = 0x18F,
    IT0400_TM73 = 0x190,
    IT0401_TM74 = 0x191,
    IT0402_TM75 = 0x192,
    IT0403_TM76 = 0x193,
    IT0404_TM77 = 0x194,
    IT0405_TM78 = 0x195,
    IT0406_TM79 = 0x196,
    IT0407_TM80 = 0x197,
    IT0408_TM81 = 0x198,
    IT0409_TM82 = 0x199,
    IT0410_TM83 = 0x19A,
    IT0411_TM84 = 0x19B,
    IT0412_TM85 = 0x19C,
    IT0413_TM86 = 0x19D,
    IT0414_TM87 = 0x19E,
    IT0415_TM88 = 0x19F,
    IT0416_TM89 = 0x1A0,
    IT0417_TM90 = 0x1A1,
    IT0418_TM91 = 0x1A2,
    IT0419_TM92 = 0x1A3,
    IT0420_HM01 = 0x1A4,
    IT0421_HM02 = 0x1A5,
    IT0422_HM03 = 0x1A6,
    IT0423_HM04 = 0x1A7,
    IT0424_HM05 = 0x1A8,
    IT0425_HM06 = 0x1A9,
    IT0426 = 0x1AA,
    IT0427 = 0x1AB,
    IT0428_EXPLORER_KIT = 0x1AC,
    IT0429_LOOT_SACK = 0x1AD,
    IT0430_RULE_BOOK = 0x1AE,
    IT0431_POKE_RADAR = 0x1AF,
    IT0432_POINT_CARD = 0x1B0,
    IT0433_JOURNAL = 0x1B1,
    IT0434_SEAL_CASE = 0x1B2,
    IT0435_FASHION_CASE = 0x1B3,
    IT0436_SEAL_BAG = 0x1B4,
    IT0437_PAL_PAD = 0x1B5,
    IT0438_WORKS_KEY = 0x1B6,
    IT0439_OLD_CHARM = 0x1B7,
    IT0440_GALACTIC_KEY = 0x1B8,
    IT0441_RED_CHAIN = 0x1B9,
    IT0442_TOWN_MAP = 0x1BA,
    IT0443_VS_SEEKER = 0x1BB,
    IT0444_COIN_CASE = 0x1BC,
    IT0445_OLD_ROD = 0x1BD,
    IT0446_GOOD_ROD = 0x1BE,
    IT0447_SUPER_ROD = 0x1BF,
    IT0448_SPRAYDUCK = 0x1C0,
    IT0449_POFFIN_CASE = 0x1C1,
    IT0450_BIKE = 0x1C2,
    IT0451_SUITE_KEY = 0x1C3,
    IT0452_OAK_S_LETTER = 0x1C4,
    IT0453_LUNAR_WING = 0x1C5,
    IT0454_MEMBER_CARD = 0x1C6,
    IT0455_AZURE_FLUTE = 0x1C7,
    IT0456_S_S_TICKET = 0x1C8,
    IT0457_CONTEST_PASS = 0x1C9,
    IT0458_MAGMA_STONE = 0x1CA,
    IT0459_PARCEL = 0x1CB,
    IT0460_COUPON_1 = 0x1CC,
    IT0461_COUPON_2 = 0x1CD,
    IT0462_COUPON_3 = 0x1CE,
    IT0463_STORAGE_KEY = 0x1CF,
    IT0464_SECRET_POTION = 0x1D0,
    IT0465_VS_RECORDER = 0x1D1,
    IT0466_GRACIDEA = 0x1D2,
    IT0467_SECRET_KEY = 0x1D3,
    IT0468_APRICORN_BOX = 0x1D4,
    IT0469_UNOWN_REPORT = 0x1D5,
    IT0470_BERRY_POTS = 0x1D6,
    IT0471_DOWSING_MACHINE = 0x1D7,
    IT0472_BLUE_CARD = 0x1D8,
    IT0473_SLOWPOKE_TAIL = 0x1D9,
    IT0474_CLEAR_BELL = 0x1DA,
    IT0475_CARD_KEY = 0x1DB,
    IT0476_BASEMENT_KEY = 0x1DC,
    IT0477_SQUIRT_BOTTLE = 0x1DD,
    IT0478_RED_SCALE = 0x1DE,
    IT0479_LOST_ITEM = 0x1DF,
    IT0480_PASS = 0x1E0,
    IT0481_MACHINE_PART = 0x1E1,
    IT0482_SILVER_WING = 0x1E2,
    IT0483_RAINBOW_WING = 0x1E3,
    IT0484_MYSTERY_EGG = 0x1E4,
    IT0485_RED_APRICORN = 0x1E5,
    IT0486_BLUE_APRICORN = 0x1E6,
    IT0487_YELLOW_APRICORN = 0x1E7,
    IT0488_GREEN_APRICORN = 0x1E8,
    IT0489_PINK_APRICORN = 0x1E9,
    IT0490_WHITE_APRICORN = 0x1EA,
    IT0491_BLACK_APRICORN = 0x1EB,
    IT0492_FAST_BALL = 0x1EC,
    IT0493_LEVEL_BALL = 0x1ED,
    IT0494_LURE_BALL = 0x1EE,
    IT0495_HEAVY_BALL = 0x1EF,
    IT0496_LOVE_BALL = 0x1F0,
    IT0497_FRIEND_BALL = 0x1F1,
    IT0498_MOON_BALL = 0x1F2,
    IT0499_SPORT_BALL = 0x1F3,
    IT0500_PARK_BALL = 0x1F4,
    IT0501_PHOTO_ALBUM = 0x1F5,
    IT0502_GB_SOUNDS = 0x1F6,
    IT0503_TIDAL_BELL = 0x1F7,
    IT0504_RAGE_CANDY_BAR = 0x1F8,
    IT0505_DATA_CARD_01 = 0x1F9,
    IT0506_DATA_CARD_02 = 0x1FA,
    IT0507_DATA_CARD_03 = 0x1FB,
    IT0508_DATA_CARD_04 = 0x1FC,
    IT0509_DATA_CARD_05 = 0x1FD,
    IT0510_DATA_CARD_06 = 0x1FE,
    IT0511_DATA_CARD_07 = 0x1FF,
    IT0512_DATA_CARD_08 = 0x200,
    IT0513_DATA_CARD_09 = 0x201,
    IT0514_DATA_CARD_10 = 0x202,
    IT0515_DATA_CARD_11 = 0x203,
    IT0516_DATA_CARD_12 = 0x204,
    IT0517_DATA_CARD_13 = 0x205,
    IT0518_DATA_CARD_14 = 0x206,
    IT0519_DATA_CARD_15 = 0x207,
    IT0520_DATA_CARD_16 = 0x208,
    IT0521_DATA_CARD_17 = 0x209,
    IT0522_DATA_CARD_18 = 0x20A,
    IT0523_DATA_CARD_19 = 0x20B,
    IT0524_DATA_CARD_20 = 0x20C,
    IT0525_DATA_CARD_21 = 0x20D,
    IT0526_DATA_CARD_22 = 0x20E,
    IT0527_DATA_CARD_23 = 0x20F,
    IT0528_DATA_CARD_24 = 0x210,
    IT0529_DATA_CARD_25 = 0x211,
    IT0530_DATA_CARD_26 = 0x212,
    IT0531_DATA_CARD_27 = 0x213,
    IT0532_JADE_ORB = 0x214,
    IT0533_LOCK_CAPSULE = 0x215,
    IT0534_RED_ORB = 0x216,
    IT0535_BLUE_ORB = 0x217,
    IT0536_ENIGMA_STONE = 0x218,
    IT0537_PRISM_SCALE = 0x219,
    IT0538_EVIOLITE = 0x21A,
    IT0539_FLOAT_STONE = 0x21B,
    IT0540_ROCKY_HELMET = 0x21C,
    IT0541_AIR_BALLOON = 0x21D,
    IT0542_RED_CARD = 0x21E,
    IT0543_RING_TARGET = 0x21F,
    IT0544_BINDING_BAND = 0x220,
    IT0545_ABSORB_BULB = 0x221,
    IT0546_CELL_BATTERY = 0x222,
    IT0547_EJECT_BUTTON = 0x223,
    IT0548_FIRE_GEM = 0x224,
    IT0549_WATER_GEM = 0x225,
    IT0550_ELECTRIC_GEM = 0x226,
    IT0551_GRASS_GEM = 0x227,
    IT0552_ICE_GEM = 0x228,
    IT0553_FIGHTING_GEM = 0x229,
    IT0554_POISON_GEM = 0x22A,
    IT0555_GROUND_GEM = 0x22B,
    IT0556_FLYING_GEM = 0x22C,
    IT0557_PSYCHIC_GEM = 0x22D,
    IT0558_BUG_GEM = 0x22E,
    IT0559_ROCK_GEM = 0x22F,
    IT0560_GHOST_GEM = 0x230,
    IT0561_DRAGON_GEM = 0x231,
    IT0562_DARK_GEM = 0x232,
    IT0563_STEEL_GEM = 0x233,
    IT0564_NORMAL_GEM = 0x234,
    IT0565_HEALTH_WING = 0x235,
    IT0566_MUSCLE_WING = 0x236,
    IT0567_RESIST_WING = 0x237,
    IT0568_GENIUS_WING = 0x238,
    IT0569_CLEVER_WING = 0x239,
    IT0570_SWIFT_WING = 0x23A,
    IT0571_PRETTY_WING = 0x23B,
    IT0572_COVER_FOSSIL = 0x23C,
    IT0573_PLUME_FOSSIL = 0x23D,
    IT0574_LIBERTY_PASS = 0x23E,
    IT0575_PASS_ORB = 0x23F,
    IT0576_DREAM_BALL = 0x240,
    IT0577_POKE_TOY = 0x241,
    IT0578_PROP_CASE = 0x242,
    IT0579_DRAGON_SKULL = 0x243,
    IT0580_BALM_MUSHROOM = 0x244,
    IT0581_BIG_NUGGET = 0x245,
    IT0582_PEARL_STRING = 0x246,
    IT0583_COMET_SHARD = 0x247,
    IT0584_RELIC_COPPER = 0x248,
    IT0585_RELIC_SILVER = 0x249,
    IT0586_RELIC_GOLD = 0x24A,
    IT0587_RELIC_VASE = 0x24B,
    IT0588_RELIC_BAND = 0x24C,
    IT0589_RELIC_STATUE = 0x24D,
    IT0590_RELIC_CROWN = 0x24E,
    IT0591_CASTELIACONE = 0x24F,
    IT0592_DIRE_HIT_2 = 0x250,
    IT0593_X_SPEED_2 = 0x251,
    IT0594_X_SP_ATK_2 = 0x252,
    IT0595_X_SP_DEF_2 = 0x253,
    IT0596_X_DEFENSE_2 = 0x254,
    IT0597_X_ATTACK_2 = 0x255,
    IT0598_X_ACCURACY_2 = 0x256,
    IT0599_X_SPEED_3 = 0x257,
    IT0600_X_SP_ATK_3 = 0x258,
    IT0601_X_SP_DEF_3 = 0x259,
    IT0602_X_DEFENSE_3 = 0x25A,
    IT0603_X_ATTACK_3 = 0x25B,
    IT0604_X_ACCURACY_3 = 0x25C,
    IT0605_X_SPEED_6 = 0x25D,
    IT0606_X_SP_ATK_6 = 0x25E,
    IT0607_X_SP_DEF_6 = 0x25F,
    IT0608_X_DEFENSE_6 = 0x260,
    IT0609_X_ATTACK_6 = 0x261,
    IT0610_X_ACCURACY_6 = 0x262,
    IT0611_ABILITY_URGE = 0x263,
    IT0612_ITEM_DROP = 0x264,
    IT0613_ITEM_URGE = 0x265,
    IT0614_RESET_URGE = 0x266,
    IT0615_DIRE_HIT_3 = 0x267,
    IT0616_LIGHT_STONE = 0x268,
    IT0617_DARK_STONE = 0x269,
    IT0618_TM93 = 0x26A,
    IT0619_TM94 = 0x26B,
    IT0620_TM95 = 0x26C,
    IT0621_XTRANSCEIVER = 0x26D,
    IT0622 = 0x26E,
    IT0623_GRAM_1 = 0x26F,
    IT0624_GRAM_2 = 0x270,
    IT0625_GRAM_3 = 0x271,
    IT0626_XTRANSCEIVER = 0x272,
    IT0627_MEDAL_BOX = 0x273,
    IT0628_DNA_SPLICERS = 0x274,
    IT0629_DNA_SPLICERS = 0x275,
    IT0630_PERMIT = 0x276,
    IT0631_OVAL_CHARM = 0x277,
    IT0632_SHINY_CHARM = 0x278,
    IT0633_PLASMA_CARD = 0x279,
    IT0634_GRUBBY_HANKY = 0x27A,
    IT0635_COLRESS_MACHINE = 0x27B,
    IT0636_DROPPED_ITEM = 0x27C,
    IT0637_DROPPED_ITEM = 0x27D,
    IT0638_REVEAL_GLASS = 0x27E,
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
    VALUE_15 = 0x15,
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


struct FRONT_POKE_SEEK_WORK
{
    u8 clientIdx;
    u8 pokeIdx;
    u8 endFlag;
    u8 unk;
};

enum PkmStat
{
    STAT_HP = 0x0,
    STAT_ATK = 0x1,
    STAT_DEF = 0x2,
    STAT_SPA = 0x3,
    STAT_SPD = 0x4,
    STAT_SPE = 0x5,
    STAT_MAX = 0x6,
};

enum MoveField
{
    MVDATA_TYPE = 0x0,
    MVDATA_QUALITY = 0x1,
    MVDATA_CATEGORY = 0x2,
    MVDATA_POWER = 0x3,
    MVDATA_ACCURACY = 0x4,
    MVDATA_BASEPP = 0x5,
    MVDATA_PRIORITY = 0x6,
    MVDATA_CRIT_STAGE = 0x7,
    MVDATA_HIT_MAX = 0x8,
    MVDATA_HIT_MIN = 0x9,
    MVDATA_FLINCH_RATE = 0xA,
    MVDATA_INFLICT_STATUS = 0xB,
    MVDATA_INFLICT_CHANCE = 0xC,
    MVDATA_INFLICT_DURATION = 0xD,
    MVDATA_TURN_MIN = 0xE,
    MVDATA_TURN_MAX = 0xF,
    MVDATA_STAT1 = 0x10,
    MVDATA_STAT2 = 0x11,
    MVDATA_STAT3 = 0x12,
    MVDATA_STAT1_STAGE = 0x13,
    MVDATA_STAT2_STAGE = 0x14,
    MVDATA_STAT3_STAGE = 0x15,
    MVDATA_STAT1_CHANCE = 0x16,
    MVDATA_STAT2_CHANCE = 0x17,
    MVDATA_STAT3_CHANCE = 0x18,
    MVDATA_RECOIL = 0x19,
    MVDATA_HEAL = 0x1A,
    MVDATA_TARGET = 0x1B,
    MVDATA_WAZASEQ_ID = 0x1C,
    MVDATA_UNUSED = 0x1D,
    MVDATA_RECOIL_NEG = 0x1E,
    MVDATA_HEAL_NEG = 0x1F,
};

enum MoveFlag
{
    FLAG_CONTACT = 0x0,
    FLAG_REQUIRES_CHARGE = 0x1,
    FLAG_RECHARGE = 0x2,
    FLAG_BLOCKED_BY_PROTECT = 0x3,
    FLAG_REFLECTED_BY_MAGIC_COAT = 0x4,
    FLAG_STOLEN_BY_SNATCH = 0x5,
    FLAG_COPIED_BY_MIRROR_MOVE = 0x6,
    FLAG_PUNCH = 0x7,
    FLAG_SOUND = 0x8,
    FLAG_GROUNDED_BY_GRAVITY = 0x9,
    FLAG_DEFROST = 0xA,
    FLAG_LONG_RANGE = 0xB,
    FLAG_HEALING = 0xC,
    FLAG_BYPASSES_SUBSTITUTE = 0xD,
};

struct EffectivenessRecorder
{
    int effectiveness[24];
};

enum ConditionFlag
{
    CONDITIONFLAG_ACTIONDONE = 0x0,
    CONDITIONFLAG_NOSWITCH = 0x1,
    CONDITIONFLAG_CHARGE = 0x2,
    CONDITIONFLAG_FLY = 0x3,
    CONDITIONFLAG_DIVE = 0x4,
    CONDITIONFLAG_DIG = 0x5,
    CONDITIONFLAG_SHADOWFORCE = 0x6,
    CONDITIONFLAG_DEFENSECURL = 0x7,
    CONDITIONFLAG_MINIMIZE = 0x8,
    CONDITIONFLAG_FOCUSENERGY = 0x9,
    CONDITIONFLAG_POWERTRICK = 0xA,
    CONDITIONFLAG_MICLEBERRY = 0xB,
    CONDITIONFLAG_NOACTION = 0xC,
    CONDITIONFLAG_FLASHFIRE = 0xD,
    CONDITIONFLAG_BATONPASS = 0xE,
};

enum TurnFlag
{
    TURNFLAG_ACTIONSTART = 0x0,
    TURNFLAG_ACTIONDONE = 0x1,
    TURNFLAG_DAMAGED = 0x2,
    TURNFLAG_MOVEPROCDONE = 0x3,
    TURNFLAG_FLINCH = 0x4,
    TURNFLAG_FOCUSPUNCHREADY = 0x5,
    TURNFLAG_FOCUSPUNCHFAIL = 0x6,
    TURNFLAG_PROTECT = 0x7,
    TURNFLAG_ITEMCONSUMED = 0x8,
    TURNFLAG_CANTUSEITEM = 0x9,
    TURNFLAG_COMBOMOVEREADY = 0xA,
    TURNFLAG_HIDEMOVEOFF = 0xB,
    TURNFLAG_MOVED = 0xC,
    TURNFLAG_TURNCHECKCONDITIONPASSED = 0xD,
    TURNFLAG_ACCURACYUP = 0xE,
    TURNFLAG_USINGFLING = 0xF,
};

enum Counter : unsigned __int8
{
    COUNTER_STOCKPILE = 0x0,
    COUNTER_STOCKPILE_DEF = 0x1,
    COUNTER_STOCKPILE_SPDEF = 0x2,
    COUNTER_PROTECT = 0x3,
    COUNTER_SKYDROP = 0x4,
};

struct ReqMoveWork
{
    u16 moveID;
    u8 targetPos;
};

enum BattleFieldEffect
{
    EFFECT_WEATHER = 0x0,
    EFFECT_TRICK_ROOM = 0x1,
    EFFECT_GRAVITY = 0x2,
    EFFECT_IMPRISON = 0x3,
    EFFECT_WATER_SPORT = 0x4,
    EFFECT_MUD_SPORT = 0x5,
    EFFECT_WONDER_ROOM = 0x6,
    EFFECT_MAGIC_ROOM = 0x7,
};

struct HandlerParam_Header
{
    u32 flags;
};

enum BattleHandlerEffect
{
    EFFECT_USE_HELD_ITEM = 0x0,
    EFFECT_USE_HELD_ITEM_ANIMATION = 0x1,
    EFFECT_ABILITY_POPUP_ADD = 0x2,
    EFFECT_ABILITY_POPUP_REMOVE = 0x3,
    EFFECT_MESSAGE = 0x4,
    EFFECT_RECOVER_HP = 0x5,
    EFFECT_DRAIN = 0x6,
    EFFECT_DAMAGE = 0x7,
    EFFECT_CHANGE_HP = 0x8,
    EFFECT_RESTORE_PP = 0x9,
    EFFECT_DECREMENT_PP = 0xA,
    EFFECT_CURE_STATUS = 0xB,
    EFFECT_ADD_CONDITION = 0xC,
    EFFECT_CHANGE_STAT_STAGE = 0xE,
    EFFECT_SET_STAT_STAGE = 0xF,
    EFFECT_RESET_STAT_STAGE = 0x10,
    EFFECT_SET_STAT = 0x11,
    EFFECT_RESTORE_STAT_STAGE = 0x12,
    EFFECT_FAINT = 0x13,
    EFFECT_CHANGE_TYPE = 0x14,
    EFFECT_SET_TURN_FLAG = 0x15,
    EFFECT_RESET_TURN_FLAG = 0x16,
    EFFECT_SET_CONDITION_FLAG = 0x17,
    EFFECT_RESET_CONDITION_FLAG = 0x18,
    EFFECT_ADD_SIDE_EFFECT = 0x19,
    EFFECT_REMOVE_SIDE_EFFECT = 0x1A,
    EFFECT_ADD_FIELD_EFFECT = 0x1B,
    EFFECT_REMOVE_FIELD_EFFECT = 0x1C,
    EFFECT_CHANGE_WEATHER = 0x1D,
    EFFECT_ADD_POS_EFFECT = 0x1E,
    EFFECT_CHANGE_ABILITY = 0x1F,
    EFFECT_SET_HELD_ITEM = 0x20,
    EFFECT_CHECK_HELD_ITEM = 0x21,
    EFFECT_FORCE_USE_HELD_ITEM = 0x22,
    EFFECT_CONSUME_ITEM = 0x23,
    EFFECT_SWAP_ITEM = 0x24,
    EFFECT_UPDATE_MOVE = 0x25,
    EFFECT_COUNTER = 0x26,
    EFFECT_DELAY_MOVE_DAMAGE = 0x27,
    EFFECT_QUIT_BATTLE = 0x28,
    EFFECT_SWITCH = 0x29,
    EFFECT_BATON_PASS = 0x2A,
    EFFECT_FLINCH = 0x2B,
    EFFECT_REVIVE = 0x2C,
    EFFECT_SET_WEIGHT = 0x2D,
    EFFECT_FORCE_SWITCH = 0x2E,
    EFFECT_INTERRUPT_ACTION = 0x2F,
    EFFECT_INTERRUPT_MOVE = 0x30,
    EFFECT_SEND_LAST = 0x31,
    EFFECT_SWAP_POKE = 0x32,
    EFFECT_TRANSFORM = 0x33,
    EFFECT_ILLUSION_BREAK = 0x34,
    EFFECT_GRAVITY_CHECK = 0x35,
    EFFECT_CANCEL_SEMI_INVULN = 0x36,
    EFFECT_ADD_ANIMATION = 0x37,
    EFFECT_REMOVE_MESSAGE_WINDOW = 0x38,
    EFFECT_CHANGE_FORM = 0x39,
    EFFECT_SET_ANIMATION_ID = 0x3A,
    EFFECT_PLAY_ANIMATION = 0x3B,
};

struct HandlerParam_SendLast
{
    HandlerParam_Header header;
    u8 pokeID;
    HandlerParam_StrParams exStr;
};

struct ItemBattleStats
{
    u8 CureInflict;
    u8 Boost[4];
    u8 FunctionFlags0;
    u8 FunctionFlags1;
    char EVHP;
    char EVATK;
    char EVDEF;
    char EVSPE;
    char EVSPA;
    char EVSPD;
    u8 HealAmount;
    u8 PPGain;
    char Friendship1;
    char Friendship2;
    char Friendship3;
    char field_1F;
    char field_20;
};

struct SWAN_ALIGNED(4) ItemData
{
    u16 Price;
    u8 HeldEffect;
    u8 HeldArgument;
    u8 NaturalGiftEffect;
    u8 FlingEffect;
    u8 FlingPower;
    u8 NaturalGiftPower;
    u16 Packed;
    u8 EffectField;
    u8 EffectBattle;
    u8 HasBattleStats;
    u8 ItemClass;
    u8 Consumable;
    u8 SortIndex;
    ItemBattleStats BattleStats;
};

enum ItemField
{
    ITSTAT_PRICE = 0,
    ITSTAT_USE_EFFECT = 1,
    ITSTAT_USE_PARAM = 2,
    ITSTAT_FLAG1 = 3,
    ITSTAT_FLAG2 = 4,
    ITSTAT_POCKET_FIELD = 5,
    ITSTAT_EFFECT_FIELD = 6,
    ITSTAT_EFFECT_BATTLE = 7,
    ITSTAT_NATURAL_GIFT_EFFECT = 8,
    ITSTAT_FLING_EFFECT = 9,
    ITSTAT_FLING_POWER = 10,
    ITSTAT_NATURAL_GIFT_POWER = 11,
    ITSTAT_NATURAL_GIFT_TYPE = 12,
    ITSTAT_POCKET_BATTLE = 13,
    ITSTAT_HAS_BATTLE_STATS = 14,
    ITSTAT_CLASS = 15,
    ITSTAT_CONSUMABLE = 16,
    ITSTAT_SORT_IDX = 17,
    ITSTAT_AILMENT_SLP = 18,
    ITSTAT_AILMENT_PSN = 19,
    ITSTAT_AILMENT_BRN = 20,
    ITSTAT_AILMENT_FRZ = 21,
    ITSTAT_AILMENT_PAR = 22,
    ITSTAT_AILMENT_CFZ = 23,
    ITSTAT_AILMENT_INF = 24,
    ITSTAT_AILMENT_GSP = 25,
    ITSTAT_BOOST_REVIVE = 26,
    ITSTAT_BOOST_SACRED_ASH = 27,
    ITSTAT_BOOST_RARECANDY = 28,
    ITSTAT_BOOST_EVOSTONE = 29,
    ITSTAT_BOOST_ATK = 30,
    ITSTAT_BOOST_DEF = 31,
    ITSTAT_BOOST_SPA = 32,
    ITSTAT_BOOST_SPD = 33,
    ITSTAT_BOOST_SPE = 34,
    ITSTAT_BOOST_ACC = 35,
    ITSTAT_BOOST_CRIT = 36,
    ITSTAT_BOOST_PP1 = 37,
    ITSTAT_BOOST_PPMAX = 38,
    ITSTAT_FLAG_PPREPLENISH = 39,
    ITSTAT_FLAG_PPREPLENISH_FULL = 40,
    ITSTAT_FLAG_HPREPLENISH = 41,
    ITSTAT_FLAG_EVADD_HP = 42,
    ITSTAT_FLAG_EVADD_ATK = 43,
    ITSTAT_FLAG_EVADD_DEF = 44,
    ITSTAT_FLAG_EVADD_SPE = 45,
    ITSTAT_FLAG_EVADD_SPA = 46,
    ITSTAT_FLAG_EVADD_SPD = 47,
    ITSTAT_FLAG_EVADD_ABOVE100 = 48,
    ITSTAT_FLAG_FRIENDSHIP_ADD1 = 49,
    ITSTAT_FLAG_FRIENDSHIP_ADD2 = 50,
    ITSTAT_FLAG_FRIENDSHIP_ADD3 = 51,
    ITSTAT_EV_HP = 52,
    ITSTAT_EV_ATK = 53,
    ITSTAT_EV_DEF = 54,
    ITSTAT_EV_SPE = 55,
    ITSTAT_EV_SPA = 56,
    ITSTAT_EV_SPD = 57,
    ITSTAT_HEAL_AMOUNT = 58,
    ITSTAT_PP_GAIN = 59,
    ITSTAT_FRIENDSHIP1 = 60,
    ITSTAT_FRIENDSHIP2 = 61,
    ITSTAT_FRIENDSHIP3 = 62,
};

enum MonsNo
{
    PK_NULL = 0x0,
    PK001_BULBASAUR = 0x1,
    PK002_IVYSAUR = 0x2,
    PK003_VENUSAUR = 0x3,
    PK004_CHARMANDER = 0x4,
    PK005_CHARMELEON = 0x5,
    PK006_CHARIZARD = 0x6,
    PK007_SQUIRTLE = 0x7,
    PK008_WARTORTLE = 0x8,
    PK009_BLASTOISE = 0x9,
    PK010_CATERPIE = 0xA,
    PK011_METAPOD = 0xB,
    PK012_BUTTERFREE = 0xC,
    PK013_WEEDLE = 0xD,
    PK014_KAKUNA = 0xE,
    PK015_BEEDRILL = 0xF,
    PK016_PIDGEY = 0x10,
    PK017_PIDGEOTTO = 0x11,
    PK018_PIDGEOT = 0x12,
    PK019_RATTATA = 0x13,
    PK020_RATICATE = 0x14,
    PK021_SPEAROW = 0x15,
    PK022_FEAROW = 0x16,
    PK023_EKANS = 0x17,
    PK024_ARBOK = 0x18,
    PK025_PIKACHU = 0x19,
    PK026_RAICHU = 0x1A,
    PK027_SANDSHREW = 0x1B,
    PK028_SANDSLASH = 0x1C,
    PK029_NIDORAN = 0x1D,
    PK030_NIDORINA = 0x1E,
    PK031_NIDOQUEEN = 0x1F,
    PK032_NIDORAN = 0x20,
    PK033_NIDORINO = 0x21,
    PK034_NIDOKING = 0x22,
    PK035_CLEFAIRY = 0x23,
    PK036_CLEFABLE = 0x24,
    PK037_VULPIX = 0x25,
    PK038_NINETALES = 0x26,
    PK039_JIGGLYPUFF = 0x27,
    PK040_WIGGLYTUFF = 0x28,
    PK041_ZUBAT = 0x29,
    PK042_GOLBAT = 0x2A,
    PK043_ODDISH = 0x2B,
    PK044_GLOOM = 0x2C,
    PK045_VILEPLUME = 0x2D,
    PK046_PARAS = 0x2E,
    PK047_PARASECT = 0x2F,
    PK048_VENONAT = 0x30,
    PK049_VENOMOTH = 0x31,
    PK050_DIGLETT = 0x32,
    PK051_DUGTRIO = 0x33,
    PK052_MEOWTH = 0x34,
    PK053_PERSIAN = 0x35,
    PK054_PSYDUCK = 0x36,
    PK055_GOLDUCK = 0x37,
    PK056_MANKEY = 0x38,
    PK057_PRIMEAPE = 0x39,
    PK058_GROWLITHE = 0x3A,
    PK059_ARCANINE = 0x3B,
    PK060_POLIWAG = 0x3C,
    PK061_POLIWHIRL = 0x3D,
    PK062_POLIWRATH = 0x3E,
    PK063_ABRA = 0x3F,
    PK064_KADABRA = 0x40,
    PK065_ALAKAZAM = 0x41,
    PK066_MACHOP = 0x42,
    PK067_MACHOKE = 0x43,
    PK068_MACHAMP = 0x44,
    PK069_BELLSPROUT = 0x45,
    PK070_WEEPINBELL = 0x46,
    PK071_VICTREEBEL = 0x47,
    PK072_TENTACOOL = 0x48,
    PK073_TENTACRUEL = 0x49,
    PK074_GEODUDE = 0x4A,
    PK075_GRAVELER = 0x4B,
    PK076_GOLEM = 0x4C,
    PK077_PONYTA = 0x4D,
    PK078_RAPIDASH = 0x4E,
    PK079_SLOWPOKE = 0x4F,
    PK080_SLOWBRO = 0x50,
    PK081_MAGNEMITE = 0x51,
    PK082_MAGNETON = 0x52,
    PK083_FARFETCH_D = 0x53,
    PK084_DODUO = 0x54,
    PK085_DODRIO = 0x55,
    PK086_SEEL = 0x56,
    PK087_DEWGONG = 0x57,
    PK088_GRIMER = 0x58,
    PK089_MUK = 0x59,
    PK090_SHELLDER = 0x5A,
    PK091_CLOYSTER = 0x5B,
    PK092_GASTLY = 0x5C,
    PK093_HAUNTER = 0x5D,
    PK094_GENGAR = 0x5E,
    PK095_ONIX = 0x5F,
    PK096_DROWZEE = 0x60,
    PK097_HYPNO = 0x61,
    PK098_KRABBY = 0x62,
    PK099_KINGLER = 0x63,
    PK100_VOLTORB = 0x64,
    PK101_ELECTRODE = 0x65,
    PK102_EXEGGCUTE = 0x66,
    PK103_EXEGGUTOR = 0x67,
    PK104_CUBONE = 0x68,
    PK105_MAROWAK = 0x69,
    PK106_HITMONLEE = 0x6A,
    PK107_HITMONCHAN = 0x6B,
    PK108_LICKITUNG = 0x6C,
    PK109_KOFFING = 0x6D,
    PK110_WEEZING = 0x6E,
    PK111_RHYHORN = 0x6F,
    PK112_RHYDON = 0x70,
    PK113_CHANSEY = 0x71,
    PK114_TANGELA = 0x72,
    PK115_KANGASKHAN = 0x73,
    PK116_HORSEA = 0x74,
    PK117_SEADRA = 0x75,
    PK118_GOLDEEN = 0x76,
    PK119_SEAKING = 0x77,
    PK120_STARYU = 0x78,
    PK121_STARMIE = 0x79,
    PK122_MR_MIME = 0x7A,
    PK123_SCYTHER = 0x7B,
    PK124_JYNX = 0x7C,
    PK125_ELECTABUZZ = 0x7D,
    PK126_MAGMAR = 0x7E,
    PK127_PINSIR = 0x7F,
    PK128_TAUROS = 0x80,
    PK129_MAGIKARP = 0x81,
    PK130_GYARADOS = 0x82,
    PK131_LAPRAS = 0x83,
    PK132_DITTO = 0x84,
    PK133_EEVEE = 0x85,
    PK134_VAPOREON = 0x86,
    PK135_JOLTEON = 0x87,
    PK136_FLAREON = 0x88,
    PK137_PORYGON = 0x89,
    PK138_OMANYTE = 0x8A,
    PK139_OMASTAR = 0x8B,
    PK140_KABUTO = 0x8C,
    PK141_KABUTOPS = 0x8D,
    PK142_AERODACTYL = 0x8E,
    PK143_SNORLAX = 0x8F,
    PK144_ARTICUNO = 0x90,
    PK145_ZAPDOS = 0x91,
    PK146_MOLTRES = 0x92,
    PK147_DRATINI = 0x93,
    PK148_DRAGONAIR = 0x94,
    PK149_DRAGONITE = 0x95,
    PK150_MEWTWO = 0x96,
    PK151_MEW = 0x97,
    PK152_CHIKORITA = 0x98,
    PK153_BAYLEEF = 0x99,
    PK154_MEGANIUM = 0x9A,
    PK155_CYNDAQUIL = 0x9B,
    PK156_QUILAVA = 0x9C,
    PK157_TYPHLOSION = 0x9D,
    PK158_TOTODILE = 0x9E,
    PK159_CROCONAW = 0x9F,
    PK160_FERALIGATR = 0xA0,
    PK161_SENTRET = 0xA1,
    PK162_FURRET = 0xA2,
    PK163_HOOTHOOT = 0xA3,
    PK164_NOCTOWL = 0xA4,
    PK165_LEDYBA = 0xA5,
    PK166_LEDIAN = 0xA6,
    PK167_SPINARAK = 0xA7,
    PK168_ARIADOS = 0xA8,
    PK169_CROBAT = 0xA9,
    PK170_CHINCHOU = 0xAA,
    PK171_LANTURN = 0xAB,
    PK172_PICHU = 0xAC,
    PK173_CLEFFA = 0xAD,
    PK174_IGGLYBUFF = 0xAE,
    PK175_TOGEPI = 0xAF,
    PK176_TOGETIC = 0xB0,
    PK177_NATU = 0xB1,
    PK178_XATU = 0xB2,
    PK179_MAREEP = 0xB3,
    PK180_FLAAFFY = 0xB4,
    PK181_AMPHAROS = 0xB5,
    PK182_BELLOSSOM = 0xB6,
    PK183_MARILL = 0xB7,
    PK184_AZUMARILL = 0xB8,
    PK185_SUDOWOODO = 0xB9,
    PK186_POLITOED = 0xBA,
    PK187_HOPPIP = 0xBB,
    PK188_SKIPLOOM = 0xBC,
    PK189_JUMPLUFF = 0xBD,
    PK190_AIPOM = 0xBE,
    PK191_SUNKERN = 0xBF,
    PK192_SUNFLORA = 0xC0,
    PK193_YANMA = 0xC1,
    PK194_WOOPER = 0xC2,
    PK195_QUAGSIRE = 0xC3,
    PK196_ESPEON = 0xC4,
    PK197_UMBREON = 0xC5,
    PK198_MURKROW = 0xC6,
    PK199_SLOWKING = 0xC7,
    PK200_MISDREAVUS = 0xC8,
    PK201_UNOWN = 0xC9,
    PK202_WOBBUFFET = 0xCA,
    PK203_GIRAFARIG = 0xCB,
    PK204_PINECO = 0xCC,
    PK205_FORRETRESS = 0xCD,
    PK206_DUNSPARCE = 0xCE,
    PK207_GLIGAR = 0xCF,
    PK208_STEELIX = 0xD0,
    PK209_SNUBBULL = 0xD1,
    PK210_GRANBULL = 0xD2,
    PK211_QWILFISH = 0xD3,
    PK212_SCIZOR = 0xD4,
    PK213_SHUCKLE = 0xD5,
    PK214_HERACROSS = 0xD6,
    PK215_SNEASEL = 0xD7,
    PK216_TEDDIURSA = 0xD8,
    PK217_URSARING = 0xD9,
    PK218_SLUGMA = 0xDA,
    PK219_MAGCARGO = 0xDB,
    PK220_SWINUB = 0xDC,
    PK221_PILOSWINE = 0xDD,
    PK222_CORSOLA = 0xDE,
    PK223_REMORAID = 0xDF,
    PK224_OCTILLERY = 0xE0,
    PK225_DELIBIRD = 0xE1,
    PK226_MANTINE = 0xE2,
    PK227_SKARMORY = 0xE3,
    PK228_HOUNDOUR = 0xE4,
    PK229_HOUNDOOM = 0xE5,
    PK230_KINGDRA = 0xE6,
    PK231_PHANPY = 0xE7,
    PK232_DONPHAN = 0xE8,
    PK233_PORYGON2 = 0xE9,
    PK234_STANTLER = 0xEA,
    PK235_SMEARGLE = 0xEB,
    PK236_TYROGUE = 0xEC,
    PK237_HITMONTOP = 0xED,
    PK238_SMOOCHUM = 0xEE,
    PK239_ELEKID = 0xEF,
    PK240_MAGBY = 0xF0,
    PK241_MILTANK = 0xF1,
    PK242_BLISSEY = 0xF2,
    PK243_RAIKOU = 0xF3,
    PK244_ENTEI = 0xF4,
    PK245_SUICUNE = 0xF5,
    PK246_LARVITAR = 0xF6,
    PK247_PUPITAR = 0xF7,
    PK248_TYRANITAR = 0xF8,
    PK249_LUGIA = 0xF9,
    PK250_HO_OH = 0xFA,
    PK251_CELEBI = 0xFB,
    PK252_TREECKO = 0xFC,
    PK253_GROVYLE = 0xFD,
    PK254_SCEPTILE = 0xFE,
    PK255_TORCHIC = 0xFF,
    PK256_COMBUSKEN = 0x100,
    PK257_BLAZIKEN = 0x101,
    PK258_MUDKIP = 0x102,
    PK259_MARSHTOMP = 0x103,
    PK260_SWAMPERT = 0x104,
    PK261_POOCHYENA = 0x105,
    PK262_MIGHTYENA = 0x106,
    PK263_ZIGZAGOON = 0x107,
    PK264_LINOONE = 0x108,
    PK265_WURMPLE = 0x109,
    PK266_SILCOON = 0x10A,
    PK267_BEAUTIFLY = 0x10B,
    PK268_CASCOON = 0x10C,
    PK269_DUSTOX = 0x10D,
    PK270_LOTAD = 0x10E,
    PK271_LOMBRE = 0x10F,
    PK272_LUDICOLO = 0x110,
    PK273_SEEDOT = 0x111,
    PK274_NUZLEAF = 0x112,
    PK275_SHIFTRY = 0x113,
    PK276_TAILLOW = 0x114,
    PK277_SWELLOW = 0x115,
    PK278_WINGULL = 0x116,
    PK279_PELIPPER = 0x117,
    PK280_RALTS = 0x118,
    PK281_KIRLIA = 0x119,
    PK282_GARDEVOIR = 0x11A,
    PK283_SURSKIT = 0x11B,
    PK284_MASQUERAIN = 0x11C,
    PK285_SHROOMISH = 0x11D,
    PK286_BRELOOM = 0x11E,
    PK287_SLAKOTH = 0x11F,
    PK288_VIGOROTH = 0x120,
    PK289_SLAKING = 0x121,
    PK290_NINCADA = 0x122,
    PK291_NINJASK = 0x123,
    PK292_SHEDINJA = 0x124,
    PK293_WHISMUR = 0x125,
    PK294_LOUDRED = 0x126,
    PK295_EXPLOUD = 0x127,
    PK296_MAKUHITA = 0x128,
    PK297_HARIYAMA = 0x129,
    PK298_AZURILL = 0x12A,
    PK299_NOSEPASS = 0x12B,
    PK300_SKITTY = 0x12C,
    PK301_DELCATTY = 0x12D,
    PK302_SABLEYE = 0x12E,
    PK303_MAWILE = 0x12F,
    PK304_ARON = 0x130,
    PK305_LAIRON = 0x131,
    PK306_AGGRON = 0x132,
    PK307_MEDITITE = 0x133,
    PK308_MEDICHAM = 0x134,
    PK309_ELECTRIKE = 0x135,
    PK310_MANECTRIC = 0x136,
    PK311_PLUSLE = 0x137,
    PK312_MINUN = 0x138,
    PK313_VOLBEAT = 0x139,
    PK314_ILLUMISE = 0x13A,
    PK315_ROSELIA = 0x13B,
    PK316_GULPIN = 0x13C,
    PK317_SWALOT = 0x13D,
    PK318_CARVANHA = 0x13E,
    PK319_SHARPEDO = 0x13F,
    PK320_WAILMER = 0x140,
    PK321_WAILORD = 0x141,
    PK322_NUMEL = 0x142,
    PK323_CAMERUPT = 0x143,
    PK324_TORKOAL = 0x144,
    PK325_SPOINK = 0x145,
    PK326_GRUMPIG = 0x146,
    PK327_SPINDA = 0x147,
    PK328_TRAPINCH = 0x148,
    PK329_VIBRAVA = 0x149,
    PK330_FLYGON = 0x14A,
    PK331_CACNEA = 0x14B,
    PK332_CACTURNE = 0x14C,
    PK333_SWABLU = 0x14D,
    PK334_ALTARIA = 0x14E,
    PK335_ZANGOOSE = 0x14F,
    PK336_SEVIPER = 0x150,
    PK337_LUNATONE = 0x151,
    PK338_SOLROCK = 0x152,
    PK339_BARBOACH = 0x153,
    PK340_WHISCASH = 0x154,
    PK341_CORPHISH = 0x155,
    PK342_CRAWDAUNT = 0x156,
    PK343_BALTOY = 0x157,
    PK344_CLAYDOL = 0x158,
    PK345_LILEEP = 0x159,
    PK346_CRADILY = 0x15A,
    PK347_ANORITH = 0x15B,
    PK348_ARMALDO = 0x15C,
    PK349_FEEBAS = 0x15D,
    PK350_MILOTIC = 0x15E,
    PK351_CASTFORM = 0x15F,
    PK352_KECLEON = 0x160,
    PK353_SHUPPET = 0x161,
    PK354_BANETTE = 0x162,
    PK355_DUSKULL = 0x163,
    PK356_DUSCLOPS = 0x164,
    PK357_TROPIUS = 0x165,
    PK358_CHIMECHO = 0x166,
    PK359_ABSOL = 0x167,
    PK360_WYNAUT = 0x168,
    PK361_SNORUNT = 0x169,
    PK362_GLALIE = 0x16A,
    PK363_SPHEAL = 0x16B,
    PK364_SEALEO = 0x16C,
    PK365_WALREIN = 0x16D,
    PK366_CLAMPERL = 0x16E,
    PK367_HUNTAIL = 0x16F,
    PK368_GOREBYSS = 0x170,
    PK369_RELICANTH = 0x171,
    PK370_LUVDISC = 0x172,
    PK371_BAGON = 0x173,
    PK372_SHELGON = 0x174,
    PK373_SALAMENCE = 0x175,
    PK374_BELDUM = 0x176,
    PK375_METANG = 0x177,
    PK376_METAGROSS = 0x178,
    PK377_REGIROCK = 0x179,
    PK378_REGICE = 0x17A,
    PK379_REGISTEEL = 0x17B,
    PK380_LATIAS = 0x17C,
    PK381_LATIOS = 0x17D,
    PK382_KYOGRE = 0x17E,
    PK383_GROUDON = 0x17F,
    PK384_RAYQUAZA = 0x180,
    PK385_JIRACHI = 0x181,
    PK386_DEOXYS = 0x182,
    PK387_TURTWIG = 0x183,
    PK388_GROTLE = 0x184,
    PK389_TORTERRA = 0x185,
    PK390_CHIMCHAR = 0x186,
    PK391_MONFERNO = 0x187,
    PK392_INFERNAPE = 0x188,
    PK393_PIPLUP = 0x189,
    PK394_PRINPLUP = 0x18A,
    PK395_EMPOLEON = 0x18B,
    PK396_STARLY = 0x18C,
    PK397_STARAVIA = 0x18D,
    PK398_STARAPTOR = 0x18E,
    PK399_BIDOOF = 0x18F,
    PK400_BIBAREL = 0x190,
    PK401_KRICKETOT = 0x191,
    PK402_KRICKETUNE = 0x192,
    PK403_SHINX = 0x193,
    PK404_LUXIO = 0x194,
    PK405_LUXRAY = 0x195,
    PK406_BUDEW = 0x196,
    PK407_ROSERADE = 0x197,
    PK408_CRANIDOS = 0x198,
    PK409_RAMPARDOS = 0x199,
    PK410_SHIELDON = 0x19A,
    PK411_BASTIODON = 0x19B,
    PK412_BURMY = 0x19C,
    PK413_WORMADAM = 0x19D,
    PK414_MOTHIM = 0x19E,
    PK415_COMBEE = 0x19F,
    PK416_VESPIQUEN = 0x1A0,
    PK417_PACHIRISU = 0x1A1,
    PK418_BUIZEL = 0x1A2,
    PK419_FLOATZEL = 0x1A3,
    PK420_CHERUBI = 0x1A4,
    PK421_CHERRIM = 0x1A5,
    PK422_SHELLOS = 0x1A6,
    PK423_GASTRODON = 0x1A7,
    PK424_AMBIPOM = 0x1A8,
    PK425_DRIFLOON = 0x1A9,
    PK426_DRIFBLIM = 0x1AA,
    PK427_BUNEARY = 0x1AB,
    PK428_LOPUNNY = 0x1AC,
    PK429_MISMAGIUS = 0x1AD,
    PK430_HONCHKROW = 0x1AE,
    PK431_GLAMEOW = 0x1AF,
    PK432_PURUGLY = 0x1B0,
    PK433_CHINGLING = 0x1B1,
    PK434_STUNKY = 0x1B2,
    PK435_SKUNTANK = 0x1B3,
    PK436_BRONZOR = 0x1B4,
    PK437_BRONZONG = 0x1B5,
    PK438_BONSLY = 0x1B6,
    PK439_MIME_JR = 0x1B7,
    PK440_HAPPINY = 0x1B8,
    PK441_CHATOT = 0x1B9,
    PK442_SPIRITOMB = 0x1BA,
    PK443_GIBLE = 0x1BB,
    PK444_GABITE = 0x1BC,
    PK445_GARCHOMP = 0x1BD,
    PK446_MUNCHLAX = 0x1BE,
    PK447_RIOLU = 0x1BF,
    PK448_LUCARIO = 0x1C0,
    PK449_HIPPOPOTAS = 0x1C1,
    PK450_HIPPOWDON = 0x1C2,
    PK451_SKORUPI = 0x1C3,
    PK452_DRAPION = 0x1C4,
    PK453_CROAGUNK = 0x1C5,
    PK454_TOXICROAK = 0x1C6,
    PK455_CARNIVINE = 0x1C7,
    PK456_FINNEON = 0x1C8,
    PK457_LUMINEON = 0x1C9,
    PK458_MANTYKE = 0x1CA,
    PK459_SNOVER = 0x1CB,
    PK460_ABOMASNOW = 0x1CC,
    PK461_WEAVILE = 0x1CD,
    PK462_MAGNEZONE = 0x1CE,
    PK463_LICKILICKY = 0x1CF,
    PK464_RHYPERIOR = 0x1D0,
    PK465_TANGROWTH = 0x1D1,
    PK466_ELECTIVIRE = 0x1D2,
    PK467_MAGMORTAR = 0x1D3,
    PK468_TOGEKISS = 0x1D4,
    PK469_YANMEGA = 0x1D5,
    PK470_LEAFEON = 0x1D6,
    PK471_GLACEON = 0x1D7,
    PK472_GLISCOR = 0x1D8,
    PK473_MAMOSWINE = 0x1D9,
    PK474_PORYGON_Z = 0x1DA,
    PK475_GALLADE = 0x1DB,
    PK476_PROBOPASS = 0x1DC,
    PK477_DUSKNOIR = 0x1DD,
    PK478_FROSLASS = 0x1DE,
    PK479_ROTOM = 0x1DF,
    PK480_UXIE = 0x1E0,
    PK481_MESPRIT = 0x1E1,
    PK482_AZELF = 0x1E2,
    PK483_DIALGA = 0x1E3,
    PK484_PALKIA = 0x1E4,
    PK485_HEATRAN = 0x1E5,
    PK486_REGIGIGAS = 0x1E6,
    PK487_GIRATINA = 0x1E7,
    PK488_CRESSELIA = 0x1E8,
    PK489_PHIONE = 0x1E9,
    PK490_MANAPHY = 0x1EA,
    PK491_DARKRAI = 0x1EB,
    PK492_SHAYMIN = 0x1EC,
    PK493_ARCEUS = 0x1ED,
    PK494_VICTINI = 0x1EE,
    PK495_SNIVY = 0x1EF,
    PK496_SERVINE = 0x1F0,
    PK497_SERPERIOR = 0x1F1,
    PK498_TEPIG = 0x1F2,
    PK499_PIGNITE = 0x1F3,
    PK500_EMBOAR = 0x1F4,
    PK501_OSHAWOTT = 0x1F5,
    PK502_DEWOTT = 0x1F6,
    PK503_SAMUROTT = 0x1F7,
    PK504_PATRAT = 0x1F8,
    PK505_WATCHOG = 0x1F9,
    PK506_LILLIPUP = 0x1FA,
    PK507_HERDIER = 0x1FB,
    PK508_STOUTLAND = 0x1FC,
    PK509_PURRLOIN = 0x1FD,
    PK510_LIEPARD = 0x1FE,
    PK511_PANSAGE = 0x1FF,
    PK512_SIMISAGE = 0x200,
    PK513_PANSEAR = 0x201,
    PK514_SIMISEAR = 0x202,
    PK515_PANPOUR = 0x203,
    PK516_SIMIPOUR = 0x204,
    PK517_MUNNA = 0x205,
    PK518_MUSHARNA = 0x206,
    PK519_PIDOVE = 0x207,
    PK520_TRANQUILL = 0x208,
    PK521_UNFEZANT = 0x209,
    PK522_BLITZLE = 0x20A,
    PK523_ZEBSTRIKA = 0x20B,
    PK524_ROGGENROLA = 0x20C,
    PK525_BOLDORE = 0x20D,
    PK526_GIGALITH = 0x20E,
    PK527_WOOBAT = 0x20F,
    PK528_SWOOBAT = 0x210,
    PK529_DRILBUR = 0x211,
    PK530_EXCADRILL = 0x212,
    PK531_AUDINO = 0x213,
    PK532_TIMBURR = 0x214,
    PK533_GURDURR = 0x215,
    PK534_CONKELDURR = 0x216,
    PK535_TYMPOLE = 0x217,
    PK536_PALPITOAD = 0x218,
    PK537_SEISMITOAD = 0x219,
    PK538_THROH = 0x21A,
    PK539_SAWK = 0x21B,
    PK540_SEWADDLE = 0x21C,
    PK541_SWADLOON = 0x21D,
    PK542_LEAVANNY = 0x21E,
    PK543_VENIPEDE = 0x21F,
    PK544_WHIRLIPEDE = 0x220,
    PK545_SCOLIPEDE = 0x221,
    PK546_COTTONEE = 0x222,
    PK547_WHIMSICOTT = 0x223,
    PK548_PETILIL = 0x224,
    PK549_LILLIGANT = 0x225,
    PK550_BASCULIN = 0x226,
    PK551_SANDILE = 0x227,
    PK552_KROKOROK = 0x228,
    PK553_KROOKODILE = 0x229,
    PK554_DARUMAKA = 0x22A,
    PK555_DARMANITAN = 0x22B,
    PK556_MARACTUS = 0x22C,
    PK557_DWEBBLE = 0x22D,
    PK558_CRUSTLE = 0x22E,
    PK559_SCRAGGY = 0x22F,
    PK560_SCRAFTY = 0x230,
    PK561_SIGILYPH = 0x231,
    PK562_YAMASK = 0x232,
    PK563_COFAGRIGUS = 0x233,
    PK564_TIRTOUGA = 0x234,
    PK565_CARRACOSTA = 0x235,
    PK566_ARCHEN = 0x236,
    PK567_ARCHEOPS = 0x237,
    PK568_TRUBBISH = 0x238,
    PK569_GARBODOR = 0x239,
    PK570_ZORUA = 0x23A,
    PK571_ZOROARK = 0x23B,
    PK572_MINCCINO = 0x23C,
    PK573_CINCCINO = 0x23D,
    PK574_GOTHITA = 0x23E,
    PK575_GOTHORITA = 0x23F,
    PK576_GOTHITELLE = 0x240,
    PK577_SOLOSIS = 0x241,
    PK578_DUOSION = 0x242,
    PK579_REUNICLUS = 0x243,
    PK580_DUCKLETT = 0x244,
    PK581_SWANNA = 0x245,
    PK582_VANILLITE = 0x246,
    PK583_VANILLISH = 0x247,
    PK584_VANILLUXE = 0x248,
    PK585_DEERLING = 0x249,
    PK586_SAWSBUCK = 0x24A,
    PK587_EMOLGA = 0x24B,
    PK588_KARRABLAST = 0x24C,
    PK589_ESCAVALIER = 0x24D,
    PK590_FOONGUS = 0x24E,
    PK591_AMOONGUSS = 0x24F,
    PK592_FRILLISH = 0x250,
    PK593_JELLICENT = 0x251,
    PK594_ALOMOMOLA = 0x252,
    PK595_JOLTIK = 0x253,
    PK596_GALVANTULA = 0x254,
    PK597_FERROSEED = 0x255,
    PK598_FERROTHORN = 0x256,
    PK599_KLINK = 0x257,
    PK600_KLANG = 0x258,
    PK601_KLINKLANG = 0x259,
    PK602_TYNAMO = 0x25A,
    PK603_EELEKTRIK = 0x25B,
    PK604_EELEKTROSS = 0x25C,
    PK605_ELGYEM = 0x25D,
    PK606_BEHEEYEM = 0x25E,
    PK607_LITWICK = 0x25F,
    PK608_LAMPENT = 0x260,
    PK609_CHANDELURE = 0x261,
    PK610_AXEW = 0x262,
    PK611_FRAXURE = 0x263,
    PK612_HAXORUS = 0x264,
    PK613_CUBCHOO = 0x265,
    PK614_BEARTIC = 0x266,
    PK615_CRYOGONAL = 0x267,
    PK616_SHELMET = 0x268,
    PK617_ACCELGOR = 0x269,
    PK618_STUNFISK = 0x26A,
    PK619_MIENFOO = 0x26B,
    PK620_MIENSHAO = 0x26C,
    PK621_DRUDDIGON = 0x26D,
    PK622_GOLETT = 0x26E,
    PK623_GOLURK = 0x26F,
    PK624_PAWNIARD = 0x270,
    PK625_BISHARP = 0x271,
    PK626_BOUFFALANT = 0x272,
    PK627_RUFFLET = 0x273,
    PK628_BRAVIARY = 0x274,
    PK629_VULLABY = 0x275,
    PK630_MANDIBUZZ = 0x276,
    PK631_HEATMOR = 0x277,
    PK632_DURANT = 0x278,
    PK633_DEINO = 0x279,
    PK634_ZWEILOUS = 0x27A,
    PK635_HYDREIGON = 0x27B,
    PK636_LARVESTA = 0x27C,
    PK637_VOLCARONA = 0x27D,
    PK638_COBALION = 0x27E,
    PK639_TERRAKION = 0x27F,
    PK640_VIRIZION = 0x280,
    PK641_TORNADUS = 0x281,
    PK642_THUNDURUS = 0x282,
    PK643_RESHIRAM = 0x283,
    PK644_ZEKROM = 0x284,
    PK645_LANDORUS = 0x285,
    PK646_KYUREM = 0x286,
    PK647_KELDEO = 0x287,
    PK648_MELOETTA = 0x288,
    PK649_GENESECT = 0x289,
    MONSNO_MAX = 0x28A,
};

struct SWAN_ALIGNED(4) HandlerParam_ConsumeItem
{
    HandlerParam_Header header;
    int fNoAction;
    HandlerParam_StrParams exStr;
};

struct SWAN_ALIGNED(4) HandlerParam_Switch
{
    HandlerParam_Header header;
    HandlerParam_StrParams preStr;
    HandlerParam_StrParams exStr;
    u8 pokeID;
    char fIntrDisable;
};

struct HandlerParam_SwapPoke
{
    HandlerParam_Header header;
    u8 pokeID1;
    u8 pokeID2;
    HandlerParam_StrParams exStr;
};

struct HandlerParam_Transform
{
    HandlerParam_Header header;
    u8 pokeID;
    HandlerParam_StrParams exStr;
};

struct HandlerParam_IllusionBreak
{
    HandlerParam_Header header;
    u8 pokeID;
    HandlerParam_StrParams exStr;
};

struct HandlerParam_AddAnimation
{
    HandlerParam_Header header;
    u16 animNo;
    u8 pos_from;
    u8 pos_to;
    u16 reservedQuePos;
    u8 fQueReserve;
    u8 fMsgWinVanish;
    HandlerParam_StrParams exStr;
};

struct HandlerParam_ChangeForm
{
    HandlerParam_Header header;
    u8 pokeID;
    u8 formNo;
    u8 fDontResetOnSwitch;
    HandlerParam_StrParams exStr;
};

struct HandlerParam_InterruptPoke
{
    HandlerParam_Header header;
    u8 pokeID;
    HandlerParam_StrParams exStr;
};

struct ShortcutSave
{
    u8 KeyItemIDs[34];
    __int16 field_22;
};

struct PokeList_Party
{
    PokeParty* pokeParty;
    BagSaveData* bagSaveData;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    ShortcutSave* shortcutSave;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    __int16 field_40;
    __int16 field_42;
    int itemUseType;
    int field_48;
    int SelectedSlot;
    int field_50;
    __int16 ItemID;
    __int16 MoveToLearn;
    char field_58;
    char field_59;
    char field_5A;
    char field_5B;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    __int16 field_6C;
    char field_6E;
    char field_6F;
    char field_70;
    char field_71;
    char field_72;
    char field_73;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    char field_A4;
    char field_A5;
    char field_A6;
    char field_A7;
};

struct PokeList_Plate
{
    PartyPkm* partyPkm;
    char Slot;
    char field_5;
    __int16 CurrentHP;
    __int16 CurrentHP2;
    __int16 field_A;
    int IsEgg;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    GFLBitmap* Bitmap;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    char field_44;
    char field_45;
    char field_46;
    char field_47;
};

struct WordSetBuffer
{
    char field_0;
    char field_1;
    char field_2;
    char field_3;
    char Skip84Pixels;
    char Pad1;
    u16 Pad2;
    StrBuf* Strbuf;
};

struct WordSetSystem
{
    int WordSetCount;
    int HeapID;
    WordSetBuffer* Buffers;
    StrBuf* MainStrbuf;
    int field_10;
};

struct PokeList_Message
{
    int field_0;
    BmpWin* bmpWin;
    int field_8;
    int field_C;
    StrBuf* strBuf;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    WordSetSystem* wordSetSystem;
    int field_30;
    int field_34;
};

struct PokeList_Menu
{
    u32 array_0[28];
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
};

struct PokeList
{
    HeapID heapID;
    __int16 field_2;
    int field_4;
    int field_8;
    char field_C;
    char field_D;
    char field_E;
    char field_F;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int SelectedSlot;
    int TargetedSlot;
    int field_38;
    PartyPkm* SelectedPkm;
    u32 field_40;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
    int field_C4;
    int field_C8;
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    char field_104;
    char field_105;
    __int16 field_106;
    char field_108;
    char field_109;
    char field_10A;
    char field_10B;
    int field_10C;
    int field_110;
    __int16 field_114;
    __int16 field_116;
    char field_118;
    char field_119;
    char field_11A;
    char field_11B;
    int field_11C;
    __int16 CurrentHP;
    char field_122;
    char field_123;
    int field_124;
    u16 field_128[6];
    BmpWin* BmpWin;
    MsgData* MsgData;
    GFLFont* Font1;
    GFLFont* Font2;
    int PrintSys;
    int field_148;
    int field_14C;
    int field_150;
    PokeList_Message* pokeList_Message;
    PokeList_Menu* pokeList_Menu;
    int field_15C;
    int field_160;
    PokeList_Plate* pokeList_Plate[6];
    int field_17C[23];
    int field_1D8;
    int field_1DC;
    int field_1E0;
    int field_1E4;
    int field_1E8;
    int field_1EC;
    int field_1F0;
    int field_1F4;
    int field_1F8;
    int field_1FC;
    int field_200;
    int field_204;
    int field_208;
    int field_20C;
    int field_210;
    char field_214;
    char field_215;
    char field_216;
    char field_217;
    int field_218;
    int field_21C;
    int field_220;
    int field_224;
    int field_228;
    int field_22C;
    int field_230;
    __int16 field_234;
    __int16 field_236;
    int field_238;
    int field_23C;
    int field_240;
    int field_244;
    int field_248;
    int field_24C;
    int field_250;
    __int16 field_254;
    __int16 field_256;
    int field_258;
    int field_25C;
    G3DCamera* G3DCamera;
    int field_264[2];
    int field_26C[2];
    int field_274;
    char field_278;
    char field_279;
    char field_27A;
    char field_27B;
    __int16 field_27C;
    __int16 field_27E;
    int field_280;
    int field_284;
    int field_288;
    PokeList_Party* pokeListParty;
};


enum ItemRestoreType
{
    RESTORETYPE_X_ITEM = 0,
    RESTORETYPE_SACRED_ASH = 1,
    RESTORETYPE_RARE_CANDY = 2,
    RESTORETYPE_SLEEP = 3,
    RESTORETYPE_POISON = 4,
    RESTORETYPE_BURN = 5,
    RESTORETYPE_FREEZE = 6,
    RESTORETYPE_PARALYSIS = 7,
    RESTORETYPE_CONFUSE = 8,
    RESTORETYPE_FULL_HEAL = 9,
    RESTORETYPE_A = 10,
    RESTORETYPE_FULL_RESTORE = 11,
    RESTORETYPE_REVIVE = 12,
    RESTORETYPE_EV_HP = 13,
    RESTORETYPE_EV_ATK = 14,
    RESTORETYPE_EV_DEF = 15,
    RESTORETYPE_EV_SPE = 16,
    RESTORETYPE_EV_SPA = 17,
    RESTORETYPE_EV_SPD = 18,
    RESTORETYPE_EV_HP_DECREASE = 19,
    RESTORETYPE_EV_ATK_DECREASE = 20,
    RESTORETYPE_EV_DEF_DECREASE = 21,
    RESTORETYPE_EV_SPE_DECREASE = 22,
    RESTORETYPE_EV_SPA_DECREASE = 23,
    RESTORETYPE_EV_SPD_DECREASE = 24,
    RESTORETYPE_EVO_STONE = 25,
    RESTORETYPE_BOOST_PP_1 = 26,
    RESTORETYPE_BOOST_PP_MAX = 27,
    RESTORETYPE_PP_REPLENISH = 28,
    RESTORETYPE_NULL = 29,
};

enum SoundResID
{
    SEQ_NULL = 0x0,
    SEQ_PV001 = 0x1,
    SEQ_PV = 0x2,
    SEQ_PV_END = 0x3,
    SEQ_BGM_DUMMY = 0x3E8,
    SEQ_BGM_SILENCE_FIELD = 0x3E9,
    SEQ_BGM_SILENCE_DUNGEON = 0x3EA,
    SEQ_BGM_TITLE = 0x3EB,
    SEQ_BGM_TITLE01 = 0x3EC,
    SEQ_BGM_OPENING_TITLE_B = 0x3ED,
    SEQ_BGM_OPENING_TITLE_W = 0x3EE,
    SEQ_BGM_POKEMON_THEME = 0x3EF,
    SEQ_BGM_STARTING = 0x3F0,
    SEQ_BGM_STARTING2 = 0x3F1,
    SEQ_BGM_SHINKA = 0x3F2,
    SEQ_BGM_KOUKAN = 0x3F3,
    SEQ_BGM_BICYCLE = 0x3F4,
    SEQ_BGM_NAMINORI = 0x3F5,
    SEQ_BGM_DIVING = 0x3F6,
    SEQ_BGM_E_DENDOUIRI = 0x3F7,
    SEQ_BGM_ENDING = 0x3F8,
    SEQ_BGM_ENDING2 = 0x3F9,
    SEQ_BGM_T_01 = 0x3FA,
    SEQ_BGM_T_02 = 0x3FB,
    SEQ_BGM_T_03 = 0x3FC,
    SEQ_BGM_T_04 = 0x3FD,
    SEQ_BGM_T_05_SP = 0x3FE,
    SEQ_BGM_T_05_SU = 0x3FF,
    SEQ_BGM_T_05_AU = 0x400,
    SEQ_BGM_T_05_WI = 0x401,
    SEQ_BGM_C_01 = 0x402,
    SEQ_BGM_C_02 = 0x403,
    SEQ_BGM_C_03 = 0x404,
    SEQ_BGM_C_04 = 0x405,
    SEQ_BGM_SW_C_5_NEW = 0x406,
    SEQ_BGM_C_06 = 0x407,
    SEQ_BGM_C_07 = 0x408,
    SEQ_BGM_C_08_B = 0x409,
    SEQ_BGM_C_08_W = 0x40A,
    SEQ_BGM_C_10_B = 0x40B,
    SEQ_BGM_C_10_W = 0x40C,
    SEQ_BGM_R_A = 0x40D,
    SEQ_BGM_R_B_SP = 0x40E,
    SEQ_BGM_R_B_SU = 0x40F,
    SEQ_BGM_R_B_AU = 0x410,
    SEQ_BGM_R_B_WI = 0x411,
    SEQ_BGM_R_C_SP = 0x412,
    SEQ_BGM_R_C_SU = 0x413,
    SEQ_BGM_R_C_AU = 0x414,
    SEQ_BGM_R_C_WI = 0x415,
    SEQ_BGM_R_D_SP = 0x416,
    SEQ_BGM_R_D_SU = 0x417,
    SEQ_BGM_R_D_AU = 0x418,
    SEQ_BGM_R_D_WI = 0x419,
    SEQ_BGM_R_E_SP = 0x41A,
    SEQ_BGM_R_E_SU = 0x41B,
    SEQ_BGM_R_E_AU = 0x41C,
    SEQ_BGM_R_E_WI = 0x41D,
    SEQ_BGM_R_F = 0x41E,
    SEQ_BGM_H_01 = 0x41F,
    SEQ_BGM_H_02 = 0x420,
    SEQ_BGM_H_03 = 0x421,
    SEQ_BGM_H_04 = 0x422,
    SEQ_BGM_H_05 = 0x423,
    SEQ_BGM_POKECEN = 0x424,
    SEQ_BGM_FS = 0x425,
    SEQ_BGM_GYM = 0x426,
    SEQ_BGM_GROUND_GYM_01 = 0x427,
    SEQ_BGM_LABO = 0x428,
    SEQ_BGM_GATE = 0x429,
    SEQ_BGM_GEAR_STATION = 0x42A,
    SEQ_BGM_BATTLE_SUBWAY = 0x42B,
    SEQ_BGM_PALPARK = 0x42C,
    SEQ_BGM_PLEASURE_BOAT = 0x42D,
    SEQ_BGM_PALACE = 0x42E,
    SEQ_BGM_UNITED_NATIONS = 0x42F,
    SEQ_BGM_POKEMON_LEAGUE = 0x430,
    SEQ_BGM_POKEMON_LEAGUE2 = 0x431,
    SEQ_BGM_SW_D_N_CASTLE = 0x432,
    SEQ_BGM_D_01 = 0x433,
    SEQ_BGM_D_02 = 0x434,
    SEQ_BGM_D_03 = 0x435,
    SEQ_BGM_D_04 = 0x436,
    SEQ_BGM_D_05 = 0x437,
    SEQ_BGM_D_06 = 0x438,
    SEQ_BGM_D_CHAMPROAD = 0x439,
    SEQ_BGM_MUGEN_lobby_B = 0x43A,
    SEQ_BGM_MUGEN_lobby_W = 0x43B,
    SEQ_BGM_MUGEN_D_B = 0x43C,
    SEQ_BGM_MUGEN_D_W = 0x43D,
    SEQ_BGM_E_TSURETEKE2 = 0x43E,
    SEQ_BGM_E_CHEREN = 0x43F,
    SEQ_BGM_E_BERU = 0x440,
    SEQ_BGM_E_DOCTOR = 0x441,
    SEQ_BGM_E_DOCTOR2 = 0x442,
    SEQ_BGM_E_N_SWAN = 0x443,
    SEQ_BGM_E_SAGE = 0x444,
    SEQ_BGM_E_NEW_G_CIS = 0x445,
    SEQ_BGM_E_INISHIE = 0x446,
    SEQ_BGM_E_EMOTION = 0x447,
    SEQ_BGM_ERECTRIC_GYM_01 = 0x448,
    SEQ_BGM_ERECTRIC_GYM_02 = 0x449,
    SEQ_BGM_INSECT_GYM_01 = 0x44A,
    SEQ_BGM_POISONE_GYM_00 = 0x44B,
    SEQ_BGM_POISONE_GYM_01 = 0x44C,
    SEQ_BGM_E_CHAMPION = 0x44D,
    SEQ_BGM_SW_DEMO_OPENING = 0x44E,
    SEQ_BGM_E_KANRANSYA = 0x44F,
    SEQ_BGM_SW_C_12 = 0x450,
    SEQ_BGM_SW_C_13 = 0x451,
    SEQ_BGM_SW_C_14 = 0x452,
    SEQ_BGM_MSL_FIELD = 0x453,
    SEQ_BGM_MSL_DRESSUP = 0x454,
    SEQ_BGM_MSL_BEFORE = 0x455,
    SEQ_BGM_MSL_01 = 0x456,
    SEQ_BGM_MSL_02 = 0x457,
    SEQ_BGM_MSL_03 = 0x458,
    SEQ_BGM_MSL_04 = 0x459,
    SEQ_BGM_EYE_01 = 0x45A,
    SEQ_BGM_EYE_02 = 0x45B,
    SEQ_BGM_EYE_03 = 0x45C,
    SEQ_BGM_EYE_04 = 0x45D,
    SEQ_BGM_EYE_05 = 0x45E,
    SEQ_BGM_EYE_06 = 0x45F,
    SEQ_BGM_EYE_07 = 0x460,
    SEQ_BGM_EYE_08 = 0x461,
    SEQ_BGM_EYE_09 = 0x462,
    SEQ_BGM_EYE_10 = 0x463,
    SEQ_BGM_EYE_11 = 0x464,
    SEQ_BGM_EYE_12 = 0x465,
    SEQ_BGM_EYE_13 = 0x466,
    SEQ_BGM_EYE_NEO_PLASMA = 0x467,
    SEQ_BGM_VS_NORAPOKE = 0x468,
    SEQ_BGM_VS_TSUYOPOKE = 0x469,
    SEQ_BGM_VS_TRAINER = 0x46A,
    SEQ_BGM_VS_SUBWAY_TRAINER = 0x46B,
    SEQ_BGM_VS_GYMLEADER = 0x46C,
    SEQ_BGM_VS_RIVAL = 0x46D,
    SEQ_BGM_VS_DPLEGEND = 0x46E,
    SEQ_BGM_VS_SHITENNO = 0x46F,
    SEQ_BGM_VS_CHAMP = 0x470,
    SEQ_BGM_VS_SWAN_N = 0x471,
    SEQ_BGM_VS_UMA = 0x472,
    SEQ_BGM_VS_REGI = 0x473,
    SEQ_BGM_VS_SHIN = 0x474,
    SEQ_BGM_VS_MU = 0x475,
    SEQ_BGM_VS_RAI = 0x476,
    SEQ_BGM_VS_MOVEPOKE = 0x477,
    SEQ_BGM_VS_SETPOKE = 0x478,
    SEQ_BGM_VS_SHIRONA = 0x479,
    SEQ_BGM_BATTLEPINCH = 0x47A,
    SEQ_BGM_BATTLESUPERIOR = 0x47B,
    SEQ_BGM_WIN1 = 0x47C,
    SEQ_BGM_WIN2 = 0x47D,
    SEQ_BGM_WIN3 = 0x47E,
    SEQ_BGM_WIN4 = 0x47F,
    SEQ_BGM_WIN5 = 0x480,
    SEQ_BGM_WIFI_ACCESS = 0x481,
    SEQ_BGM_WIFI_PRESENT = 0x482,
    SEQ_BGM_GAME_SYNC = 0x483,
    SEQ_BGM_GLTERMINAL = 0x484,
    SEQ_BGM_GURUGURU = 0x485,
    SEQ_BGM_GTS = 0x486,
    SEQ_BGM_WCS = 0x487,
    SEQ_BGM_E_SHIRONA = 0x488,
    SEQ_BGM_VS_TRAINER_M = 0x489,
    SEQ_BGM_VS_TRAINER_S = 0x48A,
    SEQ_BGM_VS_WCS = 0x48B,
    SEQ_BGM_VS_TRAINER_WIFI = 0x48C,
    SEQ_BGM_WIN6 = 0x48D,
    SEQ_BGM_E_LEGEND_BORN_B = 0x48E,
    SEQ_BGM_E_LEGEND_BORN_W = 0x48F,
    SEQ_BGM_SW_D_N_ROOM = 0x490,
    SEQ_BGM_E_EPIROGUE = 0x491,
    SEQ_BGM_E_EPIROGUE_W = 0x492,
    SEQ_BGM_GF_LOGO = 0x493,
    SEQ_BGM_MSL_DL_01 = 0x494,
    SEQ_BGM_MSL_DL_02 = 0x495,
    SEQ_BGM_MSL_DL_03 = 0x496,
    SEQ_BGM_MSL_DL_04 = 0x497,
    SEQ_BGM_SW_R_19_SP = 0x498,
    SEQ_BGM_SW_R_19_SU = 0x499,
    SEQ_BGM_SW_R_19_AU = 0x49A,
    SEQ_BGM_SW_R_19_WI = 0x49B,
    SEQ_BGM_SW_R_22_SP = 0x49C,
    SEQ_BGM_SW_R_22_SU = 0x49D,
    SEQ_BGM_SW_R_22_AU = 0x49E,
    SEQ_BGM_SW_R_22_WI = 0x49F,
    SEQ_BGM_SW_R_23 = 0x4A0,
    SEQ_BGM_SW_KAZAN = 0x4A1,
    SEQ_BGM_SW_T_06 = 0x4A2,
    SEQ_BGM_SW_T_07 = 0x4A3,
    SEQ_BGM_SW_D_22 = 0x4A4,
    SEQ_BGM_SW_D_23 = 0x4A5,
    SEQ_BGM_SW_D_GESUI = 0x4A6,
    SEQ_BGM_SW_D_24_W = 0x4A7,
    SEQ_BGM_SW_D_24_B = 0x4A8,
    SEQ_BGM_SW_D_25 = 0x4A9,
    SEQ_BGM_SW_D_27_F_AJITO = 0x4AA,
    SEQ_BGM_SW_D_27_L_AJITO = 0x4AB,
    SEQ_BGM_SW_D_27_G_AJITO = 0x4AC,
    SEQ_BGM_SW_D_REGI = 0x4AD,
    SEQ_BGM_SW_D_UMA = 0x4AE,
    SEQ_BGM_SW_H_06 = 0x4AF,
    SEQ_BGM_WBT_LOBBY = 0x4B0,
    SEQ_BGM_WBT_GROUND = 0x4B1,
    SEQ_BGM_WBT_FANFARE = 0x4B2,
    SEQ_BGM_PW_LAND = 0x4B3,
    SEQ_BGM_PW_FILMING = 0x4B4,
    SEQ_BGM_PW_SCENARIO_01_F = 0x4B5,
    SEQ_BGM_PW_SCENARIO_01_L = 0x4B6,
    SEQ_BGM_PW_SCENARIO_02_F = 0x4B7,
    SEQ_BGM_PW_SCENARIO_02_L = 0x4B8,
    SEQ_BGM_PW_SCENARIO_03_F = 0x4B9,
    SEQ_BGM_PW_SCENARIO_03_L = 0x4BA,
    SEQ_BGM_PW_SCENARIO_04_F = 0x4BB,
    SEQ_BGM_PW_SCENARIO_04_L = 0x4BC,
    SEQ_BGM_PW_SCENARIO_05_F = 0x4BD,
    SEQ_BGM_PW_SCENARIO_05_L = 0x4BE,
    SEQ_BGM_PW_SCENARIO_06_F = 0x4BF,
    SEQ_BGM_PW_SCENARIO_06_L = 0x4C0,
    SEQ_BGM_PW_SCENARIO_07_F = 0x4C1,
    SEQ_BGM_PW_SCENARIO_07_L = 0x4C2,
    SEQ_BGM_PW_SCENARIO_08_F = 0x4C3,
    SEQ_BGM_FLIGHT_GYM_01 = 0x4C4,
    SEQ_BGM_WATER_GYM_01 = 0x4C5,
    SEQ_BGM_DRAGON_GYM_01 = 0x4C6,
    SEQ_BGM_LCG_01 = 0x4C7,
    SEQ_BGM_LCG_02 = 0x4C8,
    SEQ_BGM_DATA_CONV = 0x4C9,
    SEQ_BGM_REZO_RIZO = 0x4CA,
    SEQ_BGM_FES = 0x4CB,
    SEQ_BGM_SW_DEMO_SHIP = 0x4CC,
    SEQ_BGM_SW_DEMO_FLIGHT = 0x4CD,
    SEQ_BGM_SW_DEMO_UNION_00 = 0x4CE,
    SEQ_BGM_SW_DEMO_UNION_01 = 0x4CF,
    SEQ_BGM_SW_DEMO_UNION_02 = 0x4D0,
    SEQ_BGM_SW_DEMO_UNION_03B = 0x4D1,
    SEQ_BGM_SW_DEMO_UNION_03W = 0x4D2,
    SEQ_BGM_SW_DEMO_AJITO_SHIP = 0x4D3,
    SEQ_BGM_SW_DEMO_FRIGATE = 0x4D4,
    SEQ_BGM_E_HUE = 0x4D5,
    SEQ_BGM_E_ACHROMA = 0x4D6,
    SEQ_BGM_DARK_TRINITY = 0x4D7,
    SEQ_BGM_E_7_SAGE = 0x4D8,
    SEQ_BGM_E_C08_ICE = 0x4D9,
    SEQ_BGM_E_EPILOGUE = 0x4DA,
    SEQ_BGM_EYE_DANCER = 0x4DB,
    SEQ_BGM_EYE_CLOWN = 0x4DC,
    SEQ_BGM_VS_RG_LEADER = 0x4DD,
    SEQ_BGM_VS_RG_CHAMP = 0x4DE,
    SEQ_BGM_VS_GS_LEADER = 0x4DF,
    SEQ_BGM_VS_GS_CHAMP = 0x4E0,
    SEQ_BGM_VS_RS_LEADER = 0x4E1,
    SEQ_BGM_VS_RS_CHAMP = 0x4E2,
    SEQ_BGM_VS_DP_LEADER = 0x4E3,
    SEQ_BGM_VS_DP_CHAMP = 0x4E4,
    SEQ_BGM_VS_BW_CHAMP = 0x4E5,
    SEQ_BGM_VS_WBT = 0x4E6,
    SEQ_BGM_VS_KYURAMU = 0x4E7,
    SEQ_BGM_VS_KYUROMU = 0x4E8,
    SEQ_BGM_VS_NEO_PLASMA = 0x4E9,
    SEQ_BGM_VS_ACHROMA = 0x4EA,
    SEQ_BGM_VS_NEW_G_CIS = 0x4EB,
    SEQ_BGM_VS_IRIS = 0x4EC,
    SEQ_BGM_VS_BANJIROU = 0x4ED,
    SEQ_BGM_VS_HUE = 0x4EE,
    SEQ_BGM_WIN_WBT = 0x4EF,
    SEQ_BGM_KEMONOMICHI = 0x4F0,
    SEQ_BGM_JAPARADE = 0x4F1,
    SEQ_BGM_E_PLASMA = 0x4F2,
    SEQ_BGM_VS_ELITE_PLASMA = 0x4F3,
    SEQ_BGM_THEEND = 0x4F4,
    SEQ_BGM_EV_GIANTHOLE_01 = 0x4F5,
    SEQ_BGM_EV_GIANTHOLE_02 = 0x4F6,
    SEQ_BGM_EV_GIANTHOLE_03 = 0x4F7,
    SEQ_BGM_END = 0x4F8,
    SEQ_ME_ASA = 0x514,
    SEQ_ME_LVUP = 0x515,
    SEQ_ME_ITEM = 0x516,
    SEQ_ME_KEYITEM = 0x517,
    SEQ_ME_POKEGET = 0x518,
    SEQ_ME_SHINKAOME = 0x519,
    SEQ_ME_BADGE = 0x51A,
    SEQ_ME_WAZA = 0x51B,
    SEQ_ME_ACCE = 0x51C,
    SEQ_ME_WASURE = 0x51D,
    SEQ_ME_KINOMI = 0x51E,
    SEQ_ME_HYOUKA1 = 0x51F,
    SEQ_ME_HYOUKA2 = 0x520,
    SEQ_ME_HYOUKA3 = 0x521,
    SEQ_ME_HYOUKA4 = 0x522,
    SEQ_ME_HYOUKA5 = 0x523,
    SEQ_ME_HYOUKA6 = 0x524,
    SEQ_ME_TAMAGO_GET = 0x525,
    SEQ_ME_BPGET = 0x526,
    SEQ_ME_AISHOU_H = 0x527,
    SEQ_ME_AISHOU_M = 0x528,
    SEQ_ME_AISHOU_L = 0x529,
    SEQ_ME_AISHOU_KIDOU = 0x52A,
    SEQ_ME_MISSION_START = 0x52B,
    SEQ_ME_MISSION_CLEAR = 0x52C,
    SEQ_ME_MISSION_FAILED = 0x52D,
    SEQ_ME_ITEM_MYSTERY = 0x52E,
    SEQ_ME_CALL = 0x52F,
    SEQ_ME_QUIZ = 0x530,
    SEQ_ME_DEL_POWER = 0x531,
    SEQ_ME_02 = 0x532,
    SEQ_ME_LCG_01 = 0x533,
    SEQ_ME_LCG_02 = 0x534,
    SEQ_ME_LCG_03 = 0x535,
    SEQ_ME_LCG_04 = 0x536,
    SEQ_ME_MD_FAN01 = 0x537,
    SEQ_ME_MD_FAN03 = 0x538,
    SEQ_ME_MD_FAN04 = 0x539,
    SEQ_ME_KEY_OPEN = 0x53A,
    SEQ_ME_OTAMARO = 0x53B,
    SEQ_ME_AVENUE_01 = 0x53C,
    SEQ_ME_AVENUE_02 = 0x53D,
    SEQ_ME_AVENUE_03 = 0x53E,
    SEQ_ME_CALL_LONG = 0x53F,
    SEQ_ME_ZUKAN_01 = 0x540,
    SEQ_ME_ZUKAN_02 = 0x541,
    SEQ_ME_POKEWOOD = 0x542,
    SEQ_ME_END = 0x543,
    SEQ_SE_DUMMY = 0x546,
    SEQ_SE_MESSAGE = 0x547,
    SEQ_SE_SELECT1 = 0x548,
    SEQ_SE_SELECT2 = 0x549,
    SEQ_SE_SELECT3 = 0x54A,
    SEQ_SE_SELECT4 = 0x54B,
    SEQ_SE_DECIDE1 = 0x54C,
    SEQ_SE_DECIDE2 = 0x54D,
    SEQ_SE_DECIDE3 = 0x54E,
    SEQ_SE_DECIDE4 = 0x54F,
    SEQ_SE_DECIDE5 = 0x550,
    SEQ_SE_CANCEL1 = 0x551,
    SEQ_SE_CANCEL2 = 0x552,
    SEQ_SE_CANCEL3 = 0x553,
    SEQ_SE_OPEN1 = 0x554,
    SEQ_SE_OPEN2 = 0x555,
    SEQ_SE_CLOSE1 = 0x556,
    SEQ_SE_BEEP = 0x557,
    SEQ_SE_SAVE = 0x558,
    SEQ_SE_KAIDAN = 0x559,
    SEQ_SE_WALL_HIT = 0x55A,
    SEQ_SE_PC_ON = 0x55B,
    SEQ_SE_PC_LOGIN = 0x55C,
    SEQ_SE_PC_LOGOFF = 0x55D,
    SEQ_SE_DANSA = 0x55E,
    SEQ_SE_BICYCLE = 0x55F,
    SEQ_SE_DUMMY5 = 0x560,
    SEQ_SE_BC01 = 0x561,
    SEQ_SE_BC02 = 0x562,
    SEQ_SE_KOUKA_H = 0x563,
    SEQ_SE_KOUKA_M = 0x564,
    SEQ_SE_KOUKA_L = 0x565,
    SEQ_SE_NAGERU = 0x566,
    SEQ_SE_BOWA2 = 0x567,
    SEQ_SE_BOWA1 = 0x568,
    SEQ_SE_BOWA3 = 0x569,
    SEQ_SE_NIGERU = 0x56A,
    SEQ_SE_KON = 0x56B,
    SEQ_SE_DEAD = 0x56C,
    SEQ_SE_EXP = 0x56D,
    SEQ_SE_EXPMAX = 0x56E,
    SEQ_SE_RECOVERY = 0x56F,
    SEQ_SE_TB_START = 0x570,
    SEQ_SE_TB_KON = 0x571,
    SEQ_SE_TB_KARA = 0x572,
    SEQ_SE_SHOOTER = 0x573,
    SEQ_SE_GETTING = 0x574,
    SEQ_SE_HINSHI = 0x575,
    SEQ_SE_TOUJOU_LARGE = 0x576,
    SEQ_SE_TOUJOU_MID = 0x577,
    SEQ_SE_TOUJOU_SMALL = 0x578,
    SEQ_SE_ROTATION_S = 0x579,
    SEQ_SE_ROTATION_B = 0x57A,
    SEQ_SE_TRIPLE_S = 0x57B,
    SEQ_SE_NAGERU2 = 0x57C,
    SEQ_SE_DUMMY29 = 0x57D,
    SEQ_SE_DUMMY30 = 0x57E,
    SEQ_SE_W055_01 = 0x57F,
    SEQ_SE_W055_04 = 0x580,
    SEQ_SE_W055_05 = 0x581,
    SEQ_SE_W075_01 = 0x582,
    SEQ_SE_W075_02 = 0x583,
    SEQ_SE_W075_03 = 0x584,
    SEQ_SE_W075_04 = 0x585,
    SEQ_SE_W075_05 = 0x586,
    SEQ_SE_W075_06 = 0x587,
    SEQ_SE_W157_01 = 0x588,
    SEQ_SE_W157_02 = 0x589,
    SEQ_SE_W157_03 = 0x58A,
    SEQ_SE_W157_04 = 0x58B,
    SEQ_SE_W001_01 = 0x58C,
    SEQ_SE_W002_01 = 0x58D,
    SEQ_SE_W003_01 = 0x58E,
    SEQ_SE_W010_01 = 0x58F,
    SEQ_SE_W011_01 = 0x590,
    SEQ_SE_W307_01 = 0x591,
    SEQ_SE_W307_02 = 0x592,
    SEQ_SE_W307_03 = 0x593,
    SEQ_SE_W307_04 = 0x594,
    SEQ_SE_W307_05 = 0x595,
    SEQ_SE_W006_01 = 0x596,
    SEQ_SE_W006_02 = 0x597,
    SEQ_SE_W006_03 = 0x598,
    SEQ_SE_W006_04 = 0x599,
    SEQ_SE_W014_01 = 0x59A,
    SEQ_SE_W014_02 = 0x59B,
    SEQ_SE_W014_03 = 0x59C,
    SEQ_SE_W015_01 = 0x59D,
    SEQ_SE_W017_01 = 0x59E,
    SEQ_SE_W018_01 = 0x59F,
    SEQ_SE_W018_02 = 0x5A0,
    SEQ_SE_W020_01 = 0x5A1,
    SEQ_SE_W022_01 = 0x5A2,
    SEQ_SE_W022_02 = 0x5A3,
    SEQ_SE_W026_01 = 0x5A4,
    SEQ_SE_W027_01 = 0x5A5,
    SEQ_SE_W028_01 = 0x5A6,
    SEQ_SE_W028_02 = 0x5A7,
    SEQ_SE_W029_01 = 0x5A8,
    SEQ_SE_W043_01 = 0x5A9,
    SEQ_SE_W048_01 = 0x5AA,
    SEQ_SE_W048_02 = 0x5AB,
    SEQ_SE_W051_01 = 0x5AC,
    SEQ_SE_W051_02 = 0x5AD,
    SEQ_SE_W051_03 = 0x5AE,
    SEQ_SE_W054_01 = 0x5AF,
    SEQ_SE_W074_01 = 0x5B0,
    SEQ_SE_W077_01 = 0x5B1,
    SEQ_SE_W077_02 = 0x5B2,
    SEQ_SE_W088_01 = 0x5B3,
    SEQ_SE_W088_02 = 0x5B4,
    SEQ_SE_W095_01 = 0x5B5,
    SEQ_SE_W095_02 = 0x5B6,
    SEQ_SE_W099_01 = 0x5B7,
    SEQ_SE_W099_02 = 0x5B8,
    SEQ_SE_W102_01 = 0x5B9,
    SEQ_SE_W102_02 = 0x5BA,
    SEQ_SE_W103_01 = 0x5BB,
    SEQ_SE_W111_01 = 0x5BC,
    SEQ_SE_W111_02 = 0x5BD,
    SEQ_SE_W112_01 = 0x5BE,
    SEQ_SE_W112_02 = 0x5BF,
    SEQ_SE_W113_01 = 0x5C0,
    SEQ_SE_W113_02 = 0x5C1,
    SEQ_SE_W116_01 = 0x5C2,
    SEQ_SE_W121_01 = 0x5C3,
    SEQ_SE_W122_01 = 0x5C4,
    SEQ_SE_W122_02 = 0x5C5,
    SEQ_SE_W123_01 = 0x5C6,
    SEQ_SE_W123_02 = 0x5C7,
    SEQ_SE_W124_01 = 0x5C8,
    SEQ_SE_W129_01 = 0x5C9,
    SEQ_SE_W129_03 = 0x5CA,
    SEQ_SE_W129_04 = 0x5CB,
    SEQ_SE_W165_01 = 0x5CC,
    SEQ_SE_W179_01 = 0x5CD,
    SEQ_SE_W179_02 = 0x5CE,
    SEQ_SE_W179_03 = 0x5CF,
    SEQ_SE_W181_01 = 0x5D0,
    SEQ_SE_W216_01 = 0x5D1,
    SEQ_SE_W216_02 = 0x5D2,
    SEQ_SE_W225_01 = 0x5D3,
    SEQ_SE_W225_02 = 0x5D4,
    SEQ_SE_W227_01 = 0x5D5,
    SEQ_SE_W227_02 = 0x5D6,
    SEQ_SE_W235_01 = 0x5D7,
    SEQ_SE_W235_03 = 0x5D8,
    SEQ_SE_W239_01 = 0x5D9,
    SEQ_SE_W239_02 = 0x5DA,
    SEQ_SE_W240_01 = 0x5DB,
    SEQ_SE_W253_01 = 0x5DC,
    SEQ_SE_W253_02 = 0x5DD,
    SEQ_SE_W260_01 = 0x5DE,
    SEQ_SE_W281_01 = 0x5DF,
    SEQ_SE_W295_01 = 0x5E0,
    SEQ_SE_W295_02 = 0x5E1,
    SEQ_SE_W298_01 = 0x5E2,
    SEQ_SE_W310_01 = 0x5E3,
    SEQ_SE_W310_02 = 0x5E4,
    SEQ_SE_W319_01 = 0x5E5,
    SEQ_SE_W319_02 = 0x5E6,
    SEQ_SE_W323_01 = 0x5E7,
    SEQ_SE_W320_01 = 0x5E8,
    SEQ_SE_W331_01 = 0x5E9,
    SEQ_SE_W348_02 = 0x5EA,
    SEQ_SE_W363_01 = 0x5EB,
    SEQ_SE_W368_01 = 0x5EC,
    SEQ_SE_W377_01 = 0x5ED,
    SEQ_SE_W377_02 = 0x5EE,
    SEQ_SE_W399_01 = 0x5EF,
    SEQ_SE_W401_01 = 0x5F0,
    SEQ_SE_W465_01 = 0x5F1,
    SEQ_SE_W009_01 = 0x5F2,
    SEQ_SE_W047_01 = 0x5F3,
    SEQ_SE_W050_01 = 0x5F4,
    SEQ_SE_W187_01 = 0x5F5,
    SEQ_SE_W012_01 = 0x5F6,
    SEQ_SE_W039_01 = 0x5F7,
    SEQ_SE_W044_01 = 0x5F8,
    SEQ_SE_W061_01 = 0x5F9,
    SEQ_SE_W146_01 = 0x5FA,
    SEQ_SE_W155_01 = 0x5FB,
    SEQ_SE_W155_02 = 0x5FC,
    SEQ_SE_W195_01 = 0x5FD,
    SEQ_SE_W199_01 = 0x5FE,
    SEQ_SE_W464_01 = 0x5FF,
    SEQ_SE_W464_02 = 0x600,
    SEQ_SE_W306_01 = 0x601,
    SEQ_SE_W019_01 = 0x602,
    SEQ_SE_W042_01 = 0x603,
    SEQ_SE_W215_01 = 0x604,
    SEQ_SE_W215_02 = 0x605,
    SEQ_SE_W274_01 = 0x606,
    SEQ_SE_W289_01 = 0x607,
    SEQ_SE_PASA1 = 0x608,
    SEQ_SE_PASA2 = 0x609,
    SEQ_SE_BE_SE_01 = 0x60A,
    SEQ_SE_BE_SE_02 = 0x60B,
    SEQ_SE_BE_SE_03 = 0x60C,
    SEQ_SE_BE_SE_04 = 0x60D,
    SEQ_SE_BE_SE_05 = 0x60E,
    SEQ_SE_BE_SE_06 = 0x60F,
    SEQ_SE_BE_SE_07 = 0x610,
    SEQ_SE_BE_SE_08 = 0x611,
    SEQ_SE_BE_SE_09 = 0x612,
    SEQ_SE_BE_SE_10 = 0x613,
    SEQ_SE_BE_SE_11 = 0x614,
    SEQ_SE_BE_SE_12 = 0x615,
    SEQ_SE_BE_SE_13 = 0x616,
    SEQ_SE_BE_SE_14 = 0x617,
    SEQ_SE_BE_SE_015 = 0x618,
    SEQ_SE_BE_SE_016 = 0x619,
    SEQ_SE_BE_SE_017 = 0x61A,
    SEQ_SE_BE_SE_018 = 0x61B,
    SEQ_SE_BE_SE_019 = 0x61C,
    SEQ_SE_BE_SE_020 = 0x61D,
    SEQ_SE_BE_SE_021 = 0x61E,
    SEQ_SE_BE_SE_022 = 0x61F,
    SEQ_SE_BE_SE_023 = 0x620,
    SEQ_SE_BE_SE_024 = 0x621,
    SEQ_SE_MSCL_01 = 0x622,
    SEQ_SE_MSCL_02 = 0x623,
    SEQ_SE_MSCL_03 = 0x624,
    SEQ_SE_MSCL_04 = 0x625,
    SEQ_SE_MSCL_05 = 0x626,
    SEQ_SE_MSCL_06 = 0x627,
    SEQ_SE_MSCL_07 = 0x628,
    SEQ_SE_MSCL_08 = 0x629,
    SEQ_SE_MSCL_09 = 0x62A,
    SEQ_SE_MSCL_10 = 0x62B,
    SEQ_SE_MSCL_11 = 0x62C,
    SEQ_SE_MSCL_12 = 0x62D,
    SEQ_SE_MSCL_13 = 0x62E,
    SEQ_SE_MSCL_14 = 0x62F,
    SEQ_SE_MSCL_15 = 0x630,
    SEQ_SE_MSCL_16 = 0x631,
    SEQ_SE_MSCL_17 = 0x632,
    SEQ_SE_MSCL_18 = 0x633,
    SEQ_SE_MSCL_19 = 0x634,
    SEQ_SE_SHINKA_W025 = 0x635,
    SEQ_SE_SHINKA_W060C = 0x636,
    SEQ_SE_SHINKA_W062 = 0x637,
    SEQ_SE_SHINKA_W080 = 0x638,
    SEQ_SE_SYS_73 = 0x639,
    SEQ_SE_SYS_74 = 0x63A,
    SEQ_SE_SYS_75 = 0x63B,
    SEQ_SE_FLD_123 = 0x63C,
    SEQ_SE_SYS_76 = 0x63D,
    SEQ_SE_SYS_77 = 0x63E,
    SEQ_SE_SYS_78 = 0x63F,
    SEQ_SE_SYS_01 = 0x640,
    SEQ_SE_SYS_02 = 0x641,
    SEQ_SE_SYS_03 = 0x642,
    SEQ_SE_SYS_04 = 0x643,
    SEQ_SE_SYS_05 = 0x644,
    SEQ_SE_SYS_06 = 0x645,
    SEQ_SE_SYS_07 = 0x646,
    SEQ_SE_SYS_08 = 0x647,
    SEQ_SE_SYS_09 = 0x648,
    SEQ_SE_SYS_10 = 0x649,
    SEQ_SE_SYS_11 = 0x64A,
    SEQ_SE_SYS_12 = 0x64B,
    SEQ_SE_SYS_13 = 0x64C,
    SEQ_SE_SYS_14 = 0x64D,
    SEQ_SE_SYS_15 = 0x64E,
    SEQ_SE_SYS_16 = 0x64F,
    SEQ_SE_SYS_17 = 0x650,
    SEQ_SE_SYS_18 = 0x651,
    SEQ_SE_SYS_19 = 0x652,
    SEQ_SE_SYS_20 = 0x653,
    SEQ_SE_SYS_21 = 0x654,
    SEQ_SE_SYS_22 = 0x655,
    SEQ_SE_SYS_23 = 0x656,
    SEQ_SE_SYS_24 = 0x657,
    SEQ_SE_SYS_25 = 0x658,
    SEQ_SE_SYS_26 = 0x659,
    SEQ_SE_SYS_27 = 0x65A,
    SEQ_SE_SYS_28 = 0x65B,
    SEQ_SE_SYS_29 = 0x65C,
    SEQ_SE_SYS_30 = 0x65D,
    SEQ_SE_SYS_31 = 0x65E,
    SEQ_SE_SYS_32 = 0x65F,
    SEQ_SE_SYS_33 = 0x660,
    SEQ_SE_SYS_34 = 0x661,
    SEQ_SE_SYS_35 = 0x662,
    SEQ_SE_SYS_36 = 0x663,
    SEQ_SE_SYS_37 = 0x664,
    SEQ_SE_SYS_38 = 0x665,
    SEQ_SE_SYS_39 = 0x666,
    SEQ_SE_SYS_40 = 0x667,
    SEQ_SE_SYS_41 = 0x668,
    SEQ_SE_SYS_42 = 0x669,
    SEQ_SE_SYS_43 = 0x66A,
    SEQ_SE_SYS_44 = 0x66B,
    SEQ_SE_SYS_45 = 0x66C,
    SEQ_SE_SYS_46 = 0x66D,
    SEQ_SE_SYS_47 = 0x66E,
    SEQ_SE_SYS_48 = 0x66F,
    SEQ_SE_SYS_49 = 0x670,
    SEQ_SE_SYS_50 = 0x671,
    SEQ_SE_FLD_01 = 0x672,
    SEQ_SE_FLD_02 = 0x673,
    SEQ_SE_FLD_03 = 0x674,
    SEQ_SE_FLD_04 = 0x675,
    SEQ_SE_FLD_05 = 0x676,
    SEQ_SE_FLD_06 = 0x677,
    SEQ_SE_FLD_07 = 0x678,
    SEQ_SE_FLD_08 = 0x679,
    SEQ_SE_FLD_09 = 0x67A,
    SEQ_SE_FLD_10 = 0x67B,
    SEQ_SE_FLD_11 = 0x67C,
    SEQ_SE_FLD_12 = 0x67D,
    SEQ_SE_FLD_13 = 0x67E,
    SEQ_SE_FLD_14 = 0x67F,
    SEQ_SE_FLD_15 = 0x680,
    SEQ_SE_FLD_16 = 0x681,
    SEQ_SE_FLD_17 = 0x682,
    SEQ_SE_FLD_18 = 0x683,
    SEQ_SE_FLD_19 = 0x684,
    SEQ_SE_FLD_20 = 0x685,
    SEQ_SE_FLD_21 = 0x686,
    SEQ_SE_FLD_22 = 0x687,
    SEQ_SE_FLD_23 = 0x688,
    SEQ_SE_FLD_24 = 0x689,
    SEQ_SE_FLD_25 = 0x68A,
    SEQ_SE_FLD_26 = 0x68B,
    SEQ_SE_FLD_27 = 0x68C,
    SEQ_SE_FLD_28 = 0x68D,
    SEQ_SE_FLD_29 = 0x68E,
    SEQ_SE_FLD_30 = 0x68F,
    SEQ_SE_FLD_31 = 0x690,
    SEQ_SE_FLD_32 = 0x691,
    SEQ_SE_FLD_33 = 0x692,
    SEQ_SE_FLD_34 = 0x693,
    SEQ_SE_FLD_35 = 0x694,
    SEQ_SE_FLD_36 = 0x695,
    SEQ_SE_FLD_37 = 0x696,
    SEQ_SE_FLD_38 = 0x697,
    SEQ_SE_FLD_39 = 0x698,
    SEQ_SE_FLD_40 = 0x699,
    SEQ_SE_FLD_41 = 0x69A,
    SEQ_SE_FLD_42 = 0x69B,
    SEQ_SE_FLD_43 = 0x69C,
    SEQ_SE_FLD_44 = 0x69D,
    SEQ_SE_FLD_45 = 0x69E,
    SEQ_SE_FLD_46 = 0x69F,
    SEQ_SE_FLD_47 = 0x6A0,
    SEQ_SE_FLD_48 = 0x6A1,
    SEQ_SE_FLD_49 = 0x6A2,
    SEQ_SE_FLD_50 = 0x6A3,
    SEQ_SE_W008_01 = 0x6A4,
    SEQ_SE_W012_02 = 0x6A5,
    SEQ_SE_W016_01 = 0x6A6,
    SEQ_SE_W023_01 = 0x6A7,
    SEQ_SE_W032_01 = 0x6A8,
    SEQ_SE_W034_01 = 0x6A9,
    SEQ_SE_W063_01 = 0x6AA,
    SEQ_SE_W063_02 = 0x6AB,
    SEQ_SE_W096_01 = 0x6AC,
    SEQ_SE_W100_01 = 0x6AD,
    SEQ_SE_W084_01 = 0x6AE,
    SEQ_SE_W269_01 = 0x6AF,
    SEQ_SE_W269_02 = 0x6B0,
    SEQ_SE_W278_01 = 0x6B1,
    SEQ_SE_W321_01 = 0x6B2,
    SEQ_SE_W452_01 = 0x6B3,
    SEQ_SE_W063_03 = 0x6B4,
    SEQ_SE_W059_01 = 0x6B5,
    SEQ_SE_W077_03 = 0x6B6,
    SEQ_SE_W008_02 = 0x6B7,
    SEQ_SE_W044_02 = 0x6B8,
    SEQ_SE_W100_02 = 0x6B9,
    SEQ_SE_W125_01 = 0x6BA,
    SEQ_SE_W142_01 = 0x6BB,
    SEQ_SE_W148_01 = 0x6BC,
    SEQ_SE_W163_01 = 0x6BD,
    SEQ_SE_W175_01 = 0x6BE,
    SEQ_SE_W188_01 = 0x6BF,
    SEQ_SE_W194_01 = 0x6C0,
    SEQ_SE_W208_01 = 0x6C1,
    SEQ_SE_FLD_51 = 0x6C2,
    SEQ_SE_FLD_52 = 0x6C3,
    SEQ_SE_FLD_53 = 0x6C4,
    SEQ_SE_FLD_54 = 0x6C5,
    SEQ_SE_FLD_55 = 0x6C6,
    SEQ_SE_FLD_56 = 0x6C7,
    SEQ_SE_FLD_57 = 0x6C8,
    SEQ_SE_FLD_58 = 0x6C9,
    SEQ_SE_FLD_59 = 0x6CA,
    SEQ_SE_FLD_60 = 0x6CB,
    SEQ_SE_FLD_61 = 0x6CC,
    SEQ_SE_FLD_62 = 0x6CD,
    SEQ_SE_FLD_63 = 0x6CE,
    SEQ_SE_FLD_64 = 0x6CF,
    SEQ_SE_FLD_65 = 0x6D0,
    SEQ_SE_FLD_66 = 0x6D1,
    SEQ_SE_FLD_67 = 0x6D2,
    SEQ_SE_FLD_68 = 0x6D3,
    SEQ_SE_FLD_69 = 0x6D4,
    SEQ_SE_FLD_70 = 0x6D5,
    SEQ_SE_FLD_71 = 0x6D6,
    SEQ_SE_FLD_72 = 0x6D7,
    SEQ_SE_FLD_73 = 0x6D8,
    SEQ_SE_FLD_74 = 0x6D9,
    SEQ_SE_FLD_75 = 0x6DA,
    SEQ_SE_FLD_76 = 0x6DB,
    SEQ_SE_FLD_77 = 0x6DC,
    SEQ_SE_FLD_78 = 0x6DD,
    SEQ_SE_FLD_79 = 0x6DE,
    SEQ_SE_FLD_80 = 0x6DF,
    SEQ_SE_FLD_81 = 0x6E0,
    SEQ_SE_FLD_82 = 0x6E1,
    SEQ_SE_FLD_83 = 0x6E2,
    SEQ_SE_FLD_84 = 0x6E3,
    SEQ_SE_FLD_85 = 0x6E4,
    SEQ_SE_DMY_097 = 0x6E5,
    SEQ_SE_LVUP = 0x6E6,
    SEQ_SE_FLD_86 = 0x6E7,
    SEQ_SE_FLD_87 = 0x6E8,
    SEQ_SE_FLD_88 = 0x6E9,
    SEQ_SE_FLD_89 = 0x6EA,
    SEQ_SE_FLD_90 = 0x6EB,
    SEQ_SE_FLD_91 = 0x6EC,
    SEQ_SE_FLD_92 = 0x6ED,
    SEQ_SE_FLD_93 = 0x6EE,
    SEQ_SE_FLD_94 = 0x6EF,
    SEQ_SE_FLD_95 = 0x6F0,
    SEQ_SE_W213_01 = 0x6F1,
    SEQ_SE_W227_03 = 0x6F2,
    SEQ_SE_W254_04 = 0x6F3,
    SEQ_SE_W234_HIKARI = 0x6F4,
    SEQ_SE_W288_ONNEN = 0x6F5,
    SEQ_SE_W357_JIRORI = 0x6F6,
    SEQ_SE_W379_POWER = 0x6F7,
    SEQ_SE_W381_OMAJINAI = 0x6F8,
    SEQ_SE_W396_HADOKEN = 0x6F9,
    SEQ_SE_W445_YUUWAKU = 0x6FA,
    SEQ_SE_W343_HOSHIGARI = 0x6FB,
    SEQ_SE_FLD_96 = 0x6FC,
    SEQ_SE_FLD_97 = 0x6FD,
    SEQ_SE_BREC_01 = 0x6FE,
    SEQ_SE_BREC_02 = 0x6FF,
    SEQ_SE_BREC_03 = 0x700,
    SEQ_SE_BREC_04 = 0x701,
    SEQ_SE_BREC_05 = 0x702,
    SEQ_SE_BREC_06 = 0x703,
    SEQ_SE_BREC_07 = 0x704,
    SEQ_SE_BREC_08 = 0x705,
    SEQ_SE_BE_SE_025 = 0x706,
    SEQ_SE_BE_SE_026 = 0x707,
    SEQ_SE_BE_SE_027 = 0x708,
    SEQ_SE_BE_SE_028 = 0x709,
    SEQ_SE_BE_SE_029 = 0x70A,
    SEQ_SE_BE_SE_030 = 0x70B,
    SEQ_SE_BE_SE_031 = 0x70C,
    SEQ_SE_BE_SE_032 = 0x70D,
    SEQ_SE_BE_SE_033 = 0x70E,
    SEQ_SE_BE_SE_034 = 0x70F,
    SEQ_SE_BE_SE_035 = 0x710,
    SEQ_SE_BE_SE_036 = 0x711,
    SEQ_SE_BE_SE_037 = 0x712,
    SEQ_SE_BE_SE_038 = 0x713,
    SEQ_SE_BE_SE_039 = 0x714,
    SEQ_SE_BE_SE_040 = 0x715,
    SEQ_SE_BE_SE_041 = 0x716,
    SEQ_SE_W221_NOISE = 0x717,
    SEQ_SE_W229_KIIN = 0x718,
    SEQ_SE_W245_JUMP = 0x719,
    SEQ_SE_W245_BUCKL = 0x71A,
    SEQ_SE_W250_UZUSHIO = 0x71B,
    SEQ_SE_W258_ARARE = 0x71C,
    SEQ_SE_W300_PITYA = 0x71D,
    SEQ_SE_FLD_98 = 0x71E,
    SEQ_SE_FLD_99 = 0x71F,
    SEQ_SE_FLD_101 = 0x720,
    SEQ_SE_W259_BOSO = 0x721,
    SEQ_SE_W259_PAKIP = 0x722,
    SEQ_SE_W275_PIRA = 0x723,
    SEQ_SE_SYS_51 = 0x724,
    SEQ_SE_SYS_52 = 0x725,
    SEQ_SE_SYS_53 = 0x726,
    SEQ_SE_SYS_54 = 0x727,
    SEQ_SE_SYS_55 = 0x728,
    SEQ_SE_SYS_56 = 0x729,
    SEQ_SE_SYS_57 = 0x72A,
    SEQ_SE_SYS_58 = 0x72B,
    SEQ_SE_SYS_59 = 0x72C,
    SEQ_SE_SYS_60 = 0x72D,
    SEQ_SE_SYS_61 = 0x72E,
    SEQ_SE_SYS_62 = 0x72F,
    SEQ_SE_SYS_63 = 0x730,
    SEQ_SE_SYS_64 = 0x731,
    SEQ_SE_SYS_65 = 0x732,
    SEQ_SE_SYS_66 = 0x733,
    SEQ_SE_SYS_67 = 0x734,
    SEQ_SE_SYS_68 = 0x735,
    SEQ_SE_FLD_102 = 0x736,
    SEQ_SE_FLD_103 = 0x737,
    SEQ_SE_W255_BU = 0x738,
    SEQ_SE_W288_DORO = 0x739,
    SEQ_SE_SYS_69 = 0x73A,
    SEQ_SE_SYS_70 = 0x73B,
    SEQ_SE_SYS_71 = 0x73C,
    SEQ_SE_SYS_72 = 0x73D,
    SEQ_SE_FLD_104 = 0x73E,
    SEQ_SE_FLD_105 = 0x73F,
    SEQ_SE_FLD_106 = 0x740,
    SEQ_SE_FLD_107 = 0x741,
    SEQ_SE_FLD_108 = 0x742,
    SEQ_SE_FLD_109 = 0x743,
    SEQ_SE_FLD_110 = 0x744,
    SEQ_SE_FLD_111 = 0x745,
    SEQ_SE_FLD_112 = 0x746,
    SEQ_SE_FLD_113 = 0x747,
    SEQ_SE_FLD_114 = 0x748,
    SEQ_SE_FLD_115 = 0x749,
    SEQ_SE_W255_MOMOMO = 0x74A,
    SEQ_SE_W288_HYUU = 0x74B,
    SEQ_SE_W312_HARP = 0x74C,
    SEQ_SE_W319_SWARD = 0x74D,
    SEQ_SE_W300_BESYO = 0x74E,
    SEQ_SE_W322_COSMO = 0x74F,
    SEQ_SE_W325_BUNN = 0x750,
    SEQ_SE_W350_PASU = 0x751,
    SEQ_SE_W454_MUSHI = 0x752,
    SEQ_SE_W360_MOMOMON = 0x753,
    SEQ_SE_W437_NOISEHIT = 0x754,
    SEQ_SE_FLD_116 = 0x755,
    SEQ_SE_FLD_117 = 0x756,
    SEQ_SE_FLD_118 = 0x757,
    SEQ_SE_FLD_119 = 0x758,
    SEQ_SE_W448_ONPU = 0x759,
    SEQ_SE_W065_DODODO = 0x75A,
    SEQ_SE_W470_WA = 0x75B,
    SEQ_SE_W307_EXP = 0x75C,
    SEQ_SE_W474_GUTSU = 0x75D,
    SEQ_SE_W474_DOKU = 0x75E,
    SEQ_SE_FLD_120 = 0x75F,
    SEQ_SE_FLD_121 = 0x760,
    SEQ_SE_FLD_122 = 0x761,
    SEQ_SE_SYS_79 = 0x762,
    SEQ_SE_FLD_124 = 0x763,
    SEQ_SE_FLD_125 = 0x764,
    SEQ_SE_FLD_126 = 0x765,
    SEQ_SE_FLD_127 = 0x766,
    SEQ_SE_FLD_128 = 0x767,
    SEQ_SE_FLD_129 = 0x768,
    SEQ_SE_SYS_80 = 0x769,
    SEQ_SE_SYS_81 = 0x76A,
    SEQ_SE_SYS_82 = 0x76B,
    SEQ_SE_SYS_83 = 0x76C,
    SEQ_SE_FLD_130 = 0x76D,
    SEQ_SE_SYS_84 = 0x76E,
    SEQ_SE_W532_SASU = 0x76F,
    SEQ_SE_W532_CHU = 0x770,
    SEQ_SE_W540_KYUN = 0x771,
    SEQ_SE_FLD_131 = 0x772,
    SEQ_SE_FLD_132 = 0x773,
    SEQ_SE_FLD_133 = 0x774,
    SEQ_SE_FLD_134 = 0x775,
    SEQ_SE_FLD_135 = 0x776,
    SEQ_SE_FLD_136 = 0x777,
    SEQ_SE_FLD_137 = 0x778,
    SEQ_SE_FLD_138 = 0x779,
    SEQ_SE_FLD_139 = 0x77A,
    SEQ_SE_W547_INIMELO = 0x77B,
    SEQ_SE_SYS_85 = 0x77C,
    SEQ_SE_FLD_140 = 0x77D,
    SEQ_SE_FLD_141 = 0x77E,
    SEQ_SE_W542_BOUFU = 0x77F,
    SEQ_SE_W554_PAKIN = 0x780,
    SEQ_SE_FLD_142 = 0x781,
    SEQ_SE_FLD_143 = 0x782,
    SEQ_SE_FLD_144 = 0x783,
    SEQ_SE_TDEMO_001 = 0x784,
    SEQ_SE_TDEMO_002 = 0x785,
    SEQ_SE_TDEMO_003 = 0x786,
    SEQ_SE_TDEMO_004 = 0x787,
    SEQ_SE_TDEMO_005 = 0x788,
    SEQ_SE_TDEMO_006 = 0x789,
    SEQ_SE_TDEMO_007 = 0x78A,
    SEQ_SE_TDEMO_008 = 0x78B,
    SEQ_SE_TDEMO_009 = 0x78C,
    SEQ_SE_TDEMO_010 = 0x78D,
    SEQ_SE_TDEMO_011 = 0x78E,
    SEQ_SE_SYS_86 = 0x78F,
    SEQ_SE_SYS_87 = 0x790,
    SEQ_SE_FLD_145 = 0x791,
    SEQ_SE_FLD_146 = 0x792,
    SEQ_SE_SYS_88 = 0x793,
    SEQ_SE_SYS_89 = 0x794,
    SEQ_SE_W556_FUUN = 0x795,
    SEQ_SE_NDEMO_01 = 0x796,
    SEQ_SE_NDEMO_02 = 0x797,
    SEQ_SE_W556_KAZE = 0x798,
    SEQ_SE_DDEMO_01 = 0x799,
    SEQ_SE_DDEMO_02A = 0x79A,
    SEQ_SE_DDEMO_02B = 0x79B,
    SEQ_SE_DDEMO_03 = 0x79C,
    SEQ_SE_DDEMO_04 = 0x79D,
    SEQ_SE_SYS_90 = 0x79E,
    SEQ_SE_FLD_147 = 0x79F,
    SEQ_SE_FLD_148 = 0x7A0,
    SEQ_SE_EDEMO_01 = 0x7A1,
    SEQ_SE_EDEMO_02 = 0x7A2,
    SEQ_SE_SDEMO_01 = 0x7A3,
    SEQ_SE_SDEMO_02 = 0x7A4,
    SEQ_SE_FLD_149 = 0x7A5,
    SEQ_SE_FLD_150 = 0x7A6,
    SEQ_SE_EDEMO_03 = 0x7A7,
    SEQ_SE_MSCL_20 = 0x7A8,
    SEQ_SE_MSCL_21 = 0x7A9,
    SEQ_SE_W063_BEEMB = 0x7AA,
    SEQ_SE_W281_AKUBI = 0x7AB,
    SEQ_SE_W248_TIKIRIN = 0x7AC,
    SEQ_SE_W411_HAAA = 0x7AD,
    SEQ_SE_EDEMO_04 = 0x7AE,
    SEQ_SE_EDEMO_05 = 0x7AF,
    SEQ_SE_FLD_151 = 0x7B0,
    SEQ_SE_SYS_91 = 0x7B1,
    SEQ_SE_BDEMO_01 = 0x7B2,
    SEQ_SE_BDEMO_02 = 0x7B3,
    SEQ_SE_BDEMO_03 = 0x7B4,
    SEQ_SE_BDEMO_04 = 0x7B5,
    SEQ_SE_FLD_152 = 0x7B6,
    SEQ_SE_SDEMO_03 = 0x7B7,
    SEQ_SE_LBDEMO_01 = 0x7B8,
    SEQ_SE_LBDEMO_02 = 0x7B9,
    SEQ_SE_LBDEMO_03 = 0x7BA,
    SEQ_SE_LBDEMO_04 = 0x7BB,
    SEQ_SE_LBDEMO_05 = 0x7BC,
    SEQ_SE_LBDEMO_06 = 0x7BD,
    SEQ_SE_LBDEMO_07 = 0x7BE,
    SEQ_SE_LBDEMO_08 = 0x7BF,
    SEQ_SE_LBDEMO_09 = 0x7C0,
    SEQ_SE_FLD_153 = 0x7C1,
    SEQ_SE_FLD_154 = 0x7C2,
    SEQ_SE_FLD_155 = 0x7C3,
    SEQ_SE_FLD_156 = 0x7C4,
    SEQ_SE_FLD_157 = 0x7C5,
    SEQ_SE_FLD_158 = 0x7C6,
    SEQ_SE_FLD_159 = 0x7C7,
    SEQ_SE_FLD_160 = 0x7C8,
    SEQ_SE_FLD_161 = 0x7C9,
    SEQ_SE_FLD_162 = 0x7CA,
    SEQ_SE_FLD_163 = 0x7CB,
    SEQ_SE_SYS_92 = 0x7CC,
    SEQ_SE_FLD_164 = 0x7CD,
    SEQ_SE_SYS_93 = 0x7CE,
    SEQ_SE_FLD_165 = 0x7CF,
    SEQ_SE_SYS_94 = 0x7D0,
    SEQ_SE_SYS_95 = 0x7D1,
    SEQ_SE_SYS_96 = 0x7D2,
    SEQ_SE_SYS_97 = 0x7D3,
    SEQ_SE_SYS_98 = 0x7D4,
    SEQ_SE_FLD_166 = 0x7D5,
    SEQ_SE_VDEMO_01 = 0x7D6,
    SEQ_SE_VDEMO_02 = 0x7D7,
    SEQ_SE_SYS_99 = 0x7D8,
    SEQ_SE_BGDEMO_01 = 0x7D9,
    SEQ_SE_BGDEMO_02 = 0x7DA,
    SEQ_SE_SHDEMO_01 = 0x7DB,
    SEQ_SE_SHDEMO_02 = 0x7DC,
    SEQ_SE_SHDEMO_03 = 0x7DD,
    SEQ_SE_SHDEMO_04 = 0x7DE,
    SEQ_SE_SHDEMO_05 = 0x7DF,
    SEQ_SE_NDEMO_03 = 0x7E0,
    SEQ_SE_ARDEMO_01 = 0x7E1,
    SEQ_SE_SYS_100 = 0x7E2,
    SEQ_SE_RIZDEMO_01 = 0x7E3,
    SEQ_SE_FLD_167 = 0x7E4,
    SEQ_SE_FLD_168 = 0x7E5,
    SEQ_SE_FLD_169 = 0x7E6,
    SEQ_SE_FLD_170 = 0x7E7,
    SEQ_SE_VDEMO_03 = 0x7E8,
    SEQ_SE_RIZDEMO_02 = 0x7E9,
    SEQ_SE_FLD_171 = 0x7EA,
    SEQ_SE_FLD_172 = 0x7EB,
    SEQ_SE_FLD_173 = 0x7EC,
    SEQ_SE_END_01 = 0x7ED,
    SEQ_SE_FLD_174 = 0x7EE,
    SEQ_SE_END_02 = 0x7EF,
    SEQ_SE_W156_BABBLE2 = 0x7F0,
    SEQ_SE_FLD_175 = 0x7F1,
    SEQ_SE_ZORO_01 = 0x7F2,
    SEQ_SE_BE_SE_04R = 0x7F3,
    SEQ_SE_TOUJOU_INTRO = 0x7F4,
    SEQ_SE_END_03 = 0x7F5,
    SEQ_SE_OPEN_01 = 0x7F6,
    SEQ_SE_OPEN_02 = 0x7F7,
    SEQ_SE_OPEN_03 = 0x7F8,
    SEQ_SE_END_04 = 0x7F9,
    SEQ_SE_END_05 = 0x7FA,
    SEQ_SE_SW_MDUN_BL_06 = 0x7FB,
    SEQ_SE_MEET_02 = 0x7FC,
    SEQ_SE_MEET_03 = 0x7FD,
    SEQ_SE_MEET_04 = 0x7FE,
    SEQ_SE_WCS_01 = 0x7FF,
    SEQ_SE_WCS_02 = 0x800,
    SEQ_SE_WCS_03 = 0x801,
    SEQ_SE_WCS_04 = 0x802,
    SEQ_SE_LCDEMO_01 = 0x803,
    SEQ_SE_LCDEMO_02 = 0x804,
    SEQ_SE_SYS_101 = 0x805,
    SEQ_SE_END_06 = 0x806,
    SEQ_SE_END_07 = 0x807,
    SEQ_SE_LOGO_01 = 0x808,
    SEQ_SE_LOGO_02 = 0x809,
    SEQ_SE_LOGO_03 = 0x80A,
    SEQ_SE_FLD_176 = 0x80B,
    SEQ_SE_SYS_102 = 0x80C,
    SEQ_SE_DECIDE6 = 0x80D,
    SEQ_SE_GYM_E01 = 0x80E,
    SEQ_SE_GYM_E02 = 0x80F,
    SEQ_SE_GYM_E03 = 0x810,
    SEQ_SE_GYM_E04 = 0x811,
    SEQ_SE_GYM_M01 = 0x812,
    SEQ_SE_GYM_M02 = 0x813,
    SEQ_SE_GYM_M03 = 0x814,
    SEQ_SE_GYM_M04 = 0x815,
    SEQ_SE_GYM_M05 = 0x816,
    SEQ_SE_GYM_M06 = 0x817,
    SEQ_SE_GYM_H01 = 0x818,
    SEQ_SE_GYM_H02 = 0x819,
    SEQ_SE_GYM_H03 = 0x81A,
    SEQ_SE_GYM_H04 = 0x81B,
    SEQ_SE_GYM_H05 = 0x81C,
    SEQ_SE_GYM_H06 = 0x81D,
    SEQ_SE_GYM_H07 = 0x81E,
    SEQ_SE_GYM_H08 = 0x81F,
    SEQ_SE_LCG_01 = 0x820,
    SEQ_SE_LCG_02 = 0x821,
    SEQ_SE_LCG_03 = 0x822,
    SEQ_SE_LCG_04 = 0x823,
    SEQ_SE_LCG_05 = 0x824,
    SEQ_SE_LCG_06 = 0x825,
    SEQ_SE_LCG_07 = 0x826,
    SEQ_SE_LCG_08 = 0x827,
    SEQ_SE_LCG_09 = 0x828,
    SEQ_SE_LCG_10 = 0x829,
    SEQ_SE_LCG_11 = 0x82A,
    SEQ_SE_MD_01 = 0x82B,
    SEQ_SE_MD_02 = 0x82C,
    SEQ_SE_MD_03 = 0x82D,
    SEQ_SE_MD_04 = 0x82E,
    SEQ_SE_SW_WBT_01 = 0x82F,
    SEQ_SE_SW_WBT_02 = 0x830,
    SEQ_SE_SW_WBT_03 = 0x831,
    SEQ_SE_SW_WBT_04 = 0x832,
    SEQ_SE_SW_WBT_05 = 0x833,
    SEQ_SE_SW_WBT_06 = 0x834,
    SEQ_SE_SW_WBT_07 = 0x835,
    SEQ_SE_SW_WBT_09 = 0x836,
    SEQ_SE_SW_WBT_10 = 0x837,
    SEQ_SE_SW_WBT_11 = 0x838,
    SEQ_SE_SW_WBT_12 = 0x839,
    SEQ_SE_SW_WBT_13 = 0x83A,
    SEQ_SE_SW_WBT_15 = 0x83B,
    SEQ_SE_SW_WBT_16 = 0x83C,
    SEQ_SE_SW_WBT_17 = 0x83D,
    SEQ_SE_SW_WBT_18 = 0x83E,
    SEQ_SE_SW_WBT_19 = 0x83F,
    SEQ_SE_SW_WBT_20 = 0x840,
    SEQ_SE_SW_WBT_21 = 0x841,
    SEQ_SE_SW_WBT_22 = 0x842,
    SEQ_SE_SW_FES_01 = 0x843,
    SEQ_SE_SW_FES_02 = 0x844,
    SEQ_SE_SW_FES_03 = 0x845,
    SEQ_SE_SW_FES_04 = 0x846,
    SEQ_SE_SW_FES_05 = 0x847,
    SEQ_SE_SW_FES_06 = 0x848,
    SEQ_SE_SW_FES_07 = 0x849,
    SEQ_SE_SW_FES_08 = 0x84A,
    SEQ_SE_SW_FES_09 = 0x84B,
    SEQ_SE_SW_FES_10 = 0x84C,
    SEQ_SE_SW_FES_11 = 0x84D,
    SEQ_SE_SW_FES_12 = 0x84E,
    SEQ_SE_SW_FES_13 = 0x84F,
    SEQ_SE_SW_FES_14 = 0x850,
    SEQ_SE_SW_FES_15 = 0x851,
    SEQ_SE_SW_FES_16 = 0x852,
    SEQ_SE_SW_MDUN_BL_01 = 0x853,
    SEQ_SE_SW_MDUN_BL_02 = 0x854,
    SEQ_SE_SW_MDUN_BL_03 = 0x855,
    SEQ_SE_SW_MDUN_BL_04 = 0x856,
    SEQ_SE_SW_MDUN_BL_05 = 0x857,
    SEQ_SE_SW_MDUN_WH_01 = 0x858,
    SEQ_SE_SW_MDUN_WH_02 = 0x859,
    SEQ_SE_SW_MDUN_WH_03 = 0x85A,
    SEQ_SE_SW_MDUN_WH_04 = 0x85B,
    SEQ_SE_SW_MDUN_WH_05 = 0x85C,
    SEQ_SE_SW_MDUN_BL_07 = 0x85D,
    SEQ_SE_SW_MDUN_WH_07 = 0x85E,
    SEQ_SE_SW_CARD_01 = 0x85F,
    SEQ_SE_SW_CARD_02 = 0x860,
    SEQ_SE_SW_CARD_03 = 0x861,
    SEQ_SE_SW_CARD_04 = 0x862,
    SEQ_SE_SW_CARD_05 = 0x863,
    SEQ_SE_SW_CARD_06 = 0x864,
    SEQ_SE_SW_CARD_07 = 0x865,
    SEQ_SE_SW_CARD_08 = 0x866,
    SEQ_SE_SW_CARD_09 = 0x867,
    SEQ_SE_SW_CARD_10 = 0x868,
    SEQ_SE_SW_CARD_11 = 0x869,
    SEQ_SE_SW_CARD_12 = 0x86A,
    SEQ_SE_SW_CARD_13 = 0x86B,
    SEQ_SE_SW_CARD_14 = 0x86C,
    SEQ_SE_SW_CARD_15 = 0x86D,
    SEQ_SE_SW_CARD_16 = 0x86E,
    SEQ_SE_SW_CARD_17 = 0x86F,
    SEQ_SE_SW_CARD_18 = 0x870,
    SEQ_SE_SW_CARD_19 = 0x871,
    SEQ_SE_SW_CARD_20 = 0x872,
    SEQ_SE_SW_CARD_21 = 0x873,
    SEQ_SE_SW_CARD_22 = 0x874,
    SEQ_SE_SW_GRGYM_01 = 0x875,
    SEQ_SE_SW_GRGYM_02 = 0x876,
    SEQ_SE_SW_NPOKE = 0x877,
    SEQ_SE_SW_KYUREM_01 = 0x878,
    SEQ_SE_SW_KYUREM_02 = 0x879,
    SEQ_SE_SW_KYUREM_03 = 0x87A,
    SEQ_SE_SW_KYUREM_04 = 0x87B,
    SEQ_SE_SW_MIZUGYM_01 = 0x87C,
    SEQ_SE_SW_MIZUGYM_02 = 0x87D,
    SEQ_SE_OCHIBA_01 = 0x87E,
    SEQ_SE_OCHIBA_02 = 0x87F,
    SEQ_SE_SW_ZKN_KAIHOU = 0x880,
    SEQ_SE_SW_LC_NO = 0x881,
    SEQ_SE_SW_GEEMA_01 = 0x882,
    SEQ_SE_SW_GEEMA_02 = 0x883,
    SEQ_SE_SW_GEEMA_03 = 0x884,
    SEQ_SE_SW_GEEMA_04 = 0x885,
    SEQ_SE_SW_DRGGYM_01 = 0x886,
    SEQ_SE_SW_DRGGYM_02 = 0x887,
    SEQ_SE_SW_DRGGYM_03 = 0x888,
    SEQ_SE_SW_DRGGYM_04 = 0x889,
    SEQ_SE_SW_DRGGYM_05 = 0x88A,
    SEQ_SE_SW_DRGGYM_06 = 0x88B,
    SEQ_SE_SW_DRGGYM_07 = 0x88C,
    SEQ_SE_SW_CATTLEYA_01 = 0x88D,
    SEQ_SE_SW_CATTLEYA_02 = 0x88E,
    SEQ_SE_SW_CATTLEYA_03 = 0x88F,
    SEQ_SE_SW_SHIKIMI_01 = 0x890,
    SEQ_SE_SW_SHIKIMI_02 = 0x891,
    SEQ_SE_SW_SHIKIMI_03 = 0x892,
    SEQ_SE_SW_SHIKIMI_04 = 0x893,
    SEQ_SE_SW_SHIKIMI_05 = 0x894,
    SEQ_SE_SW_SHIKIMI_06 = 0x895,
    SEQ_SE_SW_RENBU_01 = 0x896,
    SEQ_SE_SW_RENBU_02 = 0x897,
    SEQ_SE_SW_RENBU_03 = 0x898,
    SEQ_SE_SW_RENBU_04 = 0x899,
    SEQ_SE_SW_RENBU_05 = 0x89A,
    SEQ_SE_SW_BCG_01 = 0x89B,
    SEQ_SE_SW_BCG_02 = 0x89C,
    SEQ_SE_SW_BCG_03 = 0x89D,
    SEQ_SE_SW_BCG_04 = 0x89E,
    SEQ_SE_SW_BCG_05 = 0x89F,
    SEQ_SE_SW_BCG_06 = 0x8A0,
    SEQ_SE_SW_BCG_07 = 0x8A1,
    SEQ_SE_SW_KEYSYS_01 = 0x8A2,
    SEQ_SE_SW_KEYSYS_02 = 0x8A3,
    SEQ_SE_SW_KEYSYS_03 = 0x8A4,
    SEQ_SE_SW_KEYSYS_04 = 0x8A5,
    SEQ_SE_SW_PLAZMASHIP_01 = 0x8A6,
    SEQ_SE_SW_PLAZMASHIP_02 = 0x8A7,
    SEQ_SE_SW_PLAZMASHIP_03 = 0x8A8,
    SEQ_SE_SW_PLAZMASHIP_04 = 0x8A9,
    SEQ_SE_SW_PLAZMASHIP_05 = 0x8AA,
    SEQ_SE_SW_PLAZMASHIP_06 = 0x8AB,
    SEQ_SE_SW_PLAZMASHIP_07 = 0x8AC,
    SEQ_SE_SW_PLAZMASHIP_08 = 0x8AD,
    SEQ_SE_SW_PLAZMASHIP_09 = 0x8AE,
    SEQ_SE_SW_PLAZMASHIP_10 = 0x8AF,
    SEQ_SE_SW_ZUKANSTUMP_01 = 0x8B0,
    SEQ_SE_SW_YACONROAD_01 = 0x8B1,
    SEQ_SE_SW_RELIC_01 = 0x8B2,
    SEQ_SE_SW_RELIC_02 = 0x8B3,
    SEQ_SE_SW_RELIC_03 = 0x8B4,
    SEQ_SE_SW_CHAMPIRIS_01 = 0x8B5,
    SEQ_SE_SW_CHAMPIRIS_02 = 0x8B6,
    SEQ_SE_SW_CHAMPIRIS_03 = 0x8B7,
    SEQ_SE_SW_CHAMPIRIS_04 = 0x8B8,
    SEQ_SE_SW_CHAMPIRIS_05 = 0x8B9,
    SEQ_SE_SW_CHAMPIRIS_06 = 0x8BA,
    SEQ_SE_SW_PKWOOD_01 = 0x8BB,
    SEQ_SE_SW_PKWOOD_02 = 0x8BC,
    SEQ_SE_SW_PKWOOD_03 = 0x8BD,
    SEQ_SE_SW_STARPOKE = 0x8BE,
    SEQ_SE_SW_PKWOOD_AUD_01 = 0x8BF,
    SEQ_SE_SW_PKWOOD_AUD_02 = 0x8C0,
    SEQ_SE_SW_PKWOOD_AUD_03 = 0x8C1,
    SEQ_SE_SW_PKWOOD_AUD_04 = 0x8C2,
    SEQ_SE_SW_PKWOOD_AUD_05 = 0x8C3,
    SEQ_SE_SW_PKWOOD_AUD_06 = 0x8C4,
    SEQ_SE_SW_PKWOOD_AUD_07 = 0x8C5,
    SEQ_SE_SW_PKWOOD_AUD_08 = 0x8C6,
    SEQ_SE_SW_PKWOOD_AUD_09 = 0x8C7,
    SEQ_SE_SW_PKWOOD_AUD_10 = 0x8C8,
    SEQ_SE_SW_PKWOOD_AUD_11 = 0x8C9,
    SEQ_SE_SW_PKWOOD_AUD_12 = 0x8CA,
    SEQ_SE_SW_PKWOOD_AUD_13 = 0x8CB,
    SEQ_SE_SW_PKWOOD_AUD_14 = 0x8CC,
    SEQ_SE_SW_PKWOOD_AUD_15 = 0x8CD,
    SEQ_SE_SW_PKWOOD_AUD_16 = 0x8CE,
    SEQ_SE_SW_BELLBOX_01 = 0x8CF,
    SEQ_SE_SW_BELLBOX_02 = 0x8D0,
    SEQ_SE_SW_BELLBOX_03 = 0x8D1,
    SEQ_SE_SW_MEDALGET = 0x8D2,
    SEQ_SE_SW_MEDALGET_02 = 0x8D3,
    SEQ_SE_SW_JA_EXP = 0x8D4,
    SEQ_SE_SW_JA_01 = 0x8D5,
    SEQ_SE_SW_JA_02 = 0x8D6,
    SEQ_SE_SW_SHIZUI = 0x8D7,
    SEQ_SE_SW_KOBALON_01 = 0x8D8,
    SEQ_SE_SW_KOBALON_02 = 0x8D9,
    SEQ_SE_SW_SOURYU_RUMBLE = 0x8DA,
    SEQ_SE_SW_TOROS_FORM = 0x8DB,
    SEQ_SE_SW_KERTIO_FORM_01 = 0x8DC,
    SEQ_SE_SW_KERTIO_FORM_02 = 0x8DD,
    SEQ_SE_SW_KERTIO_FORM_03 = 0x8DE,
    SEQ_SE_SW_KERTIO_FORM_04 = 0x8DF,
    SEQ_SE_SW_KERTIO_FORM_05 = 0x8E0,
    SEQ_SE_SW_KERTIO_FORM_06 = 0x8E1,
    SEQ_SE_SW_GHETSIS_STICK_01 = 0x8E2,
    SEQ_SE_SW_GHETSIS_STICK_02 = 0x8E3,
    SEQ_SE_SW_CI_VSCHAMP_01 = 0x8E4,
    SEQ_SE_SW_CI_VSCHAMP_02 = 0x8E5,
    SEQ_SE_SW_CI_VSCHAMP_03 = 0x8E6,
    SEQ_SE_SW_CI_VSCHAMP_04 = 0x8E7,
    SEQ_SE_SW_CI_WBT_01 = 0x8E8,
    SEQ_SE_SW_CI_WBT_02 = 0x8E9,
    SEQ_SE_SW_CI_WBT_03 = 0x8EA,
    SEQ_SE_SW_CI_WBT_04 = 0x8EB,
    SEQ_SE_SW_CI_PKWOOD_01 = 0x8EC,
    SEQ_SE_SW_CI_PKWOOD_02 = 0x8ED,
    SEQ_SE_SW_CI_PKWOOD_03 = 0x8EE,
    SEQ_SE_SW_GIANTHOLE_01 = 0x8EF,
    SEQ_SE_SW_GIANTHOLE_02 = 0x8F0,
    SEQ_SE_SW_GIANTHOLE_03 = 0x8F1,
    SEQ_SE_SW_GIANTHOLE_04 = 0x8F2,
    SEQ_SE_SW_GIANTHOLE_05 = 0x8F3,
    SEQ_SE_SW_GIANTHOLE_06 = 0x8F4,
    SEQ_SE_SW_GIANTHOLE_07_1 = 0x8F5,
    SEQ_SE_SW_GIANTHOLE_07_2 = 0x8F6,
    SEQ_SE_SW_GIANTHOLE_07_3 = 0x8F7,
    SEQ_SE_SW_GIANTHOLE_07_4 = 0x8F8,
    SEQ_SE_SW_GIANTHOLE_08_1 = 0x8F9,
    SEQ_SE_SW_GIANTHOLE_08_2 = 0x8FA,
    SEQ_SE_SW_GIANTHOLE_08_3 = 0x8FB,
    SEQ_SE_SW_GIANTHOLE_08_4 = 0x8FC,
    SEQ_SE_SW_GIANTHOLE_09 = 0x8FD,
    SEQ_SE_SW_GIANTHOLE_10 = 0x8FE,
    SEQ_SE_SW_KUSABI_IN_01 = 0x8FF,
    SEQ_SE_SW_KUSABI_IN_02 = 0x900,
    SEQ_SE_SW_KUSABI_IN_03 = 0x901,
    SEQ_SE_SW_KUSABI_IN_04 = 0x902,
    SEQ_SE_SW_KUSABI_IN_05 = 0x903,
    SEQ_SE_SW_KUSABI_IN_06 = 0x904,
    SEQ_SE_SW_KUSABI_IN_07 = 0x905,
    SEQ_SE_SW_KUSABI_IN_08 = 0x906,
    SEQ_SE_SW_KUSABI_IN_09 = 0x907,
    SEQ_SE_SW_KUSABI_IN_10 = 0x908,
    SEQ_SE_SW_KUSABI_IN_11 = 0x909,
    SEQ_SE_SW_KUSABI_IN_12 = 0x90A,
    SEQ_SE_SW_KUSABI_IN_13 = 0x90B,
    SEQ_SE_SW_TACHIWAKI_01 = 0x90C,
    SEQ_SE_SW_TACHIWAKI_02 = 0x90D,
    SEQ_SE_SW_TACHIWAKI_03 = 0x90E,
    SEQ_SE_SW_AIRPLANE_01 = 0x90F,
    SEQ_SE_SW_AIRPLANE_02_L = 0x910,
    SEQ_SE_SW_AIRPLANE_02_R = 0x911,
    SEQ_SE_SW_AIRPLANE_03 = 0x912,
    SEQ_SE_SW_FRIGATE_01 = 0x913,
    SEQ_SE_SW_FRIGATE_02 = 0x914,
    SEQ_SE_SW_FRIGATE_03 = 0x915,
    SEQ_SE_SW_FRIGATE_04 = 0x916,
    SEQ_SE_SW_FRIGATE_05 = 0x917,
    SEQ_SE_SW_FRIGATE_06 = 0x918,
    SEQ_SE_SW_FRIGATE_07 = 0x919,
    SEQ_SE_SW_FRIGATE_08 = 0x91A,
    SEQ_SE_SW_FRIGATE_09 = 0x91B,
    SEQ_SE_SW_FRIGATE_10 = 0x91C,
    SEQ_SE_SW_FRIGATE_11 = 0x91D,
    SEQ_SE_SW_FRIGATE_12 = 0x91E,
    SEQ_SE_SW_FRIGATE_13 = 0x91F,
    SEQ_SE_SW_FRIGATE_14 = 0x920,
    SEQ_SE_SW_FRIGATE_15 = 0x921,
    SEQ_SE_SW_FRIGATE_16 = 0x922,
    SEQ_SE_SW_FRIGATE_17 = 0x923,
    SEQ_SE_SW_FRIGATE_18 = 0x924,
    SEQ_SE_SW_SOURYUCITY_01 = 0x925,
    SEQ_SE_SW_SOURYUCITY_02 = 0x926,
    SEQ_SE_SW_SOURYUCITY_03 = 0x927,
    SEQ_SE_SW_SOURYUCITY_04 = 0x928,
    SEQ_SE_SW_SOURYUCITY_05 = 0x929,
    SEQ_SE_SW_SOURYUCITY_06 = 0x92A,
    SEQ_SE_SW_SOURYUCITY_07 = 0x92B,
    SEQ_SE_SW_SOURYUCITY_08 = 0x92C,
    SEQ_SE_SW_SOURYUCITY_09 = 0x92D,
    SEQ_SE_SW_SOURYUCITY_10 = 0x92E,
    SEQ_SE_SW_SOURYUCITY_11 = 0x92F,
    SEQ_SE_SW_SOURYUCITY_12 = 0x930,
    SEQ_SE_SW_SOURYUCITY_13 = 0x931,
    SEQ_SE_SW_SOURYUCITY_14 = 0x932,
    SEQ_SE_SW_SOURYUCITY_15 = 0x933,
    SEQ_SE_SW_SOURYUCITY_16 = 0x934,
    SEQ_SE_SW_SOURYUCITY_17 = 0x935,
    SEQ_SE_SW_SOURYUCITY_18 = 0x936,
    SEQ_SE_SW_KYUREON_00_01 = 0x937,
    SEQ_SE_SW_KYUREON_00_02a = 0x938,
    SEQ_SE_SW_KYUREON_00_02b = 0x939,
    SEQ_SE_SW_KYUREON_00_02c = 0x93A,
    SEQ_SE_SW_KYUREON_00_02d = 0x93B,
    SEQ_SE_SW_KYUREON_00_03a = 0x93C,
    SEQ_SE_SW_KYUREON_00_03b = 0x93D,
    SEQ_SE_SW_KYUREON_01_01 = 0x93E,
    SEQ_SE_SW_KYUREON_01_01b = 0x93F,
    SEQ_SE_SW_KYUREON_01_02a = 0x940,
    SEQ_SE_SW_KYUREON_01_02b = 0x941,
    SEQ_SE_SW_KYUREON_01_03 = 0x942,
    SEQ_SE_SW_KYUREON_01_04 = 0x943,
    SEQ_SE_SW_KYUREON_01_05 = 0x944,
    SEQ_SE_SW_KYUREON_01_06 = 0x945,
    SEQ_SE_SW_KYUREON_01_07 = 0x946,
    SEQ_SE_SW_KYUREON_01_07b = 0x947,
    SEQ_SE_SW_KYUREON_01_08 = 0x948,
    SEQ_SE_SW_KYUREON_01_09 = 0x949,
    SEQ_SE_SW_KYUREON_01_10 = 0x94A,
    SEQ_SE_SW_KYUREON_01_11 = 0x94B,
    SEQ_SE_SW_KYUREON_01_12 = 0x94C,
    SEQ_SE_SW_KYUREON_01_13 = 0x94D,
    SEQ_SE_SW_KYUREON_01_14b = 0x94E,
    SEQ_SE_SW_KYUREON_01_14w = 0x94F,
    SEQ_SE_SW_KYUREON_01_15 = 0x950,
    SEQ_SE_SW_KYUREON_01_16 = 0x951,
    SEQ_SE_SW_KYUREON_02_01 = 0x952,
    SEQ_SE_SW_KYUREON_02_02 = 0x953,
    SEQ_SE_SW_KYUREON_02_03 = 0x954,
    SEQ_SE_SW_KYUREON_02_04 = 0x955,
    SEQ_SE_SW_KYUREON_02_05 = 0x956,
    SEQ_SE_SW_KYUREON_02_06 = 0x957,
    SEQ_SE_SW_KYUREON_02_06w = 0x958,
    SEQ_SE_SW_KYUREON_02_07 = 0x959,
    SEQ_SE_SW_KYUREON_02_08 = 0x95A,
    SEQ_SE_SW_KYUREON_02_09 = 0x95B,
    SEQ_SE_SW_KYUREON_02_10 = 0x95C,
    SEQ_SE_SW_KYUREON_03_01 = 0x95D,
    SEQ_SE_SW_KYUREON_03_02 = 0x95E,
    SEQ_SE_SW_KYUREON_03_03 = 0x95F,
    SEQ_SE_SW_KYUREON_03_04 = 0x960,
    SEQ_SE_SW_KYUREON_03_05 = 0x961,
    SEQ_SE_SW_KYUREON_03_06b = 0x962,
    SEQ_SE_SW_KYUREON_03_06w = 0x963,
    SEQ_SE_SW_KYUREON_03_07b = 0x964,
    SEQ_SE_SW_KYUREON_03_07w = 0x965,
    SEQ_SE_SW_ENDING_01 = 0x966,
    SEQ_SE_SW_ENDING_02 = 0x967,
    SEQ_SE_SW_ENDING_03 = 0x968,
    SEQ_SE_SW_CYGNUS_01 = 0x969,
    SEQ_SE_SW_CYGNUS_02 = 0x96A,
    SEQ_SE_SW_CYGNUS_03 = 0x96B,
    SEQ_SE_DMY_390 = 0x96C,
    SEQ_SE_DMY_391 = 0x96D,
    SEQ_SE_DP_SELECT = 0x96E,
    SEQ_SE_END = 0x96F,
    SEQ_INVALID = 0xFFFF,
};
struct ListMenuOption
{
    StrBuf* strBuf;
    int UID;
};

struct FieldScriptEnv;
struct BmpMenuListSetup
{
    ListMenuOption* Options;
    int field_4;
    int field_8;
    u16 OptionCount;
    u16 BlockIncrement;
    int field_10;
    u16 field_14;
    u16 field_16;
    FieldScriptEnv* Env;
    int field_1C;
    int field_20;
    void* field_24;
    int field_28;
    int field_2C;
    int field_30;
};

struct SWAN_ALIGNED(4) BmpMenuList
{
    BmpMenuListSetup Setup;
    _BYTE byte34;
    _BYTE byte35;
    _BYTE byte36;
    SWAN_ALIGNED(2) _BYTE byte38;
    _DWORD m_BmpCursor;
    unsigned __int16 FirstVisibleOptionIndex;
    unsigned __int16 RelativeIndex;
    _BYTE byte44;
    _BYTE byte45;
    _BYTE byte46;
    _BYTE byte47;
    _WORD word48;
    _BYTE ForbidCancel;
    unsigned __int8 unsigned___int84B;
};

struct ListMenuUI
{
    int dword0;
    int dword4;
    BmpWin* m_BmpWin;
    ListMenuOption* Options;
    BmpMenuList* m_BmpMenuList;
};

struct SWAN_ALIGNED(2) ListMenuWork
{
    u16 WindowX;
    u16 WindowY;
    u16 DefaultOptionIndex;
    u8 IsCancellable;
    u8 IsHorizontalAlignmentRight;
    u16* TargetWkAddr;
    WordSetSystem* FontInfo;
    int HasDefaultMessageData;
    MsgData* MessageData;
    ListMenuOption* Options;
    ListMenuUI* UI;
    StrBuf* Strbufs[32];
};

struct ElevatorTable
{
    u16 FloorNameMsgID;
    u16 ZoneID;
    u16 WarpX;
    u16 WarpY;
    u16 WarpZ;
};

struct ActionCmd
{
    u16 Action;
    u16 Amount;
};

struct FieldAcmdTCB
{
    u32 FuncIdx;
    int IsQueueEnded;
    int AmountCounter;
    FieldActor* Actor;
    ActionCmd* QueuePos;
    ActionCmd* Queue;
};

struct SWAN_ALIGNED(4) ActorMsgWinWork
{
    u16 ActorUID;
    u16 WinPosY;
    VecFx32 MsgAttachmentPosAbs;
    VecFx32 LastActorPos;
    VecFx32 MsgAttachmentPosRel;
};

struct SWAN_ALIGNED(4) AcmdTCBBuffer
{
    FieldAcmdTCB* TCBs[8];
};

struct ScriptWork;
struct ScriptSubwork
{
    ScriptWork* MainWork;
    GameSystem* m_GameSystem;
    GameData* m_GameData;
    FieldActorSystem* m_ActorSystem;
    TCB* PlayerGridEventTCB;
    AcmdTCBBuffer ActionTCBs;
    ListMenuWork m_ListMenuWork;
    ActorMsgWinWork ActorWork;
    int MapDisplayInfo;
    int SpecialMsgIconPtr;
    u8 ActorMsgWinPosActual;
    u8 ActorMsgWinPos;
    u16 PauseFramesCounter;
    int NowPkmVoice;
    ElevatorTable* ElevatorTableAddr;
};

struct StadiumTrainer
{
    u16 Group;
    u16 ID;
    u16 ObjCode;
    u16 TrainerID;
};

struct AnimCurve
{
    __int16 m_HeapID;
    s16 pad_HeapID;
    ArcTool* Arc;
    u32 ArcDatID;
    u32 KeyFrameCount;
    fx32 CurrentFrame;
    u32 CurrentPreloadFrame;
    u32 PreloadFramesCount;
    VecFx32* Translations;
    VecFx32* Rotations;
    VecFx32* Scales;
    b32 HasTranslation;
    b32 HasRotation;
    b32 HasScale;
    int Loaded;
};

struct SWAN_ALIGNED(4) FieldActorAnmProc
{
    FieldActor* Actor;
    VecFx32 AnmLoadBuffer[32];
    AnimCurve* animCurve;
    FieldAsyncProc* Proc;
    VecFx32 BaseTranslation;
    _BYTE gap198;
    u8 DatID;
    u8 IsPlaying;
    HeapID heapID;
    __int16 field_19E;
};

enum FieldBattleType
{
    FLD_BTLTYPE_SINGLE = 0x0,
    FLD_BTLTYPE_DOUBLE_SEPARATE = 0x1,
    FLD_BTLTYPE_DOUBLE_UNIFIED = 0x2,
    FLD_BTLTYPE_DOUBLE_MULTI = 0x3,
};

struct FieldScriptTrainerSetup
{
    FieldActor* TrainerActor;
    int ClashDistance;
    int ClashDirection;
    int SCRID;
    int TrainerID;
    FieldBattleType BattleType;
};

struct EventTrainerEye;
struct FieldScriptTrainerWork
{
    FieldScriptTrainerSetup m_Setup;
    EventTrainerEye* m_EventTrainerEye;
};

struct EventTrainerEye
{
    _DWORD State;
    b32 AllDone;
    FieldScriptTrainerWork m_ScriptTrainerWork;
    _DWORD m_TrainerSlot;
    u32 WaitCounter;
    FieldTerrainEffectTCB* m_TerrainEffectTCB;
    Field* m_Field;
    FieldPlayer* m_FieldPlayer;
};

struct ScrMapWk
{
    int MsgWork;
    Field* m_Field;
};

struct ScriptWork
{
    int Magic;
    u16 SCRID;
    u16 _PaddingSCRID;
    FieldActor* ExecutorActor;
    HeapID heapID;
    u16 _PaddingHeapID;
    GameSystem* GameSys;
    GameEvent* ScriptCallEvent;
    ScrMapWk MapWk;
    int field_20;
    int IsReducedFeatureSetVM;
    int CmdFeatureSetLevel;
    WordSetSystem* WordSetBuffers;
    StrBuf* Strbuf1;
    StrBuf* Strbuf2;
    int field_38;
    void* UserHeap;
    u8 SEBitMask;
    char field_41;
    __int16 field_42;
    FieldScriptTrainerWork TrainerWks[2];
    u16 ScriptVariables[1][98];
    FieldActorAnmProc* ActorAnimationWk;
    StadiumTrainer* StadiumTrainers;
    ScriptSubwork* SubWork;
};

struct SWAN_ALIGNED(4) FieldScriptEnv
{
    HeapID m_HeapID;
    u16 Padding1;
    u16 ZoneID;
    u16 SCRID;
    int CommandFeatureSetLevel;
    b32 IsReducedFeatureSetVM;
    ScriptWork* WorkEnv;
    MsgData* m_MsgData;
    u16 MsgFileNo;
    u8 VMIndex;
    char Padding2;
    u16* BackupVars;
    ScriptSubwork* SubWorkEnv;
};

enum PlayerSpecialState
{
    FLD_PLAYER_SPSTATE_NONE = 0x0,
    FLD_PLAYER_SPSTATE_CYCLING = 0x1,
    FLD_PLAYER_SPSTATE_SURF = 0x2,
    FLD_PLAYER_SPSTATE_DIVE = 0x3,
};

struct SWAN_ALIGNED(4) PlayerActionPerms
{
    char char0;
    _BYTE gap1[3];
    PlayerSpecialState playerSpecialState;
    u16 FollowFlagState;
    int field_C;
    int field_10;
    u8 ActionsBlocked[12];
};

struct NPCMdlInfo
{
    u16 UID;
    char EntityType;
    u8 SceneNodeType;
    char EnableShadow;
    char FootprintType;
    char EnableReflections;
    u8 BillboardSize;
    char SpriteAtlasSize;
    u8 SpriteControllerType;
    char Gender;
    u8 CollWidth;
    u8 CollHeight;
    char WPosOffsetX;
    char WPosOffsetY;
    char WPosOffsetZ;
    u16 RscIndices[5];
    u16 Padding;
};

#pragma pack(push, 1)
struct MMdlInfoCache
{
    u16 EntryCount;
    u16 EntryUIDs[26];
    u16 _Padding;
    NPCMdlInfo Cache[26];
};
#pragma pack(pop)

struct MoveModel;
struct SWAN_ALIGNED(4) MModelSystem
{
    u32 dword0;
    u16 ActorLimit;
    u16 ActorCount;
    s16 heapID;
    HeapID FieldHeapID;
    u16 wordC;
    u16 field_E;
    u32 dword10;
    ArcTool* DataArc;
    ArcTool* NPCSpriteIndexHandle;
    MoveModel* ActorHeap;
    void* TCBMgrHeap;
    TCBManager* TCBMgr;
    u32 field_28;
    u32 field_2C;
    void* MModelSave;
    GameData* gameData;
    FieldG3DMapper* G3DMapper;
    int NoGridMapper;
    Field* Fieldmap;
    void* field_44;
    void* field_48;
    MMdlInfoCache* mdlInfoCache;
};

struct MoveModel
{
    int Flags;
    int MovementFlags;
    u16 ActorUID;
    u16 ZoneID;
    u16 ModelID;
    u16 MoveCode;
    u16 EvType;
    u16 SpawnFlag;
    u16 SCRID;
    u16 DefaultDir;
    u16 FaceDir;
    u16 MotionDir;
    u16 LastFaceDir;
    u16 LastMotionDir;
    u16 Param0;
    u16 Param1;
    u16 Param2;
    u16 NextAcmd;
    s16 field_28;
    s16 field_2A;
    s16 AreaW;
    s16 AreaH;
    GPosXYZ DefaultGPos;
    GPosXYZ InitGPos;
    GPosXYZ GPos;
    s16 field_42;
    VecFx32 WPos;
    VecFx32 WPosOffset;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int CurrentTileUnder;
    int field_78;
    u8 CollisionWidth;
    u8 CollisionHeight;
    char ModelPosOffsetX;
    char ModelPosOffsetY;
    char ModelPosOffsetZ;
    char field_81;
    char field_82;
    char field_83;
    TCB* tCB;
    MModelSystem* mModelSystem;
    int MoveCodeExecutorFunction;
    int field_90;
    ActorPositionRail RailPosition;
    char gapA0[68];
    NPCMdlInfo MdlInfo;
};

struct SWAN_ALIGNED(4) PlayerActionPossibilities
{
    u16 ZoneID;
    u16 Flags;
    PlayerSpecialState playerSpecialState;
    GameSystem* GameSys;
    MoveModel* ActorInFront;
    Field* field;
};

enum FieldCommonEventID
{
    FLD_CMNEVENT_CYCLING = 0x0,
    FLD_CMNEVENT_ENTRALINK_WARP = 0x1,
    FLD_CMNEVENT_ESCAPE_ROPE = 0x2,
    FLD_CMNEVENT_SWEET_SCENT = 0x3,
    FLD_CMNEVENT_FISHING = 0x4,
    FLD_CMNEVENT_5 = 0x5,
};

enum FieldItemBlockReason : __int16
{
    FLD_ITEMBLOCK_GENERIC = 0x0,
    FLD_ITEMBLOCK_NO_BIKE_EXIT = 0x1,
    FLD_ITEMBLOCK_NO_RIBBON_POKE = 0x2,
    FLD_ITEMBLOCK_PAIR = 0x3,
};

enum ItemUseEventType
{
    ITEMUSE_EVENT_CHECKAGAIN = 0x0,
    ITEMUSE_EVENT_CANCEL = 0x1,
    ITEMUSE_EVENT_HIDEN = 0x2,
    ITEMUSE_EVENT_FIELD = 0x3,
};

struct AppCallFramework;
struct SWAN_ALIGNED(2) EventShortcutMenu
{
    GameEvent* ThisEvent;
    GameSystem* gameSystem;
    Field* field;
    int field_C;
    AppCallFramework* MenuWork;
    HeapID heapID;
    __int16 field_16;
    int field_18;
    int field_1C;
    int IsActionBlocked;
};


struct ShortcutMenuWork
{
    GameSystem* gameSystem;
    Field* field;
    GameEvent* ShortcutMenuEvent;
    int field_C;
    int field_10;
    int ItemID;
    int MenuRoutine1;
    int MenuRoutine2;
    int field_20;
    EventShortcutMenu* ShortcutMenuEventData;
    ItemUseEventType EventType;
    u32 EventID;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
};

struct AppCallFramework
{
    GameSystem* gameSystem;
    Field* field;
    GameEvent* ParentEvent;
    int field_C;
    int SubprocAppID;
    u32 SubprocParam;
    int MenuRoutine1;
    int MenuRoutine2;
    int field_20;
    void* ParentEventData;
    ItemUseEventType EventType;
    int EventID;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
};

enum InputButton
{
    KEY_A = 0x1,
    KEY_B = 0x2,
    KEY_SELECT = 0x4,
    KEY_START = 0x8,
    KEY_RIGHT = 0x10,
    KEY_LEFT = 0x20,
    KEY_UP = 0x40,
    KEY_DOWN = 0x80,
    KEY_R = 0x100,
    KEY_L = 0x200,
    KEY_X = 0x400,
    KEY_Y = 0x800,
    KEY_TOUCH = 0x1000,
    KEY_LID = 0x2000,
};

struct SWAN_ALIGNED(4) KeypadMgr
{
    int char0;
    int field_4;
    int field_8;
    InputButton LastHeldKeys;
    InputButton LastNewKeys;
    InputButton LastTypedKeys;
    InputButton HeldKeys60;
    InputButton NewKeys60;
    InputButton TypedKeys60;
    InputButton HeldKeys30;
    InputButton NewKeys30;
    InputButton TypedKeys30;
    InputButton HeldKeys2Frame;
    InputButton NewKeys2Frame;
    InputButton TypedKeys2Frame;
    int KeyRetypeCountdown;
    _DWORD KeyRetypeIntervalRepeated;
    _DWORD KeyRetypeIntervalFirst;
    int field_48;
};

struct SystemUI
{
    KeypadMgr* m_KeypadMgr;
    _DWORD Touchpad;
    void(* SoftResetCallback)(void*);
    void* SoftResetCallbackData;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int Callback;
    int field_24;
    int field_28;
    int field_2C;
    void* CallbackData;
    int field_34;
    char field_38;
    u8 SoftResetDisabled;
    char field_3A;
    u8 Backlight;
    _BYTE byte3C;
    u8 UpdateRate;
    u8 FrameCounter;
    _BYTE BatteryLevel;
    _BYTE byte40;
    char field_41;
    char field_42;
    char field_43;
};

enum FieldSubAppResult
{
    SUBAPP_RESULT_RETRY = 0x0,
    SUBAPP_RESULT_CANCEL = 0x1,
    SUBAPP_RESULT_USE_HIDEN = 0x2,
    SUBAPP_RESULT_USE_ITEM = 0x3,
    SUBAPP_RESULT_CALL_CHILD_APP = 0x4,
    SUBAPP_RESULT_5 = 0x5,
};

struct FieldAppCallParam
{
    b32(*field_0)(AppCallFramework*, EventShortcutMenu*);
    int field_4;
    int field_8;
    EventShortcutMenu* ParentEventData;
    AppCallFramework* FrameworkData;
};

struct SWAN_ALIGNED(4) EventFieldAppCall
{
    HeapID m_HeapID;
    __int16 field_2;
    u32 AppID;
    _DWORD LastAppID;
    _DWORD ChildAppID;
    FieldSubAppResult SubprocResult;
    GameEvent* ParentEvent;
    AppCallFramework* m_AppCallFramework;
    void* SubprocData;
    FieldAppCallParam m_Param;
    PlayerActionPerms m_ActionPerms;
    PlayerActionPossibilities m_ActionPossibilities;
    u8 field_68;
    char field_69;
    __int16 field_6A;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
};

struct SWAN_ALIGNED(4) RoamingPokemon
{
    u16 NowZoneID;
    unsigned __int8 Nature;
    _DWORD IVs;
    _DWORD PID;
    u16 Species;
    u16 CurrentHP;
    unsigned __int8 Level;
    unsigned __int8 unsigned___int811;
    unsigned __int8 Status;
    unsigned __int8 unsigned___int813;
};

struct SWAN_ALIGNED(2) EncountSave
{
    RoamingPokemon roamingPokemon[2];
    u8 RoamingPokemonZoneClock[2];
    u8 field_2A[2];
    _BYTE CurrentSwarmLocation;
    _BYTE RepelSteps;
    _WORD NowRepelItemID;
    int field_30;
};

struct FieldScriptSupervisor
{
    HeapID m_HeapID;
    char VMCount;
    char field_3;
    ScriptVM* VMs[3];
    int TerminatorRoutine;
    int TerminatorParam;
};

struct EventScriptCall
{
    ScriptWork* scriptWork;
    FieldScriptSupervisor* State;
};

enum FieldScriptFeatureLevel
{
    FLD_SCRIPT_FEATURE_EVENT = 0x0,
    FLD_SCRIPT_FEATURE_LOAD = 0x1,
    FLD_SCRIPT_FEATURE_INIT = 0x2,
};

enum ItemType
{
    ITEMTYPE_STD = 0x0,
    ITEMTYPE_EFFECT = 0x1,
    ITEMTYPE_TM = 0x2,
    ITEMTYPE_BERRY = 0x3,
    ITEMTYPE_KEY = 0x4,
    ITEMTYPE_FREE_SPACE = 0x5,
    ITEMTYPE_MAX = 0x6,
};

enum FieldmapCtrlType
{
    FLD_MAPCTRL_GRID = 0x0,
    FLD_MAPCTRL_RAIL = 0x1,
    FLD_MAPCTRL_HYBRID = 0x2,
};

enum ItemDataType
{
    ITEMDATA_PARAMS = 0x0,
    ITEMDATA_SPR1 = 0x1,
    ITEMDATA_SPR2 = 0x2,
    ITEMDATA_MAX = 0x3,
};

enum HidenEventID
{
    HIDENEVENT_CUT = 0x0,
    HIDENEVENT_SURF = 0x1,
    HIDENEVENT_WATERFALL = 0x2,
    HIDENEVENT_STRENGTH = 0x3,
    HIDENEVENT_FLY = 0x4,
    HIDENEVENT_FLASH = 0x5,
    HIDENEVENT_TELEPORT = 0x6,
    HIDENEVENT_8 = 0x7,
    HIDENEVENT_DIG = 0x7,
    HIDENEVENT_CHATOT = 0x9,
    HIDENEVENT_DIVING = 0xA,
    HIDENEVENT_MAX = 0xB,
};

enum FieldPlayerAction
{
    FLD_ACTION_CYCLING = 0x0,
    FLD_ACTION_ESCAPE_ROPE = 0x3,
    FLD_ACTION_SURF = 0x5,
};

enum RegistrableID
{
    REGIT_BIKE = 0,
    REGIT_TOWN_MAP = 1,
    REGIT_VS_RECORDER = 2,
    REGIT_PAL_PAD = 3,
    REGIT_SUPER_ROD = 4,
    REGIT_DOWSING_MCHN = 5,
    REGIT_GRACIDEA = 6,
    REGIT_DNA_SPLICERS_1 = 7,
    REGIT_DNA_SPLICERS_2 = 8,
    REGIT_POKEINFO_STATUS = 9,
    REGIT_POKEINFO_SKILLS = 10,
    REGIT_POKEINFO_RIBBONS = 11,
    REGIT_BAG_ITEMS = 12,
    REGIT_BAG_MEDICINE = 13,
    REGIT_BAG_TMS_HMS = 14,
    REGIT_BAG_BERRIES = 15,
    REGIT_BAG_KEYITEMS = 16,
    REGIT_BAG_FREESPACE = 17,
    REGIT_POKEDEX_LIST = 18,
    REGIT_POKEDEX_SEARCH = 19,
    REGIT_POKEDEX_INFO = 20,
    REGIT_POKEDEX_AREA = 21,
    REGIT_POKEDEX_CRY = 22,
    REGIT_POKEDEX_FORMS = 23,
    REGIT_TRAINERCARD_FRONT = 24,
    REGIT_TRAINERCARD_BACK = 25,
    REGIT_TRAINERCARD_BADGES = 26,
    REGIT_OPTIONS = 27,
    REGIT_XTRANSCEIVER = 28,
    REGIT_MEDAL_BOX = 29,
    REGIT_POKEMON_LIST = 30,
    REGIT_POKEDEXHABITAT_LIST = 31,
    REGIT_POKEDEXHABITAT_INFO = 32,
    REGIT_REVEAL_GLASS = 33,
    REGIT_UNUSED_34 = 34,
    REGIT_UNUSED_35 = 35,
    REGIT_UNUSED_36 = 36,
    REGIT_UNUSED_37 = 37,
    REGIT_UNUSED_38 = 38,
    REGIT_UNUSED_39 = 39,
    REGIT_UNUSED_40 = 40,
    REGIT_UNUSED_41 = 41,
    REGIT_UNUSED_42 = 42,
    REGIT_UNUSED_43 = 43,
    REGIT_UNUSED_44 = 44,
    REGIT_UNUSED_45 = 45,
    REGIT_UNUSED_46 = 46,
    REGIT_UNUSED_47 = 47,
    REGIT_UNUSED_48 = 48,
    REGIT_UNUSED_49 = 49,
    REGIT_UNUSED_50 = 50,
    REGIT_UNUSED_51 = 51,
    REGIT_UNUSED_52 = 52,
    REGIT_UNUSED_53 = 53,
    REGIT_UNUSED_54 = 54,
    REGIT_UNUSED_55 = 55,
    REGIT_UNUSED_56 = 56,
    REGIT_UNUSED_57 = 57,
    REGIT_UNUSED_58 = 58,
    REGIT_UNUSED_59 = 59,
    REGIT_UNUSED_60 = 60,
    REGIT_UNUSED_61 = 61,
    REGIT_UNUSED_62 = 62,
    REGIT_UNUSED_63 = 63,
    REGIT_NOT_REGISTRABLE
};

struct SWAN_ALIGNED(4) FieldCommonEvent
{
    GameEvent* (*function)(Field*, GameSystem*);
    int Param;
};

enum PocketType
{
    POCKETTYPE_ITEMS = 0x0,
    POCKETTYPE_MEDICINE = 0x1,
    POCKETTYPE_TM_HM = 0x2,
    POCKETTYPE_BERRIES = 0x3,
    POCKETTYPE_KEY_ITEMS = 0x4,
    POCKETTYPE_FREE_SPACE = 0x5,
    POCKETTYPE_MAX = 0x6,
};

enum ItemActionType
{
    ITEMACTION_MISC = 0x0,
    ITEMACTION_REPEL = 0x1,
    ITEMACTION_GENERIC = 0x2,
};

struct SWAN_ALIGNED(1) BigBagStructTemp
{
    GameData* gameData;
    BagSaveData* bagSaveData_4;
    void* void_ptr_8;
    PlayerActionPerms* playerActionPerms;
    int field_10;
    int field_14;
    BagSaveData* bagSaveData;
    __int64 field_1C;
    BagItem bagitem_ptrs_24[3];
    char field_30[1228];
    int(*function_4FC)(int);
    _BYTE* u8_ptr_500;
    __int16 field_504;
    char field_506[10];
    int field_510;
    _DWORD* u32_ptr_514;
    int field_518;
    int field_51C;
    MsgData* msgData;
    WordSetSystem* wordSetSystem;
    StrBuf* strBuf1;
    StrBuf* strBuf2;
    StrBuf* strBuf_530;
    int int_534;
    char field_538[20];
    HeapID heapID;
    char field_54E[342];
    _DWORD* dword_ptr_6A4;
    TCB* tcb_6A8;
    char field_6AC[12];
    u32 u32_6B8;
    char field_6BC[24];
    int array_6D4[3];
    int array_6E0[17];
    char field_724[16];
    int int_734;
    char field_738[16];
    TCBManagerEx* tCBManagerEx;
    unsigned __int8* u8_ptr_74C;
    char field_750[76];
    u32* buttonClickRelated;
    __int16 field_7A0;
    char field_7A2[93];
    char field_7FF;
    int field_800;
    char field_804[40];
    PocketType pocketType_82C;
    u32 u32_830;
    u32 u32_834;
    unsigned int* ptr_838;
    char field_83C[12];
    int BIAArraySizeUnkn[20];
    u32 u32_898;
    int bagItemActions;
    ItemID itemID_8A0;
    char field_8A4[8];
    bool bool_8AC;
    char field_8B0[16];
    int field_8C0;
    int field_8C4;
    int field_8C8;
    char field_8CC[456];
    int itemID_A94;
    char field_A98[5304];
    int field_1F50;
};

struct ItemUseFunction {
    short itemID;
    short unkn; //always 1
    int(*function)(int a1);
};

enum ActionID
{
    ACTID_BIKE = 0x0,
    ACTID_TOWN_MAP = 0x1,
    ACTID_PAL_PAD = 0x2,
    ACTID_ESCAPE_ROPE = 0x3,
    ACTID_UKN_4 = 0x4,
    ACTID_FISHING = 0x5,
    ACTID_VS_RECORDER = 0x6,
    ACTID_UKN_7 = 0x4,
    ACTID_UKN_8 = 0x4,
    ACTID_DOWSING_MCHN = 0x9,
    ACTID_XTRANSCEIVER = 0x10,
    ACTID_MEDAL_BOX = 0x11,
    ACTID_WARP_SWEETSCENT = 0xFF,
};

struct SWAN_ALIGNED(4) EventFieldToggle
{
    GameSystem* m_GameSystem;
    Field* m_Field;
};

enum GenderSet
{
    GENDER_MALE = 0x0,
    GENDER_FEMALE = 0x1,
    GENDER_EITHER = 0x2,
    GENDER_MAX = 0x3,
};

enum PokeType
{
    TYPE_NORMAL = 0x0,
    TYPE_FIGHT = 0x1,
    TYPE_FLY = 0x2,
    TYPE_POISON = 0x3,
    TYPE_GROUND = 0x4,
    TYPE_ROCK = 0x5,
    TYPE_BUG = 0x6,
    TYPE_GHOST = 0x7,
    TYPE_STEEL = 0x8,
    TYPE_FIRE = 0x9,
    TYPE_WATER = 0xA,
    TYPE_GRASS = 0xB,
    TYPE_ELEC = 0xC,
    TYPE_PSY = 0xD,
    TYPE_ICE = 0xE,
    TYPE_DRAGON = 0xF,
    TYPE_DARK = 0x10,
    TYPE_NULL = 0x11
};

enum PersonalField
{
    Personal_HP = 0x0,
    Personal_ATK = 0x1,
    Personal_DEF = 0x2,
    Personal_SPE = 0x3,
    Personal_SPA = 0x4,
    Personal_SPD = 0x5,
    Personal_Type1 = 0x6,
    Personal_Type2 = 0x7,
    Personal_CaptureRate = 0x8,
    Personal_BaseEXP = 0x9,
    Personal_EvHP = 0xA,
    Personal_EvATK = 0xB,
    Personal_EvDEF = 0xC,
    Personal_EvSPE = 0xD,
    Personal_EvSPA = 0xE,
    Personal_EvSPD = 0xF,
    Personal_Telekinesis = 0x10,
    Personal_WildItem50 = 0x11,
    Personal_WildItem5 = 0x12,
    Personal_WildItem1 = 0x13,
    Personal_GenderProb = 0x14,
    Personal_EggHappiness = 0x15,
    Personal_BaseHappiness = 0x16,
    Personal_GrowthRate = 0x17,
    Personal_EggGroup1 = 0x18,
    Personal_EggGroup2 = 0x19,
    Personal_Abil1 = 0x1A,
    Personal_Abil2 = 0x1B,
    Personal_AbilH = 0x1C,
    Personal_EscapeRate = 0x1D,
    Personal_FormeDataOffs = 0x1E,
    Personal_FormeSpritesOffset = 0x1F,
    Personal_FormeCount = 0x20,
    Personal_Color = 0x21,
    Personal_SpriteFlip = 0x22,
    Personal_SpriteForme = 0x23,
    Personal_EvoStage = 0x24,
    Personal_Height = 0x25,
    Personal_Weight = 0x26,
    Personal_TMHM1 = 0x27,
    Personal_TMHM2 = 0x28,
    Personal_TMHM3 = 0x29,
    Personal_TMHM4 = 0x2A,
    Personal_TypeTutor = 0x2B,
    Personal_SpecialTutor1 = 0x2C,
    Personal_SpecialTutor2 = 0x2D,
    Personal_SpecialTutor3 = 0x2E,
    Personal_SpecialTutor4 = 0x2F,
};

struct BGSysLCDConfig
{
    G2DDisplayMode DisplayModeA;
    G2DBGMode BGModeA;
    G2DBGMode BGModeB;
    b32 IsBG03D;
};

struct PersonalData
{
    u8 BaseHP;
    u8 BaseATK;
    u8 BaseDEF;
    u8 BaseSPE;
    u8 BaseSPA;
    u8 BaseSPD;
    u8 Type1;
    u8 Type2;
    u8 CaptureRate;
    u8 EvoStage;
    u16 EVYield;
    u16 WildItem50;
    u16 WildItem5;
    u16 WildItem1;
    u8 GenderProb;
    u8 EggHappiness;
    u8 BaseHappiness;
    u8 ExpGroup;
    u8 EggGroup1;
    u8 EggGroup2;
    u8 Abil1;
    u8 Abil2;
    u8 AbilHidden;
    u8 EscapeRate;
    u16 FormeDataOffs;
    u16 FormeSpriteOffs;
    u8 FormeCount;
    u8 Color;
    u16 BaseEXP;
    u16 HeightCm;
    u16 WeightCg;
    int TMHM1;
    int TMHM2;
    int TMHM3;
    int TMHM4;
    int TypeTutors;
    int SpecialTutors[4];
};

enum AbilID
{
    ABIL_NULL = 0x0,
    ABIL001_STENCH = 0x1,
    ABIL002_DRIZZLE = 0x2,
    ABIL003_SPEED_BOOST = 0x3,
    ABIL004_BATTLE_ARMOR = 0x4,
    ABIL005_STURDY = 0x5,
    ABIL006_DAMP = 0x6,
    ABIL007_LIMBER = 0x7,
    ABIL008_SAND_VEIL = 0x8,
    ABIL009_STATIC = 0x9,
    ABIL010_VOLT_ABSORB = 0xA,
    ABIL011_WATER_ABSORB = 0xB,
    ABIL012_OBLIVIOUS = 0xC,
    ABIL013_CLOUD_NINE = 0xD,
    ABIL014_COMPOUND_EYES = 0xE,
    ABIL015_INSOMNIA = 0xF,
    ABIL016_COLOR_CHANGE = 0x10,
    ABIL017_IMMUNITY = 0x11,
    ABIL018_FLASH_FIRE = 0x12,
    ABIL019_SHIELD_DUST = 0x13,
    ABIL020_OWN_TEMPO = 0x14,
    ABIL021_SUCTION_CUPS = 0x15,
    ABIL022_INTIMIDATE = 0x16,
    ABIL023_SHADOW_TAG = 0x17,
    ABIL024_ROUGH_SKIN = 0x18,
    ABIL025_WONDER_GUARD = 0x19,
    ABIL026_LEVITATE = 0x1A,
    ABIL027_EFFECT_SPORE = 0x1B,
    ABIL028_SYNCHRONIZE = 0x1C,
    ABIL029_CLEAR_BODY = 0x1D,
    ABIL030_NATURAL_CURE = 0x1E,
    ABIL031_LIGHTNING_ROD = 0x1F,
    ABIL032_SERENE_GRACE = 0x20,
    ABIL033_SWIFT_SWIM = 0x21,
    ABIL034_CHLOROPHYLL = 0x22,
    ABIL035_ILLUMINATE = 0x23,
    ABIL036_TRACE = 0x24,
    ABIL037_HUGE_POWER = 0x25,
    ABIL038_POISON_POINT = 0x26,
    ABIL039_INNER_FOCUS = 0x27,
    ABIL040_MAGMA_ARMOR = 0x28,
    ABIL041_WATER_VEIL = 0x29,
    ABIL042_MAGNET_PULL = 0x2A,
    ABIL043_SOUNDPROOF = 0x2B,
    ABIL044_RAIN_DISH = 0x2C,
    ABIL045_SAND_STREAM = 0x2D,
    ABIL046_PRESSURE = 0x2E,
    ABIL047_THICK_FAT = 0x2F,
    ABIL048_EARLY_BIRD = 0x30,
    ABIL049_FLAME_BODY = 0x31,
    ABIL050_RUN_AWAY = 0x32,
    ABIL051_KEEN_EYE = 0x33,
    ABIL052_HYPER_CUTTER = 0x34,
    ABIL053_PICKUP = 0x35,
    ABIL054_TRUANT = 0x36,
    ABIL055_HUSTLE = 0x37,
    ABIL056_CUTE_CHARM = 0x38,
    ABIL057_PLUS = 0x39,
    ABIL058_MINUS = 0x3A,
    ABIL059_FORECAST = 0x3B,
    ABIL060_STICKY_HOLD = 0x3C,
    ABIL061_SHED_SKIN = 0x3D,
    ABIL062_GUTS = 0x3E,
    ABIL063_MARVEL_SCALE = 0x3F,
    ABIL064_LIQUID_OOZE = 0x40,
    ABIL065_OVERGROW = 0x41,
    ABIL066_BLAZE = 0x42,
    ABIL067_TORRENT = 0x43,
    ABIL068_SWARM = 0x44,
    ABIL069_ROCK_HEAD = 0x45,
    ABIL070_DROUGHT = 0x46,
    ABIL071_ARENA_TRAP = 0x47,
    ABIL072_VITAL_SPIRIT = 0x48,
    ABIL073_WHITE_SMOKE = 0x49,
    ABIL074_PURE_POWER = 0x4A,
    ABIL075_SHELL_ARMOR = 0x4B,
    ABIL076_AIR_LOCK = 0x4C,
    ABIL077_TANGLED_FEET = 0x4D,
    ABIL078_MOTOR_DRIVE = 0x4E,
    ABIL079_RIVALRY = 0x4F,
    ABIL080_STEADFAST = 0x50,
    ABIL081_SNOW_CLOAK = 0x51,
    ABIL082_GLUTTONY = 0x52,
    ABIL083_ANGER_POINT = 0x53,
    ABIL084_UNBURDEN = 0x54,
    ABIL085_HEATPROOF = 0x55,
    ABIL086_SIMPLE = 0x56,
    ABIL087_DRY_SKIN = 0x57,
    ABIL088_DOWNLOAD = 0x58,
    ABIL089_IRON_FIST = 0x59,
    ABIL090_POISON_HEAL = 0x5A,
    ABIL091_ADAPTABILITY = 0x5B,
    ABIL092_SKILL_LINK = 0x5C,
    ABIL093_HYDRATION = 0x5D,
    ABIL094_SOLAR_POWER = 0x5E,
    ABIL095_QUICK_FEET = 0x5F,
    ABIL096_NORMALIZE = 0x60,
    ABIL097_SNIPER = 0x61,
    ABIL098_MAGIC_GUARD = 0x62,
    ABIL099_NO_GUARD = 0x63,
    ABIL100_STALL = 0x64,
    ABIL101_TECHNICIAN = 0x65,
    ABIL102_LEAF_GUARD = 0x66,
    ABIL103_KLUTZ = 0x67,
    ABIL104_MOLD_BREAKER = 0x68,
    ABIL105_SUPER_LUCK = 0x69,
    ABIL106_AFTERMATH = 0x6A,
    ABIL107_ANTICIPATION = 0x6B,
    ABIL108_FOREWARN = 0x6C,
    ABIL109_UNAWARE = 0x6D,
    ABIL110_TINTED_LENS = 0x6E,
    ABIL111_FILTER = 0x6F,
    ABIL112_SLOW_START = 0x70,
    ABIL113_SCRAPPY = 0x71,
    ABIL114_STORM_DRAIN = 0x72,
    ABIL115_ICE_BODY = 0x73,
    ABIL116_SOLID_ROCK = 0x74,
    ABIL117_SNOW_WARNING = 0x75,
    ABIL118_HONEY_GATHER = 0x76,
    ABIL119_FRISK = 0x77,
    ABIL120_RECKLESS = 0x78,
    ABIL121_MULTITYPE = 0x79,
    ABIL122_FLOWER_GIFT = 0x7A,
    ABIL123_BAD_DREAMS = 0x7B,
    ABIL124_PICKPOCKET = 0x7C,
    ABIL125_SHEER_FORCE = 0x7D,
    ABIL126_CONTRARY = 0x7E,
    ABIL127_UNNERVE = 0x7F,
    ABIL128_DEFIANT = 0x80,
    ABIL129_DEFEATIST = 0x81,
    ABIL130_CURSED_BODY = 0x82,
    ABIL131_HEALER = 0x83,
    ABIL132_FRIEND_GUARD = 0x84,
    ABIL133_WEAK_ARMOR = 0x85,
    ABIL134_HEAVY_METAL = 0x86,
    ABIL135_LIGHT_METAL = 0x87,
    ABIL136_MULTISCALE = 0x88,
    ABIL137_TOXIC_BOOST = 0x89,
    ABIL138_FLARE_BOOST = 0x8A,
    ABIL139_HARVEST = 0x8B,
    ABIL140_TELEPATHY = 0x8C,
    ABIL141_MOODY = 0x8D,
    ABIL142_OVERCOAT = 0x8E,
    ABIL143_POISON_TOUCH = 0x8F,
    ABIL144_REGENERATOR = 0x90,
    ABIL145_BIG_PECKS = 0x91,
    ABIL146_SAND_RUSH = 0x92,
    ABIL147_WONDER_SKIN = 0x93,
    ABIL148_ANALYTIC = 0x94,
    ABIL149_ILLUSION = 0x95,
    ABIL150_IMPOSTER = 0x96,
    ABIL151_INFILTRATOR = 0x97,
    ABIL152_MUMMY = 0x98,
    ABIL153_MOXIE = 0x99,
    ABIL154_JUSTIFIED = 0x9A,
    ABIL155_RATTLED = 0x9B,
    ABIL156_MAGIC_BOUNCE = 0x9C,
    ABIL157_SAP_SIPPER = 0x9D,
    ABIL158_PRANKSTER = 0x9E,
    ABIL159_SAND_FORCE = 0x9F,
    ABIL160_IRON_BARBS = 0xA0,
    ABIL161_ZEN_MODE = 0xA1,
    ABIL162_VICTORY_STAR = 0xA2,
    ABIL163_TURBOBLAZE = 0xA3,
    ABIL164_TERAVOLT = 0xA4,
    ABIL165_AROMA_VEIL = 0xA5,
    ABIL166_FLOWER_VEIL = 0xA6,
    ABIL167_CHEEK_POUCH = 0xA7,
    ABIL168_PROTEAN = 0xA8,
    ABIL169_FUR_COAT = 0xA9,
    ABIL170_MAGICIAN = 0xAA,
    ABIL171_BULLETPROOF = 0xAB,
    ABIL172_COMPETITIVE = 0xAC,
    ABIL173_STRONG_JAW = 0xAD,
    ABIL174_REFRIGERATE = 0xAE,
    ABIL175_SWEET_VEIL = 0xAF,
    ABIL176_STANCE_CHANGE = 0xB0,
    ABIL177_GALE_WINGS = 0xB1,
    ABIL178_MEGA_LAUNCHER = 0xB2,
    ABIL179_GRASS_PELT = 0xB3,
    ABIL180_SYMBIOSIS = 0xB4,
    ABIL181_TOUGH_CLAWS = 0xB5,
    ABIL182_PIXILATE = 0xB6,
    ABIL183_GOOEY = 0xB7,
    ABIL184_AERILATE = 0xB8,
    ABIL185_PARENTAL_BOND = 0xB9,
    ABIL186_DARK_AURA = 0xBA,
    ABIL187_FAIRY_AURA = 0xBB,
    ABIL188_AURA_BREAK = 0xBC,
    ABIL189_PRIMORDIAL_SEA = 0xBD,
    ABIL190_DESOLATE_LAND = 0xBE,
    ABIL191_DELTA_STREAM = 0xBF,
};
   
typedef void(*BattleEventHandler)(BattleEventItem* item, ServerFlow* ServerFlow, int pokemonID, int* work);

struct BattleEventHandlerTableEntry
{
    BattleEventType EventType;
    BattleEventHandler Handler;
};

typedef BattleEventHandlerTableEntry* (*ItemEventAddFunc)(int*);
struct ItemEventAddTable
{
    int itemID;
    ItemEventAddFunc func;
};

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

enum StatStage
{
    STATSTAGE_NULL = 0x0,
    STATSTAGE_ATTACK = 0x1,
    STATSTAGE_DEFENSE = 0x2,
    STATSTAGE_SPECIAL_ATTACK = 0x3,
    STATSTAGE_SPECIAL_DEFENSE = 0x4,
    STATSTAGE_SPEED = 0x5,
    STATSTAGE_ACCURACY = 0x6,
    STATSTAGE_EVASION = 0x7,
};

struct HandlerParam_ChangeStatStage
{
    HandlerParam_Header header;
    StatStage rankType;
    u32 pad;
    s8 rankVolume;
    u8 pad2;
    u8 fMoveAnimation;
    u8 poke_cnt;
    u8 pokeID[6];
    HandlerParam_StrParams exStr;
};

enum TypeEffectiveness
{
    EFFECTIVENESS_IMMUNE = 0x0,
    EFFECTIVENESS_1_4 = 0x1,
    EFFECTIVENESS_1_2 = 0x2,
    EFFECTIVENESS_1 = 0x3,
    EFFECTIVENESS_2 = 0x4,
    EFFECTIVENESS_4 = 0x5,
};

enum MoveID
{
    MOVE_NULL = 0x0,
    MOVE001_POUND = 0x1,
    MOVE002_KARATE_CHOP = 0x2,
    MOVE003_DOUBLE_SLAP = 0x3,
    MOVE004_COMET_PUNCH = 0x4,
    MOVE005_MEGA_PUNCH = 0x5,
    MOVE006_PAY_DAY = 0x6,
    MOVE007_FIRE_PUNCH = 0x7,
    MOVE008_ICE_PUNCH = 0x8,
    MOVE009_THUNDER_PUNCH = 0x9,
    MOVE010_SCRATCH = 0xA,
    MOVE011_VICE_GRIP = 0xB,
    MOVE012_GUILLOTINE = 0xC,
    MOVE013_RAZOR_WIND = 0xD,
    MOVE014_SWORDS_DANCE = 0xE,
    MOVE015_CUT = 0xF,
    MOVE016_GUST = 0x10,
    MOVE017_WING_ATTACK = 0x11,
    MOVE018_WHIRLWIND = 0x12,
    MOVE019_FLY = 0x13,
    MOVE020_BIND = 0x14,
    MOVE021_SLAM = 0x15,
    MOVE022_VINE_WHIP = 0x16,
    MOVE023_STOMP = 0x17,
    MOVE024_DOUBLE_KICK = 0x18,
    MOVE025_MEGA_KICK = 0x19,
    MOVE026_JUMP_KICK = 0x1A,
    MOVE027_ROLLING_KICK = 0x1B,
    MOVE028_SAND_ATTACK = 0x1C,
    MOVE029_HEADBUTT = 0x1D,
    MOVE030_HORN_ATTACK = 0x1E,
    MOVE031_FURY_ATTACK = 0x1F,
    MOVE032_HORN_DRILL = 0x20,
    MOVE033_TACKLE = 0x21,
    MOVE034_BODY_SLAM = 0x22,
    MOVE035_WRAP = 0x23,
    MOVE036_TAKE_DOWN = 0x24,
    MOVE037_THRASH = 0x25,
    MOVE038_DOUBLE_EDGE = 0x26,
    MOVE039_TAIL_WHIP = 0x27,
    MOVE040_POISON_STING = 0x28,
    MOVE041_TWINEEDLE = 0x29,
    MOVE042_PIN_MISSILE = 0x2A,
    MOVE043_LEER = 0x2B,
    MOVE044_BITE = 0x2C,
    MOVE045_GROWL = 0x2D,
    MOVE046_ROAR = 0x2E,
    MOVE047_SING = 0x2F,
    MOVE048_SUPERSONIC = 0x30,
    MOVE049_SONIC_BOOM = 0x31,
    MOVE050_DISABLE = 0x32,
    MOVE051_ACID = 0x33,
    MOVE052_EMBER = 0x34,
    MOVE053_FLAMETHROWER = 0x35,
    MOVE054_MIST = 0x36,
    MOVE055_WATER_GUN = 0x37,
    MOVE056_HYDRO_PUMP = 0x38,
    MOVE057_SURF = 0x39,
    MOVE058_ICE_BEAM = 0x3A,
    MOVE059_BLIZZARD = 0x3B,
    MOVE060_PSYBEAM = 0x3C,
    MOVE061_BUBBLE_BEAM = 0x3D,
    MOVE062_AURORA_BEAM = 0x3E,
    MOVE063_HYPER_BEAM = 0x3F,
    MOVE064_PECK = 0x40,
    MOVE065_DRILL_PECK = 0x41,
    MOVE066_SUBMISSION = 0x42,
    MOVE067_LOW_KICK = 0x43,
    MOVE068_COUNTER = 0x44,
    MOVE069_SEISMIC_TOSS = 0x45,
    MOVE070_STRENGTH = 0x46,
    MOVE071_ABSORB = 0x47,
    MOVE072_MEGA_DRAIN = 0x48,
    MOVE073_LEECH_SEED = 0x49,
    MOVE074_GROWTH = 0x4A,
    MOVE075_RAZOR_LEAF = 0x4B,
    MOVE076_SOLAR_BEAM = 0x4C,
    MOVE077_POISON_POWDER = 0x4D,
    MOVE078_STUN_SPORE = 0x4E,
    MOVE079_SLEEP_POWDER = 0x4F,
    MOVE080_PETAL_DANCE = 0x50,
    MOVE081_STRING_SHOT = 0x51,
    MOVE082_DRAGON_RAGE = 0x52,
    MOVE083_FIRE_SPIN = 0x53,
    MOVE084_THUNDER_SHOCK = 0x54,
    MOVE085_THUNDERBOLT = 0x55,
    MOVE086_THUNDER_WAVE = 0x56,
    MOVE087_THUNDER = 0x57,
    MOVE088_ROCK_THROW = 0x58,
    MOVE089_EARTHQUAKE = 0x59,
    MOVE090_FISSURE = 0x5A,
    MOVE091_DIG = 0x5B,
    MOVE092_TOXIC = 0x5C,
    MOVE093_CONFUSION = 0x5D,
    MOVE094_PSYCHIC = 0x5E,
    MOVE095_HYPNOSIS = 0x5F,
    MOVE096_MEDITATE = 0x60,
    MOVE097_AGILITY = 0x61,
    MOVE098_QUICK_ATTACK = 0x62,
    MOVE099_RAGE = 0x63,
    MOVE100_TELEPORT = 0x64,
    MOVE101_NIGHT_SHADE = 0x65,
    MOVE102_MIMIC = 0x66,
    MOVE103_SCREECH = 0x67,
    MOVE104_DOUBLE_TEAM = 0x68,
    MOVE105_RECOVER = 0x69,
    MOVE106_HARDEN = 0x6A,
    MOVE107_MINIMIZE = 0x6B,
    MOVE108_SMOKESCREEN = 0x6C,
    MOVE109_CONFUSE_RAY = 0x6D,
    MOVE110_WITHDRAW = 0x6E,
    MOVE111_DEFENSE_CURL = 0x6F,
    MOVE112_BARRIER = 0x70,
    MOVE113_LIGHT_SCREEN = 0x71,
    MOVE114_HAZE = 0x72,
    MOVE115_REFLECT = 0x73,
    MOVE116_FOCUS_ENERGY = 0x74,
    MOVE117_BIDE = 0x75,
    MOVE118_METRONOME = 0x76,
    MOVE119_MIRROR_MOVE = 0x77,
    MOVE120_SELF_DESTRUCT = 0x78,
    MOVE121_EGG_BOMB = 0x79,
    MOVE122_LICK = 0x7A,
    MOVE123_SMOG = 0x7B,
    MOVE124_SLUDGE = 0x7C,
    MOVE125_BONE_CLUB = 0x7D,
    MOVE126_FIRE_BLAST = 0x7E,
    MOVE127_WATERFALL = 0x7F,
    MOVE128_CLAMP = 0x80,
    MOVE129_SWIFT = 0x81,
    MOVE130_SKULL_BASH = 0x82,
    MOVE131_SPIKE_CANNON = 0x83,
    MOVE132_CONSTRICT = 0x84,
    MOVE133_AMNESIA = 0x85,
    MOVE134_KINESIS = 0x86,
    MOVE135_SOFT_BOILED = 0x87,
    MOVE136_HIGH_JUMP_KICK = 0x88,
    MOVE137_GLARE = 0x89,
    MOVE138_DREAM_EATER = 0x8A,
    MOVE139_POISON_GAS = 0x8B,
    MOVE140_BARRAGE = 0x8C,
    MOVE141_LEECH_LIFE = 0x8D,
    MOVE142_LOVELY_KISS = 0x8E,
    MOVE143_SKY_ATTACK = 0x8F,
    MOVE144_TRANSFORM = 0x90,
    MOVE145_BUBBLE = 0x91,
    MOVE146_DIZZY_PUNCH = 0x92,
    MOVE147_SPORE = 0x93,
    MOVE148_FLASH = 0x94,
    MOVE149_PSYWAVE = 0x95,
    MOVE150_SPLASH = 0x96,
    MOVE151_ACID_ARMOR = 0x97,
    MOVE152_CRABHAMMER = 0x98,
    MOVE153_EXPLOSION = 0x99,
    MOVE154_FURY_SWIPES = 0x9A,
    MOVE155_BONEMERANG = 0x9B,
    MOVE156_REST = 0x9C,
    MOVE157_ROCK_SLIDE = 0x9D,
    MOVE158_HYPER_FANG = 0x9E,
    MOVE159_SHARPEN = 0x9F,
    MOVE160_CONVERSION = 0xA0,
    MOVE161_TRI_ATTACK = 0xA1,
    MOVE162_SUPER_FANG = 0xA2,
    MOVE163_SLASH = 0xA3,
    MOVE164_SUBSTITUTE = 0xA4,
    MOVE165_STRUGGLE = 0xA5,
    MOVE166_SKETCH = 0xA6,
    MOVE167_TRIPLE_KICK = 0xA7,
    MOVE168_THIEF = 0xA8,
    MOVE169_SPIDER_WEB = 0xA9,
    MOVE170_MIND_READER = 0xAA,
    MOVE171_NIGHTMARE = 0xAB,
    MOVE172_FLAME_WHEEL = 0xAC,
    MOVE173_SNORE = 0xAD,
    MOVE174_CURSE = 0xAE,
    MOVE175_FLAIL = 0xAF,
    MOVE176_CONVERSION_2 = 0xB0,
    MOVE177_AEROBLAST = 0xB1,
    MOVE178_COTTON_SPORE = 0xB2,
    MOVE179_REVERSAL = 0xB3,
    MOVE180_SPITE = 0xB4,
    MOVE181_POWDER_SNOW = 0xB5,
    MOVE182_PROTECT = 0xB6,
    MOVE183_MACH_PUNCH = 0xB7,
    MOVE184_SCARY_FACE = 0xB8,
    MOVE185_FEINT_ATTACK = 0xB9,
    MOVE186_SWEET_KISS = 0xBA,
    MOVE187_BELLY_DRUM = 0xBB,
    MOVE188_SLUDGE_BOMB = 0xBC,
    MOVE189_MUD_SLAP = 0xBD,
    MOVE190_OCTAZOOKA = 0xBE,
    MOVE191_SPIKES = 0xBF,
    MOVE192_ZAP_CANNON = 0xC0,
    MOVE193_FORESIGHT = 0xC1,
    MOVE194_DESTINY_BOND = 0xC2,
    MOVE195_PERISH_SONG = 0xC3,
    MOVE196_ICY_WIND = 0xC4,
    MOVE197_DETECT = 0xC5,
    MOVE198_BONE_RUSH = 0xC6,
    MOVE199_LOCK_ON = 0xC7,
    MOVE200_OUTRAGE = 0xC8,
    MOVE201_SANDSTORM = 0xC9,
    MOVE202_GIGA_DRAIN = 0xCA,
    MOVE203_ENDURE = 0xCB,
    MOVE204_CHARM = 0xCC,
    MOVE205_ROLLOUT = 0xCD,
    MOVE206_FALSE_SWIPE = 0xCE,
    MOVE207_SWAGGER = 0xCF,
    MOVE208_MILK_DRINK = 0xD0,
    MOVE209_SPARK = 0xD1,
    MOVE210_FURY_CUTTER = 0xD2,
    MOVE211_STEEL_WING = 0xD3,
    MOVE212_MEAN_LOOK = 0xD4,
    MOVE213_ATTRACT = 0xD5,
    MOVE214_SLEEP_TALK = 0xD6,
    MOVE215_HEAL_BELL = 0xD7,
    MOVE216_RETURN = 0xD8,
    MOVE217_PRESENT = 0xD9,
    MOVE218_FRUSTRATION = 0xDA,
    MOVE219_SAFEGUARD = 0xDB,
    MOVE220_PAIN_SPLIT = 0xDC,
    MOVE221_SACRED_FIRE = 0xDD,
    MOVE222_MAGNITUDE = 0xDE,
    MOVE223_DYNAMIC_PUNCH = 0xDF,
    MOVE224_MEGAHORN = 0xE0,
    MOVE225_DRAGON_BREATH = 0xE1,
    MOVE226_BATON_PASS = 0xE2,
    MOVE227_ENCORE = 0xE3,
    MOVE228_PURSUIT = 0xE4,
    MOVE229_RAPID_SPIN = 0xE5,
    MOVE230_SWEET_SCENT = 0xE6,
    MOVE231_IRON_TAIL = 0xE7,
    MOVE232_METAL_CLAW = 0xE8,
    MOVE233_VITAL_THROW = 0xE9,
    MOVE234_MORNING_SUN = 0xEA,
    MOVE235_SYNTHESIS = 0xEB,
    MOVE236_MOONLIGHT = 0xEC,
    MOVE237_HIDDEN_POWER = 0xED,
    MOVE238_CROSS_CHOP = 0xEE,
    MOVE239_TWISTER = 0xEF,
    MOVE240_RAIN_DANCE = 0xF0,
    MOVE241_SUNNY_DAY = 0xF1,
    MOVE242_CRUNCH = 0xF2,
    MOVE243_MIRROR_COAT = 0xF3,
    MOVE244_PSYCH_UP = 0xF4,
    MOVE245_EXTREME_SPEED = 0xF5,
    MOVE246_ANCIENT_POWER = 0xF6,
    MOVE247_SHADOW_BALL = 0xF7,
    MOVE248_FUTURE_SIGHT = 0xF8,
    MOVE249_ROCK_SMASH = 0xF9,
    MOVE250_WHIRLPOOL = 0xFA,
    MOVE251_BEAT_UP = 0xFB,
    MOVE252_FAKE_OUT = 0xFC,
    MOVE253_UPROAR = 0xFD,
    MOVE254_STOCKPILE = 0xFE,
    MOVE255_SPIT_UP = 0xFF,
    MOVE256_SWALLOW = 0x100,
    MOVE257_HEAT_WAVE = 0x101,
    MOVE258_HAIL = 0x102,
    MOVE259_TORMENT = 0x103,
    MOVE260_FLATTER = 0x104,
    MOVE261_WILL_O_WISP = 0x105,
    MOVE262_MEMENTO = 0x106,
    MOVE263_FACADE = 0x107,
    MOVE264_FOCUS_PUNCH = 0x108,
    MOVE265_SMELLING_SALTS = 0x109,
    MOVE266_FOLLOW_ME = 0x10A,
    MOVE267_NATURE_POWER = 0x10B,
    MOVE268_CHARGE = 0x10C,
    MOVE269_TAUNT = 0x10D,
    MOVE270_HELPING_HAND = 0x10E,
    MOVE271_TRICK = 0x10F,
    MOVE272_ROLE_PLAY = 0x110,
    MOVE273_WISH = 0x111,
    MOVE274_ASSIST = 0x112,
    MOVE275_INGRAIN = 0x113,
    MOVE276_SUPERPOWER = 0x114,
    MOVE277_MAGIC_COAT = 0x115,
    MOVE278_RECYCLE = 0x116,
    MOVE279_REVENGE = 0x117,
    MOVE280_BRICK_BREAK = 0x118,
    MOVE281_YAWN = 0x119,
    MOVE282_KNOCK_OFF = 0x11A,
    MOVE283_ENDEAVOR = 0x11B,
    MOVE284_ERUPTION = 0x11C,
    MOVE285_SKILL_SWAP = 0x11D,
    MOVE286_IMPRISON = 0x11E,
    MOVE287_REFRESH = 0x11F,
    MOVE288_GRUDGE = 0x120,
    MOVE289_SNATCH = 0x121,
    MOVE290_SECRET_POWER = 0x122,
    MOVE291_DIVE = 0x123,
    MOVE292_ARM_THRUST = 0x124,
    MOVE293_CAMOUFLAGE = 0x125,
    MOVE294_TAIL_GLOW = 0x126,
    MOVE295_LUSTER_PURGE = 0x127,
    MOVE296_MIST_BALL = 0x128,
    MOVE297_FEATHER_DANCE = 0x129,
    MOVE298_TEETER_DANCE = 0x12A,
    MOVE299_BLAZE_KICK = 0x12B,
    MOVE300_MUD_SPORT = 0x12C,
    MOVE301_ICE_BALL = 0x12D,
    MOVE302_NEEDLE_ARM = 0x12E,
    MOVE303_SLACK_OFF = 0x12F,
    MOVE304_HYPER_VOICE = 0x130,
    MOVE305_POISON_FANG = 0x131,
    MOVE306_CRUSH_CLAW = 0x132,
    MOVE307_BLAST_BURN = 0x133,
    MOVE308_HYDRO_CANNON = 0x134,
    MOVE309_METEOR_MASH = 0x135,
    MOVE310_ASTONISH = 0x136,
    MOVE311_WEATHER_BALL = 0x137,
    MOVE312_AROMATHERAPY = 0x138,
    MOVE313_FAKE_TEARS = 0x139,
    MOVE314_AIR_CUTTER = 0x13A,
    MOVE315_OVERHEAT = 0x13B,
    MOVE316_ODOR_SLEUTH = 0x13C,
    MOVE317_ROCK_TOMB = 0x13D,
    MOVE318_SILVER_WIND = 0x13E,
    MOVE319_METAL_SOUND = 0x13F,
    MOVE320_GRASS_WHISTLE = 0x140,
    MOVE321_TICKLE = 0x141,
    MOVE322_COSMIC_POWER = 0x142,
    MOVE323_WATER_SPOUT = 0x143,
    MOVE324_SIGNAL_BEAM = 0x144,
    MOVE325_SHADOW_PUNCH = 0x145,
    MOVE326_EXTRASENSORY = 0x146,
    MOVE327_SKY_UPPERCUT = 0x147,
    MOVE328_SAND_TOMB = 0x148,
    MOVE329_SHEER_COLD = 0x149,
    MOVE330_MUDDY_WATER = 0x14A,
    MOVE331_BULLET_SEED = 0x14B,
    MOVE332_AERIAL_ACE = 0x14C,
    MOVE333_ICICLE_SPEAR = 0x14D,
    MOVE334_IRON_DEFENSE = 0x14E,
    MOVE335_BLOCK = 0x14F,
    MOVE336_HOWL = 0x150,
    MOVE337_DRAGON_CLAW = 0x151,
    MOVE338_FRENZY_PLANT = 0x152,
    MOVE339_BULK_UP = 0x153,
    MOVE340_BOUNCE = 0x154,
    MOVE341_MUD_SHOT = 0x155,
    MOVE342_POISON_TAIL = 0x156,
    MOVE343_COVET = 0x157,
    MOVE344_VOLT_TACKLE = 0x158,
    MOVE345_MAGICAL_LEAF = 0x159,
    MOVE346_WATER_SPORT = 0x15A,
    MOVE347_CALM_MIND = 0x15B,
    MOVE348_LEAF_BLADE = 0x15C,
    MOVE349_DRAGON_DANCE = 0x15D,
    MOVE350_ROCK_BLAST = 0x15E,
    MOVE351_SHOCK_WAVE = 0x15F,
    MOVE352_WATER_PULSE = 0x160,
    MOVE353_DOOM_DESIRE = 0x161,
    MOVE354_PSYCHO_BOOST = 0x162,
    MOVE355_ROOST = 0x163,
    MOVE356_GRAVITY = 0x164,
    MOVE357_MIRACLE_EYE = 0x165,
    MOVE358_WAKE_UP_SLAP = 0x166,
    MOVE359_HAMMER_ARM = 0x167,
    MOVE360_GYRO_BALL = 0x168,
    MOVE361_HEALING_WISH = 0x169,
    MOVE362_BRINE = 0x16A,
    MOVE363_NATURAL_GIFT = 0x16B,
    MOVE364_FEINT = 0x16C,
    MOVE365_PLUCK = 0x16D,
    MOVE366_TAILWIND = 0x16E,
    MOVE367_ACUPRESSURE = 0x16F,
    MOVE368_METAL_BURST = 0x170,
    MOVE369_U_TURN = 0x171,
    MOVE370_CLOSE_COMBAT = 0x172,
    MOVE371_PAYBACK = 0x173,
    MOVE372_ASSURANCE = 0x174,
    MOVE373_EMBARGO = 0x175,
    MOVE374_FLING = 0x176,
    MOVE375_PSYCHO_SHIFT = 0x177,
    MOVE376_TRUMP_CARD = 0x178,
    MOVE377_HEAL_BLOCK = 0x179,
    MOVE378_WRING_OUT = 0x17A,
    MOVE379_POWER_TRICK = 0x17B,
    MOVE380_GASTRO_ACID = 0x17C,
    MOVE381_LUCKY_CHANT = 0x17D,
    MOVE382_ME_FIRST = 0x17E,
    MOVE383_COPYCAT = 0x17F,
    MOVE384_POWER_SWAP = 0x180,
    MOVE385_GUARD_SWAP = 0x181,
    MOVE386_PUNISHMENT = 0x182,
    MOVE387_LAST_RESORT = 0x183,
    MOVE388_WORRY_SEED = 0x184,
    MOVE389_SUCKER_PUNCH = 0x185,
    MOVE390_TOXIC_SPIKES = 0x186,
    MOVE391_HEART_SWAP = 0x187,
    MOVE392_AQUA_RING = 0x188,
    MOVE393_MAGNET_RISE = 0x189,
    MOVE394_FLARE_BLITZ = 0x18A,
    MOVE395_FORCE_PALM = 0x18B,
    MOVE396_AURA_SPHERE = 0x18C,
    MOVE397_ROCK_POLISH = 0x18D,
    MOVE398_POISON_JAB = 0x18E,
    MOVE399_DARK_PULSE = 0x18F,
    MOVE400_NIGHT_SLASH = 0x190,
    MOVE401_AQUA_TAIL = 0x191,
    MOVE402_SEED_BOMB = 0x192,
    MOVE403_AIR_SLASH = 0x193,
    MOVE404_X_SCISSOR = 0x194,
    MOVE405_BUG_BUZZ = 0x195,
    MOVE406_DRAGON_PULSE = 0x196,
    MOVE407_DRAGON_RUSH = 0x197,
    MOVE408_POWER_GEM = 0x198,
    MOVE409_DRAIN_PUNCH = 0x199,
    MOVE410_VACUUM_WAVE = 0x19A,
    MOVE411_FOCUS_BLAST = 0x19B,
    MOVE412_ENERGY_BALL = 0x19C,
    MOVE413_BRAVE_BIRD = 0x19D,
    MOVE414_EARTH_POWER = 0x19E,
    MOVE415_SWITCHEROO = 0x19F,
    MOVE416_GIGA_IMPACT = 0x1A0,
    MOVE417_NASTY_PLOT = 0x1A1,
    MOVE418_BULLET_PUNCH = 0x1A2,
    MOVE419_AVALANCHE = 0x1A3,
    MOVE420_ICE_SHARD = 0x1A4,
    MOVE421_SHADOW_CLAW = 0x1A5,
    MOVE422_THUNDER_FANG = 0x1A6,
    MOVE423_ICE_FANG = 0x1A7,
    MOVE424_FIRE_FANG = 0x1A8,
    MOVE425_SHADOW_SNEAK = 0x1A9,
    MOVE426_MUD_BOMB = 0x1AA,
    MOVE427_PSYCHO_CUT = 0x1AB,
    MOVE428_ZEN_HEADBUTT = 0x1AC,
    MOVE429_MIRROR_SHOT = 0x1AD,
    MOVE430_FLASH_CANNON = 0x1AE,
    MOVE431_ROCK_CLIMB = 0x1AF,
    MOVE432_DEFOG = 0x1B0,
    MOVE433_TRICK_ROOM = 0x1B1,
    MOVE434_DRACO_METEOR = 0x1B2,
    MOVE435_DISCHARGE = 0x1B3,
    MOVE436_LAVA_PLUME = 0x1B4,
    MOVE437_LEAF_STORM = 0x1B5,
    MOVE438_POWER_WHIP = 0x1B6,
    MOVE439_ROCK_WRECKER = 0x1B7,
    MOVE440_CROSS_POISON = 0x1B8,
    MOVE441_GUNK_SHOT = 0x1B9,
    MOVE442_IRON_HEAD = 0x1BA,
    MOVE443_MAGNET_BOMB = 0x1BB,
    MOVE444_STONE_EDGE = 0x1BC,
    MOVE445_CAPTIVATE = 0x1BD,
    MOVE446_STEALTH_ROCK = 0x1BE,
    MOVE447_GRASS_KNOT = 0x1BF,
    MOVE448_CHATTER = 0x1C0,
    MOVE449_JUDGMENT = 0x1C1,
    MOVE450_BUG_BITE = 0x1C2,
    MOVE451_CHARGE_BEAM = 0x1C3,
    MOVE452_WOOD_HAMMER = 0x1C4,
    MOVE453_AQUA_JET = 0x1C5,
    MOVE454_ATTACK_ORDER = 0x1C6,
    MOVE455_DEFEND_ORDER = 0x1C7,
    MOVE456_HEAL_ORDER = 0x1C8,
    MOVE457_HEAD_SMASH = 0x1C9,
    MOVE458_DOUBLE_HIT = 0x1CA,
    MOVE459_ROAR_OF_TIME = 0x1CB,
    MOVE460_SPACIAL_REND = 0x1CC,
    MOVE461_LUNAR_DANCE = 0x1CD,
    MOVE462_CRUSH_GRIP = 0x1CE,
    MOVE463_MAGMA_STORM = 0x1CF,
    MOVE464_DARK_VOID = 0x1D0,
    MOVE465_SEED_FLARE = 0x1D1,
    MOVE466_OMINOUS_WIND = 0x1D2,
    MOVE467_SHADOW_FORCE = 0x1D3,
    MOVE468_HONE_CLAWS = 0x1D4,
    MOVE469_WIDE_GUARD = 0x1D5,
    MOVE470_GUARD_SPLIT = 0x1D6,
    MOVE471_POWER_SPLIT = 0x1D7,
    MOVE472_WONDER_ROOM = 0x1D8,
    MOVE473_PSYSHOCK = 0x1D9,
    MOVE474_VENOSHOCK = 0x1DA,
    MOVE475_AUTOTOMIZE = 0x1DB,
    MOVE476_RAGE_POWDER = 0x1DC,
    MOVE477_TELEKINESIS = 0x1DD,
    MOVE478_MAGIC_ROOM = 0x1DE,
    MOVE479_SMACK_DOWN = 0x1DF,
    MOVE480_STORM_THROW = 0x1E0,
    MOVE481_FLAME_BURST = 0x1E1,
    MOVE482_SLUDGE_WAVE = 0x1E2,
    MOVE483_QUIVER_DANCE = 0x1E3,
    MOVE484_HEAVY_SLAM = 0x1E4,
    MOVE485_SYNCHRONOISE = 0x1E5,
    MOVE486_ELECTRO_BALL = 0x1E6,
    MOVE487_SOAK = 0x1E7,
    MOVE488_FLAME_CHARGE = 0x1E8,
    MOVE489_COIL = 0x1E9,
    MOVE490_LOW_SWEEP = 0x1EA,
    MOVE491_ACID_SPRAY = 0x1EB,
    MOVE492_FOUL_PLAY = 0x1EC,
    MOVE493_SIMPLE_BEAM = 0x1ED,
    MOVE494_ENTRAINMENT = 0x1EE,
    MOVE495_AFTER_YOU = 0x1EF,
    MOVE496_ROUND = 0x1F0,
    MOVE497_ECHOED_VOICE = 0x1F1,
    MOVE498_CHIP_AWAY = 0x1F2,
    MOVE499_CLEAR_SMOG = 0x1F3,
    MOVE500_STORED_POWER = 0x1F4,
    MOVE501_QUICK_GUARD = 0x1F5,
    MOVE502_ALLY_SWITCH = 0x1F6,
    MOVE503_SCALD = 0x1F7,
    MOVE504_SHELL_SMASH = 0x1F8,
    MOVE505_HEAL_PULSE = 0x1F9,
    MOVE506_HEX = 0x1FA,
    MOVE507_SKY_DROP = 0x1FB,
    MOVE508_SHIFT_GEAR = 0x1FC,
    MOVE509_CIRCLE_THROW = 0x1FD,
    MOVE510_INCINERATE = 0x1FE,
    MOVE511_QUASH = 0x1FF,
    MOVE512_ACROBATICS = 0x200,
    MOVE513_REFLECT_TYPE = 0x201,
    MOVE514_RETALIATE = 0x202,
    MOVE515_FINAL_GAMBIT = 0x203,
    MOVE516_BESTOW = 0x204,
    MOVE517_INFERNO = 0x205,
    MOVE518_WATER_PLEDGE = 0x206,
    MOVE519_FIRE_PLEDGE = 0x207,
    MOVE520_GRASS_PLEDGE = 0x208,
    MOVE521_VOLT_SWITCH = 0x209,
    MOVE522_STRUGGLE_BUG = 0x20A,
    MOVE523_BULLDOZE = 0x20B,
    MOVE524_FROST_BREATH = 0x20C,
    MOVE525_DRAGON_TAIL = 0x20D,
    MOVE526_WORK_UP = 0x20E,
    MOVE527_ELECTROWEB = 0x20F,
    MOVE528_WILD_CHARGE = 0x210,
    MOVE529_DRILL_RUN = 0x211,
    MOVE530_DUAL_CHOP = 0x212,
    MOVE531_HEART_STAMP = 0x213,
    MOVE532_HORN_LEECH = 0x214,
    MOVE533_SACRED_SWORD = 0x215,
    MOVE534_RAZOR_SHELL = 0x216,
    MOVE535_HEAT_CRASH = 0x217,
    MOVE536_LEAF_TORNADO = 0x218,
    MOVE537_STEAMROLLER = 0x219,
    MOVE538_COTTON_GUARD = 0x21A,
    MOVE539_NIGHT_DAZE = 0x21B,
    MOVE540_PSYSTRIKE = 0x21C,
    MOVE541_TAIL_SLAP = 0x21D,
    MOVE542_HURRICANE = 0x21E,
    MOVE543_HEAD_CHARGE = 0x21F,
    MOVE544_GEAR_GRIND = 0x220,
    MOVE545_SEARING_SHOT = 0x221,
    MOVE546_TECHNO_BLAST = 0x222,
    MOVE547_RELIC_SONG = 0x223,
    MOVE548_SECRET_SWORD = 0x224,
    MOVE549_GLACIATE = 0x225,
    MOVE550_BOLT_STRIKE = 0x226,
    MOVE551_BLUE_FLARE = 0x227,
    MOVE552_FIERY_DANCE = 0x228,
    MOVE553_FREEZE_SHOCK = 0x229,
    MOVE554_ICE_BURN = 0x22A,
    MOVE555_SNARL = 0x22B,
    MOVE556_ICICLE_CRASH = 0x22C,
    MOVE557_V_CREATE = 0x22D,
    MOVE558_FUSION_FLARE = 0x22E,
    MOVE559_FUSION_BOLT = 0x22F,
    MOVE560_FLYING_PRESS = 0x230,
    MOVE561_MAT_BLOCK = 0x231,
    MOVE562_BELCH = 0x232,
    MOVE563_ROTOTILLER = 0x233,
    MOVE564_STICKY_WEB = 0x234,
    MOVE565_FELL_STINGER = 0x235,
    MOVE566_PHANTOM_FORCE = 0x236,
    MOVE567_TRICK_OR_TREAT = 0x237,
    MOVE568_NOBLE_ROAR = 0x238,
    MOVE569_ION_DELUGE = 0x239,
    MOVE570_PARABOLIC_CHARGE = 0x23A,
    MOVE571_FOREST_S_CURSE = 0x23B,
    MOVE572_PETAL_BLIZZARD = 0x23C,
    MOVE573_FREEZE_DRY = 0x23D,
    MOVE574_DISARMING_VOICE = 0x23E,
    MOVE575_PARTING_SHOT = 0x23F,
    MOVE576_TOPSY_TURVY = 0x240,
    MOVE577_DRAINING_KISS = 0x241,
    MOVE578_CRAFTY_SHIELD = 0x242,
    MOVE579_FLOWER_SHIELD = 0x243,
    MOVE580_GRASSY_TERRAIN = 0x244,
    MOVE581_MISTY_TERRAIN = 0x245,
    MOVE582_ELECTRIFY = 0x246,
    MOVE583_PLAY_ROUGH = 0x247,
    MOVE584_FAIRY_WIND = 0x248,
    MOVE585_MOONBLAST = 0x249,
    MOVE586_BOOMBURST = 0x24A,
    MOVE587_FAIRY_LOCK = 0x24B,
    MOVE588_KING_S_SHIELD = 0x24C,
    MOVE589_PLAY_NICE = 0x24D,
    MOVE590_CONFIDE = 0x24E,
    MOVE591_DIAMOND_STORM = 0x24F,
    MOVE592_STEAM_ERUPTION = 0x250,
    MOVE593_HYPERSPACE_HOLE = 0x251,
    MOVE594_WATER_SHURIKEN = 0x252,
    MOVE595_MYSTICAL_FIRE = 0x253,
    MOVE596_SPIKY_SHIELD = 0x254,
    MOVE597_AROMATIC_MIST = 0x255,
    MOVE598_EERIE_IMPULSE = 0x256,
    MOVE599_VENOM_DRENCH = 0x257,
    MOVE600_POWDER = 0x258,
    MOVE601_GEOMANCY = 0x259,
    MOVE602_MAGNETIC_FLUX = 0x25A,
    MOVE603_HAPPY_HOUR = 0x25B,
    MOVE604_ELECTRIC_TERRAIN = 0x25C,
    MOVE605_DAZZLING_GLEAM = 0x25D,
    MOVE606_CELEBRATE = 0x25E,
    MOVE607_HOLD_HANDS = 0x25F,
    MOVE608_BABY_DOLL_EYES = 0x260,
    MOVE609_NUZZLE = 0x261,
    MOVE610_HOLD_BACK = 0x262,
    MOVE611_INFESTATION = 0x263,
    MOVE612_POWER_UP_PUNCH = 0x264,
    MOVE613_OBLIVION_WING = 0x265,
    MOVE614_THOUSAND_ARROWS = 0x266,
    MOVE615_THOUSAND_WAVES = 0x267,
    MOVE616_LAND_S_WRATH = 0x268,
    MOVE617_LIGHT_OF_RUIN = 0x269,
    MOVE618_ORIGIN_PULSE = 0x26A,
    MOVE619_PRECIPICE_BLADES = 0x26B,
    MOVE620_DRAGON_ASCENT = 0x26C,
    MOVE621_HYPERSPACE_FURY = 0x26D,
};

struct SWAN_ALIGNED(4) HandlerParam_AddCondition
{
    HandlerParam_Header header;
    MoveCondition sickID;
    ConditionData sickCont;
    u8 fAlmost;
    u8 reserved;
    u8 overwriteMode;
    u8 pokeID;
    u8 OverwriteMode;
    HandlerParam_StrParams exStr;
};

typedef BattleEventHandlerTableEntry* (*MoveEventAddFunc)(int* a1);
struct MoveEventAddTable
{
    MoveID move;
    MoveEventAddFunc func;
};

struct HandlerParam_SwapItem
{
    HandlerParam_Header header;
    u8 pokeID;
    u8 fIncRecordCount;
    HandlerParam_StrParams exStr;
    HandlerParam_StrParams exSubStr1;
    HandlerParam_StrParams exSubStr2;
};

struct SWAN_ALIGNED(4) HandlerParam_Damage
{
    HandlerParam_Header header;
    u16 damage;
    u8 pokeID;
    char flags;
    u16 effectNo;
    u8 posFrom;
    u8 posTo;
    HandlerParam_StrParams exStr;
};

struct HandlerParam_ChangeAbility
{
    HandlerParam_Header header;
    u16 abilityID;
    u8 pokeID;
    u8 fSameAbilityEffective;
    u8 fSkipSwitchInEvent;
    HandlerParam_StrParams exStr;
};

struct SWAN_ALIGNED(4) HandlerParam_RemoveSideEffect
{
    HandlerParam_Header header;
    u8 flags[3];
    u8 side;
};

enum MoveFailCause
{
    MOVEFAIL_NULL = 0x0,
    MOVEFAIL_PPZERO = 0x1,
    MOVEFAIL_SLEEP = 0x2,
    MOVEFAIL_PARALYSIS = 0x3,
    MOVEFAIL_FREEZE = 0x4,
    MOVEFAIL_CONFUSION = 0x5,
    MOVEFAIL_FLINCH = 0x6,
    MOVEFAIL_FOCUSPUNCHFAIL = 0x7,
    MOVEFAIL_ATTRACT = 0x8,
    MOVEFAIL_DISABLE = 0x9,
    MOVEFAIL_TAUNT = 0xA,
    MOVEFAIL_TORMENT = 0xB,
    MOVEFAIL_IMPRISON = 0xC,
    MOVEFAIL_HEALBLOCK = 0xD,
    MOVEFAIL_HPFULL = 0xE,
    MOVEFAIL_INSOMNIA = 0xF,
    MOVEFAIL_TRUANT = 0x10,
    MOVEFAIL_MOVELOCK = 0x11,
    MOVEFAIL_ENCORE = 0x12,
    MOVEFAIL_ABILITY = 0x13,
    MOVEFAIL_GRAVITY = 0x14,
    MOVEFAIL_TO_RECOVER = 0x15,
    MOVEFAIL_IGNORE = 0x16,
    MOVEFAIL_IGNORE_FALL_ASLEEP = 0x17,
    MOVEFAIL_IGNORE_SLEEPING = 0x18,
    MOVEFAIL_NO_REACTION = 0x19,
    MOVEFAIL_OTHER = 0x1A,
};

struct HandlerParam_Message
{
    HandlerParam_Header header;
    HandlerParam_StrParams str;
};

struct HandlerParam_RecoverHP
{
    HandlerParam_Header header;
    u16 recoverHP;
    u8 pokeID;
    u8 fFailCheckThru;
    HandlerParam_StrParams exStr;
};

struct HandlerParam_SetItem
{
  HandlerParam_Header header;
  u16 itemID;
  u8 pokeID;
  u8 fClearConsume;
  u8 fClearConsumeOtherPoke;
  u8 clearConsumePokeID;
  u8 fCallConsumedEvent;
  HandlerParam_StrParams exStr;
};

struct HandlerParam_CureCondition
{
    HandlerParam_Header header;
    MoveCondition sickCode;
    u8 pokeID[12];
    u8 poke_cnt;
    u8 fStdMsgDisable;
    HandlerParam_StrParams exStr;
};

#pragma pack(push, 1)
struct RandSet
{
    unsigned __int64 seed;
    unsigned __int64 mul;
    unsigned __int64 add;
};
#pragma pack(pop)

struct SWAN_ALIGNED(4) GFLGlobalState
{
    RandSet RNGState;
    TCBManager* HBlankIRQTCBMgr;
    void* HBlankIRQTCBAlloc;
    TCBManager* G2DTcbMgr;
    void* G2DTCBAlloc;
};

struct BattleSideCondition
{
    BattleEventItem* BattleEventItem;
    ConditionData conditionData;
    int TurnCounter;
    int Count;
};

enum SideEffect
{
    SIDEEFF_REFLECT = 0x0,
    SIDEEFF_LIGHT_SCREEN = 0x1,
    SIDEEFF_SAFEGUARD = 0x2,
    SIDEEFF_MIST = 0x3,
    SIDEEFF_TAILWIND = 0x4,
    SIDEEFF_LUCKY_CHANT = 0x5,
    SIDEEFF_SPIKES = 0x6,
    SIDEEFF_TOXIC_SPIKES = 0x7,
    SIDEEFF_STEALTH_ROCK = 0x8,
    SIDEEFF_WIDE_GUARD = 0x9,
    SIDEEFF_QUICK_GUARD = 0xA,
    SIDEEFF_RAINBOW = 0xB,
    SIDEEFF_SEA_OF_FIRE = 0xC,
    SIDEEFF_SWAMP = 0xD,
};

typedef BattleEventHandlerTableEntry* (*SideEffectEventAddFunc)(int*);

struct SideEffectEventAddTable
{
    // actual data-type -> enum SideEffect
    int sideEffect;
    SideEffectEventAddFunc func;
    int maxCount;
};

struct HandlerParam_AddSideEffect
{
    HandlerParam_Header header;
    u32 effect;
    ConditionData cont;
    u8 side;
    HandlerParam_StrParams exStr;
};

struct EventFieldMenu
{
    HeapID HeapID;
    GameEvent* ThisEvent;
    GameSystem* m_GameSystem;
    Field* m_Field;
    _DWORD dword10;
    _DWORD State;
    _DWORD LastSubscreenID;
    AppCallFramework m_AppCallFramework;
};

struct FieldProcLinkEntry
{
    int OvlID;
    GameProcFunctions* ProcFuncs;
    void* (*ProcDataMakeFunc)(EventFieldAppCall* event, int itemId, int lastAppId, void* lastAppData);
    int(*CreateResult)(EventFieldAppCall*, void*);
    void* field_10;
    void(*FreeSubprocDataFunc)(void*);
};

enum Season
{
    SEASON_SPRING = 0x0,
    SEASON_SUMMER = 0x1,
    SEASON_AUTUMN = 0x2,
    SEASON_WINTER = 0x3,
};

// BattleHandler_x definitions
int HEManager_GetUseItemNo(_WORD* a1);
unsigned int HEManager_IsUsed(_DWORD* a1);
unsigned int HEManager_GetPrevResult(_DWORD* a1);
BattleMon* PokeCon_GetPokeParamConst(PokeCon* a1, int a2);
bool BattleMon_IsFainted(BattleMon* a1);
bool BattleHandler_UseHeldItem(ServerFlow* a1, _DWORD* a2);
int BattleHandler_UseItem(ServerFlow* a1, HandlerParam_Header* a2);
int BattleHandler_AbilityPopupAdd(ServerFlow* a1, _DWORD* a2);
int BattleHandler_AbilityPopupRemove(ServerFlow* a1, _DWORD* a2);
int BattleHandler_Message(ServerFlow* a1, _DWORD* a2);
bool BattleHandler_RecoverHP(ServerFlow* a1, int a2, int a3);
int BattleHandler_Drain(ServerFlow* a1, int a2);
int BattleHandler_Damage(ServerFlow* a1, HandlerParam_Header* a2);
int BattleHandler_ChangeHP(ServerFlow* a1, unsigned __int8* a2);
int BattleHandler_RecoverPP(ServerFlow* a1, HandlerParam_Header* a2);
int BattleHandler_DecrementPP(ServerFlow* a1, int a2);
int BattleHandler_CureCondition(ServerFlow* a1, int a2, int a3);
int BattleHandler_AddCondition(ServerFlow* a1, int a2);
int BattleHandler_StatChange(ServerFlow* a1, int a2, unsigned __int16 a3);
int BattleHandler_SetStatStage(int a1, unsigned __int8* a2);
int BattleHandler_ResetStatStage(int a1, int a2);
int BattleHandler_SetStatus(ServerFlow* a1, int a2);
int BattleHandler_RecoverStatStage(int a1, int a2);
int BattleHandler_Faint(ServerFlow* a1, int a2);
int BattleHandler_ChangeType(int a1, int a2);
int BattleHandler_SetTurnFlag(int a1, int a2);
int BattleHandler_ResetTurnFlag(int a1, int a2);
int BattleHandler_SetContinueFlag(ServerFlow* a1, int a2);
int BattleHandler_ResetContinueFlag(ServerFlow* a1, int a2);
int BattleHandler_AddSideEffect(ServerFlow* a1, int a2);
int BattleHandler_RemoveSideEffectCore(int a1, int a2);
int BattleHandler_AddFieldEffect(int a1, int a2);
int BattleHandler_RemoveFieldEffect(ServerFlow* a1, HandlerParam_Header* a2);
int BattleHandler_ChangeWeather(ServerFlow* a1, int a2);
bool BattleHandler_AddPosEffect(int a1, int a2);
int BattleHandler_AbilityChange(ServerFlow* a1, int a2);
int BattleHandler_SetItem(ServerFlow* a1, int a2);
int BattleHandler_CheckHeldItem(ServerFlow* a1, int a2);
int BattleHandler_ForceUseItem(ServerFlow* a1, int a2);
int BattleHandler_ConsumeItem(ServerFlow* a1, HandlerParam_ConsumeItem* a2);
int BattleHandler_SwapItem(ServerFlow* a1, int a2);
int BattleHandler_UpdateMove(int a1, int a2);
int BattleHandler_SetCounter(ServerFlow* a1, unsigned __int8* a2);
int BattleHandler_DelayMoveDamage(int* a1, int a2);
int BattleHandler_QuitBattle(ServerFlow* a1, _DWORD* a2);
int BattleHandler_Switch(ServerFlow* a1, HandlerParam_Switch* a2);
int BattleHandler_BatonPass(ServerFlow* a1, int a2);
bool BattleHandler_Flinch(ServerFlow* a1, int a2);
int BattleHandler_Revive(ServerFlow* a1, int a2);
int BattleHandler_SetWeight(ServerFlow* a1, int a2);
bool BattleHandler_ForceSwitch(int a1, int a2);
int BattleHandler_InterruptAction(ServerFlow* a1, int a2);
bool BattleHandler_InterruptMove(ServerFlow* a1, int a2);
int BattleHandler_SendLast(ServerFlow* a1, int a2);
int BattleHandler_SwapPoke(ServerFlow* a1, HandlerParam_SwapPoke* a2);
int BattleHandler_Transform(ServerFlow* a1, HandlerParam_Transform* a2);
int BattleHandler_IllusionBreak(ServerFlow* a1, HandlerParam_IllusionBreak* a2);
int BattleHandler_GravityCheck(ServerFlow* a1, _DWORD* a2);
int BattleHandler_HideTurnCancel(ServerFlow* a1, int a2);
int BattleHandler_AnimAtPos(ServerFlow* a1, HandlerParam_AddAnimation* a2);
int BattleHandler_RemoveMessageWindow(ServerFlow* a1);
int BattleHandler_ChangeForm(ServerFlow* a1, HandlerParam_ChangeForm* a2);
int BattleHandler_SetMoveAnimIndex(ServerFlow* a1, HandlerParam_Header* a2);
int BattleHandler_SetMoveAnimEnable(ServerFlow* a1);

unsigned int* HEManager_SetResult(unsigned int* result, int a2);

// AddExpAndEvs definitions
BtlSetup* MainModule_GetBtlSetup_0(MainModule* a1);
u32 CalcBaseExpGain(BattleMon* defeatedMon, int KeySystemLevelAdjust);
BtlType MainModule_GetBattleType(MainModule* a1);
int BattleParty_GetPartyCount(BattleParty* a1);
BtlType BtlSetup_GetBattleType(MainModule* a1);
void sys_memset(const void* ptr, u8 value, size_t size);
BattleMon* BattleParty_GetMonData(BattleParty* a1, unsigned int a2);
bool BattleMon_IsFainted(BattleMon* a1);
ItemID BattleMon_GetHeldItem(BattleMon* a1);
int BattleMon_GetConfrontRecCount(BattleMon* a1);
int BattleMon_GetConfrontedPokeID(BattleMon* a1, unsigned int a2);
BattleMon* PokeCon_GetBattleMon(PokeCon* a1, int a2);
int BattleMon_GetID(BattleMon* a1);
void* MainModule_GetPlayerStatus(MainModule* a1);
PartyPkm* BattleMon_GetSrcData(BattleMon* a1);
int BattleMon_GetValue(BattleMon* a1, BattleMonValue a2);
int ScaleExpGainedByLevel(int a1, unsigned int a2, int a3, int a4);
bool IsTrainerOT(BoxPkm* pPkm, void* pTrainerInfo);
u32 PokeParty_GetParam(PartyPkm* pPkm, PkmField field, void* extra);
int TrainerInfo_GetRegion(void* a1);
unsigned int fixed_round(int a1, int a2);
u32 PassPower_ApplyEXP(u32 baseExp);
BattleMon* BattleParty_GetPartyMember(BattleParty* a1, unsigned int a2);
void AddEVs(BattleMon* r0_0, BattleMon* a2, _BYTE* a3);

EventWorkSave* GameData_GetEventWork(GameData* gameData);
u16* EventWork_GetWkPtr(EventWorkSave* eventWork, int swkId);
u32 PML_UtilGetPkmLvExp(u16 species, u16 form, int level);

// ServerFlow_GenerateActionOrder definitions
SVCL_WORK* BattleServer_GetClientWork_0(BtlServerWk* a1, unsigned int a2);
int BattleAction_GetAction(BattleActionParam* a1);
BattleMon* BattleParty_GetPartyMember(BattleParty* a1, unsigned int a2);
unsigned int HEManager_PushState(unsigned int* a1);
BtlType BtlSetup_GetBattleType(MainModule* a1);
int ServerEvent_GetMovePriority(ServerFlow* a1, int a2, int a3);
int ServerEvent_CalculateSpeed(ServerFlow* a1, BattleMon* a2, bool IsTrickRoomEnabled);
_DWORD* HEManager_PopState(_DWORD* result, int a2);
unsigned int SortActionSub(int a1, unsigned int a2);
int ServerEvent_CheckSpecialPriority(ServerFlow* a1, BattleMon* a2);
int sub_219F2EC_new(int a1, int a2)
{
    return *(int*)(a1 + a2 + 48);
}
//a1 = unknown*
//a2 = a2a
//a3 = v28
int sub_219F2F4_new(int a1, int a2, int a3)
{
    return *(_DWORD*)(4 * a3 + a1 + 12 * a2);
}
bool sub_21BDC48_new(int* a1)
{
    int v1; // r0

    v1 = *a1;
    return (v1 & 0xF) == 3 && (unsigned int)(v1 << 21) >> 31 == 1;
}
int sub_219D3CC_new(int a1)
{
    if (a1 == 2)
    {
        return 1;
    }
    if (a1 == 3)
    {
        return 2;
    }
    return 0;
}

// ServerFlow_StartTurn definitions
void j_j_FaintRecord_StartTurn(unsigned int a1);
int BattleEvent_StartTurn();
BtlServerWk* BattleServer_InitChangePokemonReq(BtlServerWk* result);
unsigned int BattleEventVar_CheckStackCleared();
int ServerFlow_ClearPokeNoActionFlag(ServerFlow* a1, int a2, int a3, int a4);
unsigned int ServerFlow_GenerateActionOrder(ServerFlow* a1, int a2, int a3);
unsigned int ServerFlow_ActOrderProcMain(ServerFlow* a1, unsigned int a2);
ServerFlow* sub_21AC068_new(ServerFlow* result)
{
    result->revivedPokeCount = 0;
    return result;
}

// ServerCommand_TurnCheck definitions
void BattleMon_TurnCheck(BattleMon* a1);
void BattleMon_ClearComboMoveData(BattleMon* result);

// ServerControl_AfterMove definitions
BattleParty* GetPartyData(PokeCon* a1, int a2);
BattleMon* BattleParty_GetMonData(BattleParty* a1, unsigned int a2);
BattleMon* BattleParty_GetMonData(BattleParty* a1, unsigned int a2);
_DWORD* ServerControl_AfterMoveCore(ServerFlow* a1, BattleMon* a2);

// ServerCommand_MoveUsed definitions
int Move_SetUsedFlag(int a1, int a2);

// ServerCommand_SwitchOut definitions
bool BattleViewCmd_SwitchOut_Wait(int a1);
int MainModule_BattlePosToViewPos(MainModule* a1, int a2);
TCB* BattleViewCmd_SwitchOut_Start(BtlvCore* a1, int a2, int a3);

// ServerCommand_SwitchIn definitions
void BattleParty_SwapMembers(BattleParty* result, int a2, int a3);
void BattleMon_SetAppearTurn(BattleMon* a1, __int16 a2);

// ServerCommand_LowerStat definitions
unsigned int MainModule_PokeIDToPokePos(MainModule* a1, PokeCon* a2, unsigned int a3);
bool IsChapterSkipMode(BtlClientWk* a1);
unsigned int MainModule_AreMoveAnimationsEnabled(MainModule* a1);
bool BattleViewCmd_WaitEffect();
void sub_21DF39C(unsigned int a1, int a2);

bool sub_21D0400_new()
{
    return !BattleViewCmd_WaitEffect();
}
void sub_21D03E0_new(int a1, unsigned int a2)
{
    sub_21DF39C(a2, 610);
}

// ServerEvent_CalculateSpeed definitions
int BattleMon_GetValue(BattleMon* a1, BattleMonValue a2);
void BattleEventVar_Push();
int BattleMon_GetID(BattleMon* a1);
unsigned int BattleEventVar_SetConstValue(BattleEventVar a1, int a2);
unsigned int BattleEventVar_SetConstValue(BattleEventVar a1, int a2);
unsigned int BattleEventVar_SetValue(BattleEventVar a1, int a2);
unsigned int BattleEventVar_SetValue(BattleEventVar a1, int a2);
unsigned int BattleEventVar_SetMulValue(BattleEventVar a1, int a2, int a3, int a4);
int BattleEvent_CallHandlers(ServerFlow* a1, BattleEventType a2);
int BattleEventVar_GetValue(BattleEventVar a1);
unsigned int fixed_round(int a1, int a2);
MoveCondition BattleMon_GetStatus(BattleMon* a1);
void BattleEventVar_Pop();

// ServerControl_SortBySpeed definitions
void j_PokeSet_Clear_2(const void* a1);
void FRONT_POKE_SEEK_InitWork(FRONT_POKE_SEEK_WORK* a1, ServerFlow* a2);
int FRONT_POKE_SEEK_GetNext(FRONT_POKE_SEEK_WORK* a1, ServerFlow* a2, BattleMon** a3);
int j_PokeSet_Add_0(PokeSet* result, BattleMon* a2);
int j_PokeSet_SortBySpeed(PokeSet* a1, ServerFlow* a2);

// ServerControl_MoveStatStageChangeEffectCommon definitions
int wazaSerial_Inc(ServerFlow* a1);
PkmStat PML_MoveGetStatChangeStat(int wazaId);
int ServerEvent_GetMoveStatChangeValue(ServerFlow* a1, int a2, int a3, BattleMon* a4, int a5, int* a6, int* dest);
int ServerControl_StatStageChangeCore(ServerFlow* a1, int a2, BattleMon* a3, unsigned int a4, int a5, unsigned __int8 a6, unsigned __int16 a7, int a8, int a9, int a10);
void ServerEvent_MoveStatStageChangeApplied(ServerFlow* a1, BattleMon* a2, int a3, int a4, int a5);

// ServerControl_EffectCondition definitions
u32 PML_MoveGetParam(int wazaId, MoveField dataId);
int sub_20214DC(int a1);
unsigned int MoveConditionToPokeCondition(unsigned __int16 a1, int a2, int* a3);
void j_j_PokeSet_SeekStart_3(PokeSet* a1);
BattleMon* j_j_PokeSet_SeekNext_7(PokeSet* a1);
int ServerControl_MoveStatStageChangeEffectCommon(ServerFlow* a1, unsigned __int16* a2, BattleMon* a3, int a4, int a5);
int ServerControl_MoveConditionCore(ServerFlow* a1, BattleMon* a2, BattleMon* a3, int a4, int a5, ConditionData a6, int a7);
void sub_21A6D54();

// ServerEvent_DamageAddEffect definitions
bool BattleMon_IsFainted(BattleMon* a1);
bool ServerEvent_CheckAddStatStageChance(ServerFlow* a1, unsigned __int16* a2, BattleMon* a3, BattleMon* a4);

// ServerControl_DamageRoot definitions
u32 PML_MoveGetQuality(int wazaId);
int ServerControl_MoveUseRecordUpdate(ServerFlow* a1, int a2, int a3);
bool getMoveFlag(int a1, MoveFlag a2);
int ServerControl_ChargeUpMoveUse(ServerFlow* a1, BattleMon* a2, PokeSet* a3, int a4);
void j_j_PokeSet_Clear_5(const void* a1);
int SCQUE_RESERVE_Pos(_DWORD* a1, int a2);
void MoveAnimCtrl_Setup(MoveAnimCtrl* a1, ServerFlow* a2, BattleMon* a3, PokeSet* a4);
int ServerEvent_MoveExecuteStart(ServerFlow* a1, BattleMon* a2, int a3, PokeSet* a4);
int ServerDisplay_MoveAnimation(ServerFlow* a1, int a2, MoveAnimCtrl* a3, unsigned __int16 a4);
void j_j_PokeSet_RemoveDisablePoke(_BYTE* a1);
void ServerControl_AvoidOutOfRangeMove(ServerFlow* a1, unsigned __int16* a2, BattleMon* a3, PokeSet* a4);
void ServerControl_CheckMovedPokeAvoid(ServerFlow* a1, BattleMon* a2, _BYTE* a3, int a4, int a5);
bool j_j_PokeSet_IsRemovedAll(_BYTE* a1);
void ServerDisplay_MoveFail(ServerFlow* a1);
void flowsub_CheckPokeHideAvoid(ServerFlow* a1, unsigned __int16* a2, BattleMon* a3, int a4);
void EffectivenessRecorder_Init(EffectivenessRecorder* result);
int ServerEvent_MoveEffectivenessCheckEnable(ServerFlow* a1, unsigned __int16* a2, BattleMon* a3);
void flowsub_CheckNoEffect_TypeAffinity(ServerFlow* a1, MoveParam* a2, BattleMon* attacker, PokeSet* targets, EffectivenessRecorder* effrec);
void flowsub_CheckNoEffect_Protect(ServerFlow* a1, unsigned __int16* a2, BattleMon* a3, PokeSet* a4, int a5);
void flowsub_CheckWazaAvoid(ServerFlow* a1, unsigned __int16* a2, BattleMon* a3, PokeSet* a4);
bool j_j_PokeSet_IsRemovedAll_2(PokeSet* a1);
void ServerControl_SubstituteExclude(ServerFlow* a1, unsigned __int16* a2, BattleMon* a3, int a4, int a5);
bool j_j_PokeSet_IsRemovedAll_3(PokeSet* a1);
int ServerControl_DamageRoot(ServerFlow* a1, MoveParam* a2, BattleMon* a3, PokeSet* a4, int a5, int a6);
char* ServerControl_SimpleCondition(ServerFlow* a1, int a2, int r2_0, int a4);
BattleMon* ServerControl_SimpleEffect(ServerFlow* a1, unsigned __int16* a2, BattleMon* a3, PokeSet* a4);
PartyPkm** ServerControl_SimpleRecover(ServerFlow* a1, int a2, int a3, int a4);
void ServerControl_EffectCondition(ServerFlow* a1, unsigned __int16* a2, BattleMon* a3, PokeSet* a4);
BattleMon* ServerControl_OHKO(ServerFlow* r0_0, unsigned __int16* a2, BattleMon* a3, _BYTE* a4);
char* ServerControl_FieldEffect(ServerFlow* a1, unsigned __int16* a2, BattleMon* a3);
void ServerDisplay_UncategorizedMove(ServerFlow* a1, MoveParam* a2, BattleMon* a3, _BYTE* a4);
void ServerControl_ForceSwitch(ServerFlow* a1, int a2, int a3, int a4);
void j_j_MoveRecord_SetEffectiveLast(_DWORD* a1);
void ServerDisplay_SetConditionFlag(ServerFlow* a1, BattleMon* a2, ConditionFlag a3);
_DWORD* ServerControl_MoveUseEffective(ServerFlow* a1, int a2, int a3);
_DWORD* ServerControl_MoveUseNotEffective(ServerFlow* a1, int a2, int a3);
_DWORD* ServerControl_MoveUseEnd(ServerFlow* a1, int a2, int a3);
int ServerControl_CheckFainted(ServerFlow* a1, BattleMon* a2);

// ActionOrder_Proc definitions
void PSetStack_Push(ServerFlow* result);
bool j_j_PosPoke_IsExist(PosPoke* a1, int a2);
bool BattleMon_CheckIfMoveCondition(BattleMon* a1, MoveCondition a2);
int MainModule_GetEscapeMode(MainModule* a1);
_DWORD* sub_21A09D4(ServerFlow* a1, int a2, int a3);
void TurnFlag_Set(BattleMon* a1, TurnFlag a2);
_DWORD* ServerControl_BeforeFirstFight(ServerFlow* a1);
int ServerFlow_CheckActionIgnoreReason(ServerFlow* a1, BattleMon* a2);
unsigned int ServerControl_Fight(ServerFlow* a1, BattleMon* a2, int* a3, int a4);
int ServerControl_UseTrainerItem(ServerFlow* a1, int r1_0, int a3, int a4, unsigned int a5);
int ServerControl_RunCore(ServerFlow* a1, BattleMon* a2, int a3, int a4);
char* ServerControl_Switch(ServerFlow* a1, BattleMon* a2, int a3);
int ServerControl_AfterSwitchIn(ServerFlow* a1);
int ServerControl_Escape(ServerFlow* a1, int a2);
_DWORD* sub_21A0ED0(ServerFlow* a1, int a2);
BattleStyle BtlSetup_GetBattleStyle(MainModule* a1);
void ServerControl_GravityCheck(ServerFlow* a1, int a2, int a3);
void sub_21A9014(ServerFlow* a1, int a2);
void ServerEvent_ActProcEnd(ServerFlow* a1, BattleMon* a2, int a3);
void ServerControl_SetMonCounter(ServerFlow* a1, BattleMon* a2, Counter a3, int a4);
int BattleMon_GetCount(BattleMon* a1, int a2);
void PSetStack_Pop(int result);

// ServerFlow_ActOrderProcMain definitions
int ServerControl_TurnCheck(ServerFlow* a1);
bool ServerControl_CheckMatchup(ServerFlow* a1);
int j_j_DeadRec_GetCount_1(int a1, unsigned int a2);
int Handler_IsPosOpenForRevivedMon(ServerFlow* a1);
int ServerFlow_ReqChangePokeForServer(ServerFlow* a1, _BYTE* a2);
unsigned int ServerDisplay_IllusionSet(ServerFlow* a1, unsigned __int8* a2);
_DWORD* ServerControl_CheckActivation(ServerFlow* a1);
unsigned int sub_219FB7C(ServerFlow* a1, int a2, unsigned int a3);
int ActionOrder_Proc(ServerFlow* a1, int a2);
int ServerControl_CheckExpGet(ServerFlow* a1);
bool CheckHPDebug(BattleMon* a1);

// ServerControl_Fight definitions
ConditionFlag BattleMon_GetHideCondition(BattleMon* a1);
MoveAnimCtrl* MoveAnimCtrl_Init(MoveAnimCtrl* result);
void MoveStealParam_Init(MoveStealParam* a1);
int CheckEncoreMoveChange(BattleMon* a1, _DWORD* a2);
int AutoDecideMoveTarget(MainModule* a1, PokeCon* a2, BattleMon* a3, int a4);
void BattleAction_SetFightParam(BattleActionParam* a1, int a2, int a3);
int Move_SearchIndex(BattleMon* a1, int a2);
int MoveEvent_AddItem(BattleMon* a1, int a2, int a3);
_DWORD* ServerControl_StartMoveSeq(ServerFlow* a1, int a2, int a3);
int ServerControl_MoveExecuteCheck1(ServerFlow* a1, BattleMon* a2, int a3, int a4);
int ServerEvent_GetReqMoveParam(ServerFlow* a1, BattleMon* a2, int a3, int a4, ReqMoveWork* a5);
void ServerDisplay_MoveMessage(ServerFlow* a1, int a2, int a3);
void ServerControl_ReqMoveEffect(ServerFlow* a1, int a2, int a3, int a4);
void ServerEvent_GetMoveParam(ServerFlow* a1, int a2, int a3, MoveParam* a4);
void ServerEvent_GetMoveParam(ServerFlow* a1, int a2, int a3, MoveParam* a4);
int ServerControl_CheckReqMoveFail(ServerFlow* a1, BattleMon* a2, unsigned __int16* a3);
int ServerControl_DecrementPPUsedMove(ServerFlow* a1, PartyPkm** a2, int a3, unsigned int a4, int a5);
void j_j_PokeSet_Clear_3(const void* a1);
void j_j_PokeSet_Clear_4(const void* a1);
unsigned int ServerFlowSub_RegisterTargets(ServerFlow* a1, int a2, int a3, int a4, int a5);
int j_PokeSet_GetCountMax_0(int a1);
int j_PokeSet_RemoveDisablePoke_0(_BYTE* a1);
int j_PokeSet_CopyAlive(int a1, _BYTE* a2);
int j_PokeSet_SetDefaultTargetCount(int a1, char a2);
_DWORD* ServerControl_MoveCallDecide(ServerFlow* a1, int a2, MoveParam* a3, int a4);
void ServerControl_CheckComboMoveExe(ServerFlow* a1, int a2, MoveParam* a3);
int ServerEvent_MoveRequestMessage(ServerFlow* r0_0, BattleMon* a2, int a3, int a4, _WORD* a1);
int BattleHandler_SetString(ServerFlow* a1, HandlerParam_StrParams* a2);
void BattleHandler_StrClear(HandlerParam_StrParams* a1);
void ServerControl_CureCondition(ServerFlow* result, BattleMon* a2, MoveCondition a3, int* a4);
int ServerControl_MoveExecuteCheck2(ServerFlow* a1, BattleMon* a2, int a3);
int ServerControl_CheckDelayMoveSet(ServerFlow* a1, int a2, int a3, int a4, _BYTE* a5);
int ServerControl_CheckComboMoveReady(ServerFlow* a1, BattleMon* a2, int a3);
void j_WazaRec_Add(MoveRecord* result, __int16 a2, unsigned int a3, char a4);
int ServerControl_CheckStealMove(ServerFlow* a1, int a2, int a3, int a4, MoveStealParam* a5);
void ServerControl_StealMoveRoot(ServerFlow* a1, int a2, int a3, int a4);
int ServerControl_MoveStart(ServerFlow* flowWork, BattleMon* attacker, int MoveID, PokeSet* targetRec);
_DWORD* ServerControl_MoveExecuteFail(ServerFlow* a1, BattleMon* a2, int a3, int a4);
void BattleMon_UpdateMoveProcessingResult(BattleMon* result, char a2, int a3, char a4, unsigned __int16 a5, __int16 a6);
void ServerDisplay_AddCommon(ServerCommandQueue* a1, int a2, ...);
ConditionFlag ServerControl_ChargeUpLockClear(ServerFlow* a1, BattleMon* a2);
bool BattleMon_GetTurnFlag(BattleMon* a1, TurnFlag a2);
void ServerControl_SkyDropCheckRelease(ServerFlow* a1, BattleMon* a2, unsigned int a3);
_DWORD* ServerControl_EndMoveSeq(ServerFlow* a1, int a2, int a3, int a4);
BattleEventItem* MoveEvent_RemoveItem(BattleMon* a1, BattleEventItem* a2);
unsigned int ServerControl_MagicCoat(ServerFlow* a1, int a2);

// dont remember definitions
int BattleRandom(unsigned int a1);
PartyPkm* BattleMon_GetSrcData(BattleMon* a1);
b32 PokeParty_DecryptPkm(PartyPkm* pPkm);
u32 PokeParty_GetParam(PartyPkm* pPkm, PkmField field, void* extra);
b32 PokeParty_EncryptPkm(PartyPkm* pPkm, b32 encrypt);
u32 BattleField_CheckEffect(BattleFieldEffect a1);
ItemID BattleMon_GetHeldItem(BattleMon* a1);

// PokeList_ApplyItemEffect definitions
void* PML_ItemReadDataFile(unsigned int a1, int a2, HeapID a3);
int PML_ItemGetParam(ItemData* itemData, ItemField itemField);
void PokeParty_SetParam(PartyPkm* pPkm, PkmField field, u32 data);
void PokeList_ApplyHeal(PartyPkm* a1, int a2, unsigned int a3, unsigned int a4);
void PokeParty_RecalcStats(PartyPkm* pPoke);
int PokeList_ApplyMaxPPIncrease(PartyPkm* a1, int a2, int a3);
int PokeParty_AddPP(PartyPkm* pkm, int moveIdx, int addend);
int PokeList_GetEVsAppliedAfterLimit(int a1, int a2, int a3, int a4);
int PokeList_ApplyFriendship(PartyPkm* a1, int a2, int a3, u16 a4, unsigned __int16 a5, HeapID a6); 

// GFL_ArcSysReadHeapNewImpl definitions
void GFL_ArcSysInitArcHandle(ArcTool* arc, int arcId);
int ArchiveMoveImageTop(FSFile* p_file, u16 fileNum, int seek_length, int read_length);

// PokeList_CanItemWithBattleStatsBeUsed definitions
u32 PokeParty_GetStatusCond(PartyPkm* a1);
int PML_MoveGetMaxPP(int wazaId, unsigned int ppUpStage);
int PokeParty_CheckPPNeedsReplenish(PartyPkm* pkm, int moveSlot);
bool PokeList_CanFriendshipBeIncreased(PartyPkm* a1, ItemData* a2);

// BattleHandler_SendToLast definitions
int ActionOrder_SendToLast(ServerFlow* a1, int a2);

// BattleHandler_InterruptAction definitions
bool ActionOrder_InterruptReserve(ServerFlow* a1, int a2);

// sub_219B30C definitions
void sub_219CF2C(PokeList* a1);
int sub_219B8F0(PokeList* a1);
void sub_219CF2C(PokeList* a1);
void GFL_BGSysLoadScr(G2DBG bg);
bool PokeList_IsPPRestoringItem(PokeList* a1, unsigned int a2);
bool PokeList_CanItemWithBattleStatsBeUsed(PartyPkm* a1, unsigned int a2, int a3, unsigned __int16 a4);
ItemRestoreType PokeList_PrintItemRecoverMessage(PokeList* a1, u16 a2);
int PokeList_ApplyItemEffect(PartyPkm* a1, unsigned int a2, int a3, unsigned __int16 a4, int a5);
void sub_2038BF4(int a1);
void sub_219F290(PokeList* a1, PokeList_Plate* a2, PartyPkm* a3, int a4);
void sub_219F350(PokeList* a1, PokeList_Plate* a2);
void GFL_SndSEPlay(SoundResID sndId);
int PokeList_SubItem(PokeList* a1, u16 a2);
bool PokeList_DoesShayminNeedFormeChange(PokeList* a1);
int PokeList_ChangeShayminToSkyForme(PokeList* a1, PartyPkm* a2);
bool PokeList_IsRevealGlassMon(int a1, PartyPkm* a2);
b32 isOneShotDRObtained(void* pTrainerCard, int idx, void* pTrainerInfo);
int PokeList_ChangeRevealGlassMonForme(PokeList* a1, PartyPkm* a2);
int PokeList_GetKyuremUnfuseCase(PokeList* a1);
void sub_21A03DC(PokeList* a1);
int PokeList_GetKyuremFuseCase(PokeList* a1);
void sub_21A03EC(PokeList* a1, int a2);
int sub_219CE18(PokeList* a1, int a2);
int sub_219CE74(PokeList* a1, int a2);
void sub_219F7AC(PokeList* a1, PokeList_Message* a2, unsigned int a3);
void sub_219F880(PokeList* a1, PokeList_Message* a2, int a3);
void sub_219BBA8(PokeList* a1);
int PokeList_GetLearnMoveCase(PokeList* a1, PartyPkm* a2, char a3);
void PokeListMessage_CreateWordSetSystem(PokeList* a1, PokeList_Message* a2);
void PokeListMessage_LoadPokemonNicknameToStrBuf(PokeList* a1, PokeList_Message* a2, int a3, PartyPkm* a4);
int PokeListMessage_LoadMoveNameToStrBuf(PokeList* a1, PokeList_Message* a2, u8 a3, u16 a4);
void PokeList_PrintMessage(PokeList* a1, int a2, int a3, int a4);
void PokeListMessage_ClearWordSetSystem(PokeList* a1, PokeList_Message* a2);
void sub_219E5C8(PokeList* a1, PartyPkm* a2);
int sub_219E620(PokeList* a1, int a2);
int sub_219DE50(int result);
unsigned __int8* sub_219DEC4(PokeList* a1);
int PML_ItemIsMail(u16 itemId);
int sub_219FA58(PokeList* a1, PokeList_Message* a2, int a3, int a4);
unsigned __int8* sub_219E0AC(PokeList* a1);
void PokeList_ChangeHeldItem(PokeList* a1, PartyPkm* a2, u32 a3);
int PokeListMessage_LoadItemNameToStrBuf(PokeList* a1, PokeList_Message* a2, int a3, int a4);
void PokeList_ChangeArceusForme(PokeList* a1, PartyPkm* a2, u16 a3);
void PokeList_ChangeGenesectForme(PokeList* a1, PartyPkm* a2, int a3);
bool PokeList_DoesGiratinaNeedFormeChange(PokeList* a1);
int PokeList_ChangeGiratinaToOriginForme(PokeList* a1, PartyPkm* a2);
int sub_219E4F8(PokeList* a1);
MailData* sub_20097F4(int a1, int a2, int a3, HeapID a4);
MailData* sub_20097D0(int a1, int a2, int a3);
int PokeList_CheckEvolveSpecies(PokeList* a1, int a2, int a3);

// ShortcutSave_SetKeyItemRegistration definitions
void ShortcutSave_Condense(ShortcutSave* save);

// s002A_WorkSet definitions
u16* ScriptReadVar(ScriptVM* vm, FieldScriptEnv* cmd);
u16 ScriptReadAny(ScriptVM* vm, FieldScriptEnv* cmd);

// PokeList_SubItem definitions
int BagSave_SubItem(BagSaveData* bag, u16 itemId, u16 quantity, HeapID heapId);

// CreateBagItemUseEvent definitions
int sub_215B54C(GameEvent* a1, int* a2, int a3);
void memsetx(void* ptr, u8 value, u32 size);
int sub_215B7AC(_DWORD* a1, int a2, int a3, int a4, int a5, int a6);
void PlayerActionPerms_Create(PlayerActionPerms* perms, GameSystem* gsys, Field* field);
void CalcPlayerActionPossibilities(Field* field, PlayerActionPossibilities* dest);

// EventFieldItemUseBlock_Call definitions
PlayerState* GameData_GetPlayerState(GameData* gameData);
PlayerSpecialState GetPlayerSpecialState(PlayerState* playerState);
GameEvent* EventFieldItemUseBlock_Create(GameSystem* a1, FieldItemBlockReason reason, u16 itemId);

// CallYButtonShortcutMenu definitions
SaveControl* GameData_GetSaveControl(GameData* gameData);
ShortcutSave* SaveControl_GetShortcutSave(SaveControl* saveCtrl);
int ShortcutSave_GetShortcutCount(ShortcutSave* shortcuts);
int EventShortcutCallDirect_Callback(GameEvent* event, int* pState, EventShortcutMenu* data);
int EventShortcutChoicePopup_Callback(GameEvent* a1, int* a2, EventShortcutMenu* data);
int sub_215B36C(int a1, EventShortcutMenu* a2);
int sub_215B3DC(int a1, int a2);
FieldActorSystem* Field_GetActorSystem(Field* a1);
void DisableAllActorsMovement(FieldActorSystem* a1);
void sub_203D590(char result);

// GameData_SetKeyItemRegistration definitions
ShortcutSave* SaveControl_GetShortcutSaveCore(SaveControl* saveCtrl);
void ShortcutSave_SetKeyItemRegistration(ShortcutSave* result, int keyItemID, b32 isRegistered);

// sub_21BF3E0 definitions
int sub_2025B20(int a1, _WORD* a2);
int sub_2025B84(int a1, unsigned int a2);
InputButton GCTX_HIDGetTypedKeys();
int sub_21BF2DC(int a1, int a2, __int16 a3, _DWORD* a4);
int sub_21BF36C(int a1, int a2, int a3, _DWORD* a4);
int BmpMenuList_Update(BmpMenuList* menuList);
InputButton GCTX_HIDGetPressedKeys();
int sub_2025B78(_DWORD* a1, int a2);
int ShortcutSave_MoveItemToIndex(ShortcutSave* shortcuts, u8 keyItemId, signed int destIndex);
int sub_21BF848(int a1, _WORD* a2, _WORD* a3);
void sub_21BF770(ListMenuOption** a1, int a2, int a3, int a4);
int sub_2025B30(int result, _WORD* a2, _WORD* a3);
void sub_204C150(int result, int a2);
int sub_21BF620(int a1, int a2, int a3, int a4);

// sub_215BA0C definitions
void GameData_SetKeyItemRegistration(GameData* gameData, int keyItemId, b32 isRegistered);

// s02C2_RepelRearm definitions
HeapID FieldScriptEnv_GetHeapID(FieldScriptEnv* env);
GameData* FieldScriptEnv_GetGameData(FieldScriptEnv* cmd);
EncountSave* SaveControl_GetEncountSave(SaveControl* saveCtrl);
BagSaveData* GameData_GetBag(GameData* gameData);
u16 EncountSave_GetUsedRepelItemID(EncountSave* encountSave);
b32 EncountSave_IsRepelDepleted(EncountSave* encountSave);
int GetItemParam(u16 itemId, ItemField itemField, HeapID heapId);
void EncountSave_SetRepelSteps(EncountSave* encountSave, u8 steps);
void sub_202D3B0(unsigned int itemId);

// CheckRepelEvent definitions
bool EncountSave_DecrementRepelSteps(EncountSave* encountSave);
HeapID Field_GetHeapID(Field* field);
bool BagSave_GetItemCountByID(BagSaveData* bag, u16 itemId);
GameEvent* EventScriptCall_Create(GameSystem* gsys, u16 scrId, FieldActor* actor, HeapID heapId);

// EventScriptCall_CreateCore definitions
b32 EventScriptCall_Callback(GameEvent* event, int* state, EventScriptCall* evData);
FieldScriptSupervisor* FieldScriptSupervisor_Create(HeapID heapId);
ScriptWork* ScriptWork_Create(u32 heapId, GameSystem* gameSys, GameEvent* event, u16 scrId, int a5, FieldScriptFeatureLevel featureLevel);
void ScriptWork_SetParentActor(ScriptWork* work, FieldActor* mmdl);
int FieldScript_GetZoneIDFromGSys(GameSystem* gsys);
int ScriptWork_AddVM(ScriptWork* swk, u16 zoneId, u16 SCRID);

// EventFieldItemUseBlock_Create definitions
ScriptWork* EventScriptCall_GetWork(GameEvent* eventMainBlock);
void ScriptWork_SetParams(ScriptWork* work, u16 param1, u16 param2, u16 param3, u16 param4);

// EventFieldItemUseBlock_Call definitions
PlayerExState FieldPlayerState_GetExState(PlayerState* playerState);

// BagSave_SubItem definitions
BagItem* BagSave_GetItemHandleIfAmount(BagSaveData* bag, u16 itemId, u16 quantity);
PocketType BagSave_GetPocketPtrByItem(BagSaveData* bag, u16 itemId, BagItem** pPocket, u32* pPocketLimit);
void BagSave_Defragment(BagItem* pocket, int pocketLimit);

// Flag functions definitions
b32 EventWork_FlagGet(EventWorkSave* eventWork, u32 flagID);
void EventWork_FlagSet(EventWorkSave* eventWork, u32 flagID);
void EventWork_FlagReset(EventWorkSave* eventWork, u32 flagID);

// RepelEffect definitions
bool IsItemRepel(__int16 a1);
void GFL_MsgDataLoadStrbuf(MsgData* msgdata, int msgID, StrBuf* strbuf);
void copyVarForText(WordSetSystem* wordSet, int wsBufNo, void* a3);
int GFL_WordSetFormatStrbuf(WordSetSystem* wordSet, StrBuf* dest, StrBuf* source);
void playSeqFullVol(int a1);

// UpdatePhenomenon functions definitions
EncountState* GameData_GetEncountState(GameData* a1);
signed int EncSys_IsActive(EncountSystem* encsys);
MapMtxSys* GetMapMatrixSystem(GameData* pBaseBlk);
u16 Field_GetPlayerStateZoneID(Field* field);
FieldPlayer* Field_GetPlayer(Field* field);
FieldActor* FieldPlayer_GetActor(FieldPlayer* player);
void CopyActorWPos(FieldActor* mmdl, VecFx32* dest);
bool RangeCheckChunkCoordinateWorld(MapMtxSys* a1, int a2, int a3);
int GetZoneIDAtMatrixXZWorld(MapMtxSys* a1, fx32 x, fx32 z);
void* getTrainerCardDataBlkAddress(void* pBaseBlk);
bool isBadgeObtained(void* pTrainerCardData, int badgeNum);
FieldmapCtrlType Field_GetResolvedControllerTypeID(Field* a1);
bool sub_21A24AC(EncountState* a1, int a2);
u32 PassPower_ApplyExploringChance(u32 basePhenomenonChance);
u32 GFL_RandomLCAlt(u32 max);
int positionShakingSpot(EncountSystem* encSys, int a2, u8 type);
void sub_21A272C(void** a1, int a2);
void setShakingSpotOff(EncountState* result);

// some register expansion related definitions
GameData* GSYS_GetGameData(GameSystem* gsys);
PokeParty* GameData_GetParty(GameData* pBaseBlk);
_DWORD* sub_2034C80(GameData* a1, int a2, int a3, HeapID a4);
bool GFL_OvlLoad(u32 ovlId);
void GFL_OvlUnload(u32 ovlId);
GameEvent* CreateHidenEvent(HidenEventID eventId, int a2, PlayerActionPossibilities* actionOpts);

// EventShortcutChoicePopup_Callback definitions
void sub_215B2C4(int a1, EventShortcutMenu* a2);
int ShortcutMenu_GetActionFromKeyItem(RegistrableID registrableID, FieldPlayerAction* a2, _DWORD* a3);
bool ShortcutMenu_SetKeyItemID(ShortcutMenuWork* wk, RegistrableID registrableID);
int sub_20175E4(GameData* a1);
int CheckAnyRibbon(EventShortcutMenu* data);
int PlayerActionPerms_IsActionBlocked(PlayerActionPerms* perms, int actionNo);
int sub_215B310(EventShortcutMenu* data);
void EnableAllActorsMovement(MModelSystem* a1);
GameEvent* CreateBagItemUseEvent(ShortcutMenuWork* a1, u16 a2);
GameEvent* CallFieldCommonEventFunc(FieldCommonEventID cmnEventId, GameSystem* gsys, Field* field);
void EventFieldItemUseBlock_Call(GameEvent* parentEvent, GameSystem* gsys, FieldCommonEventID eventId, bool isBlockedByPerms);
int sub_2159458(int result, __int16 a2, __int16 a3, int a4);
void sub_215B2C4(int a1, EventShortcutMenu* a2);
int sub_21BF1E4(int a1);
unsigned int sub_21BF0C0(int a1);
int sub_21BF21C(int a1);
int sub_21BF230(int a1, _DWORD* a2);
b32 GameData_IsForceSeasonSync(GameData* gameData);
_WORD* sub_21BF208(_WORD* result);
GameEvent* EventFieldAppCall_Create(AppCallFramework* framework, HeapID heapId);

// FIELD_COMMON_EVENTS definitions
GameEvent* EventFieldToggleCycling_Create(Field* field, GameSystem* gsys);
GameEvent* EventEntralinkWarpIn_CreateDefault(Field* field, GameSystem* gsys);
GameEvent* EventMapChangeEscapeRope_Create(Field* field, GameSystem* gsys);
GameEvent* EventSweetScent_Create(Field* field, GameSystem* gsys);
GameEvent* EventFieldFishing_Create(Field* field, GameSystem* gsys);
GameEvent* EventFieldToggleDowsing_Create(Field* field, GameSystem* gsys);

int ItemUseFunction_Bicycle(int a1);
int ItemUseFunction_TownMap(int a1);
int ItemUseFunction_PalPad(int a1);
int ItemUseFunction_Honey(int a1);
int ItemUseFunction_SuperRod(int a1);
int ItemUseFunction_VsRecorder(int a1);
int ItemUseFunction_DowsingMCHN(int a1);
int ItemUseFunction_EscapeRope(int a1);
int ItemUseFunction_Xtransceiver(int a1);
int ItemUseFunction_MedalBox(int a1);
bool SaveControl_IsItemRegistered(GameData* gameData, RegistrableID registrableID);

// GetRegistrableID / Bag_GetRegistrableID definitions
int Bag_LoadItemName(BigBagStructTemp* a1, int a2, int a3);
void Bag_CreateTextBox(BigBagStructTemp* a1, int a2);
int sub_2199900(_DWORD* a1, int a2);
int sub_219A22C(int a1);

// setItemDescriptionTextToStrbuf definitions
MsgData* GFL_MsgSysLoadData(bool isPreloadAll, int arcId, int fileId, HeapID heapId);
void GFL_MsgDataFree(MsgData* msgdata);

// PML_PkmSetParamCore definitions
void* PML_PkmGetParamBlockCore(BoxPkm* pkm, u32 pid, int blk_num);
GenderSet PML_UtilDerivePkmSex(u16 species, u16 form, u32 pid);
void GFL_StrBufStoreString(StrBuf* strbuf, wchar_t* dest, int length);
bool PML_PkmIsNicknamed(BoxPkm* pPkm);
void wcharsncpy(wchar_t* pSrc, wchar_t* pDest, int length);

// PML_PkmGetParamCore definitions
u16 PML_CryptoGenKey(void* area, int length);

// BagSave_GetFreeSpaceItemCount definitions
bool BagSave_IsItemInFreeSpace(BagSaveData* bag, u32 itemId);
void GFL_StrBufLoadString(StrBuf* strbuf, wchar_t* src);
void GFL_MsgDataLoadRawStr(MsgData* msgdata, int msgId, wchar_t* string, int length);
u32 CalcLevelByExp(u16 species, u16 form, u32 experience);
PokeType PML_ItemGetArceusPlateType(u16 itemId);
u32 PML_PersonalGetParamSingle(u16 species, u16 form, PersonalField field);

// PokeList_ActionMenu definitions
void sub_219B1DC(PokeList* a1);
PartyPkm* PokeParty_GetPkm(PokeParty* party, int slot);
u8* sub_219FB94(PokeList* a1, PokeList_Menu* a2, _DWORD* a3);
bool PokeList_GetItemCountByID(PokeList* a1, u16 a2);
void sub_219DE34(PokeList* a1);
int PokeList_IsGiratinaInOriginForme(PokeList* a1);
int PokeList_ChangeGiratinaToAlteredForme(PokeList* a1, PartyPkm* a2);
int PokeList_GetFieldMoveID(PartyPkm* a1, int a2);
bool CheckAllowHidenEvent(int a1, PlayerActionPossibilities* a2);
bool sub_2018C64(int a1);
int sub_219F530(PokeList* a1, PokeList_Plate* a2);
void sub_219F4D0(PokeList* a1, PokeList_Plate* a2, int a3);
char* sub_219F008(PokeList* a1, PokeList_Plate* a2, int a3);
void sub_219FF18(int result, int a2);
int sub_219F4CC(PokeList_Plate* a1);

// PML_ItemGetParam definitions
int PML_ItemGetBattleStat(ItemBattleStats* btlStats, ItemField stat);

// PokeList_PrintItemRecoverMessage definitions
ItemRestoreType PokeList_GetItemRestoreType(ItemID itemID);
int PokeListPlate_GetCurrentHP(PokeList* a1, PokeList_Plate* a2);
void sub_21A0678(PokeList* a1);
u32 PokeParty_GetLevel(PartyPkm* pPkm);
void PokeListMessage_WordSetNumber(PokeList* a1, PokeList_Message* a2, int a3, int a4, unsigned __int8 a5);
void sub_21A0750(PokeList* a1);
StrBuf* GFL_StrBufCreate(int charCount, HeapID heapId);
int sub_202D2F4(int a1);
void GFL_StrBufFree(StrBuf* pStrbuf);
void PokeList_PrintRecoverHPMessage(PokeList* a1);
bool PokeList_DoesPkmHaveParam(PartyPkm* a1, PkmField a2);
void sub_21A0D28(PokeList* a1, int a2, unsigned __int8 a3);
void sub_219DE58(PokeList* a1);
void sub_21A0CE8(PokeList* a1, int a2);

void GFL_WordSetCopyStrbuf(WordSetSystem* wordSet, int bufId, StrBuf* source, u8* flags);
PersonalData* PML_PersonalLoadBW2(u16 species, u16 form);
u32 PML_PersonalGetParam(PersonalData* personal, PersonalField field);
int loadAbilityNameToStrbuf(WordSetSystem* wordset, int buffid, int msgID);
    
// sub_219B8F0 definitions
int PokeListPlate_IsEgg(PokeList* a1, PokeList_Plate* a2);
int PokeList_GetPPRestoreStrID(PokeList* a1, unsigned int a2);

// BattleMon_Create definitions
void BattleMon_SetupBySrcData(BattleMon* a1, PartyPkm* a2, int a3, int a4);
int BattleMon_GetNumMoves(BattleMon* a1, PartyPkm* a2, int a3);
void j_ResetStatStages(StatStageParam* result);
void ClearMoveStatusWork(BattleMon* a1, int a2);
u32 GetStatusCond(PartyPkm* a1);
ConditionData MakeBasicStatus(MoveCondition a1);
void MoveDamageRec_Init(BattleMon* a1);
void BattleMon_ClearConfrontRecCount(BattleMon* a1);

// ITEM_EVENT_ADD_TABLE definitions
BattleEventHandlerTableEntry* EventAddCheriBerry(int* a1);
BattleEventHandlerTableEntry* EventAddChestoBerry(int* a1);
BattleEventHandlerTableEntry* EventAddRawstBerry(int* a1);
BattleEventHandlerTableEntry* EventAddAspearBerry(int* a1);
BattleEventHandlerTableEntry* EventAddPersimBerry(int* a1);
BattleEventHandlerTableEntry* EventAddPechaBerry(int* a1);
BattleEventHandlerTableEntry* EventAddLumBerry(int* a1);
BattleEventHandlerTableEntry* EventAddLeppaBerry(int* a1);
BattleEventHandlerTableEntry* EventAddOranBerry(int* a1);
BattleEventHandlerTableEntry* EventAddBerryJuice(int* a1);
BattleEventHandlerTableEntry* EventAddSitrusBerry(int* a1);
BattleEventHandlerTableEntry* EventAddFigyBerry(int* a1);
BattleEventHandlerTableEntry* EventAddWikiBerry(int* a1);
BattleEventHandlerTableEntry* EventAddMagoBerry(int* a1);
BattleEventHandlerTableEntry* EventAddAguavBerry(int* a1);
BattleEventHandlerTableEntry* EventAddIapapaBerry(int* a1);
BattleEventHandlerTableEntry* EventAddLiechiBerry(int* a1);
BattleEventHandlerTableEntry* EventAddGanlonBerry(int* a1);
BattleEventHandlerTableEntry* EventAddSalacBerry(int* a1);
BattleEventHandlerTableEntry* EventAddPetayaBerry(int* a1);
BattleEventHandlerTableEntry* EventAddApicotBerry(int* a1);
BattleEventHandlerTableEntry* EventAddLansatBerry(int* a1);
BattleEventHandlerTableEntry* EventAddStarfBerry(int* a1);
BattleEventHandlerTableEntry* EventAddEnigmaBerry(int* a1);
BattleEventHandlerTableEntry* EventAddOccaBerry(int* a1);
BattleEventHandlerTableEntry* EventAddPasshoBerry(int* a1);
BattleEventHandlerTableEntry* EventAddWacanBerry(int* a1);
BattleEventHandlerTableEntry* EventAddRindoBerry(int* a1);
BattleEventHandlerTableEntry* EventAddYacheBerry(int* a1);
BattleEventHandlerTableEntry* EventAddChopleBerry(int* a1);
BattleEventHandlerTableEntry* EventAddKebiaBerry(int* a1);
BattleEventHandlerTableEntry* EventAddShucaBerry(int* a1);
BattleEventHandlerTableEntry* EventAddCobaBerry(int* a1);
BattleEventHandlerTableEntry* EventAddPayapaBerry(int* a1);
BattleEventHandlerTableEntry* EventAddTangaBerry(int* a1);
BattleEventHandlerTableEntry* EventAddChartiBerry(int* a1);
BattleEventHandlerTableEntry* EventAddKasibBerry(int* a1);
BattleEventHandlerTableEntry* EventAddHabanBerry(int* a1);
BattleEventHandlerTableEntry* EventAddColburBerry(int* a1);
BattleEventHandlerTableEntry* EventAddBabiriBerry(int* a1);
BattleEventHandlerTableEntry* EventAddChilanBerry(int* a1);
BattleEventHandlerTableEntry* EventAddCustapBerry(int* a1);
BattleEventHandlerTableEntry* EventAddMicleBerry(int* a1);
BattleEventHandlerTableEntry* EventAddJabocaBerry(int* a1);
BattleEventHandlerTableEntry* EventAddRowapBerry(int* a1);
BattleEventHandlerTableEntry* EventAddWhiteHerb(int* a1);
BattleEventHandlerTableEntry* EventAddMentalHerb(int* a1);
BattleEventHandlerTableEntry* EventAddBrightPowder(int* a1);
BattleEventHandlerTableEntry* EventAddMachoBrace(int* a1);
BattleEventHandlerTableEntry* EventAddQuickClaw(int* a1);
BattleEventHandlerTableEntry* EventAddLaggingTail(int* a1);
BattleEventHandlerTableEntry* EventAddLaggingTail(int* a1);
BattleEventHandlerTableEntry* EventAddKingsRock(int* a1);
BattleEventHandlerTableEntry* EventAddScopeLens(int* a1);
BattleEventHandlerTableEntry* EventAddWideLens(int* a1);
BattleEventHandlerTableEntry* EventAddScopeLens(int* a1);
BattleEventHandlerTableEntry* EventAddZoomLens(int* a1);
BattleEventHandlerTableEntry* EventAddLaxIncense(int* a1);
BattleEventHandlerTableEntry* EventAddMuscleBand(int* a1);
BattleEventHandlerTableEntry* EventAddWiseGlasses(int* a1);
BattleEventHandlerTableEntry* EventAddDeepSeaTooth(int* a1);
BattleEventHandlerTableEntry* EventAddDeepSeaScale(int* a1);
BattleEventHandlerTableEntry* EventAddMetalPowder(int* a1);
BattleEventHandlerTableEntry* EventAddQuickPowder(int* a1);
BattleEventHandlerTableEntry* EventAddLightBall(int* a1);
BattleEventHandlerTableEntry* EventAddLuckyPunch(int* a1);
BattleEventHandlerTableEntry* EventAddStick(int* a1);
BattleEventHandlerTableEntry* EventAddSoulDew(int* a1);
BattleEventHandlerTableEntry* EventAddThickClub(int* a1);
BattleEventHandlerTableEntry* EventAddChoiceBand(int* a1);
BattleEventHandlerTableEntry* EventAddBlackSludge(int* a1);
BattleEventHandlerTableEntry* EventAddChoiceSpecs(int* a1);
BattleEventHandlerTableEntry* EventAddChoiceScarf(int* a1);
BattleEventHandlerTableEntry* EventAddSilverPowder(int* a1);
BattleEventHandlerTableEntry* EventAddSoftSand(int* a1);
BattleEventHandlerTableEntry* EventAddHardStone(int* a1);
BattleEventHandlerTableEntry* EventAddMiracleSeed(int* a1);
BattleEventHandlerTableEntry* EventAddBlackGlasses(int* a1);
BattleEventHandlerTableEntry* EventAddBlackBelt(int* a1);
BattleEventHandlerTableEntry* EventAddMagnet(int* a1);
BattleEventHandlerTableEntry* EventAddMetalCoat(int* a1);
BattleEventHandlerTableEntry* EventAddMysticWater(int* a1);
BattleEventHandlerTableEntry* EventAddSharpBeak(int* a1);
BattleEventHandlerTableEntry* EventAddRazorFang(int* a1);
BattleEventHandlerTableEntry* EventAddPoisonBarb(int* a1);
BattleEventHandlerTableEntry* EventAddNeverMeltIce(int* a1);
BattleEventHandlerTableEntry* EventAddSpellTag(int* a1);
BattleEventHandlerTableEntry* EventAddTwistedSpoon(int* a1);
BattleEventHandlerTableEntry* EventAddCharcoal(int* a1);
BattleEventHandlerTableEntry* EventAddDragonFang(int* a1);
BattleEventHandlerTableEntry* EventAddSilkScarf(int* a1);
BattleEventHandlerTableEntry* EventAddOddIncense(int* a1);
BattleEventHandlerTableEntry* EventAddRockIncense(int* a1);
BattleEventHandlerTableEntry* EventAddWaveIncense(int* a1);
BattleEventHandlerTableEntry* EventAddSeaIncense(int* a1);
BattleEventHandlerTableEntry* EventAddRoseIncense(int* a1);
BattleEventHandlerTableEntry* EventAddFocusSash(int* a1);
BattleEventHandlerTableEntry* EventAddFocusBand(int* a1);
BattleEventHandlerTableEntry* EventAddExpertBelt(int* a1);
BattleEventHandlerTableEntry* EventAddLifeOrb(int* a1);
BattleEventHandlerTableEntry* EventAddMetronomeItem(int* a1);
BattleEventHandlerTableEntry* EventAddGripClaw(int* a1);
BattleEventHandlerTableEntry* EventAddShellBell(int* a1);
BattleEventHandlerTableEntry* EventAddLightClay(int* a1);
BattleEventHandlerTableEntry* EventAddPowerHerb(int* a1);
BattleEventHandlerTableEntry* EventAddLeftovers(int* a1);
BattleEventHandlerTableEntry* EventAddToxicOrb(int* a1);
BattleEventHandlerTableEntry* EventAddFlameOrb(int* a1);
BattleEventHandlerTableEntry* EventAddLustrousOrb(int* a1);
BattleEventHandlerTableEntry* EventAddAdamantOrb(int* a1);
BattleEventHandlerTableEntry* EventAddIronBall(int* a1);
BattleEventHandlerTableEntry* EventAddDestinyKnot(int* a1);
BattleEventHandlerTableEntry* EventAddIcyRock(int* a1);
BattleEventHandlerTableEntry* EventAddSmoothRock(int* a1);
BattleEventHandlerTableEntry* EventAddHeatRock(int* a1);
BattleEventHandlerTableEntry* EventAddDampRock(int* a1);
BattleEventHandlerTableEntry* EventAddStickyBarb(int* a1);
BattleEventHandlerTableEntry* EventAddPowerBracer(int* a1);
BattleEventHandlerTableEntry* EventAddPowerBelt(int* a1);
BattleEventHandlerTableEntry* EventAddPowerLens(int* a1);
BattleEventHandlerTableEntry* EventAddPowerBand(int* a1);
BattleEventHandlerTableEntry* EventAddPowerAnklet(int* a1);
BattleEventHandlerTableEntry* EventAddPowerWeight(int* a1);
BattleEventHandlerTableEntry* EventAddFlamePlate(int* a1);
BattleEventHandlerTableEntry* EventAddSplashPlate(int* a1);
BattleEventHandlerTableEntry* EventAddZapPlate(int* a1);
BattleEventHandlerTableEntry* EventAddMeadowPlate(int* a1);
BattleEventHandlerTableEntry* EventAddIciclePlate(int* a1);
BattleEventHandlerTableEntry* EventAddFistPlate(int* a1);
BattleEventHandlerTableEntry* EventAddToxicPlate(int* a1);
BattleEventHandlerTableEntry* EventAddEarthPlate(int* a1);
BattleEventHandlerTableEntry* EventAddSkyPlate(int* a1);
BattleEventHandlerTableEntry* EventAddMindPlate(int* a1);
BattleEventHandlerTableEntry* EventAddInsectPlate(int* a1);
BattleEventHandlerTableEntry* EventAddStonePlate(int* a1);
BattleEventHandlerTableEntry* EventAddSpookyPlate(int* a1);
BattleEventHandlerTableEntry* EventAddDracoPlate(int* a1);
BattleEventHandlerTableEntry* EventAddDreadPlate(int* a1);
BattleEventHandlerTableEntry* EventAddIronPlate(int* a1);
BattleEventHandlerTableEntry* EventAddBigRoot(int* a1);
BattleEventHandlerTableEntry* EventAddSmokeBall(int* a1);
BattleEventHandlerTableEntry* EventAddAmuletCoin(int* a1);
BattleEventHandlerTableEntry* EventAddAmuletCoin(int* a1);
BattleEventHandlerTableEntry* EventAddGriseousOrb(int* a1);
BattleEventHandlerTableEntry* EventAddFloatStone(int* a1);
BattleEventHandlerTableEntry* EventAddEviolite(int* a1);
BattleEventHandlerTableEntry* EventAddRockyHelmet(int* a1);
BattleEventHandlerTableEntry* EventAddAirBalloon(int* a1);
BattleEventHandlerTableEntry* EventAddRedCard(int* a1);
BattleEventHandlerTableEntry* EventAddRingTarget(int* a1);
BattleEventHandlerTableEntry* EventAddBindingBand(int* a1);
BattleEventHandlerTableEntry* EventAddAbsorbBulb(int* a1);
BattleEventHandlerTableEntry* EventAddCellBattery(int* a1);
BattleEventHandlerTableEntry* EventAddEjectButton(int* a1);
BattleEventHandlerTableEntry* EventAddFireGem(int* a1);
BattleEventHandlerTableEntry* EventAddWaterGem(int* a1);
BattleEventHandlerTableEntry* EventAddElectricGem(int* a1);
BattleEventHandlerTableEntry* EventAddGrassGem(int* a1);
BattleEventHandlerTableEntry* EventAddIceGem(int* a1);
BattleEventHandlerTableEntry* EventAddFightingGem(int* a1);
BattleEventHandlerTableEntry* EventAddPoisonGem(int* a1);
BattleEventHandlerTableEntry* EventAddGroundGem(int* a1);
BattleEventHandlerTableEntry* EventAddFlyingGem(int* a1);
BattleEventHandlerTableEntry* EventAddPsychicGem(int* a1);
BattleEventHandlerTableEntry* EventAddBugGem(int* a1);
BattleEventHandlerTableEntry* EventAddRockGem(int* a1);
BattleEventHandlerTableEntry* EventAddGhostGem(int* a1);
BattleEventHandlerTableEntry* EventAddDragonGem(int* a1);
BattleEventHandlerTableEntry* EventAddDarkGem(int* a1);
BattleEventHandlerTableEntry* EventAddSteelGem(int* a1);
BattleEventHandlerTableEntry* EventAddNormalGem(int* a1);

// MOVE_EVENT_ADD_TABLE definitions
BattleEventHandlerTableEntry* EventAddConversion(int* a1);
BattleEventHandlerTableEntry* EventAddCamouflage(int* a1);
BattleEventHandlerTableEntry* EventAddHaze(int* a1);
BattleEventHandlerTableEntry* EventAddDreamEater(int* a1);
BattleEventHandlerTableEntry* EventAddTriAttack(int* a1);
BattleEventHandlerTableEntry* EventAddSecretPower(int* a1);
BattleEventHandlerTableEntry* EventAddChatter(int* a1);
BattleEventHandlerTableEntry* EventAddSuperFang(int* a1);
BattleEventHandlerTableEntry* EventAddDragonRage(int* a1);
BattleEventHandlerTableEntry* EventAddSeismicToss(int* a1);
BattleEventHandlerTableEntry* EventAddSeismicToss(int* a1);
BattleEventHandlerTableEntry* EventAddPsywave(int* a1);
BattleEventHandlerTableEntry* EventAddSnore(int* a1);
BattleEventHandlerTableEntry* EventAddLastResort(int* a1);
BattleEventHandlerTableEntry* EventAddFlail(int* a1);
BattleEventHandlerTableEntry* EventAddFlail(int* a1);
BattleEventHandlerTableEntry* EventAddFalseSwipe(int* a1);
BattleEventHandlerTableEntry* EventAddSpiderWeb(int* a1);
BattleEventHandlerTableEntry* EventAddSpiderWeb(int* a1);
BattleEventHandlerTableEntry* EventAddSpiderWeb(int* a1);
BattleEventHandlerTableEntry* EventAddEndure(int* a1);
BattleEventHandlerTableEntry* EventAddSonicBoom(int* a1);
BattleEventHandlerTableEntry* EventAddFakeOut(int* a1);
BattleEventHandlerTableEntry* EventAddEndeavor(int* a1);
BattleEventHandlerTableEntry* EventAddEruption(int* a1);
BattleEventHandlerTableEntry* EventAddEruption(int* a1);
BattleEventHandlerTableEntry* EventAddRefresh(int* a1);
BattleEventHandlerTableEntry* EventAddMorningSun(int* a1);
BattleEventHandlerTableEntry* EventAddMorningSun(int* a1);
BattleEventHandlerTableEntry* EventAddMorningSun(int* a1);
BattleEventHandlerTableEntry* EventAddCrushGrip(int* a1);
BattleEventHandlerTableEntry* EventAddCrushGrip(int* a1);
BattleEventHandlerTableEntry* EventAddWeatherBall(int* a1);
BattleEventHandlerTableEntry* EventAddProtect(int* a1);
BattleEventHandlerTableEntry* EventAddProtect(int* a1);
BattleEventHandlerTableEntry* EventAddSplash(int* a1);
BattleEventHandlerTableEntry* EventAddCurse(int* a1);
BattleEventHandlerTableEntry* EventAddThrash(int* a1);
BattleEventHandlerTableEntry* EventAddUproar(int* a1);
BattleEventHandlerTableEntry* EventAddThrash(int* a1);
BattleEventHandlerTableEntry* EventAddThrash(int* a1);
BattleEventHandlerTableEntry* EventAddBrine(int* a1);
BattleEventHandlerTableEntry* EventAddWakeUpSlap(int* a1);
BattleEventHandlerTableEntry* EventAddSmellingSalts(int* a1);
BattleEventHandlerTableEntry* EventAddAcupressure(int* a1);
BattleEventHandlerTableEntry* EventAddTrumpCard(int* a1);
BattleEventHandlerTableEntry* EventAddFly(int* a1);
BattleEventHandlerTableEntry* EventAddBounce(int* a1);
BattleEventHandlerTableEntry* EventAddDive(int* a1);
BattleEventHandlerTableEntry* EventAddDig(int* a1);
BattleEventHandlerTableEntry* EventAddSolarBeam(int* a1);
BattleEventHandlerTableEntry* EventAddRazorWind(int* a1);
BattleEventHandlerTableEntry* EventAddSkyAttack(int* a1);
BattleEventHandlerTableEntry* EventAddSkullBash(int* a1);
BattleEventHandlerTableEntry* EventAddEncore(int* a1);
BattleEventHandlerTableEntry* EventAddTwister(int* a1);
BattleEventHandlerTableEntry* EventAddTwister(int* a1);
BattleEventHandlerTableEntry* EventAddEarthquake(int* a1);
BattleEventHandlerTableEntry* EventAddSurf(int* a1);
BattleEventHandlerTableEntry* EventAddSkyUppercut(int* a1);
BattleEventHandlerTableEntry* EventAddMagnitude(int* a1);
BattleEventHandlerTableEntry* EventAddFeint(int* a1);
BattleEventHandlerTableEntry* EventAddShadowForce(int* a1);
BattleEventHandlerTableEntry* EventAddStruggle(int* a1);
BattleEventHandlerTableEntry* EventAddPunishment(int* a1);
BattleEventHandlerTableEntry* EventAddTaunt(int* a1);
BattleEventHandlerTableEntry* EventAddCaptivate(int* a1);
BattleEventHandlerTableEntry* EventAddBellyDrum(int* a1);
BattleEventHandlerTableEntry* EventAddDestinyBond(int* a1);
BattleEventHandlerTableEntry* EventAddFacade(int* a1);
BattleEventHandlerTableEntry* EventAddPayback(int* a1);
BattleEventHandlerTableEntry* EventAddHiddenPower(int* a1);
BattleEventHandlerTableEntry* EventAddMinimize(int* a1);
BattleEventHandlerTableEntry* EventAddDefenseCurl(int* a1);
BattleEventHandlerTableEntry* EventAddStomp(int* a1);
BattleEventHandlerTableEntry* EventAddNightmare(int* a1);
BattleEventHandlerTableEntry* EventAddSuckerPunch(int* a1);
BattleEventHandlerTableEntry* EventAddRollout(int* a1);
BattleEventHandlerTableEntry* EventAddRollout(int* a1);
BattleEventHandlerTableEntry* EventAddAromatherapy(int* a1);
BattleEventHandlerTableEntry* EventAddHealBell(int* a1);
BattleEventHandlerTableEntry* EventAddMemento(int* a1);
BattleEventHandlerTableEntry* EventAddSpite(int* a1);
BattleEventHandlerTableEntry* EventAddRest(int* a1);
BattleEventHandlerTableEntry* EventAddLockOn(int* a1);
BattleEventHandlerTableEntry* EventAddLockOn(int* a1);
BattleEventHandlerTableEntry* EventAddReflect(int* a1);
BattleEventHandlerTableEntry* EventAddLightScreen(int* a1);
BattleEventHandlerTableEntry* EventAddSafeguard(int* a1);
BattleEventHandlerTableEntry* EventAddMist(int* a1);
BattleEventHandlerTableEntry* EventAddTailwind(int* a1);
BattleEventHandlerTableEntry* EventAddReturn(int* a1);
BattleEventHandlerTableEntry* EventAddFrustration(int* a1);
BattleEventHandlerTableEntry* EventAddPresent(int* a1);
BattleEventHandlerTableEntry* EventAddTorment(int* a1);
BattleEventHandlerTableEntry* EventAddImprison(int* a1);
BattleEventHandlerTableEntry* EventAddGravity(int* a1);
BattleEventHandlerTableEntry* EventAddGrudge(int* a1);
BattleEventHandlerTableEntry* EventAddHelpingHand(int* a1);
BattleEventHandlerTableEntry* EventAddGastroAcid(int* a1);
BattleEventHandlerTableEntry* EventAddRolePlay(int* a1);
BattleEventHandlerTableEntry* EventAddSpikes(int* a1);
BattleEventHandlerTableEntry* EventAddToxicSpikes(int* a1);
BattleEventHandlerTableEntry* EventAddStealthRock(int* a1);
BattleEventHandlerTableEntry* EventAddRoost(int* a1);
BattleEventHandlerTableEntry* EventAddMagnetRise(int* a1);
BattleEventHandlerTableEntry* EventAddFuryCutter(int* a1);
BattleEventHandlerTableEntry* EventAddPsychoShift(int* a1);
BattleEventHandlerTableEntry* EventAddAssurance(int* a1);
BattleEventHandlerTableEntry* EventAddConversion2(int* a1);
BattleEventHandlerTableEntry* EventAddCounter(int* a1);
BattleEventHandlerTableEntry* EventAddMirrorCoat(int* a1);
BattleEventHandlerTableEntry* EventAddMetalBurst(int* a1);
BattleEventHandlerTableEntry* EventAddRevenge(int* a1);
BattleEventHandlerTableEntry* EventAddRevenge(int* a1);
BattleEventHandlerTableEntry* EventAddTripleKick(int* a1);
BattleEventHandlerTableEntry* EventAddGyroBall(int* a1);
BattleEventHandlerTableEntry* EventAddPainSplit(int* a1);
BattleEventHandlerTableEntry* EventAddFollowMe(int* a1);
BattleEventHandlerTableEntry* EventAddWorrySeed(int* a1);
BattleEventHandlerTableEntry* EventAddThunderWave(int* a1);
BattleEventHandlerTableEntry* EventAddPsychUp(int* a1);
BattleEventHandlerTableEntry* EventAddHeartSwap(int* a1);
BattleEventHandlerTableEntry* EventAddPowerSwap(int* a1);
BattleEventHandlerTableEntry* EventAddGuardSwap(int* a1);
BattleEventHandlerTableEntry* EventAddAttract(int* a1);
BattleEventHandlerTableEntry* EventAddJudgement(int* a1);
BattleEventHandlerTableEntry* EventAddNaturalGift(int* a1);
BattleEventHandlerTableEntry* EventAddKnockOff(int* a1);
BattleEventHandlerTableEntry* EventAddDisable(int* a1);
BattleEventHandlerTableEntry* EventAddThief(int* a1);
BattleEventHandlerTableEntry* EventAddThief(int* a1);
BattleEventHandlerTableEntry* EventAddTrick(int* a1);
BattleEventHandlerTableEntry* EventAddTrick(int* a1);
BattleEventHandlerTableEntry* EventAddMimic(int* a1);
BattleEventHandlerTableEntry* EventAddSketch(int* a1);
BattleEventHandlerTableEntry* EventJumpKickAdd(int* a1);
BattleEventHandlerTableEntry* EventJumpKickAdd(int* a1);
BattleEventHandlerTableEntry* EventAddDefog(int* a1);
BattleEventHandlerTableEntry* EventAddBrickBreak(int* a1);
BattleEventHandlerTableEntry* EventAddTrickRoom(int* a1);
BattleEventHandlerTableEntry* EventAddWaterSport(int* a1);
BattleEventHandlerTableEntry* EventAddMudSport(int* a1);
BattleEventHandlerTableEntry* EventAddCharge(int* a1);
BattleEventHandlerTableEntry* EventAddPerishSong(int* a1);
BattleEventHandlerTableEntry* EventAddLeechSeed(int* a1);
BattleEventHandlerTableEntry* EventAddBeatUp(int* a1);
BattleEventHandlerTableEntry* EventAddAquaRing(int* a1);
BattleEventHandlerTableEntry* EventAddLunarDance(int* a1);
BattleEventHandlerTableEntry* EventAddHealingWish(int* a1);
BattleEventHandlerTableEntry* EventAddMetronome(int* a1);
BattleEventHandlerTableEntry* EventAddNaturePower(int* a1);
BattleEventHandlerTableEntry* EventAddAssist(int* a1);
BattleEventHandlerTableEntry* EventAddMirrorMove(int* a1);
BattleEventHandlerTableEntry* EventAddMeFirst(int* a1);
BattleEventHandlerTableEntry* EventAddCopycat(int* a1);
BattleEventHandlerTableEntry* EventAddSleepTalk(int* a1);
BattleEventHandlerTableEntry* EventAddLowKick(int* a1);
BattleEventHandlerTableEntry* EventAddLowKick(int* a1);
BattleEventHandlerTableEntry* EventAddFocusPunch(int* a1);
BattleEventHandlerTableEntry* EventAddStockpile(int* a1);
BattleEventHandlerTableEntry* EventAddSpitUp(int* a1);
BattleEventHandlerTableEntry* EventAddSwallow(int* a1);
BattleEventHandlerTableEntry* EventAddFutureSight(int* a1);
BattleEventHandlerTableEntry* EventAddDoomDesire(int* a1);
BattleEventHandlerTableEntry* EventAddRecycle(int* a1);
BattleEventHandlerTableEntry* EventAddPursuit(int* a1);
BattleEventHandlerTableEntry* EventAddPayDay(int* a1);
BattleEventHandlerTableEntry* EventAddBide(int* a1);
BattleEventHandlerTableEntry* EventAddSnatch(int* a1);
BattleEventHandlerTableEntry* EventAddMagicCoat(int* a1);
BattleEventHandlerTableEntry* EventAddTeleport(int* a1);
BattleEventHandlerTableEntry* EventAddUturn(int* a1);
BattleEventHandlerTableEntry* EventAddBatonPass(int* a1);
BattleEventHandlerTableEntry* EventAddPluck(int* a1);
BattleEventHandlerTableEntry* EventAddPluck(int* a1);
BattleEventHandlerTableEntry* EventAddFling(int* a1);
BattleEventHandlerTableEntry* EventAddBind(int* a1);
BattleEventHandlerTableEntry* EventAddBind(int* a1);
BattleEventHandlerTableEntry* EventAddBind(int* a1);
BattleEventHandlerTableEntry* EventAddBind(int* a1);
BattleEventHandlerTableEntry* EventAddBind(int* a1);
BattleEventHandlerTableEntry* EventAddBind(int* a1);
BattleEventHandlerTableEntry* EventAddWhirlpool(int* a1);
BattleEventHandlerTableEntry* EventAddRapidSpin(int* a1);
BattleEventHandlerTableEntry* EventAddRapidSpin(int* a1);
BattleEventHandlerTableEntry* EventAddPowerTrick(int* a1);
BattleEventHandlerTableEntry* EventAddTransform(int* a1);
BattleEventHandlerTableEntry* EventAddExplosion(int* a1);
BattleEventHandlerTableEntry* EventAddExplosion(int* a1);
BattleEventHandlerTableEntry* EventAddFocusEnergy(int* a1);
BattleEventHandlerTableEntry* EventAddRage(int* a1);
BattleEventHandlerTableEntry* EventAddAncientPower(int* a1);
BattleEventHandlerTableEntry* EventAddAncientPower(int* a1);
BattleEventHandlerTableEntry* EventAddAncientPower(int* a1);
BattleEventHandlerTableEntry* EventAddThunder(int* a1);
BattleEventHandlerTableEntry* EventAddBlizzard(int* a1);
BattleEventHandlerTableEntry* EventAddWish(int* a1);
BattleEventHandlerTableEntry* EventAddLuckyChant(int* a1);
BattleEventHandlerTableEntry* EventAddForesight(int* a1);
BattleEventHandlerTableEntry* EventAddForesight(int* a1);
BattleEventHandlerTableEntry* EventAddMiracleEye(int* a1);
BattleEventHandlerTableEntry* EventAddGrowth(int* a1);
BattleEventHandlerTableEntry* EventAddVenoshock(int* a1);
BattleEventHandlerTableEntry* EventAddFollowMe(int* a1);
BattleEventHandlerTableEntry* EventAddSoak(int* a1);
BattleEventHandlerTableEntry* EventAddSimpleBeam(int* a1);
BattleEventHandlerTableEntry* EventAddEntrainment(int* a1);
BattleEventHandlerTableEntry* EventAddClearSmog(int* a1);
BattleEventHandlerTableEntry* EventAddStoredPower(int* a1);
BattleEventHandlerTableEntry* EventAddShellSmash(int* a1);
BattleEventHandlerTableEntry* EventAddHex(int* a1);
BattleEventHandlerTableEntry* EventAddAcrobatics(int* a1);
BattleEventHandlerTableEntry* EventAddUturn(int* a1);
BattleEventHandlerTableEntry* EventAddWideGuard(int* a1);
BattleEventHandlerTableEntry* EventAddReflectType(int* a1);
BattleEventHandlerTableEntry* EventAddPowerSplit(int* a1);
BattleEventHandlerTableEntry* EventAddGuardSplit(int* a1);
BattleEventHandlerTableEntry* EventAddAutotomize(int* a1);
BattleEventHandlerTableEntry* EventAddHeavySlam(int* a1);
BattleEventHandlerTableEntry* EventAddHeavySlam(int* a1);
BattleEventHandlerTableEntry* EventAddWonderRoom(int* a1);
BattleEventHandlerTableEntry* EventAddMagicRoom(int* a1);
BattleEventHandlerTableEntry* EventAddPsyshock(int* a1);
BattleEventHandlerTableEntry* EventAddPsyshock(int* a1);
BattleEventHandlerTableEntry* EventAddFlameBurst(int* a1);
BattleEventHandlerTableEntry* EventAddElectroBall(int* a1);
BattleEventHandlerTableEntry* EventAddSynchronoise(int* a1);
BattleEventHandlerTableEntry* EventAddChipAway(int* a1);
BattleEventHandlerTableEntry* EventAddChipAway(int* a1);
BattleEventHandlerTableEntry* EventAddEchoedVoice(int* a1);
BattleEventHandlerTableEntry* EventAddIncinerate(int* a1);
BattleEventHandlerTableEntry* EventAddBestow(int* a1);
BattleEventHandlerTableEntry* EventAddCircleThrow(int* a1);
BattleEventHandlerTableEntry* EventAddCircleThrow(int* a1);
BattleEventHandlerTableEntry* EventAddRetaliate(int* a1);
BattleEventHandlerTableEntry* EventAddFoulPlay(int* a1);
BattleEventHandlerTableEntry* EventAddSmackDown(int* a1);
BattleEventHandlerTableEntry* EventAddFinalGambit(int* a1);
BattleEventHandlerTableEntry* EventAddAfterYou(int* a1);
BattleEventHandlerTableEntry* EventAddQuash(int* a1);
BattleEventHandlerTableEntry* EventAddRound(int* a1);
BattleEventHandlerTableEntry* EventAddQuickGuard(int* a1);
BattleEventHandlerTableEntry* EventAddAllySwitch(int* a1);
BattleEventHandlerTableEntry* EventAddTelekinesis(int* a1);
BattleEventHandlerTableEntry* EventAddSkyDrop(int* a1);
BattleEventHandlerTableEntry* EventAddStomp(int* a1);
BattleEventHandlerTableEntry* EventAddThunder(int* a1);
BattleEventHandlerTableEntry* EventAddPsyshock(int* a1);
BattleEventHandlerTableEntry* EventAddRelicSong(int* a1);
BattleEventHandlerTableEntry* EventAddTechnoBlast(int* a1);
BattleEventHandlerTableEntry* EventAddFreezeShock(int* a1);
BattleEventHandlerTableEntry* EventAddFreezeShock(int* a1);
BattleEventHandlerTableEntry* EventAddWaterPledge(int* a1);
BattleEventHandlerTableEntry* EventAddWaterPledge(int* a1);
BattleEventHandlerTableEntry* EventAddWaterPledge(int* a1);
BattleEventHandlerTableEntry* EventAddFusionFlare(int* a1);
BattleEventHandlerTableEntry* EventAddFusionFlare(int* a1);

// ItemEvent_AddItemCore definitions
int BattleMon_GetRealStat(BattleMon* a1, BattleMonValue a2);
BattleEventItem* BattleEvent_AddItem(BattleEventItemType EventType, __int16 SubID, BattleEventPriority MainPriority, int SubPriority, int DependID, const BattleEventHandlerTableEntry* HandlerTable, unsigned __int16 NumHandlers);

// ItemHandlers definitions
BattleMon* Handler_GetBattleMon(ServerFlow* a1, int a2);
bool BattleMon_IsStatChangeValid(BattleMon* battleMon, StatStage statChangeType, int statChangeAmount);
void ItemEvent_PushRun(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int defendingMonID);
HandlerParam_Header* BattleHandler_PushWork(ServerFlow* a1, BattleHandlerEffect a2, int a3);
void BattleHandler_PopWork(ServerFlow* a1, void* a2);
u32 PML_MoveGetCategory(int wazaId);
void BattleEventVar_MulValue(BattleEventVar a1, int a2);
void CommonTypeBoostingItem(BattleEventItem* a1, int a2, int a3, int a4);
int BattleEventVar_RewriteValue(BattleEventVar a1, int a2);
void HandlerCommonResistBerryDamageAfter(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonID, int* work);
int CommonResistBerry(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work, u8 pokeType, int a6);
bool Handler_GetFightEnableBenchPokeNum(ServerFlow* a1, unsigned int a2);
int Handler_CheckReservedMemberChangeAction(ServerFlow* a1);
unsigned int j_MainModule_PokeIDToClientID(unsigned int a1);
int BattleEventItem_GetSubID(BattleEventItem* a1);
int Handler_PokeIDToPokePos(int a1, unsigned int a2);
int CommonStatDropGuardCheck(int a1, int a2, int* a3, int a4);
void HandlerIronFist(BattleEventItem* battleEventItem, ServerFlow* serverFlow, int pokemonSlot, int* work);
bool IsAffectedBySheerForce(int a1);
bool GetSideFromMonID(unsigned int a1);
int PML_ItemIsBerry(u16 itemId);
int Handler_GetWeather(ServerFlow* a1);
int BattleMon_GetUsedItem(BattleMon* a1);
int CommonWeatherRecoveryAbility(ServerFlow* a1, int a2, int a3);
bool PML_MoveIsAlwaysHit(int moveId);
void Handler_SetMoveEffectIndex(ServerFlow* a1, char a2);

// IsUnselectableMove definitions
bool CanMonUseHeldItem(BtlClientWk* a1, BattleMon* a2);
ConditionData BattleMon_GetMoveCondition(BattleMon* a1, MoveCondition a2);
u16 Condition_GetParam(ConditionData a1);
int Move_IsUsable(BattleMon* a1, int a2);
void Btlv_StringParam_Setup(Btlv_StringParam* a1, char a2, __int16 a3);
void Btlv_StringParam_AddArg(Btlv_StringParam* a1, int a2);
bool PML_MoveIsDamaging(int wazaId);
int BattleMon_GetPreviousMove(BattleMon* a1);
int BattleMon_GetConditionAffectedMove(BattleMon* a1, MoveCondition a2);
u32 BattleField_CheckFieldEffectCore(BattleField* a1, BattleFieldEffect a2);
int BattleField_CheckImprisonCore(BattleField* a1, PokeCon* a2, BattleMon* a3, int a4);

// SetMoveCondition definitions
bool IsBasicStatus(MoveCondition a1);
int CureStatusCondition(BattleMon* a1);

// HandlerFollowMeBaitTarget definitions
BattleStyle Handler_GetBattleStyle(int a1);
bool MainModule_IsAllyMonID(unsigned int a1, unsigned int a2);
int Handler_IsMonSwitchOutInterrupted(ServerFlow* a1);
bool Handler_IsTargetInRange(ServerFlow* a1, int a2, int a3, int a4);
unsigned int BattleEventVar_SetRewriteOnceValue(BattleEventVar a1, int a2);
void HandlerFollowMeCheckFail(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work);
void HandlerFollowMeTextSet(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work);
void HandlerFollowMeTurnCheck(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work);

// MoveEvent_AddItem definitions
int MoveEvent_CanEffectBeRegistered(int a1, int a2, _BYTE* a3);

// HandlerIntimidate definitions
int Handler_GetExistFrontPokePos(ServerFlow* a1, int a2);
u8* Handler_GetTempWork(ServerFlow* a1);
int Handler_ExpandPokeID(ServerFlow* a1, __int16 a2, u8* a3);
void BattleHandler_PushRun(ServerFlow* a1, BattleHandlerEffect a2, int a3);
bool j_j_PosPoke_IsExist_3(PosPoke* a1, int a2);
void ServerDisplay_AbilityPopupAdd(ServerFlow* a1, BattleMon* a2);
bool j_j_PosPoke_IsExist_4(PosPoke* a1, int a2);
void ServerDisplay_AbilityPopupRemove(ServerFlow* a1, BattleMon* a2);
int ServerEvent_CheckSubstituteInteraction(ServerFlow* a1, BattleMon* a2, int a3, int a4, int a5, int a6);
void ServerDisplay_StatStageLimit(ServerFlow* a1, BattleMon* a2, int a3, int a4);
bool BattleMon_IsSubstituteActive(BattleMon* a1);
void ServerDisplay_AddMessageImpl(ServerCommandQueue* a1, int a2, __int16 a3, ...);
int ServerEvent_CheckStatStageChangeSuccess(ServerFlow* a1, BattleMon* a2, int a3, int a4, int a5, int a6);
void ServerDisplay_StatStage(ServerFlow* a1, BattleMon* a2, unsigned int a3, int a4, unsigned __int16 a5, int a6);
void ServerEvent_StatStageChangeApplied(ServerFlow* a1, int a2, BattleMon* a3, int a4, int a5);
void ServerEvent_StatStageChangeFail(ServerFlow* a1, BattleMon* a2, int a3);

// LoadSavedFreeSpaceItems definitions
PocketType BagSave_GetExistingItemPocket(BagSaveData* bag, u32 itemId);
void sub_21A067C(int a1, unsigned int a2, int a3, int a4);

// CommonContactStatusAbility definitions
bool AbilityEvent_RollEffectChance(ServerFlow* a1, unsigned int a2);

// HandlerPickpocket definitions
bool HandlerCommon_CheckIfCanStealPokeItem(ServerFlow* a1, unsigned int a2, int a3);
int HandlerCommon_CheckTargetMonID(int a1);
void BattleHandler_StrSetup(HandlerParam_StrParams* result, unsigned __int8 a2, __int16 a3);
void BattleHandler_AddArg(HandlerParam_StrParams* result, int a2);

// HandlerRockyHelmet definitions
int CommonGetItemParam(BattleEventItem* a1, ItemField a2);
unsigned int DivideMaxHPZeroCheck(BattleMon* a1, unsigned int a2);

// HandlerMummy definitions
bool Handler_CheckMatchup(int a1);

// Hazard Handlers definitions
bool GetSideFromMonID(unsigned int a1);
bool Handler_CheckFloating(ServerFlow* a1, int a2);
bool BattleMon_HasType(BattleMon* a1, PokeType a2);
ConditionData Condition_MakePermanent();
ConditionData Condition_MakeBadlyPoisoned();
int BattleMon_GetPokeType(BattleMon* a1);
int GetTypeEffectivenessVsMon(int a1, int a2);
bool Handler_DoesBattleMonExist(int a1, int a2);
bool Handler_CheckMatchup(int a1);
int Handler_PokeIDToPokePos(int a1, unsigned int a2);

// flowsub_CheckWazaAvoid definitions
int j_PokeSet_GetCountMax_4(PokeSet* a1);
BattleMon* j_PokeSet_Get_8(PokeSet* a1, unsigned int a2);
void j_PokeSet_SeekStart_20(PokeSet* a1);
BattleMon* j_PokeSet_SeekNext_41(PokeSet* a1);
int ServerEvent_SkipAvoidCheck(ServerFlow* a1, BattleMon* a2, BattleMon* a3, u16* a4);
bool ServerEvent_CheckHit(ServerFlow* a1, BattleMon* a2, BattleMon* a3, MoveParam* a4);
void j_PokeSet_Remove_10(PokeSet* a1, BattleMon* a2);
void ServerDisplay_MoveAvoid(ServerFlow* a1, BattleMon* a2);

// ServerEvent_CheckMultihitHits definitions
unsigned int RollMultiHitHits(unsigned int result);

// ServerControl_AfterSwitchIn definitions
void j_j_PokeSet_Clear(const void* a1);
void j_j_PokeSet_Add(PokeSet* result, BattleMon* a2);
void j_j_PokeSet_SortBySpeed_0(PokeSet* a1, ServerFlow* a2);
void ServerEvent_AfterSwitchInPrevious(ServerFlow* a1);
int BattleHandler_Result(ServerFlow* a1);
void j_j_PokeSet_SeekStart_21(PokeSet* a1);
BattleMon* j_j_PokeSet_SeekNext_43(PokeSet* a1);
void ServerEvent_SwitchIn(ServerFlow* a1, BattleMon* a2);
BattleMon* j_j_PokeSet_SeekNext_42(PokeSet* a1);
void ServerEvent_AfterLastSwitchIn(ServerFlow* a1);

// ServerControl_DamageAddCondition definitions
int ServerEvent_CheckMoveAddCondition(ServerFlow* serverFlow, int moveID, BattleMon* attackingMon, BattleMon* defendingMon, ConditionData* conditionData);

// BattleHandler_ChangeAbility definitions
bool j_j_Tables_IsAbilityMultitype(int a1);
void ServerEvent_ChangeAbilityBefore(ServerFlow* a1, int a2, int a3, int a4);
BattleEventItem* AbilityEvent_RemoveItem(BattleMon* a1);
void BattleMon_ChangeAbility(BattleMon* a1, __int16 a2);
BattleEventItem* AbilityEvent_AddItem(BattleMon* a1);
void ServerEvent_ChangeAbilityAfter(ServerFlow* a1, int a2);
void ServerControl_CheckItemReaction(ServerFlow* a1, BattleMon* a2, int a3);
int ServerControl_UnnerveAction(ServerFlow* a1, int a2);

// ServerDisplay_SkillSwap definitions
_DWORD* j_j_PokeSet_Get_3(int a1, int a2);
int j_CheckSkillSwapFailAbility(int a1);
int j_CheckSkillSwapFailAbility_0(int a1);
BattleEventItem* AbilityEvent_Swap(BattleMon* a1, BattleMon* a2);

// HandlerMoldBreakerSkipCheck definitions
int j_j_IsMoldBreakerAffectedAbility(int a1);

// FlowerGift handlers definitions
bool CheckFlowerGiftEnablePokemon(ServerFlow* a1, int a2);
void CommonFlowerGiftFormChange(BattleEventItem* a1, ServerFlow* a2, int a3, int a4, unsigned __int8 a5);
void HandlerFlowerGiftWeather(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work);
void HandlerFlowerGiftSwitchIn(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work);
void HandlerFlowerGiftSpecialDefense(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work);
void HandlerFlowerGiftPower(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work);
void HandlerFlowerGiftGotAbility(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work);
void HandlerFlowerGiftAirLock(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work);
void HandlerFlowerGiftAbilityOff(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work);
void HandlerFlowerGiftAbilityChange(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work);


// Forecast handlers definitions
int BattleMon_GetSpecies(BattleMon* a1);
void HandlerForecastWeather(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work);
void HandlerForecastSwitchIn(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work);
void HandlerForecastGetAbility(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work);
void HandlerForecastChangeAbility(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work);
void HandlerForecastAirLock(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work);
void HandlerForecastAbilityOff(BattleEventItem* a1, ServerFlow* serverFlow, int pokemonSlot, int* work);

// ServerEvent_CalcDamage definitions
int ServerEvent_GetMovePower(ServerFlow* a1, BattleMon* a2, BattleMon* a3, MoveParam* a4);
int ServerEvent_GetAttackPower(ServerFlow* a1, BattleMon* AttackingMon, BattleMon* DefendingMon, MoveParam* a4, int criticalFlag);
int ServerEvent_GetTargetDefenses(ServerFlow* a1, BattleMon* a2, BattleMon* a3, MoveParam* a4, int a5);
unsigned int CalcBaseDamage(int power, int attack, int level, unsigned int defense);
int ServerEvent_GetWeather(ServerFlow* a1);
int WeatherPowerMod(int a1, int a2);
int MainModule_GetDebugFlag();
bool sub_21AE34C(ServerFlow* a1);
int ServerEvent_SameTypeAttackBonus(ServerFlow* a1, BattleMon* a2, PokeType a3);
unsigned int TypeEffectivenessPowerMod(unsigned int result, unsigned int a2);

// AddConditionCheckFailOverwrite definitions
bool PokeTypePair_HasType(int pokemonTypePair, PokeType type);

// Hazards definitions
void BattleEventItem_SetWorkValue(BattleEventItem* a1, int a2, ConditionData a3);
void ServerEvent_CheckSideEffectParam(ServerFlow* a1, int a2, int a3, int a4, ConditionData* a5);
ConditionData Condition_MakeTurn(int a1);
void BattleEventItem_Remove(BattleEventItem* a1);
void ServerControl_SideEffectEndMessage(int a1, unsigned int a2, ServerFlow* a3);

// FieldMenu definitions
FieldSubscreen* Field_GetSubscreen(Field* a1);
b32 FieldSubscreen_IsReady(FieldSubscreen* subscreen);
void sub_20173F8(GameData* gameData, char a2);
void FieldSubscreen_ReqChange(FieldSubscreen* subscreen, FieldSubscreenID screenId);
FieldSubscreenID FieldSubscreen_GetReturnSubscreen(FieldSubscreen* subscreen);
void sub_2198524(FieldSubscreen* a1);
int sub_2198894(FieldSubscreen* subscr);
int sub_215AAA8(int a1);
GameEvent* EventFieldMenuReturn_Create(GameSystem* a1, Field* a2, int a3);
int sub_215B7E8(_DWORD* a1);
void EventFieldAppCall_ConvAppResultToEventType(FieldSubAppResult result, ItemUseEventType* dest);
b32 FieldAppCallParam_CanRetry(FieldAppCallParam* param);
GameEvent* CallFieldMapEntranceOutTransitionDefault(GameSystem* a1, Field* a2, unsigned int a3, int a4);
GameEvent* CreateFieldCloseEvent(GameSystem* gsys, Field* field);
int sub_215B800(FieldAppCallParam* a1);
GameEvent* EventFieldOpen_CreateHeadless(GameSystem* gsys);
void FieldG2D_SetLCDConfig();
void FieldG2D_Prepare3DSurface(Field* field);
GameEvent* CallFieldMapEntranceInTransition(GameSystem* gsys, Field* field, unsigned int a3, int a4, int a5, Season lastSeason, Season newSeason);
void sub_215B794(EventFieldAppCall* event);
int sub_2198B44(int a1);
TCBManagerEx* GFL_TCBExMgrCreate(HeapID heapId, HeapID workHeapId, int limit, int extraSize);
int sub_202E7D0(int a1, int a2, int a3, int a4);
int sub_20219C4(HeapID a1);
void sub_2163F20(int a1);
int sub_2164190(int a1);
int sub_2163F78(int a1, int a2);
int sub_216408C(int a1);
int sub_2164220(int a1);
int sub_216425C(int a1);
int sub_200746C(SaveControl* a1);
void sub_2024F18(BmpWin* a1, int a2);
void* GameData_GetRecords(GameData* gameData);
FieldSubscreen* Field_GetSubscreen(Field* a1);
int sub_2198B50(int a1);
int RecordAddOne(void* a1, int a2);
int sub_2198B80(int a1);
int sub_21640D0(int a1, int a2);
int sub_2164150(void);
int sub_2164298(int a1);
int sub_21642F4();
int sub_216431C(int a1);
TCBManager* GFL_VBlankGetTCBMgr();
int sub_2035630(TCBManager* a1, int a2, int a3, int a4, int a5);
int sub_2198B5C(int a1);
int sub_216433C(int a1);
SaveData* sub_201782C(GameData* a1);
int sub_2017850(int a1);
void sub_2198B74();
int sub_2164358(int a1);
int sub_21642D0(int a1);
int sub_2035838(int a1);
int sub_2198B68(int a1);
WordSetSystem* GFL_WordSetSystemCreateDefault(HeapID heapId);
StrBuf* GFL_MsgDataLoadStrbufNew(MsgData* msgData, int msgId);
u16* GetGameDataPlayerInfo(GameData* gameData);
void GFL_WordSetSystemFree(WordSetSystem* wordSet);
int sub_2163F90(int a1);
b32 GFL_SndIsPlaying(SoundResID sndId);
int sub_2163EE0(void);
void sub_2198530(int a1, HeapID heapId);

extern u32 g_GameBeaconSys;
extern SystemUI* g_SystemUI;
extern MsgData* g_PMLSpeciesNamesResident;
extern BGSysLCDConfig cfg;
#define GAME_DATA *(GameData**)(g_GameBeaconSys + 4)
extern short g_GFLStrTerminator;

bool IsEqual(int a1, int a2)
{
    char* c1 = (char*)&a1;
    char* c2 = (char*)&a2;

    for (u16 i = 0; i < 4; ++i)
    {
        if (c1[i] != c2[i])
            return false;
    }
    return true;
}

void GetWideString(short* buffer, u16 size, const char* string)
{
    for (u16 i = 0; i < size - 1; ++i)
        buffer[i] = (short)string[i];
    buffer[size - 1] = g_GFLStrTerminator;
}

C_DECL_END

// I dont care, im not organizing this
#endif