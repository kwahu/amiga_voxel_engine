#include "engine.h"
void OverwriteMap()
{
	UBYTE mapNumber;
	UWORD word;
	UBYTE yy;
	UBYTE yyplusone;

	for (UWORD x = 0; x < MAPSIZE; x++)
	{
		for (UWORD y = engine.renderer.lastOverwrittenLine; y < engine.gameState.shipParams.pZ; y++)
		{
			yy = (UBYTE)y ;
			yyplusone = (yy+1);
			mapNumber = (y / 256 + 1) % MAPLENGTH;
			word = engine.renderer.mapSource[mapNumber][x][yy/2];
			engine.renderer.mapHigh[x][yy/2] = word;
		}
	}
	engine.renderer.lastOverwrittenLine = engine.gameState.shipParams.pZ;
}