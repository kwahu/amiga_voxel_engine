#include "ray_casting.h"

void ProcessRayCasts(UBYTE *screen, WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH],
UBYTE px, UBYTE py, UBYTE ph, UBYTE tableXStart,
UBYTE tableStepSizeX, UBYTE tableStepSizeY, UBYTE tableStepNumber, UBYTE xCycles)
{
	UBYTE sx,sy,tz;
	UBYTE th = 0;
	UWORD position;
	UBYTE mx,my;
	//UBYTE mipLevel;
	UBYTE color = 0;
	//UWORD currentTableYStepSize;
	UWORD currentScreenYStepSize;
	//UBYTE *heightMap, *colorMap;

	sx = tableXStart;//set the x position in the raycast table
	//set the start position (left bottom pixel) on the destination screen
	//screenStartPosition = (YSIZE-1)*xCycles;
	//currentTableYStepSize = tableStepSizeY*XSIZE;
	currentScreenYStepSize = xCycles; //the same as the numer of x steps
	//screenYStepSize = stepSize*XSIZE;
/*
	if(tableStepSizeY == 1)
	{
		heightMap = heightMap0;
		colorMap = colorMap0;
	}
	else if(tableStepSizeY == 2)
	{
		heightMap = heightMap2;
		colorMap = colorMap2;
	}
	else if(tableStepSizeY == 4)
	{
		heightMap = heightMap3;
		colorMap = colorMap3;
	}*/




	for(UBYTE i=0;i<xCycles;i++)
	{
		//********* INITIALIZE INTERLACED CALCUTATIONS
		//sy = interlace/2 + modulo2[sx];
		sy = 0 + tableStepNumber;//YSIZE-1;//tableStepNumber;// + modulo2[sx];
		position = ((YSIZE/tableStepSizeY)-1)*xCycles+i;//tableStepNumber+sx-(XSIZE*sy);

		//********* INITIALIZE INTERLACED CALCUTATIONS
		//starting depth to look for height colission
		tz = 1;

		while(tz < renderingDepth && sy <YSIZE)
		{
			//***** set mipmap level
			//mipLevel = tz/8;

			// set x,y pooositions on source maps
			mx = (px + rayCastX[sx][tz]);
			my = (py + tz);

				th = heightMap0[ mx ][ my ];
			//*********** HEIGHT MIPMAP
		/*	if(mipLevel < 2 && tableStepSizeY == 1) th = heightMap0[ mx ][ my ];
			else if(mipLevel == 2 || tableStepSizeY == 2)th = heightMap1[ mx/2 ][ my/2 ];
			else if(mipLevel == 3 || tableStepSizeY == 3)th = heightMap2[ mx/4 ][ my/4 ];
			else if(mipLevel == 4 || tableStepSizeY == 4)th = heightMap3[ mx/8 ][ my/8 ];
			else if(mipLevel > 4)th = heightMap4[ mx/16 ][ my/16 ];*/
			//*********** HEIGHT MIPMAP

			//height to look for at a given x,y terrain coordinate accounting for z depth
			//************************************************************


			if(th>ph + rayCastY[sy][tz])
			{
				//	screenDepth[position] = tz;
				//screenMid[position] = th/16 + sy/8;
				screen[position] = colorMap0[ mx ][ my ];
				//screenMid[position] = colorMap0[ mx ][ my ];
				//*************** COLOR MIPMAP
		/*		if(mipLevel == 0 && tableStepSizeY == 1)			color = colorMap0[ mx ][ my ];
				else if(mipLevel == 1 || tableStepSizeY == 2)color = colorMap1[ mx/2 ][ my/2 ] + 1;
				else if(mipLevel == 2 || tableStepSizeY == 3)color = colorMap2[ mx/4 ][ my/4 ] + 2;
				else if(mipLevel == 3 || tableStepSizeY == 4)color = colorMap3[ mx/8 ][ my/8 ] + 3;
				else if(mipLevel > 3)	color = colorMap4[ mx/16 ][ my/16 ] + 4;
				screen[position] = color;*/
				//*************** COLOR MIPMAP

				sy+=tableStepSizeY; //move X pixels to the top in calculations
				position-=currentScreenYStepSize; //move X pixels to the top on the destination screen
			}
			else
			{
				tz+=1;//+mipLevel; //move a variable step in depth to look for next height colision
			}
		}
		//finish vertical line with SKY
		while(sy < YSIZE)
		{
			color = sy/4;//2
			//	screenDepth[position] = 0x01;
			screen[position] = color;
			sy+=tableStepSizeY;
			position-=currentScreenYStepSize;
		}
		//go to the next vertical line
		sx += tableStepSizeX;
	}
}
void ProcessRayCastsSlow(UBYTE *screen, WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH],
UBYTE px, UBYTE py, UBYTE ph, UBYTE tableXStart,
UBYTE tableStepSizeX, UBYTE tableStepSizeY, UBYTE tableStepNumber, UBYTE xCycles)
{
	UBYTE sx,sy,tz;
	UBYTE th = 0;
	UWORD position;
	UBYTE mx,my;
	//UBYTE color = 0;
	UWORD mapValue;
	BYTE xvalue,yvalue;
	UWORD rayValue;
	UWORD startPosition = ((YSIZE/tableStepSizeY)-1)*xCycles;

	//UWORD currentScreenYStepSize;

	sx = tableXStart;
	//currentScreenYStepSize = xCycles;

	for(UBYTE i=0;i<xCycles;i++)
	{
		sy = 0;// + tableStepNumber;
		position = startPosition+i;
		tz = 1;

		while(tz < renderingDepth && sy <YSIZE)
		{
			rayValue = rayCastXY[(sx<<10)+(sy<<5)+tz];
			xvalue = rayValue>>8;
			yvalue = rayValue;

			mx = (px + xvalue);
			my = (py + tz);
			mapValue = map[ mx ][ my ];//read color + height
			th = mapValue;//take just the height
			if(th>ph + yvalue)
			{
				screen[position] = mapValue >> 8;
				sy+=1;
				position-=xCycles;
			}
			else
			{
				tz+=1;
			}
		}
		//finish vertical line with SKY
		while(sy < YSIZE)
		{
			if(screen[position] == 31)
				sy = YSIZE;
			else
			{
				screen[position] = 31;
				sy+=1;//tableStepSizeY;
				position-=xCycles;
			}

		}
		//go to the next vertical line
		sx += 1;//tableStepSizeX;
	}
}
void ProcessRayCastsSlow2(UBYTE *screen, WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH],
UBYTE px, UBYTE py, UBYTE ph, UBYTE tableXStart,
UBYTE tableStepSizeX, UBYTE tableStepSizeY, UBYTE tableStepNumber, UBYTE xCycles)
{
	UBYTE sx,sy,tz;
	UBYTE th = 0;
	UWORD mapValue;
	UWORD rayCast;
	UWORD position;
	UWORD positionX, positionY;
	UBYTE mx,my;
	UBYTE color = 0;
	UWORD currentScreenYStepSize;
	UWORD ppx;
	UWORD sizeY = YSIZE/tableStepSizeY-1;

	ppx = px/4;

	sx = tableXStart;
	currentScreenYStepSize = xCycles;

	for(UBYTE i=0;i<xCycles;i++)
	{
		sy = 0 + tableStepNumber;
		position = sizeY*xCycles+i;
		positionY = 0;
		positionX = i*TERRAINDEPTH;
		tz = 0;

		while(tz < renderingDepth && sy <YSIZE)
		{
			//rayCast = rayCastXX[rayPosition];
			mx = ppx + rayCastXX[positionX] ;
			my = (py + tz)/4;
			mapValue = map[ mx ][ my ];//read color + height
			th = mapValue;//take just the height
			if(th>ph + rayCastYY[positionY])
			{
				screen[position] = mapValue >> 8;
				sy+=tableStepSizeY; //move X pixels to the top in calculations
				position-=currentScreenYStepSize; //move X pixels to the top on the destination screen
				positionY += TERRAINDEPTH;//increment Y
			}
			else
			{
				tz+=1;//+mipLevel; //move a variable step in depth to look for next height colision
				positionY++;//increment only Z
				positionX++;
			}
		}
		//finish vertical line with SKY
		while(sy < YSIZE)
		{
			if(screen[position] == 31) sy = YSIZE;
			else
			{
				screen[position] = 31;
				sy+=tableStepSizeY;
				position-=currentScreenYStepSize;
			}

		}
		//go to the next vertical line
		sx += tableStepSizeX;
	}
}
static inline WORD mul(BYTE a, BYTE b)
{
	WORD c=0;
	if(b & 0b00000001) c+=a;
	if(b & 0b00000010) c+=a<<1;
	if(b & 0b00000100) c+=a<<2;
	if(b & 0b00001000) c+=a<<3;
	if(b & 0b00010000) c+=a<<4;
	if(b & 0b00100000) c+=a<<5;
	return c;
}
void ProcessRayCastsSlow3(UBYTE *screen, WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH],
UBYTE px, UBYTE py, UBYTE ph, UBYTE tableXStart,
UBYTE tableStepSizeX, UBYTE tableStepSizeY, UBYTE tableStepNumber, UBYTE xCycles)
{
	UBYTE sx,sy,tz;
	UBYTE th = 0;
	UWORD mapValue;
	UWORD rayCast;
	UWORD position;
	UWORD positionX, positionY;
	UBYTE mx,my;
	UBYTE color = 0;
	UWORD currentScreenYStepSize;
	UWORD ppx;
	UWORD sizeY = YSIZE/tableStepSizeY-1;
	UBYTE hDifference = 0;

	ppx = px/4;

	sx = tableXStart;
	currentScreenYStepSize = xCycles;

	for(UBYTE i=0;i<xCycles;i++)
	{
		sy = 0 + tableStepNumber;
		position = ((YSIZE/tableStepSizeY)-1)*xCycles+i;
		tz = 1;

		while(tz < renderingDepth && sy <YSIZE)
		{
			mx = (px + ((sx-20)*tz)/8);
			my = (py + tz);
			th = heightMap0[ mx ][ my ];
			if(th>ph +  ((sy-16)*tz)/8)
			{
				screen[position] = colorMap0[ mx ][ my ];
				sy+=1; //move X pixels to the top in calculations
				position-=currentScreenYStepSize; //move X pixels to the top on the destination screen
			}
			else
			{
				tz+=1;//+mipLevel; //move a variable step in depth to look for next height colision
			}
		}
		//finish vertical line with SKY
		while(sy < YSIZE)
		{
			if(screen[position] == 31) sy = YSIZE;
			else
			{
				screen[position] = 31;
				sy+=tableStepSizeY;
				position-=currentScreenYStepSize;
			}

		}
		//go to the next vertical line
		sx += 1;
	}
}
void ProcessRayCastsSlowMul(UBYTE *screen, WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH],
UBYTE px, UBYTE py, UBYTE ph, UBYTE tableXStart,
UBYTE tableStepSizeX, UBYTE tableStepSizeY, UBYTE tableStepNumber, UBYTE xCycles)
{
	UBYTE sx,sy,tz;
	UBYTE th = 0;
	UWORD mapValue;
	UWORD rayCast;
	UWORD position;
	UWORD positionX, positionY;
	UBYTE mx,my;
	UBYTE color = 0;
	UWORD currentScreenYStepSize;
	UWORD ppx;
	UWORD sizeY = YSIZE/tableStepSizeY-1;
	UBYTE hDifference = 0;

	ppx = px/4;

	sx = tableXStart;
	currentScreenYStepSize = xCycles;

	for(UBYTE i=0;i<xCycles;i++)
	{
		sy = 0 + tableStepNumber;
		position = ((YSIZE/tableStepSizeY)-1)*xCycles+i;
		tz = 1;

		while(tz < renderingDepth && sy <YSIZE)
		{
			mx = (px + (mul(sx-20,tz))/8);
			my = (py + tz);
			th = heightMap0[ mx ][ my ];
			if(th>ph +  (mul(sy-16,tz))/8)
			{
				screen[position] = colorMap0[ mx ][ my ];
				sy+=1; //move X pixels to the top in calculations
				position-=currentScreenYStepSize; //move X pixels to the top on the destination screen
			}
			else
			{
				tz+=1;//+mipLevel; //move a variable step in depth to look for next height colision
			}
		}
		//finish vertical line with SKY
		while(sy < YSIZE)
		{
			if(screen[position] == 31) sy = YSIZE;
			else
			{
				screen[position] = 31;
				sy+=tableStepSizeY;
				position-=currentScreenYStepSize;
			}

		}
		//go to the next vertical line
		sx += 1;
	}
}


