#define TERRAINDEPTH 64

#define XTURNBUFFOR 55

#define YSIZEODD 90
#define YSIZEEVEN YSIZEODD / 2

WORD rayCastXOdd[XSIZEODD][TERRAINDEPTH]; //51k
WORD rayCastYOdd[YSIZEODD][TERRAINDEPTH]; //51k

WORD rayCastXEven[XSIZEEVEN][TERRAINDEPTH]; //21k
WORD rayCastYEven[YSIZEEVEN][TERRAINDEPTH]; //21k

UBYTE renderingDepth = TERRAINDEPTH;