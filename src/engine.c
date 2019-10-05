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
#define TERRAINDEPTH 30
#define XSIZE 60
#define YSIZE 60
#define LINEHEIGHT 4
#define YSTEPSCREEN XSIZE*4

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
UBYTE screenP2[XSIZE*YSIZE];
UBYTE screenP1[XSIZE*YSIZE];


UWORD fastPlane1W[PLANEWIDTH*HEIGHT];
UWORD fastPlane2W[PLANEWIDTH*HEIGHT];
UWORD fastPlane3W[PLANEWIDTH*HEIGHT];
UWORD fastPlane4W[PLANEWIDTH*HEIGHT];

WORD rayCastX[XSIZE][TERRAINDEPTH];
WORD rayCastY[YSIZE][TERRAINDEPTH];

UBYTE modulo2[XSIZE];

UWORD kolory[COLORS] =
{
	0x101,0x212,0x323,0x424,0x523,0x633,0x743,0x854,
	0xa65,0xb75,0xc85,0xd96,0xeb8,0xfdb,0xbcc,0x8be
};
/*
{
0x040,0x242,0x363,0x383,0x3a3,0x3c3,0x3e3,0x3f3,
0x4f4,0x5f5,0x6f6,0x7f7,0x8f8,0x9f9,0xafa,0x99f
};
*/

UBYTE colorByteDitherP1EvenHigh[COLORS*COLORS*COLORS];
UBYTE colorByteDitherP2EvenHigh[COLORS*COLORS*COLORS];
UBYTE colorByteDitherP3EvenHigh[COLORS*COLORS*COLORS];
UBYTE colorByteDitherP4EvenHigh[COLORS*COLORS*COLORS];

UBYTE colorByteDitherP1OddHigh[COLORS*COLORS*COLORS];
UBYTE colorByteDitherP2OddHigh[COLORS*COLORS*COLORS];
UBYTE colorByteDitherP3OddHigh[COLORS*COLORS*COLORS];
UBYTE colorByteDitherP4OddHigh[COLORS*COLORS*COLORS];


UWORD p1x,p1y,p1h,p2x,p2y,p2h;

//int heightAheadRange;
//int horizon;
//int turn;
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
		for(int i=0;i<15;i++)
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
		value /= 4;
		value -= 10;
		if(value < 0) value = 0;
		heightMap1[x][y] = value;
	}
	for (int x = 0; x < 64; x++)
	for (int y = 0; y < 64; y++)
	{
		value = 0;
		for(int a = 0; a<4 ;a++)
		for(int b = 0; b<4 ;b++)
		value += heightMap0[x*4+a][y*4+b];
		value /= 16;
		value -= 25;
		if(value < 0) value = 0;
		heightMap2[x][y] = value;
	}
	for (int x = 0; x < 32; x++)
	for (int y = 0; y < 32; y++)
	{
		value = 0;
		for(int a = 0; a<8 ;a++)
		for(int b = 0; b<8 ;b++)
		value += heightMap0[x*8+a][y*8+b];
		value /= 64;
		value -= 45;
		if(value < 0) value = 0;
		heightMap3[x][y] = value;
	}
	for (int x = 0; x < 16; x++)
	for (int y = 0; y < 16; y++)
	{
		value = 0;
		for(int a = 0; a<16 ;a++)
		for(int b = 0; b<16 ;b++)
		value += heightMap0[x*16+a][y*16+b];
		value /= 256;
		value -= 70;
		if(value < 0) value = 0;
		heightMap4[x][y] = value;
	}
}
static void SmoothHeightMap()
{
	int value;
	for (int x = 0; x < 256; x++)
	for (int y = 0; y < 256; y++)
	{
		value = 0;
		for(int a = -1; a<2 ;a++)
		for(int b = -1; b<2 ;b++)
		value += heightMap0[(UBYTE)(x+a)][(UBYTE)(y+b)];

		heightMap0[x][y] = value/9;
	}
}

static void SmoothColorMap()
{
	int value;
	for (int x = 0; x < 256; x++)
	for (int y = 0; y < 256; y++)
	{
		value = 0;
		for(int a = -1; a<2 ;a++)
		for(int b = -1; b<2 ;b++)
		value += colorMap0[(UBYTE)(x+a)][(UBYTE)(y+b)];

		colorMap0[x][y] = value/9;
	}
}

