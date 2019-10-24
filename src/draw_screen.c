#include "ray_casting.h"

void CopyFastToChipW(tBitMap *bm)
{
	CopyMemQuick(plane1W, bm->Planes[0], PLANEWIDTH*PLANEHEIGHT);
	CopyMemQuick(plane2W, bm->Planes[1], PLANEWIDTH*PLANEHEIGHT);
	CopyMemQuick(plane3W, bm->Planes[2], PLANEWIDTH*PLANEHEIGHT);
	CopyMemQuick(plane4W, bm->Planes[3], PLANEWIDTH*PLANEHEIGHT);
}
/*
void CalculateModulo2()
{
	for(UBYTE sx=0;sx<XSIZE;sx++)
	{
		modulo2[sx] = sx % 2;
	}
}*/

void DrawPlayerScreen3x2(UBYTE *screen, UBYTE player, UBYTE depth, UBYTE startScreen, UBYTE xCycles)
{
	UWORD sp,position,blockPosition;
	UWORD address1, address2;

//	UWORD startOffset;
	//UWORD evenOffset;
	UBYTE *dither1, *dither2, *dither3, *dither4;
	UBYTE *dither5, *dither6, *dither7, *dither8;

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

	//interlace init
/*	if(even == 1)
	{
		dither1 = dither3x2EvenP1;
		dither2 = dither3x2EvenP2;
		dither3 = dither3x2EvenP3;
		dither4 = dither3x2EvenP4;
		evenOffset = 0;
	}
	else
	{
		dither1 = dither3x2OddP1;
		dither2 = dither3x2OddP2;
		dither3 = dither3x2OddP3;
		dither4 = dither3x2OddP4;
		evenOffset = 20;
	}*/

	dither1 = dither3x2EvenP1;
	dither2 = dither3x2EvenP2;
	dither3 = dither3x2EvenP3;
	dither4 = dither3x2EvenP4;

	dither5 = dither3x2OddP1;
	dither6 = dither3x2OddP2;
	dither7 = dither3x2OddP3;
	dither8 = dither3x2OddP4;

sp = 0;

	//for each line
	for(UBYTE y=0;y<128;y++)
	{
		//40 bytes * y + even/odd + player screen offset WORDs
		position = y*20*2  + startScreen;
		//draw the line with WORDs made up of 2 BYTEs each consisting 3 pixels
		for(UBYTE x=0;x<xCycles;x++)
		{
			//calculate locations of BYTE values to fetch
			address1 = (screen[sp]<<10) + (screen[sp+1]<<5) + (screen[sp+2]);
			address2 = (screen[sp+3]<<10) + (screen[sp+4]<<5) + (screen[sp+5]);

			//fetch propper BYTEs and write with WORDs to plane buffers
			blockPosition=position;
			plane1W[blockPosition] = (dither1[ address1 ]<<8) + dither1[ address2 ];
			plane2W[blockPosition] = (dither2[ address1 ]<<8) + dither2[ address2 ];
			plane3W[blockPosition] = (dither3[ address1 ]<<8) + dither3[ address2 ];
			plane4W[blockPosition] = (dither4[ address1 ]<<8) + dither4[ address2 ];
			blockPosition=position+20;
			plane1W[blockPosition] = (dither5[ address1 ]<<8) + dither5[ address2 ];
			plane2W[blockPosition] = (dither6[ address1 ]<<8) + dither6[ address2 ];
			plane3W[blockPosition] = (dither7[ address1 ]<<8) + dither7[ address2 ];
			plane4W[blockPosition] = (dither8[ address1 ]<<8) + dither8[ address2 ];

			position++; //go to next WORD

			sp+=6;//move by 6 pixels
		}
	}
}
void DrawPlayerScreen4x4(UBYTE *screen, UBYTE player, UBYTE depth, UBYTE startScreen, UBYTE xCycles)
{
	UWORD sp,position,blockPosition;
	UWORD address1, address2;
	UBYTE *dither1, *dither2, *dither3, *dither4;
	UBYTE *dither5, *dither6, *dither7, *dither8;
	UWORD w1,w2,w3,w4,w5,w6,w7,w8;

	dither1 = dither4x4EvenP1;
	dither2 = dither4x4EvenP2;
	dither3 = dither4x4EvenP3;
	dither4 = dither4x4EvenP4;

	dither5 = dither4x4OddP1;
	dither6 = dither4x4OddP2;
	dither7 = dither4x4OddP3;
	dither8 = dither4x4OddP4;

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
	for(UBYTE y=0;y<64;y++)
	{
		//40 bytes * y + even/odd + player screen offset WORDs
		position = y*20*4  + startScreen;
		//draw the line with WORDs made up of 2 BYTEs each consisting 3 pixels
		for(UBYTE x=0;x<xCycles;x++)
		{
			address1 = ((screen[sp])<<5) + (screen[sp+1]);
			address2 = ((screen[sp+2])<<5) + (screen[sp+3]);

			w1 = (dither1[ address1 ]<<8) + dither1[ address2 ];
			w2 = (dither2[ address1 ]<<8) + dither2[ address2 ];
			w3 = (dither3[ address1 ]<<8) + dither3[ address2 ];
			w4 = (dither4[ address1 ]<<8) + dither4[ address2 ];

			w5 = (dither5[ address1 ]<<8) + dither5[ address2 ];
			w6 = (dither6[ address1 ]<<8) + dither6[ address2 ];
			w7 = (dither7[ address1 ]<<8) + dither7[ address2 ];
			w8 = (dither8[ address1 ]<<8) + dither8[ address2 ];

			blockPosition=position;plane1W[blockPosition]=w1;plane2W[blockPosition]=w2;plane3W[blockPosition]=w3;plane4W[blockPosition]=w4;
			blockPosition=position+20;plane1W[blockPosition]=w5;plane2W[blockPosition]=w6;plane3W[blockPosition]=w7;plane4W[blockPosition]=w8;
			blockPosition=position+40;plane1W[blockPosition]=w1;plane2W[blockPosition]=w2;plane3W[blockPosition]=w3;plane4W[blockPosition]=w4;
			blockPosition=position+60;plane1W[blockPosition]=w5;plane2W[blockPosition]=w6;plane3W[blockPosition]=w7;plane4W[blockPosition]=w8;

			position++; //go to next WORD
			sp+=4; //go to the next 2 points
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
	UBYTE lastColor;

	dither1 = dither4x4EvenP1;
	dither2 = dither4x4EvenP2;
	dither3 = dither4x4EvenP3;
	dither4 = dither4x4EvenP4;

	dither5 = dither4x4OddP1;
	dither6 = dither4x4OddP2;
	dither7 = dither4x4OddP3;
	dither8 = dither4x4OddP4;

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
		lastColor = screen[sp];
		//draw the line with WORDs made up of 2 BYTEs each consisting 3 pixels
		for(UBYTE x=0;x<xCycles;x++)
		{
			address1 = ((lastColor)<<5) + (screen[sp]);
			address2 = ((screen[sp])<<5) + (screen[sp+1]);
			lastColor = screen[sp+1];

			w1 = (dither1[ address1 ]<<8) + dither1[ address2 ];
			w2 = (dither2[ address1 ]<<8) + dither2[ address2 ];
			w3 = (dither3[ address1 ]<<8) + dither3[ address2 ];
			w4 = (dither4[ address1 ]<<8) + dither4[ address2 ];

			w5 = (dither5[ address1 ]<<8) + dither5[ address2 ];
			w6 = (dither6[ address1 ]<<8) + dither6[ address2 ];
			w7 = (dither7[ address1 ]<<8) + dither7[ address2 ];
			w8 = (dither8[ address1 ]<<8) + dither8[ address2 ];

			blockPosition=position;plane1W[blockPosition]=w1;plane2W[blockPosition]=w2;plane3W[blockPosition]=w3;plane4W[blockPosition]=w4;
			blockPosition=position+20;plane1W[blockPosition]=w5;plane2W[blockPosition]=w6;plane3W[blockPosition]=w7;plane4W[blockPosition]=w8;
			blockPosition=position+40;plane1W[blockPosition]=w1;plane2W[blockPosition]=w2;plane3W[blockPosition]=w3;plane4W[blockPosition]=w4;
			blockPosition=position+60;plane1W[blockPosition]=w5;plane2W[blockPosition]=w6;plane3W[blockPosition]=w7;plane4W[blockPosition]=w8;
			blockPosition=position+80;plane1W[blockPosition]=w1;plane2W[blockPosition]=w2;plane3W[blockPosition]=w3;plane4W[blockPosition]=w4;
			blockPosition=position+100;plane1W[blockPosition]=w5;plane2W[blockPosition]=w6;plane3W[blockPosition]=w7;plane4W[blockPosition]=w8;
			blockPosition=position+120;plane1W[blockPosition]=w1;plane2W[blockPosition]=w2;plane3W[blockPosition]=w3;plane4W[blockPosition]=w4;
			blockPosition=position+140;plane1W[blockPosition]=w5;plane2W[blockPosition]=w6;plane3W[blockPosition]=w7;plane4W[blockPosition]=w8;

			position++; //go to next WORD
			sp+=2; //go to the next 2 points
		}
	}//line
}
