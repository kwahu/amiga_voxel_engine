#pragma once

#ifdef AMIGA

#include <ace/managers/viewport/simplebuffer.h>

#define TERRAINDEPTH 64
#define PLANEHEIGHT 256
#define XTURNBUFFOR 55
#define PLANEWIDTH 40
#define PLANEWIDTHWORD 20


#define XSIZEODD 120 + XTURNBUFFOR * 2
#define XSIZEEVEN 80 + XTURNBUFFOR * 2

#define YSIZEODD 90
#define YSIZEEVEN YSIZEODD / 2


UBYTE renderingDepth = TERRAINDEPTH;

//AMIGA
UWORD plane1W[PLANEWIDTHWORD * PLANEHEIGHT]; //8k
UWORD plane2W[PLANEWIDTHWORD * PLANEHEIGHT]; //8k
UWORD plane3W[PLANEWIDTHWORD * PLANEHEIGHT]; //8k
UWORD plane4W[PLANEWIDTHWORD * PLANEHEIGHT]; //8k

static tView *s_pView;
static tVPort *s_pVPort;
static tSimpleBufferManager *s_pBuffer;

#else

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
#define XSIZEODD 120 + XTURNBUFFOR * 2
#define XSIZEEVEN 80 + XTURNBUFFOR * 2

#define XTURNBUFFOR 55

#define YSIZEODD 40
#define YSIZEEVEN YSIZEODD / 2


UBYTE renderingDepth = TERRAINDEPTH;

//ATARI
uint16_t *planes;
#endif