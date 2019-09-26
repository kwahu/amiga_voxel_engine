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

UWORD fastPlane1W[PLANEWIDTH*HEIGHT];
UWORD fastPlane2W[PLANEWIDTH*HEIGHT];
UWORD fastPlane3W[PLANEWIDTH*HEIGHT];
UWORD fastPlane4W[PLANEWIDTH*HEIGHT];

WORD rayCastX[XSIZE][TERRAINDEPTH];
WORD rayCastY[YSIZE][TERRAINDEPTH];

UWORD kolory[COLORS];
/*{
0x040,0x242,0x363,0x383,0x3a3,0x3c3,0x3e3,0x3f3,
0x4f4,0x5f5,0x6f6,0x7f7,0x8f8,0x9f9,0xafa,0x99f
};*/

UBYTE colorByte2[COLORS][COLORS][DEPTH];
UWORD colorByte4p1[COLORS][COLORS][COLORS][COLORS];
UWORD colorByte4p2[COLORS][COLORS][COLORS][COLORS];
UWORD colorByte4p3[COLORS][COLORS][COLORS][COLORS];
UWORD colorByte4p4[COLORS][COLORS][COLORS][COLORS];

UWORD colorByte4p1Flat[COLORS*COLORS*COLORS*COLORS];
UWORD colorByte4p2Flat[COLORS*COLORS*COLORS*COLORS];
UWORD colorByte4p3Flat[COLORS*COLORS*COLORS*COLORS];
UWORD colorByte4p4Flat[COLORS*COLORS*COLORS*COLORS];

int xStart, yStart;
UBYTE xPos, yPos;
int currentHeight;
int heightAheadRange;
int flightHeight;
int horizon;
int turn;
int interlace;

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

static void CopyFastToChipB(tBitMap *bm)
{
	CopyMemQuick(fastPlane1, bm->Planes[0], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane2, bm->Planes[1], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane3, bm->Planes[2], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane4, bm->Planes[3], PLANEWIDTH*HEIGHT);
}
static void CopyFastToChipW(tBitMap *bm)
{
	CopyMemQuick(fastPlane1W, bm->Planes[0], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane2W, bm->Planes[1], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane3W, bm->Planes[2], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane4W, bm->Planes[3], PLANEWIDTH*HEIGHT);
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

static void GenerateColorBytes2()
{
	for(int a=0;a<COLORS;a++)
	for(int b=0;b<COLORS;b++)
	for(int plane=0;plane<DEPTH;plane++)
	{
		colorByte2[a][b][plane] = ((a>>plane) & 1)*240 + ((b>>plane) & 1)*15;
	}
}


static void GenerateColorBytes4()
{
	for(int a=0;a<COLORS;a++)
	for(int b=0;b<COLORS;b++)
	for(int c=0;c<COLORS;c++)
	for(int d=0;d<COLORS;d++)
	{
		colorByte4p1[a][b][c][d]=
		((a>>0) & 1)*61440 +
		((b>>0) & 1)*3840+
		((c>>0) & 1)*240+
		((d>>0) & 1)*15;
		colorByte4p2[a][b][c][d]=
		((a>>1) & 1)*61440 +
		((b>>1) & 1)*3840+
		((c>>1) & 1)*240+
		((d>>1) & 1)*15;
		colorByte4p3[a][b][c][d]=
		((a>>2) & 1)*61440 +
		((b>>2) & 1)*3840+
		((c>>2) & 1)*240+
		((d>>2) & 1)*15;
		colorByte4p4[a][b][c][d]=
		((a>>3) & 1)*61440 +
		((b>>3) & 1)*3840+
		((c>>3) & 1)*240+
		((d>>3) & 1)*15;
	}
}

static void GenerateColorBytes4Flat()
{
	UWORD address;
	for(int a=0;a<COLORS;a++)
	for(int b=0;b<COLORS;b++)
	for(int c=0;c<COLORS;c++)
	for(int d=0;d<COLORS;d++)
	{
		address = (a<<12) + (b<<8) + (c<<4) + d;
		colorByte4p1Flat[address]=
		((a>>0) & 1)*61440 +
		((b>>0) & 1)*3840+
		((c>>0) & 1)*240+
		((d>>0) & 1)*15;
		colorByte4p2Flat[address]=
		((a>>1) & 1)*61440 +
		((b>>1) & 1)*3840+
		((c>>1) & 1)*240+
		((d>>1) & 1)*15;
		colorByte4p3Flat[address]=
		((a>>2) & 1)*61440 +
		((b>>2) & 1)*3840+
		((c>>2) & 1)*240+
		((d>>2) & 1)*15;
		colorByte4p4Flat[address]=
		((a>>3) & 1)*61440 +
		((b>>3) & 1)*3840+
		((c>>3) & 1)*240+
		((d>>3) & 1)*15;
	}
}


static void DrawScreen2()
{
	UBYTE color1;
	UBYTE color2;
	UWORD sp,p1,p2,p3,p4;
	UBYTE byte;
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

			byte = colorByte2[ color1 ][ color2 ][ 0 ];
			fastPlane1[p1] = byte;
			fastPlane1[p2] = byte;
			fastPlane1[p3] = byte;
			fastPlane1[p4] = byte;

			byte = colorByte2[ color1 ][ color2 ][ 1 ];
			fastPlane2[p1] = byte;
			fastPlane2[p2] = byte;
			fastPlane2[p3] = byte;
			fastPlane2[p4] = byte;

			byte = colorByte2[ color1 ][ color2 ][ 2 ];
			fastPlane3[p1] = byte;
			fastPlane3[p2] = byte;
			fastPlane3[p3] = byte;
			fastPlane3[p4] = byte;

			byte = colorByte2[ color1 ][ color2 ][ 3 ];
			fastPlane4[p1] = byte;
			fastPlane4[p2] = byte;
			fastPlane4[p3] = byte;
			fastPlane4[p4] = byte;

			p1++;
			p2++;
			p3++;
			p4++;

			sp+=2;
		}
	}
}

