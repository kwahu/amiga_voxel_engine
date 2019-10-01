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
#define COLORS 32
#define PLANEWIDTH 40
#define TERRAINDEPTH 150
#define XSIZE 80
#define YSIZE 64
//#define PIXELHEIGHT 4

UBYTE heightMap0[256][256];
UBYTE heightMap1[128][128];
UBYTE heightMap2[64][64];
UBYTE heightMap3[32][32];
UBYTE heightMap4[16][16];
UBYTE colorMap0[256][256];
UBYTE colorMap1[128][128];
UBYTE colorMap2[64][64];
UBYTE colorMap3[32][32];
UBYTE colorMap4[16][16];
UBYTE screenMid[XSIZE*YSIZE];
UBYTE screenBig[XSIZE*YSIZE*4];
//UBYTE fastPlane[DEPTH][PLANEWIDTH*HEIGHT];

/*UBYTE fastPlane1[PLANEWIDTH*HEIGHT];
UBYTE fastPlane2[PLANEWIDTH*HEIGHT];
UBYTE fastPlane3[PLANEWIDTH*HEIGHT];
UBYTE fastPlane4[PLANEWIDTH*HEIGHT];*/

UWORD fastPlane1W[PLANEWIDTH*HEIGHT];
UWORD fastPlane2W[PLANEWIDTH*HEIGHT];
UWORD fastPlane3W[PLANEWIDTH*HEIGHT];
UWORD fastPlane4W[PLANEWIDTH*HEIGHT];

WORD rayCastX[XSIZE][TERRAINDEPTH];
WORD rayCastY[YSIZE][TERRAINDEPTH];


UWORD kolory[COLORS] =
{
	0x030,0x141,0x252,0x363,0x474,0x585,0x696,0x7a7,
	0x8b8,0x9c9,0xada,0xbeb,0xcfc,0xdfd,0xefe,0x99f
};
/*
{
0x040,0x242,0x363,0x383,0x3a3,0x3c3,0x3e3,0x3f3,
0x4f4,0x5f5,0x6f6,0x7f7,0x8f8,0x9f9,0xafa,0x99f
};
*/

/*UBYTE colorByte2[COLORS][COLORS][DEPTH];
UWORD colorByte4p1[COLORS][COLORS][COLORS][COLORS];
UWORD colorByte4p2[COLORS][COLORS][COLORS][COLORS];
UWORD colorByte4p3[COLORS][COLORS][COLORS][COLORS];
UWORD colorByte4p4[COLORS][COLORS][COLORS][COLORS];
*/
/*
UWORD colorByte4p1Flat[COLORS*COLORS*COLORS*COLORS];
UWORD colorByte4p2Flat[COLORS*COLORS*COLORS*COLORS];
UWORD colorByte4p3Flat[COLORS*COLORS*COLORS*COLORS];
UWORD colorByte4p4Flat[COLORS*COLORS*COLORS*COLORS];

UWORD colorByte4p1FlatBIG[COLORS*COLORS*COLORS*COLORS];
UWORD colorByte4p2FlatBIG[COLORS*COLORS*COLORS*COLORS];
UWORD colorByte4p3FlatBIG[COLORS*COLORS*COLORS*COLORS];
UWORD colorByte4p4FlatBIG[COLORS*COLORS*COLORS*COLORS];
*/
UBYTE colorByteDitherP1Even[COLORS*COLORS];
UBYTE colorByteDitherP2Even[COLORS*COLORS];
UBYTE colorByteDitherP3Even[COLORS*COLORS];
UBYTE colorByteDitherP4Even[COLORS*COLORS];

UBYTE colorByteDitherP1Odd[COLORS*COLORS];
UBYTE colorByteDitherP2Odd[COLORS*COLORS];
UBYTE colorByteDitherP3Odd[COLORS*COLORS];
UBYTE colorByteDitherP4Odd[COLORS*COLORS];

int xStart, yStart;
UWORD xPos, yPos;
int currentHeight;
int heightAheadRange;
int flightHeight;
int horizon;
int turn;
int interlace;
UBYTE renderingDepth = TERRAINDEPTH;

