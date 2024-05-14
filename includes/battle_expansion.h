#ifndef __BATTLE_EXPANSION_H
#define __BATTLE_EXPANSION_H

#include "custom/include_all.h"

#define GIVE_EVS false

#define ORDER_ALTERING_MOVE_VAR 16400

#define INTIMIDATE_FLAG 500

#define SET_UP_NEW_EVENT BattleEventVar_SetConstValue(VAR_MON_ID, -1); BattleEventVar_SetConstValue(VAR_ATTACKING_MON, -1); BattleEventVar_SetConstValue(VAR_DEFENDING_MON, -1)
#define IS_NOT_NEW_EVENT BattleEventVar_GetValue(VAR_MON_ID) != -1 || BattleEventVar_GetValue(VAR_ATTACKING_MON) != -1 || BattleEventVar_GetValue(VAR_DEFENDING_MON) != -1
// NEW EVENTS
#define EVENT_ACCURACY_MISS EVENT_MOVE_ACCURACY_STAGE
#define EVENT_ABILITY_CHANGE_CHECK_FAIL EVENT_BEFORE_ABILITY_CHANGE
	#define NEW_VAR_MON_ID VAR_7
	#define NEW_VAR_ATTACKING_MON VAR_7
	#define NEW_VAR_DEFENDING_MON VAR_9

enum MoveCategory
{
	CATEGORY_STATUS = 0,
	CATEGORY_PHYSICAL = 1,
	CATEGORY_SPECIAL = 2,
};

enum Weather
{
	WEATHER_NULL = 0,
	WEATHER_SUN = 1,
	WEATHER_RAIN = 2,
	WEATHER_HAIL = 3,
	WEATHER_SANDSTORM = 4,
};

#endif