static void DrawScreen4()
{
	UBYTE color1;
	UBYTE color2;
	UBYTE color3;
	UBYTE color4;
	UWORD sp,p1,p2,p3,p4;
	UWORD word;
	sp = 0;

	for(UWORD y=0;y<YSIZE;y++)
	{
		p1 = y*PLANEWIDTH*4/2;
		p2 = p1+PLANEWIDTH/2;
		p3 = p2+PLANEWIDTH/2;
		p4 = p3+PLANEWIDTH/2;

		for(UWORD x=0;x<PLANEWIDTH/2;x++)
		{
			color1 = screen[sp];
			color2 = screen[sp+1];
			color3 = screen[sp+2];
			color4 = screen[sp+3];

			word = colorByte4p1[ color1 ][ color2 ][ color3 ][ color4 ];
			if(fastPlane1W[p1] != word)
			{
			fastPlane1W[p1] = word;
			fastPlane1W[p2] = word;
			fastPlane1W[p3] = word;
			fastPlane1W[p4] = word;
}

			word = colorByte4p2[ color1 ][ color2 ][ color3 ][ color4 ];
			if(fastPlane2W[p1] != word)
			{
			fastPlane2W[p1] = word;
			fastPlane2W[p2] = word;
			fastPlane2W[p3] = word;
			fastPlane2W[p4] = word;
}
			word = colorByte4p3[ color1 ][ color2 ][ color3 ][ color4 ];
			if(fastPlane3W[p1] != word)
			{
			fastPlane3W[p1] = word;
			fastPlane3W[p2] = word;
			fastPlane3W[p3] = word;
			fastPlane3W[p4] = word;
}
			word = colorByte4p4[ color1 ][ color2 ][ color3 ][ color4 ];
			if(fastPlane4W[p1] != word)
			{
			fastPlane4W[p1] = word;
			fastPlane4W[p2] = word;
			fastPlane4W[p3] = word;
			fastPlane4W[p4] = word;
		}

			p1++;
			p2++;
			p3++;
			p4++;

			sp+=4;
		}
	}
}

static void DrawScreen4Flat()
{
	UWORD sp,p1,p2,p3,p4;
	UWORD word;
	UWORD address;
	sp = 0;

	for(UWORD y=0;y<YSIZE;y++)
	{
		p1 = y*PLANEWIDTH*4/2;
		p2 = p1+PLANEWIDTH/2;
		p3 = p2+PLANEWIDTH/2;
		p4 = p3+PLANEWIDTH/2;

		for(UWORD x=0;x<PLANEWIDTH/2;x++)
		{
			address = (screen[sp]<<12) + (screen[sp+1]<<8) + (screen[sp+2]<<4) + screen[sp+3];

			word = colorByte4p1Flat[ address ];
			if(fastPlane1W[p1]!=word)
			{
				fastPlane1W[p1] = word;
				fastPlane1W[p2] = word;
				fastPlane1W[p3] = word;
				fastPlane1W[p4] = word;
			}

			word = colorByte4p2Flat[ address ];
			if(fastPlane2W[p1] != word)
			{
				fastPlane2W[p1] = word;
				fastPlane2W[p2] = word;
				fastPlane2W[p3] = word;
				fastPlane2W[p4] = word;
			}

			word = colorByte4p3Flat[ address ];
			if(fastPlane3W[p1] != word)
			{
				fastPlane3W[p1] = word;
				fastPlane3W[p2] = word;
				fastPlane3W[p3] = word;
				fastPlane3W[p4] = word;
			}

			word = colorByte4p4Flat[ address ];
			if(fastPlane4W[p1] != word)
			{
				fastPlane4W[p1] = word;
				fastPlane4W[p2] = word;
				fastPlane4W[p3] = word;
				fastPlane4W[p4] = word;
			}

			p1++;
			p2++;
			p3++;
			p4++;

			sp+=4;
		}
	}
}

