#include "engine.h"

void RecalculateEven()
{
	CalculateRayCasts(engine.renderer.rayCastX, engine.renderer.rayCastY, XSIZEEVEN, YSIZEEVEN, 2); //było 2
	engine.deltaTime = 0;
}
void RecalculateOdd()
{
	CalculateRayCasts(engine.renderer.rayCastX, engine.renderer.rayCastY, XSIZEODD, YSIZEODD, 1);
	engine.deltaTime = 0;
}

void CalculateRayCasts(WORD (*rayCastX), WORD (*rayCastY), 
                        UBYTE xSize, UBYTE ySize, int stepSize)
{
	WORD sxx;
	WORD syy;
	WORD tzz; //depth step value
	WORD fovX = xSize/engine.renderer.xFOV; //width FOV
	WORD fovY = ySize/engine.renderer.yFOV;//5; //height FOV
	WORD sxxx;
	WORD xMiddle = xSize/2;
	WORD yMiddle = ySize/2;


	tzz = 1;
	for(int tz=1;tz<TERRAINDEPTH;tz++)
	{
        WORD offsetTZ = (20*tz)/TERRAINDEPTH;
		DrawPixelWord((offsetTZ), PLANEHEIGHT-8,(offsetTZ));
		DrawPixelWord((offsetTZ), PLANEHEIGHT-7,(offsetTZ));
		DrawPixelWord((offsetTZ), PLANEHEIGHT-6,(offsetTZ));
		DrawPixelWord((offsetTZ), PLANEHEIGHT-5,(offsetTZ));
		DrawPixelWord((offsetTZ), PLANEHEIGHT-4,(offsetTZ));
		DrawPixelWord((offsetTZ), PLANEHEIGHT-3,(offsetTZ));
		DrawPixelWord((offsetTZ), PLANEHEIGHT-2,(offsetTZ));
		DrawPixelWord((offsetTZ), PLANEHEIGHT-1,(offsetTZ));
		
        DrawPanelsToScreen();
		
		//high - 2 - 8
		tzz += engine.renderer.calculationDepthStep + tz / engine.renderer.calculationDepthDivider; //+tz/16; //increase step with the distance from camera
		WORD *rayXPtr = rayCastX + tz;
        for(int sx=-xMiddle;sx<xMiddle;sx++)
		{
			sxx = (sx * tzz)/engine.renderer.stepModifier; //make smaller steps
			sxxx = sxx/fovX;
            *rayXPtr = sxxx; 
            rayXPtr += TERRAINDEPTH;
            WORD *rayYPtr = rayCastY + tz;
			for(int sy=-yMiddle;sy<yMiddle;sy++)
			{
				syy = (sy*stepSize * tzz)/engine.renderer.stepModifier;//make smaller steps
				*rayYPtr = syy/fovY;
                rayYPtr += TERRAINDEPTH;
			}
		}
	}

}

UBYTE ProcessWord1v4(UBYTE rounds, UBYTE sx, UBYTE sy, UWORD *_tz, UWORD *tzz, UBYTE px, UBYTE py,UBYTE ph,
UWORD *address1, UWORD *address2, 
WORD (*rayCastX), WORD (*rayCastY), UWORD (*map)[MAPSIZE], UBYTE threshold)
{
	UWORD mapValue;
	WORD slope;
	UBYTE c = 99;
	UBYTE th;
	UBYTE tz;
	UBYTE iHor=0;

	sx = sx + 2;
	tz = tzz[iHor];//set current depth - tz
    UWORD *rayXPtr = rayCastX + sx*TERRAINDEPTH + tz;
    UWORD *rayYPtr = rayCastY + sy*TERRAINDEPTH + tz;






	while(tz < threshold)//check depth step by step
	{
		mapValue = map[ ((UBYTE)( px + *rayXPtr))  >> 1 ][ ((UBYTE)( py + (tz<<engine.renderer.renderingDepthStep) ))  >> 1 ];//read color + height
		th = mapValue;//take just the height
		slope = th - (ph + *rayYPtr);//check if read height is higher than what we expect from the raycast table
        if(slope > tz>>2)
        {
            c = ((mapValue >> 8) + ((slope/4) & 1));
            tzz[iHor] = tz;//save the depth we've arrived at
			break;
        }
		if(slope > 0)
		{
			c = (mapValue >> 8) + 2;
			tzz[iHor] = tz;//save the depth we've arrived at
			break;
		}
		else
		{	
            rayXPtr++;
            rayYPtr++;
                ++tz;
			//go step in depth if no height hit
		}
	}

	if(c == 99) c = SKY_COLOR - ph/32 - sy/8; //draw sky if too deep
	*address1 = (c<<5) + (c); *address2 = *address1;
	
	
	return tz;
}
UBYTE ProcessWord2v4(UBYTE rounds, UBYTE sx, UBYTE sy, UWORD *_tz, UWORD *tzz, UBYTE px, UBYTE py,UBYTE ph,
UWORD *address1, UWORD *address2, 
WORD (*rayCastX), WORD (*rayCastY), UWORD (*map)[MAPSIZE], UBYTE threshold)
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
        UWORD *rayXPtr = rayCastX + sx*TERRAINDEPTH + tz;
        UWORD *rayYPtr = rayCastY + sy*TERRAINDEPTH + tz;
		while(tz < threshold)//check depth step by step
		{
			mapValue = map[ ((UBYTE)( px + *rayXPtr))  >> 1 ][ ((UBYTE)( py + (tz<<engine.renderer.renderingDepthStep) )) >> 1 ];//read color + height
			th = mapValue;//take just the height
			slope = th - (ph + *rayYPtr);//check if read height is higher than what we expect from the raycast table
			if(slope > tz>>2)
            {
                c[iHor] = ((mapValue >> 8) + ((slope/4) & 1));
                tzz[iHor] = tz;//save the depth we've arrived at
                break;
            }
            if(slope > 0)
			{
				c[iHor] = (mapValue >> 8) + 2;
				tzz[iHor] = tz;//save the depth we've arrived at
				break;
			}
			else
			{	
                rayXPtr++;
                rayYPtr++;
                ++tz;
				//go step in depth if no height hit
			}
		}
		
		sx += 2;
	}
	if(c[0] == 99) c[0] = SKY_COLOR - ph/32 - sy/8; //draw sky if too deep
	if(c[1] == 99) c[1] = SKY_COLOR - ph/32 - sy/8; //draw sky if too deep
	*address1 = (c[0]<<5) + (c[0]); *address2 = (c[1]<<5) + (c[1]);

	return tz;
}

