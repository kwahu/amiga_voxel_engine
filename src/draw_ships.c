#include "ray_casting.h"
//locate enemy on screen
/*
void CalculateEnemyPlacement()
{
	int sxx = 0;
//	int syy;
//	int tzz; //depth step value
//	int fov = 8; //this changes the field of view

//	tzz = 1;
	for(int tz=1;tz<128;tz++)
	{
		//tzz += 1; //increase step with the distance from camera, less quality but better performance
		for(int sx=0;sx<32;sx++)
		{
			enemyPlacementPerspective[sx][tz] = sxx/tz;
		}
	}
}*/
/*
void DrawPlayerShip(UBYTE player)
{
	UWORD px,py,pz,ex,ey,ez;
	UWORD position;

	BYTE offsetx,offsety,offsetz;
	UWORD startOffset;
	UWORD word;

	UBYTE shipHeight;

	shipHeight = 0;

	if(player == 1)
	{
		px = p1x;
		py = p1h;
		pz = p1y;
		ex = p2x;
		ey = p2h;
		ez = p2y;
		startOffset = 0;
	}
	else
	{
		px = p2x;
		py = p2h;
		pz = p2y;
		ex = p1x;
		ey = p1h;
		ez = p1y;
		startOffset = 10;
	}

	offsetz = ez-pz;

	offsetx = ex-px;
	offsety = ey-py;

	if(offsetx > -XSIZE/2 && offsetx < XSIZE/2 && offsety > -XSIZE/2 && offsety < XSIZE/2 && offsetz>0)
	{
		if(offsetx <0) offsetx = -enemyPlacementPerspective[-offsetx][offsetz];
		else offsetx = enemyPlacementPerspective[offsetx][offsetz];

		if(offsety <0) offsety = -enemyPlacementPerspective[-offsety][offsetz];
		else offsety = enemyPlacementPerspective[offsety][offsetz];

		position = startOffset + 5 + (60*20) + (offsety*20) + (offsetx/2);

		word = 0x0;
		if(offsetz>8)       word = 0b0000000100000000;
		else if(offsetz==8) word = 0b0000000110000000;
		else if(offsetz==7) word = 0b0000001111000000;
		else if(offsetz==6) word = 0b0000011111100000;
		else if(offsetz==5) word = 0b0000111111110000;
		else if(offsetz==4) word = 0b0001111111111000;
		else if(offsetz==3) word = 0b0011111111111100;
		else if(offsetz==2) word = 0b0111111111111110;
		else if(offsetz==1) word = 0b1111111111111111;


		if(offsetz>8)       shipHeight = 1;
		else if(offsetz==8) shipHeight = 2;
		else if(offsetz==7) shipHeight = 3;
		else if(offsetz==6) shipHeight = 4;
		else if(offsetz==5) shipHeight = 5;
		else if(offsetz==4) shipHeight = 6;
		else if(offsetz==3) shipHeight = 7;
		else if(offsetz==2) shipHeight = 8;
		else if(offsetz==1) shipHeight = 9;

		for(int i=0;i<shipHeight;i++)
		{
			fastPlane1W[position+i*20] = word;
			fastPlane2W[position+i*20] = word;
			fastPlane3W[position+i*20] = word;
			fastPlane4W[position+i*20] = 0x0;
		}
	}
}
*/
