#define ATARI atari

//#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <osbind.h>
//#include <stdint.h>
//#include <mint/osbind.h>
#include <mint/sysbind.h>
//#include <gem.h>
//#include <gemx.h>


#include "engine.h"

#include "raycasting.c"
//#include "mipmaps.c"
#include "file_read.c"
#include "physics.c"
#include "dithering.c"
#include "drawing.c"
#include "map_streaming.c"
#include "setup_maps.c"
#include "rendering_quality.c"
#include "bitmap.c"


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
	CalculateRayCasts(engine.renderer.rayCastX, engine.renderer.rayCastY, XSIZEODD, YSIZEODD, 1);
	engine.deltaTime = 0;
}

void SetDefaulResolution()
{
		renderingDepth = TERRAINDEPTH;
		//engine.renderer.renderingType = 8;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 2;
		engine.renderer.renderingDepthStep = 1;

		engine.renderer.stepModifier = 16;
		Recalculate();
}

char fadeInStatus[4], fadeOutStatus[4];

void switchIntroScreen()
{

	switch (screenIndex)
	{
	case 2:
	{
		free(bitmap1);
		bitmap1 = LoadBitmapFile("data/l2", &bitmapHeader1, bitmapPalette1);
		
		ClearBuffor();
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
		free(bitmap1);
		bitmap1 = LoadBitmapFile("data/l3", &bitmapHeader1, bitmapPalette1);
		
		ClearBuffor();
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
		
	}
	break;
	}
}