UBYTE ProcessWord1v6(UBYTE rounds, UBYTE sx, UBYTE sy, UWORD *_tz, UWORD *tzz, UBYTE px, UBYTE py,UBYTE ph,
UWORD *address1, UWORD *address2, 
WORD (*rayCastX), WORD (*rayCastY), UWORD (*map)[MAPSIZE], UBYTE threshold)
{
	UWORD mapValue;
	WORD slope;
	UBYTE c = 99;
	UBYTE th;
	UBYTE tz;
	UBYTE iHor=0;

	sx = sx + 2;
	tz = tzz[iHor];//set current depth - tz
    UWORD *rayXPtr = rayCastX + sx*TERRAINDEPTH + tz;
    UWORD *rayYPtr = rayCastY + sy*TERRAINDEPTH + tz;
	while(tz < threshold)//check depth step by step
	{
		mapValue = map[ ((UBYTE)( px + *rayXPtr))  >> 1 ][ ((UBYTE)( py + (tz<<engine.renderer.renderingDepthStep) ))  >> 1 ];//read color + height
		th = mapValue;//take just the height
		slope = th - (ph + *rayYPtr);//check if read height is higher than what we expect from the raycast table
		if(slope > 0)
		{
			c = (mapValue >> 8);
			tzz[iHor] = tz;//save the depth we've arrived at
			break;
		}
		else
		{	
            rayXPtr++;
            rayYPtr++;
                ++tz;
			//go step in depth if no height hit
		}
	}

	if(c == 99) c = SKY_COLOR - ph/32 - sy/8; //draw sky if too deep
	*address1 = (c<<10) + (c<<5) + (c); *address2 = *address1;
	
	
	return tz;
}
UBYTE ProcessWord2v6(UBYTE rounds, UBYTE sx, UBYTE sy, UWORD *_tz, UWORD *tzz, UBYTE px, UBYTE py,UBYTE ph,
UWORD *address1, UWORD *address2, 
WORD (*rayCastX), WORD (*rayCastY), UWORD (*map)[MAPSIZE], UBYTE threshold)
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
        UWORD *rayXPtr = rayCastX + sx*TERRAINDEPTH + tz;
        UWORD *rayYPtr = rayCastY + sy*TERRAINDEPTH + tz;
		while(tz < threshold)//check depth step by step
		{
			mapValue = map[ ((UBYTE)( px + *rayXPtr))  >> 1 ][ ((UBYTE)( py + (tz<<engine.renderer.renderingDepthStep) )) >> 1 ];//read color + height
			th = mapValue;//take just the height
			slope = th - (ph + *rayYPtr);//check if read height is higher than what we expect from the raycast table
			if(slope > 0)
			{
				c[iHor] = (mapValue >> 8);
				tzz[iHor] = tz;//save the depth we've arrived at
				break;
			}
			else
			{	
                rayXPtr++;
                rayYPtr++;
                ++tz;
				//go step in depth if no height hit
			}
		}
		
		sx += 3;
	}
	if(c[0] == 99) c[0] = SKY_COLOR - ph/32 - sy/8; //draw sky if too deep
	if(c[1] == 99) c[1] = SKY_COLOR - ph/32 - sy/8; //draw sky if too deep
	*address1 = (c[0]<<10) + (c[0]<<5) + (c[0]); *address2 = (c[1]<<10) + (c[1]<<5) + (c[1]);

	return tz;
}
UBYTE ProcessWord3v6(UBYTE rounds, UBYTE sx, UBYTE sy, UWORD *_tz, UWORD *tzz, UBYTE px, UBYTE py,UBYTE ph,
UWORD *address1, UWORD *address2, 
WORD (*rayCastX), WORD (*rayCastY), UWORD (*map)[MAPSIZE], UBYTE threshold)
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
        UWORD *rayXPtr = rayCastX + sx*TERRAINDEPTH + tz;
        UWORD *rayYPtr = rayCastY + sy*TERRAINDEPTH + tz;
		while(tz < threshold)//check depth step by step
		{
			mapValue = map[ ((UBYTE)( px + *rayXPtr )) >> 1 ][ ((UBYTE)( py + (tz<<engine.renderer.renderingDepthStep) )) >> 1 ];//read color + height
			th = mapValue;//take just the height
			slope = th - (ph + *rayYPtr);//check if read height is higher than what we expect from the raycast table
			if(slope > 0)
			{
				c[iHor] = (mapValue >> 8);
				tzz[iHor] = tz;//save the depth we've arrived at
				break;
			}
			else
			{	
                rayXPtr++;
                rayYPtr++;
                ++tz;
				//go step in depth if no height hit
			}
		}
		sx +=2;
	}
	if(c[0] == 99) c[0] = SKY_COLOR - ph/32 - sy/8; //draw sky if too deep
	if(c[1] == 99) c[1] = SKY_COLOR - ph/32 - sy/8; //draw sky if too deep
	if(c[2] == 99) c[2] = SKY_COLOR - ph/32 - sy/8; //draw sky if too deep
	*address1 = (c[0]<<10) + (c[0]<<5) + (c[1]); *address2 = (c[1]<<10) + (c[2]<<5) + (c[2]);

	return tz;
}

void ProcessRayCastsProgressive4x4(WORD (*rayCastX), WORD (*rayCastY), UWORD (*map)[MAPSIZE],
	UBYTE px, UBYTE py, UBYTE ph, UBYTE screenStart, UBYTE screenEnd, UBYTE zStart)
{
	UBYTE sx,sy,mist;
	UWORD tz,tzz[6];
	UWORD address1,address2;
	UBYTE threshold1 = TERRAINDEPTH/2;

	//UWORD positionStart =  (ySize-1)*20 + tableXStart/6;

	UBYTE verticalSteps;

	UBYTE iInit;


	//start with the buffor + vertical stripe start + turning amount
	sx = XTURNBUFFOR + engine.renderer.xOffsetOdd + screenStart*4;
	//currentScreenYStepSize = xCycles;


	//for each vertical line
    
 #if AMIGA

    UWORD *firstCol = engine.renderer.plane1W + (YSIZEEVEN+1)*PLANEWIDTHWORD*2;
    UWORD *secondCol = engine.renderer.plane2W + (YSIZEEVEN+1)*PLANEWIDTHWORD*2;
    UWORD *thirdCol = engine.renderer.plane3W + (YSIZEEVEN+1)*PLANEWIDTHWORD*2;
    UWORD *fourthCol = engine.renderer.plane4W + (YSIZEEVEN+1)*PLANEWIDTHWORD*2;

    #else
    UWORD *firstCol = engine.renderer.planes + (YSIZEEVEN+1)*PLANEWIDTHWORD*8;
    UWORD *secondCol = engine.renderer.planes + (YSIZEEVEN+1)*PLANEWIDTHWORD*8 + 1;
    UWORD *thirdCol = engine.renderer.planes + (YSIZEEVEN+1)*PLANEWIDTHWORD*8 + 2;
    UWORD *fourthCol = engine.renderer.planes + (YSIZEEVEN+1)*PLANEWIDTHWORD*8 + 3;
	#endif


    UWORD planeStride = PLANEWIDTHWORD*2;
    
	UWORD word1, word2, word3, word4;
	for(UBYTE iVert=screenStart;iVert<screenEnd;iVert++)
	{
        #if AMIGA
        UWORD *firstPos = firstCol + iVert;
        UWORD *secondPos = secondCol + iVert;
        UWORD *thirdPos = thirdCol + iVert;
        UWORD *fourthPos = fourthCol + iVert;
        #else
        UWORD *firstPos = firstCol + iVert*4;
        UWORD *secondPos = secondCol + iVert*4;
        UWORD *thirdPos = thirdCol + iVert*4;
        UWORD *fourthPos = fourthCol + iVert*4;
		#endif

		//start from the bottom
		sy = 0;
		//position = positionStart + iVert ;//+ 80*12;
		
		//init values for this vertical line
		tzz[0]=zStart;tzz[1]=zStart;tzz[2]=zStart;
		//tzz[3]=zStart;tzz[4]=zStart;tzz[5]=zStart;
		tz = zStart;

		//process this vertical line
		while(sy < YSIZEEVEN)
		{ 
			 if(tz < threshold1)			
			 {
	            UBYTE byte1, byte2, byte3, byte4;
				tz = ProcessWord1v4(1,sx,sy,&tz,tzz,px,py,ph,&address1,&address2,rayCastX, rayCastY, map, threshold1);
				byte1 = engine.renderer.ditherTable1[ address1 ];
				word1 = (byte1<<8) + byte1;
				byte2 = engine.renderer.ditherTable2[ address1 ];
				word2 = (byte2<<8) + byte2;
				byte3 = engine.renderer.ditherTable3[ address1 ];
				word3 = (byte3<<8) + byte3;
				byte4 = engine.renderer.ditherTable4[ address1 ];
				word4 = (byte4<<8) + byte4;

			 }
			if(tz >= threshold1 && tz < engine.renderer.renderingDepth)			
			 {
				tz = ProcessWord2v4(2,sx,sy,&tz,tzz,px,py,ph,&address1,&address2,rayCastX, rayCastY, map, engine.renderer.renderingDepth);

				word1 = (engine.renderer.ditherTable1[ address1 ]<<8) + engine.renderer.ditherTable1[ address2 ];
				word2 = (engine.renderer.ditherTable2[ address1 ]<<8) + engine.renderer.ditherTable2[ address2 ];
				word3 = (engine.renderer.ditherTable3[ address1 ]<<8) + engine.renderer.ditherTable3[ address2 ];
				word4 = (engine.renderer.ditherTable4[ address1 ]<<8) + engine.renderer.ditherTable4[ address2 ];

			 }
			 if(tz == engine.renderer.renderingDepth) 	
			 {
	            UBYTE byte1, byte2, byte3, byte4;
				UBYTE color = SKY_COLOR - ph/32 -sy/8;
				address1 = (color<<5) + (color);
	
				byte1 = engine.renderer.ditherTable1[ address1 ];
				word1 = (byte1<<8) + byte1;

				byte2 = engine.renderer.ditherTable2[ address1 ];
				word2 = (byte2<<8) + byte2;

				byte3 = engine.renderer.ditherTable3[ address1 ];
				word3 = (byte3<<8) + byte3;

				byte4 = engine.renderer.ditherTable4[ address1 ];
				word4 = (byte4<<8) + byte4;


			 }

            UWORD otherWord1 = (word1 << 1)|((word1>>1) & 1);
            UWORD otherWord2 = (word2 << 1)|((word2>>1) & 1);
            UWORD otherWord3 = (word3 << 1)|((word3>>1) & 1);
            UWORD otherWord4 = (word4 << 1)|((word4>>1) & 1);
             
            *firstPos = word1;
            firstPos -= planeStride;
            *firstPos = otherWord1;
            firstPos -= planeStride;
            *firstPos = word1;
            firstPos -= planeStride;
            *firstPos = otherWord1;
            firstPos -= planeStride;
            
            *secondPos = word2;
            secondPos -= planeStride;
            *secondPos = otherWord2;
            secondPos -= planeStride;
            *secondPos = word2;
            secondPos -= planeStride;
            *secondPos = otherWord2;
            secondPos -= planeStride;
            
            *thirdPos = word3;
            thirdPos -= planeStride;
            *thirdPos = otherWord3;
            thirdPos -= planeStride;
            *thirdPos = word3;
            thirdPos -= planeStride;
            *thirdPos = otherWord3;
            thirdPos -= planeStride;

            *fourthPos = word4;
            fourthPos -= planeStride;
            *fourthPos = otherWord4;
            fourthPos -= planeStride;
            *fourthPos = word4;
            fourthPos -= planeStride;
            *fourthPos = otherWord4;
            fourthPos -= planeStride;
			//go step higher in the raycast table
            #if AMIGA
            sy+=2;
            #else
            sy += 1;
			#endif
		}
		
		sx += 4;//go to the next vertical line
	}
}


