#include "include_all.h"

// DONE: speed stat boost mid turn update
// DONE: take into account trick room
// DONE: take into account priority
// DONE: weather abilities and speed abilities in general
// DONE: apply special priorities (lagging tail, prankster, etc)
// DONE: quash and after you work as intended
// DONE: fixed ActionOrder array when a mon faints (fixes the After You bug)
// DONE: make quash and after you trigger because of the battle handler and not the move ID

#define ORDER_ALTERING_MOVE_VAR 16400

C_DECL_BEGIN
enum OrderAlteringMoves
{
    OAM_NONE = 0,
    OAM_AFTER_YOU = 1,
    OAM_QUASH = 2
};
u16 GetOrderAlteringMoveVar()
{
    EventWorkSave* eventWork = GameData_GetEventWork(GAME_DATA);
    u16 order_altering_move = *EventWork_GetWkPtr(eventWork, ORDER_ALTERING_MOVE_VAR);

#ifdef DEBUG_PRINT
    switch (order_altering_move)
    {
    case OAM_AFTER_YOU:
        DPRINT("USED AFTER YOU\n");
        break;
    case OAM_QUASH:
        DPRINT("USED QUASH\n");
        break;
    }
#endif

    return order_altering_move;
}
void SetOrderAlteringMoveVar(u16 order_altering_move_var)
{
    EventWorkSave* eventWork = GameData_GetEventWork(GAME_DATA);
    u16* order_altering_move = EventWork_GetWkPtr(eventWork, ORDER_ALTERING_MOVE_VAR);

    *order_altering_move = order_altering_move_var;
}

int THUMB_BRANCH_BattleHandler_InterruptAction(ServerFlow* a1, HandlerParam_InterruptPoke* a2)
{
    if (!ActionOrder_InterruptReserve(a1, a2->pokeID))
    {
        return 0;
    }
    BattleHandler_SetString(a1, &a2->exStr);

    SetOrderAlteringMoveVar(OAM_AFTER_YOU);
    return 1;
}

int THUMB_BRANCH_BattleHandler_SendLast(ServerFlow* a1, HandlerParam_SendLast* a2)
{
    if (!ActionOrder_SendToLast(a1, a2->pokeID))
    {
        return 0;
    }
    BattleHandler_SetString(a1, &a2->exStr);

    SetOrderAlteringMoveVar(OAM_QUASH);
    return 1;
}

void SwapPokemonOrder(ActionOrderWork* action_order, u32* speed_stats, u32* priority, u16 slow_idx, u16 fast_idx)
{
    BattleMon* buffer_mon = action_order[fast_idx].battleMon;
    BattleActionParam* buffer_action = action_order[fast_idx].Action;
    u32 buffer_field_8 = action_order[fast_idx].field_8;
    char buffer_field_C = action_order[fast_idx].field_C;
    char buffer_fDone = action_order[fast_idx].fDone;
    char buffer_field_E = action_order[fast_idx].field_E;
    char buffer_field_F = action_order[fast_idx].field_F;
    u32 buffer_speed = speed_stats[fast_idx];
    u32 buffer_priority = priority[fast_idx];

    action_order[fast_idx].battleMon = action_order[slow_idx].battleMon;
    action_order[slow_idx].battleMon = buffer_mon;

    action_order[fast_idx].Action = action_order[slow_idx].Action;
    action_order[slow_idx].Action = buffer_action;

    action_order[fast_idx].field_8 = action_order[slow_idx].field_8;
    action_order[slow_idx].field_8 = buffer_field_8;

    action_order[fast_idx].field_C = action_order[slow_idx].field_C;
    action_order[slow_idx].field_C = buffer_field_C;

    action_order[fast_idx].fDone = action_order[slow_idx].fDone;
    action_order[slow_idx].fDone = buffer_fDone;

    action_order[fast_idx].field_E = action_order[slow_idx].field_E;
    action_order[slow_idx].field_E = buffer_field_E;

    action_order[fast_idx].field_F = action_order[slow_idx].field_F;
    action_order[slow_idx].field_F = buffer_field_F;

    speed_stats[fast_idx] = speed_stats[slow_idx];
    speed_stats[slow_idx] = buffer_speed;

    priority[fast_idx] = priority[slow_idx];
    priority[slow_idx] = buffer_priority;
}

// Make all pokemos faster or tied with the speed tie loser 1 speed point faster
void BreakSpeedTie(ActionOrderWork* action_order, u32* speed_stats, u32 loser_idx)
{
    for (u32 i = 0; i < 6; ++i)
        if (action_order[i].battleMon->ID != action_order[loser_idx].battleMon->ID)
            if (speed_stats[i] >= speed_stats[loser_idx])
                speed_stats[i] = speed_stats[i] + 1;
}

