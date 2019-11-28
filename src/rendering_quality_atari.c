#include "engine.h"

void RenderQuality()
{
    	
if(debugValue == 1)
{
	ProcessRayCasts3x2(rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,56,   4,1,12,YSIZEODD, xOffsetOdd);
}
else if(debugValue == 2)
{
	ProcessRayCasts3x2(rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,48,   6,1,12,YSIZEODD, xOffsetOdd);
}
else if(debugValue == 3)
{
	ProcessRayCasts3x2(rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,40,   8,1,12,YSIZEODD, xOffsetOdd);
}
else if(debugValue == 4)
{
	ProcessRayCasts3x2(rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,32,   10,1,12,YSIZEODD, xOffsetOdd);
}
else if(debugValue == 5)
{
	ProcessRayCasts3x2(rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,24,   12,1,12,YSIZEODD, xOffsetOdd);
}
else if(debugValue == 6)
{
	ProcessRayCasts3x2(rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,20,   14,1,12,YSIZEODD, xOffsetOdd);
}
else if(debugValue == 7)
{
	ProcessRayCasts3x2(rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,12,   16,1,12,YSIZEODD, xOffsetOdd);

}
else if(debugValue == 8)
{
	ProcessRayCasts3x2(rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,0,   20,1,12,YSIZEODD, xOffsetOdd);
}
}