# White2Updates
Code Injection Patches to upgrade the White 2 gameplay

## Patches
### Hard Level Caps
  - Limits the maximum level a pokemon can be (doesn't affect Rare Candy, use the Rare Candy Level Caps patch to add this feature)
  - By default Rare Candy ignore the level cap (like in Run&Bun)
  - Set the game variable 16415 to the level cap you want using CTRMap or some other editor, use the command WorkSet (```EventWorks.Set(16415, levelcap)```) from a script to acomplish that
### Infinite Rare Candy [622]
  - Adds the Infinite Rare Candy as a registrable Key Item, this item can't level Pokemon over the level cap
  - To change the level cap or the game variable location follow the instructions at the Hard Level Caps notes (this is independent of you having or not the Hard Level Caps patch installed)
### Infinite Repel [79]
  - Adds the Infinite Repel as a registrable Key Item, this can be toggled to activate or desactivate the effect of a Repel
  - The repel being active disables shaking grass and surfing spots from appearing by default
  - This patch makes all repel items exept the Infinite Repel useles
### Dynamic Speed
  - Instead of calculating action order at the begining of every turn, the game updates it depending on the most actualized speed stats and priority
  - Fully functional with speed/priority items and the moves After You and Quash
  - I've tested several scenearios and they all work fine, if you find a bug or a missing feature let me know :)
### Item Expansion

Added Items:
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
  - Heavy-Duty Boot [666]
  - Blunder Policy [667] (visual bug with the icon)
  - Room Service [668]
  - Utility Ubrella [669]
  - All Mints [670 - 690] (ordered the same way as the vanilla games)
  - Ability Patch [691]
  - Ability Shield [696] (visual bug with the icon)
  - Clear Amulet [697]
  - Mirror Herb [698]
  - Punching Gloves [699]
  - Covert Cloak [700]
  - Loaded Dice [701]
  - Fairy Feather [702]

Known Issues:
  - The new items can not be moved to the free space bag for now
  - Some item IDs cause the game to crash, so make sure to not use item IDs (654 - 664) and (692 - 695), there might be other crash IDs
  - Eject Pack interactions with pursuit are untested, it's likely to work like with U-Turn or Volt Switch

### Work In Progress
  - Free space expansion

## How to Install
### Installing tools
  - Install [CTRMap-Community Edition](https://github.com/kingdom-of-ds-hacking/CTRMap-CE) as well as the [CTRMapV](https://github.com/kingdom-of-ds-hacking/CTRMapV) plugin
  - Install the CTRMapV plugin (without it you won't be able to open a Pokemon White 2 ROM)
  - Create an extracted folder of the contents of your ROM, then create a new project
  - Download [PMC](https://github.com/kingdom-of-ds-hacking/PMC), go to the extras tab of CTRMap and click *Install/Update PMC* (You only need to do this the first time you create a new project)
  - Make sure there is a `patches` directory at *vfs/data/patches*, if there isn't create one and move there the latest [NitroKernel.dll file](https://github.com/HelloOO7/NitroKernel/releases).
### Compiling the code
Before compiling make sure to open the ``settings.h`` file with your prefered text editor, here you can modify the patches you will be installing, I don't recomend modifying the non-boolean definitions unless you know what you are doing.
To install the patches you will need to compile the 4 CPP files in the pathces folder, follow this steps:
  - Install the [ARM GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
  - Download the [White 2 Development Headers](https://github.com/kingdom-of-ds-hacking/swan) , this will be refered as W2DH from now.
  - Move the `custom` folder inside the folder of the W2DH (as a folder).
  - Open a cmd terminal (if you are in Windows go to the W2DH folder,  click the path input box, type "cmd" and click Enter)
  - Input the following commands (substitute the parts bettwen [] with the path in your computer):
    - ``arm-none-eabi-g++ -r -mthumb -I [W2DH path] -march=armv5t -Os [Patches path]/patch_bag.cpp -o patch_bag.elf``
    - ``arm-none-eabi-g++ -r -mthumb -I [W2DH path] -march=armv5t -Os [Patches path]/patch_battle.cpp -o patch_battle.elf``
    - ``arm-none-eabi-g++ -r -mthumb -I [W2DH path] -march=armv5t -Os [Patches path]/patch_field.cpp -o patch_field.elf``
    - ``arm-none-eabi-g++ -r -mthumb -I [W2DH path] -march=armv5t -Os [Patches path]/patch_pokelist.cpp -o patch_pokelist.elf``
  - You should have now 4 ELF files named like the patches, open CTRMap and go to the Extras tab and do the following for each of the files:
    - Make sure the *install to /patches* checkbox is enabled
    - ``LCtrl + LShift + Click`` on the "Convert ELF to DLL" button
    - Select a YML file from the ESDBs folder
    - Select the ELF file that correspond to the YML file you selected
    - Create the DLL file
  - Once you have done this with the 4 patches Save your project
  - If you didn't enable the *install to /patches* checkbox you can move the .dll files of the patches you want to install inside the patches folder
### Modifying NARCs
Some patches need modified narcs either for visual effects or to not crash the game:
  - Hard Level Caps: No modifications needed
  - Infinite Rare Candy: a/0/0/2 modification needed (files 63, 64, 481 and 482)
  - Infinite Repel: a/0/0/2 modification needed (files 63, 64, 481 and 482)
  - Dynamic Speed: No modifications needed
  - Item Expansion: all modifications needed (these contain the changes for Infinite Rare Candy and Infinite Repel, so you will have to manualy change the files 63, 64, 481 and 482 with CTRMap if you want the Item Expansion with out those features)
To apply this modifications follow this steps (I will use the a002 folder as an example, but all other folders work the same):
  - The name of each folder points to a specific NARC file in the game, so a002 is the a/0/0/2 NARC
  - Once you know the NARC go to the folder of the CTRMap Project and go to the path [CTRMap Project path]/vfs/[narc], in this case [CTRMap Project path]/vfs/a/0/0/2
  - If a folder doesn't exist create it yourself
  - Once in the final folder paste all the files of the original folder in NARC Changes
### Final Steps
Once everything is done you can go to File -> Export ROM and save your new ROM

## Compatibility
For now all patches are independent and compatible with each other, you can combine them as you wish

## Issues Reporting/Contribute
You can message me on this discord at dararoraro
If you are having troubles compiling the code or running CTRMap, I recommend joining [this server](https://discord.gg/zAtqJDW2jC)

## Credits
* Hello007 - CTRMap and the Gen V code injection system
* PlatinumMaster - Gen V code injection system
* totally_anonymous - IDB documentation
* SpagoAsparago - Template for this README
* CH340, totally_anonymous, Sunk, klark and Trusty Peaches - Aswering my questions and resolving doubts
