#define AMIGA amiga

#include <ace/generic/main.h>

#include "settings_amiga.h"
#include "engine.h"
#include "ray_casting_progressive.c"
#include "ray_casting_amiga.c"

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

tTextBitMap *pBitmapHeightLabel, *pBitmapHeight, *pBitmapTime, *pBitmapTimeLabel, *informationText;
tTextBitMap *pBitmapVelocityLabel, *pBitmapVelocity, *pBitmapScore, *pBitmapScoreLabel, *pBitmapInfo[10];

tTextBitMap *pixel[32];
char sPixel[32][10];
char sPlayerX[5], sPlayerY[5], sPlayerH[5];
char sTime[8], sVelocity[5], sScore[8];
char fadeInStatus[4], fadeOutStatus[4];
UWORD crossHairX, crossHairY;

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
		free(bitmap1);
		systemUse();
		bitmap1 = LoadBitmapFile("data/l2", &bitmapHeader1, bitmapPalette1);
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
		free(bitmap1);
		systemUse();
		bitmap1 = LoadBitmapFile("data/l3", &bitmapHeader1, bitmapPalette1);
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
	}
}

void animateIntro()
{
	if(screenDuration < 7400000 && !fadeInStatus[3])
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
	if(screenDuration < 7300000 && !fadeInStatus[2])
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
	if(screenDuration < 7200000 && !fadeInStatus[1])
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
	if(screenDuration < 7100000 && !fadeInStatus[0])
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

	if(screenDuration < 400000 && !fadeOutStatus[0])
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
	if(screenDuration < 300000 && !fadeOutStatus[1])
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
	if(screenDuration < 200000 && !fadeOutStatus[2])
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
	if(screenDuration < 100000 && !fadeOutStatus[3])
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
void Recalculate()
{
	CalculateRayCasts(rayCastXEven, rayCastYEven, XSIZEEVEN, YSIZEEVEN, 2); //było 2
	CalculateRayCasts(rayCastXOdd, rayCastYOdd, XSIZEODD, YSIZEODD, 1);
}

void ConvertIntToChar(int number, char *test, int size)
{
	int temp;
	int i = size-2;
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
	test[size-1] = 0;
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

	//ship = LoadBitmapFile("data/icar32", &shipHeader, palettePalette);
	ship = LoadBitmapFile("data/icar48", &shipHeader, palettePalette);
	//ship = LoadBitmapFile("data/icar", &shipHeader, palettePalette);
	bitmap1 = LoadBitmapFile("data/l1", &bitmapHeader1, bitmapPalette1);
	paletteBitmap = LoadBitmapFile("data/plt", &paletteHeader, palettePalette);

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
	s_pMenuFont = fontCreate("data/ss.fnt");

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

	//pBitmapPlayerX = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");
	//pBitmapPlayerY = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");
	//pBitmapPlayerH = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");
	//pBitmapTime = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");
	pBitmapVelocity = fontCreateTextBitMapFromStr(s_pMenuFont, "1234");
	pBitmapScore = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567");
	pBitmapTime = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567");
	pBitmapHeight = fontCreateTextBitMapFromStr(s_pMenuFont, "1234");
	for(UBYTE i = 0; i < 10; ++i)
	{
		//pBitmapInfo[i] = fontCreateTextBitMapFromStr(s_pMenuFont, "123456789012345678901234567890123456789012345678");
	}

	//for (int i = 0; i < 32; i++)
	//	pixel[i] = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");

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
			renderingType = 1;
			calculationDepthDivider = 2;
			calculationDepthStep = 4;
			renderingDepthStep = 2;

			stepModifier = 16;
			xFOV = 32;
			yFOV = 12;
			RecalculateEven();
			hardwareSelection = 1;
		}
		if (keyCheck(KEY_2)) //A1200
		{
			renderingDepth = 32;
			renderingType = 4;
			calculationDepthDivider = 2;
			calculationDepthStep = 2;
			renderingDepthStep = 2;

			stepModifier = 16;
			xFOV = 28;
			yFOV = 10;
			RecalculateEven();
			hardwareSelection = 2;
		}
		if (keyCheck(KEY_3)) //A3000
		{
			renderingDepth = 64;
			renderingType = 6;
			calculationDepthDivider = 2;
			calculationDepthStep = 2;
			renderingDepthStep = 1;

			stepModifier = 16;
			xFOV = 10;
			yFOV = 18;
			RecalculateOdd();
			hardwareSelection = 3;
		}
	}
	hardwareSelection = 0;
	fontDestroyTextBitMap(informationText);
	//*********************************** SELECT HARDWARE ***********************************************
	ClearBuffor();
	DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
	
	viewLoad(s_pView);
	vPortWaitForEnd(s_pVPort);
	CopyFastToChipW(s_pBuffer->pBack);

	lastTime = timerGetPrec();
	screenDuration = 7500000;



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
	if (screenIndex > 0)   //turned off
	{
		levelTime = 0;
		if(screenDuration > 7500000)
		{
			screenDuration = 7500000;
			screenIndex = (screenIndex + 1) % 4;
			switchIntroScreen();
		}

		animateIntro();

		screenDuration -= deltaTime;
		if (keyCheck(KEY_ESCAPE))
		{
			gameClose();
		}
			
	}
	else
	{
		if(infoScreen == 0)  //turned off
		{
			free(bitmap1);
			systemUse();
			bitmap1 = LoadBitmapFile("data/m0", &bitmapHeader1, bitmapPalette1);
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
			//vPortWaitForEnd(s_pVPort);
			CopyFastToChipW(s_pBuffer->pBack);

			UBYTE infoIndex = 0;
			UBYTE FireDown = 0;


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
							
							free(bitmap1);
							systemUse();
							bitmap1 = LoadBitmapFile("data/m1", &bitmapHeader1, bitmapPalette1);
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
							//vPortWaitForEnd(s_pVPort);
							CopyFastToChipW(s_pBuffer->pBack);

							FireDown = 1;
						} break;
						case 2:
						{
							
							free(bitmap1);
							systemUse();
							bitmap1 = LoadBitmapFile("data/m2", &bitmapHeader1, bitmapPalette1);
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
							//vPortWaitForEnd(s_pVPort);
							CopyFastToChipW(s_pBuffer->pBack);

							FireDown = 1;
						} break;
						case 3:
						{

							free(bitmap1);
							systemUse();
							bitmap1 = LoadBitmapFile("data/msg", &bitmapHeader1, bitmapPalette1);
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
							//vPortWaitForEnd(s_pVPort);
							CopyFastToChipW(s_pBuffer->pBack);

							pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "The ship's Anti-G engine uses the");
							pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "planet's magnetic field to move");
							pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "vertically.");
							pBitmapInfo[3] = fontCreateTextBitMapFromStr(s_pMenuFont, "The closer you get to the surface,");
							pBitmapInfo[4] = fontCreateTextBitMapFromStr(s_pMenuFont, "the more energy is generated.");
							pBitmapInfo[5] = fontCreateTextBitMapFromStr(s_pMenuFont, "This energy is converted to power");
							pBitmapInfo[6] = fontCreateTextBitMapFromStr(s_pMenuFont, "ship's propulsion engines and");
							pBitmapInfo[7] = fontCreateTextBitMapFromStr(s_pMenuFont, "accelerate it.");
							pBitmapInfo[8] = fontCreateTextBitMapFromStr(s_pMenuFont, "Remember, the lower you fly,");
							pBitmapInfo[9] = fontCreateTextBitMapFromStr(s_pMenuFont, "the faster you go.");
							 
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[0], 100, 70, 4, FONT_LEFT|FONT_COOKIE);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[1], 100, 76, 4, FONT_LEFT|FONT_COOKIE);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[2], 100, 82, 4, FONT_LEFT|FONT_COOKIE);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[3], 100, 88, 4, FONT_LEFT|FONT_COOKIE);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[4], 100, 94, 4, FONT_LEFT|FONT_COOKIE);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[5], 100, 100, 4, FONT_LEFT|FONT_COOKIE);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[6], 100, 106, 4, FONT_LEFT|FONT_COOKIE);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[7], 100, 112, 4, FONT_LEFT|FONT_COOKIE);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[8], 100, 118, 4, FONT_LEFT|FONT_COOKIE);
							fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[9], 100, 124, 4, FONT_LEFT|FONT_COOKIE);

							FireDown = 1;
						} break;
						case 4:
						{
							for(int i = 0; i < 8; ++i)
							{
								fontDestroyTextBitMap(pBitmapInfo[i]);
							}
							ClearBuffor();
							SetGamePaletter();
							viewLoad(s_pView);
							//vPortWaitForEnd(s_pVPort);
							CopyFastToChipW(s_pBuffer->pBack);
							infoScreen = 1;
							lastTime = timerGetPrec();
							startTime = timerGetPrec();
							deltaTime = 0;
							levelTime = 0;

							
							pBitmapVelocityLabel = fontCreateTextBitMapFromStr(s_pMenuFont, "SPEED");
							pBitmapScoreLabel = fontCreateTextBitMapFromStr(s_pMenuFont, "SCORE");
							pBitmapTimeLabel = fontCreateTextBitMapFromStr(s_pMenuFont, "TIME");
							pBitmapHeightLabel = fontCreateTextBitMapFromStr(s_pMenuFont, "RELATIVE HEIGHT");
						} break;
					}
				}
				else if(FireDown && !joyCheck(JOY1_FIRE))
				{
					FireDown = 0;
				}
				
				if (keyCheck(KEY_ESCAPE))
				{
					infoScreen = 1;
					gameClose();
				}
			}
		}



	//	deltaTime = 0;
		ProcessPlayerInput();
		OverwriteMap(); //this is how we go through many different maps, we just overwrite the main array with new content

		//restart
		if ((p1h - 3) < (UBYTE)(mapHigh[(UBYTE)(p1x)][(UBYTE)(p1y + 15)]))
		{
			
			ClearBuffor();
			for (int i = 0; i < 16; i++)
			{
				bitmapPalette[i] = ((bitmapPalette1[i * 4 + 2] >> 4) << 8) +
									((bitmapPalette1[i * 4 + 1] >> 4) << 4) + (bitmapPalette1[i * 4 + 0] >> 4);
			}
			memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
			DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
			viewLoad(s_pView);
			//vPortWaitForEnd(s_pVPort);
			CopyFastToChipW(s_pBuffer->pBack);

			UBYTE lines = 0;

			if(levelTime & 1)
			{
				pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "You are dead!");
				pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "The Revolt won't have any use of");
				pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "you in this state!");
				lines = 3;
			}
			else
			{
				switch((levelTime >> 1) % 10)
				{
					case 0:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "Whoops");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "This had to hurt");
						lines = 2;
				
					} break;
					case 1:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "Maybe piloting the aircraft");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "wasn't your strongest trait");
						lines = 2;
					} break;
					case 2:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "Hope you had your");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "seatbelts fastened");
						lines = 2;
					} break;
					case 3:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "You died.");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "But don't worry, your problems");
						pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "are over, forever");
						lines = 3;
					} break;
					case 4:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "Who taught you to fly?");
						lines = 1;
					} break;
					case 5:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "Here lies Nix.");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "Good friend, but a terrible pilot");
						lines = 2;
					} break;
					case 6:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "Tis but a scratch!");
						lines = 1;
					} break;
					case 7:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "Wasted!");
						lines = 1;
					} break;
					case 8:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "This is the best you can do?");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "I've seen better");
						lines = 2;
					} break;
					case 9:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "Rest in peace, Nix");
						lines = 1;
					} break;
				}
			}
			
			UBYTE y = 70;
			for(int i = 0; i < lines; ++i)
			{	
				fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[i], 100, y, 4, FONT_LEFT|FONT_COOKIE);
				y += 6;
			}
			joyProcess();
			UBYTE cont = 0;
			//wait 2 seconds
			while(!cont)
			{

				joyProcess();
				if (keyCheck(KEY_ESCAPE))
				{
					gameClose();
					cont = 1;
				}
				else if(joyCheck(JOY1_FIRE))
				{
					cont = 1;
				}
				
				
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
			velocityDenom = 128;

			CopyMapWord(mapSource[0], mapHigh);
			lastTime = timerGetPrec();
			
			for(int i = 0; i < lines; ++i)
			{
				fontDestroyTextBitMap(pBitmapInfo[i]);
			}

			ClearBuffor();
			SetGamePaletter();
			viewLoad(s_pView);
			//vPortWaitForEnd(s_pVPort);
			CopyFastToChipW(s_pBuffer->pBack);
		}
		else if(endScreen)
		{
			if(points < 1000000)
			{
				ClearBuffor();
				for (int i = 0; i < 16; i++)
				{
					bitmapPalette[i] = ((bitmapPalette1[i * 4 + 2] >> 4) << 8) +
										((bitmapPalette1[i * 4 + 1] >> 4) << 4) + (bitmapPalette1[i * 4 + 0] >> 4);
				}
				memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
				DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
				viewLoad(s_pView);
				//vPortWaitForEnd(s_pVPort);
				CopyFastToChipW(s_pBuffer->pBack);

				UBYTE lines = 0;

				
				switch((levelTime) % 4)
				{
					case 0:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "Is this all you've got?");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "You call yourself a pilot?");
						lines = 2;
				
					} break;
					case 1:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "At this speed you wouldn't outrace");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "a cargo ship!");
						pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "Get out and train!");
						lines = 3;
					} break;
					case 2:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "You wouldn't survive a second");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "as a Carrier!");
						pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "Get lost!");
						lines = 3;
					} break;
					case 3:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "Well, about time!");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "No stop wasting our time and scram!");
						lines = 2;
					} break;
					
				}


				UBYTE y = 70;
				for(int i = 0; i < lines; ++i)
				{	
					fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[i], 100, y, 4, FONT_LEFT|FONT_COOKIE);
					y += 6;
				}
				joyProcess();
				UBYTE cont = 0;
				//wait 2 seconds
				while(!cont)
				{

					joyProcess();
					if (keyCheck(KEY_ESCAPE))
					{
						gameClose();
						cont = 1;
					}
					else if(joyCheck(JOY1_FIRE))
					{
						cont = 1;
					}
					
					
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
				velocityDenom = 128;
				endScreen = 0;

				CopyMapWord(mapSource[0], mapHigh);
				lastTime = timerGetPrec();
				
				for(int i = 0; i < lines; ++i)
				{
					fontDestroyTextBitMap(pBitmapInfo[i]);
				}

				ClearBuffor();
				SetGamePaletter();
				viewLoad(s_pView);
				//vPortWaitForEnd(s_pVPort);
				CopyFastToChipW(s_pBuffer->pBack);
			}
			else
			{
				free(bitmap1);
				systemUse();
				bitmap1 = LoadBitmapFile("data/fin", &bitmapHeader1, bitmapPalette1);
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
				//vPortWaitForEnd(s_pVPort);
				CopyFastToChipW(s_pBuffer->pBack);


				ClearBuffor();
				for (int i = 0; i < 16; i++)
				{
					bitmapPalette[i] = ((bitmapPalette1[i * 4 + 2] >> 4) << 8) +
										((bitmapPalette1[i * 4 + 1] >> 4) << 4) + (bitmapPalette1[i * 4 + 0] >> 4);
				}
				memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
				DrawBitmap4bCenter(bitmap1, &bitmapHeader1);
				viewLoad(s_pView);
				//vPortWaitForEnd(s_pVPort);
				CopyFastToChipW(s_pBuffer->pBack);

				UBYTE lines = 0;

				
				switch((levelTime) % 12)
				{
					case 0:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "Good job, pilot!");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "You have proven yourself");
						pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "worthy of becoming a Carrier");
						pBitmapInfo[3] = fontCreateTextBitMapFromStr(s_pMenuFont, "of the Revolt!");
						lines = 4;
				
					} break;
					case 1:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "This is what I call flying!");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "Great job!");
						lines = 2;
					} break;
					case 2:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "You're faster than a");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "swooping Vertiraptor!");
						lines = 2;
					} break;
					case 3:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "Impressive!");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "You surely know how to handle");
						pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "this baby!");
						lines = 3;
					} break;
					case 4:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "The Revolt is going to have");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "great use of you, pilot!");
						pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "Welcome aboard!");
						lines = 3;
					} break;
					case 5:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "Those pilots serving the Reign");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "are no match for you!");
						pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "Time to kick some ass, pilot!");
						lines = 3;
					} break;
					case 6:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "Congrats, you are in!");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "Something tells me the Reign");
						pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "is going to remember your name");
						pBitmapInfo[3] = fontCreateTextBitMapFromStr(s_pMenuFont, "real soon...");
						lines = 4;
					} break;
					case 7:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "This was crazy!");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "With that speed, you can");
						pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "probably get out of the orbit!");
						lines = 3;
					} break;
					case 8:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "This is incredible!");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "And you did it with a");
						pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "twin-engine drive!");
						pBitmapInfo[3] = fontCreateTextBitMapFromStr(s_pMenuFont, "I've seen quad-engine crafts");
						pBitmapInfo[4] = fontCreateTextBitMapFromStr(s_pMenuFont, "do worse!");
						pBitmapInfo[5] = fontCreateTextBitMapFromStr(s_pMenuFont, "Respect, pilot!");
						lines = 6;
					} break;
					case 9:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "The Revolt will have good use");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "of you, pilot!");
						pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "You're in!");
						lines = 3;
					} break;
					case 10:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "I haven't seen anyone fly like");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "this since my younger days!");
						pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "Congrats!");
						lines = 3;
					} break;
					case 11:
					{
						pBitmapInfo[0] = fontCreateTextBitMapFromStr(s_pMenuFont, "Great time!");
						pBitmapInfo[1] = fontCreateTextBitMapFromStr(s_pMenuFont, "I hope you're ready to repeat");
						pBitmapInfo[2] = fontCreateTextBitMapFromStr(s_pMenuFont, "it in the future, you're in!");
						lines = 3;
					} break;
					
				}


				UBYTE y = 30;
				for(int i = 0; i < lines; ++i)
				{	
					fontDrawTextBitMap(s_pBuffer->pBack, pBitmapInfo[i], 170, y, 1, FONT_LEFT|FONT_COOKIE);
					y += 6;
				}
				joyProcess();
				UBYTE cont = 0;
				//wait 2 seconds
				while(!cont)
				{

					joyProcess();
					if (keyCheck(KEY_ESCAPE))
					{
						gameClose();
						cont = 1;
					}
					else if(joyCheck(JOY1_FIRE))
					{
						cont = 1;
					}
					
					
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
				velocityDenom = 128;
				endScreen = 0;
				infoScreen = 0;

				CopyMapWord(mapSource[0], mapHigh);
				lastTime = timerGetPrec();
				
				for(int i = 0; i < lines; ++i)
				{
					fontDestroyTextBitMap(pBitmapInfo[i]);
				}

				ClearBuffor();
				SetGamePaletter();
				viewLoad(s_pView);
				//vPortWaitForEnd(s_pVPort);
				CopyFastToChipW(s_pBuffer->pBack);
			}
			
		}
		else
		{
			
		//	ProcessQualityInput();
			
			if(p1y > 11*256 && velocityDenom < 3*255)
			{
				velocityDenom = velocityDenom + 4;
			}
			else if(velocityDenom >= 3*255)
			{
				endScreen = 1;
			}
			


			xOffsetOdd = cx / 300;
			xOffsetEven = cx / 450;

			RenderQuality();

		//draw crosshair
			

			
					//draw only even lines 
			crossHairX =  (160 + (cx / 1000));
			crossHairY = ( 126 + (cy / 1000) );

			WORD spriteIndexX = 1;
			WORD spriteIndexY = 1;
			if(cx > 8000)
			{
				spriteIndexX = 2;
			}
			else if(cx < -8000)
			{
				spriteIndexX = 0;
			}
			if(cy > 4000)
			{
				spriteIndexY = 0;
			}
			else if(cy < -4000)
			{
				spriteIndexY = 2;
			}
			
			//crossHairX = ( (160 + (cx / 150)) / 16 );
			//crossHairY = ( 110 + (cy / 200) )/2;
			//DrawPixel( crossHairX, crossHairY*2 + 4, 0);
			//DrawPixel( crossHairX, crossHairY*2 - 4, 0);

			//DrawSprite4b(ship, &shipHeader, crossHairX, crossHairY,
			//			 spriteIndexX, spriteIndexY, 32, 32, 11);
			DrawSprite4b(ship, &shipHeader, crossHairX, crossHairY,
						 spriteIndexX, spriteIndexY, 48, 48, 15);
			//DrawSprite4b(ship, &shipHeader, crossHairX, crossHairY,
			//			 spriteIndexX, spriteIndexY, 64, 64, 6);

			//DrawBitmap8b(bitmap1, &bitmapHeader1);

			vPortWaitForEnd(s_pVPort);
			CopyFastToChipW(s_pBuffer->pBack);


	// 	if(keyCheck(KEY_Q)){calculationDepthDivider=1;Recalculate();}
	// if(keyCheck(KEY_W)){calculationDepthDivider=2;Recalculate();}
	// if(keyCheck(KEY_E)){calculationDepthDivider=3;Recalculate();}
	// if(keyCheck(KEY_R)){calculationDepthDivider=4;Recalculate();}
	// if(keyCheck(KEY_T)){calculationDepthDivider=5;Recalculate();}
	// if(keyCheck(KEY_Y)){calculationDepthDivider=6;Recalculate();}
	// if(keyCheck(KEY_U)){calculationDepthDivider=7;Recalculate();}
	// if(keyCheck(KEY_I)){calculationDepthDivider=8;Recalculate();}
	// if(keyCheck(KEY_O)){calculationDepthDivider=9;Recalculate();}
	// if(keyCheck(KEY_P)){calculationDepthDivider=10;Recalculate();}

	// if(keyCheck(KEY_A)){calculationDepthStep=1;Recalculate();}
	// if(keyCheck(KEY_S)){calculationDepthStep=2;Recalculate();}
	// if(keyCheck(KEY_D)){calculationDepthStep=3;Recalculate();}
	// if(keyCheck(KEY_F)){calculationDepthStep=4;Recalculate();}
	// if(keyCheck(KEY_G)){calculationDepthStep=5;Recalculate();}
	// if(keyCheck(KEY_H)){calculationDepthStep=6;Recalculate();}
	// if(keyCheck(KEY_J)){calculationDepthStep=7;Recalculate();}
	// if(keyCheck(KEY_K)){calculationDepthStep=8;Recalculate();}
	// if(keyCheck(KEY_L)){calculationDepthStep=9;Recalculate();}
	// if(keyCheck(KEY_SEMICOLON)){calculationDepthStep=10;Recalculate();}

	// if(keyCheck(KEY_Z)){renderingDepthStep=1;}
	// if(keyCheck(KEY_X)){renderingDepthStep=2;}
	// if(keyCheck(KEY_C)){renderingDepthStep=3;}
	// if(keyCheck(KEY_V)){renderingDepthStep=4;}
	// if(keyCheck(KEY_B)){renderingDepthStep=5;}
	// if(keyCheck(KEY_N)){renderingDepthStep=6;}
	// if(keyCheck(KEY_M)){renderingDepthStep=7;}
	// if(keyCheck(KEY_COMMA)){renderingDepthStep=8;}
	// if(keyCheck(KEY_PERIOD)){renderingDepthStep=9;}
	// if(keyCheck(KEY_SLASH)){renderingDepthStep=10;}
		}
		

	}

	if (keyCheck(KEY_ESCAPE))
	{
		gameClose();
	}

	//ConvertIntToChar((lastOverwrittenLine / 256 + 1) % MAPLENGTH, sPlayerX);
	//ConvertIntToChar(p1y, sPlayerY);
	//ConvertIntToChar(bcLogo[2], sPlayerH);
	//timerFormatPrec(sTime, startTime);
	//ConvertIntToChar(bcLogo[3], sTime);
	ConvertIntToChar(points, sScore, 8);
	ConvertIntToChar(velocity, sVelocity, 5);
	ConvertIntToChar(levelTime/2500, sTime, 8);
	ConvertIntToChar(relativeHeight, sPlayerH, 5);
	


	/*for (int i = 0; i < 16; i++)
	{
		ConvertWordToChar(bitmapPalette[i], sPixel[i]);
		fontFillTextBitMap(s_pMenuFont, pixel[i], sPixel[i]);
		fontDrawTextBitMap(s_pBuffer->pBack, pixel[i], 20, i * 8, 15, FONT_LEFT);
	}*/

	//fontFillTextBitMap(s_pMenuFont, pBitmapPlayerX, sPlayerX);
	//fontFillTextBitMap(s_pMenuFont, pBitmapPlayerY, sPlayerY);
	fontFillTextBitMap(s_pMenuFont, pBitmapScore, sScore);
	fontFillTextBitMap(s_pMenuFont, pBitmapVelocity, sVelocity);
	fontFillTextBitMap(s_pMenuFont, pBitmapTime, sTime);
	fontFillTextBitMap(s_pMenuFont, pBitmapHeight, sPlayerH);

	//fontDrawTextBitMap(s_pBuffer->pBack, pBitmapPlayerX, 00, 225, 15, FONT_LEFT);
	//fontDrawTextBitMap(s_pBuffer->pBack, pBitmapPlayerY, 40, 225, 15, FONT_LEFT);
	//fontDrawTextBitMap(s_pBuffer->pBack, pBitmapPlayerH, 60, 225, 15, FONT_LEFT);
	//fontDrawTextBitMap(s_pBuffer->pBack, pBitmapTime, 80, 225, 12, FONT_LEFT);
	fontDrawTextBitMap(s_pBuffer->pBack, pBitmapVelocityLabel, 75, 225, 12, FONT_LEFT|FONT_COOKIE);
	fontDrawTextBitMap(s_pBuffer->pBack, pBitmapScoreLabel, 0, 225, 12, FONT_LEFT|FONT_COOKIE);
	fontDrawTextBitMap(s_pBuffer->pBack, pBitmapHeightLabel, 135, 225, 12, FONT_LEFT|FONT_COOKIE);
	fontDrawTextBitMap(s_pBuffer->pBack, pBitmapTimeLabel, 230, 225, 12, FONT_LEFT|FONT_COOKIE);
	fontDrawTextBitMap(s_pBuffer->pBack, pBitmapVelocity, 100, 225, 12, FONT_LEFT|FONT_COOKIE);
	fontDrawTextBitMap(s_pBuffer->pBack, pBitmapScore, 35, 225, 12, FONT_LEFT|FONT_COOKIE);
	fontDrawTextBitMap(s_pBuffer->pBack, pBitmapHeight, 195, 225, 12, FONT_LEFT|FONT_COOKIE);
	fontDrawTextBitMap(s_pBuffer->pBack, pBitmapTime, 250, 225, 12, FONT_LEFT|FONT_COOKIE);

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

	fontDestroyTextBitMap(pBitmapVelocity);
	fontDestroyTextBitMap(pBitmapScore);
	fontDestroyTextBitMap(pBitmapTime);
	fontDestroyTextBitMap(pBitmapHeight);

	fontDestroyTextBitMap(pBitmapVelocityLabel);
	fontDestroyTextBitMap(pBitmapScoreLabel);
	fontDestroyTextBitMap(pBitmapTimeLabel);
	fontDestroyTextBitMap(pBitmapHeightLabel);

	char szAvg[15];
	timerFormatPrec(szAvg, endTime - startTime);
	free(bitmap1);
	
	free(paletteBitmap);

	printf("%s", szAvg);
	printf("%lu", deltaTime);
	logAvgDestroy(s_pAvgTime);
}

/*
if(renderingType == 9)
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


	
	