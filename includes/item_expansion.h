#ifndef __ITEM_EXPANSION_H
#define __ITEM_EXPANSION_H

#include "custom/include_all.h"

typedef int ITEM_ID; // enum ItemID

#define DEFAULT_LAST_ITEM IT0638_REVEAL_GLASS

#define IT0639_WEAKNESS_POLICY      639
#define IT0640_ASSAULT_VEST         640
#define IT0644_PIXIE_PLATE          641
#define IT0645_ABILITY_CAPSULE      642
#define IT0648_LUMINOUS_MOSS        643
#define IT0649_SNOWBALL             644
#define IT0650_SAFETY_GOGGLES       645
#define IT0686_ROSELI_BERRY         646
#define IT0687_KEE_BERRY            647
#define IT0688_MARANGA_BERRY        648
#define IT0795_BOTTLE_CAP           649
#define IT0796_GOLD_BOTTLE_CAP      650
#define IT0846_ADRENALINE_ORB       651
#define IT0880_PROTECTIVE_PADS      652
#define IT1118_THROAT_SPRAY         653
// 654 - 664 crash the game (needs research)
#define IT1119_EJECT_PACK           665
#define IT1120_HEAVY_DUTY_BOOTS     666
#define IT1121_BLUNDER_POLICY       667
#define IT1122_ROOM_SERVICE         668
#define IT1123_UTILITY_UMBRELLA     669
#define IT1231_LONELY_MINT          670
#define IT1232_ADAMANT_MINT         671
#define IT1233_NAUGHTY_MINT         672
#define IT1234_BRAVE_MINT           673
#define IT1235_BOLD_MINT            674
#define IT1236_IMPISH_MINT          675
#define IT1237_LAX_MINT             676
#define IT1238_RELAXED_MINT         677
#define IT1239_MODEST_MINT          678
#define IT1240_MILD_MINT            679
#define IT1241_RASH_MINT            680
#define IT1242_QUIET_MINT           681
#define IT1243_CALM_MINT            682
#define IT1244_GENTLE_MINT          683
#define IT1245_CAREFUL_MINT         684
#define IT1246_SASSY_MINT           685
#define IT1247_TIMID_MINT           686
#define IT1248_HASTY_MINT           687
#define IT1249_JOLLY_MINT           688
#define IT1250_NAIVE_MINT           689
#define IT1251_SERIOUS_MINT         690
#define IT1606_ABILITY_PATCH        691
// 692 - 695 crash the game (needs research)
#define IT1881_ABILITY_SHIELD       696
#define IT1882_CLEAR_AMULET         697
#define IT1883_MIRROR_HERB          698
#define IT1884_PUNCHING_GLOVE       699
#define IT1885_COVERT_CLOAK         700
#define IT1886_LOADED_DICE          701
#define IT2401_FAIRY_FEATHER        702
#define IT_ITEM_AMOUNT              703 // this must be last

#define INFINITE_CANDY_ID IT0622/*GOD STONE*/
#define REGIT_INFINITE_CANDY REGIT_UNUSED_34
#define RARE_CANDY_IGNORE_LVL_CAP true

#define INFINITE_REPEL_ID IT0079_REPEL
#define REGIT_INFINITE_REPEL REGIT_UNUSED_35
#define REPEL_SHAKING_GRASS true
#define ACTID_REPEL 254

#define ITSTAT_NEW_EFFECT_TYPE 63
#define ITSTAT_NEW_EFFECT 64
#define MAKE_NEW_EFFECT(type, effect) (0x1 | (type << 1) | (effect << 3))

#define RESTORETYPE_NATURE 30
#define RESTORETYPE_ABILITY 31
#define RESTORETYPE_IV_MOD 32

#define REG_ITEM(regid, itemid) (regid << 10) | itemid

// Pokemon related effects for new items
enum NewItemPokemonEffects
{
    ITEFF_NONE = 0,
    ITEFF_NATURE = 1,
    ITEFF_ABILITY = 2,
    ITEFF_IV_MOD = 3,
};

enum PokemonNature
{
    NAT_HARDY = 0,
    NAT_LONELY = 1,
    NAT_BRAVE = 2,
    NAT_ADAMANT = 3,
    NAT_NAUGHTY = 4,
    NAT_BOLD = 5,
    NAT_DOCILE = 6,
    NAT_RELAXED = 7,
    NAT_IMPISH = 8,
    NAT_LAX = 9,
    NAT_TIMID = 10,
    NAT_HASTY = 11,
    NAT_SERIOUS = 12,
    NAT_JOLLY = 13,
    NAT_NAIVE = 14,
    NAT_MODEST = 15,
    NAT_MILD = 16,
    NAT_QUIET = 17,
    NAT_BASHFUL = 18,
    NAT_RASH = 19,
    NAT_CALM = 20,
    NAT_GENTLE = 21,
    NAT_SASSY = 22,
    NAT_CAREFUL = 23,
    NAT_QUIRKY = 24,
};

enum AbilityChangeBehaviour
{
    ABILITY_1 = 0,
    ABILITY_2 = 1,
    ABILITY_HIDDEN = 2,
    ABILITY_CAPSULE = 3,
    ABILITY_PATCH = 4,
};

enum PokemonIVModifier
{
    MAX_IV = 0,
    MAX_ALL = 1,
    MAX_ATK = 2,
    MAX_DEF = 3,
    MAX_SPE = 4,
    MAX_SPA = 5,
    MAX_SPD = 6,
    MIN_IV = 7,
    MIN_ALL = 8,
    MIN_ATK = 9,
    MIN_DEF = 10,
    MIN_SPE = 11,
    MIN_SPA = 12,
    MIN_SPD = 13,
};

#endif
