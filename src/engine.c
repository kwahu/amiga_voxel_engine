#include "engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/game.h>
#include <ace/managers/timer.h>
#include <ace/utils/file.h>

#define WIDTH 320
#define HEIGHT 256
#define DEPTH 4
#define PLANEWIDTH 40

UBYTE height[256][256];
UBYTE fastPlane[DEPTH][PLANEWIDTH*HEIGHT];
UBYTE *test = (UBYTE*)height;

UWORD kolory[] = { 0x0100, // 0x0RGB
	0x0000, 0x0000, 0x0000,
	0x0222, 0x0822, 0x0222,
	0x0444, 0x0944, 0x0444,
	0x0666, 0x9666, 0x0666,
	0x0888, 0x0a88, 0x0888,
	0x0999, 0x0a99, 0x0999,
	0x0999,	0x0b99,	0x0999,
	0x0aaa,	0x0bbb,	0x0aaa,
	0x0a88, 0x0c88, 0xa888,
	0x0bbb, 0x0cbb,	0x0bbb,
	0x0bbb, 0x0dbb,	0x0bbb,
	0x0ccc, 0x0dcc,	0x0ccc,
	0x0ccc, 0x0ecc,	0x0ccc,
	0x0ddd, 0x0edd,	0x0ddd,
	0x0eee, 0x0eee,	0x0eee,
	0x0fff, 0x0fff,	0x0fff,
	0x0000
};

UWORD mapScaleH; // was "float" - very bad idea, if you need fractions use fix16_t
UWORD mapScaleX; // was "float" - very bad idea, if you need fractions use fix16_t
UWORD mapScaleY; // was "float" - very bad idea, if you need fractions use fix16_t
UWORD xStart, yStart; // was "float" - very bad idea, if you need fractions use fix16_t
UWORD xPos, yPos; // was "float" - very bad idea, if you need fractions use fix16_t
int currentHeight;
int heightAheadRange;
UWORD flightHeight; // was "float" - very bad idea, if you need fractions use fix16_t
int horizon;
UWORD speed = 1; // was "float" - very bad idea, if you need fractions use fix16_t
int pixelsDrawn = 0;
int pixelsChecked = 0;
int pixelsRead = 0;
int linesDrawn = 0;
UWORD *address;
UWORD position;

static void ReadFile(const char *name)
{
	tFile *file;
	file = fileOpen(name, "r");
	if (file) {
		for (int x = 0; x < 256; x++) {
			for (int y = 0; y < 256; y++) {
				fileRead(file, &height[x][y], 1);
			}
		}
		fileClose(file);
	}
}

static void ClearFastPlane(tBitMap *bm)
{
	for(int p=0;p<DEPTH;p++) {
		CopyMemQuick(fastPlane[p], bm->Planes[p], PLANEWIDTH*HEIGHT);
	}
}

#define TERRAINDEPTH 30

WORD rayCastX[40][TERRAINDEPTH];
WORD rayCastY[32][TERRAINDEPTH];

static void CalculateTables(void)
{
	for(float sx=-20;sx<20;sx++) {
		for(float sy=-16;sy<16;sy++) {
			for(float tz=1;tz<TERRAINDEPTH;tz++) {
				rayCastX[20+(int)sx][(int)tz] = (WORD)(sx * tz/8);
				rayCastY[16+(int)sy][(int)tz] = (WORD)(sy * tz/8);
			}
		}
	}
}

static void DrawTerrain(void) {
	//currentHeight = flightHeight + height[(int)(xPos * mapScaleX)][(int)((yPos+heightAheadRange) * mapScaleY )];
	UBYTE color;
	BYTE sx,sy,tz,line,plane;
	WORD rayHeight;
	UBYTE tx;//screen x offest on terrain - perspective change with depth
	UBYTE ty;
	UBYTE th;

	for(sx=0;sx<40;sx++)
	{
		sy = 0;
		tz = 0;
		while(tz < TERRAINDEPTH)
		{
			pixelsChecked++;
			rayHeight = flightHeight + rayCastY[sy][tz];
			tx = xPos + rayCastX[sx][tz];//screen x offest on terrain - perspective change with depth
			ty = yPos + tz;
			//th = height[tx][ty];
			th = test[(tx << 8) | ty];
			//height to look for at a given x,y terrain coordinate accounting for z depth
			//************************************************************8
			if(th>rayHeight)
			{
				pixelsDrawn++;
				color = 1+th/16;
				position = ((31-sy)*8)*40 + sx;//OK
				//******************DRAW
				for(line=0;line<8;line++)
				{
					for(plane=0;plane<DEPTH;plane++)
					{
						if((color>>plane) & 1) {
							fastPlane[plane][position] = 0xff;
						}
						else {
							fastPlane[plane][position] = 0x00;//OK
						}
					}
					position+=40;
				}
				//******************DRAW
				sy++;
				tz++;
			}
			else
			{
				tz++;
			}
		}
		//finish vertical line with 0x00
		while(sy < TERRAINDEPTH)
		{
			sy++;
			position = ((31-sy)*8)*40 + sx;//OK
			//******************DRAW
			for(line=0;line<8;line++)
			{
				for(plane=0;plane<DEPTH;plane++)
				{
					fastPlane[plane][position] = 0xff;
				}
				position+=40;
			}
		}
	}
}

static tView *s_pView;
static tVPort *s_pVPort;
static tSimpleBufferManager *s_pBuffer;
static tAvg *s_pAvgTime;

void engineGsCreate(void) {
	s_pView = viewCreate(0, TAG_DONE);
	s_pVPort = vPortCreate(0,
		TAG_VPORT_VIEW, s_pView,
		TAG_VPORT_PALETTE_PTR, kolory,
		TAG_VPORT_PALETTE_SIZE, 1 << DEPTH,
		TAG_VPORT_BPP, DEPTH,
	TAG_DONE);
	s_pBuffer = simpleBufferCreate(0,
		TAG_SIMPLEBUFFER_VPORT, s_pVPort,
		TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR,
	TAG_DONE);

	horizon = HEIGHT / 2;

	mapScaleH = 10;
	mapScaleX = 4;
	mapScaleY = 4;

	xStart = 50;
	yStart = 0;

	xPos = xStart;
	yPos = yStart;

	heightAheadRange = 2;

	flightHeight = 0;

	ReadFile("height.raw");
	CalculateTables();

	s_pAvgTime = logAvgCreate("perf", 100);

	viewLoad(s_pView);
}

void engineGsLoop(void) {
	static UBYTE i = 0;
	if(++i == 100) {
		gameClose();
		return;
	}
	logAvgBegin(s_pAvgTime);
	DrawTerrain();
	ClearFastPlane(s_pBuffer->pBack);
	//flightHeight += 1;
	yPos += 1;
	//xPos += 10;
	logAvgEnd(s_pAvgTime);
}

void engineGsDestroy(void) {
	viewLoad(0);
	viewDestroy(s_pView);

	logAvgDestroy(s_pAvgTime);
	logWrite(
		"seconds=%ld pixelsDrawn=%d pixelsChecked=%d pixelsRead=%d linesDrawn=%d",
		0, pixelsDrawn, pixelsChecked, pixelsRead, linesDrawn
	);

}
