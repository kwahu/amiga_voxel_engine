#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <stdlib.h>


#define PLANEWIDTH 40
#define PLANEWIDTHWORD 20
#define DEPTH 4
#define COLORS 32

#define MAPSIZE 128
#define MAPLENGTH 11

UBYTE heightMap[MAPSIZE][MAPSIZE]; //16k
UBYTE colorMap[MAPSIZE][MAPSIZE];  //16k

UWORD mapHigh[256][256]; //131k

UWORD mapSource[11][MAPSIZE][MAPSIZE];  //180k

UBYTE mapLoaded0, mapLoaded1, mapLoaded2, mapLoaded3, mapLoaded4, mapLoaded5, mapLoaded6,
    mapLoaded7, mapLoaded8, mapLoaded9, mapLoaded10;

UBYTE debugValue, debugValue2, debugValue3, debugValue4, debugValue5, debugValue6, xFOV;
BYTE debugValue8, debugValue9;

//sand dunes theme
UWORD kolory[COLORS] =
    {
        0x101, 0x323, 0x523, 0x743, 0xa65, 0xc85, 0xeb8, 0xfdb,
        0xbcc, 0x8be,
        0x243, 0x9b8, 0xafd, 0x324, 0xa9b, 0xdcf};

//sand dunes theme
UWORD kolory2[COLORS] =
    {
        0x101, 0x312, 0x423, 0x533, 0x634, 0x754, 0x865, 0x975,
        0xa86, 0xb97,
        0xca8, 0xda9, 0xeba, 0xfcb, 0x12f, 0x23f};

UWORD grayColors[16] =
{
	0x000,0x111,0x222,0x333,0x444,0x555,0x666,0x777,
	0x888,0x999,0xaaa,0xbbb,0xccc,0xddd,0xeee,0xfff
};

UWORD bitmapPalette[16];

UBYTE screen4x4a[16 * 64]; //1k
UBYTE screen4x4b[16 * 64]; //1k
UBYTE screen4x4c[16 * 64]; //1k
UBYTE screen4x4d[16 * 64]; //1k
UBYTE screen4x4e[16 * 64]; //1k

UBYTE screen3x2a[24 * 128]; //3k
UBYTE screen3x2b[24 * 128]; //3k
UBYTE screen3x2c[24 * 128]; //3k
UBYTE screen3x2d[24 * 128]; //3k
UBYTE screen3x2e[24 * 128]; //3k

UBYTE dither4x4EvenP1[COLORS * COLORS]; //1k
UBYTE dither4x4EvenP2[COLORS * COLORS]; //1k
UBYTE dither4x4EvenP3[COLORS * COLORS]; //1k
UBYTE dither4x4EvenP4[COLORS * COLORS]; //1k

UBYTE dither3x2EvenP1[COLORS * COLORS * COLORS]; //32k
UBYTE dither3x2EvenP2[COLORS * COLORS * COLORS]; //32k
UBYTE dither3x2EvenP3[COLORS * COLORS * COLORS]; //32k
UBYTE dither3x2EvenP4[COLORS * COLORS * COLORS]; //32k

LONG p1xf, p1yf, p1hf, p2xf, p2yf, p2hf;
UWORD p1x, p1y, p1h, p2x, p2y, p2h;
static int interlace;
ULONG velocity = 0;
LONG acceleration = 0;

static UBYTE screenIndex;
static WORD cx, cy;
static ULONG startTime, endTime, deltaTime, lastTime;
static ULONG levelTime;
static ULONG screenDuration;
UWORD lastOverwrittenLine;
static BYTE xOffsetEven, xOffsetOdd; //camera rotation offsett when turning

typedef struct tagBITMAPFILEHEADER
{
    UWORD bfType;      //specifies the file type
    ULONG bfSize;      //specifies the size in bytes of the bitmap file
    UWORD bfReserved1; //reserved; must be 0
    UWORD bfReserved2; //reserved; must be 0
    ULONG bfOffBits;   //species the offset in bytes from the bitmapfileheader to the bitmap bits
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    ULONG biSize;          //specifies the number of bytes required by the struct
    ULONG biWidth;         //specifies width in pixels
    ULONG biHeight;        //species height in pixels
    UWORD biPlanes;        //specifies the number of color planes, must be 1
    UWORD biBitCount;      //specifies the number of bit per pixel
    ULONG biCompression;   //spcifies the type of compression
    ULONG biSizeImage;     //size of image in bytes
    ULONG biXPelsPerMeter; //number of pixels per meter in x axis
    ULONG biYPelsPerMeter; //number of pixels per meter in y axis
    ULONG biClrUsed;       //number of colors used by th ebitmap
    ULONG biClrImportant;  //number of colors that are important
} BITMAPINFOHEADER;

BITMAPINFOHEADER bitmapHeader1, bitmapHeader2, bitmapHeader3, paletteHeader;
unsigned char *bitmap1, *bitmap2, *bitmap3, *paletteBitmap;
unsigned char bitmapPalette1[16 * 4], bitmapPalette2[16 * 4], bitmapPalette3[16 * 4],palettePalette[16 * 4];

void engineGsCreate(void);

void engineGsLoop(void);

void engineGsDestroy(void);

#endif // _ENGINE_H_
