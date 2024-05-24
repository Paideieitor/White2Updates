#include "custom/settings.h"
#include "custom/item_expansion.h"

#include "custom/level_cap.h"

C_DECL_BEGIN
// --- DATA ---

// NEW

ITEM_ID InfiniteBattleItems[] = {
#if ADD_INFINITE_RARE_CANDY
    INFINITE_CANDY_ID,
#endif
    IT_ITEM_AMOUNT
};


// --- FUNCTIONS ---

// NEW

bool IsBattleItemInfinite(ItemID itemID)
{
    for (u16 i = 0; i < ARRAY_COUNT(InfiniteBattleItems); ++i)
        if (itemID == InfiniteBattleItems[i])
            return true;
    return false;
}

void LoadPokemonNatureToStrbuf(WordSetSystem* a1, int strbufidx, PartyPkm* pkm, PokemonNature nature)
{
    switch (nature)
    {
    case NAT_HARDY:
    {
        short wStr[6];
        GetWideString(wStr, 6, "Hardy");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_LONELY:
    {
        short wStr[7];
        GetWideString(wStr, 7, "Lonely");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_BRAVE:
    {
        short wStr[6];
        GetWideString(wStr, 6, "Brave");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_ADAMANT:
    {
        short wStr[8];
        GetWideString(wStr, 8, "Adamant");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_NAUGHTY:
    {
        short wStr[8];
        GetWideString(wStr, 8, "Naughty");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_BOLD:
    {
        short wStr[5];
        GetWideString(wStr, 5, "Bold");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_DOCILE:
    {
        short wStr[7];
        GetWideString(wStr, 7, "Docile");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_RELAXED:
    {
        short wStr[8];
        GetWideString(wStr, 8, "Relaxed");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_IMPISH:
    {
        short wStr[7];
        GetWideString(wStr, 7, "Impish");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_LAX:
    {
        short wStr[4];
        GetWideString(wStr, 4, "Lax");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_TIMID:
    {
        short wStr[6];
        GetWideString(wStr, 6, "Timid");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_HASTY:
    {
        short wStr[6];
        GetWideString(wStr, 6, "Hasty");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_SERIOUS:
    {
        short wStr[8];
        GetWideString(wStr, 8, "Serious");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_JOLLY:
    {
        short wStr[5];
        GetWideString(wStr, 5, "Jolly");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_NAIVE:
    {
        short wStr[6];
        GetWideString(wStr, 6, "Naive");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_MODEST:
    {
        short wStr[7];
        GetWideString(wStr, 7, "Modest");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_MILD:
    {
        short wStr[5];
        GetWideString(wStr, 5, "Mild");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_QUIET:
    {
        short wStr[6];
        GetWideString(wStr, 6, "Quiet");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_BASHFUL:
    {
        short wStr[8];
        GetWideString(wStr, 8, "Bashful");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_RASH:
    {
        short wStr[5];
        GetWideString(wStr, 5, "Rash");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_CALM:
    {
        short wStr[5];
        GetWideString(wStr, 5, "Calm");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_GENTLE:
    {
        short wStr[7];
        GetWideString(wStr, 7, "Gentle");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_SASSY:
    {
        short wStr[6];
        GetWideString(wStr, 6, "Sassy");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_CAREFUL:
    {
        short wStr[8];
        GetWideString(wStr, 8, "Careful");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    case NAT_QUIRKY:
    {
        short wStr[7];
        GetWideString(wStr, 7, "Quirky");
        GFL_StrBufLoadString(a1->MainStrbuf, wStr);
    }
        break;
    }
    GFL_WordSetCopyStrbuf(a1, strbufidx, a1->MainStrbuf, 0);
}
void PrintNatureChangeMessage(PokeList* pokeList, unsigned int itemID)
{
    ItemData* itemData = (ItemData*)PML_ItemReadDataFile(itemID, 0, HEAPID_POKELIST);
    PokemonNature nature = (PokemonNature)PML_ItemGetParam(itemData, (ItemField)ITSTAT_NEW_EFFECT);
    GFL_HeapFree(itemData);

    PokeListMessage_CreateWordSetSystem(pokeList, pokeList->pokeList_Message);
    PokeListMessage_LoadPokemonNicknameToStrBuf(pokeList, pokeList->pokeList_Message, 0, pokeList->SelectedPkm);
    LoadPokemonNatureToStrbuf(pokeList->pokeList_Message->wordSetSystem, 1, pokeList->SelectedPkm, nature);
    PokeList_PrintMessage(pokeList, POKELIST_NATURECHANGE_MSGID, 1, (int)sub_219DE58);
    PokeListMessage_ClearWordSetSystem(pokeList, pokeList->pokeList_Message);
}
bool CanNatureChangeItemBeUsed(PartyPkm* pokemon, ItemData* itemData)
{
    PokemonNature itemNature = (PokemonNature)PML_ItemGetParam(itemData, (ItemField)ITSTAT_NEW_EFFECT);
    PokemonNature pokeNature = (PokemonNature)PokeParty_GetParam(pokemon, PF_Nature, nullptr);
    if (itemNature != pokeNature)
        return true;
    return false;
}
void UseNatureChangeItem(PartyPkm* pokemon, ItemData* itemData)
{
    PokemonNature nature = (PokemonNature)PML_ItemGetParam(itemData, (ItemField)ITSTAT_NEW_EFFECT);
    PokeParty_SetParam(pokemon, PF_Nature, nature);

    PokeParty_RecalcStats(pokemon);
}

u32 GetCurrentAbilitySlot(PersonalData* personal, AbilID currentAbility)
{
    for (u16 i = 0; i < 3; ++i)
    {
        if (currentAbility == (AbilID)PML_PersonalGetParam(personal, (PersonalField)(Personal_Abil1 + i)))
        {
            return i;
        }
    }
    return 0;
}
void PrintAbilityChangeMessage(PokeList* pokeList, unsigned int itemID)
{
    ItemData* itemData = (ItemData*)PML_ItemReadDataFile(itemID, 0, HEAPID_POKELIST);
    AbilityChangeBehaviour behaviour = (AbilityChangeBehaviour)PML_ItemGetParam(itemData, (ItemField)ITSTAT_NEW_EFFECT);
    GFL_HeapFree(itemData);
    
    AbilID ability = (AbilID)PokeParty_GetParam(pokeList->SelectedPkm, PF_Ability, nullptr);

    PersonalData* personal = PML_PersonalLoadBW2(PokeParty_GetParam(pokeList->SelectedPkm, PF_Species, nullptr), PokeParty_GetParam(pokeList->SelectedPkm, PF_Forme, nullptr));
    u32 currentAbilitySlot = GetCurrentAbilitySlot(personal, ability);

    u32 changedAbilitySlot = 0;
    switch (behaviour)
    {
    case ABILITY_1:
    case ABILITY_2:
    case ABILITY_HIDDEN:
        changedAbilitySlot = Personal_Abil1 + behaviour;
        break;
    case ABILITY_CAPSULE:
        currentAbilitySlot ? changedAbilitySlot = 0 : changedAbilitySlot = 1;
        break;
    case ABILITY_PATCH:
        if (ability == PML_PersonalGetParam(personal, Personal_AbilH))
        {
            changedAbilitySlot = 1;
        }
        else
        {
            changedAbilitySlot = 2;
        }
        break;
    }

    ability = (AbilID)PML_PersonalGetParam(personal, (PersonalField)(Personal_Abil1 + changedAbilitySlot));

    PokeListMessage_CreateWordSetSystem(pokeList, pokeList->pokeList_Message);
    PokeListMessage_LoadPokemonNicknameToStrBuf(pokeList, pokeList->pokeList_Message, 0, pokeList->SelectedPkm);

    loadAbilityNameToStrbuf(pokeList->pokeList_Message->wordSetSystem, 1, ability);

    PokeList_PrintMessage(pokeList, POKELIST_ABILITYCHANGE_MSGID, 1, (int)sub_219DE58);
    PokeListMessage_ClearWordSetSystem(pokeList, pokeList->pokeList_Message);
}
bool CanAbilityChangeItemBeUsed(PartyPkm* pokemon, ItemData* itemData) 
{
    AbilityChangeBehaviour behaviour = (AbilityChangeBehaviour)PML_ItemGetParam(itemData, (ItemField)ITSTAT_NEW_EFFECT);

    AbilID ability = (AbilID)PokeParty_GetParam(pokemon, PF_Ability, nullptr);
    AbilID changedAbility = ability;

    PersonalData* personal = PML_PersonalLoadBW2(PokeParty_GetParam(pokemon, PF_Species, nullptr), PokeParty_GetParam(pokemon, PF_Forme, nullptr));
    u32 currentAbilitySlot = GetCurrentAbilitySlot(personal, ability);

    u32 changedAbilitySlot = 0;
    switch (behaviour)
    {
    case ABILITY_1:
        changedAbilitySlot = 0;
        goto SAME_ABILITY_CHECK;
    case ABILITY_2:
        changedAbilitySlot = 1;
        if (PML_PersonalGetParam(personal, Personal_Abil2) == ABIL_NULL)
        {
            break;
        }
        goto SAME_ABILITY_CHECK;
    case ABILITY_HIDDEN:
        changedAbilitySlot = 2;
        if (PML_PersonalGetParam(personal, Personal_AbilH) == ABIL_NULL)
        {
            break;
        }
        goto SAME_ABILITY_CHECK;
    case ABILITY_CAPSULE: // ability capsule can't change from hidden ability to regular ability
        if (PML_PersonalGetParam(personal, Personal_Abil2) == ABIL_NULL)
        {
            break;
        }
        if (ability == PML_PersonalGetParam(personal, Personal_AbilH))
        {
            break;
        }

        currentAbilitySlot ? changedAbilitySlot = 0 : changedAbilitySlot = 1;
        goto SAME_ABILITY_CHECK;
    case ABILITY_PATCH:
        if (PML_PersonalGetParam(personal, Personal_AbilH) == ABIL_NULL)
        {
            break;
        }

        if (ability == PML_PersonalGetParam(personal, Personal_AbilH))
        {
            changedAbilitySlot = 1;
        }
        else
        {
            changedAbilitySlot = 2;
        }

    SAME_ABILITY_CHECK:

        changedAbility = (AbilID)PML_PersonalGetParam(personal, (PersonalField)(Personal_Abil1 + changedAbilitySlot));
        if (ability != changedAbility)
        {
            return true;
        }
        break;
    }
    return false;
}
void UseAbilityChangeItem(PartyPkm* pokemon, ItemData* itemData) 
{
    AbilityChangeBehaviour behaviour = (AbilityChangeBehaviour)PML_ItemGetParam(itemData, (ItemField)ITSTAT_NEW_EFFECT);
    AbilID ability = (AbilID)PokeParty_GetParam(pokemon, PF_Ability, nullptr);

    PersonalData* personal = PML_PersonalLoadBW2(PokeParty_GetParam(pokemon, PF_Species, nullptr), PokeParty_GetParam(pokemon, PF_Forme, nullptr));
    u32 currentAbilitySlot = GetCurrentAbilitySlot(personal, ability);

    u32 changedAbilitySlot = 0;
    switch (behaviour)
    {
    case ABILITY_1:
    case ABILITY_2:
    case ABILITY_HIDDEN:
        changedAbilitySlot = behaviour;
        break;
    case ABILITY_CAPSULE:
        currentAbilitySlot ? changedAbilitySlot = 0 : changedAbilitySlot = 1;
        break;
    case ABILITY_PATCH:
        if (ability == PML_PersonalGetParam(personal, Personal_AbilH))
        {
            changedAbilitySlot = 1;
        }
        else
        {
            changedAbilitySlot = 2;
        }
        break;
    }

    PokeParty_SetParam(pokemon, PF_Ability, PML_PersonalGetParam(personal, (PersonalField)(Personal_Abil1 + changedAbilitySlot)));
}

void PrintIVModificationMessage(PokeList* pokeList, unsigned int itemID)
{
    ItemData* itemData = (ItemData*)PML_ItemReadDataFile(itemID, 0, HEAPID_POKELIST);
    PokemonIVModifier ivMod = (PokemonIVModifier)PML_ItemGetParam(itemData, (ItemField)ITSTAT_NEW_EFFECT);
    GFL_HeapFree(itemData);

    PokeListMessage_CreateWordSetSystem(pokeList, pokeList->pokeList_Message);

    ivMod = MIN_DEF;

    if (ivMod < MIN_IV)
    {
        short wStr[10];
        GetWideString(wStr, 10, "Maximized");
        GFL_StrBufLoadString(pokeList->pokeList_Message->wordSetSystem->MainStrbuf, wStr);
        GFL_WordSetCopyStrbuf(pokeList->pokeList_Message->wordSetSystem, 0, pokeList->pokeList_Message->wordSetSystem->MainStrbuf, 0);
    }
    else
    {
        short wStr[10];
        GetWideString(wStr, 10, "Minimized");
        GFL_StrBufLoadString(pokeList->pokeList_Message->wordSetSystem->MainStrbuf, wStr);
        GFL_WordSetCopyStrbuf(pokeList->pokeList_Message->wordSetSystem, 0, pokeList->pokeList_Message->wordSetSystem->MainStrbuf, 0);
    }


    PokeListMessage_LoadPokemonNicknameToStrBuf(pokeList, pokeList->pokeList_Message, 1, pokeList->SelectedPkm);

    int msgID = POKELIST_IVMODALL_MSGID;
    if (!(ivMod == MAX_ALL || ivMod == MIN_ALL))
    {
        msgID = POKELIST_IVMODSINGLE_MSGID;

        switch (ivMod)
        {
        case MAX_ATK:
        case MIN_ATK:
        {
            short wStr2[7];
            GetWideString(wStr2, 7, "Attack");
            GFL_StrBufLoadString(pokeList->pokeList_Message->wordSetSystem->MainStrbuf, wStr2);
            GFL_WordSetCopyStrbuf(pokeList->pokeList_Message->wordSetSystem, 2, pokeList->pokeList_Message->wordSetSystem->MainStrbuf, 0);
        }
        break;
        case MAX_DEF:
        case MIN_DEF:
        {
            short wStr2[8];
            GetWideString(wStr2, 8, "Defense");
            GFL_StrBufLoadString(pokeList->pokeList_Message->wordSetSystem->MainStrbuf, wStr2);
            GFL_WordSetCopyStrbuf(pokeList->pokeList_Message->wordSetSystem, 2, pokeList->pokeList_Message->wordSetSystem->MainStrbuf, 0);
        }
        break;
        case MAX_SPE:
        case MIN_SPE:
        {
            short wStr2[6];
            GetWideString(wStr2, 6, "Speed");
            GFL_StrBufLoadString(pokeList->pokeList_Message->wordSetSystem->MainStrbuf, wStr2);
            GFL_WordSetCopyStrbuf(pokeList->pokeList_Message->wordSetSystem, 2, pokeList->pokeList_Message->wordSetSystem->MainStrbuf, 0);
        }
        break;
        case MAX_SPA:
        case MIN_SPA:
        {
            short wStr2[10];
            GetWideString(wStr2, 10, "Sp.Attack");
            GFL_StrBufLoadString(pokeList->pokeList_Message->wordSetSystem->MainStrbuf, wStr2);
            GFL_WordSetCopyStrbuf(pokeList->pokeList_Message->wordSetSystem, 2, pokeList->pokeList_Message->wordSetSystem->MainStrbuf, 0);
        }
        break;
        case MAX_SPD:
        case MIN_SPD:
        {
            short wStr2[11];
            GetWideString(wStr2, 11, "Sp.Defense");
            GFL_StrBufLoadString(pokeList->pokeList_Message->wordSetSystem->MainStrbuf, wStr2);
            GFL_WordSetCopyStrbuf(pokeList->pokeList_Message->wordSetSystem, 2, pokeList->pokeList_Message->wordSetSystem->MainStrbuf, 0);
        }
        break;
        }
    }

    PokeList_PrintMessage(pokeList, msgID, 1, (int)sub_219DE58);
    PokeListMessage_ClearWordSetSystem(pokeList, pokeList->pokeList_Message);
}
bool CanIVModificationItemBeUsed(PartyPkm* pokeList, ItemData* itemData) 
{
    PokemonIVModifier ivMod = (PokemonIVModifier)PML_ItemGetParam(itemData, (ItemField)ITSTAT_NEW_EFFECT);

    // using a switch or == operator crashed the function for some reason
    if (IsEqual(ivMod, MAX_IV))
    {
    }
    else if (IsEqual(ivMod, MAX_ALL))
    {
        if (!(PokeParty_GetParam(pokeList, PF_IvATK, nullptr) == 31 &&
            PokeParty_GetParam(pokeList, PF_IvDEF, nullptr) == 31 &&
            PokeParty_GetParam(pokeList, PF_IvSPE, nullptr) == 31 &&
            PokeParty_GetParam(pokeList, PF_IvSPA, nullptr) == 31 &&
            PokeParty_GetParam(pokeList, PF_IvSPD, nullptr) == 31))
        {
            return true;
        }
    }
    else if (IsEqual(ivMod, MAX_ATK))
    {
        if (!PokeParty_GetParam(pokeList, PF_IvATK, nullptr) == 31)
        {
            return true;
        }
    }
    else if (IsEqual(ivMod, MAX_DEF))
    {
        if (!PokeParty_GetParam(pokeList, PF_IvDEF, nullptr) == 31)
        {
            return true;
        }
    }
    else if (IsEqual(ivMod, MAX_SPE))
    {
        if (!PokeParty_GetParam(pokeList, PF_IvSPE, nullptr) == 31)
        {
            return true;
        }
    }
    else if (IsEqual(ivMod, MAX_SPA))
    {
        if (!PokeParty_GetParam(pokeList, PF_IvSPA, nullptr) == 31)
        {
            return true;
        }
    }
    else if (IsEqual(ivMod, MAX_SPD))
    {
        if (!PokeParty_GetParam(pokeList, PF_IvSPD, nullptr) == 31)
        {
            return true;
        }
    }
    else if (IsEqual(ivMod, MIN_IV))
    {
    }
    else if (IsEqual(ivMod, MIN_ALL))
    {
        if (!(PokeParty_GetParam(pokeList, PF_IvATK, nullptr) == 0 &&
            PokeParty_GetParam(pokeList, PF_IvDEF, nullptr) == 0 &&
            PokeParty_GetParam(pokeList, PF_IvSPE, nullptr) == 0 &&
            PokeParty_GetParam(pokeList, PF_IvSPA, nullptr) == 0 &&
            PokeParty_GetParam(pokeList, PF_IvSPD, nullptr) == 0))
        {
            return true;
        }
    }
    else if (IsEqual(ivMod, MIN_ATK))
    {
        if (!PokeParty_GetParam(pokeList, PF_IvATK, nullptr) == 0)
        {
            return true;
        }
    }
    else if (IsEqual(ivMod, MIN_DEF))
    {
        if (!PokeParty_GetParam(pokeList, PF_IvDEF, nullptr) == 0)
        {
            return true;
        }
    }
    else if (IsEqual(ivMod, MIN_SPE))
    {
        if (!PokeParty_GetParam(pokeList, PF_IvSPE, nullptr) == 0)
        {
            return true;
        }
    }
    else if (IsEqual(ivMod, MIN_SPA))
    {
        if (!PokeParty_GetParam(pokeList, PF_IvSPA, nullptr) == 0)
        {
            return true;
        }
    }
    else if (IsEqual(ivMod, MIN_SPD))
    {
        if (!PokeParty_GetParam(pokeList, PF_IvSPD, nullptr) == 0)
        {
            return true;
        }
    }
    else
    {
    }
    return false;
}
void UseIVModificationItem(PartyPkm* pokemon, ItemData* itemData) 
{
    // using a switch or == operator crashed the function for some reason
    PokemonIVModifier ivMod = (PokemonIVModifier)PML_ItemGetParam(itemData, (ItemField)ITSTAT_NEW_EFFECT);
    if (ivMod == MAX_IV)
    {
    }
    else if (IsEqual(ivMod, MAX_ALL))
    {
        PokeParty_SetParam(pokemon, PF_IvATK, 31);
        PokeParty_SetParam(pokemon, PF_IvDEF, 31);
        PokeParty_SetParam(pokemon, PF_IvSPE, 31);
        PokeParty_SetParam(pokemon, PF_IvSPA, 31);
        PokeParty_SetParam(pokemon, PF_IvSPD, 31);
    }
    else if (IsEqual(ivMod, MAX_ATK))
    {
        PokeParty_SetParam(pokemon, PF_IvATK, 31);
    }
    else if (IsEqual(ivMod, MAX_DEF))
    {
        PokeParty_SetParam(pokemon, PF_IvDEF, 31);
    }
    else if (IsEqual(ivMod, MAX_SPE))
    {
        PokeParty_SetParam(pokemon, PF_IvSPE, 31);
    }
    else if (IsEqual(ivMod, MAX_SPA))
    {
        PokeParty_SetParam(pokemon, PF_IvSPA, 31);
    }
    else if (IsEqual(ivMod, MAX_SPD))
    {
        PokeParty_SetParam(pokemon, PF_IvSPD, 31);
    }
    else if (IsEqual(ivMod, MIN_IV))
    {
    }
    else if (IsEqual(ivMod, MIN_ALL))
    {
        PokeParty_SetParam(pokemon, PF_IvATK, 0);
        PokeParty_SetParam(pokemon, PF_IvDEF, 0);
        PokeParty_SetParam(pokemon, PF_IvSPE, 0);
        PokeParty_SetParam(pokemon, PF_IvSPA, 0);
        PokeParty_SetParam(pokemon, PF_IvSPD, 0);
    }
    else if (IsEqual(ivMod, MIN_ATK))
    {
        PokeParty_SetParam(pokemon, PF_IvATK, 0);
    }
    else if (IsEqual(ivMod, MIN_DEF))
    {
        PokeParty_SetParam(pokemon, PF_IvDEF, 0);
    }
    else if (IsEqual(ivMod, MIN_SPE))
    {
        PokeParty_SetParam(pokemon, PF_IvSPE, 0);
    }
    else if (IsEqual(ivMod, MIN_SPA))
    {
        PokeParty_SetParam(pokemon, PF_IvSPA, 0);
    }
    else if (IsEqual(ivMod, MIN_SPD))
    {
        PokeParty_SetParam(pokemon, PF_IvSPD, 0);
    }
    else
    {
    }
    PokeParty_RecalcStats(pokemon);
}

// OVL_165

// Called when an item is removed after being used
int THUMB_BRANCH_PokeList_SubItem(PokeList* a1, u16 a2)
{
    if (!IsBattleItemInfinite((ItemID)a2))
        return BagSave_SubItem(a1->pokeListParty->bagSaveData, a2, 1u, a1->heapID);
    else
        return 0;
}

#if ADD_NEW_ITEMS
// Return what type of healing or boost an item gives
ItemRestoreType THUMB_BRANCH_PokeList_GetItemRestoreType(ItemID itemID)
{
    ItemData* DataFile;
    ItemRestoreType result;
    int Param;
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
    int v9;
    int v10;
    int v11;
    int v12;
    int v13;
    int v14;

    DataFile = (ItemData*)PML_ItemReadDataFile(itemID, 0, HEAPID_POKELIST);
    if (PML_ItemGetParam(DataFile, ITSTAT_HAS_BATTLE_STATS) != 1)
    {
    LABEL_75:
        GFL_HeapFree(DataFile);
        return RESTORETYPE_NULL;
    }

    NewItemPokemonEffects newEffectType = (NewItemPokemonEffects)PML_ItemGetParam(DataFile, (ItemField)ITSTAT_NEW_EFFECT_TYPE);
    if (newEffectType)
    {
        GFL_HeapFree(DataFile);
        switch (newEffectType)
        {
        case ITEFF_NATURE:
            return (ItemRestoreType)RESTORETYPE_NATURE;
        case ITEFF_ABILITY:
            return (ItemRestoreType)RESTORETYPE_ABILITY;
        case ITEFF_IV_MOD:
            return (ItemRestoreType)RESTORETYPE_IV_MOD;
        }
    }

    if (PML_ItemGetParam(DataFile, ITSTAT_BOOST_ATK)
        || PML_ItemGetParam(DataFile, ITSTAT_BOOST_DEF)
        || PML_ItemGetParam(DataFile, ITSTAT_BOOST_SPA)
        || PML_ItemGetParam(DataFile, ITSTAT_BOOST_SPD)
        || PML_ItemGetParam(DataFile, ITSTAT_BOOST_SPE)
        || PML_ItemGetParam(DataFile, ITSTAT_BOOST_ACC)
        || PML_ItemGetParam(DataFile, ITSTAT_BOOST_CRIT))
    {
        GFL_HeapFree(DataFile);
        return RESTORETYPE_X_ITEM;
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_BOOST_SACRED_ASH))
    {
        GFL_HeapFree(DataFile);
        return RESTORETYPE_SACRED_ASH;
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_BOOST_RARECANDY))
    {
        GFL_HeapFree(DataFile);
        return RESTORETYPE_RARE_CANDY;
    }
    Param = PML_ItemGetParam(DataFile, ITSTAT_AILMENT_SLP);
    v4 = Param + 2 * PML_ItemGetParam(DataFile, ITSTAT_AILMENT_PSN);
    v5 = v4 + 4 * PML_ItemGetParam(DataFile, ITSTAT_AILMENT_BRN);
    v6 = v5 + 8 * PML_ItemGetParam(DataFile, ITSTAT_AILMENT_FRZ);
    v7 = v6 + 16 * PML_ItemGetParam(DataFile, ITSTAT_AILMENT_PAR);
    v8 = v7 + 32 * PML_ItemGetParam(DataFile, ITSTAT_AILMENT_CFZ);
    if (v8 > 16)
    {
        if (v8 > 32)
        {
            if (v8 == 63)
            {
                if (!PML_ItemGetParam(DataFile, ITSTAT_FLAG_HPREPLENISH))
                {
                    GFL_HeapFree(DataFile);
                    return RESTORETYPE_FULL_HEAL;
                }
                goto LABEL_30;
            }
        }
        else if (v8 == 32)
        {
            GFL_HeapFree(DataFile);
            return RESTORETYPE_CONFUSE;
        }
    LABEL_32:
        if (PML_ItemGetParam(DataFile, ITSTAT_AILMENT_INF))
        {
            GFL_HeapFree(DataFile);
            return RESTORETYPE_A;
        }
        if (!PML_ItemGetParam(DataFile, ITSTAT_FLAG_HPREPLENISH))
        {
            if (PML_ItemGetParam(DataFile, ITSTAT_BOOST_REVIVE))
            {
                GFL_HeapFree(DataFile);
                return RESTORETYPE_REVIVE;
            }
            v9 = PML_ItemGetParam(DataFile, ITSTAT_EV_HP);
            if (v9 > 0)
            {
                GFL_HeapFree(DataFile);
                return RESTORETYPE_EV_HP;
            }
            if (v9 < 0)
            {
                GFL_HeapFree(DataFile);
                return RESTORETYPE_EV_HP_DECREASE;
            }
            v10 = PML_ItemGetParam(DataFile, ITSTAT_EV_ATK);
            if (v10 > 0)
            {
                GFL_HeapFree(DataFile);
                return RESTORETYPE_EV_ATK;
            }
            if (v10 < 0)
            {
                GFL_HeapFree(DataFile);
                return RESTORETYPE_EV_ATK_DECREASE;
            }
            v11 = PML_ItemGetParam(DataFile, ITSTAT_EV_DEF);
            if (v11 > 0)
            {
                GFL_HeapFree(DataFile);
                return RESTORETYPE_EV_DEF;
            }
            if (v11 < 0)
            {
                GFL_HeapFree(DataFile);
                return RESTORETYPE_EV_DEF_DECREASE;
            }
            v12 = PML_ItemGetParam(DataFile, ITSTAT_EV_SPE);
            if (v12 > 0)
            {
                GFL_HeapFree(DataFile);
                return RESTORETYPE_EV_SPE;
            }
            if (v12 < 0)
            {
                GFL_HeapFree(DataFile);
                return RESTORETYPE_EV_SPE_DECREASE;
            }
            v13 = PML_ItemGetParam(DataFile, ITSTAT_EV_SPA);
            if (v13 > 0)
            {
                GFL_HeapFree(DataFile);
                return RESTORETYPE_EV_SPA;
            }
            if (v13 < 0)
            {
                GFL_HeapFree(DataFile);
                return RESTORETYPE_EV_SPA_DECREASE;
            }
            v14 = PML_ItemGetParam(DataFile, ITSTAT_EV_SPD);
            if (v14 > 0)
            {
                GFL_HeapFree(DataFile);
                return RESTORETYPE_EV_SPD;
            }
            if (v14 < 0)
            {
                GFL_HeapFree(DataFile);
                return RESTORETYPE_EV_SPD_DECREASE;
            }
            if (PML_ItemGetParam(DataFile, ITSTAT_BOOST_EVOSTONE))
            {
                GFL_HeapFree(DataFile);
                return RESTORETYPE_EVO_STONE;
            }
            if (PML_ItemGetParam(DataFile, ITSTAT_BOOST_PP1))
            {
                GFL_HeapFree(DataFile);
                return RESTORETYPE_BOOST_PP_1;
            }
            if (PML_ItemGetParam(DataFile, ITSTAT_BOOST_PPMAX))
            {
                GFL_HeapFree(DataFile);
                return RESTORETYPE_BOOST_PP_MAX;
            }
            if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_PPREPLENISH)
                || PML_ItemGetParam(DataFile, ITSTAT_FLAG_PPREPLENISH_FULL))
            {
                GFL_HeapFree(DataFile);
                return RESTORETYPE_PP_REPLENISH;
            }
            goto LABEL_75;
        }
    LABEL_30:
        GFL_HeapFree(DataFile);
        return RESTORETYPE_FULL_RESTORE;
    }
    if (v8 >= 16)
    {
        GFL_HeapFree(DataFile);
        return RESTORETYPE_PARALYSIS;
    }
    if (v8 > 8)
    {
        goto LABEL_32;
    }
    switch (v8)
    {
    case 1:
        GFL_HeapFree(DataFile);
        result = RESTORETYPE_SLEEP;
        break;
    case 2:
        GFL_HeapFree(DataFile);
        result = RESTORETYPE_POISON;
        break;
    case 4:
        GFL_HeapFree(DataFile);
        result = RESTORETYPE_BURN;
        break;
    case 8:
        GFL_HeapFree(DataFile);
        result = RESTORETYPE_FREEZE;
        break;
    default:
        goto LABEL_32;
    }
    return result;
}

// Creates the text box after using an item
ItemRestoreType THUMB_BRANCH_PokeList_PrintItemRecoverMessage(PokeList* a1, u16 itemID)
{
    ItemRestoreType itemRestoreType;
    int SelectedSlot;
    void(*v6)(PokeList*);
    __int16 Level;
    unsigned int v8;
    StrBuf* v9;
    PokeList* v10;
    int v11;
    int v12;
    PokeList* v13;
    int v14;
    unsigned __int8 v15;
    unsigned __int8 v16;
    ItemRestoreType itemRestoreTypeRet;

    itemRestoreType = PokeList_GetItemRestoreType((ItemID)a1->pokeListParty->ItemID);
    itemRestoreTypeRet = itemRestoreType;
    if (itemRestoreType != 29) // compare only to null so we can add new types
    {
        switch (itemRestoreType)
        {
        case RESTORETYPE_SACRED_ASH:
            SelectedSlot = a1->SelectedSlot;
            a1->field_C = 10;
            a1->CurrentHP = PokeListPlate_GetCurrentHP(a1, a1->pokeList_Plate[SelectedSlot]);
            v6 = sub_21A0678;
            goto LABEL_4;
        case RESTORETYPE_RARE_CANDY:
            Level = PokeParty_GetLevel(a1->SelectedPkm);
            PokeListMessage_CreateWordSetSystem(a1, a1->pokeList_Message);
            v8 = 0;
            PokeListMessage_LoadPokemonNicknameToStrBuf(a1, a1->pokeList_Message, 0, a1->SelectedPkm);
            PokeListMessage_WordSetNumber(a1, a1->pokeList_Message, 1, (Level + 1), 3u);
            PokeList_PrintMessage(a1, 169, 1, (int)sub_21A0750);
            PokeListMessage_ClearWordSetSystem(a1, a1->pokeList_Message);
            v9 = GFL_StrBufCreate(12, a1->heapID);
            PokeParty_GetParam(a1->SelectedPkm, PF_NicknameStrBuf, v9);
            sub_202D2F4((int)v9);
            GFL_StrBufFree(v9);
            while (v8 < 6)
            {
                a1->field_128[v8] = PokeParty_GetParam(a1->SelectedPkm, ((PkmField*)0x021A4C9C)[v8], 0);
                v8 = (v8 + 1);
            }
            return itemRestoreTypeRet;
        case RESTORETYPE_SLEEP:
            v10 = a1;
            v11 = 72;
            goto LABEL_25;
        case RESTORETYPE_POISON:
            v10 = a1;
            v11 = 46;
            goto LABEL_25;
        case RESTORETYPE_BURN:
            v10 = a1;
            v11 = 48;
            goto LABEL_25;
        case RESTORETYPE_FREEZE:
            v10 = a1;
            v11 = 49;
            goto LABEL_25;
        case RESTORETYPE_PARALYSIS:
            v10 = a1;
            v11 = 47;
            goto LABEL_25;
        case RESTORETYPE_FULL_HEAL:
            v10 = a1;
            v11 = 52;
            goto LABEL_25;
        case RESTORETYPE_FULL_RESTORE:
        case RESTORETYPE_REVIVE:
            v12 = a1->SelectedSlot;
            a1->field_C = 10;
            a1->CurrentHP = PokeListPlate_GetCurrentHP(a1, a1->pokeList_Plate[v12]);
            v6 = PokeList_PrintRecoverHPMessage;
        LABEL_4:
            a1->field_124 = (int)v6;
            return itemRestoreTypeRet;
        case RESTORETYPE_EV_HP:
        case RESTORETYPE_EV_ATK:
        case RESTORETYPE_EV_DEF:
        case RESTORETYPE_EV_SPE:
        case RESTORETYPE_EV_SPA:
        case RESTORETYPE_EV_SPD:
            v13 = a1;
            v14 = 56;
            v15 = itemRestoreTypeRet - 13;
            goto LABEL_19;
        case RESTORETYPE_EV_HP_DECREASE:
        case RESTORETYPE_EV_ATK_DECREASE:
        case RESTORETYPE_EV_DEF_DECREASE:
        case RESTORETYPE_EV_SPE_DECREASE:
        case RESTORETYPE_EV_SPA_DECREASE:
        case RESTORETYPE_EV_SPD_DECREASE:
            v16 = itemRestoreType - 19;
            if (PokeList_DoesPkmHaveParam(a1->SelectedPkm, (PkmField)((itemRestoreType - 19) + 13)))
            {
                if (PokeParty_GetParam(a1->SelectedPkm, PF_Happiness, 0) == 255)
                {
                    v13 = a1;
                    v14 = 90;
                }
                else
                {
                    v13 = a1;
                    v14 = 89;
                }
            }
            else
            {
                v13 = a1;
                v14 = 91;
            }
            v15 = v16;
        LABEL_19:
            sub_21A0D28(v13, v14, v15);
            break;
        case RESTORETYPE_BOOST_PP_1:
        case RESTORETYPE_BOOST_PP_MAX:
            PokeListMessage_CreateWordSetSystem(a1, a1->pokeList_Message);
            PokeListMessage_LoadMoveNameToStrBuf(a1, a1->pokeList_Message, 0, itemID);
            PokeList_PrintMessage(a1, 53, 1, (int)sub_219DE58);
            PokeListMessage_ClearWordSetSystem(a1, a1->pokeList_Message);
            break;
        case RESTORETYPE_PP_REPLENISH:
            v10 = a1;
            v11 = 50;
        LABEL_25:
            sub_21A0CE8(v10, v11);
            break;
        case RESTORETYPE_NATURE:
            PrintNatureChangeMessage(a1, (unsigned int)a1->pokeListParty->ItemID);
            break;
        case RESTORETYPE_ABILITY:
            PrintAbilityChangeMessage(a1, (unsigned int)a1->pokeListParty->ItemID);
            break;
        case RESTORETYPE_IV_MOD:
            PrintIVModificationMessage(a1, (unsigned int)a1->pokeListParty->ItemID);
            break;
        default:
            return itemRestoreTypeRet;
        }
    }
    return itemRestoreTypeRet;
}
#endif

// Checks if an item can be used
bool THUMB_BRANCH_SAFESTACK_PokeList_CanItemWithBattleStatsBeUsed(PartyPkm* a1, unsigned int item_ID, int a3, unsigned __int16 a4)
{
    ItemData* DataFile;
    u32 Status;
    u32 Param;
    u16 v9;
    int v10;
    int v11;
    int v12;
    int v13;
    int v14;
    int v15;
    int v16;
    int v17;
    int v19;
    int v20;
    int v21;
    int v22;
    int v23;
    int v24;
    NewItemPokemonEffects newEffectType;

    a4 = HEAPID_POKELIST;

    DataFile = (ItemData*)PML_ItemReadDataFile(item_ID, 0, a4);
    if (PML_ItemGetParam(DataFile, ITSTAT_HAS_BATTLE_STATS) == 1)
    {
        Status = PokeParty_GetStatusCond(a1);
        if (PML_ItemGetParam(DataFile, ITSTAT_AILMENT_SLP) && Status == 2
            || PML_ItemGetParam(DataFile, ITSTAT_AILMENT_PSN) && Status == 5
            || PML_ItemGetParam(DataFile, ITSTAT_AILMENT_BRN) && Status == 4
            || PML_ItemGetParam(DataFile, ITSTAT_AILMENT_FRZ) && Status == 3
            || PML_ItemGetParam(DataFile, ITSTAT_AILMENT_PAR) && Status == 1)
        {
            goto LABEL_102;
        }
        Param = PokeParty_GetParam(a1, PF_NowHP, 0);
        if ((PML_ItemGetParam(DataFile, ITSTAT_BOOST_REVIVE) || PML_ItemGetParam(DataFile, ITSTAT_BOOST_SACRED_ASH))
            && !PML_ItemGetParam(DataFile, ITSTAT_BOOST_RARECANDY))
        {
            if (!Param)
            {
            LABEL_102:
                GFL_HeapFree(DataFile);
                return 1;
            }
        }
        else if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_HPREPLENISH)
            && Param
            && Param < PokeParty_GetParam(a1, PF_MaxHP, 0))
        {
            goto LABEL_102;
        }
        if (PML_ItemGetParam(DataFile, ITSTAT_BOOST_RARECANDY))
        {
            u32 level_cap = 100;
#if ADD_INFINITE_RARE_CANDY
            if (item_ID == INFINITE_CANDY_ID || !RARE_CANDY_IGNORE_LVL_CAP)
                level_cap = GetLvlCap();
#else
            if (!RARE_CANDY_IGNORE_LVL_CAP)
                level_cap = GetLvlCap();
#endif
            u32 level = PokeParty_GetParam(a1, PF_Level, 0);
            if (level < level_cap)
            {
                goto LABEL_102;
            }
        }

#if ADD_NEW_ITEMS
        newEffectType = (NewItemPokemonEffects)PML_ItemGetParam(DataFile, (ItemField)ITSTAT_NEW_EFFECT_TYPE);
        if (newEffectType)
        {
            switch (newEffectType)
            {
            case ITEFF_NATURE:
                if (CanNatureChangeItemBeUsed(a1, DataFile))
                    goto LABEL_102;
                break;
            case ITEFF_ABILITY:
                if (CanAbilityChangeItemBeUsed(a1, DataFile))
                    goto LABEL_102;
                break;
            case ITEFF_IV_MOD:
                if (CanIVModificationItemBeUsed(a1, DataFile))
                    goto LABEL_102;
                break;
            }
        }
#endif
        if ((PML_ItemGetParam(DataFile, ITSTAT_BOOST_PP1) || PML_ItemGetParam(DataFile, ITSTAT_BOOST_PPMAX))
            && PokeParty_GetParam(a1, (PkmField)(a3 + 62), 0) < 3)
        {
            v9 = PokeParty_GetParam(a1, (PkmField)(a3 + 54), 0);
            if (PML_MoveGetMaxPP(v9, 0) >= 5)
            {
                goto LABEL_102;
            }
        }
        if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_PPREPLENISH) && PokeParty_CheckPPNeedsReplenish(a1, a3) == 1)
        {
            goto LABEL_102;
        }
        if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_PPREPLENISH_FULL))
        {
            v10 = 0;
            while (PokeParty_CheckPPNeedsReplenish(a1, v10) != 1)
            {
                if (++v10 >= 4)
                {
                    goto LABEL_36;
                }
            }
            goto LABEL_102;
        }
    LABEL_36:
        v11 = PokeParty_GetParam(a1, PF_EvHP, 0);
        v24 = PokeParty_GetParam(a1, PF_EvATK, 0);
        v23 = PokeParty_GetParam(a1, PF_EvDEF, 0);
        v22 = PokeParty_GetParam(a1, PF_EvSPE, 0);
        v21 = PokeParty_GetParam(a1, PF_EvSPA, 0);
        v12 = PokeParty_GetParam(a1, PF_EvSPD, 0);
        v20 = PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_ABOVE100);
        if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_HP))
        {
            v19 = PML_ItemGetParam(DataFile, ITSTAT_EV_HP);
            if (v19 <= 0 || PokeParty_GetParam(a1, PF_Species, 0) == PK292_SHEDINJA)
            {
                if (v19 < 0 && (v11 > 0 || PokeList_CanFriendshipBeIncreased(a1, DataFile)))
                {
                    goto LABEL_102;
                }
            }
            else if ((v11 < 100 || v20) && v11 < 255 && v12 + v21 + v22 + v23 + v11 + v24 < 510)
            {
                goto LABEL_102;
            }
        }
        if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_ATK))
        {
            v13 = PML_ItemGetParam(DataFile, ITSTAT_EV_ATK);
            if (v13 <= 0)
            {
                if (v13 < 0 && (v24 > 0 || PokeList_CanFriendshipBeIncreased(a1, DataFile)))
                {
                    goto LABEL_102;
                }
            }
            else if ((v24 < 100 || v20) && v24 < 255 && v12 + v21 + v22 + v23 + v11 + v24 < 510)
            {
                goto LABEL_102;
            }
        }
        if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_DEF))
        {
            v14 = PML_ItemGetParam(DataFile, ITSTAT_EV_DEF);
            if (v14 <= 0)
            {
                if (v14 < 0 && (v23 > 0 || PokeList_CanFriendshipBeIncreased(a1, DataFile)))
                {
                    goto LABEL_102;
                }
            }
            else if ((v23 < 100 || v20) && v23 < 255 && v12 + v21 + v22 + v23 + v11 + v24 < 510)
            {
                goto LABEL_102;
            }
        }
        if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_SPE))
        {
            v15 = PML_ItemGetParam(DataFile, ITSTAT_EV_SPE);
            if (v15 <= 0)
            {
                if (v15 < 0 && (v22 > 0 || PokeList_CanFriendshipBeIncreased(a1, DataFile)))
                {
                    goto LABEL_102;
                }
            }
            else if ((v22 < 100 || v20) && v22 < 255 && v12 + v21 + v22 + v23 + v11 + v24 < 510)
            {
                goto LABEL_102;
            }
        }
        if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_SPA))
        {
            v16 = PML_ItemGetParam(DataFile, ITSTAT_EV_SPA);
            if (v16 <= 0)
            {
                if (v16 < 0 && (v21 > 0 || PokeList_CanFriendshipBeIncreased(a1, DataFile)))
                {
                    goto LABEL_102;
                }
            }
            else if ((v21 < 100 || v20) && v21 < 255 && v12 + v21 + v22 + v23 + v11 + v24 < 510)
            {
                goto LABEL_102;
            }
        }
        if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_SPD))
        {
            v17 = PML_ItemGetParam(DataFile, ITSTAT_EV_SPD);
            if (v17 <= 0)
            {
                if (v17 < 0 && (v12 > 0 || PokeList_CanFriendshipBeIncreased(a1, DataFile)))
                {
                    goto LABEL_102;
                }
            }
            else if ((v12 < 100 || v20) && v12 < 255 && v12 + v21 + v22 + v23 + v11 + v24 < 510)
            {
                goto LABEL_102;
            }
        }
    }
    GFL_HeapFree(DataFile);
    return 0;
}

#if ADD_NEW_ITEMS
// Applies the effect of an item
int THUMB_BRANCH_SAFESTACK_PokeList_ApplyItemEffect(PartyPkm* a1, unsigned int a2, int a3, unsigned __int16 a4, int a5)
{
    int v6;
    ItemData* DataFile;
    int v8;
    int v9;
    int v10;
    u16 v11;
    u16 v12;
    u32 PkmLvExp;
    unsigned int v14;
    int v15;
    int i;
    int v17;
    int EVsAppliedAfterLimit;
    int v19;
    int v20;
    int v21;
    int v22;
    int v23;
    signed __int32 v25;
    int v26;
    int v27;
    int v28;
    int v29;
    u16 v30;
    int v33;
    int v34;
    int v35;
    int v36;
    int v37;
    int v38;
    int v39;
    signed int v40;
    int v41;
    int v42;
    int v43;
    int v44;
    int v45;
    int v46;
    int v47;
    u32 data;
    u32 Param;
    signed __int32 v50;
    NewItemPokemonEffects newEffectType;

    a5 = HEAPID_POKELIST;

    v30 = a2;
    v6 = 0;
    DataFile = (ItemData*)PML_ItemReadDataFile(a2, 0, a5);
    if (PML_ItemGetParam(DataFile, ITSTAT_HAS_BATTLE_STATS) != 1)
    {
        goto LABEL_85;
    }
    v8 = 0;
    Param = PokeParty_GetParam(a1, PF_StatusCond, 0);
    data = Param;
    if (PML_ItemGetParam(DataFile, ITSTAT_AILMENT_SLP))
    {
        data = 0;
        v8 = 1;
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_AILMENT_PSN))
    {
        data = 0;
        v8 = 1;
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_AILMENT_BRN))
    {
        data = 0;
        v8 = 1;
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_AILMENT_FRZ))
    {
        data = 0;
        v8 = 1;
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_AILMENT_PAR))
    {
        data = 0;
        v8 = 1;
    }
    if (Param != data)
    {
        PokeParty_SetParam(a1, PF_StatusCond, data);
        v6 = 1;
    }
    v42 = PokeParty_GetParam(a1, PF_NowHP, 0);
    v40 = PokeParty_GetParam(a1, PF_MaxHP, 0);
    if ((PML_ItemGetParam(DataFile, ITSTAT_BOOST_REVIVE) || PML_ItemGetParam(DataFile, ITSTAT_BOOST_SACRED_ASH))
        && !PML_ItemGetParam(DataFile, ITSTAT_BOOST_RARECANDY))
    {
        if (!v42)
        {
            v9 = PML_ItemGetParam(DataFile, ITSTAT_HEAL_AMOUNT);
            PokeList_ApplyHeal(a1, 0, v40, v9);
            v6 = 1;
        }
    }
    else
    {
        if (!PML_ItemGetParam(DataFile, ITSTAT_FLAG_HPREPLENISH))
        {
            goto LABEL_25;
        }
        if (v42 < v40)
        {
            v10 = PML_ItemGetParam(DataFile, ITSTAT_HEAL_AMOUNT);
            PokeList_ApplyHeal(a1, v42, v40, v10);
            v6 = 1;
        }
    }
    v8 = 1;
