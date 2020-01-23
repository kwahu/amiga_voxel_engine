#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "settings_platform.h"
#include "sound_platform.h"

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
    UBYTE highMemory;

        
    UWORD bitmapPalette[16];

    UBYTE *screenPatch;
    UBYTE *ditherTable1;
    UBYTE *ditherTable2;
    UBYTE *ditherTable3;
    UBYTE *ditherTable4;

    WORD *rayCastX;
    WORD *rayCastY;


    //WORD rayCastX[XSIZEODD][TERRAINDEPTH]; //29k AMIGA / 14k ATARI
    //WORD rayCastY[YSIZEODD][TERRAINDEPTH]; //12k AMIGA / 3k ATARI
    
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
    TextBitMap *pBitmapHeightLabel, *pBitmapHeight, *pBitmapTime, *pBitmapTimeLabel, *informationText;
    TextBitMap *pBitmapVelocityLabel, *pBitmapVelocity, *pBitmapScore, *pBitmapScoreLabel, *pBitmapInfo[10];

    UBYTE musicOn;
    UBYTE *music;

    UBYTE exitFlag;

} Engine;

Engine engine;



#include "font_platform.h"
#include "input_platform.h"
#include "screen_platform.h"

#endif // _ENGINE_H_
