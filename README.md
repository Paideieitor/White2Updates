# White2Updates
Code Injection Patches to upgrade the White 2 gameplay experience while making a ROM Hack

## Patches
### Hard Level Caps
  - Limits the maximum level a pokemon can be (doesn't affect Rare Candy, use the Rare Candy Level Caps patch to add this feature)
  - Set the game variable 16415 to the level cap you want using CTRMap or some other editor, use the command WorkSet (```EventWorks.Set(16415, levelcap)```) from a script to acomplish that
  - If you are already using the variable 16415 you can change it modifying the ```LEVEL_CAP_VAR``` definition to the value you want, and compiling and installing the new patch
### Rare Candy Level Caps
  - Adds the infinite Rare Candy item, this item can't level Pokemon over the level cap
  - By default Rare Candy ignore the level cap (like in Run&Bun)
  - Change the definition of ```RARE_CANDY_IGNORE_CAP``` to ```false``` and complile to make Rare Candy respect the level cap
  - To add the infinite Rare Candy item paste the 622 file in the *vfs/data/a/0/2/4* folder in your CTRMAP project
  - This patch doesn't change the name, description and icon of the new item, so you will need to edit that to whatever you want
  - If you are already using the item 622 you will need to change the definition of ```INFINITE_CANDY_ID``` to the item ID you are repacing and also change the name of the 622 file to the new item ID
  - The level cap is shared by default with the Hard Level Caps patch
  - To change the level cap or the game variable location follow the instructions at the Hard Level Caps notes (this is independent of you having or not the Hard Level Caps patch installed)
### Dynamic Speed
  - Instead of calculating action order at the begining of every turn, the game updates it depending on the most actualized speed stats and priority
  - Fully functional with speed/priority items and the moves After You and Quash
  - To detect when After You and Quash are used (without using move ID) I use the game variable 16400, if you are already using it change it at the ```ORDER_ALTERING_MOVE_VAR``` definition, and compile and install the new patch
  - I've tested several scenearios and they all work fine, if you find a bug or a missing feature let me know :)
### Item Expansion
IMPORTANT 

The source code in the ItemExpansion folder currently compiles to 4 dlls which contain the following:
  - Implementation of new battle items (Ability Capsule, Mints, Assault Vest, Loaded Dice, etc.)
  - Implementation of level caps
  - Implementation of Infinite Rare Candy item and Infinite Repel (that can be registered)
  - Implementation of Dynamic Speed

To see more details on each feature read their respective explanation in this readme.

If you don't want any of this features you will have to take them out of the code before compiling (some can be toggled by a definition).

After compiling the patch you will need to copy the files in the NARCs forlder on their respective folder in your CTRMap project, alternatively you can unpack, expand and repack the whole narc file with Knarc and insert it in the ROM.

Before listing the features here is a list of things that are still a work in progress or that might be good to know to use the patch:
  - The new items can not be moved to the free space bag for now
  - The jumps in ID number of the new items are due to a game breaking bug, so make sure to not use item IDs (654 - 664) and (692 - 695)
  - The new items might have behavioral and visual bugs
  - Each cpp file matches the name of the ESDB it uses
  - The code is all commented to give context of what does what and sometimes why (provably went overboard with that)
  - This is not a standalone patch because I'm building a code base that can be later divided depending on the features the user wants
  - Heavy-Duty Boots are not implemented (OVL_169 crashed the game, I'm on it)
  - Eject Pack interactions with pursuit are untested, it's likely to work like with U-Turn or Volt Switch
  - When using the Infinite Repel Item when registered, no message will pop up (work in progress)

FEATURES

This patch contains the following features:
  - Added 49 new items from newer generations (followed by their new ID):
    - Weakness Policy [639]
    - Assault Vest [640]
    - Pixie Plate [641]
    - Ability Capasule [642]
    - Luminous Moss [643]
    - Snowball [644]
    - Safety Goggles [645]
    - Roseli Berry [646]
    - Kee Berry [647]
    - Maranga Berry [648]
    - Bottle Cap [649] (not implemented)
    - Gold Bottle Cap [650]
    - Adrenaline Orb [651]
    - Protective Pads [652]
    - Throat Spray [653]
    - Eject Pack [665]
    - Heavy-Duty Boot [666] (not implemented)
    - Blunder Policy [667] (visual bug with the icon)
    - Room Service [668]
    - Utility Ubrella [669]
    - All Mints [670 - 690] (ordered the same way as the vanilla games)
    - Ability Patch [691]
    - Ability Shield [696]
    - Clear Amulet [697]
    - Mirror Herb [698]
    - Punching Gloves [699]
    - Covert Cloak [700]
    - Loaded Dice [701]
    - Fairy Feather [702]
  - Apart from adding the Infinite Rare Candy [622] and Infinite Repel [79] it now allows the player to register those items

### Work In Progress
  - Finish Item expansion

## How to Install
  - Install [CTRMap-Community Edition](https://github.com/kingdom-of-ds-hacking/CTRMap-CE) as well as the [CTRMapV](https://github.com/kingdom-of-ds-hacking/CTRMapV) plugin
  - Install the CTRMapV plugin (without it you won't be able to open a Pokemon White 2 ROM)
  - Create an extracted folder of the contents of your ROM, then create a new project
  - Download [PMC](https://github.com/kingdom-of-ds-hacking/PMC), go to the extras tab of CTRMap and click *Install/Update PMC* (You only need to do this the first time you create a new project)
  - Make sure there is a `patches` directory at *vfs/data/patches*, if there isn't create one and move there the latest [NitroKernel.dll file](https://github.com/HelloOO7/NitroKernel/releases).
  - Move the .dll files of the patches you want to install inside the patches folder, and export the ROM with CTRMap
  - If the patch requires any other modification like changing a narc or text editing it will be covered in the notes of the patch

## Compatibility
For now all patches are independent and compatible with each other, you can combine them as you wish

## Compiling the code
If you want to change some numbers or just mess around with the code, follow this steps:
  - Download the [White 2 Development Headers](https://github.com/kingdom-of-ds-hacking/swan)
  - Install the [ARM GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
  - Move the modified .cpp and .h files to the same folder along the swan-master folder
  - Launch the cmd from that folder and run `arm-none-eabi-g++ -r -mthumb -march=armv5t -Os <patch name>.cpp -Iswan-master -o <patch name>.elf`
  - Open CTRMap and go to the Extras tab, click "Convert ELF to DLL" and select the .yml file obtained from the folder of the modified patch
  - Next on file selection screen, select the `<patch name>.elf` file you previously compiled and make sure the *install to /patches* checkbox is enabled
  - Save your project and export your ROM

## Issues Reporting/Contribute
You can message me on this discord at dararoraro
If you are having troubles compiling the code or running CTRMap, I recommend joining [this server](https://discord.gg/zAtqJDW2jC)

## Credits
* Hello007 - CTRMap and the Gen V code injection system
* PlatinumMaster - Gen V code injection system
* totally_anonymous - Battle IDB documentation
* SpagoAsparago - Template for this README
* CH340, totally_anonymous, Sunk, klark and Trusty Peaches - Aswering my questions and resolving doubts
