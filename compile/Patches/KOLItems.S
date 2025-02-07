.thumb

#include "settings.h"

#if ADD_INFINITE_RARE_CANDY

.type THUMB_BRANCH_LINK_OpenPokeParty_0x94, %function

@ This adds the Infinite Rare Candy item to the list of items that have an effect when loading the PokeList
THUMB_BRANCH_LINK_OpenPokeParty_0x94:
    ldr r0, =638 @ IT0638_REVEAL_GLASS
    cmp r7, r0
    beq OpenPokeParty_return
    ldr r0, =INFINITE_CANDY_ID
    cmp r7, r0
OpenPokeParty_return:
    bx lr
    .size THUMB_BRANCH_LINK_OpenPokeParty_0x94, . - THUMB_BRANCH_LINK_OpenPokeParty_0x94

#endif // ADD_INFINITE_RARE_CANDY
