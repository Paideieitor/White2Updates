#include "custom/item_expansion.h"

// messages in game text file 6 (empty in vanilla)
#define BAG_ITEMUSED_MSGID 63
#define BAG_REPELEND_MSGID 64

C_DECL_BEGIN
bool THUMB_BRANCH_BagSave_IsItemFreeSpaceBit(BagSaveData* a1, int itemId)
{
    if (itemId > DEFAULT_LAST_ITEM)
        return false;
    return (a1->FreeSpaceBits[itemId >> 5] & (1 << ((itemId < 0) + ROR((itemId << 27) - ((unsigned int)itemId >> 31), 27)))) != 0;
}

// --- DATA ---
// NEW

// List of items that can be registered and trigger an effect on the Pokemon List menu
ItemID RegistrablePokeListItems[] = {
    IT0466_GRACIDEA,
    IT0628_DNA_SPLICERS,
    IT0629_DNA_SPLICERS,
    IT0638_REVEAL_GLASS,
    INFINITE_CANDY_ID  // Infinite Rare Candy
};


// OVL_142

// [OVL_142:021A09C0] List of items that have an effect that requires to load an overlay while in the bag, and the functions used to trigger that
ItemUseFunction ItemUseFunctionArray[] = {
    { IT0450_BIKE, 1, ItemUseFunction_Bicycle },
    { IT0442_TOWN_MAP, 1, ItemUseFunction_TownMap },
    { IT0437_PAL_PAD, 1, ItemUseFunction_PalPad },
    { IT0094_HONEY,  1, ItemUseFunction_Honey },
    { IT0447_SUPER_ROD, 1, ItemUseFunction_SuperRod },
    { IT0465_VS_RECORDER, 1, ItemUseFunction_VsRecorder },
    { IT0471_DOWSING_MACHINE, 1, ItemUseFunction_DowsingMCHN },
    { IT0078_ESCAPE_ROPE,  1, ItemUseFunction_EscapeRope },
    { IT0621_XTRANSCEIVER, 1, ItemUseFunction_Xtransceiver },
    { IT0626_XTRANSCEIVER, 1, ItemUseFunction_Xtransceiver },
    { IT0627_MEDAL_BOX, 1, ItemUseFunction_MedalBox },
};


// ARM9

// [ARM9_DATA:0208F4FC] Extended list that determines the bag pocket an item is put in, the index in the list matches the ItemID
u8 ExtItemCategories[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,3,3,3,3,3,3,
    3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
    3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
    4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,4,0,
    0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,4,4,2,2,2,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,/*Vanilla End*/
    0,0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0/*Max amount until item use battle text bugs out*/
};