static void DrawScreen4FlatMemCopy()
{
	UWORD sp,p1,p2,p3,p4;
	UWORD word;
	UWORD address;
	sp = 0;

	for(UWORD y=0;y<YSIZE;y++)
	{
		p1 = y*PLANEWIDTH*4/2;
		p2 = p1+PLANEWIDTH/2;
		p3 = p2+PLANEWIDTH/2;
		p4 = p3+PLANEWIDTH/2;

		for(UWORD x=0;x<PLANEWIDTH/2;x++)
		{
			address = (screen[sp]<<12) + (screen[sp+1]<<8) + (screen[sp+2]<<4) + screen[sp+3];

			word = colorByte4p1Flat[ address ];
			if(fastPlane1W[p1]!=word)
			{
				fastPlane1W[p1] = word;
				//fastPlane1W[p2] = word;
				//fastPlane1W[p3] = word;
				//fastPlane1W[p4] = word;
			}

			word = colorByte4p2Flat[ address ];
			if(fastPlane2W[p1] != word)
			{
				fastPlane2W[p1] = word;
			//	fastPlane2W[p2] = word;
			//	fastPlane2W[p3] = word;
			//	fastPlane2W[p4] = word;
			}

			word = colorByte4p3Flat[ address ];
			if(fastPlane3W[p1] != word)
			{
				fastPlane3W[p1] = word;
			//	fastPlane3W[p2] = word;
			//	fastPlane3W[p3] = word;
			//	fastPlane3W[p4] = word;
			}

			word = colorByte4p4Flat[ address ];
			if(fastPlane4W[p1] != word)
			{
				fastPlane4W[p1] = word;
			//	fastPlane4W[p2] = word;
				//fastPlane4W[p3] = word;
				//fastPlane4W[p4] = word;
			}

			p1++;
			p2++;
			p3++;
			p4++;

			sp+=4;
		}
	}

	for(UWORD y=0;y<YSIZE;y++)
	{
		p1 = y*PLANEWIDTH*4/2;
		p2 = p1+PLANEWIDTH/2;
		p3 = p2+PLANEWIDTH/2;
		p4 = p3+PLANEWIDTH/2;

		CopyMemQuick(&fastPlane1W[p1], &fastPlane1W[p2], PLANEWIDTH);
		CopyMemQuick(&fastPlane1W[p1], &fastPlane1W[p3], PLANEWIDTH);
		CopyMemQuick(&fastPlane1W[p1], &fastPlane1W[p4], PLANEWIDTH);

		CopyMemQuick(&fastPlane2W[p1], &fastPlane2W[p2], PLANEWIDTH);
		CopyMemQuick(&fastPlane2W[p1], &fastPlane2W[p3], PLANEWIDTH);
		CopyMemQuick(&fastPlane2W[p1], &fastPlane2W[p4], PLANEWIDTH);

		CopyMemQuick(&fastPlane3W[p1], &fastPlane3W[p2], PLANEWIDTH);
		CopyMemQuick(&fastPlane3W[p1], &fastPlane3W[p3], PLANEWIDTH);
		CopyMemQuick(&fastPlane3W[p1], &fastPlane3W[p4], PLANEWIDTH);

		CopyMemQuick(&fastPlane4W[p1], &fastPlane4W[p2], PLANEWIDTH);
		CopyMemQuick(&fastPlane4W[p1], &fastPlane4W[p3], PLANEWIDTH);
		CopyMemQuick(&fastPlane4W[p1], &fastPlane4W[p4], PLANEWIDTH);
	}
}

