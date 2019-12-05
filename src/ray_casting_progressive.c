#include "../src/engine.h"
 
UBYTE ProcessWord1v6(UBYTE rounds, UBYTE sx, UBYTE sy, UWORD *_tz, UWORD *tzz, UBYTE px, UBYTE py,UBYTE ph,
UWORD *address1, UWORD *address2, 
WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH], UWORD (*map)[256], UBYTE threshold)
{
	UWORD mapValue;
	WORD slope;
	UBYTE c = 99;
	UBYTE th;
	UBYTE tz;
	UBYTE iHor=0;

	sx = sx + 2;
	tz = tzz[iHor];//set current depth - tz
	while(tz < threshold)//check depth step by step
	{
		mapValue = map[ (UBYTE)( px + rayCastX[sx][tz] ) ][ (UBYTE)( py + (tz<<renderingDepthStep) ) ];//read color + height
		th = mapValue;//take just the height
		slope = th - (ph + rayCastY[sy][tz]);//check if read height is higher than what we expect from the raycast table
		if(slope > 0)
		{
			c = (mapValue >> 8);
			tzz[iHor] = tz;//save the depth we've arrived at
			break;
		}
		else
		{	
			tz++;//go step in depth if no height hit
		}
	}

	if(c == 99) c = skyColor - ph/32 - sy/8; //draw sky if too deep
	*address1 = (c<<10) + (c<<5) + (c); *address2 = *address1;
	
	
	return tz;
}
UBYTE ProcessWord2v6(UBYTE rounds, UBYTE sx, UBYTE sy, UWORD *_tz, UWORD *tzz, UBYTE px, UBYTE py,UBYTE ph,
UWORD *address1, UWORD *address2, 
WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH], UWORD (*map)[256], UBYTE threshold)
{
	UWORD mapValue;
	WORD slope;
	UBYTE c[2];
	UBYTE th;
	UBYTE tz;

	sx = sx + 1;
	c[0] = 99;
	c[1] = 99;
	//process 1,2,3 or 6 rounds to find colors for this WORD = 16 pixels
	for(UBYTE iHor=0;iHor<2;iHor++)
	{
		
		tz = tzz[iHor];//set current depth - tz
		while(tz < threshold)//check depth step by step
		{
			mapValue = map[ (UBYTE)( px + rayCastX[sx][tz] ) ][ (UBYTE)( py + (tz<<renderingDepthStep) ) ];//read color + height
			th = mapValue;//take just the height
			slope = th - (ph + rayCastY[sy][tz]);//check if read height is higher than what we expect from the raycast table
			if(slope > 0)
			{
				c[iHor] = (mapValue >> 8);
				tzz[iHor] = tz;//save the depth we've arrived at
				break;
			}
			else
			{	
				tz++;//go step in depth if no height hit
			}
		}
		
		sx += 3;
	}
	if(c[0] == 99) c[0] = skyColor - ph/32 - sy/8; //draw sky if too deep
	if(c[1] == 99) c[1] = skyColor - ph/32 - sy/8; //draw sky if too deep
	*address1 = (c[0]<<10) + (c[0]<<5) + (c[0]); *address2 = (c[1]<<10) + (c[1]<<5) + (c[1]);

	return tz;
}
UBYTE ProcessWord3v6(UBYTE rounds, UBYTE sx, UBYTE sy, UWORD *_tz, UWORD *tzz, UBYTE px, UBYTE py,UBYTE ph,
UWORD *address1, UWORD *address2, 
WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH], UWORD (*map)[256], UBYTE threshold)
{
	UWORD mapValue;
	WORD slope;
	UBYTE c[3];
	UBYTE th;
	UBYTE tz;

	c[0] = 99;
	c[1] = 99;
	c[2] = 99;

	//process 1,2,3 or 6 rounds to find colors for this WORD = 16 pixels
	for(UBYTE iHor=0;iHor<3;iHor++)
	{
		
		tz = tzz[iHor];//set current depth - tz
		while(tz < threshold)//check depth step by step
		{
			mapValue = map[ (UBYTE)( px + rayCastX[sx][tz] ) ][ (UBYTE)( py + (tz<<renderingDepthStep) ) ];//read color + height
			th = mapValue;//take just the height
			slope = th - (ph + rayCastY[sy][tz]);//check if read height is higher than what we expect from the raycast table
			if(slope > 0)
			{
				c[iHor] = (mapValue >> 8);
				tzz[iHor] = tz;//save the depth we've arrived at
				break;
			}
			else
			{	
				tz++;//go step in depth if no height hit
			}
		}
		sx +=2;
	}
	if(c[0] == 99) c[0] = skyColor - ph/32 - sy/8; //draw sky if too deep
	if(c[1] == 99) c[1] = skyColor - ph/32 - sy/8; //draw sky if too deep
	if(c[2] == 99) c[2] = skyColor - ph/32 - sy/8; //draw sky if too deep
	*address1 = (c[0]<<10) + (c[0]<<5) + (c[1]); *address2 = (c[1]<<10) + (c[2]<<5) + (c[2]);

	return tz;
}


