#pragma once

#include "engine.h"
#include <ace/managers/viewport/simplebuffer.h>

#define TERRAINDEPTH 64
#define PLANEHEIGHT 256
#define XTURNBUFFOR 55


#define XSIZEODD 120 + XTURNBUFFOR * 2
#define XSIZEEVEN 80 + XTURNBUFFOR * 2

#define YSIZEODD 90
#define YSIZEEVEN YSIZEODD / 2

WORD rayCastXOdd[XSIZEODD][TERRAINDEPTH]; //26k
WORD rayCastYOdd[YSIZEODD][TERRAINDEPTH]; //11k

WORD rayCastXEven[XSIZEEVEN][TERRAINDEPTH]; //24k
WORD rayCastYEven[YSIZEEVEN][TERRAINDEPTH]; //6k

UBYTE renderingDepth = TERRAINDEPTH;

//AMIGA
UWORD plane1W[PLANEWIDTHWORD * PLANEHEIGHT]; //8k
UWORD plane2W[PLANEWIDTHWORD * PLANEHEIGHT]; //8k
UWORD plane3W[PLANEWIDTHWORD * PLANEHEIGHT]; //8k
UWORD plane4W[PLANEWIDTHWORD * PLANEHEIGHT]; //8k

static tView *s_pView;
static tVPort *s_pVPort;
static tSimpleBufferManager *s_pBuffer;