#define AMIGA amiga

#include <ace/generic/main.h>

#include "settings_amiga.h"
#include "engine.h"
#include "ray_casting_amiga.c"
#include "ray_casting_progressive.c"
#include "ray_cast_calculate.c"
#include "draw_screen_amiga.c"
//#include "mipmaps.c"
#include "file_read.c"
#include "bitmap_filters.c"
#include "dithering.c"
#include "draw_ships.c"
#include "draw_sprite.c"
#include "draw_maps_amiga.c"
#include "input_handling_amiga.c"
#include "map_streaming.c"
#include "setup_maps.c"
#include "rendering_quality_amiga.c"
//#include "bitmap.c"
#include <ace/managers/game.h>
#include <ace/managers/timer.h>
#include <ace/managers/system.h>
#include <ace/managers/blit.h>
#include <ace/utils/file.h>
#include <ace/utils/font.h>

/*
docker run --rm \
-v ${PWD}:/work \
-v /path/to/extra/m68k-amigaos/lib:/tools/usr/lib \
-v /path/to/extra/m68k-amigaos/include:/tools/usr/include \
-it amigadev/crosstools:m68k-amigaos bash
*/

static tFont *s_pMenuFont;

void genericCreate(void)
{
	gamePushState(engineGsCreate, engineGsLoop, engineGsDestroy);
}

void genericProcess(void)
{
	gameProcess();
}

void genericDestroy(void)
{
}


static tAvg *s_pAvgTime;

tTextBitMap *pBitmapPlayerX, *pBitmapPlayerY, *pBitmapPlayerH, *informationText;
tTextBitMap *pBitmapTime, *pBitmapVelocity, *pBitmapScore, *pBitmapInfo[10];

tTextBitMap *pixel[32];
char sPixel[32][10];
char sPlayerX[5], sPlayerY[5], sPlayerH[5];
char sTime[10], sVelocity[8], sScore[5];
char fadeInStatus[4], fadeOutStatus[4];

unsigned char *currentPallete;

UBYTE hardwareSelection = 0;

UBYTE gamePaletteSet = 0;

UWORD screenOffset = 28;//(256 - YSIZEODD*2)/2 * 20;


void switchIntroScreen()
{

	switch (screenIndex)
	{
	case 2:
	{
		systemUse();
		bitmap1 = LoadBitmapFile("data/logo2.bmp", &bitmapHeader1, bitmapPalette1);
		systemUnuse();
		
		ClearBuffor();
		DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
		
		for(int i = 0; i < 4; i++)
		{
			fadeInStatus[i] = 0;
			fadeOutStatus[i] = 0;
		}
		viewLoad(s_pView);
		vPortWaitForEnd(s_pVPort);
		CopyFastToChipW(s_pBuffer->pBack);
	}
	break;
	case 3:
	{
		systemUse();
		bitmap1 = LoadBitmapFile("data/logo3.bmp", &bitmapHeader1, bitmapPalette1);
		systemUnuse();
		ClearBuffor();
		DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
		
		for(int i = 0; i < 4; i++)
		{
			fadeInStatus[i] = 0;
			fadeOutStatus[i] = 0;
		}
		viewLoad(s_pView);
		vPortWaitForEnd(s_pVPort);
		CopyFastToChipW(s_pBuffer->pBack);
	}
	break;
	case 0:
	{
		systemUse();
		bitmap1 = LoadBitmapFile("data/menu1.bmp", &bitmapHeader1, bitmapPalette1);
		systemUnuse();
		ClearBuffor();
		DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
		
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = ((bitmapPalette1[i * 4 + 2] >> 4) << 8) +
								((bitmapPalette1[i * 4 + 1] >> 4) << 4) + (bitmapPalette1[i * 4 + 0] >> 4);
		}
		memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
		
		viewLoad(s_pView);
		vPortWaitForEnd(s_pVPort);
		CopyFastToChipW(s_pBuffer->pBack);
	}
	break;
	}
}

