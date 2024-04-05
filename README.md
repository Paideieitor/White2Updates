# White2Updates
Code Injection Patches to upgrade the White 2 gameplay experience while making a ROM Hack

## Patches
### Hard Level Caps
  - Limits the maximum level a pokemon can be (doesn't affect Rare Candy, use the Rare Candy Level Caps patch to add this feature)
  - Set the game variable 16462 to the level cap you want using CTRMap or some other editor, use the command WorkSet (```EventWorks.Set(16462, levelcap)```) from a script to acomplish that
  - If you are already using the variable 16462 you can change it modifying the ```LEVEL_CAP_VAR``` definition to the value you want, and compiling and installing the new patch
### Rare Candy Level Caps
  - Adds the infinite Rare Candy item, this item can't level Pokemon over the level cap
  - By default Rare Candy ignore the level cap (like in Run&Bun)
  - Change the definition of ```RARE_CANDY_IGNORE_CAP``` to ```false``` and complile to make Rare Candy respect the level cap
  - To add the infinite Rare Candy item paste the 622 file in the *vfs/data/a/0/2/4* folder in your CTRMAP project
  - This patch doesn't change the name, description and icon of the new item, so you will need to edit that to whatever you want
  - If you are already using the item 622 you will need to change the definition of ```INFINITE_CANDY_ID``` to the item ID you are repacing and also change the name of the 622 file to the new item ID
  - The level cap is shared by default with the Hard Level Caps patch
  - To change the level cap or the game variable location follow the instructions at the Hard Level Caps notes
  - As of now the infinite Rare Candy item can't be registered for fast use, looking into that
### Dynamic Speed
  - Instead of calculating action order at the begining of every turn, the game updates it depending on the most actualized speed stats and priority
  - Fully functional with speed/priority items and the moves After You and Quash
  - To detect when After You and Quash are used (without using move ID) I use the game variable 16400, if you are already using it change it at the ```ORDER_ALTERING_MOVE_VAR``` definition, and compile and install the new patch
  - I've tested several scenearios and they all work fine, if you find a bug or a missing feature let me know :)
### Work In Progress
  - Item expansion

## How to Install
  - Install [CTRMap-Community Edition](https://github.com/kingdom-of-ds-hacking/CTRMap-CE) as well as the [CTRMapV](https://github.com/kingdom-of-ds-hacking/CTRMapV) plugin
  - Install the CTRMapV plugin (without it you won't be able to open a Pokemon White 2 ROM)
  - Create an extracted folder of the contents of your ROM, then create a new project
  - Download [PMC](https://github.com/kingdom-of-ds-hacking/PMC), go to the extras tab of CTRMap and click *Install/Update PMC* (You only need to do this the first time you create a new project)
  - Make sure there is a `patches` directory at *vfs/data/patches*, if there isn't create one and move there the latest [NitroKernel.dll file](https://github.com/HelloOO7/NitroKernel/releases).
  - Move the .dll files of the patches you want to install inside the patches folder, and export the ROM with CTRMap
  - If the patch requires any other modification like changing a narc or text editing it will be covered in the notes of the patch

## Compatibility
For now all patches are compatible with each other

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