void animateIntro()
{
	if(screenDuration < 7400000 && !fadeInStatus[3])
	{
		for (int i = 0; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = ((((bitmapPalette1[i * 4 + 2]/4) >> 5) << 8) +
								(((bitmapPalette1[i * 4 + 1]/4) >> 5) << 4) + ((bitmapPalette1[i * 4 + 0]/4) >> 5));
		}
		Setpalette(engine.renderer.bitmapPalette);
		fadeInStatus[3] = 1;
	}
	if(screenDuration < 7300000 && !fadeInStatus[2])
	{
		for (int i = 0; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = ((((bitmapPalette1[i * 4 + 2]/2) >> 5) << 8) +
								(((bitmapPalette1[i * 4 + 1]/2) >> 5) << 4) + ((bitmapPalette1[i * 4 + 0]/2) >> 5));
		}
		Setpalette(engine.renderer.bitmapPalette);
		fadeInStatus[2] = 1;
	}
	if(screenDuration < 7200000 && !fadeInStatus[1])
	{
		for (int i = 0; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = ((((bitmapPalette1[i * 4 + 2]*3/4) >> 5) << 8) +
								(((bitmapPalette1[i * 4 + 1]*3/4) >> 5) << 4) + ((bitmapPalette1[i * 4 + 0]*3/4) >> 5));
		}
		Setpalette(engine.renderer.bitmapPalette);
		fadeInStatus[1] = 1;
	}
	if(screenDuration < 7100000 && !fadeInStatus[0])
	{
		for (int i = 0; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = (((bitmapPalette1[i * 4 + 2] >> 5) << 8) +
								((bitmapPalette1[i * 4 + 1] >> 5) << 4) + (bitmapPalette1[i * 4 + 0] >> 5));
		}
		Setpalette(engine.renderer.bitmapPalette);
		fadeInStatus[0] = 1;
	}

	if(screenDuration < 400000 && !fadeOutStatus[0])
	{
		for (int i = 0; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = ((((bitmapPalette1[i * 4 + 2]*3/4) >> 5) << 8) +
								(((bitmapPalette1[i * 4 + 1]*3/4) >> 5) << 4) + ((bitmapPalette1[i * 4 + 0]*3/4) >> 5));
		}
		Setpalette(engine.renderer.bitmapPalette);
		fadeOutStatus[0] = 1;
	}
	if(screenDuration < 300000 && !fadeOutStatus[1])
	{
		for (int i = 0; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = ((((bitmapPalette1[i * 4 + 2]/2) >> 5) << 8) +
								(((bitmapPalette1[i * 4 + 1]/2) >> 5) << 4) + ((bitmapPalette1[i * 4 + 0]/2) >> 5));
		}
		Setpalette(engine.renderer.bitmapPalette);
		fadeOutStatus[1] = 1;
	}
	if(screenDuration < 200000 && !fadeOutStatus[2])
	{
		for (int i = 0; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = ((((bitmapPalette1[i * 4 + 2]/4) >> 5) << 8) +
								(((bitmapPalette1[i * 4 + 1]/4) >> 5) << 4) + ((bitmapPalette1[i * 4 + 0]/4) >> 5));
		}
		Setpalette(engine.renderer.bitmapPalette);
		fadeOutStatus[2] = 1;
	}
	if(screenDuration < 100000 && !fadeOutStatus[3])
	{
		for (int i = 0; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = 0;
		}
		Setpalette(engine.renderer.bitmapPalette);
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
void initdeltaTime()
{
	engine.endTime = getCurrentTime();
}
void getdeltaTime()
{
	engine.startTime = getCurrentTime();
	engine.deltaTime = (engine.startTime - engine.endTime);
	engine.endTime = engine.startTime;
	engine.accTime += engine.deltaTime;
}

void main_supervisor() 
{
	//turn off cursor
	Cursconf(0, 0);

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
			engine.renderer.renderingType = 4;
			hardwareSelection = 1;
			engine.renderer.xFOV = 28;
			engine.renderer.yFOV = 14;
		}
		if (IKBD_Keyboard[KEY_2]) 
		{
			engine.renderer.renderingType = 8;
			hardwareSelection = 2;
			engine.renderer.xFOV = 25;
			engine.renderer.yFOV = 14;
		}
	}
	//*************************************** SELECT HARDWARE




    bitmap1 = LoadBitmapFile("data/l1",&bitmapHeader1, bitmapPalette1);
	
    ship = LoadBitmapFile("data/icar48",&shipHeader, palettePalette);
	
	planes = framebuffer_get_pointer();
   
   	//process paletter from an image
	for(int i=0;i<16;i++)
	{
		engine.renderer.bitmapPalette[i] = ((bitmapPalette1[i*4+2]>>5) << 8) +
		 ((bitmapPalette1[i*4+1]>>5) << 4) + (bitmapPalette1[i*4+0]>>5);
	}

	Setpalette(engine.renderer.bitmapPalette);
	ClearBuffor();
    DrawBitmap4bCenter(bitmap1, &bitmapHeader1);


	paletteBitmap = LoadBitmapFile("data/plt", &paletteHeader, palettePalette);

	//process paletter from an image
	for(int i=0;i<16;i++)
	{
		engine.renderer.bitmapPalette[i] = ((palettePalette[i*4+2]>>5) << 8) +
		 ((palettePalette[i*4+1]>>5) << 4) + (palettePalette[i*4+0]>>5);
	}
	
	
	//*************************************************
	
	engine.gameState.shipParams.precX = 60 * 100;
	engine.gameState.shipParams.precZ = 0;
	engine.gameState.shipParams.precY = 20 * 100;
	engine.gameState.shipParams.dP = 0;
	engine.gameState.shipParams.ddP = 0;
	points = 0;
	engine.renderer.lastOverwrittenLine = 0;
	engine.gameState.crossHairX = 0;
	engine.gameState.crossHairY = 0;
	engine.accTime = 0;
				
	engine.gameState.shipParams.dPDenom = 128;

	SetupMaps();
	GenerateColorBytesDither3x2();

	SetDefaulResolution();
    //*****************************************
    
	uint8_t exitflag = 0;
	uint8_t idx,joy_id,joy[2];

	
	//IKBD_MouseOff();

	//Setpalette(grayColors);


	printf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");

	screenDuration = 7500000;
	screenIndex = 1;

	initdeltaTime();
	for(int i = 0; i < 4; i++)
	{
		fadeInStatus[i] = 1;
		fadeOutStatus[i] = 0;
	}

    while ( exitflag == 0)
    {


		getdeltaTime();
		if(screenIndex > 0)
		{
			if(screenDuration > 7500000)
			{
				screenDuration = 7500000;
				screenIndex = (screenIndex + 1) % 4;
				switchIntroScreen();
			}

			animateIntro();

			if(IKBD_Keyboard[IKBD_KEY_ESC])
			{
				//PRINT("ESC\r\n");
				exitflag = 1;
			}

			screenDuration -= engine.deltaTime;
		}
		else
		{
			
			if(infoScreen == 0)
			{
				free(bitmap1);
				bitmap1 = LoadBitmapFile("data_a/i1", &bitmapHeader1, bitmapPalette1);
				
				ClearBuffor();
				DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
				
				for (int i = 0; i < 16; i++)
				{
					engine.renderer.bitmapPalette[i] = ((bitmapPalette1[i * 4 + 2] >> 5) << 8) +
										((bitmapPalette1[i * 4 + 1] >> 5) << 4) + (bitmapPalette1[i * 4 + 0] >> 5);
				}
				Setpalette(engine.renderer.bitmapPalette);

				UBYTE infoIndex = 0;
				UBYTE FireDown = 0;

				while(!infoScreen)
				{
					if(((IKBD_STICK1 & IKBD_JOY_FIRE ) || IKBD_Keyboard[KEY_CTRL]) && !FireDown)
					{
						infoIndex += 1;
						
						switch(infoIndex)
						{
							case 1:
							{
								free(bitmap1);
								bitmap1 = LoadBitmapFile("data_a/i2", &bitmapHeader1, bitmapPalette1);
								
								ClearBuffor();
								DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
								
								for (int i = 0; i < 16; i++)
								{
									engine.renderer.bitmapPalette[i] = ((bitmapPalette1[i * 4 + 2] >> 5) << 8) +
														((bitmapPalette1[i * 4 + 1] >> 5) << 4) + (bitmapPalette1[i * 4 + 0] >> 5);
								}
								Setpalette(engine.renderer.bitmapPalette);
								FireDown = 1;
							} break;
							case 2:
							{
								free(bitmap1);
								bitmap1 = LoadBitmapFile("data_a/i3", &bitmapHeader1, bitmapPalette1);
								
								ClearBuffor();
								DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
								
								for (int i = 0; i < 16; i++)
								{
									engine.renderer.bitmapPalette[i] = ((bitmapPalette1[i * 4 + 2] >> 5) << 8) +
														((bitmapPalette1[i * 4 + 1] >> 5) << 4) + (bitmapPalette1[i * 4 + 0] >> 5);
								}
								Setpalette(engine.renderer.bitmapPalette);
								FireDown = 1;
							} break;
							case 3:
							{
								free(bitmap1);
								bitmap1 = LoadBitmapFile("data_a/i4", &bitmapHeader1, bitmapPalette1);
								
								ClearBuffor();
								DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
								
								for (int i = 0; i < 16; i++)
								{
									engine.renderer.bitmapPalette[i] = ((bitmapPalette1[i * 4 + 2] >> 5) << 8) +
														((bitmapPalette1[i * 4 + 1] >> 5) << 4) + (bitmapPalette1[i * 4 + 0] >> 5);
								}
								Setpalette(engine.renderer.bitmapPalette);
								FireDown = 1;

							} break;
							case 4:
							{
								ClearBuffor();
								for (int i = 0; i < 16; i++)
								{
									engine.renderer.bitmapPalette[i] = ((palettePalette[i * 4 + 2] >> 5) << 8) +
														((palettePalette[i * 4 + 1] >> 5) << 4) + (palettePalette[i * 4 + 0] >> 5);
								}
								Setpalette(engine.renderer.bitmapPalette);
								infoScreen = 1;
								engine.endTime = timerGetPrec();
								engine.startTime = timerGetPrec();
								engine.deltaTime = 0;
								engine.accTime = 0;
							} break;
						}
					}
					else if(FireDown && !((IKBD_STICK1 & IKBD_JOY_FIRE ) || IKBD_Keyboard[KEY_CTRL]))
					{
						FireDown = 0;
					}

					
					if(IKBD_Keyboard[IKBD_KEY_ESC])
					{
						//PRINT("ESC\r\n");
						exitflag = 1;
						infoScreen = 1;
					}
				}
			}

			//ProcessQualityInput();

			
			engine.gameState = ProcessInput(engine.gameState, engine.deltaTime);

			ULONG lowerDelta = (engine.deltaTime/10000);

			UWORD terrainHeight = getTerrainHeight(engine.gameState.shipParams, engine.renderer.mapHigh);

			engine.gameState = updateShipParams(engine.gameState, lowerDelta, terrainHeight);

			LONG addedpoints = (lowerDelta)*(128 - engine.gameState.shipParams.relHeight);
			if(addedpoints > 0)
			{
				points += addedpoints;
			}
			
			engine.renderer.xOffsetOdd = engine.gameState.crossHairX / 600;
			engine.renderer.xOffsetEven = engine.gameState.crossHairX / 900;

			OverwriteMap();
			RenderQuality();

			//draw crosshair
			//draw only even lines 
			UWORD crossPX = 160 + (engine.gameState.crossHairX / 400);
			UWORD crossPY = 90 + (engine.gameState.crossHairY / 400) ;
			

			UWORD shipDirX = 160 + (engine.gameState.crossHairX/1600);
			UWORD shipDirY = 95 + (engine.gameState.crossHairY/1600);
			//crossHairX = ( (160 + (engine.gameState.crossHairX / 10)) / 16 );
			//crossHairY = ( YSIZEODD*2 + (engine.gameState.crossHairY / 5) )/2;
			
			WORD spriteIndexX = 1;
			WORD spriteIndexY = 1;
			if(engine.gameState.crossHairX > 8000)
			{
				spriteIndexX = 2;
			}
			else if(engine.gameState.crossHairX < -8000)
			{
				spriteIndexX = 0;
			}
			if(engine.gameState.crossHairY > 4000)
			{
				spriteIndexY = 0;
			}
			else if(engine.gameState.crossHairY < -4000)
			{
				spriteIndexY = 2;
			}
			
			DrawPixel( crossPX, crossPY + 4, 0);
			DrawPixel( crossPX, crossPY - 4, 0);
			DrawSprite4b(ship, &shipHeader, shipDirX, shipDirY,
						 spriteIndexX, spriteIndexY, 48, 48, 3);

			//printf("%d	%d\r", engine.gameState.shipParams.pZ, (engine.gameState.shipParams.pZ / 256 + 1) % MAPLENGTH);
			printf("SC:%d  SP:%d  RH:%d  T:%d\r", points, engine.gameState.shipParams.dP, engine.gameState.shipParams.relHeight, engine.accTime/2500);
			
			fflush(stdout);
			//IKBD_Flush();
			Vsync();
			//IKBD_ReadMouse();
			if(IKBD_Keyboard[IKBD_KEY_ESC])
			{
				//PRINT("ESC\r\n");
				exitflag = 1;
			}
			//if(IKBD_Keyboard[IKBD_KEY_UP]) engine.gameState.shipParams.pY += 1;
			//if(IKBD_Keyboard[IKBD_KEY_DOWN]) engine.gameState.shipParams.pY -= 1;
			//if(IKBD_Keyboard[IKBD_KEY_RIGHT]) engine.gameState.shipParams.pX += 1;
			//if(IKBD_Keyboard[IKBD_KEY_LEFT]) engine.gameState.shipParams.pX -= 1;
			//if(IKBD_Keyboard[IKBD_KEY_CONTROL]) engine.gameState.shipParams.pZ += 1;
			//if(IKBD_Keyboard[IKBD_KEY_ALT]) engine.gameState.shipParams.pZ -= 1;
			

			//restart
			if ((engine.gameState.shipParams.pY - 3) < (UBYTE)(engine.renderer.mapHigh[((UBYTE)(engine.gameState.shipParams.pX)) >> 1][((UBYTE)(engine.gameState.shipParams.pZ + 15)) >> 1]))
			{
   				
				ClearBuffor();

				engine.gameState.shipParams.precX = 60 * 100;
				engine.gameState.shipParams.precZ = 0;
				engine.gameState.shipParams.precY = 20 * 100;
				engine.gameState.shipParams.dP = 0;
				engine.gameState.shipParams.ddP = 0;
				points = 0;
				CopyMapWord(engine.renderer.mapSource[0], engine.renderer.mapHigh);
				engine.renderer.lastOverwrittenLine = 0;
				engine.gameState.crossHairX = 0;
				engine.gameState.crossHairY = 0;
				engine.accTime = 0;
				
				engine.gameState.shipParams.dPDenom = 128;

				Cursconf(1, 0);
				
							
				printf("You are dead! Press fire to try again!\r");
				
				fflush(stdout);
				
				
				char cont = 0;
				while(!cont)
				{
					if(((IKBD_STICK1 & IKBD_JOY_FIRE ) || IKBD_Keyboard[KEY_CTRL]))
					{
						cont = 1;
					}
					if(IKBD_Keyboard[IKBD_KEY_ESC])
					{
						//PRINT("ESC\r\n");
						exitflag = 1;
						cont = 1;
					}
					

				}

				printf("                                      \r");
				Cursconf(0, 0);

				engine.endTime = timerGetPrec();


				for(int i=0;i<16;i++)
				{
					engine.renderer.bitmapPalette[i] = ((palettePalette[i*4+2]>>5) << 8) +
					((palettePalette[i*4+1]>>5) << 4) + (palettePalette[i*4+0]>>5);
				}
				Setpalette(engine.renderer.bitmapPalette);

			}
			else if(endScreen)
			{
				
				if(points < 1000000)
				{
					ClearBuffor();

					engine.gameState.shipParams.precX = 60 * 100;
					engine.gameState.shipParams.precZ = 0;
					engine.gameState.shipParams.precY = 20 * 100;
					engine.gameState.shipParams.dP = 0;
					engine.gameState.shipParams.ddP = 0;
					points = 0;
					CopyMapWord(engine.renderer.mapSource[0], engine.renderer.mapHigh);
					engine.renderer.lastOverwrittenLine = 0;
					engine.gameState.crossHairX = 0;
					engine.gameState.crossHairY = 0;
					engine.accTime = 0;
					engine.gameState.shipParams.dPDenom = 128;
					endScreen = 0;
					
					Cursconf(1, 0);
					printf("You failed! Press fire to try again!\r");
					fflush(stdout);

					char cont = 0;
					while(!cont)
					{
						if(((IKBD_STICK1 & IKBD_JOY_FIRE ) || IKBD_Keyboard[KEY_CTRL]))
						{
							cont = 1;
						}
						if(IKBD_Keyboard[IKBD_KEY_ESC])
						{
							//PRINT("ESC\r\n");
							exitflag = 1;
							cont = 1;
						}

					}
					engine.endTime = timerGetPrec();

					printf("                                      \r");
					Cursconf(0, 0);
				

					ClearBuffor();
					for(int i=0;i<16;i++)
					{
						engine.renderer.bitmapPalette[i] = ((palettePalette[i*4+2]>>5) << 8) +
						((palettePalette[i*4+1]>>5) << 4) + (palettePalette[i*4+0]>>5);
					}
					Setpalette(engine.renderer.bitmapPalette);
				}
				else
				{
					free(bitmap1);
					bitmap1 = LoadBitmapFile("data/fin", &bitmapHeader1, bitmapPalette1);
					
					ClearBuffor();
					DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
					
					for (int i = 0; i < 16; i++)
					{
						engine.renderer.bitmapPalette[i] = ((bitmapPalette1[i * 4 + 2] >> 5) << 8) +
											((bitmapPalette1[i * 4 + 1] >> 5) << 4) + (bitmapPalette1[i * 4 + 0] >> 5);
					}
					Setpalette(engine.renderer.bitmapPalette);

					engine.gameState.shipParams.precX = 60 * 100;
					engine.gameState.shipParams.precZ = 0;
					engine.gameState.shipParams.precY = 20 * 100;
					engine.gameState.shipParams.dP = 0;
					engine.gameState.shipParams.ddP = 0;
					points = 0;
					CopyMapWord(engine.renderer.mapSource[0], engine.renderer.mapHigh);
					engine.renderer.lastOverwrittenLine = 0;
					engine.gameState.crossHairX = 0;
					engine.gameState.crossHairY = 0;
					engine.accTime = 0;
					engine.gameState.shipParams.dPDenom = 128;
					infoScreen = 0;

					endScreen = 0;
					char cont = 0;
					while(!cont)
					{
						if(((IKBD_STICK1 & IKBD_JOY_FIRE ) || IKBD_Keyboard[KEY_CTRL]))
						{
							cont = 1;
						}
						if(IKBD_Keyboard[IKBD_KEY_ESC])
						{
							//PRINT("ESC\r\n");
							exitflag = 1;
							cont = 1;
						}

					}
					engine.endTime = timerGetPrec();

					
					ClearBuffor();
					for(int i=0;i<16;i++)
					{
						engine.renderer.bitmapPalette[i] = ((palettePalette[i*4+2]>>5) << 8) +
						((palettePalette[i*4+1]>>5) << 4) + (palettePalette[i*4+0]>>5);
					}
					Setpalette(engine.renderer.bitmapPalette);
					
				}
				
			}

			
			if(engine.gameState.shipParams.pZ > 11*256 && engine.gameState.shipParams.dPDenom < 3*255)
			{
				engine.gameState.shipParams.dPDenom = engine.gameState.shipParams.dPDenom + 8;
			}
			else if(engine.gameState.shipParams.dPDenom >= 3*255)
			{
				endScreen = 1;
			}
			
			
			
		}

		
			
	   engine.loopEndTime = timerGetPrec();
    }
	free(bitmap1);
	free(paletteBitmap);
	
	IKBD_Flush();

	IKBD_Uninstall();

	Setpalette(systemPalette);

	
	Cursconf(1, 0);
}

int main(int argc, char **argv)
{
   Supexec(&main_supervisor, 0,0,0,0,0);
   return 0;
}