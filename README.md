# White2Updates
Code Injection Patches to upgrade the White 2 gameplay. This code is fully compatible with the [Code Injection set up Guide](https://ds-pokemon-hacking.github.io/docs/generation-v/guides/b2w2-code_injection_set_up/), using a different environment will probably require some changes to the code.

## Features
### Hard Level Caps
  - Limits the maximum level a pokemon can be
  - If the level cap variable is set to 0 the level cap is ignored (0 is the default value)
  - Set the game variable 16415 to the level cap you want using CTRMap or some other editor:
    - To do that use the command WorkSet ```EventWorks.Set(16415, <new level cap>)``` in the script that should change the levelcap
### Dynamic Speed
  - Instead of calculating action order at the begining of every turn, the game updates it depending on the most actualized speed stats and priority
  - Fully functional with speed/priority items and the moves After You and Quash
### Gen6 crit changes
  - Crits do 1.5 times the damage instead of doubling it
  - Crit stages updated

## How to Install
### Installing tools
  - Install [CTRMap-Community Edition](https://github.com/kingdom-of-ds-hacking/CTRMap-CE) as well as the [CTRMapV](https://github.com/kingdom-of-ds-hacking/CTRMapV) plugin
  - Install the CTRMapV plugin (without it you won't be able to open a Pokemon White 2 ROM)
  - Create an extracted folder of the contents of your ROM, then create a new project
  - Download [PMC](https://github.com/kingdom-of-ds-hacking/PMC), go to the extras tab of CTRMap and click *Install/Update PMC* (You only need to do this the first time you create a new project)
  - Make sure there is a `patches` directory at *vfs/data/patches*, if there isn't create one
  - Move any patch(DLL file) you want to install in the *vfs/data/patches* folder
  - Create a ROM clicking ``File -> Export ROM`` or using the shortcut ``Ctrl + E``

### Compiling the code
Before compiling make sure to open the ``settings.h`` file with your prefered text editor, here you can modify the patches you will be installing, I don't recomend modifying the non-boolean definitions unless you know what you are doing.
To install the patches you will need to compile the 4 CPP files in the pathces folder, follow this steps:
  - If you don't have a Code Injection environment follow the [Code Injection set up Guide](https://ds-pokemon-hacking.github.io/docs/generation-v/guides/b2w2-code_injection_set_up/)
  - Move the source code in *compile/source* of this repository to your environment
  - Open a cmd terminal (if you are in Windows go to the path input box, type "cmd" and click Enter)
  - Input the following commands (substitute the parts bettwen [] with the path in your computer):
    - ``arm-none-eabi-g++ [Patches path]/BattleUpgrade.cpp -I [swan path] -I [NitroKernel/include path] -o [Output path]/BattleUpgradeelf -r -mthumb -march=armv5t -Os``
  - You should have now an ELF file, open CTRMap and go to the ``Extras`` tab and do the following:
    - Make sure the *install to /patches* checkbox is enabled
    - ``LShift + Click`` on the *Convert ELF to DLL* button
    - Select the ESDB file that you can find in the *compile* folder of this repository
    - Select the ELF file you want to compile
    - Choose a name for the DLL file
  - If you didn't enable the *install to /patches* checkbox you will have to move the DLL files to the *vfs/data/patches* folder in your CTRMAP project
  - Once everything is done create a ROM clicking ``File -> Export ROM`` or using the shortcut ``Ctrl + E``

## Issues Reporting/Contribute
You can message me on this discord at dararoraro
If you are having troubles compiling the code or running CTRMap, I recommend joining [Kingdom of DS Hacking](https://discord.gg/zAtqJDW2jC) and the [DS Modding Community](https://discord.gg/YBtdN3aXfv )

## Credits
* Hello007 - CTRMap and the Gen V code injection system
* PlatinumMaster - Gen V code injection system
* totally_anonymous - IDB documentation
* SpagoAsparago - Template for this README
* CH340, totally_anonymous, Sunk, klark, Trusty Peaches and Brom - Aswering my questions and resolving doubts
