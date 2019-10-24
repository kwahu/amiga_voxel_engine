#include "ray_casting.h"
//smooth the map so that there are less "spike" artifacts from the coarse rendering
void SmoothHeightMap()
{
	int value;
	for (int x = 0; x < 256; x++)
	for (int y = 0; y < 256; y++)
	{
		value = 0;
		for(int a = -1; a<2 ;a++)
		for(int b = -1; b<2 ;b++)
		value += heightMap0[(UBYTE)(x+a)][(UBYTE)(y+b)];

		heightMap0[x][y] = value/9;
	}
}

//smooth color map so that there are gradients between contrasting colours
void SmoothColorMap()
{
	int value;
	for (int x = 0; x < 256; x++)
	for (int y = 0; y < 256; y++)
	{
		value = 0;
		for(int a = -1; a<2 ;a++)
		for(int b = -1; b<2 ;b++)
		value += colorMap0[(UBYTE)(x+a)][(UBYTE)(y+b)];

		colorMap0[x][y] = value/9;
	}
}
/*
void SmoothScreen(UBYTE player)
{
	UBYTE *screen;//,*screenDepth;
	UBYTE value = 0;

	if(player == 1)
	{
		screen = screenP1;
	}
	else
	{
		screen = screenP1;
	}

	for (UWORD position = 0; position < XSIZE*YSIZE; position++)
	{
		value = (value + screen[position+1])/2;
		screen[position] = value;
	}
}*/
void GenerateColorMap()
{
	int value;
	for (int x = 0; x < 256; x++)
	for (int y = 0; y < 256; y++)
	{
		value = 4;
		value += heightMap0[x][y]/16;
		value += heightMap0[x][y] - heightMap0[x+1][y];//  + heightMap0[x][y] - heightMap0[x][y-1];
		if(value < 1) value = 1;
		if(value > 28) value = 28;
		colorMap0[x][y] = (UBYTE)(value);
	}
}
//add more light to higher ground
void AddHeightToColorMap()
{
	int value;
	for (int x = 0; x < 256; x++)
	for (int y = 0; y < 256; y++)
	{
		value = colorMap0[x][y]+heightMap0[x][y]/32-5;
		if(value < 1) value = 1;
		if(value > 28) value = 28;
		colorMap0[x][y] = (UBYTE)(value);
	}
}
//add light and shadow contrast on the left and right sides of the terrain
void AddBumpToColorMap()
{
	int value;

	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			value =  heightMap0[x][y] - heightMap0[x+1][y];//  + heightMap0[x][y] - heightMap0[x][y-1];
			value = colorMap0[x][y] + 4 +  ( value /2);
			if(value < 1) value = 1;
			if(value > 28) value = 28;
			colorMap0[x][y] = (UBYTE)(value);
		}
	}

}
