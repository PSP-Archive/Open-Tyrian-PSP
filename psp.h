#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <psppower.h>
#include <pspgu.h>
#include "SDL.h"

SceCtrlData cpad;

//#define printf pspDebugScreenPrintf 

#define SLICE_SIZE 64 // change this to experiment with different page-cache sizes
#define RGB(r,g,b) (short)((((b>>3) & 0x1F)<<10)|(((g>>3) & 0x1F)<<5)|(((r>>3) & 0x1F)<<0)|0x8000)
#define pg_vramtop ((char *)0x04000000)
unsigned short __attribute__((aligned(16))) pixels[512*272];

struct Vertex
{
        unsigned short u, v;
        unsigned short color;
        short x, y, z;
};


struct Vertex* vertices;

void clearScreen(long color);
void guStart();
void guInit(void);
void guBlit ();
void copyScreen(SDL_Surface *s);
void pspFlip(SDL_Surface *s);
void pspJoystick(Sint16 * joyX, Sint16 * joyY);
void pspGetOSK();
void clearKeys(Uint8 *keysactive);
