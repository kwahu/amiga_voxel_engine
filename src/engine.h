#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <stdlib.h>
#include <ace/managers/viewport/simplebuffer.h>

#define PLANEHEIGHT 256
#define PLANEWIDTH 40
#define DEPTH 4
#define COLORS 32

#define TERRAINDEPTH 64
#define XTURNBUFFOR 55
#define XSIZEODD 120+XTURNBUFFOR*2
#define XSIZEEVEN 80+XTURNBUFFOR*2
#define YSIZEODD 112
#define YSIZEEVEN YSIZEODD/2



UBYTE heightMap[256][256];
UBYTE colorMap[256][256];

UWORD mapLow[256][256]; UWORD mapMed[256][256]; UWORD mapHigh[256][256];

UWORD mapLow0[256][256]; UWORD mapMed0[256][256]; UWORD mapHigh0[256][256];
UWORD mapLow1[256][256]; UWORD mapMed1[256][256]; UWORD mapHigh1[256][256];
UWORD mapLow2[256][256]; UWORD mapMed2[256][256]; UWORD mapHigh2[256][256];
UWORD mapLow3[256][256]; UWORD mapMed3[256][256]; UWORD mapHigh3[256][256];
UWORD mapLow4[256][256]; UWORD mapMed4[256][256]; UWORD mapHigh4[256][256];
UWORD mapLow5[256][256]; UWORD mapMed5[256][256]; UWORD mapHigh5[256][256];
UWORD mapLow6[256][256]; UWORD mapMed6[256][256]; UWORD mapHigh6[256][256];
UWORD mapLow7[256][256]; UWORD mapMed7[256][256]; UWORD mapHigh7[256][256];
UWORD mapLow8[256][256]; UWORD mapMed8[256][256]; UWORD mapHigh8[256][256];
UWORD mapLow9[256][256]; UWORD mapMed9[256][256]; UWORD mapHigh9[256][256];
UWORD mapLow10[256][256]; UWORD mapMed10[256][256]; UWORD mapHigh10[256][256];

UWORD plane1W[PLANEWIDTH*PLANEHEIGHT];//20k
UWORD plane2W[PLANEWIDTH*PLANEHEIGHT];//20k
UWORD plane3W[PLANEWIDTH*PLANEHEIGHT];//20k
UWORD plane4W[PLANEWIDTH*PLANEHEIGHT];//20k

WORD rayCastXOdd[XSIZEODD][TERRAINDEPTH];//3,6k
WORD rayCastYOdd[YSIZEODD][TERRAINDEPTH];//3,6k

WORD rayCastXEven[XSIZEEVEN][TERRAINDEPTH];//3,6k
WORD rayCastYEven[YSIZEEVEN][TERRAINDEPTH];//3,6k


UBYTE debugValue,debugValue2,debugValue3,debugValue4,debugValue5,debugValue6,xFOV;


//sand dunes theme
UWORD kolory[COLORS] =
{
	0x101,0x323,0x523,0x743,0xa65,0xc85,0xeb8,0xfdb,
	0xbcc,0x8be,
	0x243,0x9b8,0xafd,0x324,0xa9b,0xdcf
};

UBYTE screen8x8slow[32*30];

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

// UBYTE screen2x2a[32*128];//4k
// UBYTE screen2x2b[32*128];//4k
// UBYTE screen2x2c[32*128];//4k
// UBYTE screen2x2d[32*128];//4k
// UBYTE screen2x2e[32*128];//4k

UBYTE screen3x2a[24*128];//3k
UBYTE screen3x2b[24*128];//3k
UBYTE screen3x2c[24*128];//3k
UBYTE screen3x2d[24*128];//3k
UBYTE screen3x2e[24*128];//3k

UWORD dither8x8EvenP1[COLORS*COLORS];//2k
UWORD dither8x8EvenP2[COLORS*COLORS];//2k
UWORD dither8x8EvenP3[COLORS*COLORS];//2k
UWORD dither8x8EvenP4[COLORS*COLORS];//2k
UWORD dither8x8OddP1[COLORS*COLORS];//2k
UWORD dither8x8OddP2[COLORS*COLORS];//2k
UWORD dither8x8OddP3[COLORS*COLORS];//2k
UWORD dither8x8OddP4[COLORS*COLORS];//2k

UBYTE dither4x4EvenP1[COLORS*COLORS];//1k
UBYTE dither4x4EvenP2[COLORS*COLORS];//1k
UBYTE dither4x4EvenP3[COLORS*COLORS];//1k
UBYTE dither4x4EvenP4[COLORS*COLORS];//1k
UBYTE dither4x4OddP1[COLORS*COLORS];//1k
UBYTE dither4x4OddP2[COLORS*COLORS];//1k
UBYTE dither4x4OddP3[COLORS*COLORS];//1k
UBYTE dither4x4OddP4[COLORS*COLORS];//1k

UBYTE dither3x2EvenP1[COLORS*COLORS*COLORS];//32k
UBYTE dither3x2EvenP2[COLORS*COLORS*COLORS];//32k
UBYTE dither3x2EvenP3[COLORS*COLORS*COLORS];//32k
UBYTE dither3x2EvenP4[COLORS*COLORS*COLORS];//32k
UBYTE dither3x2OddP1[COLORS*COLORS*COLORS];//32k
UBYTE dither3x2OddP2[COLORS*COLORS*COLORS];//32k
UBYTE dither3x2OddP3[COLORS*COLORS*COLORS];//32k
UBYTE dither3x2OddP4[COLORS*COLORS*COLORS];//32k


static LONG p1xf,p1yf,p1hf,p2xf,p2yf,p2hf;
static UWORD p1x,p1y,p1h,p2x,p2y,p2h;
static int interlace;
static UBYTE renderingDepth = TERRAINDEPTH;
static WORD cx,cy;
static ULONG startTime,endTime,deltaTime,lastTime;
static UWORD lastOverwrittenLine;
static BYTE xOffsetEven, xOffsetOdd; //camera rotation offsett when turning

void engineGsCreate(void);

void engineGsLoop(void);

void engineGsDestroy(void);


#endif // _ENGINE_H_