// [ARM9:02090D7C] List of stuff about item graphics, no idea so far
u16 ITEM_GRAPHICS_CHARS_AND_PALETTES[] = {
    0x3FD,0x3FE,0x2,0x3,0x4,0x5,0x6,0x7,
    0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
    0x10,0x11,0x12,0x13,0x14,0x13,0x15,0x16,
    0x17,0x16,0x297,0x298,0x299,0x29A,0x29B,0x29C,
    0x29D,0x29E,0x18,0x19,0x1A,0x1B,0x1E,0x1C,
    0x1E,0x1D,0x1E,0x1F,0x1E,0x20,0x21,0x22,
    0x21,0x23,0x18,0x24,0x18,0x25,0x26,0x27,
    0x28,0x2A,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,
    0x2F,0x30,0x31,0x32,0x33,0x34,0x35,0x36,
    0x33,0x37,0x38,0x39,0x3A,0x3B,0x3A,0x3C,
    0x3A,0x3D,0x3A,0x3E,0x3F,0x40,0x47,0x48,
    0x49,0x4A,0x53,0x54,0x55,0x56,0x55,0x57,
    0x55,0x58,0x55,0x59,0x5A,0x5B,0x5C,0x5D,
    0x55,0x5E,0x5F,0x60,0x1D2,0x1D3,0x64,0x61,
    0x64,0x62,0x64,0x63,0x64,0x65,0x64,0x66,
    0x64,0x67,0x64,0x68,0x64,0x1D5,0x69,0x6A,
    0x6B,0x6C,0x41,0x42,0x41,0x43,0x41,0x44,
    0x41,0x45,0x41,0x46,0x33,0x4B,0x4C,0x4D,
    0x4E,0x4F,0x4E,0x50,0x4E,0x51,0x4E,0x52,
    0x6D,0x6E,0x6D,0x6F,0x70,0x71,0x6D,0x72,
    0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,
    0x7B,0x7C,0x7D,0x7E,0x7F,0x81,0x80,0x81,
    0x82,0x83,0x84,0x83,0x85,0x86,0x87,0x86,
    0x88,0x89,0x8A,0x8B,0x1D6,0x1D7,0x1D8,0x1D9,
    0x1DA,0x1DB,0x1DC,0x1DD,0x1DE,0x1DF,0x188,0x189,
    0x18A,0x189,0x1AF,0x1B0,0x1B1,0x1B0,0x1A9,0x1AA,
    0x269,0x26A,0x267,0x268,0x151,0x152,0x1E0,0x1E1,
    0x1E2,0x1E3,0x1E4,0x1E5,0x1E6,0x1E7,0x1E8,0x1E9,
    0x2BB,0x2BC,0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,
    0x389,0x38A,0x389,0x38B,0x389,0x38C,0x389,0x38D,
    0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,
    0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,
    0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,
    0x3FD,0x3FE,0x3FD,0x3FE,0x38E,0x38F,0x288,0x289,
    0x286,0x287,0x319,0x31A,0x31B,0x31C,0x31D,0x31E,
    0x31F,0x320,0x321,0x322,0x323,0x324,0x325,0x326,
    0x327,0x328,0x329,0x32A,0x32B,0x32C,0x32D,0x32E,
    0x32F,0x330,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,
    0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,0xB0,0xB1,
    0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,
    0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,0xC0,0xC1,
    0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,
    0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,0xD0,0xD1,
    0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,
    0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,0xE0,0xE1,
    0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,
    0x22A,0x22B,0x22C,0x22D,0x22E,0x22F,0x230,0x231,
    0x232,0x233,0x234,0x235,0x236,0x237,0x238,0x239,
    0x23A,0x23B,0x23C,0x23D,0x23E,0x23F,0x240,0x241,
    0x242,0x243,0x244,0x245,0x246,0x247,0x248,0x249,
    0x24A,0x24B,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
    0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,
    0xF8,0xF9,0x24C,0x24D,0x24E,0x24F,0x250,0x251,
    0x252,0x253,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,
    0x100,0x101,0x102,0x103,0x104,0x105,0xFC,0x106,
    0x107,0x108,0x109,0x10A,0x10B,0x10C,0x10D,0x10E,
    0x10F,0x110,0x111,0x112,0x113,0x114,0x115,0x116,
    0x117,0x118,0x119,0x11A,0x11B,0x11C,0x11D,0x11E,
    0x11F,0x120,0x121,0x122,0x123,0x124,0x125,0x126,
    0x127,0x128,0x129,0x12A,0x12B,0x12C,0x12D,0x12E,
    0x12F,0x130,0x131,0x130,0x132,0x133,0x134,0x135,
    0x136,0x137,0x138,0x139,0x13A,0x13B,0x13C,0x13D,
    0x13E,0x13F,0x140,0x141,0x142,0x143,0x144,0x145,
    0x146,0x147,0x148,0x4D,0x149,0x14A,0x14B,0x14C,
    0x14D,0x14E,0x14F,0x150,0x151,0x152,0x153,0x154,
    0x155,0x156,0x155,0x157,0x155,0x158,0x155,0x159,
    0x155,0x15A,0x1EA,0x1EB,0x1EC,0x1ED,0x1EE,0x1EF,
    0x1F0,0x1F1,0x263,0x264,0x1F2,0x1F3,0x1F4,0x1F5,
    0x1F6,0x1F7,0x1F8,0x1F9,0x14F,0x292,0x293,0x294,
    0x1FA,0x1FB,0x1FC,0x1FD,0x265,0x266,0x28A,0x28B,
    0x28E,0x28F,0x1FE,0x1FF,0x27C,0x27D,0x27E,0x27F,
    0x280,0x281,0x282,0x283,0x28C,0x28D,0x200,0x201,
    0x202,0x203,0x2AB,0x2AC,0x2B3,0x2B4,0x2B5,0x2B6,
    0x2AD,0x2AE,0x2AF,0x2B0,0x2B1,0x2B2,0x204,0x205,
    0x206,0x207,0x208,0x209,0x26B,0x26C,0x26B,0x26D,
    0x26B,0x26E,0x26B,0x26F,0x26B,0x270,0x26B,0x271,
    0x26B,0x272,0x26B,0x273,0x26B,0x274,0x26B,0x275,
    0x26B,0x276,0x26B,0x277,0x26B,0x278,0x26B,0x279,
    0x26B,0x27A,0x26B,0x27B,0x20A,0x20B,0x20C,0x20D,
    0x20E,0x20F,0x210,0x211,0x212,0x213,0x214,0x215,
    0x216,0x217,0x2B7,0x2B8,0x218,0x219,0x21A,0x21B,
    0x21C,0x21D,0x21E,0x21F,0x220,0x221,0x222,0x223,
    0x18D,0x197,0x18D,0x18F,0x18D,0x191,0x18D,0x191,
    0x18D,0x192,0x18D,0x193,0x18D,0x194,0x18D,0x18E,
    0x18D,0x193,0x18D,0x192,0x18D,0x196,0x18D,0x197,
    0x18D,0x194,0x18D,0x194,0x18D,0x192,0x18D,0x191,
    0x18D,0x192,0x18D,0x190,0x18D,0x191,0x18D,0x192,
    0x18D,0x192,0x18D,0x195,0x18D,0x19C,0x18D,0x199,
    0x18D,0x199,0x18D,0x19A,0x18D,0x192,0x18D,0x19A,
    0x18D,0x191,0x18D,0x19B,0x18D,0x18E,0x18D,0x192,
    0x18D,0x191,0x18D,0x193,0x18D,0x196,0x18D,0x193,
    0x18D,0x19C,0x18D,0x196,0x18D,0x19C,0x18D,0x19D,
    0x18D,0x197,0x18D,0x192,0x18D,0x196,0x18D,0x191,
    0x18D,0x192,0x18D,0x197,0x18D,0x18E,0x18D,0x192,
    0x18D,0x192,0x18D,0x196,0x18D,0x191,0x18D,0x18E,
    0x18D,0x195,0x18D,0x192,0x18D,0x190,0x18D,0x197,
    0x18D,0x199,0x18D,0x19D,0x18D,0x196,0x18D,0x197,
    0x18D,0x196,0x18D,0x19D,0x18D,0x197,0x18D,0x192,
    0x18D,0x19B,0x18D,0x197,0x18D,0x192,0x18D,0x192,
    0x18D,0x19C,0x18D,0x192,0x18D,0x19C,0x18D,0x199,
    0x18D,0x199,0x18D,0x198,0x18D,0x192,0x18D,0x262,
    0x18D,0x192,0x18D,0x19A,0x18D,0x194,0x18D,0x19C,
    0x18D,0x262,0x18D,0x18F,0x18D,0x192,0x18D,0x193,
    0x18D,0x191,0x18D,0x195,0x18D,0x192,0x18D,0x19D,
    0x18D,0x262,0x18D,0x192,0x18D,0x198,0x18D,0x191,
    0x19E,0x192,0x19E,0x19D,0x19E,0x190,0x19E,0x192,
    0x19E,0x190,0x19E,0x190,0x3FD,0x3FE,0x3FD,0x3FE,
    0x224,0x225,0x226,0x227,0x228,0x229,0x2B9,0x2BA,
    0x2A9,0x2AA,0x25E,0x25F,0x29F,0x2A0,0x2A3,0x2A4,
    0x2A1,0x2A2,0x295,0x296,0x284,0x285,0x25C,0x25D,
    0x290,0x291,0x2A7,0x2A8,0x1B7,0x1B8,0x1B9,0x1BA,
    0x15D,0x15E,0x161,0x162,0x163,0x164,0x165,0x166,
    0x16B,0x16C,0x175,0x176,0x1B5,0x1B6,0x1AD,0x1AE,
    0x177,0x40,0x25A,0x25B,0x260,0x261,0x2A5,0x2A6,
    0x167,0x168,0x169,0x16A,0x254,0x255,0x19F,0x1A0,
    0x1A1,0x1A2,0x1A3,0x1A4,0x1A5,0x1A6,0x256,0x257,
    0x258,0x259,0x2BD,0x2BE,0x2BF,0x2C0,0x2C1,0x2C2,
    0x2C5,0x2C6,0x2F7,0x2F8,0x2C7,0x2C8,0x3E5,0x3E6,
    0x2F9,0x2FA,0x2FB,0x2FC,0x2FD,0x2FE,0x2FF,0x300,
    0x301,0x302,0x2C9,0x2CA,0x303,0x304,0x305,0x306,
    0x307,0x308,0x309,0x30A,0x30B,0x30C,0x30D,0x30E,
    0x30F,0x310,0x2DD,0x2DE,0x2DF,0x2E0,0x2E1,0x2E2,
    0x2E3,0x2E4,0x2E5,0x2E6,0x2E7,0x2E8,0x2E9,0x2EA,
    0x2D3,0x2D4,0x2CD,0x2CE,0x2CB,0x2CC,0x2D1,0x2D2,
    0x2D7,0x2D8,0x2D5,0x2D6,0x2CF,0x2D0,0x2DB,0x2DC,
    0x2D9,0x2DA,0x25E,0x25F,0x311,0x312,0x313,0x314,
    0x2ED,0x2EE,0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,
    0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,
    0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,
    0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,
    0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,
    0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,
    0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,0x3FD,0x3FE,
    0x2F3,0x2F4,0x317,0x318,0x2EF,0x2F0,0x2F1,0x2F2,
    0x2F5,0x2F6,0x390,0x391,0x392,0x393,0x394,0x395,
    0x396,0x397,0x398,0x399,0x39A,0x39B,0x39C,0x39D,
    0x39E,0x39F,0x3A0,0x3A1,0x3A2,0x3A3,0x3A4,0x3A5,
    0x3A6,0x3A7,0x3A6,0x3A8,0x3A6,0x3A9,0x3A6,0x3AA,
    0x3A6,0x3AB,0x3A6,0x3AC,0x3A6,0x3AD,0x3A6,0x3AE,
    0x3A6,0x3AF,0x3A6,0x3B0,0x3A6,0x3B1,0x3A6,0x3B2,
    0x3A6,0x3B3,0x3A6,0x3B4,0x3A6,0x3B5,0x3A6,0x3B6,
    0x3A6,0x3B7,0x3B8,0x3B9,0x3B8,0x3BA,0x3B8,0x3BB,
    0x3B8,0x3BC,0x3B8,0x3BD,0x3B8,0x3BE,0x3BF,0x3C0,
    0x3C1,0x3C2,0x3C3,0x3C4,0x3C5,0x3C6,0x3C7,0x3C8,
    0x3C9,0x3CA,0x3CB,0x3CC,0x331,0x332,0x333,0x334,
    0x3CD,0x3CE,0x3CF,0x3D0,0x3D1,0x3D2,0x3D3,0x3D4,
    0x3D5,0x3D6,0x3D5,0x3D8,0x3D5,0x3DA,0x3DB,0x3DC,
    0x3DD,0x3DE,0x3DF,0x3E0,0x3E1,0x3E2,0x3E3,0x3E4,
    0x348,0x349,0x348,0x34A,0x348,0x34B,0x348,0x34C,
    0x348,0x34D,0x348,0x34E,0x348,0x34F,0x350,0x352,
    0x350,0x353,0x350,0x354,0x350,0x355,0x350,0x356,
    0x350,0x357,0x358,0x359,0x358,0x35A,0x358,0x35B,
    0x358,0x35C,0x358,0x35D,0x358,0x35E,0x35F,0x360,
    0x361,0x362,0x363,0x364,0x365,0x366,0x350,0x351,
    0x335,0x336,0x337,0x338,0x18D,0x199,0x18D,0x19C,
    0x18D,0x197,0x339,0x3E7,0x33B,0x33C,0x33D,0x33E,
    0x33D,0x33E,0x33D,0x33E,0x339,0x33A,0x3EA,0x3E9,
    0x3EC,0x3EB,0x3EC,0x3EB,0x3F0,0x3EF,0x3F2,0x3F1,
    0x3F4,0x3F3,0x3F6,0x3F5,0x3F8,0x3F7,0x3FA,0x3F9,
    0x339,0x3E7,0x339,0x33A,0x3FC,0x3FB,/*Vanilla End*/
    1025, 1026, // Weakness Policy
    1027, 1028, // Assault Vest
    1029, 1030, // Pixie Plate
    1031, 1032, // Ability Capasule
    1033, 1034, // Luminous Moss
    1035, 1036, // Snowball
    1037, 1038, // Safety Goggles
    1039, 1040, // Roseli Berry
    1041, 1042, // Kee Berry
    1043, 1044, // Maranga Berry
    1045, 1046, // Bottle Cap
    1045, 1047, // Gold Bottle Cap
    1048, 1049, // Adrenaline Orb
    1050, 1051, // Protective Pads
    1052, 1053, // Throat Spray
    0x3FD, 0x3FE, // ERROR
    0x3FD, 0x3FE, // ERROR
    0x3FD, 0x3FE, // ERROR
    0x3FD, 0x3FE, // ERROR
    0x3FD, 0x3FE, // ERROR
    0x3FD, 0x3FE, // ERROR
    0x3FD, 0x3FE, // ERROR
    0x3FD, 0x3FE, // ERROR
    0x3FD, 0x3FE, // ERROR
    0x3FD, 0x3FE, // ERROR
    0x3FD, 0x3FE, // ERROR
    1054, 1055, // Eject Pack
    1056, 1057, // Heavy-Duty Boots
    1025, 1058, // Blunder Policy
    1059, 1060, // Room Service
    1061, 1062, // Utility Ubrella
    1063, 1064, // Attack Mint
    1063, 1064, // Attack Mint
    1063, 1064, // Attack Mint
    1063, 1064, // Attack Mint
    1063, 1065, // Defense Mint
    1063, 1065, // Defense Mint
    1063, 1065, // Defense Mint
    1063, 1065, // Defense Mint
    1063, 1066, // Sp.Attack Mint
    1063, 1066, // Sp.Attack Mint
    1063, 1066, // Sp.Attack Mint
    1063, 1066, // Sp.Attack Mint
    1063, 1067, // Sp.Defense Mint
    1063, 1067, // Sp.Defense Mint
    1063, 1067, // Sp.Defense Mint
    1063, 1067, // Sp.Defense Mint
    1063, 1068, // Speed Mint
    1063, 1068, // Speed Mint
    1063, 1068, // Speed Mint
    1063, 1068, // Speed Mint
    1063, 1069, // Neutral Mint
    1070, 1071, // Ability Patch
    0x3FD, 0x3FE, // ERROR
    0x3FD, 0x3FE, // ERROR
    0x3FD, 0x3FE, // ERROR
    0x3FD, 0x3FE, // ERROR
    1072, 1073, // Ability Shield
    1074, 1075, // Clear Amulet
    1076, 1077, // Mirror Herb
    1078, 1079, // Punching Gloves
    1080, 1081, // Covert Cloak
    1082, 1083, // Loaded Dice
    1084, 1085, // Fairy Feather
};

