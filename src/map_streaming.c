#include "engine.h"
void OverwriteMap()
{
	UBYTE mapNumber;
	UWORD word;
	UBYTE yy;
	UBYTE yyplusone;

	for (UWORD x = 0; x < MAPSIZE; x++)
	{
		for (UWORD y = lastOverwrittenLine; y < engine.gameState.shipParams.pZ; y++)
		{
			yy = (UBYTE)y ;
			yyplusone = (yy+1);
			mapNumber = (y / 256 + 1) % MAPLENGTH;
			word = mapSource[mapNumber][x][yy/2];
			mapHigh[x*2][yy] = word;
			mapHigh[x*2+1][yy] = word;
			mapHigh[x*2][yyplusone] = word;
			mapHigh[x*2+1][yyplusone] = word;
		}
	}
	lastOverwrittenLine = engine.gameState.shipParams.pZ;
}