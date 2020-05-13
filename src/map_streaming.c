#include "engine.h"
void OverwriteMap()
{
	UWORD word;
	UBYTE yy;

	UWORD currentZ = engine.gameState.shipParams.pZ;
	if(currentZ > engine.renderer.currentMapLength && engine.secondMapLength > 0)
	{
		engine.renderer.mapLengthRemainder += engine.renderer.currentMapLength;
		engine.renderer.currentMap = engine.renderer.secondMap;
		engine.gameState.shipParams.pZ -= engine.renderer.currentMapLength;
		engine.renderer.currentMapLength = engine.secondMapLength;
	}

	engine.renderer.mapHigh = engine.renderer.currentMap + ((engine.gameState.shipParams.pZ) >> 1);


	// for (UWORD x = 0; x < MAPSIZE; x++)
	// {
	// 	for (UWORD y = engine.renderer.lastOverwrittenLine; y < engine.gameState.shipParams.pZ; y++)
	// 	{
	// 		yy = (UBYTE)y ;
	// 		word = engine.renderer.mapSource[x*11*MAPSIZE+ yy/2];
	// 		engine.renderer.mapHigh[x][yy/2] = word;
	// 	}
	// }
	//engine.renderer.lastOverwrittenLine = engine.gameState.shipParams.pZ;
}