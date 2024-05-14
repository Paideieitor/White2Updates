#include "better_repel.h"

C_DECL_BEGIN
int sub_200DDF0(int result, __int16 a2);
int sub_219B380(int a1, u16 a2);
int sub_21999F8(int a1, int a2, int a3);
void sub_219F6E4(int a1, int a2);
int sub_2199900(_DWORD* a1, int a2);
int sub_219A22C(int a1);

int THUMB_BRANCH_RepelEffect(int a1)
{
    SaveControl* SaveControl; // r0
    EncountSave* EncountSave; // r7
    u8 ItemParam; // r0

    u32 itemID = *(_DWORD*)(a1 + 2208);
    if (itemID == INFINITE_REPEL_ID)
    {
        SaveControl = GameData_GetSaveControl(*(GameData**)a1);
        EncountSave = SaveControl_GetEncountSave(SaveControl);
        if (!EncountSave->RepelSteps)
        {
            DPRINT("ACTIVATED INFINITE REPEL\n");
            EncountSave->RepelSteps = 0xFF;

            GFL_MsgDataLoadStrbuf(*(MsgData**)(a1 + 1312), 63, *(StrBuf**)(a1 + 1320));
            copyVarForText(*(WordSetSystem**)(a1 + 1316), 0, *(void**)(a1 + 8));
            sub_21999F8(a1, 1, itemID);
            GFL_WordSetFormatStrbuf(*(WordSetSystem**)(a1 + 1316), *(StrBuf**)(a1 + 1324), *(StrBuf**)(a1 + 1320));
            sub_219F6E4(a1, 1);
            playSeqFullVol(1996);
        }
        else
        {
            DPRINT("DEACTIVATED INFINITE REPEL\n");
            EncountSave->RepelSteps = 0x00;
        }
        
        int output = sub_2199900((_DWORD*)a1, (int)sub_219A22C);
        return output;
    }
    return false;
}
C_DECL_END