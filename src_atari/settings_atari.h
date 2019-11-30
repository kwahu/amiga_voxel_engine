#pragma once

#define ULONG 	uint32_t
#define LONG	int32_t
#define UWORD 	uint16_t
#define WORD 	int16_t
#define UBYTE  unsigned char
#define BYTE	signed char

#define TERRAINDEPTH 32
#define PLANEHEIGHT 200
#define XSIZEODD 120 + XTURNBUFFOR * 2
#define XSIZEEVEN 80 + XTURNBUFFOR * 2

#define XTURNBUFFOR 55

#define YSIZEODD 40
#define YSIZEEVEN YSIZEODD / 2

WORD rayCastXOdd[XSIZEODD][TERRAINDEPTH]; //51k
WORD rayCastYOdd[YSIZEODD][TERRAINDEPTH]; //51k

//WORD rayCastXEven[XSIZEEVEN][TERRAINDEPTH]; //21k
//WORD rayCastYEven[YSIZEEVEN][TERRAINDEPTH]; //21k

UBYTE renderingDepth = TERRAINDEPTH;

//ATARI
uint16_t *planes;

