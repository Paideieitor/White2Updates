#ifndef __FEATURE_SETTINGS_H
#define __FEATURE_SETTINGS_H	

#define ADD_LEVEL_CAP true
#define GIVE_EVS false
#define RARE_CANDY_IGNORE_LVL_CAP true

#define ADD_INFINITE_RARE_CANDY true
#define INFINITE_CANDY_ID IT0622
#define REGIT_INFINITE_CANDY REGIT_UNUSED_34

#define ADD_INFINITE_REPEL true
#define REPEL_SHAKING_GRASS true
#define INFINITE_REPEL_ID IT0079_REPEL
#define REGIT_INFINITE_REPEL REGIT_UNUSED_35
#define ACTID_REPEL 254

#define ADD_DYNAMIC_SPEED true

#define ADD_CRIT_CHANGES true
#define CRIT_DMG_BOOST 1.5f

#define ADD_NEW_ITEMS true
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
#define IT_ITEM_AMOUNT              703 
// IT_ITEM_AMOUNT must be biggest number
// some item IDs might crash the game (like 654 - 664 & 692 - 695)
// more than 1023 item ID will have errors with battle text

// FLAGS
#define INTIMIDATE_FLAG 500

// VARIABLES
#define LVL_CAP_VAR 16415
#define ORDER_ALTERING_MOVE_VAR 16400

// MESSAGE IDs
// Game Text - File 6
#define BAG_ITEMUSED_MSGID 63
#define BAG_REPELEND_MSGID 64
// Game Text - File 18
#define BATTLE_ASSAULTVEST_MSGID 878
#define BARRLE_CLEARAMULET_MSGID 879
// Game Text - File 178
#define POKELIST_NATURECHANGE_MSGID 73
#define POKELIST_ABILITYCHANGE_MSGID 74
#define POKELIST_IVMODALL_MSGID 75
#define POKELIST_IVMODSINGLE_MSGID 76

#endif
