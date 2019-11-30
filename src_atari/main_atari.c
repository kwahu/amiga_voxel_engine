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
#include "ray_casting_atari_st.c"
#include "ray_casting_atari_falcon.c"
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
		//debugValue = 8;
		debugValue2 = 2;
		debugValue3 = 2;
		debugValue4 = 1;

		debugValue6 = 4;
		xFOV = 10;
		Recalculate();
}

char fadeInStatus[4], fadeOutStatus[4];

void switchIntroScreen()
{

	switch (screenIndex)
	{
	case 2:
	{
		bitmap1 = LoadBitmapFile("data/logo2.bmp", &bitmapHeader1, bitmapPalette1);
		
		ClearScreen();
		DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
		
		for(int i = 0; i < 4; i++)
		{
			fadeInStatus[i] = 0;
			fadeOutStatus[i] = 0;
		}
	}
	break;
	case 3:
	{
		bitmap1 = LoadBitmapFile("data/logo3.bmp", &bitmapHeader1, bitmapPalette1);
		
		ClearScreen();
		DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
		
		for(int i = 0; i < 4; i++)
		{
			fadeInStatus[i] = 0;
			fadeOutStatus[i] = 0;
		}
	}
	break;
	case 0:
	{
		bitmap1 = LoadBitmapFile("data/menu1.bmp", &bitmapHeader1, bitmapPalette1);
		
		ClearScreen();
		DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
		
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = ((bitmapPalette1[i * 4 + 2] >> 5) << 8) +
								((bitmapPalette1[i * 4 + 1] >> 5) << 4) + (bitmapPalette1[i * 4 + 0] >> 5);
		}
		Setpalette(bitmapPalette);
	}
	break;
	}
}

