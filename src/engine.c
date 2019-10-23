#include "engine.h"

/*
docker run --rm \
-v ${PWD}:/work \
-v /path/to/extra/m68k-amigaos/lib:/tools/usr/lib \
-v /path/to/extra/m68k-amigaos/include:/tools/usr/include \
-it amigadev/crosstools:m68k-amigaos bash
*/
void CopyFastToChipW(tBitMap *bm)
{
	CopyMemQuick(fastPlane1W, bm->Planes[0], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane2W, bm->Planes[1], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane3W, bm->Planes[2], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane4W, bm->Planes[3], PLANEWIDTH*HEIGHT);
}
void CalculateModulo2()
{
	for(UBYTE sx=0;sx<XSIZE;sx++)
	{
		modulo2[sx] = sx % 2;
	}
}
//calculate paths for raycasts going from the camera
void CalculateRayCasts()
{
	int sxx;
	int syy;
	int tzz; //depth step value
	int fov = 8; //this changes the field of view

	tzz = 1;
	for(int tz=1;tz<TERRAINDEPTH;tz++)
	{
		tzz += 1+tz/8; //increase step with the distance from camera, less quality but better performance
		for(int sx=-XSIZE/2;sx<XSIZE/2;sx++)
		{
			sxx = sx * tzz/2; //make smaller steps
			for(int sy=-YSIZE/2;sy<YSIZE/2;sy++)
			{
				syy = sy * tzz/2;//make smaller steps
				rayCastX[XSIZE/2+sx][tz] = sxx/fov;
				rayCastY[YSIZE/2+sy][tz] = syy/fov;
			}
		}
	}
}
//translates calculated view from chunky to planar
//writes a WORD made up of 2 BYTEs each made up of 3 pixels
//translates 32 colors into 16 dithered using precalculated bit sets
void DrawPlayerScreen(UBYTE player, UBYTE depth, UBYTE even)
{
	UWORD sp,position;
	UWORD address1, address2;
	UBYTE *screen;
	UWORD startOffset;
	UWORD evenOffset;
	UBYTE *dither1, *dither2, *dither3, *dither4;

	sp = 0;

	//interlace init
	if(even == 1)
	{
		dither1 = colorByteDitherP1EvenHigh;
		dither2 = colorByteDitherP2EvenHigh;
		dither3 = colorByteDitherP3EvenHigh;
		dither4 = colorByteDitherP4EvenHigh;
		evenOffset = 0;
	}
	else
	{
		dither1 = colorByteDitherP1OddHigh;
		dither2 = colorByteDitherP2OddHigh;
		dither3 = colorByteDitherP3OddHigh;
		dither4 = colorByteDitherP4OddHigh;
		evenOffset = 20;
	}

	//player init
	if(player == 1)
	{
		//color or depth map
		if(depth == 1)
		{
			screen = screenP1depth;
		}
		else
		{
			screen = screenP1;
		}

		startOffset = 0;
	}
	else
	{
		//color or depth map
		if(depth == 1)
		{
			screen = screenP2depth;
		}
		else
		{
			screen = screenP2;
		}
		startOffset = 10;
	}


	//for each line
	for(UWORD y=0;y<YSIZE;y++)
	{
		position = y*32 + y*8 + evenOffset + startOffset;

		//draw the line with WORDs made up of 2 BYTEs each consisting 3 pixels
		for(UWORD x=0;x<XSIZE/6;x++)
		{
			//calculate locations of BYTE values to fetch
			address1 = (screen[sp]<<10) + (screen[sp+1]<<5) + (screen[sp+2]);
			address2 = (screen[sp+3]<<10) + (screen[sp+4]<<5) + (screen[sp+5]);

			//fetch propper BYTEs and write with WORDs to plane buffers
			fastPlane1W[position] = (dither1[ address1 ]<<8) + dither1[ address2 ];
			fastPlane2W[position] = (dither2[ address1 ]<<8) + dither2[ address2 ];
			fastPlane3W[position] = (dither3[ address1 ]<<8) + dither3[ address2 ];
			fastPlane4W[position] = (dither4[ address1 ]<<8) + dither4[ address2 ];

			position++; //go to next WORD

			sp+=6;//move by 6 pixels
		}
	}
}

