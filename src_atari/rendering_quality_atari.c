#include "../src/engine.h"

void RenderQuality()
{
    	
if(renderingType == 1)
{
	ProcessRayCastsProgressiveAtari(rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,50,   4,1,16,YSIZEODD, xOffsetOdd);
}
else if(renderingType == 2)
{
	ProcessRayCastsProgressiveAtari(rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,44,   6,1,16,YSIZEODD, xOffsetOdd);
}
else if(renderingType == 3)
{
	ProcessRayCastsProgressiveAtari(rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,40,   8,1,16,YSIZEODD, xOffsetOdd);
}
else if(renderingType == 4)
{
	ProcessRayCastsProgressiveAtari(rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,32,   10,1,16,YSIZEODD, xOffsetOdd);
}
else if(renderingType == 5)
{
	ProcessRayCastsProgressiveAtari(rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,24,   12,1,16,YSIZEODD, xOffsetOdd);
}
else if(renderingType == 6)
{
	ProcessRayCastsProgressiveAtari(rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,20,   14,1,16,YSIZEODD, xOffsetOdd);
}
else if(renderingType == 7)
{
	ProcessRayCasts3x2(rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,12,   16,1,12,YSIZEODD, xOffsetOdd);

}
else if(renderingType == 8)
{
	ProcessRayCasts3x2(rayCastXOdd, rayCastYOdd,mapHigh,p1x,p1y,p1h,0,   20,1,12,YSIZEODD, xOffsetOdd);
}
}