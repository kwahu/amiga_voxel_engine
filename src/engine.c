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

//#define WIDTH 320
#define HEIGHT 256
#define DEPTH 4
#define COLORS 16
#define PLANEWIDTH 40
#define TERRAINDEPTH 60
#define XSIZE PLANEWIDTH*2
#define YSIZE 64
//#define PIXELHEIGHT 4

UBYTE height[256][256];
UBYTE colorMap[256][256];
UBYTE screen[XSIZE*YSIZE];
//UBYTE fastPlane[DEPTH][PLANEWIDTH*HEIGHT];

UBYTE fastPlane1[PLANEWIDTH*HEIGHT];
UBYTE fastPlane2[PLANEWIDTH*HEIGHT];
UBYTE fastPlane3[PLANEWIDTH*HEIGHT];
UBYTE fastPlane4[PLANEWIDTH*HEIGHT];

WORD rayCastX[XSIZE][TERRAINDEPTH];
WORD rayCastY[YSIZE][TERRAINDEPTH];

UWORD kolory[COLORS];
/*{
	0x040,0x242,0x363,0x383,0x3a3,0x3c3,0x3e3,0x3f3,
	0x4f4,0x5f5,0x6f6,0x7f7,0x8f8,0x9f9,0xafa,0x99f
};*/

UBYTE colorByte[COLORS][COLORS][DEPTH];



int xStart, yStart;
UBYTE xPos, yPos;
int currentHeight;
int heightAheadRange;
int flightHeight;
int horizon;
int turn;

static void ReadHeight(const char *name)
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
static void ReadPalette(const char *name)
{
	char r,g,b;
	tFile *file;
	file = fileOpen(name, "r");
	if (file)
	{
		for(int i=0;i<12;i++)
		{
				fileRead(file, &r, 1);
				fileRead(file, &g, 1);
				fileRead(file, &b, 1);
				kolory[i] = r*256 + g*16 + b;
		}
		fileClose(file);
	}
	kolory[15] = 0xaaf;
}
static void ReadColor(const char *name)
{
	tFile *file;
	file = fileOpen(name, "r");
	if (file) {
		for (int x = 0; x < 256; x++) {
			for (int y = 0; y < 256; y++) {
				fileRead(file, &colorMap[x][y], 1);
			}
		}
		fileClose(file);
	}
}

static void CopyFastToChip(tBitMap *bm)
{
	//for(int p=0;p<DEPTH;p++)
	//	CopyMemQuick(fastPlane[p], bm->Planes[p], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane1, bm->Planes[0], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane2, bm->Planes[1], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane3, bm->Planes[2], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane4, bm->Planes[3], PLANEWIDTH*HEIGHT);
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

}

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
				rayCastX[XSIZE/2+sx][tz] = sxx/12;
				rayCastY[YSIZE/2+sy][tz] = syy/12;
			}
		}
	}
}
/*
static void DrawByte(UBYTE color,UWORD position,UWORD p2,UWORD p3,UWORD p4 )
{
	UWORD bits;
	for(UBYTE plane=0;plane<DEPTH;plane++)
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
}*/

static void GenerateColorBytes()
{
	for(int a=0;a<COLORS;a++)
	for(int b=0;b<COLORS;b++)
	for(int plane=0;plane<DEPTH;plane++)
	{
		colorByte[a][b][plane] = ((a>>plane) & 1)*240 + ((b>>plane) & 1)*15;
	}
}

