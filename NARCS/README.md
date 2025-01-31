# NARC modifications
This is the indepth explanation of what does each file change:
## NARC ``a/0/0/2`` (Game Text NARC)
  - *6* adds the repel toggled messages when used from the bag
  - *18* adds the battle messages that don't depend on a specific Pokémon, like the message anouncing a terrain has been set
  - *19* adds the battle messages that depend on a specific Pokémon each message has 3 entries, for a player, foe and wild Pokémons
  - *63* changes the description of the Infinite Rare Candy and Infinite Repel items
  - *64* changes the name of the Infinite Rare Candy and Infinite Repel items
  - *365* adds the Infinite Rare Candy and Infinite Repel to the register list
  - *374* adds the name of all new abilities
  - *375* adds the description of all new abilities (currently place holders)
  - *396* adds the new dialog for each new abilities for an NPC (currently place holders)
  - *481* changes the name of the Infinite Rare Candy and Infinite Repel items when being picked up
  - *482* changes the plural name of the Infinite Rare Candy and Infinite Repel items
  - *487* adds the name of all new abilities in all caps
## NARC ``a/0/0/3`` (Script Text NARC)
  - *411* changes the message when the repel is depleted to the toggled messages
## NARC ``a/0/2/4`` (Item Data NARC)
  - *622* changes the item flags of the Infinite Rare Candy item so it can be used and registered
  - *623* changes the item flags of the Infinite Repel item so it can be used and registered
## NARC ``a/0/5/6`` (Global Scripts NARC)
  - *1248* changes the repel global script to display the toggle messages instead of the vanilla behaviour

# NARCS required per patch
Here you can check which files you need depending on the patch you want to compile:
## Infinite Rare Candy Item
``002/63``, ``002/64``, ``002/365``, ``002/481``, ``002/482``, ``024/622``
## Infinite Repel Item
``002/6``, ``002/63``, ``002/64``, ``002/365``, ``002/481``, ``002/482``. ``003/411``, ``024/623``, ``056/1248``
## Ability Expansion
``002/18``, ``002/19``, ``002/374``, ``002/375``, ``002/396``, ``002/487``