void animateIntro()
{
	if(screenDuration < 9600000 && !fadeInStatus[3])
	{
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = ((((currentPallete[i * 4 + 2]/4) >> 4) << 8) +
								(((currentPallete[i * 4 + 1]/4) >> 4) << 4) + ((currentPallete[i * 4 + 0]/4) >> 4));
		}
		memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
		viewLoad(s_pView);
		fadeInStatus[3] = 1;
	}
	if(screenDuration < 9200000 && !fadeInStatus[2])
	{
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = ((((currentPallete[i * 4 + 2]/2) >> 4) << 8) +
								(((currentPallete[i * 4 + 1]/2) >> 4) << 4) + ((currentPallete[i * 4 + 0]/2) >> 4));
		}
		memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
		viewLoad(s_pView);
		fadeInStatus[2] = 1;
	}
	if(screenDuration < 8800000 && !fadeInStatus[1])
	{
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = ((((currentPallete[i * 4 + 2]*3/4) >> 4) << 8) +
								(((currentPallete[i * 4 + 1]*3/4) >> 4) << 4) + ((currentPallete[i * 4 + 0]*3/4) >> 4));
		}
		memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
		viewLoad(s_pView);
		fadeInStatus[1] = 1;
	}
	if(screenDuration < 8400000 && !fadeInStatus[0])
	{
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = (((currentPallete[i * 4 + 2] >> 4) << 8) +
								((currentPallete[i * 4 + 1] >> 4) << 4) + (currentPallete[i * 4 + 0] >> 4));
		}
		memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
		viewLoad(s_pView);
		fadeInStatus[0] = 1;
	}

	if(screenDuration < 1600000 && !fadeOutStatus[0])
	{
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = ((((currentPallete[i * 4 + 2]*3/4) >> 4) << 8) +
								(((currentPallete[i * 4 + 1]*3/4) >> 4) << 4) + ((currentPallete[i * 4 + 0]*3/4) >> 4));
		}
		memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
		viewLoad(s_pView);
		fadeOutStatus[0] = 1;
	}
	if(screenDuration < 1200000 && !fadeOutStatus[1])
	{
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = ((((currentPallete[i * 4 + 2]/2) >> 4) << 8) +
								(((currentPallete[i * 4 + 1]/2) >> 4) << 4) + ((currentPallete[i * 4 + 0]/2) >> 4));
		}
		memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
		viewLoad(s_pView);
		fadeOutStatus[1] = 1;
	}
	if(screenDuration < 800000 && !fadeOutStatus[2])
	{
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = ((((currentPallete[i * 4 + 2]/4) >> 4) << 8) +
								(((currentPallete[i * 4 + 1]/4) >> 4) << 4) + ((currentPallete[i * 4 + 0]/4) >> 4));
		}
		memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
		viewLoad(s_pView);
		fadeOutStatus[2] = 1;
	}
	if(screenDuration < 400000 && !fadeOutStatus[3])
	{
		for (int i = 0; i < 16; i++)
		{
			bitmapPalette[i] = 0;
		}
		memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
		viewLoad(s_pView);
		fadeOutStatus[3] = 1;
	}
}


void RecalculateEven()
{
	CalculateRayCasts(rayCastXEven, rayCastYEven, XSIZEEVEN, YSIZEEVEN, 2); //było 2
	deltaTime = 0;
}
void RecalculateOdd()
{
	CalculateRayCasts(rayCastXOdd, rayCastYOdd, XSIZEODD, YSIZEODD, 1);
	deltaTime = 0;
}

void ConvertIntToChar(int number, char *test)
{
	int temp;
	int i = 5;
	while (number != 0)
	{
		temp = number % 10;
		number /= 10;
		switch (temp)
		{
		case 1:
			test[i] = '1';
			break;
		case 2:
			test[i] = '2';
			break;
		case 3:
			test[i] = '3';
			break;
		case 4:
			test[i] = '4';
			break;
		case 5:
			test[i] = '5';
			break;
		case 6:
			test[i] = '6';
			break;
		case 7:
			test[i] = '7';
			break;
		case 8:
			test[i] = '8';
			break;
		case 9:
			test[i] = '9';
			break;
		case 0:
			test[i] = '0';
			break;
		}
		i--;
	}
	while (i >= 0)
	{
		test[i] = ' ';
		i--;
	}
}

