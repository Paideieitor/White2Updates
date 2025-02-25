#ifndef __MOVES_H
#define __MOVES_H

#include "defs.h"

struct BattleMon;
struct BattleEventItem;
struct ServerFlow;

enum MoveCategory
{
    CATEGORY_STATUS = 0,
    CATEGORY_PHYSICAL = 1,
    CATEGORY_SPECIAL = 2,
};

typedef u32 MOVE_ID;
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

enum SecretPowerEffect
{
    SCTPOWEFF_ADD_COND = 0,
    SCTPOWEFF_LOWER_STAT = 1,
    SCTPOWEFF_FLINCH = 2,
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

extern "C" u8 PML_MoveGetType(MOVE_ID moveID);
extern "C" MoveCategory PML_MoveGetCategory(MOVE_ID moveID);
extern "C" u32 PML_MoveGetMaxPP(MOVE_ID moveID, u32 ppUpStage);
extern "C" u32 PML_MoveGetParam(MOVE_ID moveID, MoveField field);

extern "C" b32 Move_IsUsable(BattleMon* battleMon, MOVE_ID moveID);
extern "C" b32 IsComboMove(MOVE_ID moveID);

extern "C" b32 getMoveFlag(MOVE_ID moveID, MoveFlag moveFlag);

extern "C" void HandlerThiefStart(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work);

extern "C" void MoveEvent_ForceRemoveItemFromBattleMon(BattleMon* battleMon, MOVE_ID moveID);

#endif // __MOVES_H
