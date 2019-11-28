#define ATARI atari

//#include <inttypes.h>
#include <string.h>
#include <stdio.h>
//#include <stdlib.h>
#include <osbind.h>
//#include <stdint.h>
//#include <mint/osbind.h>
#include <mint/sysbind.h>
#include "time_atari.c"
//#include <gem.h>
//#include <gemx.h>

#include "settings_atari.h"
#include "../src/bitmap.c"
#include "../src/file_read.c"
#include "../src/engine.h"
#include "draw_maps_atari.c"
#include "draw_sprite_atari.c"
#include "../src/ray_cast_calculate.c"
#include "../src/bitmap_filters.c"
#include "../src/setup_maps.c"
#include "../src/dithering.c"
#include "ray_casting_atari.c"
#include "draw_screen_atari.c"
#include "rendering_quality_atari.c"
#include "../src/map_streaming.c"
#include "input_handling_atari.c"

//http://retrospec.sgn.net/users/tomcat/miodrag/Atari_ST/Atari%20ST%20Internals.htm#SCREEN_DISPLAY
//m68k-atari-mint-gcc rough.c -o hello.tos -Ofast -std=c99 -lgem

#define COLOR0 (*(uint16_t*)0xFFFF8240UL)

#define PRINT(x) (void)Cconws(x)


uint16_t *physBase;
uint16_t *logBase;

void framebuffer_open() {
    physBase=Physbase();
    logBase=Logbase();//physBase-0x4000;
    memset(logBase,0,32000);
    memset(physBase,0,32000);
}

uint16_t *framebuffer_get_pointer() {
    return (uint16_t *)logBase;
}

void framebuffer_flip() {
    uint16_t *tmp;
    tmp=physBase;
    physBase=logBase;
    logBase=tmp;
    Setscreen(logBase,physBase,-1);
}

void framebuffer_set_palette_entry(int index,int red, int green, int blue) {
    *(uint16_t *)(0xffff8240+(index*2))=blue|(green<<4)|(red<<8);
}

//COLOR_TAB *systemPalette;

uint16_t systemPalette[16];

void Recalculate()
{
	CalculateRayCasts(rayCastXOdd, rayCastYOdd, XSIZEODD, YSIZEODD, 1);
	deltaTime = 0;
}

void SetDefaulResolution()
{
		renderingDepth = TERRAINDEPTH;
		debugValue = 8;
		debugValue2 = 2;
		debugValue3 = 2;
		debugValue4 = 1;

		debugValue6 = 4;
		xFOV = 10;
		Recalculate();
}




// struct joydecode
// {
//     char *txt;
//     uint8_t mask;
// };

// struct joydecode joydecode[] =
// {
//     {"up ",IKBD_JOY_UP},
//     {"down ",IKBD_JOY_DOWN},
//     {"left ",IKBD_JOY_LEFT},
//     {"right ",IKBD_JOY_RIGHT},
//     {"fire ",IKBD_JOY_FIRE},
// };

WORD app_handle;
WORD *wcell, *hcell, *wbox, *hbox;

void readPalette(uint16_t *systemPalette)
{
	for(int i=0;i<16;i++)
		systemPalette[i] = *(uint16_t *)(0xffff8240+(i*2));
}
void initDeltaTime()
{
	lastTime = timerGetPrec();
}
void getDeltaTime()
{
	startTime = timerGetPrec();
	deltaTime = (startTime - lastTime);
	lastTime = startTime;
	levelTime += deltaTime;
}