void animateIntro()
{
	if(screenDuration < 3900 && !fadeInStatus[3])
	{
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = ((((bitmapPalette1[i * 4 + 2]/4) >> 5) << 8) +
								(((bitmapPalette1[i * 4 + 1]/4) >> 5) << 4) + ((bitmapPalette1[i * 4 + 0]/4) >> 5));
		}
		Setpalette(bitmapPalette);
		fadeInStatus[3] = 1;
	}
	if(screenDuration < 3800 && !fadeInStatus[2])
	{
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = ((((bitmapPalette1[i * 4 + 2]/2) >> 5) << 8) +
								(((bitmapPalette1[i * 4 + 1]/2) >> 5) << 4) + ((bitmapPalette1[i * 4 + 0]/2) >> 5));
		}
		Setpalette(bitmapPalette);
		fadeInStatus[2] = 1;
	}
	if(screenDuration < 3700 && !fadeInStatus[1])
	{
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = ((((bitmapPalette1[i * 4 + 2]*3/4) >> 5) << 8) +
								(((bitmapPalette1[i * 4 + 1]*3/4) >> 5) << 4) + ((bitmapPalette1[i * 4 + 0]*3/4) >> 5));
		}
		Setpalette(bitmapPalette);
		fadeInStatus[1] = 1;
	}
	if(screenDuration < 3600 && !fadeInStatus[0])
	{
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = (((bitmapPalette1[i * 4 + 2] >> 5) << 8) +
								((bitmapPalette1[i * 4 + 1] >> 5) << 4) + (bitmapPalette1[i * 4 + 0] >> 5));
		}
		Setpalette(bitmapPalette);
		fadeInStatus[0] = 1;
	}

	if(screenDuration < 400 && !fadeOutStatus[0])
	{
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = ((((bitmapPalette1[i * 4 + 2]*3/4) >> 5) << 8) +
								(((bitmapPalette1[i * 4 + 1]*3/4) >> 5) << 4) + ((bitmapPalette1[i * 4 + 0]*3/4) >> 5));
		}
		Setpalette(bitmapPalette);
		fadeOutStatus[0] = 1;
	}
	if(screenDuration < 300 && !fadeOutStatus[1])
	{
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = ((((bitmapPalette1[i * 4 + 2]/2) >> 5) << 8) +
								(((bitmapPalette1[i * 4 + 1]/2) >> 5) << 4) + ((bitmapPalette1[i * 4 + 0]/2) >> 5));
		}
		Setpalette(bitmapPalette);
		fadeOutStatus[1] = 1;
	}
	if(screenDuration < 200 && !fadeOutStatus[2])
	{
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = ((((bitmapPalette1[i * 4 + 2]/4) >> 5) << 8) +
								(((bitmapPalette1[i * 4 + 1]/4) >> 5) << 4) + ((bitmapPalette1[i * 4 + 0]/4) >> 5));
		}
		Setpalette(bitmapPalette);
		fadeOutStatus[2] = 1;
	}
	if(screenDuration < 100 && !fadeOutStatus[3])
	{
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = 0;
		}
		Setpalette(bitmapPalette);
		fadeOutStatus[3] = 1;
	}
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
UBYTE hardwareSelection = 0;

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
	IKBD_Install();

	//*************************************** SELECT HARDWARE
	printf("KEY 1 ATARI ST     KEY 2 ATARI FALCON / TT\r\n");
	printf("Change quality at any time KEYS 1-8\r\n");
	while(hardwareSelection == 0)
	{
		if (IKBD_Keyboard[KEY_1])
		{
			debugValue = 4;
			hardwareSelection = 1;
		}
		if (IKBD_Keyboard[KEY_2]) 
		{
			debugValue = 8;
			hardwareSelection = 2;
		}
	}
	//*************************************** SELECT HARDWARE




    bitmap1 = LoadBitmapFile("data/logo1.bmp",&bitmapHeader1, bitmapPalette1);
	
	planes = framebuffer_get_pointer();
   
   	//process paletter from an image
	for(int i=0;i<16;i++)
	{
		bitmapPalette[i] = ((bitmapPalette1[i*4+2]>>5) << 8) +
		 ((bitmapPalette1[i*4+1]>>5) << 4) + (bitmapPalette1[i*4+0]>>5);
	}

	Setpalette(bitmapPalette);
	ClearScreen();
    DrawBitmap4bCenter(bitmap1, &bitmapHeader1);


	paletteBitmap = LoadBitmapFile("data/palette.bmp", &paletteHeader, palettePalette);

	//process paletter from an image
	for(int i=0;i<16;i++)
	{
		bitmapPalette[i] = ((palettePalette[i*4+2]>>5) << 8) +
		 ((palettePalette[i*4+1]>>5) << 4) + (palettePalette[i*4+0]>>5);
	}
	
	
	//*************************************************
	
	p1xf = 64 * 100;
	p1yf = 0;
	p1hf = 50 * 100;

	p2x = 0;
	p2y = 0;
	p2h = 10;

	lastOverwrittenLine = 0;
	SetupMaps();
	GenerateColorBytesDither3x2();

	SetDefaulResolution();
    //*****************************************
    
	uint8_t exitflag = 0;
	uint8_t idx,joy_id,joy[2];

	
	//IKBD_MouseOff();

	//Setpalette(grayColors);
	//DrawColorMap(mapHigh);

	printf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");

	screenDuration = 4000;
	screenIndex = 1;

	initDeltaTime();
	for(int i = 0; i < 4; i++)
	{
		fadeInStatus[i] = 1;
		fadeOutStatus[i] = 0;
	}

    while ( exitflag == 0)
    {


		getDeltaTime();
		if(screenIndex > 0)
		{
			if(screenDuration > 4000)
			{
				screenDuration = 4000;
				screenIndex = (screenIndex + 1) % 4;
				switchIntroScreen();
			}

			animateIntro();


			screenDuration -= deltaTime;
		}
		else
		{
			
			if(infoScreen == 0)
			{
				UBYTE infoIndex = 0;
				UBYTE FireDown = 0;

				while(!infoScreen)
				{
					if(IKBD_Keyboard[KEY_DOWN] && !FireDown)
					{
						infoIndex += 1;
						
						switch(infoIndex)
						{
							case 1:
							{
										
								ClearScreen();
								DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
								
								FireDown = 1;
							} break;
							case 2:
							{
									
								ClearScreen();
								DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
								
								FireDown = 1;
							} break;
							case 3:
							{
								bitmap1 = LoadBitmapFile("data/menu2.bmp", &bitmapHeader1, bitmapPalette1);
										
								ClearScreen();
								DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
								for (int i = 0; i < 16; i++)
								{
									bitmapPalette[i] = ((bitmapPalette1[i * 4 + 2] >> 5) << 8) +
														((bitmapPalette1[i * 4 + 1] >> 5) << 4) + (bitmapPalette1[i * 4 + 0] >> 5);
								}
								Setpalette(bitmapPalette);

								FireDown = 1;

							} break;
							case 4:
							{
								ClearScreen();
								for (int i = 0; i < 16; i++)
								{
									bitmapPalette[i] = ((palettePalette[i * 4 + 2] >> 5) << 8) +
														((palettePalette[i * 4 + 1] >> 5) << 4) + (palettePalette[i * 4 + 0] >> 5);
								}
								Setpalette(bitmapPalette);
								infoScreen = 1;
								lastTime = timerGetPrec();
								startTime = timerGetPrec();
								deltaTime = 0;
							} break;
						}
					}
					else if(FireDown && !IKBD_Keyboard[KEY_DOWN])
					{
						FireDown = 0;
					}
				}
			}

			ProcessQualityInputAtari();
			ProcessPlayerInputAtari();
			OverwriteMap();
			
			RenderQuality();

			//draw crosshair
			DrawPixel((160 + (cx / 150)) / 16, YSIZEODD + (cy / 100) + 4, 0);
			DrawPixel((160 + (cx / 150)) / 16, YSIZEODD + (cy / 100) - 4, 0);
				
			printf("%d	%d\r", p1y, (p1y / 256 + 1) % MAPLENGTH);
			printf("%d  %d\r", points, velocity);
			fflush(stdout);
			//IKBD_Flush();
			//Vsync();
			//IKBD_ReadMouse();
			if(IKBD_Keyboard[IKBD_KEY_ESC])
			{
				//PRINT("ESC\r\n");
				exitflag = 1;
			}
			//if(IKBD_Keyboard[IKBD_KEY_UP]) p1h += 1;
			//if(IKBD_Keyboard[IKBD_KEY_DOWN]) p1h -= 1;
			//if(IKBD_Keyboard[IKBD_KEY_RIGHT]) p1x += 1;
			//if(IKBD_Keyboard[IKBD_KEY_LEFT]) p1x -= 1;
			//if(IKBD_Keyboard[IKBD_KEY_CONTROL]) p1y += 1;
			//if(IKBD_Keyboard[IKBD_KEY_ALT]) p1y -= 1;
			

			//restart
			if ((p1h - 3) < (UBYTE)(mapHigh[(UBYTE)(p1x)][(UBYTE)(p1y + 15)]))
			{
   				DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
				for(int i=0;i<16;i++)
				{
					bitmapPalette[i] = ((bitmapPalette1[i*4+2]>>5) << 8) +
					((bitmapPalette1[i*4+1]>>5) << 4) + (bitmapPalette1[i*4+0]>>5);
				}

				Setpalette(bitmapPalette);

				p1xf = 64 * 100;
				p1yf = 0;
				p1hf = 50 * 100;
				velocity = 0;
				acceleration = 0;
				points = 0;
				CopyMapWord(mapSource[0], mapHigh);
				lastOverwrittenLine = 0;
				cx = 0;
				cy = 0;
				levelTime = 0;


				while(!IKBD_Keyboard[KEY_DOWN])
				{

				}
				lastTime = timerGetPrec();



				for(int i=0;i<16;i++)
				{
					bitmapPalette[i] = ((palettePalette[i*4+2]>>5) << 8) +
					((palettePalette[i*4+1]>>5) << 4) + (palettePalette[i*4+0]>>5);
				}
				ClearScreen();
				Setpalette(bitmapPalette);

			}
			
		}
		
			
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