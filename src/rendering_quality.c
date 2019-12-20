#include "engine.h"

#ifdef AMIGA

void RenderQuality()
{
			
	if(engine.renderer.renderingType == 1)
	{
		ProcessRayCastsFull4x4(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 4, 16, 12);

	}
	else if(engine.renderer.renderingType == 2)
	{

		ProcessRayCastsFull4x4(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 2, 18, 12);

		//ProcessRayCasts164x4(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 0, 20, 1);
		
	}
	else if(engine.renderer.renderingType == 3)
	{


		//ProcessRayCasts164x4(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 0, 8, 1);
		ProcessRayCastsFull4x4(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 0, 20, 12);
		//ProcessRayCasts163x2(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 8, 12, 1);
		//ProcessRayCasts164x4(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 12, PLANEWIDTHWORD, 1);
	
	}
	else if(engine.renderer.renderingType == 4)
	{



		ProcessRayCastsFull3x2(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 4, 16, 12);
		
	}
	else if(engine.renderer.renderingType == 5)
	{

		//ProcessRayCastsFull4x4(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 0, 4, 12);
		ProcessRayCastsFull3x2(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 2, 18, 12);
		//ProcessRayCastsFull4x4(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 16, 20, 12);
	
	}
	else if(engine.renderer.renderingType == 6)
	{

		ProcessRayCastsFull3x2(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 0, 20, 12);
	}
	else if(engine.renderer.renderingType == 7)
	{


		ProcessRayCastsFull3x2(engine.renderer.rayCastX, engine.renderer.rayCastY, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 0, 20, 8);
	}
	else if(engine.renderer.renderingType == 8)
	{
		ProcessRayCastsProgressive(engine.renderer.rayCastX, engine.renderer.rayCastY,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 0, 20, 12);
	}
}
#else

void RenderQuality()
{
            
    if(engine.renderer.renderingType == 1)
    {
        ProcessRayCastsProgressive(engine.renderer.rayCastX, engine.renderer.rayCastY,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,8,   12,1);
    }
    else if(engine.renderer.renderingType == 2)
    {
        ProcessRayCastsProgressive(engine.renderer.rayCastX, engine.renderer.rayCastY,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,7,   13,1);
    }
    else if(engine.renderer.renderingType == 3)
    {
        ProcessRayCastsProgressive(engine.renderer.rayCastX, engine.renderer.rayCastY,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,2, 18,1);
    }
    else if(engine.renderer.renderingType == 4)
    {
        ProcessRayCastsProgressiveNonInterleaved(engine.renderer.rayCastX, engine.renderer.rayCastY,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,0,   20,1);
        //ProcessRayCastsProgressive(engine.renderer.rayCastX, engine.renderer.rayCastY,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,5,   15,1);
    }
    else if(engine.renderer.renderingType == 5)
    {
        ProcessRayCastsProgressive(engine.renderer.rayCastX, engine.renderer.rayCastY,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,4,   16,1);
    }
    else if(engine.renderer.renderingType == 6)
    {
        ProcessRayCastsProgressive(engine.renderer.rayCastX, engine.renderer.rayCastY,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,3,   17,1);
    }
    else if(engine.renderer.renderingType == 7)
    {
        ProcessRayCastsProgressiveNonInterleaved(engine.renderer.rayCastX, engine.renderer.rayCastY,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,2,   18,1);

    }
    else if(engine.renderer.renderingType == 8)
    {
        ProcessRayCastsProgressiveNonInterleaved(engine.renderer.rayCastX, engine.renderer.rayCastY,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,0,   20,1);
    }
}
#endif