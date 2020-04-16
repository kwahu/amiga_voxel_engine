#pragma once

#ifdef AMIGA


#define PLANEWIDTH 40
#define PLANEWIDTHWORD 20
#define TERRAINDEPTH 64
#define PLANEHEIGHT 256
#define XTURNBUFFOR 25
#define GAME_SCREEN_BASELINE 218
#define GAME_SHIP_POS 135


#define XSIZEODD (120 + XTURNBUFFOR * 2)
#define XSIZEEVEN (80 + XTURNBUFFOR * 2)

#define YSIZEODD 90
#define YSIZEEVEN (YSIZEODD / 2)


struct VPort;
struct ScreenView;
struct SimpleBufferManager;

typedef struct PlatformScreen
{
	struct ScreenView *view;
	struct VPort *vPort;
	struct BufferManager *buffer;
} PlatformScreen;



#else

#include "key_map_atari.h"
#include <osbind.h>
#include <mint/sysbind.h>

#define ULONG 	uint32_t
#define LONG	int32_t
#define UWORD 	uint16_t
#define WORD 	int16_t
#define UBYTE  unsigned char
#define BYTE	signed char

#define PLANEWIDTH 160
#define PLANEWIDTHWORD 80
#define TERRAINDEPTH 32
#define PLANEHEIGHT 200
#define XTURNBUFFOR 55
#define XSIZEODD (120 + XTURNBUFFOR * 2)
#define XSIZEEVEN (80 + XTURNBUFFOR * 2)
#define GAME_SCREEN_BASELINE 164
#define GAME_SHIP_POS 82


#define YSIZEODD 40
#define YSIZEEVEN (YSIZEODD / 2)


typedef struct PlatformScreen
{
    uint16_t *physBase;
    uint16_t *logBase;
	uint16_t *planesAtari;
	uint16_t systemPalette[16];

} PlatformScreen;

#endif


typedef struct Font {
	UWORD Width;       ///< Packed font bitmap width.
	UWORD Height;      ///< Packed font bitmap height.
	UBYTE Chars;       ///< Glyph count in font.
	UWORD *CharOffsets; ///< Glyph offsets in packed bitmap.
	UWORD *RawData;   ///< Pointer to packed bitmap.
} Font;


typedef struct TextBitMap {
	UWORD *BitMap;    ///< Word-aligned bitmap buffer with pre-drawn text.
	UWORD ActualWidth; ///< Actual text width for precise blitting.
	UWORD ActualHeight; ///< Actual text height for precise blitting.
	UWORD Width; ///< Actual text width for precise blitting.
	UWORD Height; ///< Actual text height for precise blitting.
} TextBitMap;
 