u32 Dynamic_PokeSet_SortBySpeed(ServerFlow* server_flow, ActionOrderWork* action_order, u32 first_idx, u8 current_idx, short* quash_order, bool turn_start)
{
    u32 poke_amount = server_flow->numActOrder - first_idx; // first_idx is used for "half" turns. Ex. when a mon faints the turn ends regardles of the mons left to move,
    if (poke_amount > 1)                                    // so to keep going, the next "turn" starts with the mon that should have moved next
    {
        u16 fainted_amount = 0;

        u32 isTrickRoom = BattleField_CheckEffect(EFFECT_TRICK_ROOM);
        u32 speed_stats[6];
        u32 priority[6];
        for (u32 i = first_idx; i < first_idx + poke_amount; ++i)
        {
            if (!BattleMon_IsFainted(action_order[i].battleMon))
            {
                speed_stats[i] = ServerEvent_CalculateSpeed(server_flow, action_order[i].battleMon, isTrickRoom);

                priority[i] = (action_order[i].field_8 >> 16) & 0x3FFFFF;
                int special_priority = ((action_order[i].field_8 >> 13) & 0x7); // special priority takes into account item & ability prio boosts (1 = no added prio)
                priority[i] += (special_priority - 1);
            }
            else
            {
                priority[i] = 0xFFFFFFFF;
                ++fainted_amount;
            }
        }

        u32 start_idx = current_idx + 1; // Skip the mons that have already moved
        u16 changes = 1;
        while (changes != 0)
        {
            changes = 0;
            for (u16 i = first_idx + start_idx; i < first_idx + poke_amount; ++i)
            {
                if (priority[i] != 0xFFFFFFFF) // check if the first mon is fainted
                {
                    if (i + 1 < first_idx + poke_amount)
                    {
                        for (u16 j = i + 1; j < first_idx + poke_amount; ++j)
                        {
                            if (priority[j] != 0xFFFFFFFF) // check if the second mon is fainted
                            {
                                u32 poke1_priority = priority[i];
                                u32 poke2_priority = priority[j];

                                if (poke2_priority > poke1_priority)
                                {
                                    SwapPokemonOrder(action_order, speed_stats, priority, i, j);
                                    ++changes;
                                }
                                else if (poke2_priority == poke1_priority)
                                {
                                    u32 poke1_speed = speed_stats[i];
                                    u32 poke2_speed = speed_stats[j];
                                    if (poke2_speed > poke1_speed || poke2_speed == poke1_speed && BattleRandom(2u))
                                    {
                                        SwapPokemonOrder(action_order, speed_stats, priority, i, j);
                                        ++changes;

                                        if (poke2_speed == poke1_speed) // completly break speed tie to avoid an infinite loop
                                            BreakSpeedTie(action_order, speed_stats, i);
                                    }
                                    if (speed_stats[j] == speed_stats[i]) // also break speed tie if the second lost
                                        BreakSpeedTie(action_order, speed_stats, j);
                                }
                            }
                        }
                    }
                }
            }
        }

        u16 quash_count = 0;
        for (u16 i = 0; i < 6; ++i)
        {
            if (quash_order[i] < 0)
                break;

            for (u16 j = first_idx + start_idx; j < first_idx + poke_amount - 1; ++j) // loop from the "current" to the one before the last one, the last can't be slower :(
                if (action_order[j].battleMon->ID == quash_order[i])
                    SwapPokemonOrder(action_order, speed_stats, priority, j, j + 1);

            ++quash_count;
        }

        if (turn_start || quash_count) // push fainted mons to the end of the array at the start of the turn
            for (u16 i = 0; i < fainted_amount; ++i) // I only rearrange this when quash is used because it looks better in the debug message, not really necesary
                for (u32 j = first_idx; j < first_idx + poke_amount - 1; ++j)
                    if (BattleMon_IsFainted(action_order[j].battleMon))
                        SwapPokemonOrder(action_order, speed_stats, priority, j, j + 1);

        DPRINT("DYNAMIC SPEED:\n");
        for (u32 i = first_idx; i < first_idx + poke_amount; ++i)
        {
            if (!BattleMon_IsFainted(action_order[i].battleMon))
            {
                DPRINTF("[ ID: %d | SPECIES: %d | PRIORITY: %d | SPEED: %d ]\n", action_order[i].battleMon->ID, action_order[i].battleMon->Species, priority[i], speed_stats[i]);
            }
            else
            {
                DPRINTF("[ ID:% d | SPECIES : % d | IS FAINTED ]\n", action_order[i].battleMon->ID, action_order[i].battleMon->Species);
            }
        }
        DPRINT("---\n");
    }

    return poke_amount;
}