void ConvertWordToChar(UWORD number, char *test)
{
	UWORD temp;
	int i = 4;
	while (i >= 0)
	{
		temp = number % 16;
		number /= 16;
		switch (temp)
		{
		case 0:
			test[i] = '0';
			break;
		case 1:
			test[i] = '1';
			break;
		case 2:
			test[i] = '2';
			break;
		case 3:
			test[i] = '3';
			break;
		case 4:
			test[i] = '4';
			break;
		case 5:
			test[i] = '5';
			break;
		case 6:
			test[i] = '6';
			break;
		case 7:
			test[i] = '7';
			break;
		case 8:
			test[i] = '8';
			break;
		case 9:
			test[i] = '9';
			break;
		case 10:
			test[i] = 'a';
			break;
		case 11:
			test[i] = 'b';
			break;
		case 12:
			test[i] = 'c';
			break;
		case 13:
			test[i] = 'd';
			break;
		case 14:
			test[i] = 'e';
			break;
		case 15:
			test[i] = 'f';
			break;
		}
		i--;
	}
	//  while(i>=0)
	//  {
	//  	test[i] = '_';
	//  	i--;
	//  }
}

void ConvertByteToChar(UBYTE number, char *test)
{
	UBYTE temp;
	int i = 3;
	while (number != 0)
	{
		temp = number % 10;
		number /= 10;
		switch (temp)
		{
		case 1:
			test[i] = '1';
			break;
		case 2:
			test[i] = '2';
			break;
		case 3:
			test[i] = '3';
			break;
		case 4:
			test[i] = '4';
			break;
		case 5:
			test[i] = '5';
			break;
		case 6:
			test[i] = '6';
			break;
		case 7:
			test[i] = '7';
			break;
		case 8:
			test[i] = '8';
			break;
		case 9:
			test[i] = '9';
			break;
		case 0:
			test[i] = '0';
			break;
		}
		i--;
	}
	while (i >= 0)
	{
		test[i] = ' ';
		i--;
	}
}

void SetGamePaletter()
{
	//process paletter from an image
	for (int i = 0; i < 16; i++)
	{
		bitmapPalette[i] = ((palettePalette[i * 4 + 2] >> 4) << 8) +
						   ((palettePalette[i * 4 + 1] >> 4) << 4) + (palettePalette[i * 4 + 0] >> 4);
	}

	memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
	viewLoad(s_pView);
}