// [ARM9:02092A24] Extended list of items that can be registered and their respective RegisterID
u16 ExtRegistrableItems[] = {
    REG_ITEM(REGIT_BIKE, IT0450_BIKE),
    REG_ITEM(REGIT_SUPER_ROD, IT0447_SUPER_ROD),
    REG_ITEM(REGIT_TOWN_MAP, IT0442_TOWN_MAP),
    REG_ITEM(REGIT_VS_RECORDER, IT0465_VS_RECORDER),
    REG_ITEM(REGIT_PAL_PAD, IT0437_PAL_PAD),
    REG_ITEM(REGIT_DOWSING_MCHN, IT0471_DOWSING_MACHINE),
    REG_ITEM(REGIT_GRACIDEA, IT0466_GRACIDEA),
    REG_ITEM(REGIT_XTRANSCEIVER, IT0621_XTRANSCEIVER),
    REG_ITEM(REGIT_XTRANSCEIVER, IT0626_XTRANSCEIVER),
    REG_ITEM(REGIT_MEDAL_BOX, IT0627_MEDAL_BOX),
    REG_ITEM(REGIT_DNA_SPLICERS_1, IT0628_DNA_SPLICERS),
    REG_ITEM(REGIT_DNA_SPLICERS_2, IT0629_DNA_SPLICERS),
    REG_ITEM(REGIT_REVEAL_GLASS, IT0638_REVEAL_GLASS),
    REG_ITEM(REGIT_INFINITE_CANDY, IT0622),
    REG_ITEM(REGIT_INFINITE_REPEL, IT0079_REPEL),
};

