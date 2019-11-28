#define AMIGA amiga

#include <ace/generic/main.h>

#include "settings_amiga.h"
#include "engine.h"
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

static tView *s_pView;
static tVPort *s_pVPort;
static tSimpleBufferManager *s_pBuffer;
static tAvg *s_pAvgTime;

tTextBitMap *pBitmapPlayerX, *pBitmapPlayerY, *pBitmapPlayerH;
tTextBitMap *pBitmapTime, *pBitmapVelocity, *pBitmapScore;

tTextBitMap *pixel[32];
char sPixel[32][10];
char sPlayerX[5], sPlayerY[5], sPlayerH[5];
char sTime[10], sVelocity[5], sScore[5];
char fadeInStatus[4], fadeOutStatus[4];
unsigned char *currentPallete;

UBYTE gamePaletteSet = 0;

void Recalculate()
{
	CalculateRayCasts(rayCastXEven, rayCastYEven, XSIZEEVEN, YSIZEEVEN, 2); //było 2
	CalculateRayCasts(rayCastXOdd, rayCastYOdd, XSIZEODD, YSIZEODD, 1);
	deltaTime = 0;
}

void SetDefaulResolution()
{
		renderingDepth = 64;
		debugValue = 6;
		debugValue2 = 2;
		debugValue3 = 2;
		debugValue4 = 1;


	debugValue6 = 4;
	xFOV = 10;
	Recalculate();
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
	bitmap2 = LoadBitmapFile("data/logo2.bmp", &bitmapHeader2, bitmapPalette2);
	bitmap3 = LoadBitmapFile("data/logo3.bmp", &bitmapHeader3, bitmapPalette3);
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

	DrawBitmap8b(bitmap1, &bitmapHeader1);
	vPortWaitForEnd(s_pVPort);
	CopyFastToChipW(s_pBuffer->pBack);
	ULONG blitTime = timerGetPrec();

	
	// Load font
	s_pMenuFont = fontCreate("data/silkscreen.fnt");

	p1xf = 100 * 100;
	p1yf = 0;
	p1hf = 200 * 100;

	p2x = 0;
	p2y = 0;
	p2h = 10;

	lastOverwrittenLine = 0;

	SetupMaps();

	//GenerateWordDither8x8();
	GenerateColorBytesNoDither4x4();
	GenerateColorBytesDither3x2();

	SetDefaulResolution();

	pBitmapPlayerX = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");
	pBitmapPlayerY = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");
	pBitmapPlayerH = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");
	pBitmapTime = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");
	pBitmapVelocity = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");
	pBitmapScore = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");

	for (int i = 0; i < 32; i++)
		pixel[i] = fontCreateTextBitMapFromStr(s_pMenuFont, "1234567890");

	s_pAvgTime = logAvgCreate("perf", 100);
	levelTime = 0;
	screenIndex = 1;

	keyCreate();

	joyOpen(0);
	systemUnuse();
	screenDuration = 5000000;
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
	if (screenIndex > 0)
	{
		if (screenDuration > 5000000)
		{
			screenDuration = 5000000;
			screenIndex = (screenIndex + 1) % 4;

			switch (screenIndex)
			{
			case 2:
			{
				// for (int i = 0; i < 16; i++)
				// {
				// 	bitmapPalette[i] = ((bitmapPalette2[i * 4 + 2] >> 4) << 8) +
				// 					   ((bitmapPalette2[i * 4 + 1] >> 4) << 4) + (bitmapPalette2[i * 4 + 0] >> 4);
				// }
				
				currentPallete = bitmapPalette2;
				//memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
				DrawBitmap8b(bitmap2, &bitmapHeader2);
				
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
				// for (int i = 0; i < 16; i++)
				// {
				// 	bitmapPalette[i] = ((bitmapPalette3[i * 4 + 2] >> 4) << 8) +
				// 					   ((bitmapPalette3[i * 4 + 1] >> 4) << 4) + (bitmapPalette3[i * 4 + 0] >> 4);
				// }
				
				currentPallete = bitmapPalette3;
				//memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
				DrawBitmap8b(bitmap3, &bitmapHeader3);
				
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
				for (int i = 0; i < 16; i++)
				{
					bitmapPalette[i] = ((bitmapPalette3[i * 4 + 2] >> 4) << 8) +
									   ((bitmapPalette3[i * 4 + 1] >> 4) << 4) + (bitmapPalette3[i * 4 + 0] >> 4);
				}
				memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
				//memcpy(s_pVPort->pPalette, kolory2, 16 * sizeof(UWORD));
				viewLoad(s_pView);
			}
			break;
			}
		}

		if(screenDuration < 4600000 && !fadeInStatus[3])
		{
			//CopyPlane(plane4W, s_pBuffer->pBack->Planes[3]);
			for (int i = 0; i < 16; i++)
			{
			 	bitmapPalette[i] = ((((currentPallete[i * 4 + 2]/4) >> 4) << 8) +
			 					   (((currentPallete[i * 4 + 1]/4) >> 4) << 4) + ((currentPallete[i * 4 + 0]/4) >> 4));
			}
			fadeInStatus[3] = 1;
				viewLoad(s_pView);
		}
		if(screenDuration < 4200000 && !fadeInStatus[2])
		{
			for (int i = 0; i < 16; i++)
			{
			 	bitmapPalette[i] = ((((currentPallete[i * 4 + 2]/2) >> 4) << 8) +
			 					   (((currentPallete[i * 4 + 1]/2) >> 4) << 4) + ((currentPallete[i * 4 + 0]/2) >> 4));
			}
				memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
			//CopyPlane(plane3W, s_pBuffer->pBack->Planes[2]);
				viewLoad(s_pView);
			fadeInStatus[2] = 1;
		}
		if(screenDuration < 3800000 && !fadeInStatus[1])
		{
			for (int i = 0; i < 16; i++)
			{
			 	bitmapPalette[i] = ((((currentPallete[i * 4 + 2]*3/4) >> 4) << 8) +
			 					   (((currentPallete[i * 4 + 1]*3/4) >> 4) << 4) + ((currentPallete[i * 4 + 0]*3/4) >> 4));
			}
				memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
			//CopyPlane(plane2W, s_pBuffer->pBack->Planes[1]);
				viewLoad(s_pView);
			fadeInStatus[1] = 1;
		}
		if(screenDuration < 3400000 && !fadeInStatus[0])
		{
			for (int i = 0; i < 16; i++)
			{
			 	bitmapPalette[i] = (((currentPallete[i * 4 + 2] >> 4) << 8) +
			 					   ((currentPallete[i * 4 + 1] >> 4) << 4) + (currentPallete[i * 4 + 0] >> 4));
			}
				memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
			//CopyPlane(plane1W, s_pBuffer->pBack->Planes[0]);
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
			//ZeroPlane(s_pBuffer->pBack->Planes[0]);
			fadeOutStatus[0] = 1;
				viewLoad(s_pView);
		}
		if(screenDuration < 1200000 && !fadeOutStatus[1])
		{
			for (int i = 0; i < 16; i++)
			{
			 	bitmapPalette[i] = ((((currentPallete[i * 4 + 2]/2) >> 4) << 8) +
			 					   (((currentPallete[i * 4 + 1]/2) >> 4) << 4) + ((currentPallete[i * 4 + 0]/2) >> 4));
			}
				memcpy(s_pVPort->pPalette, bitmapPalette, 16 * sizeof(UWORD));
			//ZeroPlane(s_pBuffer->pBack->Planes[1]);
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
			//ZeroPlane(s_pBuffer->pBack->Planes[2]);
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
			//ZeroPlane(s_pBuffer->pBack->Planes[3]);
			viewLoad(s_pView);
			fadeOutStatus[3] = 1;
		}

		screenDuration -= deltaTime;
	}
	else
	{
		if(gamePaletteSet == 0)
		{
			SetGamePaletter();
			gamePaletteSet = 1;
		}

	
		if (keyCheck(KEY_SPACE))
		{
			gameClose();
		}
		else
		{
			if (keyCheck(KEY_Q))
			{
				debugValue5 = 1;
			}
			if (keyCheck(KEY_W))
			{
				debugValue5 = 2;
			}
			if (keyCheck(KEY_E))
			{
				debugValue5 = 3;
			}
			if (keyCheck(KEY_R))
			{
				debugValue5 = 4;
			}
			if (keyCheck(KEY_T))
			{
				debugValue5 = 5;
			}
			if (keyCheck(KEY_Y))
			{
				debugValue5 = 6;
			}
			if (keyCheck(KEY_U))
			{
				debugValue5 = 7;
			}
			if (keyCheck(KEY_I))
			{
				debugValue5 = 8;
			}
			if (keyCheck(KEY_O))
			{
				debugValue5 = 9;
			}
			if (keyCheck(KEY_P))
			{
				debugValue5 = 10;
			}

			if (keyCheck(KEY_A))
			{
				debugValue6 = 1;
				Recalculate();
			}
			if (keyCheck(KEY_S))
			{
				debugValue6 = 2;
				Recalculate();
			}
			if (keyCheck(KEY_D))
			{
				debugValue6 = 3;
				Recalculate();
			}
			if (keyCheck(KEY_F))
			{
				debugValue6 = 4;
				Recalculate();
			}
			if (keyCheck(KEY_G))
			{
				debugValue6 = 5;
				Recalculate();
			}
			if (keyCheck(KEY_H))
			{
				debugValue6 = 6;
				Recalculate();
			}
			if (keyCheck(KEY_J))
			{
				debugValue6 = 7;
				Recalculate();
			}
			if (keyCheck(KEY_K))
			{
				debugValue6 = 8;
				Recalculate();
			}
			if (keyCheck(KEY_L))
			{
				debugValue6 = 9;
				Recalculate();
			}
			if (keyCheck(KEY_SEMICOLON))
			{
				debugValue6 = 10;
				Recalculate();
			}

			if (keyCheck(KEY_Z))
			{
				debugValue8 = -2;
			}
			if (keyCheck(KEY_X))
			{
				debugValue8 = -1;
			}
			if (keyCheck(KEY_C))
			{
				debugValue8 = 0;
			}
			if (keyCheck(KEY_V))
			{
				debugValue8 = 1;
			}
			if (keyCheck(KEY_B))
			{
				debugValue8 = 2;
			}
			if (keyCheck(KEY_N))
			{
				debugValue9 = -2;
			}
			if (keyCheck(KEY_M))
			{
				debugValue9 = -1;
			}
			if (keyCheck(KEY_COMMA))
			{
				debugValue9 = 0;
			}
			if (keyCheck(KEY_PERIOD))
			{
				debugValue9 = 1;
			}
			if (keyCheck(KEY_SLASH))
			{
				debugValue9 = 2;
			}

			ProcessPlayerInput();
			OverwriteMap(); //this is how we go through many different maps, we just overwrite the main array with new content

			//restart
			// if ((p1h - 3) < (UBYTE)(mapHigh[(UBYTE)(p1x)][(UBYTE)(p1y + 15)]))
			// {
			// 	p1xf = 64 * 100;
			// 	p1yf = 0;
			// 	p1hf = 50 * 100;
			// 	CopyMapWord(mapSource[0], mapHigh);

			// 	levelTime = 0;
			// }

			ProcessQualityInput();
		}



		xOffsetOdd = cx / 300;
		xOffsetEven = cx / 450;

		RenderQuality();

		//draw crosshair
		DrawPixel((160 + (cx / 150)) / 16, YSIZEODD + (cy / 100) + 4, 0);
		DrawPixel((160 + (cx / 150)) / 16, YSIZEODD + (cy / 100) - 4, 0);

		//DrawBitmap8b(bitmap1, &bitmapHeader1);

	//	vPortWaitForEnd(s_pVPort);
		CopyFastToChipW(s_pBuffer->pBack);
	}

	//ConvertIntToChar(bcLogo[0], sPlayerX);
	ConvertIntToChar(p1y, sPlayerY);
	//ConvertIntToChar(bcLogo[2], sPlayerH);
	//timerFormatPrec(sTime, startTime);
	//ConvertIntToChar(bcLogo[3], sTime);
	//ConvertIntToChar(bcLogo[4], sVelocity);
	//ConvertIntToChar(bcLogo[5], sScore);

	/*for (int i = 0; i < 16; i++)
	{
		ConvertWordToChar(bitmapPalette[i], sPixel[i]);
		fontFillTextBitMap(s_pMenuFont, pixel[i], sPixel[i]);
		fontDrawTextBitMap(s_pBuffer->pBack, pixel[i], 20, i * 8, 15, FONT_LEFT);
	}*/

	//fontFillTextBitMap(s_pMenuFont, pBitmapPlayerX, sPlayerX);
	fontFillTextBitMap(s_pMenuFont, pBitmapPlayerY, sPlayerY);
	//fontFillTextBitMap(s_pMenuFont, pBitmapPlayerH, sPlayerH);
	//fontFillTextBitMap(s_pMenuFont, pBitmapTime, sTime);
	//fontFillTextBitMap(s_pMenuFont, pBitmapVelocity, sVelocity);
	//fontFillTextBitMap(s_pMenuFont, pBitmapScore, sScore);

	//fontDrawTextBitMap(s_pBuffer->pBack, pBitmapPlayerX, 20, 225, 15, FONT_LEFT);
	fontDrawTextBitMap(s_pBuffer->pBack, pBitmapPlayerY, 40, 225, 15, FONT_LEFT);
	//fontDrawTextBitMap(s_pBuffer->pBack, pBitmapPlayerH, 60, 225, 15, FONT_LEFT);
	//fontDrawTextBitMap(s_pBuffer->pBack, pBitmapTime, 80, 225, 12, FONT_LEFT);
	//fontDrawTextBitMap(s_pBuffer->pBack, pBitmapVelocity, 100, 225, 12, FONT_LEFT);
	//fontDrawTextBitMap(s_pBuffer->pBack, pBitmapScore, 120, 225, 12, FONT_LEFT);

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

	char szAvg[15];
	timerFormatPrec(szAvg, endTime - startTime);
	free(bitmap1);
	free(bitmap2);
	free(bitmap3);
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