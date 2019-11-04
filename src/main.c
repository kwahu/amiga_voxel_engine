#include <ace/generic/main.h>
#include "ray_casting.h"
#include "ray_casting.c"
#include "draw_screen.c"
#include "mipmaps.c"
#include "file_read.c"
#include "bitmap_filters.c"
#include "dithering.c"
#include "draw_ships.c"
#include "draw_maps.c"
#include <ace/managers/game.h>
#include <ace/managers/timer.h>
#include <ace/managers/system.h>
#include <ace/managers/blit.h>
#include <ace/utils/file.h>

/*
docker run --rm \
-v ${PWD}:/work \
-v /path/to/extra/m68k-amigaos/lib:/tools/usr/lib \
-v /path/to/extra/m68k-amigaos/include:/tools/usr/include \
-it amigadev/crosstools:m68k-amigaos bash
*/

ULONG startTime,endTime,deltaTime,lastTime;

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


	p1xf = 40*15;
	p1yf = 0;
	p1hf = 30*15;

	p2x = 0;
	p2y = 0;
	p2h = 10;


	debugValue = 8;

	ReadHeight("height.raw");
	//	ReadPalette("palette.raw");
	ReadColor("color.raw");
	//CalculateRayCasts(rayCastXOdd, rayCastYOdd, XSIZEODD, YSIZE);
	//CalculateRayCasts(rayCastXEven, rayCastYEven, XSIZEEVEN, YSIZE);
	//CalculateRayCastsSlow(rayCastXEven, rayCastYEven);
	CalculateRayCastsSingle(32, 32);


	GenerateWordDither8x8();
//	GenerateColorBytesDither4x4();
	//GenerateColorBytesDither3x2();

	//SmoothHeightMap();
	CalculateColorMipMaps();
	CalculateHeightMipMaps();

	CombineMaps(heightMap2, colorMap2, map);//combine into WORDs


	//CalculateModulo2();
	//CalculateEnemyPlacement();


	memcpy(s_pVPort->pPalette, kolory, 16 * sizeof(UWORD));

	s_pAvgTime = logAvgCreate("perf", 100);

	viewLoad(s_pView);
	keyCreate();
	systemUnuse();
}

