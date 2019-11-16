#include "engine.h"

void RenderQuality()
{
    	
if(debugValue == 1)
{


	//ProcessRayCastsMist(screen8x8a,rayCastXEven, rayCastYEven,mapLow,p1x,p1y,p1h,	0,  2, 2, 0, 8,4,1,YSIZEEVEN, xOffsetEven);
	ProcessRayCastsMist(screen4x4b,rayCastXEven, rayCastYEven,mapMed,p1x,p1y,p1h,	16, 2, 2, 0, 16,4,2,YSIZEEVEN, xOffsetEven);
	ProcessRayCastsMist(screen4x4c,rayCastXEven, rayCastYEven,mapMed,p1x,p1y,p1h,	48, 2, 2, 0, 16,4,3,YSIZEEVEN, xOffsetEven);
	ProcessRayCastsMist(screen4x4d,rayCastXEven, rayCastYEven,mapMed,p1x,p1y,p1h,	80, 2, 2, 0, 16,4,2,YSIZEEVEN, xOffsetEven);
	//ProcessRayCastsMist(screen8x8e,rayCastXEven, rayCastYEven,mapLow,p1x,p1y,p1h,	64, 2, 2, 0, 8,4,1,YSIZEEVEN, xOffsetEven);

	//DrawPlayerScreen4x4(screen4x4a,1,0,0,4);
	DrawPlayerScreen4x4(screen4x4b,1,0,4,4);
	DrawPlayerScreen4x4(screen4x4c,1,0,8,4);
	DrawPlayerScreen4x4(screen4x4d,1,0,12,4);
	//DrawPlayerScreen4x4(screen4x4e,1,0,16,4);
}
else if(debugValue == 2)
{

	ProcessRayCastsMist(screen4x4a,rayCastXEven, rayCastYEven,mapMed,p1x,p1y,p1h,	0,  1, 1, 0, 16,4,1,YSIZEEVEN, xOffsetEven);
	ProcessRayCastsMist(screen4x4b,rayCastXEven, rayCastYEven,mapMed,p1x,p1y,p1h,	16, 1, 1, 0, 16,4,2,YSIZEEVEN, xOffsetEven);
	ProcessRayCastsMist(screen4x4c,rayCastXEven, rayCastYEven,mapMed,p1x,p1y,p1h,	32, 1, 1, 0, 16,4,3,YSIZEEVEN, xOffsetEven);
	ProcessRayCastsMist(screen4x4d,rayCastXEven, rayCastYEven,mapMed,p1x,p1y,p1h,	48, 1, 1, 0, 16,4,2,YSIZEEVEN, xOffsetEven);
	ProcessRayCastsMist(screen4x4e,rayCastXEven, rayCastYEven,mapMed,p1x,p1y,p1h,	64, 1, 1, 0, 16,4,1,YSIZEEVEN, xOffsetEven);

	DrawPlayerScreen4x4(screen4x4a,1,0,0,4);
	DrawPlayerScreen4x4(screen4x4b,1,0,4,4);
	DrawPlayerScreen4x4(screen4x4c,1,0,8,4);
	DrawPlayerScreen4x4(screen4x4d,1,0,12,4);
	DrawPlayerScreen4x4(screen4x4e,1,0,16,4);
}
else if(debugValue == 3)
{


	ProcessRayCastsMist(screen8x8a,rayCastXEven, rayCastYEven,mapLow,p1x,p1y,p1h,	0,  2, 2, 0, 8,4,1,YSIZEEVEN, xOffsetEven);
	ProcessRayCastsMist(screen4x4b,rayCastXEven, rayCastYEven,mapMed,p1x,p1y,p1h,	16, 1, 1, 0, 16,4,2,YSIZEEVEN, xOffsetEven);
	ProcessRayCastsMist(screen3x2c,rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,	48, 1, 1, 0, 24,4,3,YSIZEODD, xOffsetOdd);
	ProcessRayCastsMist(screen4x4d,rayCastXEven, rayCastYEven,mapMed,p1x,p1y,p1h,	48, 1, 1, 0, 16,4,2,YSIZEEVEN, xOffsetEven);
	ProcessRayCastsMist(screen8x8e,rayCastXEven, rayCastYEven,mapLow,p1x,p1y,p1h,	64, 2, 2, 0, 8,4,1,YSIZEEVEN, xOffsetEven);

	DrawPlayerScreen8x8(screen8x8a,1,0,0,4);
	DrawPlayerScreen4x4(screen4x4b,1,0,4,4);
	DrawPlayerScreen3x2(screen3x2c,1,0,8,4);
	DrawPlayerScreen4x4(screen4x4d,1,0,12,4);
	DrawPlayerScreen8x8(screen8x8e,1,0,16,4);
}
else if(debugValue == 4)
{


	ProcessRayCastsMist(screen4x4a,rayCastXEven, rayCastYEven,mapMed,p1x,p1y,p1h,	0,  1, 1, 0, 16,2,3,YSIZEEVEN, xOffsetEven);
	ProcessRayCastsMist(screen4x4b,rayCastXEven, rayCastYEven,mapMed,p1x,p1y,p1h,	16, 1, 1, 0, 16,2,4,YSIZEEVEN, xOffsetEven);
	ProcessRayCastsMist(screen3x2c,rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,	48, 1, 1, 0, 24,2,5,YSIZEODD, xOffsetOdd);
	ProcessRayCastsMist(screen4x4d,rayCastXEven, rayCastYEven,mapMed,p1x,p1y,p1h,	48, 1, 1, 0, 16,2,4,YSIZEEVEN, xOffsetEven);
	ProcessRayCastsMist(screen4x4e,rayCastXEven, rayCastYEven,mapMed,p1x,p1y,p1h,	64, 1, 1, 0, 16,2,3,YSIZEEVEN, xOffsetEven);

	DrawPlayerScreen4x4(screen4x4a,1,0,0,4);
	DrawPlayerScreen4x4(screen4x4b,1,0,4,4);
	DrawPlayerScreen3x2(screen3x2c,1,0,8,4);
	DrawPlayerScreen4x4(screen4x4d,1,0,12,4);
	DrawPlayerScreen4x4(screen4x4e,1,0,16,4);
}
else if(debugValue == 5)
{


	ProcessRayCastsMist(screen4x4a,rayCastXEven, rayCastYEven,mapMed,p1x,p1y,p1h,	0,  1, 1, 0, 16,2,3,YSIZEEVEN, xOffsetEven);
	ProcessRayCastsMist(screen3x2b,rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,	24, 1, 1, 0, 24,2,4,YSIZEODD, xOffsetOdd);
	ProcessRayCastsMist(screen3x2c,rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,	48, 1, 1, 0, 24,2,5,YSIZEODD, xOffsetOdd);
	ProcessRayCastsMist(screen3x2d,rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,	72, 1, 1, 0, 24,2,4,YSIZEODD, xOffsetOdd);
	ProcessRayCastsMist(screen4x4e,rayCastXEven, rayCastYEven,mapMed,p1x,p1y,p1h,	64, 1, 1, 0, 16,2,3,YSIZEEVEN, xOffsetEven);

	DrawPlayerScreen4x4(screen4x4a,1,0,0,4);
	DrawPlayerScreen3x2(screen3x2b,1,0,4,4);
	DrawPlayerScreen3x2(screen3x2c,1,0,8,4);
	DrawPlayerScreen3x2(screen3x2d,1,0,12,4);
	DrawPlayerScreen4x4(screen4x4e,1,0,16,4);
}
else if(debugValue == 6)
{


	ProcessRayCastsMist(screen3x2a,rayCastXOdd, rayCastYOdd,mapLow,p1x,p1y,p1h,	0,  1, 1, 0, 24,2,3,YSIZEODD, xOffsetOdd);
	ProcessRayCastsMist(screen3x2b,rayCastXOdd, rayCastYOdd,mapMed,p1x,p1y,p1h,	24, 1, 1, 0, 24,2,4,YSIZEODD, xOffsetOdd);
	ProcessRayCastsMist(screen3x2c,rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,48, 1, 1, 0, 24,2,5,YSIZEODD, xOffsetOdd);
	ProcessRayCastsMist(screen3x2d,rayCastXOdd, rayCastYOdd,mapMed,p1x,p1y,p1h,	72, 1, 1, 0, 24,2,4,YSIZEODD, xOffsetOdd);
	ProcessRayCastsMist(screen3x2e,rayCastXOdd, rayCastYOdd,mapLow,p1x,p1y,p1h,	96, 1, 1, 0, 24,2,3,YSIZEODD, xOffsetOdd);

	DrawPlayerScreen3x2(screen3x2a,1,0,0,4);
	DrawPlayerScreen3x2(screen3x2b,1,0,4,4);
	DrawPlayerScreen3x2(screen3x2c,1,0,8,4);
	DrawPlayerScreen3x2(screen3x2d,1,0,12,4);
	DrawPlayerScreen3x2(screen3x2e,1,0,16,4);
}
else if(debugValue == 7)
{


	ProcessRayCastsMist(screen3x2a,rayCastXOdd, rayCastYOdd,mapLow,p1x,p1y,p1h,	0,  1, 1, 0, 24,1,8,YSIZEODD, xOffsetOdd);
	ProcessRayCastsMist(screen3x2b,rayCastXOdd, rayCastYOdd,mapMed,p1x,p1y,p1h,	24, 1, 1, 0, 24,1,9,YSIZEODD, xOffsetOdd);
	ProcessRayCastsMist(screen3x2c,rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,48, 1, 1, 0, 24,1,10,YSIZEODD, xOffsetOdd);
	ProcessRayCastsMist(screen3x2d,rayCastXOdd, rayCastYOdd,mapMed,p1x,p1y,p1h,	72, 1, 1, 0, 24,1,9,YSIZEODD, xOffsetOdd);
	ProcessRayCastsMist(screen3x2e,rayCastXOdd, rayCastYOdd,mapLow,p1x,p1y,p1h,	96, 1, 1, 0, 24,1,8,YSIZEODD, xOffsetOdd);

	DrawPlayerScreen3x2(screen3x2a,1,0,0,4);
	DrawPlayerScreen3x2(screen3x2b,1,0,4,4);
	DrawPlayerScreen3x2(screen3x2c,1,0,8,4);
	DrawPlayerScreen3x2(screen3x2d,1,0,12,4);
	DrawPlayerScreen3x2(screen3x2e,1,0,16,4);
}
}