#include "engine.h"
void OverwriteMap()
{
	UBYTE mapNumber;
	UWORD word;
	UBYTE yy;
	UBYTE yyplusone;

	for (UWORD y = lastOverwrittenLine; y < p1y; y++)
	{
		yy = (UBYTE)y ;
		yyplusone = (yy+1);
		mapNumber = (y / 256 + 1) % MAPLENGTH;
		for (UWORD x = 0; x < MAPSIZE; x++)
		{
			word = mapSource[mapNumber][x][yy/2];
			mapHigh[x*2][yy] = word;
			mapHigh[x*2+1][yy] = word;
			mapHigh[x*2][yyplusone] = word;
			mapHigh[x*2+1][yyplusone] = word;
		}
	}
	lastOverwrittenLine = p1y;
}