void ProcessRayCastsWithMipMaps(UBYTE *screen, WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH],
UBYTE px, UBYTE py, UBYTE ph,
	UBYTE tableXStart,	UBYTE tableStepSizeX, UBYTE tableStepSizeY, UBYTE tableStepNumber, UBYTE xCycles)
{
	UBYTE sx,sy,tz;
	UBYTE th = 0;
	UWORD position;
	UBYTE mx,my;
	UBYTE mipLevel;
	UBYTE color = 0;
	UWORD currentScreenYStepSize;
	UBYTE resolutionMip;

	sx = tableXStart;//set the x position in the raycast table
	currentScreenYStepSize = xCycles; //the same as the numer of x steps

	if(tableStepSizeY == 1)	resolutionMip = 0;
	else if (tableStepSizeY == 2)	resolutionMip = 1;
	else if (tableStepSizeY == 4)	resolutionMip = 2;

	for(UBYTE i=0;i<xCycles;i++)
	{
		//********* INITIALIZE INTERLACED CALCUTATIONS
		//sy = interlace/2 + modulo2[sx];
		sy = 0 + tableStepNumber;//YSIZE-1;//tableStepNumber;// + modulo2[sx];
		position = ((YSIZE/tableStepSizeY)-1)*xCycles+i;//tableStepNumber+sx-(XSIZE*sy);

		//********* INITIALIZE INTERLACED CALCUTATIONS
		//starting depth to look for height colission
		tz = 1;

		while(tz < renderingDepth && sy <YSIZE)
		{
			//***** set mipmap level
			mipLevel = tz/8 + resolutionMip;

			// set x,y pooositions on source maps
			mx = (px + rayCastX[sx][tz]);
			my = (py + tz);

			//*********** HEIGHT MIPMAP
			if(mipLevel == 0) th = heightMap0[ mx ][ my ];
			else if(mipLevel == 1)th = heightMap1[ mx/2 ][ my/2 ];
			else if(mipLevel == 2)th = heightMap2[ mx/4 ][ my/4 ];
			else if(mipLevel == 3)th = heightMap3[ mx/8 ][ my/8 ];
			else if(mipLevel > 3)th = heightMap4[ mx/16 ][ my/16 ];
			//*********** HEIGHT MIPMAP

			//height to look for at a given x,y terrain coordinate accounting for z depth
			//************************************************************


			if(th>ph + rayCastY[sy][tz])
			{

				//*************** COLOR MIPMAP
				if(mipLevel == 0)			color = colorMap0[ mx ][ my ];
				else if(mipLevel == 1)color = colorMap1[ mx/2 ][ my/2 ] + 1;
				else if(mipLevel == 2)color = colorMap2[ mx/4 ][ my/4 ] + 2;
				else if(mipLevel == 3)color = colorMap3[ mx/8 ][ my/8 ] + 3;
				else if(mipLevel > 3)	color = colorMap4[ mx/16 ][ my/16 ] + 4;
				screen[position] = color;
				//*************** COLOR MIPMAP

				sy+=tableStepSizeY; //move X pixels to the top in calculations
				position-=currentScreenYStepSize; //move X pixels to the top on the destination screen
			}
			else
			{
				tz+=1;//+mipLevel; //move a variable step in depth to look for next height colision
			}
		}
		//finish vertical line with SKY
		while(sy < YSIZE)
		{
			color = sy/4;//2
			//	screenDepth[position] = 0x01;
			screen[position] = color;
			sy+=tableStepSizeY;
			position-=currentScreenYStepSize;
		}
		//go to the next vertical line
		sx += tableStepSizeX;
	}
}
//calculate paths for raycasts going from the camera
void CalculateRayCasts(WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH], UBYTE xSize, UBYTE ySize)
{
	int sxx;
	int syy;
	int tzz; //depth step value
	int fovX = xSize/20; //this changes the field of view
	int fovY = 8;

	tzz = 1;
	for(int tz=0;tz<TERRAINDEPTH;tz++)
	{
		tzz += 1+tz/16; //increase step with the distance from camera
		for(int sx=-xSize/2;sx<xSize/2;sx++)
		{
			sxx = sx * tzz/2; //make smaller steps
			for(int sy=-ySize/2;sy<ySize/2;sy++)
			{
				syy = sy * tzz/2;//make smaller steps
				rayCastX[xSize/2+sx][tz] = sxx/fovX;
				rayCastY[ySize/2+sy][tz] = syy/fovY;
			}
		}
	}
}
void CalculateRayCastsSingle(UBYTE xSize, UBYTE ySize)
{
	int sxx;
	int syy;
	int tzz; //depth step value
	int fovX = 8;//xSize/20; //this changes the field of view
	int fovY = 8;
	BYTE sxxx, syyy;
	UBYTE xa,ya,ta;
	UWORD address;

	tzz = 1;
	for(UBYTE tz=0;tz<TERRAINDEPTH;tz++)
	{



		tzz += 1+tz/16; //increase step with the distance from camera
		for(int sx=-xSize/2;sx<xSize/2;sx++)
		{
			sxx = sx * tzz/2; //make smaller steps
sxx = sxx/fovX;

			for(int sy=-ySize/2;sy<ySize/2;sy++)
			{
				syy = sy * tzz/2;//make smaller steps

				syy = syy/fovY;
			//	sxxx = sxx;
				//syyy = syy;

				if(sxx<-127) sxx = -127;
				if(sxx>127) sxx = 127;
				//sxx += 128;
				sxxx = sxx;

				if(syy<-127) syy = -127;
				if(syy>127) syy = 127;
				//syy += 128;
				syyy = syy;

				//rayCastXY[xSize/2+sx][ySize/2+sy][tz] = (sxx<<8) + syy;

				xa = xSize/2+sx;
				ya = ySize/2+sy;
				ta = tz;

				address = (xa<<10) + (ya<<5) + ta;
				rayCastXY[address] = (sxxx<<8) + syyy;
			//	rayCastXY[xSize/2+sx][ySize/2+sy][tz][0] = sxx;//
			//	rayCastXY[xSize/2+sx][ySize/2+sy][tz][1] = syy;
			//	rayCastXY[xSize/2+sx][ySize/2+sy][tz] = (sxxx<<8) + syyy;
			}
		}

	}
}

void CalculateRayCastsSlow(WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH])
{
	UWORD position = 0;

	for(int y=0;y<32;y++)
	{
		for(int tz=0;tz<TERRAINDEPTH;tz++)
		{
			rayCastYY[position] = rayCastY[y*4][tz];
			position ++;
		}
	}

	position = 0;
	for(int x=0;x<40;x++)
	{
		for(int tz=0;tz<TERRAINDEPTH;tz++)
		{
			rayCastXX[position] = rayCastX[x*4][tz]/4;
			position ++;
		}
	}
}
void CombineMaps(UBYTE (*height)[64], UBYTE (*color)[64], UWORD (*map)[256])
{
	for (int x = 0; x < 256; x++) {
		for (int y = 0; y < 256; y++) {
			map[x][y] = (color[x/4][y/4] << 8) + height[x/4][y/4];
		}
	}
}
