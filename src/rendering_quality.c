#include "engine.h"

#ifdef AMIGA

void RenderQuality()
{
			
	if(engine.renderer.renderingType == 1)
	{
		ProcessRayCastsFull4x4(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 4, 16);
	}
	else if(engine.renderer.renderingType == 2)
	{
		ProcessRayCastsFull4x4(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 2, 18);
	}
	else if(engine.renderer.renderingType == 3)
	{
		ProcessRayCastsFull4x4(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 0, 20);
	}
	else if(engine.renderer.renderingType == 4)
	{
		ProcessRayCastsFull3x2(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 4, 16);
	}
	else if(engine.renderer.renderingType == 5)
	{
		ProcessRayCastsFull3x2(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 2, 18);
	}
	else if(engine.renderer.renderingType == 6)
	{
		ProcessRayCastsFull3x2(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 0, 20);
	}
}
#else

void RenderQuality()
{
            
    if(engine.renderer.renderingType == 1)
    {
        ProcessRayCastsProgressive4x4NonInterleaved(engine.renderer.rayCastX, engine.renderer.rayCastY,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,4, 16);
    }
    else if(engine.renderer.renderingType == 2)
    {
        ProcessRayCastsProgressive4x4NonInterleaved(engine.renderer.rayCastX, engine.renderer.rayCastY,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,0,  20);
    }
    else if(engine.renderer.renderingType == 3)
    {
        ProcessRayCastsProgressiveNonInterleaved(engine.renderer.rayCastX, engine.renderer.rayCastY,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,4, 16);
    }
    else if(engine.renderer.renderingType == 4)
    {
        ProcessRayCastsProgressiveNonInterleaved(engine.renderer.rayCastX, engine.renderer.rayCastY,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,0,   20);
    }
}
#endif