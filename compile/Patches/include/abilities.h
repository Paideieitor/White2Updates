#ifndef __ABILITIES_H
#define __ABILITIES_H

#include "defs.h"

typedef u32 ABILITY;
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
};

typedef BattleEventHandlerTableEntry* (*AbilityEventAddFunc)(u32*);

struct BattleMon;

struct AbilityEventAddTable
{
	// AbilID
	ABILITY ability;
	AbilityEventAddFunc func;
};

extern "C" BattleEventHandlerTableEntry* EventAddIntimidate(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddClearBody(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddInnerFocus(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSteadfast(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddThickFat(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddHyperCutter(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddHugePower(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddTintedLens(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSpeedBoost(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddBlaze(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddTorrent(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddOvergrow(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSwarm(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddGuts(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSkillLink(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddKeenEye(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSimple(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSolidRock(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMarvelScale(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddRivalry(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddLeafGuard(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddDrizzle(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddDrought(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSandStream(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSnowWarning(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddAirLock(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddTechnician(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddOblivious(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddHydration(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddPoisonHeal(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddIceBody(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddRainDish(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddShieldDust(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddAdaptability(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSereneGrace(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSolarPower(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSwiftSwim(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddChlorophyll(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddShedSkin(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddTangledFeet(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddQuickFeet(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddHustle(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddBattleArmor(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSuperLuck(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddAngerPoint(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSniper(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddIronFist(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddCompoundEyes(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddRockHead(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddReckless(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddStatic(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddPoisonPoint(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddFlameBody(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddEffectSpore(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddPlusMinus(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddCuteCharm(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSandVeil(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSnowCloak(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddTrace(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddNormalize(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddRoughSkin(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddNaturalCure(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSynchronize(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddDownload(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSturdy(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddHeatproof(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddUnaware(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddDrySkin(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddVoltAbsorb(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddWaterAbsorb(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMotorDrive(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddLimber(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddInsomnia(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddOwnTempo(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMagmaArmor(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddWaterVeil(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddImmunity(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddScrappy(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSoundproof(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddLevitate(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddFlowerGift(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddFlashFire(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddForewarn(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddAnticipation(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddFrisk(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddAftermath(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddRunAway(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddColorChange(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMoldBreaker(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddTruant(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddLightningRod(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddStormDrain(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSlowStart(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddDamp(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddWonderGuard(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddStall(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddForecast(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSuctionCups(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddLiquidOoze(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddKlutz(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddStickyHold(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddPressure(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMagicGuard(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddBadDreams(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddPickup(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddUnburden(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddStench(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddShadowTag(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddArenaTrap(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMagnetPull(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddPickpocket(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSheerForce(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddDefiant(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddDefeatist(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMultiscale(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddHeavyMetal(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddLightMetal(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddContrary(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddUnnerve(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddCursedBody(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddHealer(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddFriendGuard(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddWeakArmor(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddToxicBoost(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddFlareBoost(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddHarvest(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddTelepathy(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMoody(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddOvercoat(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddPoisonTouch(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddRegenerator(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddBigPecks(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSandRush(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddWonderSkin(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddAnalytic(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddIllusion(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddImposter(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddInfiltrator(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMummy(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMoxie(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddJustified(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddRattled(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMagicBounce(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSapSipper(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddPrankster(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSandForce(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddRoughSkin(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddZenMode(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddVictoryStar(u32* handlerAmount);

extern "C" u32 AbilityEvent_GetSubPriority(BattleMon* battleMon);
extern "C" BattleEventPriority GetHandlerMainPriority(u32* handlerAmount);

extern "C" b32 j_j_IsRolePlayFailAbility(ABILITY ability);
extern "C" b32 j_CheckSkillSwapFailAbility(ABILITY ability);

#endif // __ABILITIES_H