static void AddHeightToColorMap()
{
	int value;
	for (int x = 0; x < 256; x++)
	for (int y = 0; y < 256; y++)
	{
		value = colorMap0[x][y]+heightMap0[x][y]/32-5;
		if(value < 0) value = 0;
		if(value > 14) value = 14;
		colorMap0[x][y] = (UBYTE)(value);
	}
}
static void AddBumpToColorMap()
{
	int value;

		for (int x = 0; x < 256; x++)
		{
			for (int y = 0; y < 256; y++)
			{
				value =  heightMap0[x][y] - heightMap0[x+1][y]  + heightMap0[x][y] - heightMap0[x][y-1];
				value = colorMap0[x][y] + (16 + value / 2)/2;
				if(value < 0) value = 0;
				if(value > 14) value = 14;
				colorMap0[x][y] = (UBYTE)(value);
			}
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
	int tzz;

	tzz = 1;
	for(int tz=1;tz<TERRAINDEPTH;tz++)
	{
		tzz += (1+tz/4);
		for(int sx=-XSIZE/2;sx<XSIZE/2;sx++)
		{
			sxx = sx * tzz/2;
			for(int sy=-YSIZE/2;sy<YSIZE/2;sy++)
			{
				syy = sy * tzz/2;
				rayCastX[XSIZE/2+sx][tz] = sxx/8;
				rayCastY[YSIZE/2+sy][tz] = syy/8;
			}
		}
	}
}

static void GenerateColorBytesDitherHigh()
{
	UWORD address;
	UBYTE a1,a2,b1,b2,c1,c2;
	for(int a=0;a<COLORS;a++)
	for(int b=0;b<COLORS;b++)
	for(int c=0;c<COLORS;c++)
	{
		address = (a<<10) + (b<<5) + c;

		if(a % 2)
		{
			a1 = a/2;
			a2 = a/2;
		}
		else
		{
			a1 = a/2;
			a2 = a/2-1;
		}
		if(b % 2)
		{
			b1 = b/2;
			b2 = b/2;
		}
		else
		{
			b1 = b/2;
			b2 = b/2-1;
		}
		if(c % 2)
		{
			c1 = c/2;
			c2 = c/2;
		}
		else
		{
			c1 = c/2;
			c2 = c/2-1;
		}
		colorByteDitherP1EvenHigh[address]=
		((a1>>0) & 1) *0b10000000+
		((a2>>0) & 1) *0b01000000+
		((a1>>0) & 1) *0b00100000+
		((b2>>0) & 1) *0b00010000+
		((b1>>0) & 1) *0b00001000+
		((c2>>0) & 1) *0b00000100+
		((c1>>0) & 1) *0b00000010+
		((c2>>0) & 1) *0b00000001;

		colorByteDitherP2EvenHigh[address]=
		((a1>>1) & 1) *0b10000000+
		((a2>>1) & 1) *0b01000000+
		((a1>>1) & 1) *0b00100000+
		((b2>>1) & 1) *0b00010000+
		((b1>>1) & 1) *0b00001000+
		((c2>>1) & 1) *0b00000100+
		((c1>>1) & 1) *0b00000010+
		((c2>>1) & 1) *0b00000001;

		colorByteDitherP3EvenHigh[address]=
		((a1>>2) & 1) *0b10000000+
		((a2>>2) & 1) *0b01000000+
		((a1>>2) & 1) *0b00100000+
		((b2>>2) & 1) *0b00010000+
		((b1>>2) & 1) *0b00001000+
		((c2>>2) & 1) *0b00000100+
		((c1>>2) & 1) *0b00000010+
		((c2>>2) & 1) *0b00000001;

		colorByteDitherP4EvenHigh[address]=
		((a1>>3) & 1) *0b10000000+
		((a2>>3) & 1) *0b01000000+
		((a1>>3) & 1) *0b00100000+
		((b2>>3) & 1) *0b00010000+
		((b1>>3) & 1) *0b00001000+
		((c2>>3) & 1) *0b00000100+
		((c1>>3) & 1) *0b00000010+
		((c2>>3) & 1) *0b00000001;

		colorByteDitherP1OddHigh[address]=
		((a2>>0) & 1) *0b10000000+
		((a1>>0) & 1) *0b01000000+
		((b2>>0) & 1) *0b00100000+
		((b1>>0) & 1) *0b00010000+
		((b2>>0) & 1) *0b00001000+
		((b1>>0) & 1) *0b00000100+
		((c2>>0) & 1) *0b00000010+
		((c1>>0) & 1) *0b00000001;

		colorByteDitherP2OddHigh[address]=
		((a2>>1) & 1) *0b10000000+
		((a1>>1) & 1) *0b01000000+
		((b2>>1) & 1) *0b00100000+
		((b1>>1) & 1) *0b00010000+
		((b2>>1) & 1) *0b00001000+
		((b1>>1) & 1) *0b00000100+
		((c2>>1) & 1) *0b00000010+
		((c1>>1) & 1) *0b00000001;

		colorByteDitherP3OddHigh[address]=
		((a2>>2) & 1) *0b10000000+
		((a1>>2) & 1) *0b01000000+
		((b2>>2) & 1) *0b00100000+
		((b1>>2) & 1) *0b00010000+
		((b2>>2) & 1) *0b00001000+
		((b1>>2) & 1) *0b00000100+
		((c2>>2) & 1) *0b00000010+
		((c1>>2) & 1) *0b00000001;

		colorByteDitherP4OddHigh[address]=
		((a2>>3) & 1) *0b10000000+
		((a1>>3) & 1) *0b01000000+
		((b2>>3) & 1) *0b00100000+
		((b1>>3) & 1) *0b00010000+
		((b2>>3) & 1) *0b00001000+
		((b1>>3) & 1) *0b00000100+
		((c2>>3) & 1) *0b00000010+
		((c1>>3) & 1) *0b00000001;

	}
}
/*
static void DrawScreenHighDither(UBYTE player)
{
	UWORD sp,p1,p2,p3,p4;
	UWORD word;
	UWORD address1, address2;

	sp = 0;


	for(UWORD y=0;y<YSIZE;y++)
	{
		p1 = y*32+y*8;
		p2 = p1+20;

		for(UWORD x=0;x<XSIZE/6;x++)
		{
			address1 = (screenMid[sp]<<10) + (screenMid[sp+1]<<5) + (screenMid[sp+2]);
			address2 = (screenMid[sp+3]<<10) + (screenMid[sp+4]<<5) + (screenMid[sp+5]);

			fastPlane1W[p2] = (colorByteDitherP1EvenHigh[ address1 ]<<8) + colorByteDitherP1EvenHigh[ address2 ];
			fastPlane2W[p2] = (colorByteDitherP2EvenHigh[ address1 ]<<8) + colorByteDitherP2EvenHigh[ address2 ];
			fastPlane3W[p2] = (colorByteDitherP3EvenHigh[ address1 ]<<8) + colorByteDitherP3EvenHigh[ address2 ];
			fastPlane4W[p2] = (colorByteDitherP4EvenHigh[ address1 ]<<8) + colorByteDitherP4EvenHigh[ address2 ];

			fastPlane1W[p1] = (colorByteDitherP1EvenHigh[ address1 ]<<8) + colorByteDitherP1EvenHigh[ address2 ];
			fastPlane2W[p1] = (colorByteDitherP2EvenHigh[ address1 ]<<8) + colorByteDitherP2EvenHigh[ address2 ];
			fastPlane3W[p1] = (colorByteDitherP3EvenHigh[ address1 ]<<8) + colorByteDitherP3EvenHigh[ address2 ];
			fastPlane4W[p1] = (colorByteDitherP4EvenHigh[ address1 ]<<8) + colorByteDitherP4EvenHigh[ address2 ];

			p2++;
			p1++;
			sp+=6;
		}
	}
}*/
static void DrawScreenHighDitherEven(UBYTE player)
{
	UWORD sp,p1,p2,p3,p4;
	UWORD word;
	UWORD address1, address2;
	UBYTE *screen;
	UWORD startOffset;

	sp = 0;

	if(player == 1)
	{
		screen = screenP1;
		startOffset = 0;
	}
	else
	{
		screen = screenP2;
		startOffset = 10;
	}



	for(UWORD y=0;y<YSIZE;y++)
	{
		p2 = y*32+y*8+20 + startOffset;

		for(UWORD x=0;x<XSIZE/6;x++)
		{
			address1 = (screen[sp]<<10) + (screen[sp+1]<<5) + (screen[sp+2]);
			address2 = (screen[sp+3]<<10) + (screen[sp+4]<<5) + (screen[sp+5]);

			fastPlane1W[p2] = (colorByteDitherP1EvenHigh[ address1 ]<<8) + colorByteDitherP1EvenHigh[ address2 ];
			fastPlane2W[p2] = (colorByteDitherP2EvenHigh[ address1 ]<<8) + colorByteDitherP2EvenHigh[ address2 ];
			fastPlane3W[p2] = (colorByteDitherP3EvenHigh[ address1 ]<<8) + colorByteDitherP3EvenHigh[ address2 ];
			fastPlane4W[p2] = (colorByteDitherP4EvenHigh[ address1 ]<<8) + colorByteDitherP4EvenHigh[ address2 ];

			p2++;

			sp+=6;
		}
	}
}

static void DrawScreenHighDitherOdd(UBYTE player)
{
	UWORD sp,p1,p2,p3,p4;
	UWORD word;
	UWORD address1,address2;
	UBYTE *screen;
	UWORD startOffset;

	sp = 0;

	if(player == 1)
	{
		screen = screenP1;
		startOffset = 0;
	}
	else
	{
		screen = screenP2;
		startOffset = 10;
	}



	for(UWORD y=0;y<YSIZE;y++)
	{
		p1 = y*32+y*8 + startOffset;
		for(UWORD x=0;x<XSIZE/6;x++)
		{
			address1 = (screen[sp]<<10) + (screen[sp+1]<<5) + (screen[sp+2]);
			address2 = (screen[sp+3]<<10) + (screen[sp+4]<<5) + (screen[sp+5]);

			fastPlane1W[p1] = (colorByteDitherP1OddHigh[ address1 ]<<8) + colorByteDitherP1OddHigh[ address2 ];
			fastPlane2W[p1] = (colorByteDitherP2OddHigh[ address1 ]<<8) + colorByteDitherP2OddHigh[ address2 ];
			fastPlane3W[p1] = (colorByteDitherP3OddHigh[ address1 ]<<8) + colorByteDitherP3OddHigh[ address2 ];
			fastPlane4W[p1] = (colorByteDitherP4OddHigh[ address1 ]<<8) + colorByteDitherP4OddHigh[ address2 ];

			p1++;
			sp+=6;
		}
	}
}
static void CalculateModulo2()
{
	for(UBYTE sx=0;sx<XSIZE;sx++)
	{
		modulo2[sx] = sx % 2;
	}
}
static void DrawColorMap(UBYTE player)
{
	UWORD p1,p2,p3,p4;
	UWORD position;
	UBYTE b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16;
	UBYTE xx,yy;
	UBYTE offsetx,offsety;
	UWORD startOffset;

	if(player == 1)
	{
		offsety = p1y/4;
		startOffset = 5;
	}
	else
	{
		offsety = p2y/4;
		startOffset = 15;
	}

	position = startOffset+192*20;

	for (int y = 0; y < 64; y++)
	{
		yy = (y - offsety) % 64;

	for (int x = 0; x < 64/16; x++)
	{
		xx = x*16;
		//if(xx > 48) xx -= 48;

		b1 = colorMap2[(UBYTE)(xx)][yy];
		b2 = colorMap2[(UBYTE)(xx+1)][yy];
		b3 = colorMap2[(UBYTE)(xx+2)][yy];
		b4 = colorMap2[(UBYTE)(xx+3)][yy];
		b5 = colorMap2[(UBYTE)(xx+4)][yy];
		b6 = colorMap2[(UBYTE)(xx+5)][yy];
		b7 = colorMap2[(UBYTE)(xx+6)][yy];
		b8 = colorMap2[(UBYTE)(xx+7)][yy];
		b9 = colorMap2[(UBYTE)(xx+8)][yy];
		b10 = colorMap2[(UBYTE)(xx+9)][yy];
		b11 = colorMap2[(UBYTE)(xx+10)][yy];
		b12 = colorMap2[(UBYTE)(xx+11)][yy];
		b13 = colorMap2[(UBYTE)(xx+12)][yy];
		b14 = colorMap2[(UBYTE)(xx+13)][yy];
		b15 = colorMap2[(UBYTE)(xx+14)][yy];
		b16 = colorMap2[(UBYTE)(xx+15)][yy];
		fastPlane1W[position] = ((b1>>0) & 1) *0b1000000000000000+
		((b2>>0) & 1) *0b0100000000000000+
		((b3>>0) & 1) *0b0010000000000000+
		((b4>>0) & 1) *0b0001000000000000+
		((b5>>0) & 1) *0b0000100000000000+
		((b6>>0) & 1) *0b0000010000000000+
		((b7>>0) & 1) *0b0000001000000000+
		((b8>>0) & 1) *0b0000000100000000+
		((b9>>0) & 1) *0b0000000010000000+
		((b10>>0) & 1) *0b0000000001000000+
		((b11>>0) & 1) *0b0000000000100000+
		((b12>>0) & 1) *0b0000000000010000+
		((b13>>0) & 1) *0b0000000000001000+
		((b14>>0) & 1) *0b0000000000000100+
		((b15>>0) & 1) *0b0000000000000010+
		((b16>>0) & 1) *0b0000000000000001;
		fastPlane2W[position] = ((b1>>1) & 1) *0b1000000000000000+
		((b2>>1) & 1) *0b0100000000000000+
		((b3>>1) & 1) *0b0010000000000000+
		((b4>>1) & 1) *0b0001000000000000+
		((b5>>1) & 1) *0b0000100000000000+
		((b6>>1) & 1) *0b0000010000000000+
		((b7>>1) & 1) *0b0000001000000000+
		((b8>>1) & 1) *0b0000000100000000+
		((b9>>1) & 1) *0b0000000010000000+
		((b10>>1) & 1) *0b0000000001000000+
		((b11>>1) & 1) *0b0000000000100000+
		((b12>>1) & 1) *0b0000000000010000+
		((b13>>1) & 1) *0b0000000000001000+
		((b14>>1) & 1) *0b0000000000000100+
		((b15>>1) & 1) *0b0000000000000010+
		((b16>>1) & 1) *0b0000000000000001;
		fastPlane3W[position] = ((b1>>2) & 1) *0b1000000000000000+
		((b2>>2) & 1) *0b0100000000000000+
		((b3>>2) & 1) *0b0010000000000000+
		((b4>>2) & 1) *0b0001000000000000+
		((b5>>2) & 1) *0b0000100000000000+
		((b6>>2) & 1) *0b0000010000000000+
		((b7>>2) & 1) *0b0000001000000000+
		((b8>>2) & 1) *0b0000000100000000+
		((b9>>2) & 1) *0b0000000010000000+
		((b10>>2) & 1) *0b0000000001000000+
		((b11>>2) & 1) *0b0000000000100000+
		((b12>>2) & 1) *0b0000000000010000+
		((b13>>2) & 1) *0b0000000000001000+
		((b14>>2) & 1) *0b0000000000000100+
		((b15>>2) & 1) *0b0000000000000010+
		((b16>>2) & 1) *0b0000000000000001;
		fastPlane4W[position] = ((b1>>3) & 1) *0b1000000000000000+
		((b2>>3) & 1) *0b0100000000000000+
		((b3>>3) & 1) *0b0010000000000000+
		((b4>>3) & 1) *0b0001000000000000+
		((b5>>3) & 1) *0b0000100000000000+
		((b6>>3) & 1) *0b0000010000000000+
		((b7>>3) & 1) *0b0000001000000000+
		((b8>>3) & 1) *0b0000000100000000+
		((b9>>3) & 1) *0b0000000010000000+
		((b10>>3) & 1) *0b0000000001000000+
		((b11>>3) & 1) *0b0000000000100000+
		((b12>>3) & 1) *0b0000000000010000+
		((b13>>3) & 1) *0b0000000000001000+
		((b14>>3) & 1) *0b0000000000000100+
		((b15>>3) & 1) *0b0000000000000010+
		((b16>>3) & 1) *0b0000000000000001;
		position++;
	}
	position+=PLANEWIDTH/2-4;
}
}
static void DrawHeightMap(UBYTE player)
{
	UWORD p1,p2,p3,p4;
	UWORD position;
	UBYTE b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16;
	UBYTE xx,yy;
	UBYTE offsetx,offsety;
	UWORD startOffset;

	if(player == 1)
	{
		offsety = p1y/4;
		startOffset = 0;
	}
	else
	{
		offsety = p2y/4;
		startOffset = 10;
	}



	position = startOffset+192*20;
	for (int y = 0; y < 64; y++)
	{
		yy = (y - offsety) % 64;

	for (int x = 0; x < 64/16; x++)
	{
		xx = x*16;
		//if(xx > 48) xx -= 48;

		b1 = heightMap2[(UBYTE)(xx)][yy]/16;
		b2 = heightMap2[(UBYTE)(xx+1)][yy]/16;
		b3 = heightMap2[(UBYTE)(xx+2)][yy]/16;
		b4 = heightMap2[(UBYTE)(xx+3)][yy]/16;
		b5 = heightMap2[(UBYTE)(xx+4)][yy]/16;
		b6 = heightMap2[(UBYTE)(xx+5)][yy]/16;
		b7 = heightMap2[(UBYTE)(xx+6)][yy]/16;
		b8 = heightMap2[(UBYTE)(xx+7)][yy]/16;
		b9 = heightMap2[(UBYTE)(xx+8)][yy]/16;
		b10 = heightMap2[(UBYTE)(xx+9)][yy]/16;
		b11 = heightMap2[(UBYTE)(xx+10)][yy]/16;
		b12 = heightMap2[(UBYTE)(xx+11)][yy]/16;
		b13 = heightMap2[(UBYTE)(xx+12)][yy]/16;
		b14 = heightMap2[(UBYTE)(xx+13)][yy]/16;
		b15 = heightMap2[(UBYTE)(xx+14)][yy]/16;
		b16 = heightMap2[(UBYTE)(xx+15)][yy]/16;
		fastPlane1W[position] = ((b1>>0) & 1) *0b1000000000000000+
		((b2>>0) & 1) *0b0100000000000000+
		((b3>>0) & 1) *0b0010000000000000+
		((b4>>0) & 1) *0b0001000000000000+
		((b5>>0) & 1) *0b0000100000000000+
		((b6>>0) & 1) *0b0000010000000000+
		((b7>>0) & 1) *0b0000001000000000+
		((b8>>0) & 1) *0b0000000100000000+
		((b9>>0) & 1) *0b0000000010000000+
		((b10>>0) & 1) *0b0000000001000000+
		((b11>>0) & 1) *0b0000000000100000+
		((b12>>0) & 1) *0b0000000000010000+
		((b13>>0) & 1) *0b0000000000001000+
		((b14>>0) & 1) *0b0000000000000100+
		((b15>>0) & 1) *0b0000000000000010+
		((b16>>0) & 1) *0b0000000000000001;
		fastPlane2W[position] = ((b1>>1) & 1) *0b1000000000000000+
		((b2>>1) & 1) *0b0100000000000000+
		((b3>>1) & 1) *0b0010000000000000+
		((b4>>1) & 1) *0b0001000000000000+
		((b5>>1) & 1) *0b0000100000000000+
		((b6>>1) & 1) *0b0000010000000000+
		((b7>>1) & 1) *0b0000001000000000+
		((b8>>1) & 1) *0b0000000100000000+
		((b9>>1) & 1) *0b0000000010000000+
		((b10>>1) & 1) *0b0000000001000000+
		((b11>>1) & 1) *0b0000000000100000+
		((b12>>1) & 1) *0b0000000000010000+
		((b13>>1) & 1) *0b0000000000001000+
		((b14>>1) & 1) *0b0000000000000100+
		((b15>>1) & 1) *0b0000000000000010+
		((b16>>1) & 1) *0b0000000000000001;
		fastPlane3W[position] = ((b1>>2) & 1) *0b1000000000000000+
		((b2>>2) & 1) *0b0100000000000000+
		((b3>>2) & 1) *0b0010000000000000+
		((b4>>2) & 1) *0b0001000000000000+
		((b5>>2) & 1) *0b0000100000000000+
		((b6>>2) & 1) *0b0000010000000000+
		((b7>>2) & 1) *0b0000001000000000+
		((b8>>2) & 1) *0b0000000100000000+
		((b9>>2) & 1) *0b0000000010000000+
		((b10>>2) & 1) *0b0000000001000000+
		((b11>>2) & 1) *0b0000000000100000+
		((b12>>2) & 1) *0b0000000000010000+
		((b13>>2) & 1) *0b0000000000001000+
		((b14>>2) & 1) *0b0000000000000100+
		((b15>>2) & 1) *0b0000000000000010+
		((b16>>2) & 1) *0b0000000000000001;
		fastPlane4W[position] = ((b1>>3) & 1) *0b1000000000000000+
		((b2>>3) & 1) *0b0100000000000000+
		((b3>>3) & 1) *0b0010000000000000+
		((b4>>3) & 1) *0b0001000000000000+
		((b5>>3) & 1) *0b0000100000000000+
		((b6>>3) & 1) *0b0000010000000000+
		((b7>>3) & 1) *0b0000001000000000+
		((b8>>3) & 1) *0b0000000100000000+
		((b9>>3) & 1) *0b0000000010000000+
		((b10>>3) & 1) *0b0000000001000000+
		((b11>>3) & 1) *0b0000000000100000+
		((b12>>3) & 1) *0b0000000000010000+
		((b13>>3) & 1) *0b0000000000001000+
		((b14>>3) & 1) *0b0000000000000100+
		((b15>>3) & 1) *0b0000000000000010+
		((b16>>3) & 1) *0b0000000000000001;
		position++;
	}
	position+=PLANEWIDTH/2-4;
}
}
static void DrawTerrain(UBYTE player)
{
	UBYTE sx,sy,tz;
	UWORD px,py,ph;
	UBYTE th;
	UWORD position;
	UBYTE mx,my;
	UBYTE mipLevel;
	UWORD startPosition;
	UBYTE *screen;

	//initialize player data
	if(player == 1)
	{
		ph = p1h;
		px = p1x;
		py = p1y;
		screen = screenP1;
	}
	else
	{
		ph = p2h;
		px = p2x;
		py = p2y;
		screen = screenP2;
	}

	startPosition = (YSIZE-1)*XSIZE; //set the start position (left bottom pixel) on the destination screen

	for(sx=0;sx<XSIZE;sx++)
	{
		//********* INITIALIZE INTERLACED CALCUTATIONS


			sy = interlace + modulo2[sx];
			position = startPosition+sx-(XSIZE*sy);


		//********* INITIALIZE INTERLACED CALCUTATIONS

		//starting depth to look for height colission
		tz = 1;


		while(tz < renderingDepth && sy <YSIZE)
		{
			//***** set mipmap level
			mipLevel = tz/8;

			// set x,y pooositions on source maps
			mx = (px + rayCastX[sx][tz]);
			my = (py + tz);

			//th = heightMap0[ mx ][ my ];
			//*********** HEIGHT MIPMAP
			if(mipLevel < 2)
			{
				th = heightMap0[ mx ][ my ];
			}
			else if(mipLevel == 2)
			{
				th = heightMap1[ mx/2 ][ my/2 ];
			}
			else if(mipLevel == 3)
			{
				th = heightMap2[ mx/4 ][ my/4 ];
			}
			else if(mipLevel == 4)
			{
				th = heightMap3[ mx/8 ][ my/8 ];
			}
			else if(mipLevel > 4)
			{
				th = heightMap4[ mx/16 ][ my/16 ];
			}
			//*********** HEIGHT MIPMAP

			//height to look for at a given x,y terrain coordinate accounting for z depth
			//************************************************************
			if(th>ph + rayCastY[sy][tz])
			{
				//screenMid[position] = th/16 + sy/8;
				//screenMid[position] = colorMap0[ mx ][ my ];
				//screenMid[position] = colorMap0[ mx ][ my ];
				//*************** COLOR MIPMAP
				if(mipLevel == 0)
				{
					screen[position] = colorMap0[ mx ][ my ] ;
				}
				else if(mipLevel == 1)
				{
					screen[position] = colorMap1[ mx/2 ][ my/2 ] + 2;
				}
				else if(mipLevel == 2)
				{
					screen[position] = colorMap2[ mx/4 ][ my/4 ] + 3;
				}
				else if(mipLevel == 3)
				{
					screen[position] = colorMap3[ mx/8 ][ my/8 ] + 4;
				}
				else if(mipLevel > 3)
				{
					screen[position] = colorMap4[ mx/16 ][ my/16 ] + 5;
				}
				//*************** COLOR MIPMAP

				// check shadow

			/*	if(mx < xPos+5 && mx > xPos-5 && my < yPos+5 && my > yPos-5)
					screenMid[position] = screenMid[position]/2;*/

				// check shadow


				sy+=4; //move X pixels to the top in calculations
				position-=YSTEPSCREEN; //move X pixels to the top on the destination screen
			}
			else
			{
				tz+=1;//+mipLevel; //move a variable step in depth to look for next height colision
			}
		}
		//finish vertical line with SKY
		while(sy < YSIZE)
		{
			sy++;
			//if(screenMid[position] == 0x0f)
			//break;
			screen[position] = sy/2;//0x1f;
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


	p1x = 64;
	p1y = 0;
	p1h = 30;

	p2x = 200;
	p2y = 0;
	p2h = 60;

	ReadHeight("height.raw");
//	ReadPalette("palette.raw");
	ReadColor("color.raw");
	CalculateRayCasts();

	SmoothHeightMap();
	SmoothColorMap();
	AddHeightToColorMap();
	AddBumpToColorMap();


	GenerateColorBytesDitherHigh();
	CalculateColorMipMaps();
	CalculateHeightMipMaps();
  CalculateModulo2();


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
yPos += 1;*/
//xPos += 1;

if(keyCheck(KEY_SPACE)) {
	gameClose();
}
else
{
	if(keyCheck(KEY_UP))p1y++;
	if(keyCheck(KEY_DOWN))p1y--;
	if(keyCheck(KEY_RIGHT))p1x+=2;
	if(keyCheck(KEY_LEFT))p1x-=2;
	if(keyCheck(KEY_F))p1h+=3;
	if(keyCheck(KEY_V))p1h-=3;
	if(keyCheck(KEY_W))p2y++;
	if(keyCheck(KEY_S))p2y--;
	if(keyCheck(KEY_D))p2x+=2;
	if(keyCheck(KEY_A))p2x-=2;
	if(keyCheck(KEY_G))p2h+=3;
	if(keyCheck(KEY_B))p2h-=3;
	if(keyCheck(KEY_H))renderingDepth++;
	if(keyCheck(KEY_N))renderingDepth--;

}
//		turn = turn/1.2;
//	xPos += turn;
if(renderingDepth<10) renderingDepth = 10;
else if(renderingDepth>TERRAINDEPTH) renderingDepth = TERRAINDEPTH;

/*
if(flightHeight<10) flightHeight = 10;
currentHeight = flightHeight;*/

logAvgBegin(s_pAvgTime);

if(interlace % 2)
{
	DrawTerrain(1);
	DrawScreenHighDitherEven(1);
}
else
{
	DrawTerrain(1);
	DrawScreenHighDitherOdd(1);
}

if(interlace % 2)
{
	DrawTerrain(2);
	DrawScreenHighDitherEven(2);
}
else
{
	DrawTerrain(2);
	DrawScreenHighDitherOdd(2);
}

if(interlace == 0) DrawColorMap(1);
if(interlace == 1) DrawHeightMap(1);
if(interlace == 2) DrawColorMap(2);
if(interlace == 3) DrawHeightMap(2);
//DrawTerrain();
//DrawScreenHighDither();

interlace++;
if(interlace == 4) interlace = 0;

CopyFastToChipW(s_pBuffer->pBack);

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
