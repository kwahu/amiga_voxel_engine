#include "engine.h"
void OverwriteMap()
{
	UBYTE mapNumber;
	UWORD word;
	UWORD yy;

	for (UWORD y = lastOverwrittenLine; y < p1y; y++)
	{
		yy = y % 256;
		mapNumber = (p1y / 256 + 256) % 11;
		for (UWORD x = 0; x < MAPSIZE; x++)
		{
			word = mapSource[mapNumber][x][yy/2];
			mapHigh[x*2][yy] = word;
			mapHigh[x*2+1][yy] = word;
			mapHigh[x*2][yy+1] = word;
			mapHigh[x*2+1][yy+1] = word;
		}
	}
	lastOverwrittenLine = p1y;
}