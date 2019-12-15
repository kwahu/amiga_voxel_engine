
#include <stdlib.h>
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

GameState updateShipParams(GameState gameState, LONG deltaTime, UWORD terrainHeight)
{
    ShipParams shipParams = gameState.shipParams;
	shipParams.relHeight = ((shipParams.pY - 3) - terrainHeight);
	shipParams.ddP = 5*((256 - shipParams.relHeight) - (shipParams.dP/4));
	
	LONG addedVelocity = 5*(shipParams.ddP/48);
	if(addedVelocity > 1000)
	{
		if(shipParams.ddP < 0)
		{
			shipParams.ddP = 5*((256 - shipParams.relHeight) - (shipParams.dP/4));
			addedVelocity = 5*(shipParams.ddP/48);
		}
		else
		{
			addedVelocity = 1000 - shipParams.dP;	
		}
		
		
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


	shipParams.precX += (LONG)deltaTime * gameState.crossHairX / 2000;
	shipParams.precY -= (LONG)deltaTime * gameState.crossHairY / 1000;

	if (shipParams.precY > 7000)
		shipParams.precY = 7000; //block going above hills
	if (shipParams.precY < 100)
		shipParams.precY = 100; //block going below ground

	shipParams.pZ = shipParams.precZ / 32;
	shipParams.pX = shipParams.precX / 32;
	shipParams.pY = shipParams.precY / 32;
    gameState.shipParams = shipParams;

    return gameState;
}

GameState ProcessInput(GameState gameState, LONG deltaTime)
{
	
	LONG lowerDelta = deltaTime/2000;
	if(lowerDelta == 0)
	{
		lowerDelta = 1;
	}

    if (getJoy(1, RIGHT))
	{
		gameState.crossHairX += deltaTime / 60;
	}
	else if (getJoy(1, LEFT))
	{
		gameState.crossHairX -= deltaTime / 60;
	}
	else if (gameState.crossHairX != 0)
	{
		gameState.crossHairX = gameState.crossHairX - gameState.crossHairX / (lowerDelta);
	}

	if (gameState.crossHairX > 0x4000)
		gameState.crossHairX = 0x4000;
	else if (gameState.crossHairX < -0x4000)
		gameState.crossHairX = -0x4000;

	if (gameState.crossHairY > 0x2000)
		gameState.crossHairY = 0x2000;
	else if (gameState.crossHairY < -0x2000)
		gameState.crossHairY = -0x2000;

	if (getJoy(1, DOWN))
	{
		gameState.crossHairY += deltaTime / 60;
		//gameState.crossHairY += deltaTime / 10;
	}
	else if (getJoy(1, UP))
	{
		gameState.crossHairY -= deltaTime / 60;
	}
	else if (gameState.crossHairY != 0)
	{
		gameState.crossHairY = gameState.crossHairY - gameState.crossHairY / (lowerDelta);
	}

    return gameState;

}

UWORD getTerrainHeight(ShipParams shipParams, UWORD map[][128])
{
    return ((UBYTE)(map[((UBYTE)(shipParams.pX)) >> 1][((UBYTE)(shipParams.pZ + 15)) >> 1]));
}
