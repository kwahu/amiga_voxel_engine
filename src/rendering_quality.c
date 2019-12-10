#include "engine.h"

#ifdef AMIGA
#include "settings.h"

void RenderQuality()
{
			
	if(engine.renderer.renderingType == 1)
	{
		ProcessRayCastsFull4x4(rayCastXEven, rayCastYEven, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 4, 16, 12);
	}
	else if(engine.renderer.renderingType == 2)
	{


		ProcessRayCasts164x4(rayCastXEven, rayCastYEven, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 0, PLANEWIDTHWORD, 1);
		
	}
	else if(engine.renderer.renderingType == 3)
	{


		ProcessRayCasts164x4(rayCastXEven, rayCastYEven, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 0, 8, 1);
		ProcessRayCasts164x4(rayCastXOdd, rayCastYOdd, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 8, 12, 1);
		ProcessRayCasts164x4(rayCastXEven, rayCastYEven, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 12, PLANEWIDTHWORD, 1);
	
	}
	else if(engine.renderer.renderingType == 4)
	{



		ProcessRayCastsFull4x4(rayCastXEven, rayCastYEven, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 0, PLANEWIDTHWORD, 12);
		
	}
	else if(engine.renderer.renderingType == 5)
	{

		ProcessRayCastsFull4x4(rayCastXEven, rayCastYEven, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 0, 4, 12);
		ProcessRayCastsFull3x2(rayCastXOdd, rayCastYOdd, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 4, 16, 12);
		ProcessRayCastsFull4x4(rayCastXEven, rayCastYEven, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 16, 20, 12);
	
	}
	else if(engine.renderer.renderingType == 6)
	{

		ProcessRayCastsFull3x2(rayCastXOdd, rayCastYOdd, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 0, PLANEWIDTHWORD, 12);
	}
	else if(engine.renderer.renderingType == 7)
	{


		ProcessRayCastsFull3x2(rayCastXOdd, rayCastYOdd, engine.renderer.mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 0, PLANEWIDTHWORD, 8);
	}
	else if(engine.renderer.renderingType == 8)
	{
		ProcessRayCastsProgressive(rayCastXOdd, rayCastYOdd,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY, 0, PLANEWIDTHWORD, 1);
	}
}
#else

void RenderQuality()
{
            
    if(engine.renderer.renderingType == 1)
    {
        ProcessRayCastsProgressive(rayCastXOdd, rayCastYOdd,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,8,   12,1);
    }
    else if(engine.renderer.renderingType == 2)
    {
        ProcessRayCastsProgressive(rayCastXOdd, rayCastYOdd,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,7,   13,1);
    }
    else if(engine.renderer.renderingType == 3)
    {
        ProcessRayCastsProgressive(rayCastXOdd, rayCastYOdd,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,6, 14,1);
    }
    else if(engine.renderer.renderingType == 4)
    {
        ProcessRayCastsProgressive(rayCastXOdd, rayCastYOdd,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,5,   15,1);
    }
    else if(engine.renderer.renderingType == 5)
    {
        ProcessRayCastsProgressive(rayCastXOdd, rayCastYOdd,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,4,   16,1);
    }
    else if(engine.renderer.renderingType == 6)
    {
        ProcessRayCastsProgressive(rayCastXOdd, rayCastYOdd,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,3,   17,1);
    }
    else if(engine.renderer.renderingType == 7)
    {
        ProcessRayCastsProgressiveNonInterleaved(rayCastXOdd, rayCastYOdd,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,2,   18,1);

    }
    else if(engine.renderer.renderingType == 8)
    {
        ProcessRayCastsProgressiveNonInterleaved(rayCastXOdd, rayCastYOdd,engine.renderer.mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,0,   20,1);
    }
}
#endif