ITEM_ID ExtBerryItemIDs[] = {
IT0149_CHERI_BERRY,
IT0150_CHESTO_BERRY,
IT0151_PECHA_BERRY,
IT0152_RAWST_BERRY,
IT0153_ASPEAR_BERRY,
IT0154_LEPPA_BERRY,
IT0155_ORAN_BERRY,
IT0156_PERSIM_BERRY,
IT0157_LUM_BERRY,
IT0158_SITRUS_BERRY,
IT0159_FIGY_BERRY,
IT0160_WIKI_BERRY,
IT0161_MAGO_BERRY,
IT0162_AGUAV_BERRY,
IT0163_IAPAPA_BERRY,
IT0164_RAZZ_BERRY,
IT0165_BLUK_BERRY,
IT0166_NANAB_BERRY,
IT0167_WEPEAR_BERRY,
IT0168_PINAP_BERRY,
IT0169_POMEG_BERRY,
IT0170_KELPSY_BERRY,
IT0171_QUALOT_BERRY,
IT0172_HONDEW_BERRY,
IT0173_GREPA_BERRY,
IT0174_TAMATO_BERRY,
IT0175_CORNN_BERRY,
IT0176_MAGOST_BERRY,
IT0177_RABUTA_BERRY,
IT0178_NOMEL_BERRY,
IT0179_SPELON_BERRY,
IT0180_PAMTRE_BERRY,
IT0181_WATMEL_BERRY,
IT0182_DURIN_BERRY,
IT0183_BELUE_BERRY,
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
IT0201_LIECHI_BERRY,
IT0202_GANLON_BERRY,
IT0203_SALAC_BERRY,
IT0204_PETAYA_BERRY,
IT0205_APICOT_BERRY,
IT0206_LANSAT_BERRY,
IT0207_STARF_BERRY,
IT0208_ENIGMA_BERRY,
IT0209_MICLE_BERRY,
IT0210_CUSTAP_BERRY,
IT0211_JABOCA_BERRY,
IT0212_ROWAP_BERRY,
IT0686_ROSELI_BERRY,
IT0687_KEE_BERRY,
IT0688_MARANGA_BERRY,
};

// --- FUNCTIONS ---
// NEW

// Checks if an item is a registrable item with an effect in the Pokemon List menu
bool IsRegistrablePokeListItem(ItemID itemID)
{
    for (u16 i = 0; i < ARRAY_COUNT(RegistrablePokeListItems); ++i)
        if (itemID == RegistrablePokeListItems[i])
            return true;
    return false;
}

