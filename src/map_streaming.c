#include "engine.h"
void OverwriteMap()
{
	UWORD word;
	UBYTE yy;



	for (UWORD x = 0; x < MAPSIZE; x++)
	{
		for (UWORD y = engine.renderer.lastOverwrittenLine; y < engine.gameState.shipParams.pZ; y++)
		{
			yy = (UBYTE)y ;
			word = engine.renderer.mapSource[x*11*MAPSIZE+ yy/2];
			engine.renderer.mapHigh[x][yy/2] = word;
		}
	}
	engine.renderer.lastOverwrittenLine = engine.gameState.shipParams.pZ;
}