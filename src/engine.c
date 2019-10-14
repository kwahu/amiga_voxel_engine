#include "engine.h"
#include "file_read.c"
#include "mipmaps.c"
#include "bitmap_filters.c"
#include "dithering.c"
#include "draw_ships.c"
#include "draw_maps.c"
/*
docker run --rm \
-v ${PWD}:/work \
-v /path/to/extra/m68k-amigaos/lib:/tools/usr/lib \
-v /path/to/extra/m68k-amigaos/include:/tools/usr/include \
-it amigadev/crosstools:m68k-amigaos bash
*/
static void CopyFastToChipW(tBitMap *bm)
{
	CopyMemQuick(fastPlane1W, bm->Planes[0], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane2W, bm->Planes[1], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane3W, bm->Planes[2], PLANEWIDTH*HEIGHT);
	CopyMemQuick(fastPlane4W, bm->Planes[3], PLANEWIDTH*HEIGHT);
}
static void CalculateModulo2()
{
	for(UBYTE sx=0;sx<XSIZE;sx++)
	{
		modulo2[sx] = sx % 2;
	}
}
//calculate paths for raycasts going from the camera
static void CalculateRayCasts()
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
static void DrawPlayerScreen(UBYTE player, UBYTE depth, UBYTE even)
{
	UWORD sp,p1,p2,p3,p4;
	UWORD word;
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
		p2 = y*32 + y*8 + evenOffset + startOffset;

		//draw the line with WORDs made up of 2 BYTEs each consisting 3 pixels
		for(UWORD x=0;x<XSIZE/6;x++)
		{
			//calculate locations of BYTE values to fetch
			address1 = (screen[sp]<<10) + (screen[sp+1]<<5) + (screen[sp+2]);
			address2 = (screen[sp+3]<<10) + (screen[sp+4]<<5) + (screen[sp+5]);

			//fetch propper BYTEs and write with WORDs to plane buffers
			fastPlane1W[p2] = (dither1[ address1 ]<<8) + dither1[ address2 ];
			fastPlane2W[p2] = (dither2[ address1 ]<<8) + dither2[ address2 ];
			fastPlane3W[p2] = (dither3[ address1 ]<<8) + dither3[ address2 ];
			fastPlane4W[p2] = (dither4[ address1 ]<<8) + dither4[ address2 ];

			p2++; //go to next WORD

			sp+=6;//move by 6 pixels
		}
	}
}

