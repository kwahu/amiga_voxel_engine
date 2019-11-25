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

UBYTE heightMap[256][256];//65k
UBYTE colorMap[256][256];//65k

UWORD mapHigh[256][256];//131k

UWORD mapHigh0[256][256];//131k
UWORD mapHigh1[256][256];//131k
UWORD mapHigh2[256][256];//131k
UWORD mapHigh3[256][256];//131k
UWORD mapHigh4[256][256];//131k
UWORD mapHigh5[256][256];//131k
UWORD mapHigh6[256][256];//131k
UWORD mapHigh7[256][256];//131k
UWORD mapHigh8[256][256];//131k
UWORD mapHigh9[256][256];//131k
UWORD mapHigh10[256][256];//131k
UBYTE mapLoaded0,mapLoaded1,mapLoaded2,mapLoaded3,mapLoaded4,mapLoaded5,mapLoaded6,
mapLoaded7,mapLoaded8,mapLoaded9,mapLoaded10;

UWORD plane1W[PLANEWIDTH*PLANEHEIGHT];//20k
UWORD plane2W[PLANEWIDTH*PLANEHEIGHT];//20k
UWORD plane3W[PLANEWIDTH*PLANEHEIGHT];//20k
UWORD plane4W[PLANEWIDTH*PLANEHEIGHT];//20k

WORD rayCastXOdd[XSIZEODD][TERRAINDEPTH];//51k
WORD rayCastYOdd[YSIZEODD][TERRAINDEPTH];//51k

WORD rayCastXEven[XSIZEEVEN][TERRAINDEPTH];//21k
WORD rayCastYEven[YSIZEEVEN][TERRAINDEPTH];//21k


UBYTE debugValue,debugValue2,debugValue3,debugValue4,debugValue5,debugValue6,xFOV;
BYTE debugValue8,debugValue9;


//sand dunes theme
UWORD kolory[COLORS] =
{
	0x101,0x323,0x523,0x743,0xa65,0xc85,0xeb8,0xfdb,
	0xbcc,0x8be,
	0x243,0x9b8,0xafd,0x324,0xa9b,0xdcf
};

//sand dunes theme
UWORD kolory2[COLORS] =
{
	0x101,0x312,0x423,0x533,0x634,0x754,0x865,0x975,
	0xa86,0xb97,
	0xca8,0xda9,0xeba,0xfcb,0x12f,0x23f
};

UWORD bitmapPalette[16];

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
static ULONG levelTime;
static UWORD lastOverwrittenLine;
static BYTE xOffsetEven, xOffsetOdd; //camera rotation offsett when turning

typedef struct tagBITMAPFILEHEADER
{
    UWORD bfType;  //specifies the file type
    ULONG  bfSize;  //specifies the size in bytes of the bitmap file
    UWORD bfReserved1;  //reserved; must be 0
    UWORD bfReserved2;  //reserved; must be 0
    ULONG  bfOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER;


typedef struct tagBITMAPINFOHEADER
{
    ULONG  biSize;  //specifies the number of bytes required by the struct
    ULONG  biWidth;  //specifies width in pixels
    ULONG  biHeight;  //species height in pixels
    UWORD biPlanes; //specifies the number of color planes, must be 1
    UWORD biBitCount; //specifies the number of bit per pixel
    ULONG  biCompression;//spcifies the type of compression
    ULONG  biSizeImage;  //size of image in bytes
    ULONG  biXPelsPerMeter;  //number of pixels per meter in x axis
    ULONG  biYPelsPerMeter;  //number of pixels per meter in y axis
    ULONG  biClrUsed;  //number of colors used by th ebitmap
    ULONG  biClrImportant;  //number of colors that are important
}BITMAPINFOHEADER;


BITMAPINFOHEADER bitmapHeader1,bitmapHeader2,bitmapHeader2;
unsigned char *bitmap1,*bitmap2,*bitmap3;
unsigned char bitmapPalette1[16*4], bitmapPalette2[16*4], bitmapPalette3[16*4];

void engineGsCreate(void);

void engineGsLoop(void);

void engineGsDestroy(void);




#endif // _ENGINE_H_