static void ReadHeight(const char *name)
{
	UBYTE byte;
	tFile *file;
	file = fileOpen(name, "r");
	if (file) {
		for (int x = 0; x < 256; x++) {
			for (int y = 0; y < 256; y++) {
				fileRead(file, &byte , 1);
				heightMap0[x][y] = byte;
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
				fileRead(file, &colorMap0[x][y], 1);
			}
		}
		fileClose(file);
	}
}
static void CalculateColorMipMaps()
{
	UWORD value;
	for (int x = 0; x < 128; x++)
	for (int y = 0; y < 128; y++)
	{
		value = 0;
		for(int a = 0; a<2 ;a++)
		for(int b = 0; b<2 ;b++)
		value += colorMap0[x*2+a][y*2+b];
		colorMap1[x][y] = value/4;
	}
	for (int x = 0; x < 64; x++)
	for (int y = 0; y < 64; y++)
	{
		value = 0;
		for(int a = 0; a<4 ;a++)
		for(int b = 0; b<4 ;b++)
		value += colorMap0[x*4+a][y*4+b];
		colorMap2[x][y] = value/16;
	}
	for (int x = 0; x < 32; x++)
	for (int y = 0; y < 32; y++)
	{
		value = 0;
		for(int a = 0; a<8 ;a++)
		for(int b = 0; b<8 ;b++)
		value += colorMap0[x*8+a][y*8+b];
		colorMap3[x][y] = value/64;
	}
	for (int x = 0; x < 16; x++)
	for (int y = 0; y < 16; y++)
	{
		value = 0;
		for(int a = 0; a<16 ;a++)
		for(int b = 0; b<16 ;b++)
		value += colorMap0[x*16+a][y*16+b];
		colorMap4[x][y] = value/256;
	}
}
static void CalculateHeightMipMaps()
{
	int value;
	for (int x = 0; x < 128; x++)
	for (int y = 0; y < 128; y++)
	{
		value = 0;
		for(int a = 0; a<2 ;a++)
		for(int b = 0; b<2 ;b++)
		value += heightMap0[x*2+a][y*2+b];
		heightMap1[x][y] = value/4;
	}
	for (int x = 0; x < 64; x++)
	for (int y = 0; y < 64; y++)
	{
		value = 0;
		for(int a = 0; a<4 ;a++)
		for(int b = 0; b<4 ;b++)
		value += heightMap0[x*4+a][y*4+b];
		heightMap2[x][y] = value/16;
	}
	for (int x = 0; x < 32; x++)
	for (int y = 0; y < 32; y++)
	{
		value = 0;
		for(int a = 0; a<8 ;a++)
		for(int b = 0; b<8 ;b++)
		value += heightMap0[x*8+a][y*8+b];
		heightMap3[x][y] = value/64;
	}
	for (int x = 0; x < 16; x++)
	for (int y = 0; y < 16; y++)
	{
		value = 0;
		for(int a = 0; a<16 ;a++)
		for(int b = 0; b<16 ;b++)
		value += heightMap0[x*16+a][y*16+b];
		heightMap4[x][y] = value/256;
	}
}

static void CopyFastToChipW(tBitMap *bm)
{
	CopyMemQuick(fastPlane1W, bm->Planes[0], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane2W, bm->Planes[1], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane3W, bm->Planes[2], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane4W, bm->Planes[3], PLANEWIDTH*HEIGHT);
}

static void CalculateRayCasts()
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
				rayCastX[XSIZE/2+sx][tz] = sxx/8;
				rayCastY[YSIZE/2+sy][tz] = syy/8;
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

/*static void GenerateColorBytes2()
{
for(int a=0;a<COLORS;a++)
for(int b=0;b<COLORS;b++)
for(int plane=0;plane<DEPTH;plane++)
{
colorByte2[a][b][plane] = ((a>>plane) & 1)*240 + ((b>>plane) & 1)*15;
}
}*/