static void DrawScreen()
{
	UBYTE bits;
	UBYTE color1;
	UBYTE color2;
	UWORD color;
	UWORD sp,p1,p2,p3,p4;
	sp = 0;

	for(UWORD y=0;y<YSIZE;y++)
	{
		p1 = y*PLANEWIDTH*4;
		p2 = p1+PLANEWIDTH;
		p3 = p2+PLANEWIDTH;
		p4 = p3+PLANEWIDTH;

		for(UWORD x=0;x<PLANEWIDTH;x++)
		{
			color1 = screen[sp];
			color2 = screen[sp+1];

			fastPlane1[p1] = colorByte[ color1 ][ color2 ][ 0 ];
			fastPlane1[p2] = colorByte[ color1 ][ color2 ][ 0 ];
			fastPlane1[p3] = colorByte[ color1 ][ color2 ][ 0 ];
			fastPlane1[p4] = colorByte[ color1 ][ color2 ][ 0 ];

			fastPlane2[p1] = colorByte[ color1 ][ color2 ][ 1 ];
			fastPlane2[p2] = colorByte[ color1 ][ color2 ][ 1 ];
			fastPlane2[p3] = colorByte[ color1 ][ color2 ][ 1 ];
			fastPlane2[p4] = colorByte[ color1 ][ color2 ][ 1 ];

			fastPlane3[p1] = colorByte[ color1 ][ color2 ][ 2 ];
			fastPlane3[p2] = colorByte[ color1 ][ color2 ][ 2 ];
			fastPlane3[p3] = colorByte[ color1 ][ color2 ][ 2 ];
			fastPlane3[p4] = colorByte[ color1 ][ color2 ][ 2 ];

			fastPlane4[p1] = colorByte[ color1 ][ color2 ][ 3 ];
			fastPlane4[p2] = colorByte[ color1 ][ color2 ][ 3 ];
			fastPlane4[p3] = colorByte[ color1 ][ color2 ][ 3 ];
			fastPlane4[p4] = colorByte[ color1 ][ color2 ][ 3 ];

			p1++;
			p2++;
			p3++;
			p4++;

			sp+=2;
		}
	}
}

static void DrawTerrain(void) {
	currentHeight = (currentHeight*3 + flightHeight + height[xPos][yPos+heightAheadRange])/4;
	UBYTE color;
	UBYTE sx,sy,tz;
	UBYTE th;
	UWORD position;
	UBYTE mx,my;

	UWORD startPosition;
	startPosition = (YSIZE-1)*XSIZE;

	for(sx=0;sx<XSIZE;sx++)
	{
		sy = 0;
		tz = 0;
		position = startPosition+sx;

		while(tz < TERRAINDEPTH && sy <YSIZE)
		{
			mx = xPos + rayCastX[sx][tz];
			my = yPos + tz;
			th = height[ mx ][ my ]/2;
			//height to look for at a given x,y terrain coordinate accounting for z depth
			//************************************************************
			if(th>currentHeight + rayCastY[sy][tz])
			{
				color = colorMap[mx][my];//th % 14;
				screen[position] = color;
				sy++;
				position-=XSIZE;
			}
			else
			{
				tz+=1;//+tz/8;
			}
		}
		//finish vertical line with 0xff
		while(sy < YSIZE)
		{
			sy++;
			screen[position] = 0x0f;
			position-=XSIZE;
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


				//viewUpdateCLUT(s_pView);

				xStart = 50;
				yStart = 0;

				xPos = xStart;
				yPos = yStart;

				heightAheadRange = 5;

				flightHeight = 15;

				ReadHeight("height.raw");
				ReadPalette("palette.raw");
				ReadColor("color.raw");
				CalculateTables();
				GenerateColorBytes();

				memcpy(s_pVPort->pPalette, kolory, 16 * sizeof(UWORD));

				s_pAvgTime = logAvgCreate("perf", 100);

				viewLoad(s_pView);
				keyCreate();
				systemUnuse();
			}

			void engineGsLoop(void) {
				if(yPos == 250)
				{
					gameClose();
				}

			/*		if(keyCheck(KEY_ESCAPE)) {
				gameClose();
			}
			else
			{
			if(keyCheck(KEY_UP))yPos++;
			if(keyCheck(KEY_DOWN))yPos--;
			if(keyCheck(KEY_RIGHT))turn+=20;
			if(keyCheck(KEY_LEFT))turn-=20;
		}

				turn -= turn/8;
				xPos += turn/8;*/

			logAvgBegin(s_pAvgTime);
			DrawTerrain();
			DrawScreen();
			CopyFastToChip(s_pBuffer->pBack);
			//flightHeight += 1;
			yPos += 1;
			//	xPos += 1;
			logAvgEnd(s_pAvgTime);
			//}
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