void DrawPlayerScreen3x2(UBYTE player, UBYTE depth, UBYTE even, UBYTE startTable, UBYTE startScreen, UBYTE length)
{
	UWORD sp,position;
	UWORD address1, address2;
	UBYTE *screen;
//	UWORD startOffset;
	UWORD evenOffset;
	UBYTE *dither1, *dither2, *dither3, *dither4;



	//interlace init
	if(even == 1)
	{
		dither1 = colorByteDitherP1EvenHigh;
		dither2 = colorByteDitherP2EvenHigh;
		dither3 = colorByteDitherP3EvenHigh;
		dither4 = colorByteDitherP4EvenHigh;
		evenOffset = 0;
	}
	else
	{
		dither1 = colorByteDitherP1OddHigh;
		dither2 = colorByteDitherP2OddHigh;
		dither3 = colorByteDitherP3OddHigh;
		dither4 = colorByteDitherP4OddHigh;
		evenOffset = 20;
	}

	//player init
	if(player == 1)
	{
		//color or depth map
		if(depth == 1)
		{
			screen = screenP1depth;
		}
		else
		{
			screen = screenP1;
		}

		//startOffset = 0;
	}
	else
	{
		//color or depth map
		if(depth == 1)
		{
			screen = screenP2depth;
		}
		else
		{
			screen = screenP2;
		}
	//	startOffset = 10;
	}


	//for each line
	for(UWORD y=0;y<YSIZE;y++)
	{
		//2 * 20 bytes * y + even/odd + player screen offset WORDs
		position = y*32 + y*8 + evenOffset  + startScreen;

		//screen position
		sp = y*XSIZE + startTable;

		//draw the line with WORDs made up of 2 BYTEs each consisting 3 pixels
		for(UWORD x=0;x<length/6;x++)
		{
			//calculate locations of BYTE values to fetch
			address1 = (screen[sp]<<10) + (screen[sp+1]<<5) + (screen[sp+2]);
			address2 = (screen[sp+3]<<10) + (screen[sp+4]<<5) + (screen[sp+5]);

			//fetch propper BYTEs and write with WORDs to plane buffers
			fastPlane1W[position] = (dither1[ address1 ]<<8) + dither1[ address2 ];
			fastPlane2W[position] = (dither2[ address1 ]<<8) + dither2[ address2 ];
			fastPlane3W[position] = (dither3[ address1 ]<<8) + dither3[ address2 ];
			fastPlane4W[position] = (dither4[ address1 ]<<8) + dither4[ address2 ];

			position++; //go to next WORD

			sp+=6;//move by 6 pixels
		}
	}
}
void DrawPlayerScreen4x4(UBYTE player, UBYTE depth, UBYTE startTable, UBYTE startScreen, UBYTE length)
{
	UWORD sp,position,blockPosition;
	UWORD address1, address2;
	UBYTE *screen;
//	UWORD startOffset;
	//UWORD evenOffset;
	UBYTE *dither1, *dither2, *dither3, *dither4;
	UBYTE pixelSize = 4;
	//	UWORD yStep;

	UWORD w1,w2,w3,w4;

	//player init
	if(player == 1)
	{
		//color or depth map
		if(depth == 1)
		{
			screen = screenP1depth;
		}
		else
		{
			screen = screenP1;
		}

	//	startOffset = 0;
	}
	else
	{
		//color or depth map
		if(depth == 1)
		{
			screen = screenP2depth;
		}
		else
		{
			screen = screenP2;
		}
	//	startOffset = 10;
	}

	dither1 = colorByteDitherP1EvenHigh;
	dither2 = colorByteDitherP2EvenHigh;
	dither3 = colorByteDitherP3EvenHigh;
	dither4 = colorByteDitherP4EvenHigh;

	//	yStep = XSIZE*2;

	//for each line
	for(UWORD y=0;y<YSIZE/2;y++)
	{
		//40 bytes * y + even/odd + player screen offset WORDs
		//position = y*20*pixelSize  + startScreen + debugValue;

		position = y*80  + startScreen;
		//screen position
		//sp = y*yStep + startTable  + 1*XSIZE ;
		sp = y*240 + startTable  + 120 ;

		//draw the line with WORDs made up of 2 BYTEs
		for(UWORD x=0;x<length;x++)
		{
			address1 = (screen[sp]<<10) + (screen[sp+2]);
			address2 = (screen[sp+4]<<10) + (screen[sp+6]);

			w1 = (dither1[ address1 ]<<8) + dither1[ address2 ];
			w2 = (dither2[ address1 ]<<8) + dither2[ address2 ];
			w3 = (dither3[ address1 ]<<8) + dither3[ address2 ];
			w4 = (dither4[ address1 ]<<8) + dither4[ address2 ];
			//calculate locations of BYTE values to fetch
			for(UBYTE e=0;e<pixelSize;e++)
			{
				blockPosition = position+e*20;

				fastPlane1W[blockPosition] = w1;
				fastPlane2W[blockPosition] = w2;
				fastPlane3W[blockPosition] = w3;
				fastPlane4W[blockPosition] = w4;
			}


			position++; //go to next WORD

			sp+=8;
		}
	}//line
}
void DrawPlayerScreen8x8(UBYTE *screen, UBYTE player, UBYTE depth, UBYTE startScreen, UBYTE xCycles)
{
	UWORD sp,position,blockPosition;
	UWORD address1, address2;
	UBYTE *dither1, *dither2, *dither3, *dither4;
	UBYTE *dither5, *dither6, *dither7, *dither8;
	UWORD w1,w2,w3,w4,w5,w6,w7,w8;

	dither1 = colorByteDither8x8EvenP1;
	dither2 = colorByteDither8x8EvenP2;
	dither3 = colorByteDither8x8EvenP3;
	dither4 = colorByteDither8x8EvenP4;

	dither5 = colorByteDither8x8OddP1;
	dither6 = colorByteDither8x8OddP2;
	dither7 = colorByteDither8x8OddP3;
	dither8 = colorByteDither8x8OddP4;

	sp = 0;//screen position
	if(player == 1)
	{

	}
	else
	{

	}
	if(depth == 1)
	{

	}
	else
	{

	}

	//for each line
	for(UBYTE y=0;y<32;y++)
	{
		//40 bytes * y + even/odd + player screen offset WORDs
		position = y*20*8  + startScreen;
		//draw the line with WORDs made up of 2 BYTEs each consisting 3 pixels
		for(UBYTE x=0;x<xCycles;x++)
		{
			address1 = (screen[sp]);
			address2 = (screen[sp+1]);

			w1 = (dither1[ address1 ]<<8) + dither1[ address2 ];
			w2 = (dither2[ address1 ]<<8) + dither2[ address2 ];
			w3 = (dither3[ address1 ]<<8) + dither3[ address2 ];
			w4 = (dither4[ address1 ]<<8) + dither4[ address2 ];

			w5 = (dither5[ address1 ]<<8) + dither5[ address2 ];
			w6 = (dither6[ address1 ]<<8) + dither6[ address2 ];
			w7 = (dither7[ address1 ]<<8) + dither7[ address2 ];
			w8 = (dither8[ address1 ]<<8) + dither8[ address2 ];

			blockPosition=position;fastPlane1W[blockPosition]=w1;fastPlane2W[blockPosition]=w2;fastPlane3W[blockPosition]=w3;fastPlane4W[blockPosition]=w4;
			blockPosition=position+20;fastPlane1W[blockPosition]=w5;fastPlane2W[blockPosition]=w6;fastPlane3W[blockPosition]=w7;fastPlane4W[blockPosition]=w8;
			blockPosition=position+40;fastPlane1W[blockPosition]=w1;fastPlane2W[blockPosition]=w2;fastPlane3W[blockPosition]=w3;fastPlane4W[blockPosition]=w4;
			blockPosition=position+60;fastPlane1W[blockPosition]=w5;fastPlane2W[blockPosition]=w6;fastPlane3W[blockPosition]=w7;fastPlane4W[blockPosition]=w8;
			blockPosition=position+80;fastPlane1W[blockPosition]=w1;fastPlane2W[blockPosition]=w2;fastPlane3W[blockPosition]=w3;fastPlane4W[blockPosition]=w4;
			blockPosition=position+100;fastPlane1W[blockPosition]=w5;fastPlane2W[blockPosition]=w6;fastPlane3W[blockPosition]=w7;fastPlane4W[blockPosition]=w8;
			blockPosition=position+120;fastPlane1W[blockPosition]=w1;fastPlane2W[blockPosition]=w2;fastPlane3W[blockPosition]=w3;fastPlane4W[blockPosition]=w4;
			blockPosition=position+140;fastPlane1W[blockPosition]=w5;fastPlane2W[blockPosition]=w6;fastPlane3W[blockPosition]=w7;fastPlane4W[blockPosition]=w8;

			position++; //go to next WORD
			sp+=2; //go to the next 2 points
		}
	}//line
}
/*
void DrawPlayerScreenPartialResolution(UBYTE player, UBYTE depth, UBYTE even, UBYTE start, UBYTE length)
{
UWORD sp,position,blockPosition;
UWORD word;
UWORD address1, address2;
UBYTE *screen;
UWORD startOffset;
UWORD evenOffset;
UBYTE *dither1, *dither2, *dither3, *dither4;
UBYTE pa,pb,pc,pd,pe,pf;
UWORD yStep;

//player init
if(player == 1)
{
//color or depth map
if(depth == 1)
{
screen = screenP1depth;
}
else
{
screen = screenP1;
}

startOffset = 0;
}
else
{
//color or depth map
if(depth == 1)
{
screen = screenP2depth;
}
else
{
screen = screenP2;
}
startOffset = 10;
}

yStep = XSIZE*4;

//for each line
for(UWORD y=0;y<YSIZE;y++)
{
//40 bytes * y + even/odd + player screen offset WORDs
position = y*20*8  + start;

//screen position
sp = y*yStep + start + p2y*XSIZE + p2x;

//draw the line with WORDs made up of 2 BYTEs each consisting 3 pixels
for(UWORD x=0;x<length;x++)
{
pa = screen[sp]/4;
pb = screen[sp+4]/4;
pc = screen[sp+yStep]/4;
pd = screen[sp+yStep+4]/4;
pe = screen[sp+8]/4;
pf = screen[sp+yStep+8]/4;
//calculate locations of BYTE values to fetch
for(UBYTE e=0;e<8;e++)
{
blockPosition = position+e*20;
address1 = (e<<12) + (pa<<9) + (pb<<6) + (pc<<3) + pd;
address2 = (e<<12) + (pb<<9) + (pe<<6) + (pd<<3) + pf;

//fetch propper BYTEs and write with WORDs to plane buffers
fastPlane1W[blockPosition] = (colorByteDither8x8P1[ address1 ]<<8) + colorByteDither8x8P1[ address2 ];
fastPlane2W[blockPosition] = (colorByteDither8x8P2[ address1 ]<<8) + colorByteDither8x8P2[ address2 ];
fastPlane3W[blockPosition] = (colorByteDither8x8P3[ address1 ]<<8) + colorByteDither8x8P3[ address2 ];
fastPlane4W[blockPosition] = (colorByteDither8x8P4[ address1 ]<<8) + colorByteDither8x8P4[ address2 ];
}


position++; //go to next WORD

sp+=8;//move by 6 pixels
}
}//line
}*/
/*
* tableXStart - which point on the X line should the routine start at
* tableStepSize - how many points should we move by with each vertical pass
* tableStepNumber - which step for interleaved rendering
* xCycles - how many x cycles to process
*/
void ProcessRayCasts(UBYTE *screen, UBYTE px, UBYTE py, UBYTE ph,
	UBYTE tableXStart,	UBYTE tableStepSizeX, UBYTE tableStepSizeY, UBYTE tableStepNumber, UBYTE xCycles)
{
	UBYTE sx,sy,tz;
	UBYTE th = 0;
	UWORD position;
	UBYTE mx,my;
	UBYTE mipLevel;
	UBYTE color = 0;
	//UWORD currentTableYStepSize;
	UWORD currentScreenYStepSize;

	sx = tableXStart;//set the x position in the raycast table
	//set the start position (left bottom pixel) on the destination screen
	//screenStartPosition = (YSIZE-1)*xCycles;
	//currentTableYStepSize = tableStepSizeY*XSIZE;
	currentScreenYStepSize = xCycles; //the same as the numer of x steps
	//screenYStepSize = stepSize*XSIZE;


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
			mipLevel = tz/8;

			// set x,y pooositions on source maps
			mx = (px + rayCastX[sx][tz]);
			my = (py + tz);

			//	th = heightMap0[ mx ][ my ];
			//*********** HEIGHT MIPMAP
			if(mipLevel < 2 && tableStepSizeY == 1) th = heightMap0[ mx ][ my ];
			else if(mipLevel == 2 || tableStepSizeY == 2)th = heightMap1[ mx/2 ][ my/2 ];
			else if(mipLevel == 3 || tableStepSizeY == 3)th = heightMap2[ mx/4 ][ my/4 ];
			else if(mipLevel == 4 || tableStepSizeY == 4)th = heightMap3[ mx/8 ][ my/8 ];
			else if(mipLevel > 4)th = heightMap4[ mx/16 ][ my/16 ];
			//*********** HEIGHT MIPMAP

			//height to look for at a given x,y terrain coordinate accounting for z depth
			//************************************************************


			if(th>ph + rayCastY[sy][tz])
			{
				//	screenDepth[position] = tz;
				//screenMid[position] = th/16 + sy/8;
				//	screen[position] = colorMap0[ mx ][ my ];
				//screenMid[position] = colorMap0[ mx ][ my ];
				//*************** COLOR MIPMAP
				if(mipLevel == 0 && tableStepSizeY == 1)			color = colorMap0[ mx ][ my ];
				else if(mipLevel == 1 || tableStepSizeY == 2)color = colorMap1[ mx/2 ][ my/2 ] + 1;
				else if(mipLevel == 2 || tableStepSizeY == 3)color = colorMap2[ mx/4 ][ my/4 ] + 2;
				else if(mipLevel == 3 || tableStepSizeY == 4)color = colorMap3[ mx/8 ][ my/8 ] + 3;
				else if(mipLevel > 3)	color = colorMap4[ mx/16 ][ my/16 ] + 4;
				//*************** COLOR MIPMAP
				screen[position] = color;
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
			sy+=tableStepSizeY;
			color = sy/4-1;//2
			//	screenDepth[position] = 0x01;
			screen[position] = color;
			position-=currentScreenYStepSize;
		}
		//go to the next vertical line
		sx += tableStepSizeX;
	}
}