/*static void GenerateColorBytes4()
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
}*/
/*
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
}*/
/*
static void GenerateColorInterpolationsByte()
{
	UWORD address;
	UBYTE a1,a2,a3,b1,b2,c1,c2,c3;
	for(int a=0;a<COLORS;a++)
	for(int b=0;b<COLORS;b++)
	for(int c=0;c<COLORS;c++)
	for(int d=0;d<COLORS;d++)
	{
		address = (a<<12) + (b<<8) + (c<<4) + d;

		//interpolate a to b
		if(a == 15)
		{
			a1 = b;
		}
		else if(b == 15)
		{
			a1 = a;
		}
		else
		{
			a1 = (a+b)/2;
		}

		//interpolate b to c
		if(b == 15)
		{
			b1 = c;
		}
		else if(c == 15)
		{
			b1 = b;
		}
		else
		{
			b1 = (b*2+c)/3;
			b2 = (b+c*2)/3;
		}

		//interpolate c to d
		if(c == 15)
		{
			c1 = d;
		}
		else if(d == 15)
		{
			c1 = c;
		}
		else
		{
			c1 = (c+d)/2;
		}

		//aa12 3bb4 5cc6 78dd
		colorInterpolationBIG[address]=
		((a>>0) & 1) *0xC000+
		((a1>>0) & 1)*0x2000+
		((b>>0) & 1) *0x0600+
		((b1>>0) & 1)*0x0100+
		((b2>>0) & 1)*0x0080+
		((c>>0) & 1)*0x0060+
		((c1>>0) & 1)*0x0010+
		((d>>0) & 1)*0x0003;
		colorInterpolationBIG[address]=

		colorInterpolationBIG[address]=

		colorInterpolationBIG[address]=

	}
}
*/
/*
static void GenerateColorBytes4FlatBIG()
{
	UWORD address;
	UBYTE a1,a2,a3,b1,b2,c1,c2,c3;
	for(int a=0;a<COLORS;a++)
	for(int b=0;b<COLORS;b++)
	for(int c=0;c<COLORS;c++)
	for(int d=0;d<COLORS;d++)
	{
		address = (a<<12) + (b<<8) + (c<<4) + d;

		//interpolate a to b
		if(a == 15)
		{
			a1 = b;
			a2 = b;
			a3 = b;
		}
		else if(b == 15)
		{
			a1 = a;
			a2 = a;
			a3 = a;
		}
		else
		{
			a1 = (a*2+b)/3;
			a2 = (a+b)/2;
			a3 = (a+b*2)/3;
		}

		//interpolate b to c
		if(b == 15)
		{
			b1 = c;
			b2 = c;
		}
		else if(c == 15)
		{
			b1 = b;
			b2 = b;
		}
		else
		{
			b1 = (b*2+c)/3;
			b2 = (b+c*2)/3;
		}

		//interpolate c to d
		if(c == 15)
		{
			c1 = d;
			c2 = d;
			c3 = d;
		}
		else if(d == 15)
		{
			c1 = c;
			c2 = c;
			c3 = c;
		}
		else
		{
			c1 = (c*2+d)/3;
			c2 = (c+d)/2;
			c3 = (c+d*2)/3;
		}

		//aa12 3bb4 5cc6 78dd
		colorByte4p1FlatBIG[address]=
		((a>>0) & 1) *0xC000+
		((a1>>0) & 1)*0x2000+
		((a2>>0) & 1)*0x1000+
		((a3>>0) & 1)*0x0800+
		((b>>0) & 1) *0x0600+
		((b1>>0) & 1)*0x0100+
		((b2>>0) & 1)*0x0080+
		((c>>0) & 1)*0x0060+
		((c1>>0) & 1)*0x0010+
		((c2>>0) & 1)*0x0008+
		((c3>>0) & 1)*0x0004+
		((d>>0) & 1)*0x0003;
		colorByte4p2FlatBIG[address]=
		((a>>1) & 1) *0xC000+
		((a1>>1) & 1)*0x2000+
		((a2>>1) & 1)*0x1000+
		((a3>>1) & 1)*0x0800+
		((b>>1) & 1) *0x0600+
		((b1>>1) & 1)*0x0100+
		((b2>>1) & 1)*0x0080+
		((c>>1) & 1)*0x0060+
		((c1>>1) & 1)*0x0010+
		((c2>>1) & 1)*0x0008+
		((c3>>1) & 1)*0x0004+
		((d>>1) & 1)*0x0003;
		colorByte4p3FlatBIG[address]=
		((a>>2) & 1) *0xC000+
		((a1>>2) & 1)*0x2000+
		((a2>>2) & 1)*0x1000+
		((a3>>2) & 1)*0x0800+
		((b>>2) & 1) *0x0600+
		((b1>>2) & 1)*0x0100+
		((b2>>2) & 1)*0x0080+
		((c>>2) & 1)*0x0060+
		((c1>>2) & 1)*0x0010+
		((c2>>2) & 1)*0x0008+
		((c3>>2) & 1)*0x0004+
		((d>>2) & 1)*0x0003;
		colorByte4p4FlatBIG[address]=
		((a>>3) & 1) *0xC000+
		((a1>>3) & 1)*0x2000+
		((a2>>3) & 1)*0x1000+
		((a3>>3) & 1)*0x0800+
		((b>>3) & 1) *0x0600+
		((b1>>3) & 1)*0x0100+
		((b2>>3) & 1)*0x0080+
		((c>>3) & 1)*0x0060+
		((c1>>3) & 1)*0x0010+
		((c2>>3) & 1)*0x0008+
		((c3>>3) & 1)*0x0004+
		((d>>3) & 1)*0x0003;
	}
}

*/