void main_supervisor() 
{
	int32_t key = 0;
    int i;
	readPalette(systemPalette);
	Setpalette(systemPalette);
    framebuffer_open();
    VsetMode(0x80|2|0x20);
    bitmap1 = LoadBitmapFile("data/logo1.bmp",&bitmapHeader1, bitmapPalette1);

	planes = framebuffer_get_pointer();
   
   	//process paletter from an image
	for(int i=0;i<16;i++)
	{
		bitmapPalette[i] = ((bitmapPalette1[i*4+2]>>5) << 8) +
		 ((bitmapPalette1[i*4+1]>>5) << 4) + (bitmapPalette1[i*4+0]>>5);
	}

	Setpalette(bitmapPalette);
    DrawBitmap8b(bitmap1, &bitmapHeader1);

	paletteBitmap = LoadBitmapFile("data/palette.bmp", &paletteHeader, palettePalette);

	//process paletter from an image
	for(int i=0;i<16;i++)
	{
		bitmapPalette[i] = ((palettePalette[i*4+2]>>5) << 8) +
		 ((palettePalette[i*4+1]>>5) << 4) + (palettePalette[i*4+0]>>5);
	}
	
	
	//*************************************************
		p1xf = 40*100;
		p1yf = 0;
		p1hf = 20*100;

	p2x = 0;
	p2y = 0;
	p2h = 10;

	lastOverwrittenLine = 0;


	printf("LOADING MAPS\r\n");
	SetupMaps();

	//GenerateWordDither8x8();
	//GenerateColorBytesNoDither4x4();
	printf("GENERATING DITHER\r\n");
	GenerateColorBytesDither3x2();


	printf("CALCULATING RAY TABLES\r\n");
	SetDefaulResolution();
    //*****************************************
    
	uint8_t exitflag = 0;
	uint8_t idx,joy_id,joy[2];

	IKBD_Install();
	//IKBD_MouseOff();

	//Setpalette(grayColors);
	//DrawColorMap(mapHigh);

	initDeltaTime();
	printf("last \r\n");

	Setpalette(bitmapPalette);

    while ( exitflag == 0)
    {
		
		getDeltaTime();
		OverwriteMap();
		ProcessQualityInputAtari();
		ProcessPlayerInputAtari();

		
		RenderQuality();

		//draw crosshair
		DrawPixel((160 + (cx / 150)) / 16, YSIZEODD + (cy / 100) + 4, 0);
		DrawPixel((160 + (cx / 150)) / 16, YSIZEODD + (cy / 100) - 4, 0);
            

		//IKBD_Flush();
        Vsync();
		//IKBD_ReadMouse();
		if(IKBD_Keyboard[IKBD_KEY_ESC])
		{
		    //PRINT("ESC\r\n");
			exitflag = 1;
		}
		if(IKBD_Keyboard[IKBD_KEY_UP]) p1h += 1;
		if(IKBD_Keyboard[IKBD_KEY_DOWN]) p1h -= 1;
		//if(IKBD_Keyboard[IKBD_KEY_RIGHT]) p1x += 1;
		//if(IKBD_Keyboard[IKBD_KEY_LEFT]) p1x -= 1;
		if(IKBD_Keyboard[IKBD_KEY_CONTROL]) p1y += 1;
		if(IKBD_Keyboard[IKBD_KEY_ALT]) p1y -= 1;
		
		if(IKBD_Keyboard[IKBD_KEY_SPACE])  printf("[%ul] \r\n", deltaTime);
		

		//  joy[0] = IKBD_Joystick0;
		// joy[1] = IKBD_Joystick1;

        //     for(joy_id = 0; joy_id < 2; joy_id ++)
        //     {
        //         for(idx = 0; idx < 5; idx++)
        //         {
        //             if(joy[joy_id] & joydecode[idx].mask)
        //             {
        //                 Bconout(2,'0'+joy_id);
        //                 Bconout(2,' ');
        //                 PRINT(joydecode[idx].txt);
        //                 //PRINT("\r\n");
        //             }
        //         }
        //     }

        
       // framebuffer_flip();
       //key = Cconin ();
       // main game loop goes here, one iteration per frame drawn
       // draw content into framebuffer provided by framebuffer_get_pointer()
       // make hidden framebuffer visible using framebuffer_flip()
	   endTime = timerGetPrec();
    }
	free(bitmap1);
	free(paletteBitmap);
	
	IKBD_Flush();

	IKBD_Uninstall();

	Setpalette(systemPalette);
}

int main(int argc, char **argv)
{
   Supexec(&main_supervisor, 0,0,0,0,0);
   return 0;
}