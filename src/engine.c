#include "engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <ace/managers/viewport/simplebuffer.h>
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

#define WIDTH 320
#define HEIGHT 256
#define DEPTH 4
#define PLANEWIDTH 40

UBYTE height[256][256];
UBYTE fastPlane[DEPTH][PLANEWIDTH*HEIGHT];

UWORD kolory[] = {
	0x000,0x121,0x131,0x141,0x252,0x262,0x272,0x282,
	0x393,0x3a3,0x3b3,0x4c4,0x4d4,0x5e5,0x5f5,0x55f
};

int xStart, yStart;
UBYTE xPos, yPos;
int currentHeight;
int heightAheadRange;
int flightHeight;
int horizon;

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
		/*
			tBitMap *pSrc, WORD wSrcX, WORD wSrcY,
			tBitMap *pDst, WORD wDstX, WORD wDstY, WORD wWidth, WORD wHeight,
			UBYTE ubMinterm, UBYTE ubMask
			*/
		/*blitUnsafeCopy(
			fastPlane[p], 0, 0,
			bm->Planes[p], 0,0, WIDTH, HEIGHT,
			 0x00, 0xff
		);*/
		/*for(int i=0;i<PLANEWIDTH*HEIGHT/4;i++)
		{

		}*/
		CopyMemQuick(fastPlane[p], bm->Planes[p], PLANEWIDTH*HEIGHT);
	}
}

#define TERRAINDEPTH 100
#define XSIZE 40
#define YSIZE 60
#define PIXELHEIGHT 4

BYTE rayCastX[XSIZE][TERRAINDEPTH];
BYTE rayCastY[YSIZE][TERRAINDEPTH];

static void CalculateTables()
{
	int sxx;
	int syy;

	for(int tz=1;tz<TERRAINDEPTH;tz++)
	{
		for(int sx=-XSIZE/2;sx<XSIZE/2;sx++)
		{
			sxx = sx * tz;

			for(int sy=-YSIZE/2;sy<YSIZE/2;sy++)
			{
				syy = sy * tz;
				rayCastX[XSIZE/2+sx][tz] = sxx/24;
				rayCastY[YSIZE/2+sy][tz] = syy/24;
			}
		}
	}
}

static void DrawTerrain(void) {
	currentHeight = (currentHeight*3 + flightHeight + height[xPos][yPos+heightAheadRange])/4;
	UBYTE color;
	UBYTE sx,sy,tz,plane;
	UBYTE th;
	UWORD bits;
	UWORD position,p2,p3,p4;
	UWORD pixelYOffset;
	UWORD startPosition;

  pixelYOffset = PIXELHEIGHT*PLANEWIDTH;
	startPosition = YSIZE*PIXELHEIGHT*PLANEWIDTH;

	for(sx=0;sx<XSIZE;sx++)
	{
		sy = 0;
		tz = 0;
		position = startPosition + sx;

		while(tz < TERRAINDEPTH && sy <YSIZE)
		{
			th = height[ xPos + rayCastX[sx][tz]*2 ][ yPos + tz ]/2;
			//height to look for at a given x,y terrain coordinate accounting for z depth
			//************************************************************
			if(th>currentHeight + rayCastY[sy][tz])
			{
				color = 1+th/16;
				p2 = position+40;
				p3 = position+80;
				p4 = position+120;

				for(plane=0;plane<DEPTH;plane++)
				{
					if((color>>plane) & 1) bits = 0xff;
					else bits = 0x00;

					if(fastPlane[plane][position] != bits)
					{
						fastPlane[plane][position] = bits;
						fastPlane[plane][p2] = bits;
						fastPlane[plane][p3] = bits;
						fastPlane[plane][p4] = bits;
					}
				}
				sy++;
				position -= pixelYOffset;
			}
			else
			{
				tz+=1+tz/8;
			}
		}
		//finish vertical line with 0xff


		while(sy < YSIZE)
		{
			sy++;
			position -= pixelYOffset;

			p2 = position+40;
			p3 = position+80;
			p4 = position+120;

			for(plane=0;plane<DEPTH;plane++)
			{
				if(fastPlane[plane][position] != 0xff)
				{
					fastPlane[plane][position] = 0xff;
					fastPlane[plane][p2] = 0xff;
					fastPlane[plane][p3] = 0xff;
					fastPlane[plane][p4] = 0xff;
				}
			}
		}
	}
}

static tView *s_pView;
static tVPort *s_pVPort;
static tSimpleBufferManager *s_pBuffer;
static tAvg *s_pAvgTime;

void engineGsCreate(void) {
	s_pView = viewCreate(0,
		TAG_VIEW_GLOBAL_CLUT, 1,
		 TAG_DONE);
/*	s_pVPort = vPortCreate(0,
		TAG_VPORT_VIEW, s_pView,
		TAG_VPORT_PALETTE_PTR, kolory,
		TAG_VPORT_PALETTE_SIZE, 1 << DEPTH,
		TAG_VPORT_BPP, DEPTH,
	TAG_DONE);*/

	s_pVPort = vPortCreate(0,
	TAG_VPORT_VIEW, s_pView,
	TAG_VPORT_BPP, DEPTH,
TAG_END);

	s_pBuffer = simpleBufferCreate(0,
		TAG_SIMPLEBUFFER_VPORT, s_pVPort,
		TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR,
	TAG_DONE);

	horizon = HEIGHT / 2;


memcpy(s_pVPort->pPalette, kolory, 16 * sizeof(UWORD));
//viewUpdateCLUT(s_pView);

	xStart = 100;
	yStart = 0;

	xPos = xStart;
	yPos = yStart;

	heightAheadRange = 5;

	flightHeight = 15;

	ReadFile("height.raw");
	CalculateTables();

	s_pAvgTime = logAvgCreate("perf", 100);

	viewLoad(s_pView);
	systemUnuse();
}

void engineGsLoop(void) {
	static int i = 0;
	if(++i == 300) {
		gameClose();
		return;
	}
	logAvgBegin(s_pAvgTime);
	DrawTerrain();
	ClearFastPlane(s_pBuffer->pBack);
	//flightHeight += 1;
	yPos += 1;
//	xPos += 1;
	logAvgEnd(s_pAvgTime);
}

void engineGsDestroy(void) {
	systemUse();
	viewLoad(0);
	viewDestroy(s_pView);

	logAvgDestroy(s_pAvgTime);
	logWrite(
		"seconds=%ld pixelsDrawn=%d pixelsChecked=%d pixelsRead=%d linesDrawn=%d",
		0, pixelsDrawn, pixelsChecked, pixelsRead, linesDrawn
	);

}