//****************************** CREATE
void engineGsCreate(void)
{
	s_pView = viewCreate(0,
						 TAG_VIEW_GLOBAL_CLUT, 1,
						 TAG_DONE);

	s_pVPort = vPortCreate(0,
						   TAG_VPORT_VIEW, s_pView,
						   TAG_VPORT_BPP, DEPTH,
						   TAG_END);

	s_pBuffer = simpleBufferCreate(0,
								   TAG_SIMPLEBUFFER_VPORT, s_pVPort,
								   TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR,
								   TAG_DONE);

	bitmap1 = LoadBitmapFile("data/logo1.bmp", &bitmapHeader1, bitmapPalette1);
	paletteBitmap = LoadBitmapFile("data/palette.bmp", &paletteHeader, palettePalette);

	//process paletter from an image
	for (int i = 0; i < 16; i++)
	{
		bitmapPalette[i] = ((bitmapPalette1[i * 4 + 2] >> 4) << 8) +
						   ((bitmapPalette1[i * 4 + 1] >> 4) << 4) + (bitmapPalette1[i * 4 + 0] >> 4);
	}

	currentPallete = bitmapPalette1;
	for(int i = 0; i < 4; i++)
	{
		fadeInStatus[i] = 1;
		fadeOutStatus[i] = 0;
	}
	memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
	viewLoad(s_pView);
	// Load font
	s_pMenuFont = fontCreate("data/silkscreen.fnt");

	p1xf = 60 * 100;
	p1yf = 0;
	p1hf = 20 * 100;

	p2x = 0;
	p2y = 0;
	p2h = 10;

	lastOverwrittenLine = 0;

	SetupMaps();

	GenerateColorBytesNoDither4x4();
	GenerateColorBytesDither3x2();

	pBitmapPlayerX = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");
	pBitmapPlayerY = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");
	pBitmapPlayerH = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");
	pBitmapTime = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");
	pBitmapVelocity = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");
	pBitmapScore = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");
	for(UBYTE i = 0; i < 10; ++i)
	{
		//pBitmapInfo[i] = fontCreateTextBitMapFromStr(s_pMenuFont, "123456789012345678901234567890123456789012345678");
	}

	for (int i = 0; i < 32; i++)
		pixel[i] = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");

	s_pAvgTime = logAvgCreate("perf", 100);
	levelTime = 0;
	screenIndex = 1;

	keyCreate();

	joyOpen(0);
	systemUnuse();


	//*********************************** SELECT HARDWARE ***********************************************
	informationText = fontCreateTextBitMapFromStr(s_pMenuFont, "KEY 1 = A500   KEY 2 = A1200   KEY 3 = A3000");
	fontDrawTextBitMap(s_pBuffer->pBack, informationText, 50, PLANEHEIGHT/2, 3, FONT_LEFT);

	while(hardwareSelection == 0) //A500
	{
		if (keyCheck(KEY_1))
		{
			renderingDepth = 16;
			debugValue = 8;
			debugValue2 = 2;
			debugValue3 = 2;
			debugValue4 = 4;

			debugValue6 = 4;
			xFOV = 10;
			RecalculateOdd();
			hardwareSelection = 1;
		}
		if (keyCheck(KEY_2)) //A1200
		{
			renderingDepth = 32;
			debugValue = 4;
			debugValue2 = 2;
			debugValue3 = 2;
			debugValue4 = 2;

			debugValue6 = 4;
			xFOV = 20;
			RecalculateEven();
			hardwareSelection = 2;
		}
		if (keyCheck(KEY_3)) //A3000
		{
			renderingDepth = 64;
			debugValue = 6;
			debugValue2 = 2;
			debugValue3 = 2;
			debugValue4 = 1;

			debugValue6 = 4;
			xFOV = 10;
			RecalculateOdd();
			hardwareSelection = 3;
		}
	}
	//*********************************** SELECT HARDWARE ***********************************************
	ClearBuffor();
	DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
	
	viewLoad(s_pView);
	vPortWaitForEnd(s_pVPort);
	CopyFastToChipW(s_pBuffer->pBack);

	lastTime = timerGetPrec();
	screenDuration = 10000000;



	//memcpy(s_pVPort->pPalette, kolory2, 16 * sizeof(UWORD));
}

