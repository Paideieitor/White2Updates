#ifndef __BATTLE_H
#define __BATTLE_H

#include "swantypes.h"

#include "battle/btl_result.h"
#include "field/fieldmap.h"
#include "field/field_mmodel.h"
#include "gfl/core/gfl_heap.h"
#include "math/angle.h"
#include "math/vector.h"
#include "pml/poke_party.h"
#include "save/save_control.h"

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
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
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
    char ID;
    char field_1A;
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
    MoveSet moveSet[4];
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
    MoveDamageRec moveDamageRec[3][6];
    unsigned __int8 DamageRecCount[3];
    char DamageRecTurn;
    char DamageRec;
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
    BattleMon* activePokeParam[24];
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

struct SWAN_PACKED SWAN_ALIGNED(4) BtlClientWk
{
    MainModule* mainModule;
    PokeCon* pokeCon;
    BattleMon* procPoke;
    BattleActionParam* procAction;
    BtlRecWk* recData;
    int battleRecReader;
    int recPlayer;
    _BYTE gap1C[6];
    __int16 field_22;
    _BYTE gap24[4];
    _DWORD dword28;
    EscapeInfo escapeInfo;
    BattleField* fieldSim;
    _BYTE gap38[24];
    Adapter* adapter;
    BtlvCore* btlvCore;
    Btlv_StringParam strParam;
    int field_7C;
    Btlv_StringParam field_80;
    _BYTE gapA4[4];
    _DWORD cmdCheckServer;
    BattleMon* field_AC[6];
    int field_C4;
    int field_C8;
    _BYTE byteCC;
    char field_CD;
    SWAN_ALIGNED(4) int field_D0;
    int field_D4;
    u32* subProcFunction; // int(*subProcFunction)(BtlClientWk * a1, unsigned int* a2);
    int field_DC;
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
    _BYTE gap11A[6];
    BattleParty* actPokeParty;
    unsigned __int8 myCoverPosNum;
    unsigned __int8 procPokeIdx;
    char prevPokeIdx;
    char firstPokeIdx;
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
    __int16 field_1A2;
    char field_1A4;
    SWAN_ALIGNED(2) char field_1A6;
    char field_1A7;
    _WORD field_1A8[3];
    char myID;
    char field_1AF;
    _BYTE gap1B0;
    char commWaitInfoOn;
    char field_1B2;
    char field_1B3;
    _BYTE field_1B4;
    _BYTE forceQuitActionSelect;
    _BYTE cmdCheckTimingCode;
    char currentActionCount;
    char moveInfoPokeIdx;
    char moveInfoMoveIdx;
    char actSelectFlags;
    char myChangePokeCount;
    char field_1BC;
    _BYTE field_1BD;
    unsigned __int8 myChangePokePos[6];
    char field_1C4;
    char field_1C5;
    char field_1C6;
    char field_1C7;
    char field_1C8;
    char field_1C9;
    char field_1CA;
    _BYTE gap1CB[5];
    int field_1D0;
    _BYTE gap1D4[20];
    __int16 field_1E8;
    __int16 field_1EA;
    _BYTE gap1EC[4];
    int field_1F0;
    _BYTE gap1F4[16];
    int field_204;
    _BYTE gap208[12];
    int field_214;
    _BYTE gap218[4];
    _WORD word21C;
    SWAN_ALIGNED(4) int dword220;
    int field_224;
    int field_228;
    int field_22C;
    int field_230;
    int field_234;
    int field_238;
    int field_23C;
    int field_240;
    int field_244;
    int field_248;
    int field_24C;
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
    int evolutionDataPtr;
    MoveAnimCtrl* moveAnimCtrl;
    MoveStealParam* moveStealParam;
    MoveStealParam* magicCoatParam;
    HITCHECK_PARAM* hitCheckParam;
    EscapeInfo escapeInfo;
    int field_4C0;
    char field_4C4;
    _BYTE gap4C5[9];
    SWAN_PACKED SWAN_ALIGNED(1) int field_4CE;
    SWAN_ALIGNED(4) u8 field_4D4[672];
    int SimulationCounter;
    int moveSerial;
    char cmdBuildStep;
    char field_77D;
    char field_77E;
    char defaultTargetPos;
    __int16 heapID;
    char numActOrder;
    char numEndActOrder;
    _BYTE gap784;
    char revivedPokeCount;
    char field_786;
    char field_787;
    char field_788;
    char thruDeadMsgPokeID;
    char field_78A;
    char gap78B;
    char field_78C;
    SWAN_ALIGNED(4) char field_790;
    u8 revivePokeID[24];
    char pokeInFlag[24];
    char field_7C1[24];
    char switchCount[4];
    char field_7DD;
    PokeSet* monsInBattle;
    int field_7E4;
    _BYTE gap7E8[104];
    PokeSet* setTargetOriginal;
    PokeSet* setTarget;
    PokeSet* setAlly;
    PokeSet* setEnemy;
    PokeSet* setDamaged;
    PokeSet* setStealTarget;
    PokeSet* setTemp;
    CALC_DAMAGE_REC* calcDamageAlly;
    CALC_DAMAGE_REC* calcDamageEnemy;
    POKESET_STACK_UNIT pokesetUnit[7];
    int pokesetStackPtr;
    char field_1A68;
    _BYTE gap1A69[71];
    MoveParam* moveParam;
    MoveParam* moveParamOriginal;
    PosPoke posPoke;
    char field_1AE2;
    _BYTE gap1AE3[37];
    char gap1B08;
    char gap1B09;
    char gap1B0A;
    CalcExpWork levelUpInfo[6];
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
    SWAN_ALIGNED(4) char field_1D88;
    char field_1D89;
    char field_1D8A;
    _BYTE gap1D8B[481];
    char field_1F6C;
    _BYTE gap1F6D[11];
    __int16 PrevUsedMove;
    char gap1F7A;
    SWAN_ALIGNED(8) EffectivenessCounter effectivenessCounter;
    int dmgAffRec;
    _BYTE gap1F90[8];
    char field_1F98;
    _BYTE gap1F99[83];
    char field_1FEC;
    char field_1FED;
    SWAN_ALIGNED(4) u8 TempWork;
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
    IT0639_WEAKNESS_POLICY = 0x27F,
    IT0640_ASSAULT_VEST = 0x280,
    IT0641_HOLO_CASTER = 0x281,
    IT0642_PROF_S_LETTER = 0x282,
    IT0643_ROLLER_SKATES = 0x283,
    IT0644_PIXIE_PLATE = 0x284,
    IT0645_ABILITY_CAPSULE = 0x285,
    IT0646_WHIPPED_DREAM = 0x286,
    IT0647_SACHET = 0x287,
    IT0648_LUMINOUS_MOSS = 0x288,
    IT0649_SNOWBALL = 0x289,
    IT0650_SAFETY_GOGGLES = 0x28A,
    IT0651_POKE_FLUTE = 0x28B,
    IT0652_RICH_MULCH = 0x28C,
    IT0653_SURPRISE_MULCH = 0x28D,
    IT0654_BOOST_MULCH = 0x28E,
    IT0655_AMAZE_MULCH = 0x28F,
    IT0656_GENGARITE = 0x290,
    IT0657_GARDEVOIRITE = 0x291,
    IT0658_AMPHAROSITE = 0x292,
    IT0659_VENUSAURITE = 0x293,
    IT0660_CHARIZARDITE_X = 0x294,
    IT0661_BLASTOISINITE = 0x295,
    IT0662_MEWTWONITE_X = 0x296,
    IT0663_MEWTWONITE_Y = 0x297,
    IT0664_BLAZIKENITE = 0x298,
    IT0665_MEDICHAMITE = 0x299,
    IT0666_HOUNDOOMINITE = 0x29A,
    IT0667_AGGRONITE = 0x29B,
    IT0668_BANETTITE = 0x29C,
    IT0669_TYRANITARITE = 0x29D,
    IT0670_SCIZORITE = 0x29E,
    IT0671_PINSIRITE = 0x29F,
    IT0672_AERODACTYLITE = 0x2A0,
    IT0673_LUCARIONITE = 0x2A1,
    IT0674_ABOMASITE = 0x2A2,
    IT0675_KANGASKHANITE = 0x2A3,
    IT0676_GYARADOSITE = 0x2A4,
    IT0677_ABSOLITE = 0x2A5,
    IT0678_CHARIZARDITE_Y = 0x2A6,
    IT0679_ALAKAZITE = 0x2A7,
    IT0680_HERACRONITE = 0x2A8,
    IT0681_MAWILITE = 0x2A9,
    IT0682_MANECTITE = 0x2AA,
    IT0683_GARCHOMPITE = 0x2AB,
    IT0684_LATIASITE = 0x2AC,
    IT0685_LATIOSITE = 0x2AD,
    IT0686_ROSELI_BERRY = 0x2AE,
    IT0687_KEE_BERRY = 0x2AF,
    IT0688_MARANGA_BERRY = 0x2B0,
    IT0689_SPRINKLOTAD = 0x2B1,
    IT0690_TM96 = 0x2B2,
    IT0691_TM97 = 0x2B3,
    IT0692_TM98 = 0x2B4,
    IT0693_TM99 = 0x2B5,
    IT0694_TM100 = 0x2B6,
    IT0695_POWER_PLANT_PASS = 0x2B7,
    IT0696_MEGA_RING = 0x2B8,
    IT0697_INTRIGUING_STONE = 0x2B9,
    IT0698_COMMON_STONE = 0x2BA,
    IT0699_DISCOUNT_COUPON = 0x2BB,
    IT0700_ELEVATOR_KEY = 0x2BC,
    IT0701_TMV_PASS = 0x2BD,
    IT0702_HONOR_OF_KALOS = 0x2BE,
    IT0703_ADVENTURE_RULES = 0x2BF,
    IT0704_STRANGE_SOUVENIR = 0x2C0,
    IT0705_LENS_CASE = 0x2C1,
    IT0706_TRAVEL_TRUNK = 0x2C2,
    IT0707_TRAVEL_TRUNK = 0x2C3,
    IT0708_LUMIOSE_GALETTE = 0x2C4,
    IT0709_SHALOUR_SABLE = 0x2C5,
    IT0710_JAW_FOSSIL = 0x2C6,
    IT0711_SAIL_FOSSIL = 0x2C7,
    IT0712_LOOKER_TICKET = 0x2C8,
    IT0713_BIKE = 0x2C9,
    IT0714_HOLO_CASTER = 0x2CA,
    IT0715_FAIRY_GEM = 0x2CB,
    IT0716_MEGA_CHARM = 0x2CC,
    IT0717_MEGA_GLOVE = 0x2CD,
    IT0718_MACH_BIKE = 0x2CE,
    IT0719_ACRO_BIKE = 0x2CF,
    IT0720_WAILMER_PAIL = 0x2D0,
    IT0721_DEVON_PARTS = 0x2D1,
    IT0722_SOOT_SACK = 0x2D2,
    IT0723_BASEMENT_KEY = 0x2D3,
    IT0724_POKEBLOCK_KIT = 0x2D4,
    IT0725_LETTER = 0x2D5,
    IT0726_EON_TICKET = 0x2D6,
    IT0727_SCANNER = 0x2D7,
    IT0728_GO_GOGGLES = 0x2D8,
    IT0729_METEORITE = 0x2D9,
    IT0730_KEY_TO_ROOM_1 = 0x2DA,
    IT0731_KEY_TO_ROOM_2 = 0x2DB,
    IT0732_KEY_TO_ROOM_4 = 0x2DC,
    IT0733_KEY_TO_ROOM_6 = 0x2DD,
    IT0734_STORAGE_KEY = 0x2DE,
    IT0735_DEVON_SCOPE = 0x2DF,
    IT0736_S_S_TICKET = 0x2E0,
    IT0737_HM07 = 0x2E1,
    IT0738_DEVON_SCUBA_GEAR = 0x2E2,
    IT0739_CONTEST_COSTUME = 0x2E3,
    IT0740_CONTEST_COSTUME = 0x2E4,
    IT0741_MAGMA_SUIT = 0x2E5,
    IT0742_AQUA_SUIT = 0x2E6,
    IT0743_PAIR_OF_TICKETS = 0x2E7,
    IT0744_MEGA_BRACELET = 0x2E8,
    IT0745_MEGA_PENDANT = 0x2E9,
    IT0746_MEGA_GLASSES = 0x2EA,
    IT0747_MEGA_ANCHOR = 0x2EB,
    IT0748_MEGA_STICKPIN = 0x2EC,
    IT0749_MEGA_TIARA = 0x2ED,
    IT0750_MEGA_ANKLET = 0x2EE,
    IT0751_METEORITE = 0x2EF,
    IT0752_SWAMPERTITE = 0x2F0,
    IT0753_SCEPTILITE = 0x2F1,
    IT0754_SABLENITE = 0x2F2,
    IT0755_ALTARIANITE = 0x2F3,
    IT0756_GALLADITE = 0x2F4,
    IT0757_AUDINITE = 0x2F5,
    IT0758_METAGROSSITE = 0x2F6,
    IT0759_SHARPEDONITE = 0x2F7,
    IT0760_SLOWBRONITE = 0x2F8,
    IT0761_STEELIXITE = 0x2F9,
    IT0762_PIDGEOTITE = 0x2FA,
    IT0763_GLALITITE = 0x2FB,
    IT0764_DIANCITE = 0x2FC,
    IT0765_PRISON_BOTTLE = 0x2FD,
    IT0766_MEGA_CUFF = 0x2FE,
    IT0767_CAMERUPTITE = 0x2FF,
    IT0768_LOPUNNITE = 0x300,
    IT0769_SALAMENCITE = 0x301,
    IT0770_BEEDRILLITE = 0x302,
    IT0771_METEORITE = 0x303,
    IT0772_METEORITE = 0x304,
    IT0773_KEY_STONE = 0x305,
    IT0774_METEORITE_SHARD = 0x306,
    IT0775_EON_FLUTE = 0x307,
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
    BattleEvent_Null = 0,
    BattleEvent_ActionProcessingStart = 1,
    BattleEvent_ActionProcessingEnd = 2,
    BattleEvent_MoveSequenceStart = 3,
    BattleEvent_MoveSequenceEnd = 4,
    BattleEvent_BypassSubstitute = 5,
    BattleEvent_CheckDelayedMove = 6,
    BattleEvent_DecideDelayedMove = 7,
    BattleEvent_MoveSequenceSteal = 8,
    BattleEvent_MoveSequenceReflect = 9,
    BattleEvent_A = 0xA,
    BattleEvent_SkipRunCalc = 0xB,
    BattleEvent_PreventRun = 0xC,
    BattleEvent_RunExitMessage = 0xD,
    BattleEvent_CheckSleep = 0xE,
    BattleEvent_CheckSpecialPriority = 0xF,
    BattleEvent_GetMovePriority = 0x11,
    BattleEvent_CheckFloating = 0x12,
    BattleEvent_CalcSpeed = 0x13,
    BattleEvent_14 = 0x14,
    BattleEvent_BeforeAttacks = 0x15,
    BattleEvent_MoveRequestForActionOrder = 0x16,
    BattleEvent_MoveRequestCheckFail = 0x17,
    BattleEvent_MoveRequestParam = 0x18,
    BattleEvent_MoveRequestMessage = 0x19,
    BattleEvent_CheckMoveSteal = 0x1A,
    BattleEvent_FloatingImmuneToMove = 0x1B,
    BattleEvent_SkipAvoidCheck = 0x1C,
    BattleEvent_CheckMoveFail = 0x1D,
    BattleEvent_MoveExecuteCheck1 = 0x1E,
    BattleEvent_MoveExecuteCheck2 = 0x1F,
    BattleEvent_MoveExecuteCheck3 = 0x20,
    BattleEvent_MoveExecuteFail = 0x21,
    BattleEvent_ChooseMove = 0x22,
    BattleEvent_BreakOpponentGuard = 0x23,
    BattleEvent_MoveExecuteStart = 0x24,
    BattleEvent_MoveExecuteEffective = 0x25,
    BattleEvent_MoveExecuteNoEffect = 0x26,
    BattleEvent_MoveExecuteEnd = 0x27,
    BattleEvent_MoveParam = 0x28,
    BattleEvent_DecideTarget = 0x29,
    BattleEvent_RedirectTarget = 0x2A,
    BattleEvent_RedirectTargetEnd = 0x2B,
    BattleEvent_NoEffectCheck = 0x2C,
    BattleEvent_AbilityCheckNoEffect = 0x2D,
    BattleEvent_CheckProtectBreak = 0x2E,
    BattleEvent_AvoidMove = 0x2F,
    BattleEvent_CheckDamageToRecover = 0x30,
    BattleEvent_ApplyDamageToRecover = 0x31,
    BattleEvent_BypassAccuracyCheck = 0x32,
    BattleEvent_MoveAccuracyStage = 0x33,
    BattleEvent_MoveAccuracy = 0x34,
    BattleEvent_MoveHitCount = 0x35,
    BattleEvent_CriticalCheck = 0x36,
    BattleEvent_MoveBasePower = 0x37,
    BattleEvent_MovePower = 0x38,
    BattleEvent_BeforeAttackerPower = 0x39,
    BattleEvent_BeforeDefenderGuard = 0x3A,
    BattleEvent_AttackerPower = 0x3B,
    BattleEvent_DefenderGuard = 0x3C,
    BattleEvent_CheckTypeEffectivenessEnable = 0x3D,
    BattleEvent_CheckTypeEffectiveness = 0x3E,
    BattleEvent_RewriteTypeEffectiveness = 0x3F,
    BattleEvent_TypeMatchCheck = 0x40,
    BattleEvent_SameTypeAttackBonus = 0x41,
    BattleEvent_AttackerType = 0x42,
    BattleEvent_DefenderType = 0x43,
    BattleEvent_AfterDamageReaction = 0x44,
    BattleEvent_DetermineMoveDamage = 0x45,
    BattleEvent_MoveDamageProcessing1 = 0x46,
    BattleEvent_MoveDamageProcessing2 = 0x47,
    BattleEvent_MoveDamageProcessingEnd = 0x48,
    BattleEvent_MoveDamageProcessingFinal = 0x49,
    BattleEvent_PreviousMoveReaction = 0x4A,
    BattleEvent_MoveDamageReaction1 = 0x4B,
    BattleEvent_MoveDamageReaction2 = 0x4C,
    BattleEvent_MoveDamageSideAfter = 0x4D,
    BattleEvent_DecrementPP = 0x4E,
    BattleEvent_DecrementPPDone = 0x4F,
    BattleEvent_CalcRecoil = 0x50,
    BattleEvent_AddStatStageChangeTarget = 0x51,
    BattleEvent_AddStatStageChangeUser = 0x52,
    BattleEvent_SwitchOutInterrupt = 0x53,
    BattleEvent_SwitchOutEnd = 0x54,
    BattleEvent_SwitchIn = 0x55,
    BattleEvent_SwitchInPrevious = 0x56,
    BattleEvent_AfterLastSwitchIn = 0x57,
    BattleEvent_CheckActivation = 0x58,
    BattleEvent_GetStatStageChangeValue = 0x59,
    BattleEvent_StatStageChange = 0x5A,
    BattleEvent_StatStageChangeLastCheck = 0x5B,
    BattleEvent_StatStageChangeFail = 0x5C,
    BattleEvent_StatStageChangeApplied = 0x5D,
    BattleEvent_MoveStatStageChangeApplied = 0x5E,
    BattleEvent_MoveConditionTurnCount = 0x5F,
    BattleEvent_MoveConditionSpecial = 0x60,
    BattleEvent_MoveConditionMessage = 0x61,
    BattleEvent_MoveConditionParam = 0x62,
    BattleEvent_AddConditionType = 0x63,
    BattleEvent_AddCondition = 0x64,
    BattleEvent_AddConditionCheckFail = 0x65,
    BattleEvent_AddConditionApplied = 0x66,
    BattleEvent_AddConditionFail = 0x67,
    BattleEvent_AddBasicStatus = 0x68,
    BattleEvent_MoveConditionApplied = 0x69,
    BattleEvent_AbilityNullified = 0x6A,
    BattleEvent_ConditionDamage = 0x6B,
    BattleEvent_MoveFlinchChance = 0x6C,
    BattleEvent_FlinchCheck = 0x6D,
    BattleEvent_FlinchFail = 0x6E,
    BattleEvent_FlinchApplied = 0x6F,
    BattleEvent_OHKOCheck = 0x70,
    BattleEvent_OHKOPrevent = 0x71,
    BattleEvent_UseItem = 0x72,
    BattleEvent_UseItemTemp = 0x73,
    BattleEvent_EndureCheck = 0x74,
    BattleEvent_Endure = 0x75,
    BattleEvent_TurnCheckBegin = 0x76,
    BattleEvent_TurnCheckEnd = 0x77,
    BattleEvent_TurnCheckDone = 0x78,
    BattleEvent_NotifyAirLock = 0x79,
    BattleEvent_WeatherCheck = 0x7A,
    BattleEvent_WeightRatio = 0x7B,
    BattleEvent_MoveWeatherTurnCount = 0x7C,
    BattleEvent_WeatherChange = 0x7D,
    BattleEvent_AfterWeatherChange = 0x7E,
    BattleEvent_WeatherReaction = 0x7F,
    BattleEvent_EnableSimpleDamage = 0x80,
    BattleEvent_DamageProcessingStart = 0x81,
    BattleEvent_DamageProcessingEndPreviousHit = 0x82,
    BattleEvent_DamageProcessingEndHitReal = 0x83,
    BattleEvent_DamageProcessingEndHit1 = 0x84,
    BattleEvent_DamageProcessingEndHit2 = 0x85,
    BattleEvent_DamageProcessingEndHit3 = 0x86,
    BattleEvent_DamageProcessingEndHit4 = 0x87,
    BattleEvent_DamageProcessingEnd = 0x88,
    BattleEvent_BeforeAbilityChange = 0x89,
    BattleEvent_AfterAbilityChange = 0x8A,
    BattleEvent_CheckForceSwitch = 0x8B,
    BattleEvent_CalcDrain = 0x8C,
    BattleEvent_CalcDrainEnd = 0x8D,
    BattleEvent_RecoverHP = 0x8F,
    BattleEvent_AfterItemEquip = 0x90,
    BattleEvent_CheckItemReaction = 0x91,
    BattleEvent_ItemConsumed = 0x92,
    BattleEvent_CheckChargeUpFail = 0x93,
    BattleEvent_CheckChargeUpSkip = 0x94,
    BattleEvent_ChargeUpStart = 0x95,
    BattleEvent_ChargeUpStartDone = 0x96,
    BattleEvent_ChargeUpSkip = 0x97,
    BattleEvent_ChargeUpEnd = 0x98,
    BattleEvent_CheckSemiInvuln = 0x99,
    BattleEvent_HeldItemCheck = 0x9A,
    BattleEvent_HeldItemFail = 0x9B,
    BattleEvent_HeldItemDecide = 0x9C,
    BattleEvent_ItemRewriteDone = 0x9D,
    BattleEvent_CallFieldEffect = 0x9E,
    BattleEvent_CheckSideEffectParam = 0x9F,
    BattleEvent_UncategorizedMove = 0xA0,
    BattleEvent_UncategorizedMoveNoTarget = 0xA1,
    BattleEvent_CombinedMoveCheck = 0xA2,
    BattleEvent_NotifyFainted = 0xA3,
    BattleEvent_AfterMove = 0xA4
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
    VAR_MON_ID = 0x2,
    VAR_ATTACKING_MON = 0x3,
    VAR_DEFENDING_MON = 0x4,
    VAR_POKE_COUNT = 0x5,
    VAR_TARGET_MON_ID = 0x6,
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
    VAR_STAT_BOOST_AMOUNT = 0x20,
    VAR_FLOATING = 0x21,
    VAR_FAIL_CAUSE = 0x22,
    VAR_EFFECT_TURN_COUNT = 0x24,
    VAR_DEFAULT_EFFECT_CHANCE = 0x25,
    VAR_EFFECT_CHANCE = 0x26,
    VAR_EVASION_STAGE = 0x27,
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
    VAR_RATIO = 0x35,
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
    VAR_SEMI_INVULNERABLE = 0x42,
    VAR_CRITICAL_FLAG = 0x45,
    VAR_SUBSTITUTE = 0x46,
    VAR_EFFECT_BLOCKED = 0x47,
    VAR_SHEER_FORCE = 0x48,
    VAR_TRICK_ROOM_FLAG = 0x4A,
    VAR_NO_TYPE_EFFECTIVENESS = 0x4B,
    VAR_SET_TYPE_EFFECTIVENESS = 0x4C,
    VAR_DELAY_ATTACK_FLAG = 0x4D,
    VAR_MAGIC_COAT = 0x4E,
    VAR_MESSAGE_FLAG = 0x4F,
    VAR_GENERAL_USE_FLAG = 0x51,
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

struct HandlerParam_StrParams
{
    u16 ID;
    int type;
    int args[8];
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

C_DECL_BEGIN
// AddExpAndEvs definitions
BtlSetup* MainModule_GetBtlSetup_0(MainModule* a1);
u32 CalcBaseExpGain(BattleMon* defeatedMon, int KeySystemLevelAdjust);
BtlType BtlSetup_GetBattleType(MainModule* a1);
int BattleParty_GetPartyCount(BattleParty* a1);
BtlType BtlSetup_GetBattleType(MainModule* a1);
void sys_memset(const void* ptr, u8 value, size_t size);
BattleMon* BattleParty_GetMonData(BattleParty* a1, unsigned int a2);
bool BattleMon_IsFainted(BattleMon* a1);
ItemID BattleMon_GetHeldItem(BattleMon* a1);
int BattleMon_GetConfrontRecCount(BattleMon* a1);
int BattleMon_GetConfrontedPokeID(BattleMon* a1, unsigned int a2);
BattleMon* PokeCon_GetPokeParam(PokeCon* a1, int a2);
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
BattleMon* PokeCon_GetPokeParam(PokeCon* a1, int a2);
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
int ServerControl_MoveConditionCore(ServerFlow* a1, int a2, int a3, int a4, int a5, int a6, int a7);
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
int IsPosOpenForRevivedMon(ServerFlow* a1);
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

u32* g_GameBeaconSys;
C_DECL_END

#endif