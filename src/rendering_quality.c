#include "engine.h"

#ifdef AMIGA
#include "settings.h"

void RenderQuality()
{
			
	if(renderingType == 1)
	{
		//ProcessRayCastsFull(screen4x4a,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	0,  1, 1, 0, 16,2,12,YSIZEEVEN, xOffsetEven);
		ProcessRayCastsFull(screen4x4b,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	16, 1, 1, 0, 16,2,1,YSIZEEVEN, xOffsetEven);
		ProcessRayCastsFull(screen4x4c,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	32, 1, 1, 0, 16,2,1,YSIZEEVEN, xOffsetEven);
		ProcessRayCastsFull(screen4x4d,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	48, 1, 1, 0, 16,2,1,YSIZEEVEN, xOffsetEven);
		//ProcessRayCastsFull(screen4x4e,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	64, 1, 1, 0, 16,2,12,YSIZEEVEN, xOffsetEven);

		//DrawPlayerScreen4x4(screen4x4a,1,0,0,4);
		DrawPlayerScreen4x4(screen4x4b,1,0,4,4);
		DrawPlayerScreen4x4(screen4x4c,1,0,8,4);
		DrawPlayerScreen4x4(screen4x4d,1,0,12,4);
		// DrawPlayerScreen4x4(screen4x4e,1,0,16,4);
	}
	else if(renderingType == 2)
	{

		ProcessRayCasts16(screen4x4a,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	0,  1, 1, 0, 16,4,1,YSIZEEVEN, xOffsetEven);
		ProcessRayCasts16(screen4x4b,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	16, 1, 1, 0, 16,4,1,YSIZEEVEN, xOffsetEven);
		ProcessRayCasts16(screen4x4c,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	32, 1, 1, 0, 16,4,1,YSIZEEVEN, xOffsetEven);
		ProcessRayCasts16(screen4x4d,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	48, 1, 1, 0, 16,4,1,YSIZEEVEN, xOffsetEven);
		ProcessRayCasts16(screen4x4e,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	64, 1, 1, 0, 16,4,1,YSIZEEVEN, xOffsetEven);

		DrawPlayerScreen4x4(screen4x4a,1,0,0,4);
		DrawPlayerScreen4x4(screen4x4b,1,0,4,4);
		DrawPlayerScreen4x4(screen4x4c,1,0,8,4);
		DrawPlayerScreen4x4(screen4x4d,1,0,12,4);
		DrawPlayerScreen4x4(screen4x4e,1,0,16,4);
	}
	else if(renderingType == 3)
	{


		ProcessRayCasts16(screen4x4a,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	0,  1, 1, 0, 16,4,1,YSIZEEVEN, xOffsetEven);
		ProcessRayCasts16(screen4x4b,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	16, 1, 1, 0, 16,4,1,YSIZEEVEN, xOffsetEven);
		ProcessRayCasts16(screen3x2c,rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,		48, 1, 1, 0, 24,4,1,YSIZEODD, xOffsetOdd);
		ProcessRayCasts16(screen4x4d,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	48, 1, 1, 0, 16,4,1,YSIZEEVEN, xOffsetEven);
		ProcessRayCasts16(screen4x4e,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	64, 1, 1, 0, 16,4,1,YSIZEEVEN, xOffsetEven);

		DrawPlayerScreen4x4(screen4x4a,1,0,0,4);
		DrawPlayerScreen4x4(screen4x4b,1,0,4,4);
		DrawPlayerScreen3x2(screen3x2c,1,0,8,4);
		DrawPlayerScreen4x4(screen4x4d,1,0,12,4);
		DrawPlayerScreen4x4(screen4x4e,1,0,16,4);
	}
	else if(renderingType == 4)
	{


		ProcessRayCastsFull(screen4x4a,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	0,  1, 1, 0, 16,2,12,YSIZEEVEN, xOffsetEven);
		ProcessRayCastsFull(screen4x4b,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	16, 1, 1, 0, 16,2,12,YSIZEEVEN, xOffsetEven);
		ProcessRayCastsFull(screen4x4c,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	32, 1, 1, 0, 16,2,12,YSIZEEVEN, xOffsetEven);
		ProcessRayCastsFull(screen4x4d,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	48, 1, 1, 0, 16,2,12,YSIZEEVEN, xOffsetEven);
		ProcessRayCastsFull(screen4x4e,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	64, 1, 1, 0, 16,2,12,YSIZEEVEN, xOffsetEven);

		DrawPlayerScreen4x4(screen4x4a,1,0,0,4);
		DrawPlayerScreen4x4(screen4x4b,1,0,4,4);
		DrawPlayerScreen4x4(screen4x4c,1,0,8,4);
		DrawPlayerScreen4x4(screen4x4d,1,0,12,4);
		DrawPlayerScreen4x4(screen4x4e,1,0,16,4);

		//ProcessRayCastsFull4x4(rayCastXEven, rayCastYEven, mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY);
		
	}
	else if(renderingType == 5)
	{


		ProcessRayCastsFull(screen4x4a,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	0,  1, 1, 0, 16,2,12,YSIZEEVEN, xOffsetEven);
		ProcessRayCastsFull(screen3x2b,rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	24, 1, 1, 0, 24,2,12,YSIZEODD, xOffsetOdd);
		ProcessRayCastsFull(screen3x2c,rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	48, 1, 1, 0, 24,2,12,YSIZEODD, xOffsetOdd);
		ProcessRayCastsFull(screen3x2d,rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	72, 1, 1, 0, 24,2,12,YSIZEODD, xOffsetOdd);
		ProcessRayCastsFull(screen4x4e,rayCastXEven, rayCastYEven,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	64, 1, 1, 0, 16,2,12,YSIZEEVEN, xOffsetEven);

		DrawPlayerScreen4x4(screen4x4a,1,0,0,4); 
		DrawPlayerScreen3x2(screen3x2b,1,0,4,4);
		DrawPlayerScreen3x2(screen3x2c,1,0,8,4);
		DrawPlayerScreen3x2(screen3x2d,1,0,12,4);
		DrawPlayerScreen4x4(screen4x4e,1,0,16,4);
	}
	else if(renderingType == 6)
	{
		// ProcessRayCastsFull(screen3x2a,rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	0,  1, 1, 0, 24,2,12,YSIZEODD, xOffsetOdd);
		// ProcessRayCastsFull(screen3x2b,rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	24, 1, 1, 0, 24,2,12,YSIZEODD, xOffsetOdd);
		// ProcessRayCastsFull(screen3x2c,rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,48, 1, 1, 0, 24,2,12,YSIZEODD, xOffsetOdd);
		// ProcessRayCastsFull(screen3x2d,rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	72, 1, 1, 0, 24,2,12,YSIZEODD, xOffsetOdd);
		// ProcessRayCastsFull(screen3x2e,rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,	96, 1, 1, 0, 24,2,12,YSIZEODD, xOffsetOdd);

		// DrawPlayerScreen3x2(screen3x2a,1,0,0,4);
		// DrawPlayerScreen3x2(screen3x2b,1,0,4,4);
		// DrawPlayerScreen3x2(screen3x2c,1,0,8,4);
		// DrawPlayerScreen3x2(screen3x2d,1,0,12,4);
		// DrawPlayerScreen3x2(screen3x2e,1,0,16,4);

		ProcessRayCastsFull3x2(rayCastXOdd, rayCastYOdd, mapHigh, engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY);
	}
	else if(renderingType == 7)
	{


		ProcessRayCastsFull(screen3x2a,rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,0,  1, 1, 0, 24,1,8,YSIZEODD, xOffsetOdd);
		ProcessRayCastsFull(screen3x2b,rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,24, 1, 1, 0, 24,1,8,YSIZEODD, xOffsetOdd);
		ProcessRayCastsFull(screen3x2c,rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,48, 1, 1, 0, 24,1,8,YSIZEODD, xOffsetOdd);
		ProcessRayCastsFull(screen3x2d,rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,72, 1, 1, 0, 24,1,8,YSIZEODD, xOffsetOdd);
		ProcessRayCastsFull(screen3x2e,rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,96, 1, 1, 0, 24,1,8,YSIZEODD, xOffsetOdd);

		DrawPlayerScreen3x2(screen3x2a,1,0,0,4);
		DrawPlayerScreen3x2(screen3x2b,1,0,4,4);
		DrawPlayerScreen3x2(screen3x2c,1,0,8,4);
		DrawPlayerScreen3x2(screen3x2d,1,0,12,4);
		DrawPlayerScreen3x2(screen3x2e,1,0,16,4);
	}
	else if(renderingType == 8)
	{
		ProcessRayCastsProgressiveAmiga(rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,0,   20,1,1,YSIZEODD, xOffsetOdd);
	}
}
#elif ATARI

