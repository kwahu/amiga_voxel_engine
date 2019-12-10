#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <stdlib.h>
#include <stdint.h>
#include "settings.h"


#define DEPTH 4
#define COLORS 32

#define MAPSIZE 128
#define MAPLENGTH 11





static int interlace;
ULONG points = 0;

static UBYTE screenIndex;
static ULONG screenDuration;
static UBYTE infoScreen = 0;
static UBYTE endScreen = 0;

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

BITMAPINFOHEADER bitmapHeader1, bitmapHeader2, bitmapHeader3, bitmapHeader4, paletteHeader, shipHeader;
unsigned char *bitmap1, *paletteBitmap, *ship;
unsigned char bitmapPalette1[16 * 4], palettePalette[16 * 4];


UBYTE skyColor = 33;

void engineGsCreate(void);

void engineGsLoop(void);

void engineGsDestroy(void);

typedef struct ShipParams
{
    
    LONG precX, precY, precZ;
    UWORD pX, pY, pZ;
    LONG dP;
    LONG ddP;
    ULONG relHeight;
    ULONG dPDenom;
} ShipParams;

typedef struct MenuState
{
    UBYTE Dummy;
} MenuState;

typedef struct LogoState
{
    UBYTE Dummy;
} LogoState;

typedef struct CutsceneState
{
    UBYTE Dummy;
} CutsceneState;

typedef struct GameState
{
    WORD crossHairX, crossHairY;
    ShipParams shipParams;
} GameState;

typedef enum State
{
    State_Logo,
    State_Menu,
    State_Game,
    State_Cutscene,
    State_Count
} State;

typedef struct Renderer
{

    UBYTE heightMap[MAPSIZE][MAPSIZE]; //16k
    UBYTE colorMap[MAPSIZE][MAPSIZE];  //16k

    UWORD mapHigh[MAPSIZE][MAPSIZE]; //131k
    UWORD lastOverwrittenLine;

    UWORD mapSource[11][MAPSIZE][MAPSIZE];  //360k

    UBYTE mapLoaded0, mapLoaded1, mapLoaded2, mapLoaded3, mapLoaded4, mapLoaded5, mapLoaded6,
        mapLoaded7, mapLoaded8, mapLoaded9, mapLoaded10;

    UBYTE renderingType, calculationDepthDivider, calculationDepthStep, renderingDepthStep, stepModifier, xFOV, yFOV;

        
    UWORD bitmapPalette[16];

    UBYTE screen3x2[6 * 90]; //3k
    UBYTE screen4x4[4 * 45]; //3k

    UBYTE dither4x4EvenP1[COLORS * COLORS]; //1k
    UBYTE dither4x4EvenP2[COLORS * COLORS]; //1k
    UBYTE dither4x4EvenP3[COLORS * COLORS]; //1k
    UBYTE dither4x4EvenP4[COLORS * COLORS]; //1k

    UBYTE dither3x2EvenP1[COLORS * COLORS * COLORS]; //32k
    UBYTE dither3x2EvenP2[COLORS * COLORS * COLORS]; //32k
    UBYTE dither3x2EvenP3[COLORS * COLORS * COLORS]; //32k
    UBYTE dither3x2EvenP4[COLORS * COLORS * COLORS]; //32k
    
    WORD rayCastX[XSIZEODD][TERRAINDEPTH]; //51k
    WORD rayCastY[YSIZEODD][TERRAINDEPTH]; //51k
    
    BYTE xOffsetEven, xOffsetOdd; //camera rotation offsett when turning


} Renderer;

typedef struct Engine
{
    State CurrentState;
    union
    {
        LogoState logoState;
        MenuState menuState;
        GameState gameState;
        CutsceneState cutsceneState;

    };

    Renderer renderer;
    
    ULONG startTime, endTime, deltaTime, accTime, loopEndTime;

} Engine;

Engine engine;


// //sand dunes theme
// UWORD kolory[COLORS] =
//     {
//         0x101, 0x323, 0x523, 0x743, 0xa65, 0xc85, 0xeb8, 0xfdb,
//         0xbcc, 0x8be,
//         0x243, 0x9b8, 0xafd, 0x324, 0xa9b, 0xdcf};

// //sand dunes theme
// UWORD kolory2[COLORS] =
//     {
//         0x101, 0x312, 0x423, 0x533, 0x634, 0x754, 0x865, 0x975,
//         0xa86, 0xb97,
//         0xca8, 0xda9, 0xeba, 0xfcb, 0x12f, 0x23f};

// UWORD grayColors[16] = 
// {
// 	0x000,0x111,0x222,0x333,0x444,0x555,0x666,0x777,
// 	0x888,0x999,0xaaa,0xbbb,0xccc,0xddd,0xeee,0xfff
// };



#endif // _ENGINE_H_