void ProcessRayCastsProgressive4x4NonInterleaved(WORD (*rayCastX), WORD (*rayCastY), UWORD (*map)[MAPSIZE],
	UBYTE px, UBYTE py, UBYTE ph, UBYTE screenStart, UBYTE screenEnd, UBYTE zStart)
{
	UBYTE sx,sy,mist;
	UWORD tz,tzz[6];
	UWORD address1,address2;
	UBYTE threshold1 = TERRAINDEPTH/2;

	//UWORD positionStart =  (ySize-1)*20 + tableXStart/6;

	UBYTE verticalSteps;

	UBYTE iInit;


	//start with the buffor + vertical stripe start + turning amount
	sx = XTURNBUFFOR + engine.renderer.xOffsetOdd + screenStart*4;
	//currentScreenYStepSize = xCycles;


	//for each vertical line
    
#if AMIGA

    UWORD *firstCol = engine.renderer.plane1W + (YSIZEEVEN+1)*PLANEWIDTHWORD*2;
    UWORD *secondCol = engine.renderer.plane2W + (YSIZEEVEN+1)*PLANEWIDTHWORD*2;
    UWORD *thirdCol = engine.renderer.plane3W + (YSIZEEVEN+1)*PLANEWIDTHWORD*2;
    UWORD *fourthCol = engine.renderer.plane4W + (YSIZEEVEN+1)*PLANEWIDTHWORD*2;

    #else
    UWORD *firstCol = engine.renderer.planes + (YSIZEEVEN+1)*PLANEWIDTHWORD*8;
    UWORD *secondCol = engine.renderer.planes + (YSIZEEVEN+1)*PLANEWIDTHWORD*8 + 1;
    UWORD *thirdCol = engine.renderer.planes + (YSIZEEVEN+1)*PLANEWIDTHWORD*8 + 2;
    UWORD *fourthCol = engine.renderer.planes + (YSIZEEVEN+1)*PLANEWIDTHWORD*8 + 3;
	#endif


    UWORD planeStride = PLANEWIDTHWORD;
    
	UWORD word1, word2, word3, word4;
	for(UBYTE iVert=screenStart;iVert<screenEnd;iVert++)
	{
        #if AMIGA
        UWORD *firstPos = firstCol + iVert;
        UWORD *secondPos = secondCol + iVert;
        UWORD *thirdPos = thirdCol + iVert;
        UWORD *fourthPos = fourthCol + iVert;
        #else
        UWORD *firstPos = firstCol + iVert*4;
        UWORD *secondPos = secondCol + iVert*4;
        UWORD *thirdPos = thirdCol + iVert*4;
        UWORD *fourthPos = fourthCol + iVert*4;
		#endif

		//start from the bottom
		sy = 0;
		//position = positionStart + iVert ;//+ 80*12;
		
		//init values for this vertical line
		tzz[0]=zStart;tzz[1]=zStart;tzz[2]=zStart;
		//tzz[3]=zStart;tzz[4]=zStart;tzz[5]=zStart;
		tz = zStart;

		//process this vertical line
		while(sy < YSIZEEVEN)
		{ 
			 if(tz < threshold1)			
			 {
	            UBYTE byte1, byte2, byte3, byte4;
				tz = ProcessWord1v4(1,sx,sy,&tz,tzz,px,py,ph,&address1,&address2,rayCastX, rayCastY, map, threshold1);
				byte1 = engine.renderer.ditherTable1[ address1 ];
				word1 = (byte1<<8) + byte1;
				byte2 = engine.renderer.ditherTable2[ address1 ];
				word2 = (byte2<<8) + byte2;
				byte3 = engine.renderer.ditherTable3[ address1 ];
				word3 = (byte3<<8) + byte3;
				byte4 = engine.renderer.ditherTable4[ address1 ];
				word4 = (byte4<<8) + byte4;

			 }
			if(tz >= threshold1 && tz < engine.renderer.renderingDepth)			
			 {
				tz = ProcessWord2v4(2,sx,sy,&tz,tzz,px,py,ph,&address1,&address2,rayCastX, rayCastY, map, engine.renderer.renderingDepth);

				word1 = (engine.renderer.ditherTable1[ address1 ]<<8) + engine.renderer.ditherTable1[ address2 ];
				word2 = (engine.renderer.ditherTable2[ address1 ]<<8) + engine.renderer.ditherTable2[ address2 ];
				word3 = (engine.renderer.ditherTable3[ address1 ]<<8) + engine.renderer.ditherTable3[ address2 ];
				word4 = (engine.renderer.ditherTable4[ address1 ]<<8) + engine.renderer.ditherTable4[ address2 ];

			 }
			 if(tz == engine.renderer.renderingDepth) 	
			 {
	            UBYTE byte1, byte2, byte3, byte4;
				UBYTE color = SKY_COLOR - ph/32 -sy/8;
				address1 = (color<<5) + (color);
	
				byte1 = engine.renderer.ditherTable1[ address1 ];
				word1 = (byte1<<8) + byte1;

				byte2 = engine.renderer.ditherTable2[ address1 ];
				word2 = (byte2<<8) + byte2;

				byte3 = engine.renderer.ditherTable3[ address1 ];
				word3 = (byte3<<8) + byte3;

				byte4 = engine.renderer.ditherTable4[ address1 ];
				word4 = (byte4<<8) + byte4;


			 }

            UWORD otherWord1 = (word1 << 1)|((word1>>1) & 1);
            UWORD otherWord2 = (word2 << 1)|((word2>>1) & 1);
            UWORD otherWord3 = (word3 << 1)|((word3>>1) & 1);
            UWORD otherWord4 = (word4 << 1)|((word4>>1) & 1);
             
            *firstPos = word1;
            firstPos -= planeStride;
            *firstPos = otherWord1;
            firstPos -= planeStride;
            *firstPos = word1;
            firstPos -= planeStride;
            *firstPos = otherWord1;
            firstPos -= planeStride;
            *firstPos = word1;
            firstPos -= planeStride;
            *firstPos = otherWord1;
            firstPos -= planeStride;
            *firstPos = word1;
            firstPos -= planeStride;
            *firstPos = otherWord1;
            firstPos -= planeStride;
            
            *secondPos = word2;
            secondPos -= planeStride;
            *secondPos = otherWord2;
            secondPos -= planeStride;
            *secondPos = word2;
            secondPos -= planeStride;
            *secondPos = otherWord2;
            secondPos -= planeStride;
            *secondPos = word2;
            secondPos -= planeStride;
            *secondPos = otherWord2;
            secondPos -= planeStride;
            *secondPos = word2;
            secondPos -= planeStride;
            *secondPos = otherWord2;
            secondPos -= planeStride;
            
            *thirdPos = word3;
            thirdPos -= planeStride;
            *thirdPos = otherWord3;
            thirdPos -= planeStride;
            *thirdPos = word3;
            thirdPos -= planeStride;
            *thirdPos = otherWord3;
            thirdPos -= planeStride;
            *thirdPos = word3;
            thirdPos -= planeStride;
            *thirdPos = otherWord3;
            thirdPos -= planeStride;
            *thirdPos = word3;
            thirdPos -= planeStride;
            *thirdPos = otherWord3;
            thirdPos -= planeStride;

            *fourthPos = word4;
            fourthPos -= planeStride;
            *fourthPos = otherWord4;
            fourthPos -= planeStride;
            *fourthPos = word4;
            fourthPos -= planeStride;
            *fourthPos = otherWord4;
            fourthPos -= planeStride;
            *fourthPos = word4;
            fourthPos -= planeStride;
            *fourthPos = otherWord4;
            fourthPos -= planeStride;
            *fourthPos = word4;
            fourthPos -= planeStride;
            *fourthPos = otherWord4;
            fourthPos -= planeStride;
			//go step higher in the raycast table
            #if AMIGA
            sy+=2;
            #else
            sy += 1;
			#endif
		}
		sx += 4;//go to the next vertical line
	}
}