ItemData* LoadNewItemData(ItemID item_idx, HeapID heapId)
{
    if (IsEqual(item_idx, IT0639_WEAKNESS_POLICY))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 200;
        data->HeldEffect = 167;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 1;
        data->SortIndex = 0;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT0640_ASSAULT_VEST))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 200;
        data->HeldEffect = 115;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 80;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 0;
        data->SortIndex = 11;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT0644_PIXIE_PLATE)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 300;
        data->HeldEffect = 117;
        data->HeldArgument = 4;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 70;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 0;
        data->SortIndex = 60;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT0645_ABILITY_CAPSULE)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_ABILITY, ABILITY_CAPSULE);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT0648_LUMINOUS_MOSS))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 200;
        data->HeldEffect = 167;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 1;
        data->SortIndex = 0;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT0649_SNOWBALL))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 200;
        data->HeldEffect = 167;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 1;
        data->SortIndex = 0;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT0650_SAFETY_GOGGLES))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 200;
        data->HeldEffect = 115;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 10;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 0;
        data->SortIndex = 11;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT0686_ROSELI_BERRY))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        // COPIED FROM OCCA BERRY, PROBABLY WRONG
        data->Price = 2;
        data->HeldEffect = 19;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 10;
        data->NaturalGiftPower = 80;
        data->Packed = 0x189; // fire type natural gift
        data->EffectField = 8;
        data->EffectBattle = 0;
        data->HasBattleStats = 1;
        data->ItemClass = 0;
        data->Consumable = 1;
        data->SortIndex = 0;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT0687_KEE_BERRY))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        // COPIED FROM OCCA BERRY, PROBABLY WRONG
        data->Price = 2;
        data->HeldEffect = 19;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 1; // added fling effect
        data->FlingPower = 10;
        data->NaturalGiftPower = 100;
        data->Packed = 0x189; // fire type natural gift
        data->EffectField = 8;
        data->EffectBattle = 0;
        data->HasBattleStats = 1;
        data->ItemClass = 0;
        data->Consumable = 1;
        data->SortIndex = 0;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT0688_MARANGA_BERRY))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        // COPIED FROM OCCA BERRY, PROBABLY WRONG
        data->Price = 2;
        data->HeldEffect = 19;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 1; // added fling effect
        data->FlingPower = 10;
        data->NaturalGiftPower = 100;
        data->Packed = 0x189; // fire type natural gift
        data->EffectField = 8;
        data->EffectBattle = 0;
        data->HasBattleStats = 1;
        data->ItemClass = 0;
        data->Consumable = 1;
        data->SortIndex = 1;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT0795_BOTTLE_CAP)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_IV_MOD, MAX_IV); // doesn't work
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;

    }
    if (IsEqual(item_idx, IT0796_GOLD_BOTTLE_CAP)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_IV_MOD, MAX_ALL);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;

    }
    if (IsEqual(item_idx, IT0846_ADRENALINE_ORB))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 200;
        data->HeldEffect = 167;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 1;
        data->SortIndex = 0;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT0880_PROTECTIVE_PADS))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 200;
        data->HeldEffect = 115;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 0;
        data->SortIndex = 11;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1118_THROAT_SPRAY))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 200;
        data->HeldEffect = 167;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 1;
        data->SortIndex = 0;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1119_EJECT_PACK))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 300;
        data->HeldEffect = 167;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 50;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 1;
        data->SortIndex = 0;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1120_HEAVY_DUTY_BOOTS))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 200;
        data->HeldEffect = 115;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 80;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 0;
        data->SortIndex = 11;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1121_BLUNDER_POLICY))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);
    
        data->Price = 200;
        data->HeldEffect = 167;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 1;
        data->SortIndex = 0;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;
    
        return data;
    }
    if (IsEqual(item_idx, IT1122_ROOM_SERVICE))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 200;
        data->HeldEffect = 167;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 100;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 1;
        data->SortIndex = 0;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1123_UTILITY_UMBRELLA))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 200;
        data->HeldEffect = 115;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 80;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 0;
        data->SortIndex = 11;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1231_LONELY_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_LONELY);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1232_ADAMANT_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_ADAMANT);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1233_NAUGHTY_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_NAUGHTY);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1234_BRAVE_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_BRAVE);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1235_BOLD_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_BOLD);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1236_IMPISH_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_IMPISH);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1237_LAX_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_LAX);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1238_RELAXED_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_RELAXED);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1239_MODEST_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_MODEST);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1240_MILD_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_MILD);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1241_RASH_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_RASH);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1242_QUIET_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_QUIET);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1243_CALM_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_CALM);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1244_GENTLE_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_GENTLE);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1245_CAREFUL_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_CAREFUL);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;

    }
    if (IsEqual(item_idx, IT1246_SASSY_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_SASSY);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;

    }
    if (IsEqual(item_idx, IT1247_TIMID_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_TIMID);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;

    }
    if (IsEqual(item_idx, IT1248_HASTY_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_HASTY);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;

    }
    if (IsEqual(item_idx, IT1249_JOLLY_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_JOLLY);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;

    }
    if (IsEqual(item_idx, IT1250_NAIVE_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_NAIVE);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;

    }
    if (IsEqual(item_idx, IT1251_SERIOUS_MINT)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_NATURE, NAT_SERIOUS);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;

    }
    if (IsEqual(item_idx, IT1606_ABILITY_PATCH)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 480;
        data->HeldEffect = 0;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x9F;
        data->EffectField = 1;
        data->EffectBattle = 0;
        data->HasBattleStats = MAKE_NEW_EFFECT(ITEFF_ABILITY, ABILITY_PATCH);
        data->ItemClass = 0;
        data->Consumable = 0;
        data->SortIndex = 100;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;

    }
    if (IsEqual(item_idx, IT1881_ABILITY_SHIELD))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 300;
        data->HeldEffect = 115;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 0;
        data->SortIndex = 11;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1882_CLEAR_AMULET))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 300;
        data->HeldEffect = 115;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 0;
        data->SortIndex = 11;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1883_MIRROR_HERB))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 200;
        data->HeldEffect = 167;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 1;
        data->SortIndex = 0;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1886_LOADED_DICE))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 300;
        data->HeldEffect = 115;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 0;
        data->SortIndex = 11;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1884_PUNCHING_GLOVE))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 300;
        data->HeldEffect = 115;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 0;
        data->SortIndex = 11;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT1885_COVERT_CLOAK))
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 300;
        data->HeldEffect = 115;
        data->HeldArgument = 0;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 0;
        data->SortIndex = 11;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    if (IsEqual(item_idx, IT2401_FAIRY_FEATHER)) // NEW POKEMON EFFECT ITEM ARCHETYPE
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 300;
        data->HeldEffect = 117;
        data->HeldArgument = 4;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 0;
        data->SortIndex = 60;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
    
    if (item_idx > DEFAULT_LAST_ITEM)
    {
        ItemData* data;
        data = (ItemData*)GFL_HeapAllocate(heapId, sizeof(ItemData), 0, "arc_tool.c", 0xBBu);

        data->Price = 300;
        data->HeldEffect = 117;
        data->HeldArgument = 4;
        data->NaturalGiftEffect = 0;
        data->FlingEffect = 0;
        data->FlingPower = 30;
        data->NaturalGiftPower = 0;
        data->Packed = 0x1F;
        data->EffectField = 0;
        data->EffectBattle = 0;
        data->HasBattleStats = 0;
        data->ItemClass = 1;
        data->Consumable = 0;
        data->SortIndex = 60;
        data->BattleStats.CureInflict = 0;
        data->BattleStats.Boost[0] = 0;
        data->BattleStats.Boost[1] = 0;
        data->BattleStats.Boost[2] = 0;
        data->BattleStats.Boost[3] = 0;
        data->BattleStats.FunctionFlags0 = 0;
        data->BattleStats.FunctionFlags1 = 0;
        data->BattleStats.EVHP = 0;
        data->BattleStats.EVATK = 0;
        data->BattleStats.EVDEF = 0;
        data->BattleStats.EVSPE = 0;
        data->BattleStats.EVSPA = 0;
        data->BattleStats.EVSPD = 0;
        data->BattleStats.HealAmount = 0;
        data->BattleStats.PPGain = 0;
        data->BattleStats.Friendship1 = 0;
        data->BattleStats.Friendship2 = 0;
        data->BattleStats.Friendship3 = 0;
        data->BattleStats.field_1F = 0;
        data->BattleStats.field_20 = 0;

        return data;
    }
   
    return nullptr;
}

// OVL_142

// Function that is called when the repel item is used from the bag, triggers the effect and creates the text boxes
int THUMB_BRANCH_Bag_RepelEffect(BigBagStructTemp* a1)
{
    SaveControl* SaveControl;
    EncountSave* EncountSave;
    u8 ItemParam;

    u32 itemID = a1->itemID_8A0;
    if (itemID == INFINITE_REPEL_ID)
    {
        SaveControl = GameData_GetSaveControl(*(GameData**)a1);
        EncountSave = SaveControl_GetEncountSave(SaveControl);
        if (!EncountSave->RepelSteps)
        {
            EncountSave->RepelSteps = 0xFF;

            GFL_MsgDataLoadStrbuf(a1->msgData, BAG_ITEMUSED_MSGID, a1->strBuf1);
            copyVarForText(a1->wordSetSystem, 0, a1->void_ptr_8);
            Bag_LoadItemName(a1, 1, itemID);
            GFL_WordSetFormatStrbuf(a1->wordSetSystem, a1->strBuf2, a1->strBuf1);
            Bag_CreateTextBox(a1, 1);
            playSeqFullVol(1996);
        }
        else
        {
            EncountSave->RepelSteps = 0x00;

            GFL_MsgDataLoadStrbuf(a1->msgData, BAG_REPELEND_MSGID, a1->strBuf1);
            copyVarForText(a1->wordSetSystem, 0, a1->void_ptr_8);
            Bag_LoadItemName(a1, 1, itemID);
            GFL_WordSetFormatStrbuf(a1->wordSetSystem, a1->strBuf2, a1->strBuf1);
            Bag_CreateTextBox(a1, 1);
            playSeqFullVol(1996);
        }

        int output = sub_2199900((_DWORD*)a1, (int)sub_219A22C);
        return output;
    }
    return false;
}

