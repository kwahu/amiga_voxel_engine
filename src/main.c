#include <ace/generic/main.h>
#include "engine.h"
#include "engine.c"
#include <ace/managers/game.h>
#include <ace/managers/timer.h>
#include <ace/managers/system.h>
#include <ace/managers/blit.h>
#include <ace/utils/file.h>

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


	p1x = 40;
	p1y = 0;
	p1h = 30;

	p2x = 50;
	p2y = 0;
	p2h = 10;

	ReadHeight("height.raw");
	//	ReadPalette("palette.raw");
	ReadColor("color.raw");
	CalculateRayCasts();
	SmoothHeightMap();
	SmoothColorMap();


	/*	SmoothHeightMap();
	SmoothHeightMap();
	SmoothHeightMap();
	SmoothHeightMap();
	SmoothHeightMap();
	SmoothHeightMap();
	SmoothHeightMap();
	SmoothHeightMap();

	GenerateColorMap();

	//AddHeightToColorMap();
	//AddBumpToColorMap();
	SmoothColorMap();
	SmoothColorMap();
	SmoothColorMap();
	SmoothColorMap();*/

	GenerateColorBytesDitherHigh();
	CalculateColorMipMaps();
	CalculateHeightMipMaps();
	CalculateModulo2();
	CalculateEnemyPlacement();


	memcpy(s_pVPort->pPalette, kolory, 16 * sizeof(UWORD));

	s_pAvgTime = logAvgCreate("perf", 100);

	viewLoad(s_pView);
	keyCreate();
	systemUnuse();
}

void engineGsLoop(void) {
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
		if(keyCheck(KEY_RIGHT))p2x+=2;
		if(keyCheck(KEY_LEFT))p2x-=2;
		if(keyCheck(KEY_F))p2h+=3;
		if(keyCheck(KEY_V))p2h-=3;
	}

	//	if(interlace % 2 == 0)
	{
		if(keyCheck(KEY_W))p1y++;
		if(keyCheck(KEY_S))p1y--;
		if(keyCheck(KEY_D))p1x+=3;
		if(keyCheck(KEY_A))p1x-=3;
		if(keyCheck(KEY_G))p1h+=3;
		if(keyCheck(KEY_B))p1h-=3;
	}
	if(keyCheck(KEY_H))renderingDepth++;
	if(keyCheck(KEY_N))renderingDepth--;

}
//		turn = turn/1.2;
//	xPos += turn;
if(renderingDepth<10) renderingDepth = 10;
else if(renderingDepth>TERRAINDEPTH) renderingDepth = TERRAINDEPTH;


if(p1h<5) p1h = 5;
if(p2h<5) p2h = 5;

logAvgBegin(s_pAvgTime);
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
if(interlace == 0)
{
	ProcessRayCasts(1,4,1);
	//SmoothScreen(1);
	DrawPlayerScreen(1,0,0);

}
if(interlace == 1)
{
	ProcessRayCasts(1,4,2);
	//	SmoothScreen(1);
	DrawPlayerScreen(1,0,1);

}
if(interlace == 2)
{
	ProcessRayCasts(1,4,3);
	//SmoothScreen(1);
	DrawPlayerScreen(1,0,0);

}
if(interlace == 3)
{
	ProcessRayCasts(1,4,4);
	//SmoothScreen(1);
	DrawPlayerScreen(1,0,1);

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

vPortWaitForEnd(s_pVPort);

CopyFastToChipW(s_pBuffer->pBack);

//blitWait();
/*blitLine(
s_pBuffer->pBack,
0, 200,
320, 200,
0x1f, 0xAAAA, 0 // Try patterns 0xAAAA, 0xEEEE, etc.
);*/

logAvgEnd(s_pAvgTime);
}

void engineGsDestroy(void)
{
	systemUse();
	viewLoad(0);
	viewDestroy(s_pView);

	logAvgDestroy(s_pAvgTime);
	logWrite(
		"seconds=%ld pixelsDrawn=%d pixelsChecked=%d pixelsRead=%d linesDrawn=%d",
		0, pixelsDrawn, pixelsChecked, pixelsRead, linesDrawn
	);
}
