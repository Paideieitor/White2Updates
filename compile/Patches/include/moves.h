#ifndef __MOVES_H
#define __MOVES_H

#include "defs.h"

enum MoveCategory
{
    CATEGORY_STATUS = 0,
    CATEGORY_PHYSICAL = 1,
    CATEGORY_SPECIAL = 2,
};

extern "C" u32 PML_MoveGetCategory(u32 moveID);
extern "C" u32 PML_MoveGetMaxPP(u32 moveID, u32 ppUpStage);

#endif // __MOVES_H