//****************************** LOOP
void engineGsLoop(void)
{
	logAvgBegin(s_pAvgTime);
	joyProcess();
	//	keyProcess();
	startTime = timerGetPrec();
	deltaTime = startTime - lastTime;
	lastTime = startTime;
	levelTime += deltaTime;
	if (screenIndex == 0)   //turned off
	{
		if(screenDuration > 10000000)
		{
			screenDuration = 10000000;
			screenIndex = (screenIndex + 1) % 4;
			switchIntroScreen();
		}

		animateIntro();

		screenDuration -= deltaTime;
	}
	else
	{
		if(infoScreen != 0)  //turned off
		{
			UBYTE infoIndex = 0;
			UBYTE FireDown = 0;

			pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "In the distant future, on the distant,");
			pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "inhospitable planet, mankind engages in one");
			pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "thing it knows well - war. The tyrannical,");
			pBitmapInfo[3] = fontCreateTextBitMapFromStr(s_pMenuFont, "corporate regime wants to enslave all");
			pBitmapInfo[4] = fontCreateTextBitMapFromStr(s_pMenuFont, "inhabitants of the Kingdom - humanity's new home.");
			pBitmapInfo[5] = fontCreateTextBitMapFromStr(s_pMenuFont, "As a young smuggler aboard the aircraft Icarus");
			pBitmapInfo[6] = fontCreateTextBitMapFromStr(s_pMenuFont, "you want to help the oppressed by joining the");
			pBitmapInfo[7] = fontCreateTextBitMapFromStr(s_pMenuFont, "Revolt, but you need to prove your worth first");
			pBitmapInfo[8] = fontCreateTextBitMapFromStr(s_pMenuFont, "by completing the infamous Death Run challenge...");
			fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[0], 95, 100, 12, FONT_LEFT);
			fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[1], 95, 106, 12, FONT_LEFT);
			fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[2], 95, 112, 12, FONT_LEFT);
			fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[3], 95, 118, 12, FONT_LEFT);
			fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[4], 95, 124, 12, FONT_LEFT);
			fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[5], 95, 130, 12, FONT_LEFT);
			fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[6], 95, 136, 12, FONT_LEFT);
			fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[7], 95, 142, 12, FONT_LEFT);
			fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[8], 95, 148, 12, FONT_LEFT);

			while(!infoScreen)
			{
				joyProcess();
				if(joyCheck(JOY1_FIRE) && !FireDown)
				{
					infoIndex += 1;
					
					switch(infoIndex)
					{
						case 1:
						{
							for(int i = 0; i < 9; ++i)
							{
								free(pBitmapInfo[i]);
							}
							ClearBuffor();
							DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
							
							for (int i = 0; i < 16; i++)
							{
								bitmapPalette[i] = ((bitmapPalette1[i * 4 + 2] >> 4) << 8) +
													((bitmapPalette1[i * 4 + 1] >> 4) << 4) + (bitmapPalette1[i * 4 + 0] >> 4);
							}
							memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
							
							viewLoad(s_pView);
							vPortWaitForEnd(s_pVPort);
							CopyFastToChipW(s_pBuffer->pBack);

							pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "After setting off the Earth in enormous");
							pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "generation ships, humanity reached the nearest");
							pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "star system and colonized its' only suitable");
							pBitmapInfo[3] = fontCreateTextBitMapFromStr(s_pMenuFont, "planet, renaming it the Kingdom. The harsh");
							pBitmapInfo[4] = fontCreateTextBitMapFromStr(s_pMenuFont, "environment was one problem, the ideological");
							pBitmapInfo[5] = fontCreateTextBitMapFromStr(s_pMenuFont, "differences were the other. People of the");
							pBitmapInfo[6] = fontCreateTextBitMapFromStr(s_pMenuFont, "Kingdom fell prey to their nature, which");
							pBitmapInfo[7] = fontCreateTextBitMapFromStr(s_pMenuFont, "pushed them to the open conflict between");
							pBitmapInfo[8] = fontCreateTextBitMapFromStr(s_pMenuFont, "the Reign, and the Revolt.");
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[0], 95, 100, 12, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[1], 95, 106, 12, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[2], 95, 112, 12, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[3], 95, 118, 12, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[4], 95, 124, 12, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[5], 95, 130, 12, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[6], 95, 136, 12, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[7], 95, 142, 12, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[8], 95, 148, 12, FONT_LEFT);

							FireDown = 1;
						} break;
						case 2:
						{
							for(int i = 0; i < 9; ++i)
							{
								free(pBitmapInfo[i]);
							}
							ClearBuffor();
							DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
							
							for (int i = 0; i < 16; i++)
							{
								bitmapPalette[i] = ((bitmapPalette1[i * 4 + 2] >> 4) << 8) +
													((bitmapPalette1[i * 4 + 1] >> 4) << 4) + (bitmapPalette1[i * 4 + 0] >> 4);
							}
							memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
							
							viewLoad(s_pView);
							vPortWaitForEnd(s_pVPort);
							CopyFastToChipW(s_pBuffer->pBack);

							pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "You are Nix, the pilot of Icarus, who feels");
							pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "the urge to help the Revolt in overthrowing");
							pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "the Reign. You want to become the Carrier,");
							pBitmapInfo[3] = fontCreateTextBitMapFromStr(s_pMenuFont, "who supplies the military units of the Revolt.");
							pBitmapInfo[4] = fontCreateTextBitMapFromStr(s_pMenuFont, "As Carriers can move freely only in the deep");
							pBitmapInfo[5] = fontCreateTextBitMapFromStr(s_pMenuFont, "canyons covering the planet, the Revolt wants");
							pBitmapInfo[6] = fontCreateTextBitMapFromStr(s_pMenuFont, "you to prove your abilities and loyalty by");
							pBitmapInfo[7] = fontCreateTextBitMapFromStr(s_pMenuFont, "beating the route leading through narrow");
							pBitmapInfo[8] = fontCreateTextBitMapFromStr(s_pMenuFont, "valleys in the best time possible...");
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[0], 95, 100, 12, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[1], 95, 106, 12, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[2], 95, 112, 12, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[3], 95, 118, 12, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[4], 95, 124, 12, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[5], 95, 130, 12, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[6], 95, 136, 12, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[7], 95, 142, 12, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[8], 95, 148, 12, FONT_LEFT);

							FireDown = 1;
						} break;
						case 3:
						{
							for(int i = 0; i < 9; ++i)
							{
								free(pBitmapInfo[i]);
							}

							systemUse();
							bitmap1 = LoadBitmapFile("data/menu2.bmp", &bitmapHeader1, bitmapPalette1);
							systemUnuse();
							ClearBuffor();
							DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
							
							for (int i = 0; i < 16; i++)
							{
								bitmapPalette[i] = ((bitmapPalette1[i * 4 + 2] >> 4) << 8) +
													((bitmapPalette1[i * 4 + 1] >> 4) << 4) + (bitmapPalette1[i * 4 + 0] >> 4);
							}
							memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
							
							viewLoad(s_pView);
							vPortWaitForEnd(s_pVPort);
							CopyFastToChipW(s_pBuffer->pBack);

							pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "The ship's Anti-G engine uses the");
							pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "planet's magnetic field to move");
							pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "vertically. The closer you get to");
							pBitmapInfo[3] = fontCreateTextBitMapFromStr(s_pMenuFont, "the surface, the more energy is");
							pBitmapInfo[4] = fontCreateTextBitMapFromStr(s_pMenuFont, "generated. This energy is converted");
							pBitmapInfo[5] = fontCreateTextBitMapFromStr(s_pMenuFont, "to power ship's propulsion engines");
							pBitmapInfo[6] = fontCreateTextBitMapFromStr(s_pMenuFont, "and accelerate it. Remember, the");
							pBitmapInfo[7] = fontCreateTextBitMapFromStr(s_pMenuFont, "lower you fly, the faster you go.");
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[0], 110, 70, 6, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[1], 110, 76, 6, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[2], 110, 82, 6, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[3], 110, 88, 6, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[4], 110, 94, 6, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[5], 110, 100, 6, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[6], 110, 106, 6, FONT_LEFT);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[7], 110, 112, 6, FONT_LEFT);

							FireDown = 1;
						} break;
						case 4:
						{
							for(int i = 0; i < 8; ++i)
							{
								free(pBitmapInfo[i]);
							}
							ClearBuffor();
							SetGamePaletter();
							viewLoad(s_pView);
							vPortWaitForEnd(s_pVPort);
							CopyFastToChipW(s_pBuffer->pBack);
							infoScreen = 1;
							lastTime = timerGetPrec();
							startTime = timerGetPrec();
							deltaTime = 0;
						} break;
					}
				}
				else if(FireDown && !joyCheck(JOY1_FIRE))
				{
					FireDown = 0;
				}
			}
		}

		if (keyCheck(KEY_ESCAPE))
		{
			gameClose();
		}



		ProcessPlayerInput();
		OverwriteMap(); //this is how we go through many different maps, we just overwrite the main array with new content

		//restart
		if ((p1h - 3) < (UBYTE)(mapHigh[(UBYTE)(p1x)][(UBYTE)(p1y + 15)]))
		{
			for (int i = 0; i < 16; i++)
			{
				bitmapPalette[i] = ((bitmapPalette1[i * 4 + 2] >> 4) << 8) +
									((bitmapPalette1[i * 4 + 1] >> 4) << 4) + (bitmapPalette1[i * 4 + 0] >> 4);
			}
			memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
			DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
			viewLoad(s_pView);
			vPortWaitForEnd(s_pVPort);
			CopyFastToChipW(s_pBuffer->pBack);
			pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "You are dead!");
			pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "The Revolt won't have any use of");
			pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "you in this state!");
			fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[0], 110, 70, 6, FONT_LEFT);
			fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[1], 110, 76, 6, FONT_LEFT);
			fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[2], 110, 82, 6, FONT_LEFT);
											
			joyProcess();
			//wait 2 seconds
			while(!joyCheck(JOY1_FIRE))
			{

				joyProcess();
			}

			p1xf = 60 * 100;
			p1yf = 0;
			p1hf = 20 * 100;
			
			velocity = 0;
			acceleration = 0;
			points = 0;
			cx = 0;
			cy = 0;
			levelTime = 0;
			deltaTime = 0;
			lastOverwrittenLine = 0;

			CopyMapWord(mapSource[0], mapHigh);
			lastTime = timerGetPrec();
			
			for(int i = 0; i < 3; ++i)
			{
				free(pBitmapInfo[i]);
			}

			ClearBuffor();
			SetGamePaletter();
			viewLoad(s_pView);
			vPortWaitForEnd(s_pVPort);
			CopyFastToChipW(s_pBuffer->pBack);
		}
		else
		{
			
			ProcessQualityInput();
			



			xOffsetOdd = cx / 300;
			xOffsetEven = cx / 450;

			RenderQuality();

		//draw crosshair
		DrawPixel((160 + (cx / 150)) / 16, 110 + (cy / 200) + 4, 0);
		DrawPixel((160 + (cx / 150)) / 16, 110 + (cy / 200) - 4, 0);

			//DrawBitmap8b(bitmap1, &bitmapHeader1);

			vPortWaitForEnd(s_pVPort);
			CopyFastToChipW(s_pBuffer->pBack);
		}
		

	}

	//ConvertIntToChar((lastOverwrittenLine / 256 + 1) % MAPLENGTH, sPlayerX);
	//ConvertIntToChar(p1y, sPlayerY);
	//ConvertIntToChar(bcLogo[2], sPlayerH);
	//timerFormatPrec(sTime, startTime);
	//ConvertIntToChar(bcLogo[3], sTime);
	ConvertIntToChar(points, sVelocity);
	ConvertIntToChar(velocity, sScore);

	/*for (int i = 0; i < 16; i++)
	{
		ConvertWordToChar(bitmapPalette[i], sPixel[i]);
		fontFillTextBitMap(s_pMenuFont, pixel[i], sPixel[i]);
		fontDrawTextBitMap(s_pBuffer->pBack, pixel[i], 20, i * 8, 15, FONT_LEFT);
	}*/

	//fontFillTextBitMap(s_pMenuFont, pBitmapPlayerX, sPlayerX);
	//fontFillTextBitMap(s_pMenuFont, pBitmapPlayerY, sPlayerY);
	//fontFillTextBitMap(s_pMenuFont, pBitmapPlayerH, sPlayerH);
	//fontFillTextBitMap(s_pMenuFont, pBitmapTime, sTime);
	fontFillTextBitMap(s_pMenuFont, pBitmapVelocity, sVelocity);
	fontFillTextBitMap(s_pMenuFont, pBitmapScore, sScore);

	//fontDrawTextBitMap(s_pBuffer->pBack, pBitmapPlayerX, 00, 225, 15, FONT_LEFT);
	//fontDrawTextBitMap(s_pBuffer->pBack, pBitmapPlayerY, 40, 225, 15, FONT_LEFT);
	//fontDrawTextBitMap(s_pBuffer->pBack, pBitmapPlayerH, 60, 225, 15, FONT_LEFT);
	//fontDrawTextBitMap(s_pBuffer->pBack, pBitmapTime, 80, 225, 12, FONT_LEFT);
	fontDrawTextBitMap(s_pBuffer->pBack, pBitmapVelocity, 100, 225, 12, FONT_LEFT);
	fontDrawTextBitMap(s_pBuffer->pBack, pBitmapScore, 150, 225, 12, FONT_LEFT);

	interlace++;
	if (interlace == 4)
		interlace = 0;

	logAvgEnd(s_pAvgTime);
	endTime = timerGetPrec();
}