LABEL_25:
    v50 = PokeParty_GetParam(a1, PF_Level, 0);
    if (PML_ItemGetParam(DataFile, ITSTAT_BOOST_RARECANDY))
    {
        if (v50 < 100)
        {
            v11 = PokeParty_GetParam(a1, PF_Species, 0);
            v12 = PokeParty_GetParam(a1, PF_Forme, 0);
            PkmLvExp = PML_UtilGetPkmLvExp(v11, v12, (v50 + 1));
            PokeParty_SetParam(a1, PF_Experience, PkmLvExp);
            PokeParty_RecalcStats(a1);
            if (!v42)
            {
                v14 = PokeParty_GetParam(a1, PF_MaxHP, 0);
                PokeList_ApplyHeal(a1, 0, v14, v14 - v40);
            }
            v6 = 1;
        }
        v8 = 1;
    }

    newEffectType = (NewItemPokemonEffects)PML_ItemGetParam(DataFile, (ItemField)ITSTAT_NEW_EFFECT_TYPE);
    if (newEffectType)
    {
        switch (newEffectType)
        {
        case ITEFF_NATURE:
            UseNatureChangeItem(a1, DataFile);
            break;
        case ITEFF_ABILITY:
            UseAbilityChangeItem(a1, DataFile);
            break;
        case ITEFF_IV_MOD:
            UseIVModificationItem(a1, DataFile);
            break;
        }
        v6 = 1;
        v8 = 1;
    }

    if (PML_ItemGetParam(DataFile, ITSTAT_BOOST_EVOSTONE))
    {
        v8 = 1;
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_BOOST_PP1))
    {
        if (PokeList_ApplyMaxPPIncrease(a1, a3, 1) == 1)
        {
            v6 = 1;
        }
    }
    else
    {
        if (!PML_ItemGetParam(DataFile, ITSTAT_BOOST_PPMAX))
        {
            goto LABEL_41;
        }
        if (PokeList_ApplyMaxPPIncrease(a1, a3, 3) == 1)
        {
            v6 = 1;
        }
    }
    v8 = 1;