void ProcessRayCastsProgressive(WORD (*rayCastX), WORD (*rayCastY), UWORD (*map)[MAPSIZE],
	UBYTE px, UBYTE py, UBYTE ph, UBYTE screenStart, UBYTE screenEnd, UBYTE zStart)
{
	UBYTE sx,sy,mist;
	UWORD tz,tzz[6];
	UWORD address1,address2;
	UBYTE threshold1 = TERRAINDEPTH/2;
	UBYTE threshold2 = TERRAINDEPTH - TERRAINDEPTH/4;

	//UWORD positionStart =  (ySize-1)*20 + tableXStart/6;

	UBYTE verticalSteps;

	UBYTE iInit;


	//start with the buffor + vertical stripe start + turning amount
	sx = XTURNBUFFOR + engine.renderer.xOffsetOdd + screenStart*6;
	//currentScreenYStepSize = xCycles;


	//for each vertical line
    
 	#if AMIGA

    UWORD *firstCol = engine.renderer.plane1W + (YSIZEODD+1)*PLANEWIDTHWORD*2;
    UWORD *secondCol = engine.renderer.plane2W + (YSIZEODD+1)*PLANEWIDTHWORD*2;
    UWORD *thirdCol = engine.renderer.plane3W + (YSIZEODD+1)*PLANEWIDTHWORD*2;
    UWORD *fourthCol = engine.renderer.plane4W + (YSIZEODD+1)*PLANEWIDTHWORD*2;

    #else
    UWORD *firstCol = engine.renderer.planes + (YSIZEODD+1)*PLANEWIDTHWORD*4;
    UWORD *secondCol = engine.renderer.planes + (YSIZEODD+1)*PLANEWIDTHWORD*4 + 1;
    UWORD *thirdCol = engine.renderer.planes + (YSIZEODD+1)*PLANEWIDTHWORD*4 + 2;
    UWORD *fourthCol = engine.renderer.planes + (YSIZEODD+1)*PLANEWIDTHWORD*4 + 3;
	#endif


    UWORD planeStride = PLANEWIDTHWORD*2;
    
	UWORD word1, word2, word3, word4;
	for(UBYTE iVert=screenStart;iVert<screenEnd;iVert++)
	{
         
        #if AMIGA
        UWORD *firstPos = firstCol + iVert;
        UWORD *secondPos = secondCol + iVert;
        UWORD *thirdPos = thirdCol + iVert;
        UWORD *fourthPos = fourthCol + iVert;
        #else
        UWORD *firstPos = firstCol + iVert*4;
        UWORD *secondPos = secondCol + iVert*4;
        UWORD *thirdPos = thirdCol + iVert*4;
        UWORD *fourthPos = fourthCol + iVert*4;
		#endif

		//start from the bottom
		sy = 0;
		//position = positionStart + iVert ;//+ 80*12;
		
		//init values for this vertical line
		tzz[0]=zStart;tzz[1]=zStart;tzz[2]=zStart;
		//tzz[3]=zStart;tzz[4]=zStart;tzz[5]=zStart;
		tz = zStart;

		//process this vertical line
		while(sy < YSIZEODD)
		{ 
			 if(tz < threshold1)			
			 {
	            UBYTE byte1, byte2, byte3, byte4;
				tz = ProcessWord1v6(1,sx,sy,&tz,tzz,px,py,ph,&address1,&address2,rayCastX, rayCastY, map, threshold1);
				byte1 = engine.renderer.ditherTable1[ address1 ];
				word1 = (byte1<<8) + byte1;
				byte2 = engine.renderer.ditherTable2[ address1 ];
				word2 = (byte2<<8) + byte2;
				byte3 = engine.renderer.ditherTable3[ address1 ];
				word3 = (byte3<<8) + byte3;
				byte4 = engine.renderer.ditherTable4[ address1 ];
				word4 = (byte4<<8) + byte4;

			 }
			if(tz >= threshold1 && tz < threshold2)			
			 {
				tz = ProcessWord2v6(2,sx,sy,&tz,tzz,px,py,ph,&address1,&address2,rayCastX, rayCastY, map, threshold2);

				word1 = (engine.renderer.ditherTable1[ address1 ]<<8) + engine.renderer.ditherTable1[ address2 ];
				word2 = (engine.renderer.ditherTable2[ address1 ]<<8) + engine.renderer.ditherTable2[ address2 ];
				word3 = (engine.renderer.ditherTable3[ address1 ]<<8) + engine.renderer.ditherTable3[ address2 ];
				word4 = (engine.renderer.ditherTable4[ address1 ]<<8) + engine.renderer.ditherTable4[ address2 ];

			 }
			if(tz >= threshold2 && tz < engine.renderer.renderingDepth)			
			 {
				tz = ProcessWord3v6(3,sx,sy,&tz,tzz,px,py,ph,&address1,&address2,rayCastX, rayCastY, map, engine.renderer.renderingDepth);

				word1 = (engine.renderer.ditherTable1[ address1 ]<<8) + engine.renderer.ditherTable1[ address2 ];

				word2 = (engine.renderer.ditherTable2[ address1 ]<<8) + engine.renderer.ditherTable2[ address2 ];
				word3 = (engine.renderer.ditherTable3[ address1 ]<<8) + engine.renderer.ditherTable3[ address2 ];
				word4 = (engine.renderer.ditherTable4[ address1 ]<<8) + engine.renderer.ditherTable4[ address2 ];
			 }
			 if(tz == engine.renderer.renderingDepth) 	
			 {
	            UBYTE byte1, byte2, byte3, byte4;
				UBYTE color = SKY_COLOR - ph/32 -sy/8;
				address1 = (color<<10) + (color<<5) + (color);
	
				byte1 = engine.renderer.ditherTable1[ address1 ];
				word1 = (byte1<<8) + byte1;

				byte2 = engine.renderer.ditherTable2[ address1 ];
				word2 = (byte2<<8) + byte2;

				byte3 = engine.renderer.ditherTable3[ address1 ];
				word3 = (byte3<<8) + byte3;

				byte4 = engine.renderer.ditherTable4[ address1 ];
				word4 = (byte4<<8) + byte4;


			 }

             
            *firstPos = word1;
            firstPos -= planeStride;
            *firstPos = word1;
            firstPos -= planeStride;
            *firstPos = word1;
            firstPos -= planeStride;
            *firstPos = word1;
            firstPos -= planeStride;
            
            *secondPos = word2;
            secondPos -= planeStride;
            *secondPos = word2;
            secondPos -= planeStride;
            *secondPos = word2;
            secondPos -= planeStride;
            *secondPos = word2;
            secondPos -= planeStride;
            
            *thirdPos = word3;
            thirdPos -= planeStride;
            *thirdPos = word3;
            thirdPos -= planeStride;
            *thirdPos = word3;
            thirdPos -= planeStride;
            *thirdPos = word3;
            thirdPos -= planeStride;

            *fourthPos = word4;
            fourthPos -= planeStride;
            *fourthPos = word4;
            fourthPos -= planeStride;
            *fourthPos = word4;
            fourthPos -= planeStride;
            *fourthPos = word4;
            fourthPos -= planeStride;
			//go step higher in the raycast table
            #if AMIGA
            sy+=4;
            #else
            sy += 2;
			#endif
		}
		sx += 6;//go to the next vertical line
	}
}


