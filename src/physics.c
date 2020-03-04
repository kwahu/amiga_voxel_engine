
#include "engine.h"

void ResetTime()
{
	engine.accTime = 0;
	engine.deltaTime = 0;
	engine.endTime = getCurrentTime();
	engine.startTime = getCurrentTime();
}

void TimeStep()
{
	engine.startTime = getCurrentTime();
	engine.deltaTime = engine.startTime - engine.endTime;
	engine.endTime = engine.startTime;
	engine.accTime += engine.deltaTime;
}

ULONG getDeltaTime(ULONG *refTime)
{
    ULONG newTime = getCurrentTime();
    ULONG result = newTime - *refTime;
    *refTime = newTime;
    return result;
}

void updateShipParams(LONG deltaTime, UWORD terrainHeight)
{
    ShipParams shipParams = engine.gameState.shipParams;
	shipParams.relHeight = ((shipParams.pY - 2) - terrainHeight);
	shipParams.ddP = 5*((256 - shipParams.relHeight) - (shipParams.dP/4));
	
	LONG addedVelocity = 5*(shipParams.ddP/48);
	if(addedVelocity > 1000)
	{
			addedVelocity = 1000 - shipParams.dP;	
		
	}
	shipParams.dP = shipParams.dP + addedVelocity;

	if(shipParams.dP > 1000)
	{
		shipParams.dP = 1000;
	}
	if(shipParams.dP < 0)
	{
		shipParams.dP = 0;
	}

	shipParams.precZ += deltaTime*shipParams.dP/shipParams.dPDenom;


	shipParams.precX += (LONG)deltaTime * engine.gameState.crossHairX / 3000;
	shipParams.precY -= (LONG)deltaTime * engine.gameState.crossHairY / 1500;

	if (shipParams.precY > 7000)
		shipParams.precY = 7000; //block going above hills
	if (shipParams.precY < 100)
		shipParams.precY = 100; //block going below ground

	shipParams.pZ = shipParams.precZ / 32;
	shipParams.pX = shipParams.precX / 32;
	shipParams.pY = shipParams.precY / 32;
    engine.gameState.shipParams = shipParams;

}

void ProcessInput()
{
	
	LONG lowerDelta = engine.deltaTime/2000;
	if(lowerDelta == 0)
	{
		lowerDelta = 1;
	}

    if (getJoy(1, RIGHT))
	{
		engine.gameState.crossHairX += engine.deltaTime / 60;
	}
	else if (getJoy(1, LEFT))
	{
		engine.gameState.crossHairX -= engine.deltaTime / 60;
	}
	else if (engine.gameState.crossHairX != 0)
	{
		engine.gameState.crossHairX = engine.gameState.crossHairX - engine.gameState.crossHairX / (lowerDelta);
	}

	if (engine.gameState.crossHairX > 0x4000)
		engine.gameState.crossHairX = 0x4000;
	else if (engine.gameState.crossHairX < -0x4000)
		engine.gameState.crossHairX = -0x4000;

	if (engine.gameState.crossHairY > 0x2000)
		engine.gameState.crossHairY = 0x2000;
	else if (engine.gameState.crossHairY < -0x2000)
		engine.gameState.crossHairY = -0x2000;

	if (getJoy(1, DOWN))
	{
		engine.gameState.crossHairY += engine.yAxis*(LONG)(engine.deltaTime / 60);
		//engine.gameState.crossHairY += deltaTime / 10;
	}
	else if (getJoy(1, UP))
	{
		engine.gameState.crossHairY -= engine.yAxis*(LONG)(engine.deltaTime / 60);
	}
	else if (engine.gameState.crossHairY != 0)
	{
		engine.gameState.crossHairY = engine.gameState.crossHairY - engine.gameState.crossHairY / (lowerDelta);
	}


}

UWORD getTerrainHeight(ShipParams shipParams, UWORD map[][128])
{
    return ((UBYTE)(map[((UBYTE)(shipParams.pX)) >> 1][((UBYTE)(shipParams.pZ + engine.renderer.zStart + 8)) >> 1]));
}
