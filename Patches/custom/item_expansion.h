#ifndef __ITEM_EXPANSION_H
#define __ITEM_EXPANSION_H

typedef int ITEM_ID; // enum ItemID

#define DEFAULT_LAST_ITEM IT0638_REVEAL_GLASS

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