LABEL_41:
    if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_PPREPLENISH))
    {
        v15 = PML_ItemGetParam(DataFile, ITSTAT_PP_GAIN);
        if (PokeParty_AddPP(a1, a3, v15) == 1)
        {
            v6 = 1;
        }
    }
    else
    {
        if (!PML_ItemGetParam(DataFile, ITSTAT_FLAG_PPREPLENISH_FULL))
        {
            goto LABEL_51;
        }
        for (i = 0; i < 4; ++i)
        {
            v17 = PML_ItemGetParam(DataFile, ITSTAT_PP_GAIN);
            if (PokeParty_AddPP(a1, i, v17) == 1)
            {
                v6 = 1;
            }
        }
    }
    v8 = 1;
LABEL_51:
    v43 = PokeParty_GetParam(a1, PF_EvHP, 0);
    v41 = PokeParty_GetParam(a1, PF_EvATK, 0);
    v39 = PokeParty_GetParam(a1, PF_EvDEF, 0);
    v38 = PokeParty_GetParam(a1, PF_EvSPE, 0);
    v47 = PokeParty_GetParam(a1, PF_EvSPA, 0);
    v46 = PokeParty_GetParam(a1, PF_EvSPD, 0);
    v45 = PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_ABOVE100);
    if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_HP))
    {
        v44 = PML_ItemGetParam(DataFile, ITSTAT_EV_HP);
        if (v44 > 0 && PokeParty_GetParam(a1, PF_Species, 0) == 292)
        {
        LABEL_85:
            GFL_HeapFree(DataFile);
            return 0;
        }
        EVsAppliedAfterLimit = PokeList_GetEVsAppliedAfterLimit(v43, v46 + v47 + v38 + v41 + v39, v44, v45);
        if (EVsAppliedAfterLimit != -1)
        {
            v43 = EVsAppliedAfterLimit;
            PokeParty_SetParam(a1, PF_EvHP, EVsAppliedAfterLimit);
            PokeParty_RecalcStats(a1);
            v6 = 1;
        }
        if (v44 > 0)
        {
            v8 = 1;
        }
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_ATK))
    {
        v33 = PML_ItemGetParam(DataFile, ITSTAT_EV_ATK);
        v19 = PokeList_GetEVsAppliedAfterLimit(v41, v46 + v47 + v38 + v43 + v39, v33, v45);
        if (v19 != -1)
        {
            v41 = v19;
            PokeParty_SetParam(a1, PF_EvATK, v19);
            PokeParty_RecalcStats(a1);
            v6 = 1;
        }
        if (v33 > 0)
        {
            v8 = 1;
        }
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_DEF))
    {
        v34 = PML_ItemGetParam(DataFile, ITSTAT_EV_DEF);
        v20 = PokeList_GetEVsAppliedAfterLimit(v39, v46 + v47 + v38 + v43 + v41, v34, v45);
        if (v20 != -1)
        {
            v39 = v20;
            PokeParty_SetParam(a1, PF_EvDEF, v20);
            PokeParty_RecalcStats(a1);
            v6 = 1;
        }
        if (v34 > 0)
        {
            v8 = 1;
        }
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_SPE))
    {
        v35 = PML_ItemGetParam(DataFile, ITSTAT_EV_SPE);
        v21 = PokeList_GetEVsAppliedAfterLimit(v38, v46 + v47 + v39 + v43 + v41, v35, v45);
        if (v21 != -1)
        {
            v38 = v21;
            PokeParty_SetParam(a1, PF_EvSPE, v21);
            PokeParty_RecalcStats(a1);
            v6 = 1;
        }
        if (v35 > 0)
        {
            v8 = 1;
        }
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_SPA))
    {
        v36 = PML_ItemGetParam(DataFile, ITSTAT_EV_SPA);
        v22 = PokeList_GetEVsAppliedAfterLimit(v47, v46 + v38 + v39 + v43 + v41, v36, v45);
        if (v22 != -1)
        {
            v47 = v22;
            PokeParty_SetParam(a1, PF_EvSPA, v22);
            PokeParty_RecalcStats(a1);
            v6 = 1;
        }
        if (v36 > 0)
        {
            v8 = 1;
        }
    }
    if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_EVADD_SPD))
    {
        v37 = PML_ItemGetParam(DataFile, ITSTAT_EV_SPD);
        v23 = PokeList_GetEVsAppliedAfterLimit(v46, v47 + v38 + v39 + v43 + v41, v37, v45);
        if (v23 != -1)
        {
            PokeParty_SetParam(a1, PF_EvSPD, v23);
            PokeParty_RecalcStats(a1);
            v6 = 1;
        }
        if (v37 > 0)
        {
            v8 = 1;
        }
    }
    if (!v6 && v8 == 1)
    {
        goto LABEL_85;
    }
    v25 = PokeParty_GetParam(a1, PF_Happiness, 0);
    v26 = v25;
    if (v25 >= 100)
    {
        if (v25 >= 200)
        {
            if (v25 <= 255 && PML_ItemGetParam(DataFile, ITSTAT_FLAG_FRIENDSHIP_ADD3))
            {
                v29 = PML_ItemGetParam(DataFile, ITSTAT_FRIENDSHIP3);
                PokeList_ApplyFriendship(a1, v26, v29, v30, a4, a5);
                GFL_HeapFree(DataFile);
                return v6;
            }
        }
        else if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_FRIENDSHIP_ADD2))
        {
            v28 = PML_ItemGetParam(DataFile, ITSTAT_FRIENDSHIP2);
            PokeList_ApplyFriendship(a1, v26, v28, v30, a4, a5);
            GFL_HeapFree(DataFile);
            return v6;
        }
    }
    else if (PML_ItemGetParam(DataFile, ITSTAT_FLAG_FRIENDSHIP_ADD1))
    {
        v27 = PML_ItemGetParam(DataFile, ITSTAT_FRIENDSHIP1);
        PokeList_ApplyFriendship(a1, v26, v27, v30, a4, a5);
        GFL_HeapFree(DataFile);
        return v6;
    }
    GFL_HeapFree(DataFile);
    return v6;
}
#endif

