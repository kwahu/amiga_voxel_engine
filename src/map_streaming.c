#include "engine.h"
void OverwriteMap()
{
	UWORD word;
	UBYTE yy;

	UWORD currentZ = (engine.gameState.shipParams.pZ >> 1);
	if(currentZ > engine.renderer.currentMapLength && engine.secondMapLength > 0 && engine.renderer.mapLengthRemainder == 0)
	{
		engine.renderer.mapLengthRemainder = (engine.renderer.currentMapLength << 1);
		engine.renderer.currentMap = engine.renderer.secondMap;
		engine.gameState.shipParams.precZ = (engine.gameState.shipParams.precZ & 0x1FFF);
		engine.gameState.shipParams.pZ = (engine.gameState.shipParams.pZ & 0xFF);
		engine.renderer.currentMapLength = engine.secondMapLength;
	}

	engine.renderer.mapHigh = engine.renderer.currentMap + ((engine.gameState.shipParams.pZ) >> 1);


}