void engineGsLoop(void) {
	logAvgBegin(s_pAvgTime);
	startTime = timerGetPrec();
	deltaTime = startTime - lastTime;
	lastTime = startTime;

/*if(p1y == 250)
	{
	gameClose();
}
p1y += 1;
p2y += 1;*/

if(keyCheck(KEY_SPACE)) {
	gameClose();
}
else
{
	//	if(interlace % 2)
	{
		if(keyCheck(KEY_UP))p2y++;
		if(keyCheck(KEY_DOWN))p2y--;
		if(keyCheck(KEY_RIGHT))p2x++;
		if(keyCheck(KEY_LEFT))p2x--;
		if(keyCheck(KEY_F))p2h+=3;
		if(keyCheck(KEY_V))p2h-=3;
	}

	//	if(interlace % 2 == 0)
	{
		if(keyCheck(KEY_W))p1yf+=deltaTime/10000;
		if(keyCheck(KEY_S))p1yf-=deltaTime/10000;
		if(keyCheck(KEY_D))p1xf+=deltaTime/10000*3;
		if(keyCheck(KEY_A))p1xf-=deltaTime/10000*3;
		if(keyCheck(KEY_G))p1hf+=deltaTime/10000*3;
		if(keyCheck(KEY_B))p1hf-=deltaTime/10000*3;
	}
	if(keyCheck(KEY_H))renderingDepth++;
	if(keyCheck(KEY_N))renderingDepth--;

	if(keyCheck(KEY_0))debugValue=0;
	if(keyCheck(KEY_1))debugValue=1;
	if(keyCheck(KEY_2))debugValue=2;
	if(keyCheck(KEY_3))debugValue=3;
	if(keyCheck(KEY_4))debugValue=4;
	if(keyCheck(KEY_5))debugValue=5;
	if(keyCheck(KEY_6))debugValue=6;
	if(keyCheck(KEY_7))debugValue=7;
	if(keyCheck(KEY_8))debugValue=8;
	if(keyCheck(KEY_9))debugValue=9;
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
//		turn = turn/1.2;
//	xPos += turn;
if(renderingDepth<10) renderingDepth = 10;
else if(renderingDepth>TERRAINDEPTH) renderingDepth = TERRAINDEPTH;

p1y = p1yf/15;
p1x = p1xf/15;
p1h = p1hf/15;

if(p1h<5) p1h = 5;
if(p2h<5) p2h = 5;



/*
if(interlace == 0)
{
ProcessRayCasts(1);
DrawPlayerScreen(1,1,0);
}
if(interlace == 1)
{
ProcessRayCasts(2);
DrawPlayerScreen(2,0,0);
}
if(interlace == 2)
{
ProcessRayCasts(1);
DrawPlayerScreen(1,1,1);
}
if(interlace == 3)
{
ProcessRayCasts(2);
DrawPlayerScreen(2,0,1);
}
if(interlace == 4)
{
ProcessRayCasts(1);
DrawPlayerScreen(1,1,0);
}
if(interlace == 5)
{
ProcessRayCasts(2);
DrawPlayerScreen(2,0,0);
}
if(interlace == 6)
{
ProcessRayCasts(1);
DrawPlayerScreen(1,1,1);
}
if(interlace == 7)
{
ProcessRayCasts(2);
DrawPlayerScreen(2,0,1);
}*/
//DrawPlayerScreen(1,0,1);
//ProcessRayCasts(1,1,0);
if(debugValue == 1)
{

	ProcessRayCastsSlow(screen8x8a,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	0,  4, 4, 0, 8);
	ProcessRayCastsSlow(screen8x8b,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	32, 4, 4, 0, 8);
	ProcessRayCastsSlow(screen8x8c,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	64, 4, 4, 0, 8);
	ProcessRayCastsSlow(screen8x8d,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	96, 4, 4, 0, 8);
	//ProcessRayCastsSlow(screen8x8e,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	128, 4, 4, 0, 8);

	DrawPlayerScreen8x8Slow(screen8x8a,1,0,0,4);
	DrawPlayerScreen8x8Slow(screen8x8b,1,0,4,4);
	DrawPlayerScreen8x8Slow(screen8x8c,1,0,8,4);
	DrawPlayerScreen8x8Slow(screen8x8d,1,0,12,4);
	//DrawPlayerScreen8x8Slow(screen8x8e,1,0,16,4);
}
else if(debugValue == 2)
{
	ProcessRayCastsWithMipMaps(screen8x8a,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	0,  4, 4, 0, 8);
	ProcessRayCastsWithMipMaps(screen8x8b,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	32, 4, 4, 0, 8);
	ProcessRayCastsWithMipMaps(screen4x4c,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	64, 2, 2, 0, 16);
	ProcessRayCastsWithMipMaps(screen8x8d,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	96, 4, 4, 0, 8);
	ProcessRayCastsWithMipMaps(screen8x8e,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	128, 4, 4, 0, 8);

	DrawPlayerScreen8x8(screen8x8a,1,0,0,4);
	DrawPlayerScreen8x8(screen8x8b,1,0,4,4);
	DrawPlayerScreen4x4(screen4x4c,1,0,8,4);
	DrawPlayerScreen8x8(screen8x8d,1,0,12,4);
	DrawPlayerScreen8x8(screen8x8e,1,0,16,4);
}
else if(debugValue == 3)
{
	ProcessRayCastsWithMipMaps(screen8x8a,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	0,  4, 4, 0, 8);
	ProcessRayCastsWithMipMaps(screen4x4b,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	32, 2, 2, 0, 16);
	ProcessRayCastsWithMipMaps(screen4x4c,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	64, 2, 2, 0, 16);
	ProcessRayCastsWithMipMaps(screen4x4d,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	96, 2, 2, 0, 16);
	ProcessRayCastsWithMipMaps(screen8x8e,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	128, 4, 4, 0, 8);

	DrawPlayerScreen8x8(screen8x8a,1,0,0,4);
	DrawPlayerScreen4x4(screen4x4b,1,0,4,4);
	DrawPlayerScreen4x4(screen4x4c,1,0,8,4);
	DrawPlayerScreen4x4(screen4x4d,1,0,12,4);
	DrawPlayerScreen8x8(screen8x8e,1,0,16,4);
}
else if(debugValue == 4)
{
	ProcessRayCastsWithMipMaps(screen8x8a,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	0,  4, 4, 0, 8);
	ProcessRayCastsWithMipMaps(screen4x4b,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	32, 2, 2, 0, 16);
	ProcessRayCastsWithMipMaps(screen3x2c,rayCastXOdd, rayCastYOdd,p1x,p1y,p1h,		48, 1, 1, 0, 24);
	ProcessRayCastsWithMipMaps(screen4x4d,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	96, 2, 2, 0, 16);
	ProcessRayCastsWithMipMaps(screen8x8e,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	128, 4, 4, 0, 8);

	DrawPlayerScreen8x8(screen8x8a,1,0,0,4);
	DrawPlayerScreen4x4(screen4x4b,1,0,4,4);
	DrawPlayerScreen3x2(screen3x2c,1,0,8,4);
	DrawPlayerScreen4x4(screen4x4d,1,0,12,4);
	DrawPlayerScreen8x8(screen8x8e,1,0,16,4);
}
else if(debugValue == 5)
{
	ProcessRayCastsWithMipMaps(screen4x4a,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	0,  2, 2, 0, 16);
	ProcessRayCastsWithMipMaps(screen4x4b,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	32, 2, 2, 0, 16);
	ProcessRayCastsWithMipMaps(screen3x2c,rayCastXOdd, rayCastYOdd,p1x,p1y,p1h,		48, 1, 1, 0, 24);
	ProcessRayCastsWithMipMaps(screen4x4d,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	96, 2, 2, 0, 16);
	ProcessRayCastsWithMipMaps(screen4x4e,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	128, 2, 2, 0,16);

	DrawPlayerScreen4x4(screen4x4a,1,0,0,4);
	DrawPlayerScreen4x4(screen4x4b,1,0,4,4);
	DrawPlayerScreen3x2(screen3x2c,1,0,8,4);
	DrawPlayerScreen4x4(screen4x4d,1,0,12,4);
	DrawPlayerScreen4x4(screen4x4e,1,0,16,4);
}
else if(debugValue == 6)
{
	ProcessRayCastsWithMipMaps(screen4x4a,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	0,  2, 2, 0, 16);
	ProcessRayCastsWithMipMaps(screen3x2b,rayCastXOdd, rayCastYOdd,p1x,p1y,p1h,	24, 1, 1, 0, 24);
	ProcessRayCastsWithMipMaps(screen3x2c,rayCastXOdd, rayCastYOdd,p1x,p1y,p1h,	48, 1, 1, 0, 24);
	ProcessRayCastsWithMipMaps(screen3x2d,rayCastXOdd, rayCastYOdd,p1x,p1y,p1h,	72, 1, 1, 0, 24);
	ProcessRayCastsWithMipMaps(screen4x4e,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	128, 2, 2, 0,16);

	DrawPlayerScreen4x4(screen4x4a,1,0,0,4);
	DrawPlayerScreen3x2(screen3x2b,1,0,4,4);
	DrawPlayerScreen3x2(screen3x2c,1,0,8,4);
	DrawPlayerScreen3x2(screen3x2d,1,0,12,4);
	DrawPlayerScreen4x4(screen4x4e,1,0,16,4);
}
else if(debugValue == 7)
{
	ProcessRayCastsWithMipMaps(screen3x2a,rayCastXOdd, rayCastXOdd,p1x,p1y,p1h,	0,  1, 1, 0, 24);
	ProcessRayCastsWithMipMaps(screen3x2b,rayCastXOdd, rayCastYOdd,p1x,p1y,p1h,	24, 1, 1, 0, 24);
	ProcessRayCastsWithMipMaps(screen3x2c,rayCastXOdd, rayCastYOdd,p1x,p1y,p1h,	48, 1, 1, 0, 24);
	ProcessRayCastsWithMipMaps(screen3x2d,rayCastXOdd, rayCastYOdd,p1x,p1y,p1h,	72, 1, 1, 0, 24);
	ProcessRayCastsWithMipMaps(screen3x2e,rayCastXOdd, rayCastXOdd,p1x,p1y,p1h,	96, 1, 1, 0, 24);

	DrawPlayerScreen3x2(screen3x2a,1,0,0,4);
	DrawPlayerScreen3x2(screen3x2b,1,0,4,4);
	DrawPlayerScreen3x2(screen3x2c,1,0,8,4);
	DrawPlayerScreen3x2(screen3x2d,1,0,12,4);
	DrawPlayerScreen3x2(screen3x2e,1,0,16,4);
}
else if(debugValue == 8)
{
	//raycast 1 dimensional
	ProcessRayCastsSlow(screen8x8slow,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	0,  4, 4, 0, 32);
	DrawPlayerScreen8x8Slow(screen8x8slow,1,0,0,16);
}
else if(debugValue == 9)
{
	//mul
	ProcessRayCastsSlow3(screen8x8slow,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	0,  4, 4, 0, 40);
	DrawPlayerScreen8x8Slow(screen8x8slow,1,0,0,20);
}
else if(debugValue == 0)
{
	//custom mul
	ProcessRayCastsSlowMul(screen8x8slow,rayCastXEven, rayCastYEven,p1x,p1y,p1h,	0,  4, 4, 0, 40);
	DrawPlayerScreen8x8Slow(screen8x8slow,1,0,0,20);
}

/*
case 0: sx = 0;xStep = 4;xCycles = 8;currentTableStepSize=4;currentScreenStepSize=4*XSIZE;currentStep=0;break;
case 1: sx = 118;xStep = 4;xCycles = 8;currentTableStepSize=4;currentScreenStepSize=4*XSIZE;currentStep=0;break;
case 2: sx = 32;xStep = 2;xCycles = 16;currentTableStepSize=2;currentScreenStepSize=2*XSIZE;currentStep=0;break;
case 3: sx = 86;xStep = 2;xCycles = 16;currentTableStepSize=2;currentScreenStepSize=2*XSIZE;currentStep=0;break;
case 4: sx = 64;xStep = 1;xCycles = 23;currentTableStepSize=stepSize;currentScreenStepSize=screenYStepSize;currentStep=stepNumber;break;
*/
//xStep = 1;xCycles = 120;currentTableStepSize=1;currentScreenStepSize=1*XSIZE;currentStep=0;
//xStep = 2;xCycles = 60;currentTableStepSize=2;currentScreenStepSize=2*XSIZE;currentStep=0;
//xStep = 4;xCycles = 30;currentTableStepSize=4;currentScreenStepSize=4*XSIZE;currentStep=0;

//DrawPlayerScreen(1,0,1);
//DrawPlayerScreen8x8(1,0,0,0,0,20);
//DrawPlayerScreen4x4(1,0,0,0,0,20);
if(interlace == 0)
{
//	ProcessRayCasts(1,4,1);
	//SmoothScreen(1);
//	DrawPlayerScreen(1,0,0);

//	DrawPlayerScreen8x8(1,0,0,0,0,4);
	//DrawPlayerScreen4x4(1,0,0,32,4,4);
	//DrawPlayerScreen8x8(1,0,0,86,16,4);
	//DrawPlayerScreen3x2(1,0,0,24,24,72);
}
if(interlace == 1)
{
//	ProcessRayCasts(1,4,2);
	//	SmoothScreen(1);
//	DrawPlayerScreen(1,0,1);

//	DrawPlayerScreen8x8(1,0,0,0,0,4);
//	DrawPlayerScreen4x4(1,0,0,32,4,4);
//	DrawPlayerScreen8x8(1,0,0,86,16,4);
	//DrawPlayerScreen3x2(1,0,1,24,24,72);
}
if(interlace == 2)
{
//	ProcessRayCasts(1,4,3);
	//SmoothScreen(1);
//	DrawPlayerScreen(1,0,0);

//	DrawPlayerScreen8x8(1,0,0,0,0,4);
//	DrawPlayerScreen4x4(1,0,0,32,4,4);
//	DrawPlayerScreen8x8(1,0,0,86,16,4);
	//DrawPlayerScreen3x2(1,0,0,24,24,72);
}
if(interlace == 3)
{
//	ProcessRayCasts(1,4,4);
	//SmoothScreen(1);
//	DrawPlayerScreen(1,0,1);

//	DrawPlayerScreen8x8(1,0,0,0,0,4);
//	DrawPlayerScreen4x4(1,0,0,32,4,4);
//	DrawPlayerScreen8x8(1,0,0,86,16,4);
	//DrawPlayerScreen3x2(1,0,1,24,24,72);
}
/*
if(interlace == 0) DrawColorMap(1);
if(interlace == 1) DrawHeightMap(1);
if(interlace == 2) DrawColorMap(2);
if(interlace == 3) DrawHeightMap(2);*/

//DrawPlayerShip(1);
//DrawPlayerShip(2);

//DrawTerrain();
//DrawScreenHighDither();

interlace++;
if(interlace == 4) interlace = 0;

//vPortWaitForEnd(s_pVPort);

CopyFastToChipW(s_pBuffer->pBack);

//blitWait();
/*blitLine(
s_pBuffer->pBack,
0, 200,
320, 200,
0x1f, 0xAAAA, 0 // Try patterns 0xAAAA, 0xEEEE, etc.
);*/


logAvgEnd(s_pAvgTime);
endTime = timerGetPrec();
}

void logAvgWriteKwahu(tAvg *pAvg) {
	ULONG ulAvg = 0;
	char szAvg[15];
	char szMin[15];
	char szMax[15];

	if(!pAvg->uwUsedCount) {
		printf("Avg %s: No measures taken!\n", pAvg->szName);
		return;
	}
	// Calculate average time
	for(UWORD i = pAvg->uwUsedCount; i--;) {
		ulAvg += pAvg->pDeltas[i];
	}
	ulAvg /= pAvg->uwUsedCount;

	// Display info
	timerFormatPrec(szAvg, ulAvg);
	timerFormatPrec(szMin, pAvg->ulMin);
	timerFormatPrec(szMax, pAvg->ulMax);
	printf("Avg %s: %s, min: %s, max: %s\n", pAvg->szName, szAvg, szMin, szMax);
}

void engineGsDestroy(void)
{
	systemUse();
	viewLoad(0);
	viewDestroy(s_pView);

	char szAvg[15];
	//timerFormatPrec(szAvg, s_pAvgTime->uwCurrDelta);
	timerFormatPrec(szAvg, endTime - startTime);

	printf("%s", szAvg );
	printf("%lu", deltaTime);
	//printf("%d ", mul(13,7));
	//printf("%d ", mul(-13,7));


	logAvgDestroy(s_pAvgTime);
}