// ARM9

#if ADD_NEW_ITEMS
// Extracts data from the ItemData struct
int THUMB_BRANCH_PML_ItemGetParam(ItemData* itemData, ItemField itemField)
{
    int result; // r0

    if (itemField > ITSTAT_SORT_IDX && itemField < ITSTAT_NEW_EFFECT_TYPE) // make sure we proccess new Stats in this function
    {
    LABEL_21:
        if (itemData->HasBattleStats & 0x01) // HasBattleStats is now a flag
        {
            return PML_ItemGetBattleStat(&itemData->BattleStats, itemField);
        }
        else
        {
            return itemData->BattleStats.CureInflict;
        }
    }
    else
    {
        switch (itemField)
        {
        case ITSTAT_PRICE:
            result = 10 * itemData->Price;
            break;
        case ITSTAT_USE_EFFECT:
            result = itemData->HeldEffect;
            break;
        case ITSTAT_USE_PARAM:
            result = itemData->HeldArgument;
            break;
        case ITSTAT_FLAG1:
            result = (itemData->Packed << 26) >> 31;
            break;
        case ITSTAT_FLAG2:
            result = (itemData->Packed << 25) >> 31;
            break;
        case ITSTAT_POCKET_FIELD:
            result = (itemData->Packed << 21) >> 28;
            break;
        case ITSTAT_EFFECT_FIELD:
            result = itemData->EffectField;
            break;
        case ITSTAT_EFFECT_BATTLE:
            result = itemData->EffectBattle;
            break;
        case ITSTAT_NATURAL_GIFT_EFFECT:
            result = itemData->NaturalGiftEffect;
            break;
        case ITSTAT_FLING_EFFECT:
            result = itemData->FlingEffect;
            break;
        case ITSTAT_FLING_POWER:
            result = itemData->FlingPower;
            break;
        case ITSTAT_NATURAL_GIFT_POWER:
            result = itemData->NaturalGiftPower;
            break;
        case ITSTAT_NATURAL_GIFT_TYPE:
            result = itemData->Packed & 0x1F;
            break;
        case ITSTAT_POCKET_BATTLE:
            result = (itemData->Packed << 16) >> 27;
            break;
        case ITSTAT_HAS_BATTLE_STATS:
            result = itemData->HasBattleStats & 0x01; // 0000 0001
            break;
        case ITSTAT_CLASS:
            result = itemData->ItemClass & 0x07; // 0000 0111
            break;
        case ITSTAT_CONSUMABLE:
            result = itemData->Consumable & 0x01; // 0000 0001
            break;
        case ITSTAT_SORT_IDX:
            result = itemData->SortIndex;
            break;
        case ITSTAT_NEW_EFFECT_TYPE: // NewPokemonItemEffect
            result = (itemData->HasBattleStats & 0x06) >> 1; // 0000 0110 -> 0000 0011
            break;
        case ITSTAT_NEW_EFFECT:
            result = (itemData->HasBattleStats & 0xF8) >> 3; // 1111 1000 -> 0001 1111
            break;
        default:
            goto LABEL_21;
        }
    }
    return result;
}

