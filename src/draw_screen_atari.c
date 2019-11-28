#include "engine.h"

// void CopyFastToChipW(tBitMap *bm)
// {
// 	CopyMemQuick(planes, bm->Planes[0], PLANEWIDTH*PLANEHEIGHT);
// 	CopyMemQuick(planes, bm->Planes[1], PLANEWIDTH*PLANEHEIGHT);
// 	CopyMemQuick(planes, bm->Planes[2], PLANEWIDTH*PLANEHEIGHT);
// 	CopyMemQuick(planes, bm->Planes[3], PLANEWIDTH*PLANEHEIGHT);
// }
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

	sp = 0;

	//for each line
	for(UBYTE y=0;y<YSIZEODD;y++)
	{
		//40 bytes * y + even/odd + player screen offset WORDs
		position = y*80*2  + startScreen*4;
		//draw the line with WORDs made up of 2 BYTEs each consisting 3 pixels
		for(UBYTE x=0;x<xCycles;x++)
		{
			//calculate locations of BYTE values to fetch
			address1 = (screen[sp]<<10) + (screen[sp+1]<<5) + (screen[sp+2]);
			address2 = (screen[sp+3]<<10) + (screen[sp+4]<<5) + (screen[sp+5]);

			//fetch propper BYTEs and write with WORDs to plane buffers
			blockPosition=position;
			planes[blockPosition] = (dither3x2EvenP1[ address1 ]<<8) + dither3x2EvenP1[ address2 ];
			planes[blockPosition+1] = (dither3x2EvenP2[ address1 ]<<8) + dither3x2EvenP2[ address2 ];
			planes[blockPosition+2] = (dither3x2EvenP3[ address1 ]<<8) + dither3x2EvenP3[ address2 ];
			planes[blockPosition+3] = (dither3x2EvenP4[ address1 ]<<8) + dither3x2EvenP4[ address2 ];
			blockPosition=position+80;
			// planes[blockPosition] = (dither5[ address1 ]<<8) + dither5[ address2 ];
			// planes[blockPosition+1] = (dither6[ address1 ]<<8) + dither6[ address2 ];
			// planes[blockPosition+2] = (dither7[ address1 ]<<8) + dither7[ address2 ];
			// planes[blockPosition+3] = (dither8[ address1 ]<<8) + dither8[ address2 ];

			position+=4; //go to next WORD

			sp+=6;//move by 6 pixels
		}
	}
}
void DrawPlayerScreen4x4(UBYTE *screen, UBYTE player, UBYTE depth, UBYTE startScreen, UBYTE xCycles)
{
	UWORD sp,position,blockPosition;
	UWORD address1, address2;
	UWORD w1,w2,w3,w4,w5,w6,w7,w8;
	UBYTE val1, val2;


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
	for(UBYTE y=0;y<YSIZEEVEN;y++)
	{
		//40 bytes * y + even/odd + player screen offset WORDs
		position = y*20*4  + startScreen*4;
		//draw the line with WORDs made up of 2 BYTEs each consisting 3 pixels
		for(UBYTE x=0;x<xCycles;x++)
		{
			address1 = ((screen[sp])<<5) + (screen[sp+1]);
			address2 = ((screen[sp+2])<<5) + (screen[sp+3]);

	/*		val1 = dither1[ address1 ];
			val2 = dither1[ address2 ];
			w1 = (val1<<8) + val2;
			w5 = ( ((val1 << 1) | (val1 >> 15))<<8 ) + ( (val2 << 1) | (val2 >> 15) );

			val1 = dither2[ address1 ];
			val2 = dither2[ address2 ];
			w2 = (val1<<8) + val2;
			w6 = ( ((val1 << 1) | (val1 >> 15))<<8 ) + ( (val2 << 1) | (val2 >> 15) );

			val1 = dither3[ address1 ];
			val2 = dither3[ address2 ];
			w3 = (val1<<8) + val2;
			w7 = ( ((val1 << 1) | (val1 >> 15))<<8 ) + ( (val2 << 1) | (val2 >> 15) );

			val1 = dither4[ address1 ];
			val2 = dither4[ address2 ];
			w4 = (val1<<8) + val2;
			w8 = ( ((val1 << 1) | (val1 >> 15))<<8 ) + ( (val2 << 1) | (val2 >> 15) );*/

			w1 = (dither4x4EvenP1[ address1 ]<<8) + dither4x4EvenP1[ address2 ];
			w2 = (dither4x4EvenP2[ address1 ]<<8) + dither4x4EvenP2[ address2 ];
			w3 = (dither4x4EvenP3[ address1 ]<<8) + dither4x4EvenP3[ address2 ];
			w4 = (dither4x4EvenP4[ address1 ]<<8) + dither4x4EvenP4[ address2 ];

			// w5 = (dither5[ address1 ]<<8) + dither5[ address2 ];
			// w6 = (dither6[ address1 ]<<8) + dither6[ address2 ];
			// w7 = (dither7[ address1 ]<<8) + dither7[ address2 ];
			// w8 = (dither8[ address1 ]<<8) + dither8[ address2 ];

			blockPosition=position;
			planes[blockPosition]=w1;
			planes[blockPosition]=w2;
			planes[blockPosition]=w3;
			planes[blockPosition]=w4;
			blockPosition=position+20;
			// planes[blockPosition]=w5;
			// planes[blockPosition]=w6;
			// planes[blockPosition]=w7;
			// planes[blockPosition]=w8;
			blockPosition=position+40;
			planes[blockPosition]=w1;
			planes[blockPosition]=w2;
			planes[blockPosition]=w3;
			planes[blockPosition]=w4;
			blockPosition=position+60;
			// planes[blockPosition]=w5;
			// planes[blockPosition]=w6;
			// planes[blockPosition]=w7;
			// planes[blockPosition]=w8;

			position++; //go to next WORD
			sp+=4; //go to the next 2 points
		}
	}//line
}
/*
void DrawPlayerScreen2x2(UBYTE *screen, UBYTE player, UBYTE depth, UBYTE startScreen, UBYTE xCycles)
{
	UWORD sp,position,blockPosition;
	UWORD address1, address2;
	UBYTE *dither1, *dither2, *dither3, *dither4;
	UBYTE *dither5, *dither6, *dither7, *dither8;
	UWORD w1,w2,w3,w4,w5,w6,w7,w8;
	UBYTE val1, val2;

	dither1 = dither2x2P1Even;
	dither2 = dither2x2P2Even;
	dither3 = dither2x2P3Even;
	dither4 = dither2x2P4Even;

	dither5 = dither2x2P1Odd;
	dither6 = dither2x2P2Odd;
	dither7 = dither2x2P3Odd;
	dither8 = dither2x2P4Odd;


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
	for(UBYTE y=0;y<128;y++)
	{
		//40 bytes * y + even/odd + player screen offset WORDs
		position = y*20*2  + startScreen;
		//draw the line with WORDs made up of 2 BYTEs each consisting 3 pixels
		for(UBYTE x=0;x<xCycles;x++)
		{
			address1 = ((screen[sp])<<15) + ((screen[sp+1])<<10) + ((screen[sp+2])<<5) + screen[sp+3];
			address2 = ((screen[sp+4])<<15) + ((screen[sp+5])<<10) + ((screen[sp+6])<<5) + screen[sp+7];




			w5 = (dither5[ address1 ]<<8) + dither5[ address2 ];
			w6 = (dither6[ address1 ]<<8) + dither6[ address2 ];
			w7 = (dither7[ address1 ]<<8) + dither7[ address2 ];
			w8 = (dither8[ address1 ]<<8) + dither8[ address2 ];

			blockPosition=position;
			planes[blockPosition]=w1;
			planes[blockPosition]=w2;
			planes[blockPosition]=w3;
			planes[blockPosition]=w4;
			blockPosition=position+20;
			planes[blockPosition]=w5;
			planes[blockPosition]=w6;
			planes[blockPosition]=w7;
			planes[blockPosition]=w8;

			position++; //go to next WORD
			sp+=8; //go to the next 2 points
		}
	}//line
}
*/
void DrawPlayerScreen8x8(UBYTE *screen, UBYTE player, UBYTE depth, UBYTE startScreen, UBYTE xCycles)
{
	UWORD sp,position,blockPosition;
	UWORD address1, address2;
	UWORD w1,w2,w3,w4,w5,w6,w7,w8;
	UBYTE lastColor;


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
	for(UBYTE y=0;y<YSIZEEVEN/2;y++)
	{
		//40 bytes * y + even/odd + player screen offset WORDs
		position = y*20*8  + startScreen*4;
		lastColor = screen[sp];
		//draw the line with WORDs made up of 2 BYTEs each consisting 3 pixels
		for(UBYTE x=0;x<xCycles;x++)
		{
			address1 = ((lastColor)<<5) + (screen[sp]);
			address2 = ((screen[sp])<<5) + (screen[sp+1]);
			lastColor = screen[sp+1];

			w1 = (dither4x4EvenP1[ address1 ]<<8) + dither4x4EvenP1[ address2 ];
			w2 = (dither4x4EvenP2[ address1 ]<<8) + dither4x4EvenP2[ address2 ];
			w3 = (dither4x4EvenP3[ address1 ]<<8) + dither4x4EvenP3[ address2 ];
			w4 = (dither4x4EvenP4[ address1 ]<<8) + dither4x4EvenP4[ address2 ];

			// w5 = (dither5[ address1 ]<<8) + dither5[ address2 ];
			// w6 = (dither6[ address1 ]<<8) + dither6[ address2 ];
			// w7 = (dither7[ address1 ]<<8) + dither7[ address2 ];
			// w8 = (dither8[ address1 ]<<8) + dither8[ address2 ];

			blockPosition=position;planes[blockPosition]=w1;planes[blockPosition]=w2;planes[blockPosition]=w3;planes[blockPosition]=w4;
		//	blockPosition=position+20;planes[blockPosition]=w5;planes[blockPosition]=w6;planes[blockPosition]=w7;planes[blockPosition]=w8;
			blockPosition=position+40;planes[blockPosition]=w1;planes[blockPosition]=w2;planes[blockPosition]=w3;planes[blockPosition]=w4;
		//	blockPosition=position+60;planes[blockPosition]=w5;planes[blockPosition]=w6;planes[blockPosition]=w7;planes[blockPosition]=w8;
			blockPosition=position+80;planes[blockPosition]=w1;planes[blockPosition]=w2;planes[blockPosition]=w3;planes[blockPosition]=w4;
		//	blockPosition=position+100;planes[blockPosition]=w5;planes[blockPosition]=w6;planes[blockPosition]=w7;planes[blockPosition]=w8;
			blockPosition=position+120;planes[blockPosition]=w1;planes[blockPosition]=w2;planes[blockPosition]=w3;planes[blockPosition]=w4;
		//	blockPosition=position+140;planes[blockPosition]=w5;planes[blockPosition]=w6;planes[blockPosition]=w7;planes[blockPosition]=w8;

			position++; //go to next WORD
			sp+=2; //go to the next 2 points
		}
	}//line
}