static void GenerateColorBytesDither()
{
	UWORD address;
	UBYTE a1,a2,a3,a4,b1,b2,b3,b4;
	for(int a=0;a<COLORS;a++)
	for(int b=0;b<COLORS;b++)
	{
		address = (a<<5) + b;

		if(a % 2)
		{
			a1 = a/2;
			a2 = a/2+1;
		}
		else
		{
			a1 = a/2;
			a2 = a/2;
		}
		if(b % 2)
		{
			b1 = b/2;
			b2 = b/2+1;
		}
		else
		{
			b1 = b/2;
			b2 = b/2;
		}
		colorByteDitherP1Even[address]=
		((a1>>0) & 1) *0b10000000+
		((a2>>0) & 1) *0b01000000+
		((a1>>0) & 1) *0b00100000+
		((a2>>0) & 1) *0b00010000+
		((b1>>0) & 1) *0b00001000+
		((b2>>0) & 1) *0b00000100+
		((b1>>0) & 1) *0b00000010+
		((b2>>0) & 1) *0b00000001;

		colorByteDitherP2Even[address]=
		((a1>>1) & 1) *0b10000000+
		((a2>>1) & 1) *0b01000000+
		((a1>>1) & 1) *0b00100000+
		((a2>>1) & 1) *0b00010000+
		((b1>>1) & 1) *0b00001000+
		((b2>>1) & 1) *0b00000100+
		((b1>>1) & 1) *0b00000010+
		((b2>>1) & 1) *0b00000001;

		colorByteDitherP3Even[address]=
		((a1>>2) & 1) *0b10000000+
		((a2>>2) & 1) *0b01000000+
		((a1>>2) & 1) *0b00100000+
		((a2>>2) & 1) *0b00010000+
		((b1>>2) & 1) *0b00001000+
		((b2>>2) & 1) *0b00000100+
		((b1>>2) & 1) *0b00000010+
		((b2>>2) & 1) *0b00000001;

		colorByteDitherP4Even[address]=
		((a1>>3) & 1) *0b10000000+
		((a2>>3) & 1) *0b01000000+
		((a1>>3) & 1) *0b00100000+
		((a2>>3) & 1) *0b00010000+
		((b1>>3) & 1) *0b00001000+
		((b2>>3) & 1) *0b00000100+
		((b1>>3) & 1) *0b00000010+
		((b2>>3) & 1) *0b00000001;

		colorByteDitherP1Odd[address]=
		((a2>>0) & 1) *0b10000000+
		((a1>>0) & 1) *0b01000000+
		((a2>>0) & 1) *0b00100000+
		((a1>>0) & 1) *0b00010000+
		((b2>>0) & 1) *0b00001000+
		((b1>>0) & 1) *0b00000100+
		((b2>>0) & 1) *0b00000010+
		((b1>>0) & 1) *0b00000001;

		colorByteDitherP2Odd[address]=
		((a2>>1) & 1) *0b10000000+
		((a1>>1) & 1) *0b01000000+
		((a2>>1) & 1) *0b00100000+
		((a1>>1) & 1) *0b00010000+
		((b2>>1) & 1) *0b00001000+
		((b1>>1) & 1) *0b00000100+
		((b2>>1) & 1) *0b00000010+
		((b1>>1) & 1) *0b00000001;

		colorByteDitherP3Odd[address]=
		((a2>>2) & 1) *0b10000000+
		((a1>>2) & 1) *0b01000000+
		((a2>>2) & 1) *0b00100000+
		((a1>>2) & 1) *0b00010000+
		((b2>>2) & 1) *0b00001000+
		((b1>>2) & 1) *0b00000100+
		((b2>>2) & 1) *0b00000010+
		((b1>>2) & 1) *0b00000001;

		colorByteDitherP4Odd[address]=
		((a2>>3) & 1) *0b10000000+
		((a1>>3) & 1) *0b01000000+
		((a2>>3) & 1) *0b00100000+
		((a1>>3) & 1) *0b00010000+
		((b2>>3) & 1) *0b00001000+
		((b1>>3) & 1) *0b00000100+
		((b2>>3) & 1) *0b00000010+
		((b1>>3) & 1) *0b00000001;

	}
}
/*static void DrawScreen2()
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
}*/