void ProcessRayCastsProgressiveNonInterleaved(WORD (*rayCastX), WORD (*rayCastY), UWORD (*map)[MAPSIZE],
	UBYTE px, UBYTE py, UBYTE ph, UBYTE screenStart, UBYTE screenEnd, UBYTE zStart)
{
	UBYTE sx,sy,mist;
	UWORD tz,tzz[6];
	UWORD address1,address2;
	UBYTE threshold1 = TERRAINDEPTH/2;
	UBYTE threshold2 = TERRAINDEPTH - TERRAINDEPTH/4;

	//UWORD positionStart =  (ySize-1)*20 + tableXStart/6;

	UBYTE verticalSteps;

	UBYTE iInit;


	//start with the buffor + vertical stripe start + turning amount
	sx = XTURNBUFFOR + engine.renderer.xOffsetOdd + screenStart*6;
	//currentScreenYStepSize = xCycles;


	//for each vertical line
    #if AMIGA

    UWORD *firstCol = engine.renderer.plane1W + (YSIZEODD+1)*PLANEWIDTHWORD*2;
    UWORD *secondCol = engine.renderer.plane2W + (YSIZEODD+1)*PLANEWIDTHWORD*2;
    UWORD *thirdCol = engine.renderer.plane3W + (YSIZEODD+1)*PLANEWIDTHWORD*2;
    UWORD *fourthCol = engine.renderer.plane4W + (YSIZEODD+1)*PLANEWIDTHWORD*2;

    #else

    UWORD *firstCol = engine.renderer.planes + (YSIZEODD+1)*PLANEWIDTHWORD*4;
    UWORD *secondCol = engine.renderer.planes + (YSIZEODD+1)*PLANEWIDTHWORD*4 + 1;
    UWORD *thirdCol = engine.renderer.planes + (YSIZEODD+1)*PLANEWIDTHWORD*4 + 2;
    UWORD *fourthCol = engine.renderer.planes + (YSIZEODD+1)*PLANEWIDTHWORD*4 + 3;

	#endif


    UWORD planeStride = PLANEWIDTHWORD;
    
	UWORD word1, word2, word3, word4;
	for(UBYTE iVert=screenStart;iVert<screenEnd;iVert++)
	{
        #if AMIGA
        UWORD *firstPos = firstCol + iVert;
        UWORD *secondPos = secondCol + iVert;
        UWORD *thirdPos = thirdCol + iVert;
        UWORD *fourthPos = fourthCol + iVert;
        #else
        UWORD *firstPos = firstCol + iVert*4;
        UWORD *secondPos = secondCol + iVert*4;
        UWORD *thirdPos = thirdCol + iVert*4;
        UWORD *fourthPos = fourthCol + iVert*4;
		#endif

		//start from the bottom
		sy = 0;
		//position = positionStart + iVert ;//+ 80*12;
		
		//init values for this vertical line
		tzz[0]=zStart;tzz[1]=zStart;tzz[2]=zStart;
		//tzz[3]=zStart;tzz[4]=zStart;tzz[5]=zStart;
		tz = zStart;

		//process this vertical line
		while(sy < YSIZEODD)
		{ 
			 if(tz < threshold1)			
			 {
	            UBYTE byte1, byte2, byte3, byte4;
				tz = ProcessWord1v6(1,sx,sy,&tz,tzz,px,py,ph,&address1,&address2,rayCastX, rayCastY, map, threshold1);
				byte1 = engine.renderer.ditherTable1[ address1 ];
				word1 = (byte1<<8) + byte1;
				byte2 = engine.renderer.ditherTable2[ address1 ];
				word2 = (byte2<<8) + byte2;
				byte3 = engine.renderer.ditherTable3[ address1 ];
				word3 = (byte3<<8) + byte3;
				byte4 = engine.renderer.ditherTable4[ address1 ];
				word4 = (byte4<<8) + byte4;

			 }
			if(tz >= threshold1 && tz < threshold2)			
			 {
				tz = ProcessWord2v6(2,sx,sy,&tz,tzz,px,py,ph,&address1,&address2,rayCastX, rayCastY, map, threshold2);

				word1 = (engine.renderer.ditherTable1[ address1 ]<<8) + engine.renderer.ditherTable1[ address2 ];
				word2 = (engine.renderer.ditherTable2[ address1 ]<<8) + engine.renderer.ditherTable2[ address2 ];
				word3 = (engine.renderer.ditherTable3[ address1 ]<<8) + engine.renderer.ditherTable3[ address2 ];
				word4 = (engine.renderer.ditherTable4[ address1 ]<<8) + engine.renderer.ditherTable4[ address2 ];

			 }
			if(tz >= threshold2 && tz < engine.renderer.renderingDepth)			
			 {
				tz = ProcessWord3v6(3,sx,sy,&tz,tzz,px,py,ph,&address1,&address2,rayCastX, rayCastY, map, engine.renderer.renderingDepth);

				word1 = (engine.renderer.ditherTable1[ address1 ]<<8) + engine.renderer.ditherTable1[ address2 ];

				word2 = (engine.renderer.ditherTable2[ address1 ]<<8) + engine.renderer.ditherTable2[ address2 ];
				word3 = (engine.renderer.ditherTable3[ address1 ]<<8) + engine.renderer.ditherTable3[ address2 ];
				word4 = (engine.renderer.ditherTable4[ address1 ]<<8) + engine.renderer.ditherTable4[ address2 ];
			 }
			 if(tz == engine.renderer.renderingDepth) 	
			 {
	            UBYTE byte1, byte2, byte3, byte4;
				UBYTE color = SKY_COLOR - ph/32 -sy/8;
				address1 = (color<<10) + (color<<5) + (color);
	
				byte1 = engine.renderer.ditherTable1[ address1 ];
				word1 = (byte1<<8) + byte1;

				byte2 = engine.renderer.ditherTable2[ address1 ];
				word2 = (byte2<<8) + byte2;

				byte3 = engine.renderer.ditherTable3[ address1 ];
				word3 = (byte3<<8) + byte3;

				byte4 = engine.renderer.ditherTable4[ address1 ];
				word4 = (byte4<<8) + byte4;


			 }

            
            UWORD word11 = (word1 << 1)|((word1>>1) & 1);
            UWORD word21 = (word2 << 1)|((word2>>1) & 1);
            UWORD word31 = (word3 << 1)|((word3>>1) & 1);
            UWORD word41 = (word4 << 1)|((word4>>1) & 1);
             
            *firstPos = word1;
            firstPos -= planeStride;
            *firstPos = word11;
            firstPos -= planeStride;
            *firstPos = word1;
            firstPos -= planeStride;
            *firstPos = word11;
            firstPos -= planeStride;
            
            *secondPos = word2;
            secondPos -= planeStride;
            *secondPos = word21;
            secondPos -= planeStride;
            *secondPos = word2;
            secondPos -= planeStride;
            *secondPos = word21;
            secondPos -= planeStride;
            
            *thirdPos = word3;
            thirdPos -= planeStride;
            *thirdPos = word31;
            thirdPos -= planeStride;
            *thirdPos = word3;
            thirdPos -= planeStride;
            *thirdPos = word31;
            thirdPos -= planeStride;

            *fourthPos = word4;
            fourthPos -= planeStride;
            *fourthPos = word41;
            fourthPos -= planeStride;
            *fourthPos = word4;
            fourthPos -= planeStride;
            *fourthPos = word41;
            fourthPos -= planeStride;
			//go step higher in the raycast table
            #if AMIGA
            sy+=2;
            #else
            sy += 1;
			#endif
		}
		sx += 6;//go to the next vertical line
	}
}

