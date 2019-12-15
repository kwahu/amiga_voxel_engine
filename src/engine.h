#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "settings_platform.h"

#define ULONG 	uint32_t
#define LONG	int32_t
#define UWORD 	uint16_t
#define WORD 	int16_t
#define UBYTE  unsigned char
#define BYTE	signed char

#define DEPTH 4
#define COLORS 32

#define MAPSIZE 128
#define MAPLENGTH 11

#define PLANEWIDTH 160
#define PLANEWIDTHWORD 80

#define LOGORUNTIME 7500000
#define SKY_COLOR 33

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
    UBYTE infoScreen;
} MenuState;

typedef struct LogoState
{
    
    UBYTE screenIndex;
    ULONG screenDuration;
    char fadeInStatus[4], fadeOutStatus[4];
} LogoState;


typedef struct GameState
{
    WORD crossHairX, crossHairY;
    ShipParams shipParams;
    ULONG points;
    BYTE runOver;
    char sPlayerY[5];
    char sTime[8], sVelocity[5], sScore[8];
} GameState;

typedef enum State
{
    State_Logo,
    State_Menu,
    State_Game,
    State_Cutscene,
    State_Count
} State;

typedef enum Cutscene
{
    Cutscene_Death,
    Cutscene_TooLate,
    Cutscene_Win,
} Cutscene;

typedef struct Renderer
{
    #ifdef AMIGA
    UWORD planes[PLANEWIDTHWORD*PLANEHEIGHT];
    #else
    uint16_t *planes;
    #endif
    UBYTE heightMap[MAPSIZE][MAPSIZE]; //16k
    UBYTE colorMap[MAPSIZE][MAPSIZE];  //16k

    UWORD mapHigh[MAPSIZE][MAPSIZE]; //32k
    UWORD lastOverwrittenLine;

    UWORD mapSource[11][MAPSIZE][MAPSIZE];  //360k

    UBYTE mapLoaded0, mapLoaded1, mapLoaded2, mapLoaded3, mapLoaded4, mapLoaded5, mapLoaded6,
        mapLoaded7, mapLoaded8, mapLoaded9, mapLoaded10;

    UBYTE renderingType, calculationDepthDivider, calculationDepthStep, renderingDepthStep, stepModifier, xFOV, yFOV;
    UBYTE renderingDepth;

        
    UWORD bitmapPalette[16];

    UBYTE screen3x2[6 * 90]; //540B
    UBYTE screen4x4[4 * 45]; //180B

    UBYTE dither4x4EvenP1[COLORS * COLORS]; //1k
    UBYTE dither4x4EvenP2[COLORS * COLORS]; //1k
    UBYTE dither4x4EvenP3[COLORS * COLORS]; //1k
    UBYTE dither4x4EvenP4[COLORS * COLORS]; //1k

    UBYTE dither3x2EvenP1[COLORS * COLORS * COLORS]; //32k
    UBYTE dither3x2EvenP2[COLORS * COLORS * COLORS]; //32k
    UBYTE dither3x2EvenP3[COLORS * COLORS * COLORS]; //32k
    UBYTE dither3x2EvenP4[COLORS * COLORS * COLORS]; //32k
    
    WORD rayCastX[XSIZEODD][TERRAINDEPTH]; //29k AMIGA / 14k ATARI
    WORD rayCastY[YSIZEODD][TERRAINDEPTH]; //12k AMIGA / 3k ATARI
    
    BYTE xOffsetEven, xOffsetOdd; //camera rotation offsett when turning


} Renderer;

typedef struct Engine
{
    State currentState;
    State newState;
    union
    {
        LogoState logoState;
        MenuState menuState;
        GameState gameState;

    };

    PlatformScreen platformScreen;

    Renderer renderer;
    
    ULONG startTime, endTime, deltaTime, accTime, loopEndTime;
    
    UBYTE activePalette[16 * 4], palettePalette[16 * 4];
    BITMAPINFOHEADER activeBitmapHeader, paletteHeader, shipHeader;
    UBYTE *activeBitmap, *paletteBitmap, *shipBitmap;

    Font *font;
    TextBitmap *pBitmapHeightLabel, *pBitmapHeight, *pBitmapTime, *pBitmapTimeLabel, *informationText;
    TextBitmap *pBitmapVelocityLabel, *pBitmapVelocity, *pBitmapScore, *pBitmapScoreLabel, *pBitmapInfo[10];

    UBYTE exitFlag;

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


#include "font_platform.h"
#include "input_platform.h"
#include "screen_platform.h"

#endif // _ENGINE_H_