// CAUTION: This function is called LoadSavedFreeSpaceItems in other ESDBs
void THUMB_BRANCH_sub_21A0430(_WORD* a1, BagSaveData* bagSaveData, unsigned __int16 a3, __int16 heapID)
{
    int v5; // r4
    int v7; // r2
    int v8; // r3
    int ExistingItemPocket; // r0
    _WORD* v10; // r1
    _WORD* v11; // r0

    *((_DWORD*)a1 + 1) = (_DWORD)bagSaveData;
    v5 = 0;
    a1[4] = heapID;
    memsetx(a1 + 6, 0, 5112u);
    for (u16 i = 0; i < IT_ITEM_AMOUNT; ++i) // removed limiter
    {
        bool first = !BagSave_IsItemInFreeSpace(*((BagSaveData**)a1 + 1), i); // why is this giving false positives?
        ExistingItemPocket = BagSave_GetExistingItemPocket(*((BagSaveData**)a1 + 1), i);
        bool second = ExistingItemPocket == 6;

        // DPRINTF("%d || %d -> ", first, second);

        if (first
            || second)
        {
            // DPRINTF("%d NOT IN FREE SPACE\n", i);
            v11 = &a1[4 * v5];
            v11[6] = 0;
            v11[7] = 6;
            *((_DWORD*)v11 + 4) = 0;
        }
        else
        {
            // DPRINTF("%d IN FREE SPACE\n", i);
            v10 = &a1[4 * v5];
            v10[6] = i;
            v10[7] = ExistingItemPocket;
            *((_DWORD*)v10 + 4) = 1;
            ++v5;
        }
    }
    *a1 = 0;
    a1[1] = 0;
    sub_21A067C((int)a1, a3, v7, v8);
}


// ARM 9

// Checks if an item is a Berry
int THUMB_BRANCH_PML_ItemIsBerry(u16 itemId)
{
    for (u16 i = 0; i < ARRAY_COUNT(ExtBerryItemIDs); ++i)
        if (itemId == ExtBerryItemIDs[i]) // Using extended list
            return 1;
    return 0;
};

// Checks if an item can be registered and returns its RegisterID if it does
// CAUTION: This function is called GetRegistrableID in the newer ESDBs
RegistrableID THUMB_BRANCH_Bag_GetRegistrableID(ItemID itemID)
{
    for (u16 i = 0; i < ARRAY_COUNT(ExtRegistrableItems); ++i)
    {
        u16 regItem = ExtRegistrableItems[i];
        if (itemID == (regItem & 0x3FF))
        {
            return (RegistrableID)(regItem >> 10);
        }
    }
    return REGIT_NOT_REGISTRABLE;
}

// Returns the pocket an item is ment to be stored at
PocketType THUMB_BRANCH_PML_ItemGetType(u16 item_idx)
{
    return (PocketType)ExtItemCategories[item_idx];
}

void* THUMB_BRANCH_PML_ItemReadDataFile(u16 item_idx, ItemDataType info_type, s16 heapId)
{
    int idxSafe;

    idxSafe = item_idx;
    if (item_idx > DEFAULT_LAST_ITEM)
        idxSafe = 0;
    switch (info_type)
    {
    case ITEMDATA_PARAMS:
    {
        ItemData* itemData = LoadNewItemData((ItemID)item_idx, heapId);
        if (itemData)
            return itemData;
    }
        return GFL_ArcSysReadHeapNew(24, idxSafe, (HeapID)heapId);
    case ITEMDATA_SPR1:
        return GFL_ArcSysReadHeapNew(25, ITEM_GRAPHICS_CHARS_AND_PALETTES[2 * idxSafe], (HeapID)heapId);
    case ITEMDATA_SPR2:
        return GFL_ArcSysReadHeapNew(25, ITEM_GRAPHICS_CHARS_AND_PALETTES[2 * idxSafe + 1], (HeapID)heapId);
    }
    return 0;
}

void* THUMB_BRANCH_PML_ItemArcHandleReadFile(ArcTool* arc, u16 item_idx, HeapID heapId)
{
    if (item_idx >= IT_ITEM_AMOUNT) // removed item limiter
    {
        item_idx = 0;
    }
    ItemData* itemData = LoadNewItemData((ItemID)item_idx, heapId);
    if (itemData)
        return itemData;
        
    return GFL_ArcToolReadHeapNew(arc, item_idx, heapId);
}

void THUMB_BRANCH_setItemNameToStrbuf(StrBuf* subStrBuf, int msgID, s16 blkGroupID)
{
    int v4;
    MsgData* Data;

    v4 = msgID;
    if (msgID >= IT_ITEM_AMOUNT) // removed limiter
    {
        v4 = 0;
    }
    Data = GFL_MsgSysLoadData(0, 2, 64, blkGroupID);
    GFL_MsgDataLoadStrbuf(Data, v4, subStrBuf);
    GFL_MsgDataFree(Data);
}

void THUMB_BRANCH_setItemDescriptionTextToStrbuf(u16* subStrBuf, int msgID, s16 blkGroupID)
{
    int v4;
    MsgData* Data;

    v4 = msgID;
    if (msgID >= IT_ITEM_AMOUNT) // removed limiter
    {
        v4 = 0;
    }
    Data = GFL_MsgSysLoadData(0, 2, 63, (HeapID)blkGroupID);
    GFL_MsgDataLoadStrbuf(Data, v4, (StrBuf*)subStrBuf);
    GFL_MsgDataFree(Data);
}

int THUMB_BRANCH_BagSave_GetFreeSpaceItemCount(BagSaveData* bag)
{
    int v2;
    int i;

    v2 = 0;
    for (i = 0; i < IT_ITEM_AMOUNT; ++i) // removed limiter
    {
        if (i && BagSave_IsItemInFreeSpace(bag, i))
        {
            ++v2;
        }
    }

    return v2;
}

// OVL_12

// Returns data from the a Registrable ID, if the function returns 0 with a3 being 0 the item loads the Pokemon List overlay
// CAUTION: This function is called ShortcutMenu_GetActionFromKeyItem in other ESDBs
int THUMB_BRANCH_sub_215B22C(RegistrableID registrableID, _DWORD* actionID, _DWORD* a3)
{
    *a3 = 0;
    switch (registrableID)
    {
    case REGIT_BIKE:            *actionID = ACTID_BIKE;            break;
    case REGIT_TOWN_MAP:        *actionID = ACTID_TOWN_MAP;        break;
    case REGIT_VS_RECORDER:     *actionID = ACTID_VS_RECORDER;     break;
    case REGIT_PAL_PAD:         *actionID = ACTID_PAL_PAD;         break;
    case REGIT_SUPER_ROD:       *actionID = ACTID_FISHING;         break;
    case REGIT_DOWSING_MCHN:    *actionID = ACTID_DOWSING_MCHN;    break;
    case REGIT_XTRANSCEIVER:    *actionID = ACTID_XTRANSCEIVER;    break;
    case REGIT_MEDAL_BOX:       *actionID = ACTID_MEDAL_BOX;       break;
    case REGIT_INFINITE_REPEL:  *actionID = ACTID_REPEL;           break;
    case REGIT_GRACIDEA:
    case REGIT_DNA_SPLICERS_1:
    case REGIT_DNA_SPLICERS_2:
    case REGIT_REVEAL_GLASS:
    case REGIT_INFINITE_CANDY:
        return 0;
    case REGIT_NOT_REGISTRABLE:
    default:
        *a3 = 1;
        return 0;
    }
    return 1;
}

