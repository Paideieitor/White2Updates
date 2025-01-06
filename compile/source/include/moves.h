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

#endif // __MOVES_H