unsigned int THUMB_BRANCH_ServerFlow_ActOrderProcMain(ServerFlow* a1, unsigned int a2)
{
    unsigned int current_idx; // r4
    ActionOrderWork* action_order_work; // r7
    int v5; // r6
    int Action; // r0
    BattleMon* PokeParam; // r0
    bool v8; // r6
    BattleMon* v9; // r0
    int flowResult; // r0
    BattleMon* v12; // r0
    int v13; // r4
    int Count_1; // r4
    int v15; // [sp+0h] [bp-28h]
    int v16; // [sp+4h] [bp-24h]
    BattleActionParam* v17; // [sp+8h] [bp-20h]
    char* p_numActOrder; // [sp+Ch] [bp-1Ch]

    v15 = 0;
    current_idx = a2;
    if (a2 >= (unsigned __int8)a1->numActOrder)
    {
    LABEL_14:
        if (!a1->flowResult)
        {
            v13 = (unsigned __int8)ServerControl_TurnCheck(a1);
            if (ServerControl_CheckMatchup(a1))
            {
                a1->flowResult = 4;
                return (unsigned __int8)a1->numActOrder;
            }
            if (v13)
            {
                a1->flowResult = 3;
                return (unsigned __int8)a1->numActOrder;
            }
            Count_1 = j_j_DeadRec_GetCount_1((int)&a1->faintRecord, 0);
            if (IsPosOpenForRevivedMon(a1) || Count_1)
            {
                ServerFlow_ReqChangePokeForServer(a1, (unsigned char*)&a1->field_4CE);
                ServerDisplay_IllusionSet(a1, (unsigned __int8*)&a1->field_4CE);
                a1->flowResult = 2;

                return (unsigned __int8)a1->numActOrder;
            }
            a1->flowResult = 0;
        }
        return (unsigned __int8)a1->numActOrder;
    }
    else
    {
        action_order_work = a1->actionOrderWork;
        p_numActOrder = &a1->numActOrder;

        short quash_order[6];
        for (u16 i = 0; i < 6; ++i)
            quash_order[i] = -1;

        Dynamic_PokeSet_SortBySpeed(a1, action_order_work, a2, current_idx, quash_order, true); // pre-calc speed to take into account switch-in speed modifiers (weather...)

        while (1)
        {
            v5 = 4 * current_idx;
            BattleMon* battle_mon = action_order_work[current_idx].battleMon;
            if (!BattleMon_IsFainted(battle_mon))
            {
                BattleActionParam* battle_action = action_order_work[current_idx].Action;

                Action = BattleAction_GetAction(battle_action);
                if (v15 == 6 && Action != 6)
                {
                    ServerControl_CheckActivation(a1);
                    sub_219FB7C(a1, (int)&action_order_work[current_idx], (unsigned __int8)*p_numActOrder - current_idx);
                }
                v15 = ActionOrder_Proc(a1, (int)&action_order_work[current_idx]);
                
                u16 order_altering_move = GetOrderAlteringMoveVar();
                if (order_altering_move == OAM_QUASH) // Quash: this move overrides the last pokemon on the list, so we save it to always keep it there
                {
                    u16 free_slot = 0;
                    for (free_slot; free_slot < 6; ++free_slot)
                        if (quash_order[free_slot] == -1)
                            break;

                    BattleMon* last_mon = action_order_work[*p_numActOrder - 1].battleMon;
                    if (!BattleMon_IsFainted(last_mon))
                    {
                        quash_order[free_slot] = last_mon->ID;
                    }
                }

                if (order_altering_move != OAM_AFTER_YOU) // After You: this move overrides the next pokemon to move, we don't need to calc speed this iteration
                    Dynamic_PokeSet_SortBySpeed(a1, action_order_work, a2, current_idx, quash_order, false); // re-calc speed
                else
                {
                    DPRINT("AFTER YOU ORDER:\n");
                    for (u32 i = 0; i < *p_numActOrder; ++i)
                    {
                        if (!BattleMon_IsFainted(action_order_work[i].battleMon))
                        {
                            DPRINTF("[ ID: %d | SPECIES: %d ]\n", action_order_work[i].battleMon->ID, action_order_work[i].battleMon->Species);
                        }
                        else
                        {
                            DPRINTF("[ ID:% d | SPECIES : % d | IS FAINTED ]\n", action_order_work[i].battleMon->ID, action_order_work[i].battleMon->Species);
                        }
                    }
                    DPRINT("---\n");
                }

                SetOrderAlteringMoveVar(0);

                v16 = (unsigned __int8)ServerControl_CheckExpGet(a1);
                PokeParam = PokeCon_GetPokeParam(a1->pokeCon, 0);
                CheckHPDebug(PokeParam);
                v8 = ServerControl_CheckMatchup(a1);
                v9 = PokeCon_GetPokeParam(a1->pokeCon, 0);
                CheckHPDebug(v9);
            }

            if (v8)
            {
                a1->flowResult = 4;
                return current_idx + 1;
            }
            flowResult = a1->flowResult;
            if (flowResult == 6 || flowResult == 1)
            {
                return current_idx + 1;
            }
            if (v16)
            {
                break;
            }
            if (++current_idx >= (unsigned __int8)*p_numActOrder)
            {
                goto LABEL_14;
            }
        }
        v12 = PokeCon_GetPokeParam(a1->pokeCon, 0);
        CheckHPDebug(v12);
        a1->flowResult = 3;
        return current_idx + 1;
    }
}
C_DECL_END