/* vim: set noet:
 *
 * OpenTyrian Classic: A modern cross-platform port of Tyrian
 * Copyright (C) 2007  The OpenTyrian Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include "opentyr.h"

#include "config.h"
#include "episodes.h"
#include "error.h"
#include "fonthand.h"
#include "helptext.h"
#include "joystick.h"
#include "jukebox.h"
#include "keyboard.h"
#include "loudness.h"
#include "mainint.h"
#include "musmast.h"
#include "network.h"
#include "newshape.h"
#include "nortsong.h"
#include "nortvars.h"
#include "pallib.h"
#include "params.h"
#include "picload.h"
#include "scroller.h"
#include "setup.h"
#include "shpmast.h"
#include "starfade.h"
#include "tyrian2.h"
#include "varz.h"
#include "vga256d.h"

#include "SDL.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


const JE_byte shapereorderlist[7] = {1, 2, 5, 0, 3, 4, 6};

const char *opentyrian_str = "OpenTyrian";
const char *opentyrian_menu_items[] =
{
	"About OpenTyrian",
	"Fullscreen",
	/* "Play Destruct", */
	"Jukebox",
	"Return to Main Menu"
};

/* zero-terminated strncpy */
char *strnztcpy( char *to, char *from, size_t count )
{
	to[count] = '\0';
	return strncpy(to, from, count);
}

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
/* endian-swapping fread */
size_t efread( void *buffer, size_t size, size_t num, FILE *stream )
{
	size_t i, f = fread(buffer, size, num, stream);

	switch (size)
	{
		case 2:
			for (i = 0; i < num; i++)
			{
				((Uint16 *)buffer)[i] = SDL_Swap16(((Uint16 *)buffer)[i]);
			}
			break;
		case 4:
			for (i = 0; i < num; i++)
			{
				((Uint32 *)buffer)[i] = SDL_Swap32(((Uint32 *)buffer)[i]);
			}
			break;
		case 8:
			for (i = 0; i < num; i++)
			{
				((Uint64 *)buffer)[i] = SDL_Swap64(((Uint64 *)buffer)[i]);
			}
			break;
		default:
			break;
	}

	return f;
}

/* endian-swapping fwrite */
size_t efwrite( void *buffer, size_t size, size_t num, FILE *stream )
{
	void *swap_buffer;
	size_t i, f;

	switch (size)
	{
		case 2:
			swap_buffer = malloc(size * num);
			for (i = 0; i < num; i++)
			{
				((Uint16 *)swap_buffer)[i] = SDL_SwapLE16(((Uint16 *)buffer)[i]);
			}
			break;
		case 4:
			swap_buffer = malloc(size * num);
			for (i = 0; i < num; i++)
			{
				((Uint32 *)swap_buffer)[i] = SDL_SwapLE32(((Uint32 *)buffer)[i]);
			}
			break;
		case 8:
			swap_buffer = malloc(size * num);
			for (i = 0; i < num; i++)
			{
				((Uint64 *)swap_buffer)[i] = SDL_SwapLE64(((Uint64 *)buffer)[i]);
			}
			break;
		default:
			swap_buffer = buffer;
			break;
	}

	f = fwrite(swap_buffer, size, num, stream);

	if (swap_buffer != buffer)
	{
		free(swap_buffer);
	}

	return f;
}
#endif

void opentyrian_menu( void )
{
	int sel = 0;
	int maxSel = COUNTOF(opentyrian_menu_items) - 1;
	bool quit = false, fade_in = true;

	JE_fadeBlack(10);
	JE_loadPic(13, false); /* 2, 5, or 13? */

	JE_outTextAdjust(JE_fontCenter(opentyrian_str, FONT_SHAPES), 5, opentyrian_str, 15, -3, FONT_SHAPES, false);

	memcpy(VGAScreen2->pixels, VGAScreen->pixels, VGAScreen2->pitch * VGAScreen2->h);

	JE_showVGA();

	if (currentJukeboxSong == 0) currentJukeboxSong = 37; /* A Field for Mag */
	JE_playSong(currentJukeboxSong);

	do
	{
		memcpy(VGAScreen->pixels, VGAScreen2->pixels, VGAScreen->pitch * VGAScreen->h);
		for ( int i = 0; i <= maxSel; i++)
		{
			const char *text = opentyrian_menu_items[i];
			if (i == 1) /* fullscreen */
			{
				static char buf[12+3+17+1];
				snprintf(buf, sizeof(buf), "Fullscreen: %s%s",
				         (fullscreen_set ? "On" : "Off"),
				         (fullscreen_set != fullscreen_enabled ? " -Please restart-" : ""));
				text = buf;
			}

			JE_outTextAdjust(JE_fontCenter(text, SMALL_FONT_SHAPES),
			                 (i != maxSel) ? (i * 16 + 32) : 118, text,
			                 15, (i != sel ? -4 : -2), SMALL_FONT_SHAPES, true);
		}

		JE_showVGA();

		if (fade_in)
		{
			fade_in = false;
			JE_fadeColor(20);
			wait_noinput(true, false, false);
		}

		tempW = 0;
		JE_textMenuWait(&tempW, false);

		if (newkey) {
			switch (lastkey_sym)
			{
				case SDLK_UP:
					sel--;
					if (sel < 0)
					{
						sel = maxSel;
					}
					JE_playSampleNum(CURSOR_MOVE);
					break;
				case SDLK_DOWN:
					sel++;
					if (sel > maxSel)
					{
						sel = 0;
					}
					JE_playSampleNum(CURSOR_MOVE);
					break;
				case SDLK_RETURN:
					switch (sel)
					{
						case 0: /* About */
							JE_playSampleNum(SELECT);
							scroller_sine(about_text);
							memcpy(VGAScreen->pixels, VGAScreen2->pixels, VGAScreen->pitch * VGAScreen->h);
							JE_showVGA();
							fade_in = true;
							break;
						case 1: /* Fullscreen */
#ifdef PSP
							guBlit();
                                                        clearScreen(0x11111111);
                                                        guBlit();
#endif
							fullscreen_set = !fullscreen_set;
							JE_playSampleNum(SELECT);
							break;
						case 2: /* Jukebox */
							JE_playSampleNum(SELECT);
							JE_jukeboxGo();
							memcpy(VGAScreen->pixels, VGAScreen2->pixels, VGAScreen->pitch * VGAScreen->h);
							JE_showVGA();
							fade_in = true;
							break;
						default: /* Return to main menu */
							quit = true;
							JE_playSampleNum(ESC);
							break;
					}
					break;
				case SDLK_ESCAPE:
					quit = true;
					JE_playSampleNum(ESC);
					return;
				default:
					break;
			}
		}
	} while (!quit);
}
#ifndef PSP
int main( int argc, char *argv[] )
#else
PSP_HEAP_SIZE_KB(19000);
int main( int argc, char *argv[] )
#endif

