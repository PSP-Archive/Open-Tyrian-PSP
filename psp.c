#include "opentyr.h"
#include "vga256d.h"
#include "config.h" // For fullscreen stuff
#include "sndmast.h"

#include "SDL.h"

unsigned int __attribute__((aligned(16))) list[262144];

extern char curchar;

void clearKeys(Uint8 *keysactive) {
	int i;
	for (i=8; i<320; i++)
		keysactive[i]=0;
}


void clearScreen(long color) {
        memset(pixels,0x00000000,sizeof(pixels));
	int i=fullscreen_set;
	fullscreen_set=1;
	guBlit();
	guBlit();
	fullscreen_set=i;
}

void guStart() {
        sceGuStart(GU_DIRECT,list);
}

void guInit(void) {
        sceGuInit();
        guStart();
        sceGuDrawBuffer(GU_PSM_5551,(void*)0,512);
        sceGuDispBuffer(480,272,(void*)0x88000,512);
        sceGuDepthBuffer((void*)0x110000,512);
        sceGuOffset(2048 - (480/2),2048 - (272/2));
        sceGuViewport(2048,2048,480,272);
        sceGuDepthRange(0xc350,0x2710);
        sceGuScissor(0,0,480,272);
        sceGuEnable(GU_SCISSOR_TEST);
        //sceGuFrontFace(GU_CW);
        sceGuEnable(GU_TEXTURE_2D);
        //sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
        sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGB);
        sceGuTexFilter(GU_LINEAR,GU_LINEAR);
        sceGuTexMode(GU_PSM_5551,0,0,0);
        sceGuTexScale(1,1);
        sceGuTexOffset(0.0f,0.0f);
        sceGuAmbientColor(0x000000);
        sceGuFinish();
        sceGuSync(0,0);

        sceDisplayWaitVblankStart();
        sceGuDisplay(1);
}

int j;
float ratio=480.0/320.0;

int frame=0;
void guBlit () {
        //sceKernelDcacheWritebackAll();
        guStart();

        sceGuTexImage(0,512,512,512,pixels);
        //sceGuTexFlush();
        for (j = 0; (j+SLICE_SIZE) <= 320; j = j+SLICE_SIZE) {
                vertices = (struct Vertex*)sceGuGetMemory(2 * sizeof(struct Vertex));
                if (fullscreen_set==1) {
                        vertices[0].x = j*ratio; vertices[0].y = 0; vertices[0].z = 0;
                        vertices[1].x = (j+SLICE_SIZE)*ratio; vertices[1].y = 272; vertices[1].z = 0;

                } else {
                        vertices[0].x = j+80; vertices[0].y = 36; vertices[0].z = 0;
                        vertices[1].x = (j+SLICE_SIZE)+80; vertices[1].y = 236; vertices[1].z = 0;
                }
                vertices[0].u = j; vertices[0].v = 0;
                vertices[1].u = j+SLICE_SIZE; vertices[1].v = 200;

                sceGuDrawArray(GU_SPRITES,GU_TEXTURE_16BIT|GU_COLOR_5551|GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vertices);

         }

        sceGuFinish();
        sceGuSync(0,0);
        //sceDisplayWaitVblankStart();
        sceGuSwapBuffers();
}


void copyScreen(SDL_Surface *s) {
        int i,j;
        int x=0;
        int y=0;
        Uint8 index;
        SDL_Color color;
	frame++;
        for (i=0; i<200; i++) {
                for (j=0; j<263+(frame%2)*57; j++) {
                        index= *(Uint8 *) (&s->pixels[j+y]);
                        color= (s->format->palette->colors[index]);
                        pixels[x+j]=RGB(color.r,color.g,color.b);
                }
                x+=512;
                y+=320;
        }
}

void pspFlip(SDL_Surface *s) {
	copyScreen(s);
       	guBlit();
}

void pspJoystick(Sint16 * joyX, Sint16 * joyY) {
	*joyY=0;
        *joyX=0;
        sceCtrlPeekBufferPositive(&cpad,1);
        if (cpad.Buttons & PSP_CTRL_UP ||  cpad.Ly <= 0x30)
                *joyY = -3000;
        if (cpad.Buttons & PSP_CTRL_DOWN ||  cpad.Ly >= 0xD0)
                *joyY = 3000;
        if (cpad.Buttons & PSP_CTRL_LEFT ||  cpad.Lx <= 0x30)
                *joyX = -3000;
        if (cpad.Buttons & PSP_CTRL_RIGHT ||  cpad.Lx >= 0xD0)
                *joyX = 3000;
}


void getOSK() {
                JE_playSampleNum(CLICK);
                SceCtrlData cpad;
		pspDebugScreenInit();
                pspDebugScreenSetBackColor(0x00000000);
                pspDebugScreenClear();
                sceKernelDelayThread(500000);
                pspDebugScreenSetBackColor(0x00551111);
                pspDebugScreenPrintf("\n\n\t\t\t\t\tONSCREEN KEYBOARD\n\n\n\t\tUSE UP & DOWN ARROWS TO CHANGE THE LETTER,\n\n\t\t\t THEN HIT RIGHT TRIGGER\n\n\n");
                pspDebugScreenSetBackColor(0x00330000);
		pspDebugScreenPrintf("\n\n\n\n\n");
                sceCtrlPeekBufferPositive(&cpad,1);
                while (!(cpad.Buttons & PSP_CTRL_RTRIGGER)) {
                        pspDebugScreenSetXY(0,13);
                        pspDebugScreenPrintf("==> %c", curchar);
			sceGuSwapBuffers();
		
                        if (cpad.Buttons & PSP_CTRL_UP||  cpad.Ly <= 0x30) {
                                curchar++;
                                JE_playSampleNum(CURSOR_MOVE);
                                sceKernelDelayThread(130000);
                        }
                        if (cpad.Buttons & PSP_CTRL_DOWN || cpad.Ly >= 0xD0) {
                                curchar--;
                                JE_playSampleNum(CURSOR_MOVE);
                                sceKernelDelayThread(130000);
                        }
                sceCtrlPeekBufferPositive(&cpad,1);

                }
                JE_playSampleNum(SELECT);
		guInit();
		clearScreen(0x00000000);
                sceKernelDelayThread(90000);
}