void ProcessRayCastsFull3x2(WORD (*rayCastX), WORD (*rayCastY), UWORD (*map)[MAPSIZE],
UBYTE px, UBYTE py, UBYTE ph, UBYTE screenStart, UBYTE screenEnd, UBYTE zStart)
{

    
	UWORD yOffset = (256 - YSIZEODD*2)/2 * PLANEWIDTHWORD;
    WORD baseX = XTURNBUFFOR + engine.renderer.xOffsetOdd + screenStart*6;

	//for each vertical line
    

    for(UBYTE x = screenStart; x < screenEnd; ++x)
    {

        for(UBYTE i = 0; i < 6; ++i)
        {
            UBYTE sy = 0;
            UWORD tz = zStart;
            WORD *rayXPtr = rayCastX + baseX*TERRAINDEPTH + tz;
            WORD *rayYPtr = rayCastY + tz;
            UBYTE *screenPtr = engine.renderer.screenPatch + ((YSIZEODD)-1)*6 + i;
	        UBYTE mx,my;
	        UWORD mapValue;

            while(tz < engine.renderer.renderingDepth)
            {
                mx = px + *rayXPtr;
                my = (py + (tz<<engine.renderer.renderingDepthStep));
                mapValue = map[ mx >> 1 ][ my >> 1 ];
                UBYTE th = mapValue;

                WORD slope = th - (ph + *rayYPtr);

                if(slope > tz>>2)
                {
                    *screenPtr = ((mapValue >> 8) + ((slope/4) & 1));
                    sy+=1;//go step higher in the raycast table
                    rayYPtr += TERRAINDEPTH;
                    screenPtr-=6;//go step higher on screen
                    if(sy == YSIZEODD) tz=engine.renderer.renderingDepth; //break if end of screen
                }
                else if(slope > 0)
                {
                    *screenPtr = (mapValue >> 8) + 2;
                    sy+=1;//go step higher in the raycast table
                    rayYPtr += TERRAINDEPTH;
                    screenPtr-=6;//go step higher on screen
                    if(sy == YSIZEODD) tz=engine.renderer.renderingDepth; //break if end of screen
                }
                else
                {	
                    //screen[position] = 0;
                    tz++;//go step in depth if no height hit
                    rayXPtr++;
                    rayYPtr++;
                }
            }
            
            while(sy < YSIZEODD)
            {
                //if(screen[position] == 31)
                //	sy = YSIZE;
                //else
                {
                    *screenPtr =  32 - ph/32 -sy/8;
                    sy+=1;
                    screenPtr-=6;
                }

            }

            baseX++;
        }

#if AMIGA

    UWORD *planePos1 = engine.renderer.plane1W + yOffset + x;
    UWORD *planePos2 = engine.renderer.plane2W + yOffset + x;
    UWORD *planePos3 = engine.renderer.plane3W + yOffset + x;
    UWORD *planePos4 = engine.renderer.plane4W + yOffset + x;
    
    #else
        UWORD *planePos1 = engine.renderer.planes + yOffset + x*4;
        UWORD *planePos2 = engine.renderer.planes + yOffset + x*4 + 1;
        UWORD *planePos3 = engine.renderer.planes + yOffset + x*4 + 2;
        UWORD *planePos4 = engine.renderer.planes + yOffset + x*4 + 3;
		#endif
        UWORD sp = 0;

        for(UBYTE y=0;y<YSIZEODD;y++)
	    {

			UWORD address1 = (engine.renderer.screenPatch[sp]<<10) + (engine.renderer.screenPatch[sp+1]<<5) + (engine.renderer.screenPatch[sp+2]);
			UWORD address2 = (engine.renderer.screenPatch[sp+3]<<10) + (engine.renderer.screenPatch[sp+4]<<5) + (engine.renderer.screenPatch[sp+5]);

            
			UWORD word = (engine.renderer.ditherTable1[ address1 ]<<8) + engine.renderer.ditherTable1[ address2 ];
			*planePos1 = word;
            planePos1 += PLANEWIDTHWORD;
			*planePos1 = (word << 1)|((word>>1) & 1);
            planePos1 += PLANEWIDTHWORD;
			word = (engine.renderer.ditherTable2[ address1 ]<<8) + engine.renderer.ditherTable2[ address2 ];
			*planePos2 = word;
            planePos2 += PLANEWIDTHWORD;
			*planePos2 = (word << 1)|((word>>1) & 1);
            planePos2 += PLANEWIDTHWORD;
			word = (engine.renderer.ditherTable3[ address1 ]<<8) + engine.renderer.ditherTable3[ address2 ];
			*planePos3 = word;
            planePos3 += PLANEWIDTHWORD;
			*planePos3 = word = (word << 1)|((word>>1) & 1);
            planePos3 += PLANEWIDTHWORD;
			word = (engine.renderer.ditherTable4[ address1 ]<<8) + engine.renderer.ditherTable4[ address2 ];
			*planePos4 = word;
            planePos4 += PLANEWIDTHWORD;
			*planePos4 = word = (word << 1)|((word>>1) & 1);
            planePos4 += PLANEWIDTHWORD;

            sp += 6;
        }

    }

}