static void DrawScreen4FlatEven()
{
	UWORD sp,p1,p2,p3,p4;
	UWORD word;
	UWORD address;
	sp = 0;

	for(UWORD y=0;y<YSIZE;y++)
	{
		p1 = y*PLANEWIDTH*4/2;
		p2 = p1+PLANEWIDTH/2;
		p3 = p2+PLANEWIDTH/2;
		p4 = p3+PLANEWIDTH/2;

		for(UWORD x=0;x<PLANEWIDTH/2;x++)
		{
			address = (screen[sp]<<12) + (screen[sp+1]<<8) + (screen[sp+2]<<4) + screen[sp+3];

			word = colorByte4p1Flat[ address ];
			//	fastPlane1W[p1] = word;
			fastPlane1W[p2] = word;
			//fastPlane1W[p3] = word;
			fastPlane1W[p4] = word;

			word = colorByte4p2Flat[ address ];
			//fastPlane2W[p1] = word;
			fastPlane2W[p2] = word;
			//fastPlane2W[p3] = word;
			fastPlane2W[p4] = word;

			word = colorByte4p3Flat[ address ];
			//fastPlane3W[p1] = word;
			fastPlane3W[p2] = word;
			//fastPlane3W[p3] = word;
			fastPlane3W[p4] = word;

			word = colorByte4p4Flat[ address ];
			//fastPlane4W[p1] = word;
			fastPlane4W[p2] = word;
			//fastPlane4W[p3] = word;
			fastPlane4W[p4] = word;

			p1++;
			p2++;
			p3++;
			p4++;

			sp+=4;
		}
	}
}

static void DrawScreen4FlatOdd()
{
	UWORD sp,p1,p2,p3,p4;
	UWORD word;
	UWORD address;
	sp = 0;

	for(UWORD y=0;y<YSIZE;y++)
	{
		p1 = y*PLANEWIDTH*4/2;
		p2 = p1+PLANEWIDTH/2;
		p3 = p2+PLANEWIDTH/2;
		p4 = p3+PLANEWIDTH/2;

		for(UWORD x=0;x<PLANEWIDTH/2;x++)
		{
			address = (screen[sp]<<12) + (screen[sp+1]<<8) + (screen[sp+2]<<4) + screen[sp+3];

			word = colorByte4p1Flat[ address ];
			fastPlane1W[p1] = word;
			//fastPlane1W[p2] = word;
			fastPlane1W[p3] = word;
			//fastPlane1W[p4] = word;

			word = colorByte4p2Flat[ address ];
			fastPlane2W[p1] = word;
			//fastPlane2W[p2] = word;
			fastPlane2W[p3] = word;
			//	fastPlane2W[p4] = word;

			word = colorByte4p3Flat[ address ];
			fastPlane3W[p1] = word;
			//fastPlane3W[p2] = word;
			fastPlane3W[p3] = word;
			//	fastPlane3W[p4] = word;

			word = colorByte4p4Flat[ address ];
			fastPlane4W[p1] = word;
			//fastPlane4W[p2] = word;
			fastPlane4W[p3] = word;
			//	fastPlane4W[p4] = word;

			p1++;
			p2++;
			p3++;
			p4++;

			sp+=4;
		}
	}
}

/*
1 x 1 0
x x 0 0
1 0 1 0

4 odczyty
3 odczyty - interpolacji
3 zapisy
*/
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
				tz+=1+tz/16;
			}
		}
		//finish vertical line with 0xff
		while(sy < YSIZE)
		{
			sy++;
			if(screen[position] == 0x0f)
			break;
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


		s_pVPort = vPortCreate(0,
			TAG_VPORT_VIEW, s_pView,
			TAG_VPORT_BPP, DEPTH,
			TAG_END);

			s_pBuffer = simpleBufferCreate(0,
				TAG_SIMPLEBUFFER_VPORT, s_pVPort,
				TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR,
				TAG_DONE);


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
				GenerateColorBytes2();
				GenerateColorBytes4();
				GenerateColorBytes4Flat();

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

				/*		if(keyCheck(KEY_SPACE)) {
				gameClose();
			}
			else
			{
			if(keyCheck(KEY_UP))yPos++;
			if(keyCheck(KEY_DOWN))yPos--;
			if(keyCheck(KEY_RIGHT))xPos++;
			if(keyCheck(KEY_LEFT))xPos--;
		}*/

		//		turn = turn/1.2;
		//	xPos += turn;



		logAvgBegin(s_pAvgTime);
		DrawTerrain();
		//DrawScreen2();
		//DrawScreen4();
		DrawScreen4Flat();
		//DrawScreen4FlatMemCopy();
		/*	if(interlace > 0)
		{
		DrawScreen4FlatEven();
		interlace = -1;
	}
	else
	{
	DrawScreen4FlatOdd();
	interlace = 1;
}*/


CopyFastToChipW(s_pBuffer->pBack);
//flightHeight += 1;
yPos += 1;
//xPos += 1;
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