// Fills the data to trigger the registered item effect
// CAUTION: This function is called ShortcutMenu_SetKeyItemID in other ESDBs
bool THUMB_BRANCH_sub_215B050(ShortcutMenuWork* wk, RegistrableID registrableID)
{
    bool isCommonEvent;

    switch (registrableID)
    {
    case REGIT_BIKE:
        wk->EventID = 0;
        isCommonEvent = 1;
        break;
    case REGIT_TOWN_MAP:
        wk->field_10 = 8;
        isCommonEvent = 0;
        break;
    case REGIT_VS_RECORDER:
        wk->field_10 = 12;
        isCommonEvent = 0;
        break;
    case REGIT_PAL_PAD:
        wk->field_10 = 9;
        isCommonEvent = 0;
        break;
    case REGIT_SUPER_ROD:
        wk->EventID = 4;
        isCommonEvent = 1;
        break;
    case REGIT_DOWSING_MCHN:
        wk->EventID = 5;
        isCommonEvent = 1;
        break;
    case REGIT_GRACIDEA:
        wk->field_10 = 0;
        wk->ItemID = IT0466_GRACIDEA;
        isCommonEvent = 0;
        break;
    case REGIT_DNA_SPLICERS_1:
        wk->field_10 = 0;
        wk->ItemID = IT0628_DNA_SPLICERS;
        isCommonEvent = 0;
        break;
    case REGIT_DNA_SPLICERS_2:
        wk->field_10 = 0;
        wk->ItemID = IT0629_DNA_SPLICERS;
        isCommonEvent = 0;
        break;
    case REGIT_POKEINFO_STATUS:
        wk->field_10 = 7;
        wk->ItemID = 0;
        isCommonEvent = 0;
        break;
    case REGIT_POKEINFO_SKILLS:
        wk->field_10 = 7;
        wk->ItemID = 1;
        isCommonEvent = 0;
        break;
    case REGIT_POKEINFO_RIBBONS:
        wk->field_10 = 7;
        wk->ItemID = 2;
        isCommonEvent = 0;
        break;
    case REGIT_BAG_ITEMS:
        wk->field_10 = 2;
        wk->ItemID = 0;
        isCommonEvent = 0;
        break;
    case REGIT_BAG_MEDICINE:
        wk->field_10 = 2;
        wk->ItemID = 1;
        isCommonEvent = 0;
        break;
    case REGIT_BAG_TMS_HMS:
        wk->field_10 = 2;
        wk->ItemID = 2;
        isCommonEvent = 0;
        break;
    case REGIT_BAG_BERRIES:
        wk->field_10 = 2;
        wk->ItemID = 3;
        isCommonEvent = 0;
        break;
    case REGIT_BAG_KEYITEMS:
        wk->field_10 = 2;
        wk->ItemID = 4;
        isCommonEvent = 0;
        break;
    case REGIT_BAG_FREESPACE:
        wk->field_10 = 2;
        wk->ItemID = 5;
        isCommonEvent = 0;
        break;
    case REGIT_POKEDEX_LIST:
        wk->field_10 = 1;
        wk->ItemID = 1;
        isCommonEvent = 0;
        break;
    case REGIT_POKEDEX_SEARCH:
        wk->field_10 = 1;
        wk->ItemID = 6;
        isCommonEvent = 0;
        break;
    case REGIT_POKEDEX_INFO:
        wk->field_10 = 1;
        wk->ItemID = 2;
        isCommonEvent = 0;
        break;
    case REGIT_POKEDEX_AREA:
        wk->field_10 = 1;
        wk->ItemID = 3;
        isCommonEvent = 0;
        break;
    case REGIT_POKEDEX_CRY:
        wk->field_10 = 1;
        wk->ItemID = 4;
        isCommonEvent = 0;
        break;
    case REGIT_POKEDEX_FORMS:
        wk->field_10 = 1;
        wk->ItemID = 5;
        isCommonEvent = 0;
        break;
    case REGIT_TRAINERCARD_FRONT:
        wk->field_10 = 3;
        wk->ItemID = 1;
        isCommonEvent = 0;
        break;
    case REGIT_TRAINERCARD_BACK:
        wk->field_10 = 3;
        wk->ItemID = 2;
        isCommonEvent = 0;
        break;
    case REGIT_TRAINERCARD_BADGES:
        wk->field_10 = 3;
        wk->ItemID = 3;
        isCommonEvent = 0;
        break;
    case REGIT_OPTIONS:
        wk->field_10 = 5;
        isCommonEvent = 0;
        break;
    case REGIT_XTRANSCEIVER:
        wk->field_10 = 13;
        isCommonEvent = 0;
        break;
    case REGIT_MEDAL_BOX:
        wk->field_10 = 14;
        isCommonEvent = 0;
        break;
    case REGIT_POKEMON_LIST:
        wk->field_10 = 0;
        wk->ItemID = -1;
        isCommonEvent = 0;
        break;
    case REGIT_POKEDEXHABITAT_LIST:
        wk->field_10 = 1;
        wk->ItemID = 7;
        isCommonEvent = 0;
        break;
    case REGIT_POKEDEXHABITAT_INFO:
        wk->field_10 = 1;
        wk->ItemID = 8;
        isCommonEvent = 0;
        break;
    case REGIT_REVEAL_GLASS:
        wk->field_10 = 0;
        wk->ItemID = IT0638_REVEAL_GLASS;
        isCommonEvent = 0;
        break;
    case REGIT_INFINITE_CANDY:
        wk->field_10 = 0; // triggers the Pokemon List menu
        wk->ItemID = INFINITE_CANDY_ID; // sets the item to use
        isCommonEvent = 0;
        break;
    case REGIT_INFINITE_REPEL:
        wk->EventID = 6; // sets the event type
        isCommonEvent = 1; // calls an event
        break;
    case REGIT_NOT_REGISTRABLE:
    default:
        wk->field_10 = 1;
        return 0;
    }
    return isCommonEvent;
}

