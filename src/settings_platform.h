#pragma once

#ifdef AMIGA

#include <ace/managers/viewport/simplebuffer.h>

#define TERRAINDEPTH 64
#define PLANEHEIGHT 256
#define XTURNBUFFOR 55


#define XSIZEODD (120 + XTURNBUFFOR * 2)
#define XSIZEEVEN (80 + XTURNBUFFOR * 2)

#define YSIZEODD 90
#define YSIZEEVEN (YSIZEODD / 2)




#else

#define ULONG 	uint32_t
#define LONG	int32_t
#define UWORD 	uint16_t
#define WORD 	int16_t
#define UBYTE  unsigned char
#define BYTE	signed char

#define TERRAINDEPTH 32
#define PLANEHEIGHT 200
#define XTURNBUFFOR 55
#define XSIZEODD (120 + XTURNBUFFOR * 2)
#define XSIZEEVEN (80 + XTURNBUFFOR * 2)


#define YSIZEODD 40
#define YSIZEEVEN (YSIZEODD / 2)


#endif


typedef struct PlatformScreen
{
	#ifdef AMIGA
	tView *s_pView;
	tVPort *s_pVPort;
	tSimpleBufferManager *s_pBuffer;
	#else
    uint16_t *physBase;
    uint16_t *logBase;
	uint16_t *planesAtari;
	uint16_t systemPalette[16];
	#endif

} PlatformScreen;

#ifdef AMIGA
#include <ace/utils/font.h>
typedef tFont Font;
typedef tTextBitMap TextBitmap; 

#else

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
 

#endif