// Modifies data in the BoxPkm struct
void THUMB_BRANCH_PML_PkmSetParamCore(BoxPkm* pPkmBlock, PkmField field, u32 value)
{
    int v8;
    int EvSPD;
    int v10;
    int v11;
    int v12;
    int EvSPA;
    int v14;
    int v15;
    int v16;
    int v17;
    int v18;
    BoxPkmBlock0* block0;
    BoxPkmBlock1* block1;
    BoxPkmBlock3* block3;
    BoxPkmBlock2* block2;

    block0 = (BoxPkmBlock0*)PML_PkmGetParamBlockCore(pPkmBlock, pPkmBlock->pid, 0);
    block1 = (BoxPkmBlock1*)PML_PkmGetParamBlockCore(pPkmBlock, pPkmBlock->pid, 1);
    block2 = (BoxPkmBlock2*)PML_PkmGetParamBlockCore(pPkmBlock, pPkmBlock->pid, 2);
    block3 = (BoxPkmBlock3*)PML_PkmGetParamBlockCore(pPkmBlock, pPkmBlock->pid, 3);
    if (field <= PF_PokestarFame)
    {
        switch (field)
        {
        case PF_PID:
            pPkmBlock->pid = value;
            break;
        case 1:
            pPkmBlock->SanityFlags = value & 1 | pPkmBlock->SanityFlags & 0xFFFE;
            break;
        case 2:
            pPkmBlock->SanityFlags = (2 * value) & 3 | pPkmBlock->SanityFlags & 0xFFFD;
            break;
        case 3:
            pPkmBlock->SanityFlags = (4 * value) & 7 | pPkmBlock->SanityFlags & 0xFFFB;
            break;
        case 4:
            pPkmBlock->checksum = value;
            break;
        case PF_Species:
            block0->Species = value;
            break;
        case PF_Item:
            if (value < IT_ITEM_AMOUNT) // removed limiter
            {
                block0->HeldItem = value;
            }
            break;
        case PF_IdSet:
            block0->TIDSID = value;
            break;
        case PF_Experience:
            block0->Experience = value;
            break;
        case PF_Happiness:
            block0->Happiness = value;
            break;
        case PF_Ability:
            block0->Ability = value;
            break;
        case PF_Markings:
            block0->Markings = value;
            break;
        case PF_Region:
            block0->Region = value;
            break;
        case PF_EvHP:
            if ((value - block0->EvHP) > 510
                - (block0->EvSPD
                    + block0->EvSPA
                    + block0->EvSPE
                    + block0->EvDEF
                    + block0->EvHP
                    + block0->EvATK))
            {
                value = 510 - (block0->EvSPD + block0->EvSPA + block0->EvSPE + block0->EvDEF + block0->EvATK);
            }
            if (value > 255)
            {
                LOBYTE(value) = 255;
            }
            block0->EvHP = value;
            break;
        case PF_EvATK:
            if ((value - block0->EvATK) > 510
                - (block0->EvSPD
                    + block0->EvSPA
                    + block0->EvSPE
                    + block0->EvDEF
                    + block0->EvHP
                    + block0->EvATK))
            {
                value = 510 - (block0->EvSPD + block0->EvSPA + block0->EvSPE + block0->EvDEF + block0->EvHP);
            }
            if (value > 0xFF)
            {
                LOBYTE(value) = -1;
            }
            block0->EvATK = value;
            break;
        case PF_EvDEF:
            if ((value - block0->EvDEF) > 510
                - (block0->EvSPD
                    + block0->EvSPA
                    + block0->EvSPE
                    + block0->EvDEF
                    + block0->EvHP
                    + block0->EvATK))
            {
                value = 510 - (block0->EvSPD + block0->EvSPA + block0->EvSPE + block0->EvHP + block0->EvATK);
            }
            if (value > 0xFF)
            {
                LOBYTE(value) = -1;
            }
            block0->EvDEF = value;
            break;
        case PF_EvSPE:
            v8 = block0->EvHP + block0->EvATK;
            EvSPD = block0->EvSPD;
            if ((value - block0->EvSPE) > 510 - (EvSPD + block0->EvSPA + block0->EvSPE + block0->EvDEF + v8))
            {
                value = 510 - (EvSPD + block0->EvSPA + block0->EvDEF + v8);
            }
            if (value > 0xFF)
            {
                LOBYTE(value) = -1;
            }
            block0->EvSPE = value;
            break;
        case PF_EvSPA:
            v10 = block0->EvHP + block0->EvATK;
            v11 = block0->EvSPD;
            if ((value - block0->EvSPA) > 510 - (v11 + block0->EvSPA + block0->EvSPE + block0->EvDEF + v10))
            {
                value = 510 - (v11 + block0->EvSPE + block0->EvDEF + v10);
            }
            if (value > 0xFF)
            {
                LOBYTE(value) = -1;
            }
            block0->EvSPA = value;
            break;
        case PF_EvSPD:
            v12 = block0->EvHP + block0->EvATK;
            EvSPA = block0->EvSPA;
            if ((value - block0->EvSPD) > 510 - (block0->EvSPD + EvSPA + block0->EvSPE + block0->EvDEF + v12))
            {
                value = 510 - (EvSPA + block0->EvSPE + block0->EvDEF + v12);
            }
            if (value > 255)
            {
                LOBYTE(value) = 0xFF;
            }
            block0->EvSPD = value;
            break;
        case PF_ContestCool:
            block0->ContestCool = value;
            break;
        case PF_ContestBeauty:
            block0->ContestBeauty = value;
            break;
        case PF_ContestCute:
            block0->ContestCute = value;
            break;
        case PF_ContestSmart:
            block0->ContestSmart = value;
            break;
        case PF_ContestTough:
            block0->ContestTough = value;
            break;
        case PF_ContestSheen:
            block0->ContestSheen = value;
            break;
        case PF_RibbonChampionSinnoh:
        case PF_RibbonAbility:
        case PF_RibbonAbilityGreat:
        case PF_RibbonAbilityDouble:
        case PF_RibbonAbilityMulti:
        case PF_RibbonAbilityPair:
        case PF_RibbonAbilityWorld:
        case PF_RibbonAlert:
        case PF_RibbonShock:
        case PF_RibbonDowncast:
        case PF_RibbonCareless:
        case PF_RibbonRelax:
        case PF_RibbonSnooze:
        case PF_RibbonSmile:
        case PF_RibbonGorgeous:
        case PF_RibbonRoyal:
        case PF_RibbonGorgeousRoyal:
        case PF_RibbonFootprint:
        case PF_RibbonRecord:
        case PF_RibbonEvent:
        case PF_RibbonLegend:
        case PF_RibbonChampionWorld:
        case PF_RibbonBirthday:
        case PF_RibbonSpecial:
        case PF_RibbonSouvenir:
        case PF_RibbonWishing:
        case PF_RibbonClassic:
        case PF_RibbonPremier:
        case PF_Ribbon0x35:
            v14 = 1 << (field - PF_RibbonChampionSinnoh);
            if (value)
            {
                block0->Ribbons1 |= v14;
            }
            else
            {
                block0->Ribbons1 &= ~v14;
            }
            break;
        case PF_Move1:
        case PF_Move2:
        case PF_Move3:
        case PF_Move4:
            block1->Moves[field - PF_Move1] = value;
            break;
        case PF_Move1PP:
        case PF_Move2PP:
        case PF_Move3PP:
        case PF_Move4PP:
            block1->MovePPs[field - PF_Move1PP] = value;
            break;
        case PF_Move1PPUp:
        case PF_Move2PPUp:
        case PF_Move3PPUp:
        case PF_Move4PPUp:
            block1->MovePPUpStages[field - PF_Move1PPUp] = value;
            break;
        case PF_IvHP:
            block1->IVBits = value & 0x1F | block1->IVBits & 0xFFFFFFE0;
            break;
        case PF_IvATK:
            block1->IVBits = (32 * value) & 0x3FF | block1->IVBits & 0xFFFFFC1F;
            break;
        case PF_IvDEF:
            block1->IVBits = (value << 10) & 0x7FFF | block1->IVBits & 0xFFFF83FF;
            break;
        case PF_IvSPE:
            block1->IVBits = (value << 15) & 0xFFFFF | block1->IVBits & 0xFFF07FFF;
            break;
        case PF_IvSPA:
            block1->IVBits = (value << 20) & 0x1FFFFFF | block1->IVBits & 0xFE0FFFFF;
            break;
        case PF_IvSPD:
            block1->IVBits = (value << 27 >> 2) | block1->IVBits & 0xC1FFFFFF;
            break;
        case PF_IsEgg:
            block1->IVBits = (value << 31 >> 1) | block1->IVBits & 0xBFFFFFFF;
            break;
        case PF_RibbonG3Cool:
        case PF_RibbonG3CoolSuper:
        case PF_RibbonG3CoolHyper:
        case PF_RibbonG3CoolMaster:
        case PF_RibbonG3Beauty:
        case PF_RibbonG3BeautySuper:
        case PF_RibbonG3BeautyHyper:
        case PF_RibbonG3BeautyMaster:
        case PF_RibbonG3Cute:
        case PF_RibbonG3CuteSuper:
        case PF_RibbonG3CuteHyper:
        case PF_RibbonG3CuteMaster:
        case PF_RibbonG3Smart:
        case PF_RibbonG3SmartSuper:
        case PF_RibbonG3SmartHyper:
        case PF_RibbonG3SmartMaster:
        case PF_RibbonG3Tough:
        case PF_RibbonG3ToughSuper:
        case PF_RibbonG3ToughHyper:
        case PF_RibbonG3ToughMaster:
        case PF_RibbonChampionHoenn:
        case PF_RibbonWinning:
        case PF_RibbonVictory:
        case PF_RibbonArtist:
        case PF_RibbonEffort:
        case PF_RibbonChampionBattle:
        case PF_RibbonChampionRegional:
        case PF_RibbonChampionNational:
        case PF_RibbonCountry:
        case PF_RibbonNational:
        case PF_RibbonEarth:
        case PF_RibbonWorld:
            v15 = 1 << (field - 77);
            if (value)
            {
                block1->Ribbons2 |= v15;
            }
            else
            {
                block1->Ribbons2 &= ~v15;
            }
            break;
        case PF_FatefulEncounter:
            block1->GenderAndForme = value & 1 | block1->GenderAndForme & 0xFE;
            break;
        case PF_Sex:
            block1->GenderAndForme = (2
                * PML_UtilDerivePkmSex(
                    block0->Species,
                    (block1->GenderAndForme << 8) >> 11,
                    pPkmBlock->pid)) & 7 | block1->GenderAndForme & 0xF9;
            break;
        case PF_Forme:
            block1->GenderAndForme = (8 * value) | block1->GenderAndForme & 7;
            break;
        case PF_Nature:
            block1->Nature = value;
            break;
        case PF_IsHiddenAbility:
            block1->HiddenAbilAndNPoke = value & 1 | block1->HiddenAbilAndNPoke & 0xFFFE;
            break;
        case PF_Nature | 0x2:
            block1->dword1C = value;
            break;
        case PF_NicknameStrBuf:
            GFL_StrBufStoreString((StrBuf*)value, (short*)block2, 11);
            block1->IVBits = (PML_PkmIsNicknamed(pPkmBlock) << 31) | block1->IVBits & 0x7FFFFFFF;
            break;
        case PF_NicknameRaw:
            wcharsncpy((short*)value, (short*)block2, 11);
            block1->IVBits = (PML_PkmIsNicknamed(pPkmBlock) << 31) | block1->IVBits & 0x7FFFFFFF;
            break;
        case PF_NicknameRaw | 0x2:
            block2->unsigned___int816 = value;
            break;
        case PF_MetGameVersion:
            block2->OriginGame = value;
            break;
        case PF_RibbonG4Cool:
        case PF_RibbonG4CoolGreat:
        case PF_RibbonG4CoolUltra:
        case PF_RibbonG4CoolMaster:
        case PF_RibbonG4Beauty:
        case PF_RibbonG4BeautyGreat:
        case PF_RibbonG4BeautyUltra:
        case PF_RibbonG4BeautyMaster:
        case PF_RibbonG4Cute:
        case PF_RibbonG4CuteGreat:
        case PF_RibbonG4CuteUltra:
        case PF_RibbonG4CuteMaster:
        case PF_RibbonG4Smart:
        case PF_RibbonG4SmartGreat:
        case PF_RibbonG4SmartUltra:
        case PF_RibbonG4SmartMaster:
        case PF_RibbonG4Tough:
        case PF_RibbonG4ToughGreat:
        case PF_RibbonG4ToughUltra:
        case PF_RibbonG4ToughMaster:
        case PF_Ribbon0x8C:
            v16 = 1 << (field - 120);
            v17 = v16 >> 31;
            if (value)
            {
                v18 = HIDWORD(block2->Ribbons3) | v17;
                LODWORD(block2->Ribbons3) |= v16;
            }
            else
            {
                v18 = HIDWORD(block2->Ribbons3) & ~v17;
                LODWORD(block2->Ribbons3) &= ~v16;
            }
            HIDWORD(block2->Ribbons3) = v18;
            break;
        case PF_OTNameStrBuf:
            GFL_StrBufStoreString((StrBuf*)value, (short*)block3, 8);
            break;
        case PF_OTNameRaw:
            wcharsncpy((short*)value, (short*)block3, 8);
            break;
        case PF_HatchDateYear:
            block3->HatchDateYear = value;
            break;
        case PF_HatchDateMonth:
            block3->HatchDateMonth = value;
            break;
        case PF_HatchDateDay:
            block3->HatchDateDay = value;
            break;
        case PF_MetYear:
            block3->MetYear = value;
            break;
        case PF_MetMonth:
            block3->MetMonth = value;
            break;
        case PF_MetDay:
            block3->MetDay = value;
            break;
        case PF_HatchLocation:
            block3->HatchLocation = value;
            break;
        case PF_MetLocation:
            block3->MetLocation = value;
            break;
        case PF_Pokerus:
            block3->Pokerus = value;
            break;
        case PF_Pokeball:
            block3->Pokeball = value;
            break;
        case PF_MetLevel:
            block3->MetLevelAndOTGender = value & 0x7F | block3->MetLevelAndOTGender & 0x80;
            break;
        case PF_TrGender:
            block3->MetLevelAndOTGender = (value << 7) | block3->MetLevelAndOTGender & 0x7F;
            break;
        case PF_GroundTile:
            block3->CatchTerrainTileType = value;
            break;
        case PF_Pokeball | 0x4:
            block3->unsigned___int81E = value;
            break;
        case PF_IVsAll:
            block1->IVBits = (value >> 25 << 15) & 0xFFFFF | (value >> 20 << 27 >> 2) & 0xFFF07FFF | (value >> 10 << 10) & 0x7FFF | (32 * (value >> 5)) & 0x3FF | value & 0x1F | block1->IVBits & 0xC0000000 | (value >> 15 << 20) & 0x1F07FFF;
            break;
        case PF_NicknameStrBufKeepFlags:
            GFL_StrBufStoreString((StrBuf*)value, (short*)block2, 11);
            break;
        case PF_NicknameRawKeepFlags:
            wcharsncpy((short*)value, (short*)block2, 11);
            break;
        case PF_NPokemon:
            block1->HiddenAbilAndNPoke = (2 * value) & 3 | block1->HiddenAbilAndNPoke & 0xFFFD;
            break;
        case PF_PokestarFame:
            block3->PokeStarFame = value;
            break;
        default:
            return;
        }
    }
}