//****************************** DESTROY
void engineGsDestroy(void)
{
	systemUse();
	joyClose();
	viewLoad(0);
	fontDestroy(s_pMenuFont);
	viewDestroy(s_pView);

	fontDestroyTextBitMap(pBitmapPlayerX);
	fontDestroyTextBitMap(pBitmapPlayerY);

	char szAvg[15];
	timerFormatPrec(szAvg, endTime - startTime);
	free(bitmap1);
	free(bitmap2);
	free(bitmap3);
	free(bitmap4);
	
	free(paletteBitmap);

	printf("%s", szAvg);
	printf("%lu", deltaTime);
	logAvgDestroy(s_pAvgTime);
}

/*
if(debugValue == 9)
{
	 systemSetDma(DMAB_RASTER, 0);
	 systemSetDma(DMAB_DISK, 0);
	 systemSetDma(DMAB_SPRITE, 0);
	 systemSetDma(DMAB_BLITTER , 0);
	 systemSetDma(DMAB_COPPER  , 0);
	 systemSetDma(DMAB_BLITHOG  , 0);
}
else
{
	systemSetDma(DMAB_RASTER, 1);
	systemSetDma(DMAB_DISK, 1);
	systemSetDma(DMAB_SPRITE, 1);
	systemSetDma(DMAB_BLITTER , 1);
	systemSetDma(DMAB_COPPER  , 1);
	systemSetDma(DMAB_BLITHOG  , 1);
}*/

