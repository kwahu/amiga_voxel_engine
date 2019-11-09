#ifndef _RAY_CASTING_H_
#define _RAY_CASTING_H_


#include <stdlib.h>
#include <ace/managers/viewport/simplebuffer.h>
//#define WIDTH 320
#define PLANEHEIGHT 256
#define PLANEWIDTH 40
#define DEPTH 4
#define COLORS 32

#define TERRAINDEPTH 64
#define XSIZEODD 120
#define XSIZEEVEN 160
#define YSIZE 128

 UBYTE heightMap0[256][256];//64k
 UBYTE heightMap1[256][256];//16k
 UBYTE heightMap2[256][256];//4k
 //UBYTE heightMap3[32][32];//1k
 //UBYTE heightMap4[16][16];

 UBYTE colorMap0[256][256];//64
 UBYTE colorMap1[256][256];//16
 UBYTE colorMap2[256][256];//4
 //UBYTE colorMap3[32][32];//1
 //UBYTE colorMap4[16][16];

 UWORD mapLow[256][256];
 UWORD mapMed[256][256];
 UWORD mapHigh[256][256];

 UWORD plane1W[PLANEWIDTH*PLANEHEIGHT];//20k
 UWORD plane2W[PLANEWIDTH*PLANEHEIGHT];//20k
 UWORD plane3W[PLANEWIDTH*PLANEHEIGHT];//20k
 UWORD plane4W[PLANEWIDTH*PLANEHEIGHT];//20k

 WORD rayCastXOdd[XSIZEODD][TERRAINDEPTH];//3,6k
 WORD rayCastYOdd[YSIZE][TERRAINDEPTH];//3,6k

 WORD rayCastXEven[XSIZEEVEN][TERRAINDEPTH];//3,6k
 WORD rayCastYEven[YSIZE][TERRAINDEPTH];//3,6k

//static WORD rayCastXX[40*TERRAINDEPTH];
//static WORD rayCastYY[32*TERRAINDEPTH];

 //UWORD rayCastXYLow[32*32*TERRAINDEPTH];

 //UWORD rayCastXYEven[128*128*TERRAINDEPTH];

//UBYTE enemyPlacementPerspective[XSIZE/2][128];

//UBYTE modulo2[XSIZE];

UBYTE debugValue,debugValue2,debugValue3,debugValue4;


//sand dunes theme
UWORD kolory[COLORS] =
{
	0x101,0x212,0x323,0x424,0x523,0x633,0x743,0x854,
	0xa65,0xb75,0xc85,0xd96,0xeb8,0xfdb,0xbcc,0x8be
};

// UBYTE screen8x8slow[40*32];

 UBYTE screen8x8a[8*32];//0,256k
 UBYTE screen8x8b[8*32];//0,256k
 UBYTE screen8x8c[8*32];//0,256k
 UBYTE screen8x8d[8*32];//0,256k
 UBYTE screen8x8e[8*32];//0,256k

 UBYTE screen4x4a[16*64];//1k
 UBYTE screen4x4b[16*64];//1k
 UBYTE screen4x4c[16*64];//1k
 UBYTE screen4x4d[16*64];//1k
 UBYTE screen4x4e[16*64];//1k

 UBYTE screen2x2a[32*128];//4k
 UBYTE screen2x2b[32*128];//4k
 UBYTE screen2x2c[32*128];//4k
 UBYTE screen2x2d[32*128];//4k
 UBYTE screen2x2e[32*128];//4k

 UBYTE screen3x2a[24*128];//3k
 UBYTE screen3x2b[24*128];//3k
 UBYTE screen3x2c[24*128];//3k
 UBYTE screen3x2d[24*128];//3k
 UBYTE screen3x2e[24*128];//3k

static UWORD dither8x8EvenP1[COLORS*COLORS];//2k
static UWORD dither8x8EvenP2[COLORS*COLORS];//2k
static UWORD dither8x8EvenP3[COLORS*COLORS];//2k
static UWORD dither8x8EvenP4[COLORS*COLORS];//2k
static UWORD dither8x8OddP1[COLORS*COLORS];//2k
static UWORD dither8x8OddP2[COLORS*COLORS];//2k
static UWORD dither8x8OddP3[COLORS*COLORS];//2k
static UWORD dither8x8OddP4[COLORS*COLORS];//2k

static UBYTE dither4x4EvenP1[COLORS*COLORS];//1k
static UBYTE dither4x4EvenP2[COLORS*COLORS];//1k
static UBYTE dither4x4EvenP3[COLORS*COLORS];//1k
static UBYTE dither4x4EvenP4[COLORS*COLORS];//1k
static UBYTE dither4x4OddP1[COLORS*COLORS];//1k
static UBYTE dither4x4OddP2[COLORS*COLORS];//1k
static UBYTE dither4x4OddP3[COLORS*COLORS];//1k
static UBYTE dither4x4OddP4[COLORS*COLORS];//1k

UBYTE dither3x2EvenP1[COLORS*COLORS*COLORS];//32k
UBYTE dither3x2EvenP2[COLORS*COLORS*COLORS];//32k
UBYTE dither3x2EvenP3[COLORS*COLORS*COLORS];//32k
UBYTE dither3x2EvenP4[COLORS*COLORS*COLORS];//32k
UBYTE dither3x2OddP1[COLORS*COLORS*COLORS];//32k
UBYTE dither3x2OddP2[COLORS*COLORS*COLORS];//32k
UBYTE dither3x2OddP3[COLORS*COLORS*COLORS];//32k
UBYTE dither3x2OddP4[COLORS*COLORS*COLORS];//32k

// UBYTE dither2x2P1Even[COLORS*COLORS*COLORS*COLORS];//64k
// UBYTE dither2x2P2Even[COLORS*COLORS*COLORS*COLORS];//64k
// UBYTE dither2x2P3Even[COLORS*COLORS*COLORS*COLORS];//64k
// UBYTE dither2x2P4Even[COLORS*COLORS*COLORS*COLORS];//64k
/*
 UBYTE dither2x2P1Odd[COLORS/2*COLORS*COLORS*COLORS/2];//64k
 UBYTE dither2x2P2Odd[COLORS/2*COLORS*COLORS*COLORS/2];//64k
 UBYTE dither2x2P3Odd[COLORS/2*COLORS*COLORS*COLORS/2];//64k
 UBYTE dither2x2P4Odd[COLORS/2*COLORS*COLORS*COLORS/2];//64k*/

static UWORD p1xf,p1yf,p1hf,p2xf,p2yf,p2hf;
static UWORD p1x,p1y,p1h,p2x,p2y,p2h;
static int interlace;
static UBYTE renderingDepth = TERRAINDEPTH;
static WORD cx,cy;

void engineGsCreate(void);

void engineGsLoop(void);

void engineGsDestroy(void);


#endif // _RAY_CASTING_H_