static void DrawPlayerScreen3x2(UBYTE player, UBYTE depth, UBYTE even, UBYTE startTable, UBYTE startScreen, UBYTE length)
{
	UWORD sp,p1,p2,p3,p4;
	UWORD word;
	UWORD address1, address2;
	UBYTE *screen;
	UWORD startOffset;
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
		//2 * 20 bytes * y + even/odd + player screen offset WORDs
		p2 = y*32 + y*8 + evenOffset + startOffset + startScreen;

		//screen position
		sp = y*XSIZE + startTable;

		//draw the line with WORDs made up of 2 BYTEs each consisting 3 pixels
		for(UWORD x=0;x<length/6;x++)
		{
			//calculate locations of BYTE values to fetch
			address1 = (screen[sp]<<10) + (screen[sp+1]<<5) + (screen[sp+2]);
			address2 = (screen[sp+3]<<10) + (screen[sp+4]<<5) + (screen[sp+5]);

			//fetch propper BYTEs and write with WORDs to plane buffers
			fastPlane1W[p2] = (dither1[ address1 ]<<8) + dither1[ address2 ];
			fastPlane2W[p2] = (dither2[ address1 ]<<8) + dither2[ address2 ];
			fastPlane3W[p2] = (dither3[ address1 ]<<8) + dither3[ address2 ];
			fastPlane4W[p2] = (dither4[ address1 ]<<8) + dither4[ address2 ];

			p2++; //go to next WORD

			sp+=6;//move by 6 pixels
		}
	}
}
static void DrawPlayerScreen4x4(UBYTE player, UBYTE depth, UBYTE even, UBYTE startTable, UBYTE startScreen, UBYTE length)
{
	UWORD sp,position,blockPosition;
	UWORD word;
	UWORD address1, address2;
	UBYTE *screen;
	UWORD startOffset;
	UWORD evenOffset;
	UBYTE *dither1, *dither2, *dither3, *dither4;
	UBYTE p1,p2,p3,p4;
	UWORD yStep;
	UBYTE pixelSize = 4;

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

	yStep = XSIZE*2;

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
static void DrawPlayerScreen8x8(UBYTE player, UBYTE depth, UBYTE even, UBYTE startTable, UBYTE startScreen, UBYTE length)
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
		UBYTE pixelSize = 8;
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
	for(UWORD y=0;y<YSIZE/4;y++)
	{
		//40 bytes * y + even/odd + player screen offset WORDs
		position = y*20*8  + startScreen;

		//screen position
		sp = y*yStep + startTable  + 3*XSIZE ;

		//draw the line with WORDs made up of 2 BYTEs each consisting 3 pixels
		for(UWORD x=0;x<length;x++)
		{
			address1 = (screen[sp]);
			address2 = (screen[sp+4]);

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

			sp+=8;//move by 6 pixels
		}
	}//line
}
/*
static void DrawPlayerScreenPartialResolution(UBYTE player, UBYTE depth, UBYTE even, UBYTE start, UBYTE length)
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

static void ProcessRayCasts(UBYTE player, UBYTE stepSize, UBYTE step)
{
	UBYTE sx,sy,tz;
	UWORD px,py,ph; //player
	UBYTE th;
	UWORD position;
	UBYTE mx,my;
	UBYTE mipLevel;
	UWORD startPosition;
	UBYTE *screen;
	UBYTE *screenDepth;
	UWORD ex,ey,eh; //enemy
	UWORD screenYStepSize;
	UBYTE xStep;
	UBYTE xCycles;
	UBYTE color;
	UWORD currentTableStepSize;
	UWORD currentScreenStepSize;
	UWORD currentStep;

	//initialize player data
	if(player == 1)
	{
		ph = p1h;
		px = p1x;
		py = p1y;
		ex = p2x;
		ey = p2y;
		eh = p2h;
		screen = screenP1;
		screenDepth = screenP1depth;
	}
	else
	{
		ph = p2h;
		px = p2x;
		py = p2y;
		ex = p1x;
		ey = p1y;
		eh = p1h;
		screen = screenP2;
		screenDepth = screenP2depth;
	}
	//set the start position (left bottom pixel) on the destination screen

	startPosition = (YSIZE-1)*XSIZE;
	screenYStepSize = stepSize*XSIZE;

	sx = 0;
	//FOVeated view
	for(int i=0;i<5;i++)
	{
		switch(i)
		{
			case 0: sx = 0;xStep = 4;xCycles = 8;currentTableStepSize=4;currentScreenStepSize=4*XSIZE;currentStep=0;break;
			case 1: sx = 118;xStep = 4;xCycles = 8;currentTableStepSize=4;currentScreenStepSize=4*XSIZE;currentStep=0;break;
			case 2: sx = 32;xStep = 2;xCycles = 16;currentTableStepSize=2;currentScreenStepSize=2*XSIZE;currentStep=0;break;
			case 3: sx = 86;xStep = 2;xCycles = 16;currentTableStepSize=2;currentScreenStepSize=2*XSIZE;currentStep=0;break;
			case 4: sx = 64;xStep = 1;xCycles = 23;currentTableStepSize=stepSize;currentScreenStepSize=screenYStepSize;currentStep=step;break;
		}
//xStep = 1;xCycles = 120;currentTableStepSize=1;currentScreenStepSize=1*XSIZE;currentStep=0;
//xStep = 2;xCycles = 60;currentTableStepSize=2;currentScreenStepSize=2*XSIZE;currentStep=0;
//xStep = 4;xCycles = 30;currentTableStepSize=4;currentScreenStepSize=4*XSIZE;currentStep=0;

		for(UBYTE i=0;i<xCycles;i++)
		{
			//********* INITIALIZE INTERLACED CALCUTATIONS

			//sy = interlace/2 + modulo2[sx];
			sy = currentStep;// + modulo2[sx];
			position = startPosition+sx-(XSIZE*sy);


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
				if(mipLevel < 2 && xStep == 1) th = heightMap0[ mx ][ my ];
				else if(mipLevel == 2 || xStep == 2)th = heightMap1[ mx/2 ][ my/2 ];
				else if(mipLevel == 3 || xStep == 3)th = heightMap2[ mx/4 ][ my/4 ];
				else if(mipLevel == 4 || xStep == 4)th = heightMap3[ mx/8 ][ my/8 ];
				else if(mipLevel > 4)th = heightMap4[ mx/16 ][ my/16 ];
				//*********** HEIGHT MIPMAP

				//height to look for at a given x,y terrain coordinate accounting for z depth
				//************************************************************


				if(th>ph + rayCastY[sy][tz])
				{
					screenDepth[position] = tz;
					//screenMid[position] = th/16 + sy/8;
					//	screen[position] = colorMap0[ mx ][ my ];
					//screenMid[position] = colorMap0[ mx ][ my ];
					//*************** COLOR MIPMAP
					if(mipLevel == 0 && xStep == 1)			color = colorMap0[ mx ][ my ];
					else if(mipLevel == 1 || xStep == 2)color = colorMap1[ mx/2 ][ my/2 ] + 1;
					else if(mipLevel == 2 || xStep == 3)color = colorMap2[ mx/4 ][ my/4 ] + 2;
					else if(mipLevel == 3 || xStep == 4)color = colorMap3[ mx/8 ][ my/8 ] + 3;
					else if(mipLevel > 3)	color = colorMap4[ mx/16 ][ my/16 ] + 4;
					//*************** COLOR MIPMAP

					screen[position] = color;
			/*		if(xStep == 1)
					{
						screen[position] = color;
					}
					else if(xStep == 2)
					{
						screen[position] = color;
						//screen[position+1] = color;
					}
					else if(xStep == 4)
					{
						screen[position] = color;
						//screen[position+1] = color;
						//screen[position+2] = color;
						//screen[position+3] = color;
					}*/

					// check shadow

					/*	if(mx < xPos+5 && mx > xPos-5 && my < yPos+5 && my > yPos-5)
					screenMid[position] = screenMid[position]/2;*/

					/*	if( mx>ex-8 && mx<ex+8 && my>ey-4 && my<ey+4)
					{
					screen[position] = screen[position]/2;
				}*/
				// check shadow


				sy+=currentTableStepSize; //move X pixels to the top in calculations

				position-=currentScreenStepSize; //move X pixels to the top on the destination screen
			}
			else
			{
				tz+=1;//+mipLevel; //move a variable step in depth to look for next height colision
			}

		}
		//finish vertical line with SKY
		while(sy < YSIZE)
		{
			sy+=currentTableStepSize;
			color = sy/4;//2
			screenDepth[position] = 0x01;
			screen[position] = color;
		/*	if(xStep == 1)
			{
				screen[position] = color;
			}
			else if(xStep == 2)
			{
				screen[position] = color;
				//screen[position+1] = color;
			}
			else if(xStep == 4)
			{
				screen[position] = color;
				//screen[position+1] = color;
				//screen[position+2] = color;
				//screen[position+3] = color;
			}*/

			position-=currentScreenStepSize;
		}
		//go to the next vertical line
			sx += xStep;
	}
}
}
