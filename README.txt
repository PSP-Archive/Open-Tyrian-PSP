TYRIAN PSP v1.0
---------------

This is a PSP adoptation of TYRIAN, PC game by Eclipse Productions, published Epic MegaGames.
The original game was programmed by Jason Emery, illustrated by Daniel Cook, and its music composed 
by Alexander Brandon and Andreas Molnar.

This port is based on OpenTyrian project code (http://code.google.com/p/opentyrian/), maintained by
mindless2112, syntaxglitch,emacs.hacker, yuriks.br.

GAMEPLAY
--------
Tyrian is an arcade-style vertical scrolling shooter.
The player controls a space ship fitted with different weapons and enhanced energy shields. 
Money is earned by destroying enemies and grabbing bonuses, which is then used to purchase 
upgrades such as weapons, shields, energy generators, and different ships from interlevel menus. 
The game is fast paced and presents a variety of enemies and bosses.

CONTROLS
--------
[ANALOG PAD]          MENU NAVIGATION & SHIP'S IN GAME MOVEMENTS
[TRANGLE]             SELECT/FIRE
[SQUARE]/[O][X]       joystick buttons 2,3,4 (can be reconfigured in setup)
[SELECT]              in-game setup menu / back button
[START]               in game pause
[R_TRIGGER]	      Bring up custom OSK; enter a letter.

INSTALLATION
------------
This game requires homebrew-friendly firmware (1.0, 1.5, M33 X.XX, etc)
Copy contents of TYRIAN folder in to /PSP/GAME (or /PSP/GAMEXXX, depending on your firmware).

1.5 FW users must 'kexploit" EBOOT.PBP from OpenTyrian_PSP_09 patch and overwrite the existing file in the 1.0 disctribution.



KNOWN ISSUES
------------
- Network play is not yet implemented
- 2 player mode buttons are not mapped


UPDATE HISTORY
--------------
Update: [01/18/08] OpenTyrian_PSP_1_0 released.
- based on latest (689) revision of OpenTyrian
- recompiled for 3.xx custom firmware / slim PSP

Update: [01/04/08] OpenTyrian_PSP_09 patch released.
- music volume increased
- Christmas mode activation dialog added
- based on the latest (674) revision of OpenTyrian
(Download via comments)

Update: [12/27/07] OpenTyrian_PSP_08 patch released.
- some tweaks in music emulation code resulting some speedup
- based on the latest (647) revision of OpenTyrian
- better OSK support

Update: [12/12/07] OpenTyrian_PSP_07 patch released.
- basic OSK support. Hit [Right Trigger] to enter key entry mode. Use UP/DOWN to change the character, then press Right Trigger again to exit.
Now you should be able to access super arcade modes and some additional texture modes..

Update: [12/06/07] OpenTyrian_PSP_06 patch released. (download via comments)
- ~10% speed increase due to new rendering routine.
- better smoothing/scaling in full-screen mode
-original size mode fixed
- data cubes text scroll fixed
- black screen on startup bug hopefully gone
- fixed the startup title screen animations
- slightly better key handling in main menus.. still needs some work
To install, just overwrite the EBOOT.PBP from 0.5 version with the patched one

Enjoy,

DENISKA