
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

void updateShipParams(LONG deltaTime, ULONG terrainHeight)
{
    ShipParams shipParams = engine.gameState.shipParams;
	shipParams.relHeight = ((shipParams.pY - 2) - terrainHeight);
	shipParams.ddP = (((200 - shipParams.relHeight) - (shipParams.dP>>3)) << 1);
	
	LONG addedVelocity = (shipParams.ddP>>3);
	if(addedVelocity > 1500)
	{
			addedVelocity = 1500 - shipParams.dP;	
		
	}
	shipParams.dP = shipParams.dP + addedVelocity;

	if(shipParams.dP > 1500)
	{
		shipParams.dP = 1500;
	}
	if(shipParams.dP < 0)
	{
		shipParams.dP = 0;
	}

	shipParams.precZ += deltaTime*shipParams.dP>>8;///shipParams.dPDenom;


	shipParams.precX += ((LONG)deltaTime * engine.gameState.crossHairX) >> 12;
	shipParams.precY -= ((LONG)deltaTime * engine.gameState.crossHairY) >> 10;

	if (shipParams.precY > 7000)
		shipParams.precY = 7000; //block going above hills
	if (shipParams.precY < 100)
		shipParams.precY = 100; //block going below ground

	shipParams.pZ = (shipParams.precZ  >> 5);
	shipParams.pX = (shipParams.precX  >> 5);
	shipParams.pY = (shipParams.precY  >> 5);
    engine.gameState.shipParams = shipParams;

}

void ProcessInput()
{
	
	LONG lowerDelta = engine.deltaTime>>11;
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


	engine.gameState.crossHairX += turnHorizontal*((LONG)engine.deltaTime >> 6);
	LONG crossHairHorizontalComeback = - ((engine.gameState.crossHairX / (lowerDelta)) << 3);
	
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

	
	engine.gameState.crossHairY += turnVertical*engine.yAxis*(LONG)(engine.deltaTime >> 6);
	LONG crossHairVerticalComeback = - ((engine.gameState.crossHairY / (lowerDelta)) << 3);
	
	if (crossHairVerticalComeback * turnVertical*engine.yAxis > 0 || turnVertical == 0)
	{
		engine.gameState.crossHairY = engine.gameState.crossHairY + crossHairVerticalComeback;
	}


}

UWORD getTerrainHeight(ShipParams shipParams, UWORD *map)
{
	ULONG length = engine.renderer.currentMapLength;
	if(engine.secondMapLength > 0)
	{
		length += MAPSIZE;
	}
	return ((UBYTE)map[(((UBYTE)(shipParams.pX)) >> 1)*length + (((UBYTE)(2*engine.renderer.zStart)) >> 1)]);
    //return ((UBYTE)(map[((UBYTE)(shipParams.pX)) >> 1][((UBYTE)(shipParams.pZ + engine.renderer.zStart + 8)) >> 1]));
}
