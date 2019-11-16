#include <ace/generic/main.h>
#include "engine.h"
#include "ray_casting.c"
#include "ray_cast_calculate.c"
#include "draw_screen.c"
//#include "mipmaps.c"
#include "file_read.c"
#include "bitmap_filters.c"
#include "dithering.c"
#include "draw_ships.c"
#include "draw_sprite.c"
#include "draw_maps.c"
#include "input_handling.c"
#include "map_streaming.c"
#include "setup_maps.c"
#include "rendering_quality.c"
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

void genericCreate(void) {
	gamePushState(engineGsCreate, engineGsLoop, engineGsDestroy);
}

void genericProcess(void) {
	gameProcess();
}

void genericDestroy(void) {

}

static tView *s_pView;
static tVPort *s_pVPort;
static tSimpleBufferManager *s_pBuffer;
static tAvg *s_pAvgTime;


void SetDefaulResolution()
{
		renderingDepth = 64;
		debugValue=7;
		debugValue2 = 7;
		debugValue3 = 1;
		debugValue4 = 0;

		debugValue6 = 4;
		xFOV = 10;
		Recalculate();
}
void Recalculate()
{
	CalculateRayCasts(rayCastXEven, rayCastYEven, XSIZEEVEN, YSIZEEVEN,2);
	CalculateRayCasts(rayCastXOdd, rayCastYOdd, XSIZEODD, YSIZEODD,1);
	deltaTime = 0;
}

void ConvertIntToChar( int number, char *test)
{
	int temp;
	int i = 9;
	while(number!=0)
	{
		temp=number%10;
		number/=10;
		switch(temp)
		{
			case 1: test[i] = '1';break;
			case 2: test[i] = '2';break;
			case 3: test[i] = '3';break;
			case 4: test[i] = '4';break;
			case 5: test[i] = '5';break;
			case 6: test[i] = '6';break;
			case 7: test[i] = '7';break;
			case 8: test[i] = '8';break;
			case 9: test[i] = '9';break;
			case 0: test[i] = '0';break;
		}
		i--;
	}
	while(i>=0)
	{
		test[i] = ' ';
		i--;
	}
}

tTextBitMap *pBitmapPlayerX, *pBitmapPlayerY, *pBitmapPlayerH;
char sPlayerX[10],sPlayerY[10],sPlayerH[10];

//****************************** CREATE
void engineGsCreate(void)
{
	s_pView = viewCreate(0,
		TAG_VIEW_GLOBAL_CLUT, 1,
		TAG_DONE
	);


	s_pVPort = vPortCreate(0,
		TAG_VPORT_VIEW, s_pView,
		TAG_VPORT_BPP, DEPTH,
		TAG_END
	);

	s_pBuffer = simpleBufferCreate(0,
		TAG_SIMPLEBUFFER_VPORT, s_pVPort,
		TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR,
		TAG_DONE
	);

		// Load font
	s_pMenuFont = fontCreate("silkscreen.fnt");


	p1xf = 60*100;
	p1yf = 0;
	p1hf = 40*100;

	p2x = 0;
	p2y = 0;
	p2h = 10;

	lastOverwrittenLine = 0;

	SetupMaps();

	GenerateWordDither8x8();
	GenerateColorBytesNoDither4x4();
	GenerateColorBytesDither3x2();

	SetDefaulResolution();


	memcpy(s_pVPort->pPalette, kolory, 16 * sizeof(UWORD));

	s_pAvgTime = logAvgCreate("perf", 100);

	viewLoad(s_pView);
	keyCreate();
	joyOpen(0);
	systemUnuse();

	pBitmapPlayerX = fontCreateTextBitMapFromStr(s_pMenuFont, "ACE Showcase");
	pBitmapPlayerY = fontCreateTextBitMapFromStr(s_pMenuFont, "ACE Showcase");
	pBitmapPlayerH = fontCreateTextBitMapFromStr(s_pMenuFont, "ACE Showcase");
}



