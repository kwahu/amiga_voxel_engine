
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


	shipParams.precX += (LONG)deltaTime * engine.gameState.crossHairX / 4000;
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

	BYTE turnHorizontal = 0;

    if (getJoy(1, RIGHT))
	{
		turnHorizontal = 1;
	}
	else if (getJoy(1, LEFT))
	{
		turnHorizontal = -1;
	}	


	engine.gameState.crossHairX += turnHorizontal*(LONG)engine.deltaTime / 60;
	LONG crossHairHorizontalComeback = - 8*(engine.gameState.crossHairX / (lowerDelta));
	
	if (crossHairHorizontalComeback * turnHorizontal > 0 || turnHorizontal == 0)
	{
		engine.gameState.crossHairX = engine.gameState.crossHairX + crossHairHorizontalComeback;
	}

	if (engine.gameState.crossHairX > 0x4000)
		engine.gameState.crossHairX = 0x4000;
	else if (engine.gameState.crossHairX < -0x4000)
		engine.gameState.crossHairX = -0x4000;

	if (engine.gameState.crossHairY > 0x2000)
		engine.gameState.crossHairY = 0x2000;
	else if (engine.gameState.crossHairY < -0x2000)
		engine.gameState.crossHairY = -0x2000;

	BYTE turnVertical = 0;

    if (getJoy(1, DOWN))
	{
		turnVertical = 1;
	}
	else if (getJoy(1, UP))
	{
		turnVertical = -1;
	}	

	
	engine.gameState.crossHairY += turnVertical*engine.yAxis*(LONG)(engine.deltaTime / 60);
	LONG crossHairVerticalComeback = - 8*(engine.gameState.crossHairY / (lowerDelta));
	
	if (crossHairVerticalComeback * turnVertical*engine.yAxis > 0 || turnVertical == 0)
	{
		engine.gameState.crossHairY = engine.gameState.crossHairY + crossHairVerticalComeback;
	}


}

UWORD getTerrainHeight(ShipParams shipParams, UWORD *map)
{
	return ((UBYTE)map[(((UBYTE)(shipParams.pX)) >> 1)*11*MAPSIZE + (((UBYTE)(engine.renderer.zStart + 8)) >> 1)]);
    //return ((UBYTE)(map[((UBYTE)(shipParams.pX)) >> 1][((UBYTE)(shipParams.pZ + engine.renderer.zStart + 8)) >> 1]));
}