void ProcessRayCastsFull4x4(WORD (*rayCastX), WORD (*rayCastY), UWORD (*map)[MAPSIZE],
UBYTE px, UBYTE py, UBYTE ph, UBYTE screenStart, UBYTE screenEnd, UBYTE zStart)
{

    
	UWORD yOffset = (256 - YSIZEEVEN*4)/2 * PLANEWIDTHWORD;
    UBYTE baseX = XTURNBUFFOR + engine.renderer.xOffsetEven + screenStart*4;

	//for each vertical line
    

    for(UBYTE x = screenStart; x < screenEnd; ++x)
    {

        for(UBYTE i = 0; i < 4; ++i)
        {
            UBYTE sy = 0;
            UWORD tz = zStart;
            UWORD *rayXPtr = rayCastX + baseX*TERRAINDEPTH + tz;
            UWORD *rayYPtr = rayCastY + tz;
            UBYTE *screenPtr = engine.renderer.screenPatch + ((YSIZEEVEN)-1)*4 + i;
	        UBYTE mx,my;
	        UWORD mapValue;

            while(tz < engine.renderer.renderingDepth)
            {
                mx = px + *rayXPtr;
                my = (py + (tz<<engine.renderer.renderingDepthStep));
                mapValue = map[ mx >> 1 ][ my >> 1 ];
                UBYTE th = mapValue;

                WORD slope = th - (ph + *rayYPtr);

                if(slope > tz>>2)
                {
                    *screenPtr = ((mapValue >> 8) + ((slope/4) & 1));
                    sy+=1;//go step higher in the raycast table
                    rayYPtr += TERRAINDEPTH;
                    screenPtr-=4;//go step higher on screen
                    if(sy == YSIZEEVEN) tz=engine.renderer.renderingDepth; //break if end of screen
                }
                else if(slope > 0)
                {
                    *screenPtr = (mapValue >> 8) + 2;
                    sy+=1;//go step higher in the raycast table
                    rayYPtr += TERRAINDEPTH;
                    screenPtr-=4;//go step higher on screen
                    if(sy == YSIZEEVEN) tz=engine.renderer.renderingDepth; //break if end of screen
                }
                else
                {	
                    tz++;//go step in depth if no height hit
                    rayXPtr++;
                    rayYPtr++;
                }
            }
            
            while(sy < YSIZEEVEN)
            {
                *screenPtr =  32 - ph/32 -sy/8;
                sy+=1;
                screenPtr-=4;

            }

            baseX++;
        }

#if AMIGA

    UWORD *planePos1 = engine.renderer.plane1W + yOffset + x;
    UWORD *planePos2 = engine.renderer.plane2W + yOffset + x;
    UWORD *planePos3 = engine.renderer.plane3W + yOffset + x;
    UWORD *planePos4 = engine.renderer.plane4W + yOffset + x;
    
    #else
        UWORD *planePos1 = engine.renderer.planes + yOffset + x*4;
        UWORD *planePos2 = engine.renderer.planes + yOffset + x*4 + 1;
        UWORD *planePos3 = engine.renderer.planes + yOffset + x*4 + 2;
        UWORD *planePos4 = engine.renderer.planes + yOffset + x*4 + 3;
        
		#endif
		UWORD sp = 0;
        for(UBYTE y=0;y<YSIZEEVEN;y++)
	    {

			UWORD address1 = ((engine.renderer.screenPatch[sp])<<5) + (engine.renderer.screenPatch[sp+1]);
			UWORD address2 = ((engine.renderer.screenPatch[sp+2])<<5) + (engine.renderer.screenPatch[sp+3]);

            
			UWORD word = (engine.renderer.ditherTable1[ address1 ]<<8) + engine.renderer.ditherTable1[ address2 ];
            UWORD otherWord = (word << 1)|((word>>1) & 1);
			*planePos1 = word;
            planePos1 += PLANEWIDTHWORD;
			*planePos1 = otherWord;
            planePos1 += PLANEWIDTHWORD;
            *planePos1 = word;
            planePos1 += PLANEWIDTHWORD;
            *planePos1 = otherWord;
            planePos1 += PLANEWIDTHWORD;
			word = (engine.renderer.ditherTable2[ address1 ]<<8) + engine.renderer.ditherTable2[ address2 ];
            otherWord = (word << 1)|((word>>1) & 1);
			*planePos2 = word;
            planePos2 += PLANEWIDTHWORD;
			*planePos2 = otherWord;
            planePos2 += PLANEWIDTHWORD;
			*planePos2 = word;
            planePos2 += PLANEWIDTHWORD;
			*planePos2 = otherWord;
            planePos2 += PLANEWIDTHWORD;
			word = (engine.renderer.ditherTable3[ address1 ]<<8) + engine.renderer.ditherTable3[ address2 ];
            otherWord = (word << 1)|((word>>1) & 1);
			*planePos3 = word;
            planePos3 += PLANEWIDTHWORD;
			*planePos3 = otherWord;
            planePos3 += PLANEWIDTHWORD;
			*planePos3 = word;
            planePos3 += PLANEWIDTHWORD;
			*planePos3 = otherWord;
            planePos3 += PLANEWIDTHWORD;
			word = (engine.renderer.ditherTable4[ address1 ]<<8) + engine.renderer.ditherTable4[ address2 ];
            otherWord = (word << 1)|((word>>1) & 1);
			*planePos4 = word;
            planePos4 += PLANEWIDTHWORD;
			*planePos4 = otherWord;
            planePos4 += PLANEWIDTHWORD;
			*planePos4 = word;
            planePos4 += PLANEWIDTHWORD;
			*planePos4 = otherWord;
            planePos4 += PLANEWIDTHWORD;

            sp += 4;
        }

    }

}

void ProcessRayCasts163x2( WORD (*rayCastX), WORD (*rayCastY), UWORD (*map)[MAPSIZE],
UBYTE px, UBYTE py, UBYTE ph, UBYTE screenStart, UBYTE screenEnd, UBYTE zStart)
{

    
    
	UWORD yOffset = (256 - YSIZEODD*2)/2 * PLANEWIDTHWORD;
    UBYTE baseX = XTURNBUFFOR + engine.renderer.xOffsetOdd + screenStart*6;

	//for each vertical line
    

    for(UBYTE x = screenStart; x < screenEnd; ++x)
    {

        for(UBYTE i = 0; i < 6; ++i)
        {
            UBYTE sy = 0;
            UWORD tz = zStart;
            UWORD *rayXPtr = rayCastX + baseX*TERRAINDEPTH + tz;
            UWORD *rayYPtr = rayCastY + tz;
            UBYTE *screenPtr = engine.renderer.screenPatch + ((YSIZEODD)-1)*6 + i;
	        UBYTE mx,my;
	        UWORD mapValue;

            while(tz < engine.renderer.renderingDepth)
            {
                mx = px + *rayXPtr;
                my = (py + (tz<<engine.renderer.renderingDepthStep));
                mapValue = map[ mx >> 1 ][ my >> 1 ];
                UBYTE th = mapValue;

                if(th > ph + *rayYPtr)//tz>>3)
                {
                    *screenPtr = (mapValue >> 8);// - (mist>>4);//+ ((slope/4) & 1);//( ( 13 - (mapValue >> 8) ) >> (mist>>5) )+ 13 + ((slope/4) & 1);// + (((tz+py)>>2) & 1);//write pixel color
                    
                    sy+=1;//go step higher in the raycast table
                    rayYPtr += TERRAINDEPTH;
                    screenPtr-=6;//go step higher on screen
                }
                else
                {	
                    //screen[position] = 0;
                    tz++;//go step in depth if no height hit
                    rayXPtr++;
                    rayYPtr++;
                }
            }
            
            while(sy < YSIZEODD)
            {
                //if(screen[position] == 31)
                //	sy = YSIZE;
                //else
                {
                    *screenPtr =  32 - ph/32 -sy/8;
                    sy+=1;
                    screenPtr-=6;
                }

            }

            baseX++;
        }
#if AMIGA

    UWORD *planePos1 = engine.renderer.plane1W + yOffset + x;
    UWORD *planePos2 = engine.renderer.plane2W + yOffset + x;
    UWORD *planePos3 = engine.renderer.plane3W + yOffset + x;
    UWORD *planePos4 = engine.renderer.plane4W + yOffset + x;
    
    #else
        UWORD *planePos1 = engine.renderer.planes + yOffset + x*4;
        UWORD *planePos2 = engine.renderer.planes + yOffset + x*4 + 1;
        UWORD *planePos3 = engine.renderer.planes + yOffset + x*4 + 2;
        UWORD *planePos4 = engine.renderer.planes + yOffset + x*4 + 3;
		#endif
        UWORD sp = 0;

        for(UBYTE y=0;y<YSIZEODD;y++)
	    {

			UWORD address1 = (engine.renderer.screenPatch[sp]<<10) + (engine.renderer.screenPatch[sp+1]<<5) + (engine.renderer.screenPatch[sp+2]);
			UWORD address2 = (engine.renderer.screenPatch[sp+3]<<10) + (engine.renderer.screenPatch[sp+4]<<5) + (engine.renderer.screenPatch[sp+5]);

            
			UWORD word = (engine.renderer.ditherTable1[ address1 ]<<8) + engine.renderer.ditherTable1[ address2 ];
			*planePos1 = word;
            planePos1 += PLANEWIDTHWORD;
			*planePos1 = (word << 1)|((word>>1) & 1);
            planePos1 += PLANEWIDTHWORD;
			word = (engine.renderer.ditherTable2[ address1 ]<<8) + engine.renderer.ditherTable2[ address2 ];
			*planePos2 = word;
            planePos2 += PLANEWIDTHWORD;
			*planePos2 = (word << 1)|((word>>1) & 1);
            planePos2 += PLANEWIDTHWORD;
			word = (engine.renderer.ditherTable3[ address1 ]<<8) + engine.renderer.ditherTable3[ address2 ];
			*planePos3 = word;
            planePos3 += PLANEWIDTHWORD;
			*planePos3 = word = (word << 1)|((word>>1) & 1);
            planePos3 += PLANEWIDTHWORD;
			word = (engine.renderer.ditherTable4[ address1 ]<<8) + engine.renderer.ditherTable4[ address2 ];
			*planePos4 = word;
            planePos4 += PLANEWIDTHWORD;
			*planePos4 = word = (word << 1)|((word>>1) & 1);
            planePos4 += PLANEWIDTHWORD;

            sp += 6;
        }

    }
}

