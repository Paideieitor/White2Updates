# White2Updates
Code Injection Patches to upgrade the Pokémon White 2 Edition gameplay. 
This code is fully compatible with the [Code Injection set up Guide](https://ds-pokemon-hacking.github.io/docs/generation-v/guides/b2w2-code_injection_set_up/).

## Features
All the features are active by default unless it is specified
To personalize the patch modify the ``settings.h`` file (I don't recomend modifying the non-boolean definitions unless you know what you are doing) and then build the DLL
### Hard Level Caps
  - Limits the maximum level a Pokémon can be
  - If the level cap variable is set to 0 the level cap is ignored (0 is the default value)
  - Rare Candies ignore the level cap, you can change that in the ``settings.h`` file
  - Set the game variable 16415 to the level cap you want using CTRMap or some other editor:
    - To change the levelcap use the command WorkSet ``EventWorks.Set(16415, <new level cap>)`` in a script
### Disable EVs
  - Removes the gain of EVs from defeating Pokémons, independetly of the EV gain value in the Personal NARC
  - Does NOT prevent the gain of EVs through EV items like *Protein*
### Infinite Rare Candy Item
  - Allows you to level up a Pokémon until the level cap (100 by default).
  - Make sure to read the ``NARCS/README.md`` to know which files you need to use
  - Respects the hard level cap if that patch is active
  - The default item ID for this is 622, you can change this in the ``settings.h`` file
### Infinite Repel Item
  - Allows you to toggle the repel effect on and off
  - Make sure to read the ``NARCS/README.md`` to know which files you need to use
  - Removes the functionality of regular repel effect items like Super Repel
  - The default item ID for this is 623, you can change this in the ``settings.h`` file
### Repel Effect Removes Shaking Grass
  - Removes the chance of a shaking grass or a fishing spot appearing while the repel effect is active
### Dynamic Speed
  - Instead of calculating action order at the begining of every turn, the game updates it depending on the most actualized speed stats and priority
  - Fully functional with speed/priority items and the moves After You and Quash
### Gen6 crit changes
  - Crits damage boost updated to Gen6 (1.5 multiplier instead of doubling the damage)
  - Crit stages updated to Gen6
### Gen6 weather abilities
  - Weather triggered by abilites like Drizzle now last 5 turns
  - You can change the value in ``settings.h``
### Ability Expansion (Up to Gen7)
  - Every single ability from Gen6 and Gen7 has been added to the battle engine
  - Make sure to read the ``NARCS/README.md`` to know which files you need to use
  - To change a pokemon ability to an expanded just modify the ability ID in that particular Pokémon Personal NARC (the ability IDs are preservet from vanilla games, but you can check them in ``BattleUpgrade.h``)
  - Big shoutout to [Spago](https://github.com/SpagoAsparago) who made the first Gen6 ability expansion, this patch adds the abilities that where missing from his patch and attempts to fix some bugs and inconsistencies with the vanilla mechanics

## How to Install
### Installing tools
  - Set up a *Pokémon White 2 Edition USA* CTRMap project following this [guide](https://ds-pokemon-hacking.github.io/docs/generation-v/guides/bw_b2w2-using_ctrmap/)
  - Download [PMC](https://github.com/kingdom-of-ds-hacking/PMC), go to the extras tab of CTRMap and click *Install/Update PMC* (You only need to do this the first time you create a new project)
  - Make sure there is a `patches` directory at *vfs/data/patches*, if there isn't create one
  - Move ``White2Updates.dll`` in the *vfs/data/patches* folder
  - Make sure to read the ``NARCS/README.md`` to know which files you need to use
  - Create a ROM clicking ``File -> Export ROM`` or using the shortcut ``Ctrl + E``

### Building the DLL
If you have followed the [Code Injection set up Guide](https://ds-pokemon-hacking.github.io/docs/generation-v/guides/b2w2-code_injection_set_up/) you will already have all the tools you need to build, but if you havent
  - Set up a *Pokémon White 2 Edition USA* CTRMap project following this [guide](https://ds-pokemon-hacking.github.io/docs/generation-v/guides/bw_b2w2-using_ctrmap/)
  - Download [PMC](https://github.com/kingdom-of-ds-hacking/PMC), go to the extras tab of CTRMap and click *Install/Update PMC* (You only need to do this the first time you create a new project)
  - Make sure there is a `patches` directory at *vfs/data/patches*, if there isn't create one
  - Install the [ARM GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
Once you have set up:
  - Open ``buildDLL.bat`` with your prefered text editor
  - Change the *CTRMAP* variable to the location of ``CTRMap.jar`` in your computer
  - Change the *OUTPUT_DIR* variable to the path where you want the DLL to be moved, I recommend putting it directly in the ``patches`` folder of your CTRMap project
  - Save the changes you just made
  - Double click the ``buildDLL.bat`` file and a command prompt window should appear (if any errors pop up you can contact me through Discord, @dararoraro)
  - ``White2Updates.dll`` should be in the output folder you selected
  - If it is not already, move ``White2Updates.dll`` in the ``vfs/data/patches`` folder of your CTRMap project
  - Create a ROM clicking ``File -> Export ROM`` or using the shortcut ``Ctrl + E``

## Reporting Issues/Contributing
You can message me through Discord, @dararoraro
If you are having troubles building the patches or running CTRMap, I recommend joining [Kingdom of DS Hacking](https://discord.gg/zAtqJDW2jC) and the [DS Modding Community](https://discord.gg/YBtdN3aXfv)

## Credits
* Hello007 - CTRMap and the Gen V code injection system
* PlatinumMaster - Gen V code injection system
* totally_anonymous - IDB documentation
* SpagoAsparago - Original Gen6 ability expansion
* CH340, totally_anonymous, Sunk, klark, Trusty Peaches and Brom - Aswering my questions, resolving doubts and hanging around
