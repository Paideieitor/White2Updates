# White2Updates
Code Injection Patches to upgrade the White 2 gameplay experience while making a ROM Hack

## Patches
### Hard Level Caps
  - Limits the maximum level a pokemon can be (doesn't affect Rare Candy yet)
  - To change this caps modify the LEVEL_CAPS definition in exp_cap.cpp, don't forget to modify LEVEL_CAP_AMOUNT acording to the number of entries in LEVEL_CAPS
  - To increase the level cap in-game you will need to modify the game variable at the LVL_CAP_VAR index using CTRMap or some other editor (untested, pretty sure it works... 65% sure)
### Dynamic Speed
  - Instead of calculating action order at the begining of every turn, the game updates it depending on the most actualized speed stats and priority
  - I have tested several scenarios where this is relevant
  - Doesn`t work with after you and quash

## How to Install
  - Install [CTRMap-Community Edition](https://github.com/kingdom-of-ds-hacking/CTRMap-CE) as well as the [CTRMapV](https://github.com/kingdom-of-ds-hacking/CTRMapV) plugin
  - Install the CTRMapV plugin (without it you won't be able to open a Pokemon White 2 ROM)
  - Create an extracted folder of the contents of your ROM, then create a new project
  - Download [PMC](https://github.com/kingdom-of-ds-hacking/PMC), go to the extras tab of CTRMap and click *Install/Update PMC* (You only need to do this the first time you create a new project)
  - Make sure there is a `patches` directory at *vfs/data/patches*, if there isn't create one and move there the latest [NitroKernel.dll file](https://github.com/HelloOO7/NitroKernel/releases).
  - Move the .dll files of the patches you want to install inside the patches folder, and export the ROM with CTRMap

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