/*static void DrawScreen4()
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
}*/
/*
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

		for(UWORD x=0;x<XSIZE/4;x++)
		{
			address = (screenMid[sp]<<12) + (screenMid[sp+1]<<8) + (screenMid[sp+2]<<4) + screenMid[sp+3];

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
*/
/*
static void DrawScreen4FlatBIG()
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

		for(UWORD x=0;x<XSIZE/4;x++)
		{
			address = (screenMid[sp]<<12) + (screenMid[sp+1]<<8) + (screenMid[sp+2]<<4) + screenMid[sp+3];

			word = colorByte4p1FlatBIG[ address ];
			if(fastPlane1W[p1]!=word)
			{
				fastPlane1W[p1] = word;
				fastPlane1W[p2] = word;
				fastPlane1W[p3] = word;
				fastPlane1W[p4] = word;
			}

			word = colorByte4p2FlatBIG[ address ];
			if(fastPlane2W[p1] != word)
			{
				fastPlane2W[p1] = word;
				fastPlane2W[p2] = word;
				fastPlane2W[p3] = word;
				fastPlane2W[p4] = word;
			}

			word = colorByte4p3FlatBIG[ address ];
			if(fastPlane3W[p1] != word)
			{
				fastPlane3W[p1] = word;
				fastPlane3W[p2] = word;
				fastPlane3W[p3] = word;
				fastPlane3W[p4] = word;
			}

			word = colorByte4p4FlatBIG[ address ];
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
*/
/*
static void DrawScreenMediumEven()
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

		for(UWORD x=0;x<XSIZE/4;x++)
		{
			address = (screenMid[sp]<<12) + (screenMid[sp+1]<<8) + (screenMid[sp+2]<<4) + screenMid[sp+3];

			word = colorByte4p1FlatBIG[ address ];
			//	if(fastPlane1W[p1]!=word)
			{
				//fastPlane1W[p1] = word;
				fastPlane1W[p2] = word;
				//fastPlane1W[p3] = word;
				fastPlane1W[p4] = word;
			}

			word = colorByte4p2FlatBIG[ address ];
			//if(fastPlane2W[p1] != word)
			{
				//fastPlane2W[p1] = word;
				fastPlane2W[p2] = word;
				//fastPlane2W[p3] = word;
				fastPlane2W[p4] = word;
			}

			word = colorByte4p3FlatBIG[ address ];
			//	if(fastPlane3W[p1] != word)
			{
				//fastPlane3W[p1] = word;
				fastPlane3W[p2] = word;
				//	fastPlane3W[p3] = word;
				fastPlane3W[p4] = word;
			}

			word = colorByte4p4FlatBIG[ address ];
			//	if(fastPlane4W[p1] != word)
			{
				//	fastPlane4W[p1] = word;
				fastPlane4W[p2] = word;
				//fastPlane4W[p3] = word;
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

static void DrawScreenMediumOdd()
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
		//sp = y;

		for(UWORD x=0;x<XSIZE/4;x++)
		{
			address = (screenMid[sp]<<12) + (screenMid[sp+1]<<8) + (screenMid[sp+2]<<4) + screenMid[sp+3];

			word = colorByte4p1FlatBIG[ address ];
			//	if(fastPlane1W[p1]!=word)
			{
				fastPlane1W[p1] = word;
				//	fastPlane1W[p2] = word;
				fastPlane1W[p3] = word;
				//	fastPlane1W[p4] = word;
			}

			word = colorByte4p2FlatBIG[ address ];
			//if(fastPlane2W[p1] != word)
			{
				fastPlane2W[p1] = word;
				//	fastPlane2W[p2] = word;
				fastPlane2W[p3] = word;
				//	fastPlane2W[p4] = word;
			}

			word = colorByte4p3FlatBIG[ address ];
			//	if(fastPlane3W[p1] != word)
			{
				fastPlane3W[p1] = word;
				//	fastPlane3W[p2] = word;
				fastPlane3W[p3] = word;
				//	fastPlane3W[p4] = word;
			}

			word = colorByte4p4FlatBIG[ address ];
			//		if(fastPlane4W[p1] != word)
			{
				fastPlane4W[p1] = word;
				//	fastPlane4W[p2] = word;
				fastPlane4W[p3] = word;
				//	fastPlane4W[p4] = word;
			}

			p1++;
			p2++;
			p3++;
			p4++;

			sp+=4;
		}
	}
}
*/
static void DrawScreenMediumDitherEven()
{
	UWORD sp,p1,p2,p3,p4;
	UWORD word;
	UWORD address1, address2;
	sp = 0;

	for(UWORD y=0;y<YSIZE;y++)
	{
		p1 = y*PLANEWIDTH*4/2;
		p2 = p1+PLANEWIDTH/2;
		p3 = p2+PLANEWIDTH/2;
		p4 = p3+PLANEWIDTH/2;

		for(UWORD x=0;x<XSIZE/4;x++)
		{
			address1 = (screenMid[sp]<<5) + (screenMid[sp+1]);
			address2 = (screenMid[sp+2]<<5) + screenMid[sp+3];

			word = (colorByteDitherP1Even[ address1 ]<<8) + colorByteDitherP1Even[ address2 ];
			//	if(fastPlane1W[p1]!=word)
			{
				//fastPlane1W[p1] = word;
				fastPlane1W[p2] = word;
				//fastPlane1W[p3] = word;
				fastPlane1W[p4] = word;
			}

			word = (colorByteDitherP2Even[ address1 ]<<8) + colorByteDitherP2Even[ address2 ];
			//if(fastPlane2W[p1] != word)
			{
				//fastPlane2W[p1] = word;
				fastPlane2W[p2] = word;
				//fastPlane2W[p3] = word;
				fastPlane2W[p4] = word;
			}

			word = (colorByteDitherP3Even[ address1 ]<<8) + colorByteDitherP3Even[ address2 ];
			//	if(fastPlane3W[p1] != word)
			{
				//fastPlane3W[p1] = word;
				fastPlane3W[p2] = word;
				//	fastPlane3W[p3] = word;
				fastPlane3W[p4] = word;
			}

			word = (colorByteDitherP4Even[ address1 ]<<8) + colorByteDitherP4Even[ address2 ];
			//	if(fastPlane4W[p1] != word)
			{
				//	fastPlane4W[p1] = word;
				fastPlane4W[p2] = word;
				//fastPlane4W[p3] = word;
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

static void DrawScreenMediumDitherOdd()
{
	UWORD sp,p1,p2,p3,p4;
	UWORD word;
	UWORD address1,address2;
	sp = 0;

	for(UWORD y=0;y<YSIZE;y++)
	{
		p1 = y*PLANEWIDTH*4/2;
		p2 = p1+PLANEWIDTH/2;
		p3 = p2+PLANEWIDTH/2;
		p4 = p3+PLANEWIDTH/2;
		//sp = y;

		for(UWORD x=0;x<XSIZE/4;x++)
		{
			address1 = (screenMid[sp]<<5) + (screenMid[sp+1]);
			address2 = (screenMid[sp+2]<<5) + screenMid[sp+3];

			word = (colorByteDitherP1Odd[ address1 ]<<8) + colorByteDitherP1Odd[ address2 ];
			//	if(fastPlane1W[p1]!=word)
			{
				fastPlane1W[p1] = word;
				//fastPlane1W[p2] = word;
				fastPlane1W[p3] = word;
				//fastPlane1W[p4] = word;
			}

			word = (colorByteDitherP2Odd[ address1 ]<<8) + colorByteDitherP2Odd[ address2 ];
			//if(fastPlane2W[p1] != word)
			{
				fastPlane2W[p1] = word;
				//fastPlane2W[p2] = word;
				fastPlane2W[p3] = word;
				//fastPlane2W[p4] = word;
			}

			word = (colorByteDitherP3Odd[ address1 ]<<8) + colorByteDitherP3Odd[ address2 ];
			//	if(fastPlane3W[p1] != word)
			{
				fastPlane3W[p1] = word;
				//fastPlane3W[p2] = word;
				fastPlane3W[p3] = word;
				//fastPlane3W[p4] = word;
			}

			word = (colorByteDitherP4Odd[ address1 ]<<8) + colorByteDitherP4Odd[ address2 ];
			//	if(fastPlane4W[p1] != word)
			{
				fastPlane4W[p1] = word;
				//fastPlane4W[p2] = word;
				fastPlane4W[p3] = word;
				//fastPlane4W[p4] = word;
			}

			p1++;
			p2++;
			p3++;
			p4++;

			sp+=4;
		}
	}
}

/*static void DrawScreen4FlatMemCopy()
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
}*/
/*
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

		for(UWORD x=0;x<XSIZE/4;x++)
		{
			address = (screenMid[sp]<<12) + (screenMid[sp+1]<<8) + (screenMid[sp+2]<<4) + screenMid[sp+3];

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

		for(UWORD x=0;x<XSIZE/4;x++)
		{
			address = (screenMid[sp]<<12) + (screenMid[sp+1]<<8) + (screenMid[sp+2]<<4) + screenMid[sp+3];

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
*/
static void DrawTerrain(void)
{
	UBYTE sx,sy,tz;
	UBYTE th;
	UWORD position;
	UBYTE mx,my;
	UBYTE mipLevel;

	UWORD startPosition;

	startPosition = (YSIZE-1)*XSIZE; //set the start position (left bottom pixel) on the destination screen

	for(sx=0;sx<XSIZE;sx++)
	{
		//********* INITIALIZE INTERLACED CALCUTATIONS
		if(interlace > 0)
		{
			sy = 0;
			position = startPosition+sx;
		}
		else
		{
			sy = 1;
			position = startPosition+sx-XSIZE;
		}
		//********* INITIALIZE INTERLACED CALCUTATIONS

		//starting depth to look for height colission
		tz = 1;


		while(tz < renderingDepth && sy <YSIZE)
		{
			//***** set mipmap level
			mipLevel = tz/32;

			// set x,y pooositions on source maps
			mx = (xPos + rayCastX[sx][tz])/4;
			my = (yPos + tz)/4;

			//th = heightMap0[ mx ][ my ];
			//*********** HEIGHT MIPMAP
			if(mipLevel == 0)
			{
				th = heightMap0[ mx ][ my ];
			}
			else if(mipLevel == 1)
			{
				th = heightMap1[ mx/2 ][ my/2 ];
			}
			else if(mipLevel == 2)
			{
				th = heightMap2[ mx/4 ][ my/4 ];
			}
			else if(mipLevel == 3)
			{
				th = heightMap3[ mx/8 ][ my/8 ];
			}
			else if(mipLevel > 3)
			{
				th = heightMap4[ mx/16 ][ my/16 ];
			}
			//*********** HEIGHT MIPMAP

			//height to look for at a given x,y terrain coordinate accounting for z depth
			//************************************************************
			if(th>currentHeight + rayCastY[sy][tz])
			{
				//*************** COLOR MIPMAP
				if(mipLevel == 0)
				{
					screenMid[position] = colorMap0[ mx ][ my ];
				}
				else if(mipLevel == 1)
				{
					screenMid[position] = colorMap1[ mx/2 ][ my/2 ];
				}
				else if(mipLevel == 2)
				{
					screenMid[position] = colorMap2[ mx/4 ][ my/4 ];
				}
				else if(mipLevel == 3)
				{
					screenMid[position] = colorMap3[ mx/8 ][ my/8 ];
				}
				else if(mipLevel > 3)
				{
					screenMid[position] = colorMap4[ mx/16 ][ my/16 ];
				}
				//*************** COLOR MIPMAP

				screenMid[position] = th/4;

				sy+=2; //move 2 pixels to the top in calculations
				position-=XSIZE*2; //move 2 pixels to the top on the destination screen
			}
			else
			{
				tz+=1+tz/8; //move a variable step in depth to look for next height colision
			}
		}
		//finish vertical line with SKY
		while(sy < YSIZE)
		{
			sy++;
			//if(screenMid[position] == 0x0f)
			//break;
			screenMid[position] = 0x1f;
			position-=XSIZE;
		}
	}
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


	xStart = 50;
	yStart = 0;

	xPos = xStart;
	yPos = yStart;

	heightAheadRange = 5;

	flightHeight = 20;

	ReadHeight("height.raw");
		//ReadPalette("palette.raw");
	ReadColor("color.raw");
	CalculateRayCasts();
	//GenerateColorBytes2();
	//GenerateColorBytes4();
//	GenerateColorBytes4Flat();
//	GenerateColorBytes4FlatBIG();
GenerateColorBytesDither();
	//CalculateMipMap();
	CalculateColorMipMaps();
	CalculateHeightMipMaps();

	renderingDepth = 50;

	memcpy(s_pVPort->pPalette, kolory, 16 * sizeof(UWORD));

	s_pAvgTime = logAvgCreate("perf", 100);

	viewLoad(s_pView);
	keyCreate();
	systemUnuse();
}

void engineGsLoop(void) {
/*	if(yPos == 250)
	{
	gameClose();
}
yPos += 1;
xPos += 1;*/

if(keyCheck(KEY_SPACE)) {
	gameClose();
}
else
{
	if(keyCheck(KEY_UP))yPos++;
	if(keyCheck(KEY_DOWN))yPos--;
	if(keyCheck(KEY_RIGHT))xPos+=3;
	if(keyCheck(KEY_LEFT))xPos-=3;
	if(keyCheck(KEY_A))flightHeight+=2;
	if(keyCheck(KEY_Z))flightHeight-=2;
	if(keyCheck(KEY_S))renderingDepth++;
	if(keyCheck(KEY_X))renderingDepth--;

}
//		turn = turn/1.2;
//	xPos += turn;
if(renderingDepth<10) renderingDepth = 10;
else if(renderingDepth>TERRAINDEPTH) renderingDepth = TERRAINDEPTH;

if(flightHeight<10) flightHeight = 10;
currentHeight = flightHeight;
//currentHeight = (currentHeight*3 + flightHeight + height[xPos][yPos+heightAheadRange])/4;


logAvgBegin(s_pAvgTime);
if(interlace > 0)
{
	//
	DrawTerrain();
	//DrawScreen4FlatEven();
	//DrawScreen4FlatBIG();
	DrawScreenMediumDitherEven();
	interlace = -1;
}
else
{
	//
	DrawTerrain();
	//DrawScreen4FlatBIG();
	//DrawScreen4FlatOdd();
	DrawScreenMediumDitherOdd();
	interlace = 1;
}

//DrawScreen2();
//DrawScreen4();
//DrawScreen4Flat();
//DrawScreen4FlatBIG();
//DrawScreen4FlatMemCopy();



CopyFastToChipW(s_pBuffer->pBack);

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