// Extracts data from the BoxPkm struct
u32 THUMB_BRANCH_PML_PkmGetParamCore(BoxPkm* pkm, PkmField field, void* extra)
{
    unsigned int SanityFlags;
    unsigned int IVBits;
    int HiddenAbilAndNPoke;
    unsigned int v13;
    StrBuf* v14;
    wchar_t* v15;
    int v16;
    int v17;
    int Species;
    int v19;
    __int32 v22;

    u32 result = 0;
    u32 num = 1;

    BoxPkmBlock0* block0 = (BoxPkmBlock0*)PML_PkmGetParamBlockCore(pkm, pkm->pid, 0);
    BoxPkmBlock1* block1 = (BoxPkmBlock1*)PML_PkmGetParamBlockCore(pkm, pkm->pid, 1);
    BoxPkmBlock2* block2 = (BoxPkmBlock2*)PML_PkmGetParamBlockCore(pkm, pkm->pid, 2);
    BoxPkmBlock3* block3 = (BoxPkmBlock3*)PML_PkmGetParamBlockCore(pkm, pkm->pid, 3);

    wchar_t* v8 = (wchar_t*)block3;
    if (field > PF_PokestarFame)
    {
        return 0;
    }
    switch (field)
    {
    case PF_PID:
        return pkm->pid;
    case 1:
        SanityFlags = pkm->SanityFlags;
        goto LABEL_6;
    case 2:
        HiddenAbilAndNPoke = pkm->SanityFlags;
        goto LABEL_73;
    case 3:
        IVBits = pkm->SanityFlags << 29;
        goto LABEL_7;
    case 4:
        return pkm->checksum;
    case PF_Species:
        if ((pkm->SanityFlags & 4) != 0)
        {
            return 650;
        }
        return block0->Species;
    case PF_Item:
        if (block0->HeldItem < IT_ITEM_AMOUNT) // removed limiter
        {
            return block0->HeldItem;
        }
        return 0;
    case PF_IdSet:
        return block0->TIDSID;
    case PF_Experience:
        return block0->Experience;
    case PF_Happiness:
        return block0->Happiness;
    case PF_Ability:
        return block0->Ability;
    case PF_Markings:
        return block0->Markings;
    case PF_Region:
        return block0->Region;
    case PF_EvHP:
        return block0->EvHP;
    case PF_EvATK:
        return block0->EvATK;
    case PF_EvDEF:
        return block0->EvDEF;
    case PF_EvSPE:
        return block0->EvSPE;
    case PF_EvSPA:
        return block0->EvSPA;
    case PF_EvSPD:
        return block0->EvSPD;
    case PF_ContestCool:
        return block0->ContestCool;
    case PF_ContestBeauty:
        return block0->ContestBeauty;
    case PF_ContestCute:
        return block0->ContestCute;
    case PF_ContestSmart:
        return block0->ContestSmart;
    case PF_ContestTough:
        return block0->ContestTough;
    case PF_ContestSheen:
        return block0->ContestSheen;
    case PF_RibbonChampionSinnoh:
    case PF_RibbonAbility:
    case PF_RibbonAbilityGreat:
    case PF_RibbonAbilityDouble:
    case PF_RibbonAbilityMulti:
    case PF_RibbonAbilityPair:
    case PF_RibbonAbilityWorld:
    case PF_RibbonAlert:
    case PF_RibbonShock:
    case PF_RibbonDowncast:
    case PF_RibbonCareless:
    case PF_RibbonRelax:
    case PF_RibbonSnooze:
    case PF_RibbonSmile:
    case PF_RibbonGorgeous:
    case PF_RibbonRoyal:
    case PF_RibbonGorgeousRoyal:
    case PF_RibbonFootprint:
    case PF_RibbonRecord:
    case PF_RibbonEvent:
    case PF_RibbonLegend:
    case PF_RibbonChampionWorld:
    case PF_RibbonBirthday:
    case PF_RibbonSpecial:
    case PF_RibbonSouvenir:
    case PF_RibbonWishing:
    case PF_RibbonClassic:
    case PF_RibbonPremier:
    case PF_Ribbon0x35:
        if ((block0->Ribbons1 & (1 << (field - PF_RibbonChampionSinnoh))) == 0)
        {
            return 0;
        }
        return num;
    case PF_Move1:
    case PF_Move2:
    case PF_Move3:
    case PF_Move4:
        return block1->Moves[field - PF_Move1];
        break;
    case PF_Move1PP:
    case PF_Move2PP:
    case PF_Move3PP:
    case PF_Move4PP:
        return block1->MovePPs[field - PF_Move1PP];
        break;
    case PF_Move1PPUp:
    case PF_Move2PPUp:
    case PF_Move3PPUp:
    case PF_Move4PPUp:
        return block1->MovePPUpStages[field - PF_Move1PPUp];
    case PF_Move1PPMax:
    case PF_Move2PPMax:
    case PF_Move3PPMax:
    case PF_Move4PPMax:
        v22 = field - 66;
        if (!block1->Moves[v22])
        {
            return result;
        }
        return PML_MoveGetMaxPP(block1->Moves[v22], block1->MovePPUpStages[v22]);
    case PF_IvHP:
        v13 = block1->IVBits << 27;
        return v13 >> 27;
    case PF_IvATK:
        v13 = block1->IVBits << 22;
        return v13 >> 27;
    case PF_IvDEF:
        v13 = block1->IVBits << 17;
        return v13 >> 27;
    case PF_IvSPE:
        v13 = block1->IVBits << 12;
        return v13 >> 27;
    case PF_IvSPA:
        v13 = block1->IVBits << 7;
        return v13 >> 27;
    case PF_IvSPD:
        v13 = 4 * block1->IVBits;
        return v13 >> 27;
    case PF_IsEgg:
        result = (pkm->SanityFlags << 29) >> 31;
        if ((pkm->SanityFlags & 4) != 0)
        {
            return result;
        }
        goto LABEL_62;
    case PF_RibbonG3Cool:
    case PF_RibbonG3CoolSuper:
    case PF_RibbonG3CoolHyper:
    case PF_RibbonG3CoolMaster:
    case PF_RibbonG3Beauty:
    case PF_RibbonG3BeautySuper:
    case PF_RibbonG3BeautyHyper:
    case PF_RibbonG3BeautyMaster:
    case PF_RibbonG3Cute:
    case PF_RibbonG3CuteSuper:
    case PF_RibbonG3CuteHyper:
    case PF_RibbonG3CuteMaster:
    case PF_RibbonG3Smart:
    case PF_RibbonG3SmartSuper:
    case PF_RibbonG3SmartHyper:
    case PF_RibbonG3SmartMaster:
    case PF_RibbonG3Tough:
    case PF_RibbonG3ToughSuper:
    case PF_RibbonG3ToughHyper:
    case PF_RibbonG3ToughMaster:
    case PF_RibbonChampionHoenn:
    case PF_RibbonWinning:
    case PF_RibbonVictory:
    case PF_RibbonArtist:
    case PF_RibbonEffort:
    case PF_RibbonChampionBattle:
    case PF_RibbonChampionRegional:
    case PF_RibbonChampionNational:
    case PF_RibbonCountry:
    case PF_RibbonNational:
    case PF_RibbonEarth:
    case PF_RibbonWorld:
        if ((block1->Ribbons2 & (1 << (field - PF_RibbonG3Cool))) == 0)
        {
            return 0;
        }
        return num;
    case PF_FatefulEncounter:
        SanityFlags = block1->GenderAndForme;
        goto LABEL_6;
    case PF_Sex:
        result = PML_UtilDerivePkmSex(block0->Species, (block1->GenderAndForme << 8) >> 11, pkm->pid);
        block1->GenderAndForme = block1->GenderAndForme & 0xF9 | (2 * result) & 7;
        pkm->checksum = PML_CryptoGenKey(&pkm->ContentBuffer, 128);
        return result;
    case PF_Forme:
        v13 = block1->GenderAndForme << 24;
        return v13 >> 27;
    case PF_Nature:
        return block1->Nature;
    case PF_IsHiddenAbility:
        SanityFlags = block1->HiddenAbilAndNPoke;
    LABEL_6:
        IVBits = SanityFlags << 31;
        goto LABEL_7;
    case PF_Nature | 0x2:
        return block1->dword1C;
    case PF_NicknameStrBuf:
        if ((pkm->SanityFlags & 4) != 0)
        {
            GFL_MsgDataLoadStrbuf(g_PMLSpeciesNamesResident, 651, (StrBuf*)extra);
        }
        else
        {
            v14 = (StrBuf*)extra;
            v8 = (short*)block2;
        LABEL_78:
            GFL_StrBufLoadString(v14, v8);
        }
        return result;
    case PF_NicknameRaw:
        if ((pkm->SanityFlags & 4) != 0)
        {
            GFL_MsgDataLoadRawStr(g_PMLSpeciesNamesResident, 651, (short*)extra, 11);
        }
        else
        {
            block3 = (BoxPkmBlock3*)block2;
            v15 = (short*)extra;
            v16 = 11;
        LABEL_82:
            wcharsncpy((short*)block3, v15, v16);
        }
        return result;
    case PF_HasNickname:
        IVBits = block1->IVBits;
        goto LABEL_7;
    case PF_NicknameRaw | 0x2:
        return block2->unsigned___int816;
    case PF_MetGameVersion:
        return block2->OriginGame;
    case PF_RibbonG4Cool:
    case PF_RibbonG4CoolGreat:
    case PF_RibbonG4CoolUltra:
    case PF_RibbonG4CoolMaster:
    case PF_RibbonG4Beauty:
    case PF_RibbonG4BeautyGreat:
    case PF_RibbonG4BeautyUltra:
    case PF_RibbonG4BeautyMaster:
    case PF_RibbonG4Cute:
    case PF_RibbonG4CuteGreat:
    case PF_RibbonG4CuteUltra:
    case PF_RibbonG4CuteMaster:
    case PF_RibbonG4Smart:
    case PF_RibbonG4SmartGreat:
    case PF_RibbonG4SmartUltra:
    case PF_RibbonG4SmartMaster:
    case PF_RibbonG4Tough:
    case PF_RibbonG4ToughGreat:
    case PF_RibbonG4ToughUltra:
    case PF_RibbonG4ToughMaster:
    case PF_Ribbon0x8C:
        if ((block2->Ribbons3 & (1LL << (field - 120))) == 0)
        {
            return 0;
        }
        return num;
    case PF_OTNameStrBuf:
        v14 = (StrBuf*)extra;
        goto LABEL_78;
    case PF_OTNameRaw:
        v15 = (short*)extra;
        v16 = 8;
        goto LABEL_82;
    case PF_HatchDateYear:
        return block3->HatchDateYear;
    case PF_HatchDateMonth:
        return block3->HatchDateMonth;
    case PF_HatchDateDay:
        return block3->HatchDateDay;
    case PF_MetYear:
        return block3->MetYear;
    case PF_MetMonth:
        return block3->MetMonth;
    case PF_MetDay:
        return block3->MetDay;
    case PF_HatchLocation:
        return block3->HatchLocation;
    case PF_MetLocation:
        return block3->MetLocation;
    case PF_Pokerus:
        return block3->Pokerus;
    case PF_Pokeball:
        return block3->Pokeball;
    case PF_MetLevel:
        return block3->MetLevelAndOTGender & 0x7F;
    case PF_TrGender:
        IVBits = block3->MetLevelAndOTGender << 24;
        goto LABEL_7;
    case PF_GroundTile:
        return block3->CatchTerrainTileType;
    case PF_Pokeball | 0x4:
        return block3->unsigned___int81E;
    case PF_Level:
        return CalcLevelByExp(block0->Species, (block1->GenderAndForme << 8) >> 11, block0->Experience);
    case PF_IsSpeciesValid:
        if (!block0->Species)
        {
            return 0;
        }
        return num;
    case PF_ATK | PF_Experience:
        result = (pkm->SanityFlags << 29) >> 31;
        if ((pkm->SanityFlags & 4) != 0)
        {
            return result;
        }
    LABEL_62:
        IVBits = 2 * block1->IVBits;
    LABEL_7:
        result = IVBits >> 31;
        break;
    case PF_LegalSpecies:
        result = block0->Species;
        if (block0->Species && ((block1->IVBits & 0x40000000) != 0 || (pkm->SanityFlags & 4) != 0))
        {
            return 650;
        }
        return result;
    case PF_IVsAll:
        v17 = block1->IVBits;
        return ((4 * v17) >> 27 << 25) | v17 & 0x1F | (32 * ((v17 << 22) >> 27)) | ((v17 << 17) >> 27 << 10) | ((v17 << 12) >> 27 << 15) | ((v17 << 7) >> 27 << 20);
    case PF_IsNotNidoran:
        Species = block0->Species;
        return Species != 29 && Species != 32 || block1->IVBits < 0;
    case PF_Type1:
    case PF_Type2:
        v19 = block0->Species;
        if (v19 == 493 && block0->Ability == 121)
        {
            return PML_ItemGetArceusPlateType(block0->HeldItem);
        }
        else
        {
            return PML_PersonalGetParamSingle(v19, (block1->GenderAndForme << 8) >> 11, (PersonalField)(field - 168));
        }
    case PF_NPokemon:
        HiddenAbilAndNPoke = block1->HiddenAbilAndNPoke;
    LABEL_73:
        IVBits = HiddenAbilAndNPoke << 30;
        goto LABEL_7;
    case PF_PokestarFame:
        return block3->PokeStarFame;
    default:
        return 0;
    }
    return result;
}
#endif

C_DECL_END