// Called when a registered item that triggers the Pokemon List menu is used (pretty sure is called when an item is used in general)
int THUMB_BRANCH_sub_215B818(int a1, int itemId, int a3, int a4)
{
    PokeParty* Party;
    int v8;
    int v9;
    int v10;
    int v11;
    int v12;
    unsigned int v13;
    int v14;
    char v15;
    int v16;
    int v17;
    int v18;
    GameData* gameData;

    gameData = GSYS_GetGameData(**(GameSystem***)(a1 + 24));
    Party = GameData_GetParty(gameData);
    v8 = (int)sub_2034C80(gameData, 0, (int)Party, HEAPID_GAMEEVENT);
    v9 = *(_DWORD*)(a1 + 88);
    *(_DWORD*)(v8 + 44) = *(_DWORD*)(a1 + 84);
    *(_DWORD*)(v8 + 48) = v9;
    v10 = *(_DWORD*)(a1 + 96);
    *(_DWORD*)(v8 + 52) = *(_DWORD*)(a1 + 92);
    *(_DWORD*)(v8 + 56) = v10;
    *(_DWORD*)(v8 + 60) = *(_DWORD*)(a1 + 100);
    *(_DWORD*)(v8 + 76) = *(_DWORD*)(*(_DWORD*)(a1 + 24) + 48);
    *(_WORD*)(v8 + 86) = 0;
    *(_BYTE*)(v8 + 110) = -1;
    *(_WORD*)(v8 + 64) = GameData_GetPlayerState(gameData)->ZoneID;
    *(_BYTE*)(v8 + 165) = SaveControl_IsItemRegistered(gameData, REGIT_POKEMON_LIST);
    if (IsRegistrablePokeListItem((ItemID)itemId)) // This now uses the ExtRegistrableItems list instead of hardcoded values
    {
        *(_DWORD*)(v8 + 76) = 0;
        *(_DWORD*)(v8 + 68) = 5;
        *(_WORD*)(v8 + 84) = itemId;
        *(_DWORD*)(a1 + 116) = 1;
        return v8;
    }
    if (IsEqual(a3, 0))
    {
        v11 = 0;
    LABEL_11:
        *(_DWORD*)(v8 + 76) = v11;
        return v8;
    }
    if (IsEqual(a3, 2))
    {
        *(_WORD*)(v8 + 84) = *(_DWORD*)(a4 + 72);
        v12 = *(_DWORD*)(a4 + 68);
        if (v12 != 3 && v12 != 4 && v12 != 1 && v12 != 18)
            *(_DWORD*)(v8 + 76) = 0;
        v13 = *(_DWORD*)(a4 + 68);
        if (v13 > 0x12)
        {
        LABEL_24:
            v14 = 9;
        }
        else
        {
            switch (v13)
            {
            case 0:
            case 2:
            case 7:
            case 8:
            case 9:
            case 0xA:
            case 0xB:
            case 0xD:
            case 0xE:
            case 0xF:
            case 0x10:
            case 0x11:
                goto LABEL_24;
            case 1:
                if (*(_DWORD*)(a4 + 56) != 2)
                    return v8;
                v14 = 0;
                break;
            case 3:
            case 4:
            case 0x12:
                v14 = 10;
                break;
            case 5:
                v14 = 5;
                break;
            case 6:
                v14 = 6;
                break;
            case 0xC:
                v14 = 16;
                break;
            default:
                goto LABEL_52;
            }
        }
        *(_DWORD*)(v8 + 68) = v14;
    }
    else
    {
        if (!IsEqual(a3, 7))
        {
            if (IsEqual(a3, 10))
            {
                GFL_OvlLoad(215u);
                v16 = ((int(*)(int))0x21A7720)(a4);
                GFL_OvlUnload(215u);
                v17 = *(_DWORD*)(a1 + 112);
                if (v17)
                {
                    if (v17 != 2)
                        return v8;
                    if (v16 != 1)
                    {
                        *(_DWORD*)(v8 + 68) = 0;
                        v11 = *(unsigned __int8*)(a1 + 104);
                        goto LABEL_11;
                    }
                    v18 = 12;
                }
                else
                {
                    v18 = 11;
                }
                *(_DWORD*)(v8 + 68) = v18;
                *(_DWORD*)(v8 + 76) = *(unsigned __int8*)(a1 + 104);
            }
            else
            {
                if (!IsEqual(a3, 11))
                    return v8;
                *(_DWORD*)(v8 + 68) = 5;
            }
        LABEL_52:
            *(_WORD*)(v8 + 84) = *(_WORD*)(a1 + 106);
            return v8;
        }
        if (!*(_BYTE*)(a4 + 18) || *(_BYTE*)(a4 + 18) == 1)
        {
            if (*(_BYTE*)(a4 + 13) != 2)
            {
                *(_DWORD*)(v8 + 68) = 0;
                v11 = *(unsigned __int8*)(a4 + 15);
                goto LABEL_11;
            }
            if (*(_DWORD*)(a1 + 112) == 6)
            {
                *(_DWORD*)(v8 + 68) = 8;
                *(_DWORD*)(v8 + 96) = *(_DWORD*)(a1 + 108);
            }
            else
            {
                *(_DWORD*)(v8 + 68) = 7;
            }
            *(_WORD*)(v8 + 84) = *(_WORD*)(a1 + 106);
            *(_WORD*)(v8 + 86) = *(_WORD*)(a4 + 20);
            if (*(_BYTE*)(a4 + 18))
                v15 = -1;
            else
                v15 = *(_BYTE*)(a4 + 17);
            *(_BYTE*)(v8 + 88) = v15;
        }
    }
    return v8;
}

unsigned int sub_2035970(int a1);
u32 THUMB_BRANCH_GetItemGraphicsDatID(ITEM_ID itemId, unsigned int paletteType)
{
    if (itemId >= IT_ITEM_AMOUNT)
    {
        itemId = 0;
    }

    switch (paletteType)
    {
    case 0:
        if (!itemId)
            return 0;
        break;
    case 1:
        if (itemId)
            itemId = ITEM_GRAPHICS_CHARS_AND_PALETTES[2 * itemId];
        else
            itemId = 1021;
        break;
    case 2:
        if (itemId)
            itemId = ITEM_GRAPHICS_CHARS_AND_PALETTES[2 * itemId + 1];
        else
            itemId = 1022;
        break;
    case 3:
        itemId = ((u16*)0x2090BF8)[2 * sub_2035970(itemId)];
        break;
    case 4:
        itemId = ((u16*)0x2090BFA)[2 * sub_2035970(itemId)];
        break;
    default:
        return 0;
    }
    return itemId;
}
C_DECL_END

// LIMITER FUNCTIONS
/*
// ARM 9

// function not researched (just removed item limiter)
unsigned int THUMB_BRANCH_sub_2009A18(int a1, int a2)
{
    unsigned int result; // r0

    if (a2 > 20)
        return 0;
    result = *(unsigned __int16*)(a1 + 2 * a2 + 228);
    if (result >= IT_ITEM_AMOUNT) // removed limiter
        return 0;
    return result;
};

// function not researched (just removed item limiter)
bool THUMB_BRANCH_sub_2014340(int* a1)
{
    return !*(a1 + 52) || *(a1 + 52) > (IT_ITEM_AMOUNT - 1); // removed limiter
};

// function not researched (just removed item limiter)
bool THUMB_BRANCH_sub_20143A4(int* a1)
{
    return !*(a1 + 54) || *(a1 + 54) > (IT_ITEM_AMOUNT - 1);
}

// function not researched (just removed item limiter) MAY HAVE ERROR
int THUMB_BRANCH_sub_2026C98(unsigned int a1)
{
    if (a1 < IT_ITEM_AMOUNT)
        return ((int*)0x02090AC4)[a1 >> 5] & (1 << (((a1 & 0x80000000) != 0) + ROR((a1 << 27) - (a1 >> 31), 27)));
    else
        return 0;
}

// function not researched (just removed item limiter) MAY HAVE ERROR
int THUMB_BRANCH_sub_2026CC8(unsigned int a1)
{
    if (a1 < IT_ITEM_AMOUNT)
    {
        return ((int*)0x02090A74)[a1 >> 5] & (1 << (((a1 & 0x80000000) != 0) + ROR((a1 << 27) - (a1 >> 31), 27)));
    }
    else
    {
        return 1;
    }
}
*/