{
	srand(time(NULL));
#ifdef PSP
	pspDebugScreenInit();
	scePowerSetClockFrequency(333,333,166);
#endif

	if (SDL_Init(0))
	{
		printf("Failed to initialize SDL: %s\n", SDL_GetError());
	}

	/* JE_detectCFG(); YKS: Removed */

	printf("\nWelcome to... >> OpenTyrian v0.1 <<\n\n");

	printf("Copyright (C) 2007 The OpenTyrian Development Team\n\n");

	printf("This program comes with ABSOLUTELY NO WARRANTY.\n");
	printf("This is free software, and you are welcome to redistribute it\n");
	printf("under certain conditions.  See the file GPL.txt for details.\n\n");

	JE_scanForEpisodes();

	recordFileNum = 1;
	playDemoNum = 0;
	playDemo = false;

	JE_loadConfiguration();

	JE_initVGA256();
	init_keyboard();

	/* TODO: Tyrian originally checked availible memory here. */

	JE_paramCheck(argc, argv);

	if (scanForJoystick)
	{
		JE_joystickInit();
		if (joystick_installed)
		{
			printf("Joystick detected. %d %d\n", jCenterX, jCenterY);
		} else {
			printf("No joystick found.\n");
		}
	} else {
		printf("Joystick override.\n");
		joystick_installed = false;
	}

	if (mouseInstalled)
	{
		printf("Mouse Detected.   ");
		if (mouse_threeButton)
		{
			printf("Mouse driver reports three buttons.");
		}
		printf("\n");
	} else {
		printf("No mouse found.\n");
	}

	if (tyrianXmas)
	{
		if (JE_getFileSize("tyrianc.shp") == 0)
		{
			tyrianXmas = false;
		}
		/*if (JE_getFileSize("voicesc.shp") == 0) tyrianXmas = false;*/
#ifndef TARGET_GP2X
		if (tyrianXmas)
		{
#ifdef PSP
			pspDebugScreenClear();
			pspDebugScreenPrintf("*****************************\n"
			       "Christmas has been detected.\n"
			       "   Press [O] to Activate\n"
			       "   Press [X] to Cancel\n"
			       "*****************************\n");
			wait_input(true, false, false);
			if (lastkey_sym != SDLK_SPACE)

#else
			printf("*****************************\n"
			       "Christmas has been detected.\n"
			       "  Activate Christmas? (Y/N)\n"
			       "*****************************\n");
			wait_input(true, true, true);
			if (lastkey_sym != SDLK_y)
#endif
			{
				tyrianXmas = false;
			}
		} else {
			printf("Christmas is missing.\n");
		}
#endif /*TARGET_GP2X*/
	}

	/* Default Options */
	youAreCheating = false;
	smoothScroll = true;
	showMemLeft = false;
	playerPasswordInput = true;

	printf("Initializing SDL audio...\n");
	JE_loadSong(1);

	if ( noSound ) /* TODO: Check if sound is enabled, handle either appropriately */
	{
		/* TODO: Do we actually need to do anything here? */
		/* JE_initialize(0, 0, 0, 0, 0); */
	} else {
		/* SYN: This code block doesn't really resemble the original, because the
		    underlying sound code is very different. I've left out some stuff that
		    checked hardware values and stuff here. */

		JE_initialize(0, 0, 0, 0, 0); /* TODO: Fix arguments */

		soundEffects = true; /* TODO: find a real way to give this a value */
		if (soundEffects)
		{
			JE_multiSampleInit(0, 0, 0, 0); /* TODO: Fix arguments */

			/* I don't think these messages matter, but I'll replace them with more useful stuff if I can. */
			/*if (soundEffects == 2) printf("SoundBlaster active");
			printf ("DSP Version ***\n");
			printf ("SB port ***\n");
			printf ("Interrupt ***\n");*/

			JE_loadSndFile();
		}

	}

	if (recordDemo)
	{
		printf("Game will be recorded.\n");
	}

	megaData1 = malloc(sizeof(*megaData1));
	megaData2 = malloc(sizeof(*megaData2));
	megaData3 = malloc(sizeof(*megaData3));

	newshape_init();
	JE_loadMainShapeTables();
	/* TODO JE_loadExtraShapes;*/  /*Editship*/

	JE_loadHelpText();
	/*debuginfo("Help text complete");*/

	JE_loadPals();

	SDL_LockSurface(VGAScreen);
	JE_main();
	SDL_UnlockSurface(VGAScreen);

	JE_closeVGA256();

	return 0;
}
