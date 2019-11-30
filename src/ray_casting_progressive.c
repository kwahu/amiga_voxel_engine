#include "../src/engine.h"

UBYTE ProcessWord1v6(UBYTE rounds, UBYTE sx, UBYTE sy, UWORD *_tz, UWORD *tzz, UBYTE px, UBYTE py,UBYTE ph,
UWORD *address1, UWORD *address2, 
WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH], UWORD (*map)[256])
{
	UWORD mapValue;
	WORD slope;
	UBYTE c = 99;
	UBYTE th;
	UBYTE tz;
	UBYTE iHor=0;

	sx = sx + 2;
	tz = tzz[iHor];//set current depth - tz
	while(tz < TERRAINDEPTH)//check depth step by step
	{
		mapValue = map[ (UBYTE)( px + rayCastX[sx][tz] ) ][ (UBYTE)( py + (tz<<debugValue4) ) ];//read color + height
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
WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH], UWORD (*map)[256])
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
		while(tz < TERRAINDEPTH)//check depth step by step
		{
			mapValue = map[ (UBYTE)( px + rayCastX[sx][tz] ) ][ (UBYTE)( py + (tz<<debugValue4) ) ];//read color + height
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
WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH], UWORD (*map)[256])
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
		while(tz < TERRAINDEPTH)//check depth step by step
		{
			mapValue = map[ (UBYTE)( px + rayCastX[sx][tz] ) ][ (UBYTE)( py + (tz<<debugValue4) ) ];//read color + height
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

void ProcessRayCastsProgressiveAtari(WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH], UWORD (*map)[256],
	UBYTE px, UBYTE py, UBYTE ph, 
	UBYTE tableXStart, UBYTE xCycles, UBYTE zStep, UBYTE zStart, 
	UBYTE ySize, BYTE xOffset)
{
	UBYTE sx,sy,mist;
	UWORD tz,tzz[6];
	UWORD position,address1,address2;
	UBYTE threshold1 = TERRAINDEPTH/2;
	UBYTE threshold2 = TERRAINDEPTH - TERRAINDEPTH/4;

	UWORD word;
	UBYTE color;
	UWORD positionStart =  ySize*20*4*4 + tableXStart/6*4;

	UBYTE verticalSteps;

	UBYTE iInit, iVert;

	UBYTE byte;

	//start with the buffor + vertical stripe start + turning amount
	sx = XTURNBUFFOR + tableXStart + xOffset;
	//currentScreenYStepSize = xCycles;


	//for each vertical line
	for(iVert=0;iVert<xCycles;iVert++)
	{
		//start from the bottom
		sy = 0;
		position = positionStart + iVert*4 ;//+ 80*12;
		
		//init values for this vertical line
		tzz[0]=zStart;tzz[1]=zStart;tzz[2]=zStart;
		//tzz[3]=zStart;tzz[4]=zStart;tzz[5]=zStart;
		tz = zStart;

		//process this vertical line
		while(sy < ySize)
		{
			 if(tz < threshold1)			
			 {
				tz = ProcessWord1v6(1,sx,sy,&tz,tzz,px,py,ph,&address1,&address2,rayCastX, rayCastY, map);
				byte = dither3x2EvenP1[ address1 ];
				word = (byte<<8) + byte;
				planes[position] = word;
				planes[position-160] = word;
				planes[position-320] = word;
				planes[position-480] = word;

				byte = dither3x2EvenP2[ address1 ];
				word = (byte<<8) + byte;
				planes[position+1] = word;
				planes[position-160+1] = word;
				planes[position-320+1] = word;
				planes[position-480+1] = word;

				byte = dither3x2EvenP3[ address1 ];
				word = (byte<<8) + byte;
				planes[position+2] = word;
				planes[position-160+2] = word;
				planes[position-320+2] = word;
				planes[position-480+2] = word;

				byte = dither3x2EvenP4[ address1 ];
				word = (byte<<8) + byte;
				planes[position+3] = word;
				planes[position-160+3] = word;
				planes[position-320+3] = word;
				planes[position-480+3] = word;
				position -= 640;

				sy+=2;
			 }
			else if(tz < threshold2)			
			 {
				tz = ProcessWord2v6(2,sx,sy,&tz,tzz,px,py,ph,&address1,&address2,rayCastX, rayCastY, map);

				word = (dither3x2EvenP1[ address1 ]<<8) + dither3x2EvenP1[ address2 ];
				planes[position] = word;
				planes[position-160] = word;
				planes[position-320] = word;
				planes[position-480] = word;
				word = (dither3x2EvenP2[ address1 ]<<8) + dither3x2EvenP2[ address2 ];
				planes[position+1] = word;
				planes[position-160+1] = word;
				planes[position-320+1] = word;
				planes[position-480+1] = word;
				word = (dither3x2EvenP3[ address1 ]<<8) + dither3x2EvenP3[ address2 ];
				planes[position+2] = word;
				planes[position-160+2] = word;
				planes[position-320+2] = word;
				planes[position-480+2] = word;
				word = (dither3x2EvenP4[ address1 ]<<8) + dither3x2EvenP4[ address2 ];
				planes[position+3] = word;
				planes[position-160+3] = word;
				planes[position-320+3] = word;
				planes[position-480+3] = word;
				position -= 640;

				sy+=2;
			 }
			else if(tz < TERRAINDEPTH)			
			 {
				tz = ProcessWord3v6(3,sx,sy,&tz,tzz,px,py,ph,&address1,&address2,rayCastX, rayCastY, map);

				word = (dither3x2EvenP1[ address1 ]<<8) + dither3x2EvenP1[ address2 ];
				planes[position] = word;
				planes[position-160] = word;
				planes[position-320] = word;
				planes[position-480] = word;
				word = (dither3x2EvenP2[ address1 ]<<8) + dither3x2EvenP2[ address2 ];
				planes[position+1] = word;
				planes[position-160+1] = word;
				planes[position-320+1] = word;
				planes[position-480+1] = word;
				word = (dither3x2EvenP3[ address1 ]<<8) + dither3x2EvenP3[ address2 ];
				planes[position+2] = word;
				planes[position-160+2] = word;
				planes[position-320+2] = word;
				planes[position-480+2] = word;
				word = (dither3x2EvenP4[ address1 ]<<8) + dither3x2EvenP4[ address2 ];
				planes[position+3] = word;
				planes[position-160+3] = word;
				planes[position-320+3] = word;
				planes[position-480+3] = word;
				position -= 640;

				sy+=2;
			 }
			 else 	
			 {
				color = skyColor - ph/32 -sy/8;
				address1 = (color<<10) + (color<<5) + (color);
	
				byte = dither3x2EvenP1[ address1 ];
				word = (byte<<8) + byte;
				planes[position] = word;
				planes[position-160] = word;
				planes[position-320] = word;
				planes[position-480] = word;
				byte = dither3x2EvenP2[ address1 ];
				word = (byte<<8) + byte;
				planes[position+1] = word;
				planes[position-160+1] = word;
				planes[position-320+1] = word;
				planes[position-480+1] = word;
				byte = dither3x2EvenP3[ address1 ];
				word = (byte<<8) + byte;
				planes[position+2] = word;
				planes[position-160+2] = word;
				planes[position-320+2] = word;
				planes[position-480+2] = word;
				byte = dither3x2EvenP4[ address1 ];
				word = (byte<<8) + byte;
				planes[position+3] = word;
				planes[position-160+3] = word;
				planes[position-320+3] = word;
				planes[position-480+3] = word;
				position -= 640;

				sy+=2;

			 }
			//go step higher in the raycast table
		}
		sx += 6;//go to the next vertical line
	}
}
void ProcessRayCastsProgressiveAmiga(WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH], UWORD (*map)[256],
	UBYTE px, UBYTE py, UBYTE ph, 
	UBYTE tableXStart, UBYTE xCycles, UBYTE zStep, UBYTE zStart, 
	UBYTE ySize, BYTE xOffset)
{
	UBYTE sx,sy,mist;
	UWORD tz,tzz[6];
	UWORD position,address1,address2;
	UBYTE threshold1 = TERRAINDEPTH/2;
	UBYTE threshold2 = TERRAINDEPTH - TERRAINDEPTH/4;

	UWORD word;
	UBYTE color;
	UWORD positionStart =  ySize*20*4*4 + tableXStart/6*4;

	UBYTE verticalSteps;

	UBYTE iInit, iVert;

	UBYTE byte;

	//start with the buffor + vertical stripe start + turning amount
	sx = XTURNBUFFOR + tableXStart + xOffset;
	//currentScreenYStepSize = xCycles;


	//for each vertical line
	for(iVert=0;iVert<xCycles;iVert++)
	{
		//start from the bottom
		sy = 0;
		position = positionStart + iVert*4 ;//+ 80*12;
		
		//init values for this vertical line
		tzz[0]=zStart;tzz[1]=zStart;tzz[2]=zStart;
		//tzz[3]=zStart;tzz[4]=zStart;tzz[5]=zStart;
		tz = zStart;

		//process this vertical line
		while(sy < ySize)
		{
			 if(tz < threshold1)			
			 {
				tz = ProcessWord1v6(1,sx,sy,&tz,tzz,px,py,ph,&address1,&address2,rayCastX, rayCastY, map);
				byte = dither3x2EvenP1[ address1 ];
				word = (byte<<8) + byte;
				planes[position] = word;
				planes[position-160] = word;
				planes[position-320] = word;
				planes[position-480] = word;

				byte = dither3x2EvenP2[ address1 ];
				word = (byte<<8) + byte;
				planes[position+1] = word;
				planes[position-160+1] = word;
				planes[position-320+1] = word;
				planes[position-480+1] = word;

				byte = dither3x2EvenP3[ address1 ];
				word = (byte<<8) + byte;
				planes[position+2] = word;
				planes[position-160+2] = word;
				planes[position-320+2] = word;
				planes[position-480+2] = word;

				byte = dither3x2EvenP4[ address1 ];
				word = (byte<<8) + byte;
				planes[position+3] = word;
				planes[position-160+3] = word;
				planes[position-320+3] = word;
				planes[position-480+3] = word;
				position -= 640;

				sy+=2;
			 }
			else if(tz < threshold2)			
			 {
				tz = ProcessWord2v6(2,sx,sy,&tz,tzz,px,py,ph,&address1,&address2,rayCastX, rayCastY, map);

				word = (dither3x2EvenP1[ address1 ]<<8) + dither3x2EvenP1[ address2 ];
				planes[position] = word;
				planes[position-160] = word;
				planes[position-320] = word;
				planes[position-480] = word;
				word = (dither3x2EvenP2[ address1 ]<<8) + dither3x2EvenP2[ address2 ];
				planes[position+1] = word;
				planes[position-160+1] = word;
				planes[position-320+1] = word;
				planes[position-480+1] = word;
				word = (dither3x2EvenP3[ address1 ]<<8) + dither3x2EvenP3[ address2 ];
				planes[position+2] = word;
				planes[position-160+2] = word;
				planes[position-320+2] = word;
				planes[position-480+2] = word;
				word = (dither3x2EvenP4[ address1 ]<<8) + dither3x2EvenP4[ address2 ];
				planes[position+3] = word;
				planes[position-160+3] = word;
				planes[position-320+3] = word;
				planes[position-480+3] = word;
				position -= 640;

				sy+=2;
			 }
			else if(tz < TERRAINDEPTH)			
			 {
				tz = ProcessWord3v6(3,sx,sy,&tz,tzz,px,py,ph,&address1,&address2,rayCastX, rayCastY, map);

				word = (dither3x2EvenP1[ address1 ]<<8) + dither3x2EvenP1[ address2 ];
				planes[position] = word;
				planes[position-160] = word;
				planes[position-320] = word;
				planes[position-480] = word;
				word = (dither3x2EvenP2[ address1 ]<<8) + dither3x2EvenP2[ address2 ];
				planes[position+1] = word;
				planes[position-160+1] = word;
				planes[position-320+1] = word;
				planes[position-480+1] = word;
				word = (dither3x2EvenP3[ address1 ]<<8) + dither3x2EvenP3[ address2 ];
				planes[position+2] = word;
				planes[position-160+2] = word;
				planes[position-320+2] = word;
				planes[position-480+2] = word;
				word = (dither3x2EvenP4[ address1 ]<<8) + dither3x2EvenP4[ address2 ];
				planes[position+3] = word;
				planes[position-160+3] = word;
				planes[position-320+3] = word;
				planes[position-480+3] = word;
				position -= 640;

				sy+=2;
			 }
			 else 	
			 {
				color = skyColor - ph/32 -sy/8;
				address1 = (color<<10) + (color<<5) + (color);
	
				byte = dither3x2EvenP1[ address1 ];
				word = (byte<<8) + byte;
				planes[position] = word;
				planes[position-160] = word;
				planes[position-320] = word;
				planes[position-480] = word;
				byte = dither3x2EvenP2[ address1 ];
				word = (byte<<8) + byte;
				planes[position+1] = word;
				planes[position-160+1] = word;
				planes[position-320+1] = word;
				planes[position-480+1] = word;
				byte = dither3x2EvenP3[ address1 ];
				word = (byte<<8) + byte;
				planes[position+2] = word;
				planes[position-160+2] = word;
				planes[position-320+2] = word;
				planes[position-480+2] = word;
				byte = dither3x2EvenP4[ address1 ];
				word = (byte<<8) + byte;
				planes[position+3] = word;
				planes[position-160+3] = word;
				planes[position-320+3] = word;
				planes[position-480+3] = word;
				position -= 640;

				sy+=2;

			 }
			//go step higher in the raycast table
		}
		sx += 6;//go to the next vertical line
	}
}