void ProcessRayCasts164x4(WORD (*rayCastX), WORD (*rayCastY), UWORD (*map)[MAPSIZE],
UBYTE px, UBYTE py, UBYTE ph, UBYTE screenStart, UBYTE screenEnd, UBYTE zStart)
{

    
	UWORD yOffset = (256 - YSIZEEVEN*4)/2 * PLANEWIDTHWORD;
    UBYTE baseX = XTURNBUFFOR + engine.renderer.xOffsetEven + screenStart*4;

	//for each vertical line
    

    for(UBYTE x = screenStart; x < screenEnd; ++x)
    {

        for(UBYTE i = 0; i < 4; ++i)
        {
            UBYTE sy = 0;
            UWORD tz = zStart;
            UWORD *rayXPtr = rayCastX + baseX*TERRAINDEPTH + tz;
            UWORD *rayYPtr = rayCastY + tz;
            UBYTE *screenPtr = engine.renderer.screenPatch + ((YSIZEEVEN)-1)*4 + i;
	        UBYTE mx,my;
	        UWORD mapValue;

            while(tz < engine.renderer.renderingDepth)
            {
                mx = px + *rayXPtr;
                my = (py + (tz<<engine.renderer.renderingDepthStep));
                mapValue = map[ mx >> 1 ][ my >> 1 ];
                UBYTE th = mapValue;

                if(th > ph + *rayYPtr)//tz>>3)
                {
                    *screenPtr = (mapValue >> 8);// - (mist>>4);//+ ((slope/4) & 1);//( ( 13 - (mapValue >> 8) ) >> (mist>>5) )+ 13 + ((slope/4) & 1);// + (((tz+py)>>2) & 1);//write pixel color
                    
                    sy+=1;//go step higher in the raycast table
                    rayYPtr += TERRAINDEPTH;
                    screenPtr-=4;//go step higher on screen
                }
                else
                {	
                    //screen[position] = 0;
                    tz++;//go step in depth if no height hit
                    rayXPtr++;
                    rayYPtr++;
                }
            }
            
            while(sy < YSIZEEVEN)
            {
                *screenPtr =  32 - ph/32 -sy/8;
                sy+=1;
                screenPtr-=4;

            }

            baseX++;
        }


#if AMIGA

    UWORD *planePos1 = engine.renderer.plane1W + yOffset + x;
    UWORD *planePos2 = engine.renderer.plane2W + yOffset + x;
    UWORD *planePos3 = engine.renderer.plane3W + yOffset + x;
    UWORD *planePos4 = engine.renderer.plane4W + yOffset + x;
    
    #else
        UWORD *planePos1 = engine.renderer.planes + yOffset + x*4;
        UWORD *planePos2 = engine.renderer.planes + yOffset + x*4 + 1;
        UWORD *planePos3 = engine.renderer.planes + yOffset + x*4 + 2;
        UWORD *planePos4 = engine.renderer.planes + yOffset + x*4 + 3;
		#endif
        UWORD sp = 0;

        for(UBYTE y=0;y<YSIZEEVEN;y++)
	    {

			UWORD address1 = ((engine.renderer.screenPatch[sp])<<5) + (engine.renderer.screenPatch[sp+1]);
			UWORD address2 = ((engine.renderer.screenPatch[sp+2])<<5) + (engine.renderer.screenPatch[sp+3]);

            
			UWORD word = (engine.renderer.ditherTable1[ address1 ]<<8) + engine.renderer.ditherTable1[ address2 ];
            UWORD otherWord = (word << 1)|((word>>1) & 1);
			*planePos1 = word;
            planePos1 += PLANEWIDTHWORD;
			*planePos1 = otherWord;
            planePos1 += PLANEWIDTHWORD;
            *planePos1 = word;
            planePos1 += PLANEWIDTHWORD;
            *planePos1 = otherWord;
            planePos1 += PLANEWIDTHWORD;
			word = (engine.renderer.ditherTable2[ address1 ]<<8) + engine.renderer.ditherTable2[ address2 ];
            otherWord = (word << 1)|((word>>1) & 1);
			*planePos2 = word;
            planePos2 += PLANEWIDTHWORD;
			*planePos2 = otherWord;
            planePos2 += PLANEWIDTHWORD;
			*planePos2 = word;
            planePos2 += PLANEWIDTHWORD;
			*planePos2 = otherWord;
            planePos2 += PLANEWIDTHWORD;
			word = (engine.renderer.ditherTable3[ address1 ]<<8) + engine.renderer.ditherTable3[ address2 ];
            otherWord = (word << 1)|((word>>1) & 1);
			*planePos3 = word;
            planePos3 += PLANEWIDTHWORD;
			*planePos3 = otherWord;
            planePos3 += PLANEWIDTHWORD;
			*planePos3 = word;
            planePos3 += PLANEWIDTHWORD;
			*planePos3 = otherWord;
            planePos3 += PLANEWIDTHWORD;
			word = (engine.renderer.ditherTable4[ address1 ]<<8) + engine.renderer.ditherTable4[ address2 ];
            otherWord = (word << 1)|((word>>1) & 1);
			*planePos4 = word;
            planePos4 += PLANEWIDTHWORD;
			*planePos4 = otherWord;
            planePos4 += PLANEWIDTHWORD;
			*planePos4 = word;
            planePos4 += PLANEWIDTHWORD;
			*planePos4 = otherWord;
            planePos4 += PLANEWIDTHWORD;

            sp += 4;
        }

    }

}