//****************************** LOOP
void engineGsLoop(void) {
	logAvgBegin(s_pAvgTime);
	joyProcess();
//	keyProcess();
	startTime = timerGetPrec();
	deltaTime = startTime - lastTime;
	lastTime = startTime;

if(keyCheck(KEY_SPACE)) {
	gameClose();
}
else
{
	if(keyCheck(KEY_Q)){debugValue5=1;}
	if(keyCheck(KEY_W)){debugValue5=2;}
	if(keyCheck(KEY_E)){debugValue5=3;}
	if(keyCheck(KEY_R)){debugValue5=4;}
	if(keyCheck(KEY_T)){debugValue5=5;}
	if(keyCheck(KEY_Y)){debugValue5=6;}
	if(keyCheck(KEY_U)){debugValue5=7;}
	if(keyCheck(KEY_I)){debugValue5=8;}
	if(keyCheck(KEY_O)){debugValue5=9;}
	if(keyCheck(KEY_P)){debugValue5=10;}

	if(keyCheck(KEY_A)){debugValue6=1;Recalculate();}
	if(keyCheck(KEY_S)){debugValue6=2;Recalculate();}
	if(keyCheck(KEY_D)){debugValue6=3;Recalculate();}
	if(keyCheck(KEY_F)){debugValue6=4;Recalculate();}
	if(keyCheck(KEY_G)){debugValue6=5;Recalculate();}
	if(keyCheck(KEY_H)){debugValue6=6;Recalculate();}
	if(keyCheck(KEY_J)){debugValue6=7;Recalculate();}
	if(keyCheck(KEY_K)){debugValue6=8;Recalculate();}
	if(keyCheck(KEY_L)){debugValue6=9;Recalculate();}
	if(keyCheck(KEY_SEMICOLON)){debugValue6=10;Recalculate();}

	/*if(keyCheck(KEY_Z)){debugValue7=2;Recalculate();}
	if(keyCheck(KEY_X)){debugValue7=4;Recalculate();}
	if(keyCheck(KEY_C)){debugValue7=6;Recalculate();}
	if(keyCheck(KEY_V)){debugValue7=8;Recalculate();}
	if(keyCheck(KEY_B)){debugValue7=10;Recalculate();}
	if(keyCheck(KEY_N)){debugValue7=12;Recalculate();}
	if(keyCheck(KEY_M)){debugValue7=14;Recalculate();}
	if(keyCheck(KEY_COMMA)){debugValue7=16;Recalculate();}
	if(keyCheck(KEY_PERIOD)){debugValue7=18;Recalculate();}
	if(keyCheck(KEY_SLASH)){debugValue7=20;Recalculate();}*/
	
	ProcessPlayerInput();
	OverwriteMap(); //this is how we go through many different maps, we just overwrite the main array with new content

//restart
	if( (p1h-3) < (UBYTE)(mapHigh[(UBYTE)(p1x)][(UBYTE)(p1y+10)]) ) 
	{
		p1xf = 60*100;
		p1yf = 0;
		p1hf = 10*100;
		CopyMapWord(mapHigh0, mapHigh);
		CopyMapWord(mapMed0, mapMed);
		CopyMapWord(mapLow0, mapLow);
	}
	

 	ProcessQualityInput();
}


if(renderingDepth<10) renderingDepth = 10;
else if(renderingDepth>TERRAINDEPTH) renderingDepth = TERRAINDEPTH;

xOffsetOdd = cx/300;
xOffsetEven = cx/450;

RenderQuality();

//draw crosshair
DrawPixel((160+(cx/150))/16, YSIZEODD+(cy/100)+4, 0);
DrawPixel((160+(cx/150))/16, YSIZEODD+(cy/100)-4, 0);






vPortWaitForEnd(s_pVPort);
CopyFastToChipW(s_pBuffer->pBack);

ConvertIntToChar( p1x, sPlayerX);
ConvertIntToChar( p1y, sPlayerY);
ConvertIntToChar( p1h, sPlayerH);

fontFillTextBitMap(s_pMenuFont, pBitmapPlayerX, sPlayerX);
fontFillTextBitMap(s_pMenuFont, pBitmapPlayerY, sPlayerY);
fontFillTextBitMap(s_pMenuFont, pBitmapPlayerH, sPlayerH);
fontDrawTextBitMap(s_pBuffer->pBack, pBitmapPlayerX, 20, 225, 1, FONT_LEFT);
fontDrawTextBitMap(s_pBuffer->pBack, pBitmapPlayerY, 40, 235, 1, FONT_LEFT);
fontDrawTextBitMap(s_pBuffer->pBack, pBitmapPlayerH, 60, 245, 1, FONT_LEFT);

interlace++;
if(interlace == 4) interlace = 0;

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

	printf("%s", szAvg );
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