/*
	if(keyCheck(KEY_Q)){debugValue2=1;Recalculate();}
	if(keyCheck(KEY_W)){debugValue2=2;Recalculate();}
	if(keyCheck(KEY_E)){debugValue2=3;Recalculate();}
	if(keyCheck(KEY_R)){debugValue2=4;Recalculate();}
	if(keyCheck(KEY_T)){debugValue2=5;Recalculate();}
	if(keyCheck(KEY_Y)){debugValue2=6;Recalculate();}
	if(keyCheck(KEY_U)){debugValue2=7;Recalculate();}
	if(keyCheck(KEY_I)){debugValue2=8;Recalculate();}
	if(keyCheck(KEY_O)){debugValue2=9;Recalculate();}
	if(keyCheck(KEY_P)){debugValue2=10;Recalculate();}

	if(keyCheck(KEY_A)){debugValue3=1;Recalculate();}
	if(keyCheck(KEY_S)){debugValue3=2;Recalculate();}
	if(keyCheck(KEY_D)){debugValue3=3;Recalculate();}
	if(keyCheck(KEY_F)){debugValue3=4;Recalculate();}
	if(keyCheck(KEY_G)){debugValue3=5;Recalculate();}
	if(keyCheck(KEY_H)){debugValue3=6;Recalculate();}
	if(keyCheck(KEY_J)){debugValue3=7;Recalculate();}
	if(keyCheck(KEY_K)){debugValue3=8;Recalculate();}
	if(keyCheck(KEY_L)){debugValue3=9;Recalculate();}
	if(keyCheck(KEY_SEMICOLON)){debugValue3=10;Recalculate();}

	if(keyCheck(KEY_Z)){debugValue4=1;Recalculate();}
	if(keyCheck(KEY_X)){debugValue4=2;Recalculate();}
	if(keyCheck(KEY_C)){debugValue4=3;Recalculate();}
	if(keyCheck(KEY_V)){debugValue4=4;Recalculate();}
	if(keyCheck(KEY_B)){debugValue4=5;Recalculate();}
	if(keyCheck(KEY_N)){debugValue4=6;Recalculate();}
	if(keyCheck(KEY_M)){debugValue4=7;Recalculate();}
	if(keyCheck(KEY_COMMA)){debugValue4=8;Recalculate();}
	if(keyCheck(KEY_PERIOD)){debugValue4=9;Recalculate();}
	if(keyCheck(KEY_SLASH)){debugValue4=10;Recalculate();}
	*/