void RenderQuality()
{
            
    if(renderingType == 1)
    {
        ProcessRayCastsProgressiveAtari(rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,50,   4,1,16,YSIZEODD, xOffsetOdd);
    }
    else if(renderingType == 2)
    {
        ProcessRayCastsProgressiveAtari(rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,44,   6,1,16,YSIZEODD, xOffsetOdd);
    }
    else if(renderingType == 3)
    {
        ProcessRayCastsProgressiveAtari(rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,40,   8,1,16,YSIZEODD, xOffsetOdd);
    }
    else if(renderingType == 4)
    {
        ProcessRayCastsProgressiveAtari(rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,32,   10,1,16,YSIZEODD, xOffsetOdd);
    }
    else if(renderingType == 5)
    {
        ProcessRayCastsProgressiveAtari(rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,24,   12,1,16,YSIZEODD, xOffsetOdd);
    }
    else if(renderingType == 6)
    {
        ProcessRayCastsProgressiveAtari(rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,20,   14,1,16,YSIZEODD, xOffsetOdd);
    }
    else if(renderingType == 7)
    {
        ProcessRayCasts3x2(rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,12,   16,1,12,YSIZEODD, xOffsetOdd);

    }
    else if(renderingType == 8)
    {
        ProcessRayCasts3x2(rayCastXOdd, rayCastYOdd,mapHigh,engine.gameState.shipParams.pX,engine.gameState.shipParams.pZ,engine.gameState.shipParams.pY,0,   20,1,12,YSIZEODD, xOffsetOdd);
    }
}
#endif