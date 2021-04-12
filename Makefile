TARGET = opentyrian
PSPSDK=$(shell psp-config --pspsdk-path)
PSPBIN = $(PSPSDK)/../bin
SDL_CONFIG = $(PSPBIN)/sdl-config
PSP_EBOOT_PIC1 = PIC1.png
PSP_EBOOT_ICON = ICON0.png

PSP_FW_VERSION=371
BUILD_PRX = 1

OBJS = animlib.o backgrnd.o config.o destruct.o editship.o episodes.o error.o fm_synth.o fmopl.o fonthand.o helptext.o joystick.o jukebox.o keyboard.o lds_play.o loudness.o lvllib.o lvlmast.o mainint.o musmast.o network.o newshape.o nortsong.o nortvars.o opentyr.o pallib.o params.o pcxload.o pcxmast.o picload.o  psp.o scroller.o setup.o shplib.o shpmast.o sndmast.o starfade.o starlib.o tyrian2.o varz.o vga256d.o


INCDIR = 
CFLAGS = -O3  -G0 -Wall -g $(shell $(SDL_CONFIG) --cflags)  -DPSP -std=c99
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -fsingle-precision-constant -mno-check-zero-division -ffast-math -funsafe-math-optimizations -fpeel-loops
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =

#LIBS = -lstdc++  -lSDL_mixer  -lvorbisidec   -lSDL_image -lpng -lz -ljpeg  -lSDL_gfx -lm -L/usr/local/pspdev/psp/lib -lSDL  -lGL -lpspvfpu -L/usr/local/pspdev/psp/sdk/lib -lpspdebug -lpspgu -lpspctrl -lpspge -lpspdisplay -lpsphprm -lpspsdk -lpsprtc -lpspaudio -lc -lpspuser -lpsputility -lpspkernel -lpspnet_inet -lpspirkeyb -lpsppower

LIBS = -lstdc++  -lSDL_mixer  -lvorbisidec   -lSDL_image -lpng -lz -ljpeg -lGL -lSDL_gfx -lm $(shell $(SDL_CONFIG) --libs) -lGL  -lpsprtc   -lpspvfpu  -lpsppower
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = opentyrian

include $(PSPSDK)/lib/build.mak
