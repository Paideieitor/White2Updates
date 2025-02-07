.thumb

#include "settings.h"

#if WEATHER_ABILITY_TURNS != 255

@ This changes the amount of turns weather will last after being summoned by an ability
FULL_COPY_CommonWeatherChangeAbility_0x2A:
    movs    r0, #WEATHER_ABILITY_TURNS
    .size FULL_COPY_CommonWeatherChangeAbility_0x2A, . - FULL_COPY_CommonWeatherChangeAbility_0x2A

#endif // WEATHER_ABILITY_TURNS != 255

#if EXPAND_ABILITIES

@ Remove the if statements that check for Unnerve so we can replace them with 
@ an Unnerve and As One check inside the ServerControl_UnnerveAction function
FULL_COPY_ServerControl_AddCondition_0x132:
    nop
    nop
    .size FULL_COPY_ServerControl_AddCondition_0x132, . - FULL_COPY_ServerControl_AddCondition_0x132

FULL_COPY_ServerDisplay_SkillSwap_0x148:
    nop
    nop
    .size FULL_COPY_ServerDisplay_SkillSwap_0x148, . - FULL_COPY_ServerDisplay_SkillSwap_0x148

FULL_COPY_ServerDisplay_SkillSwap_0x16E:
    nop
    nop
    .size FULL_COPY_ServerDisplay_SkillSwap_0x16E, . - FULL_COPY_ServerDisplay_SkillSwap_0x16E

FULL_COPY_ServerControl_ClearMonDependentEffects_0xB6:
    nop
    nop
    .size FULL_COPY_ServerControl_ClearMonDependentEffects_0xB6, . - FULL_COPY_ServerControl_ClearMonDependentEffects_0xB6

FULL_COPY_BattleHandler_ChangeAbility_0x126:
    nop
    nop
    .size FULL_COPY_BattleHandler_ChangeAbility_0x126, . - FULL_COPY_BattleHandler_ChangeAbility_0x126

#endif // EXPAND_ABILITIES

#if GEN6_CRIT

FULL_COPY_OVL_167_0x21D74E0:
    .byte 16
    .byte 8
    .byte 2
    .byte 1
    .byte 1
    .size FULL_COPY_OVL_167_0x21D74E0, . - FULL_COPY_OVL_167_0x21D74E0

#endif // GEN6_CRIT