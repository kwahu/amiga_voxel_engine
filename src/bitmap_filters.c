#include "ray_casting.h"
//smooth the map so that there are less "spike" artifacts from the coarse rendering
void SmoothHeightMap(UBYTE (*map)[256])
{
	int value;
	for (int x = 0; x < 256; x++)
	for (int y = 0; y < 256; y++)
	{
		value = 0;
		for(int a = -1; a<2 ;a++)
		for(int b = -1; b<2 ;b++)
		value += map[(UBYTE)(x+a)][(UBYTE)(y+b)];

		map[x][y] = value/9;
	}
}

//smooth color map so that there are gradients between contrasting colours
void SmoothColorMap(UBYTE (*map)[256])
{
	int value;
	for (int x = 0; x < 256; x++)
	for (int y = 0; y < 256; y++)
	{
		value = 0;
		for(int a = -1; a<2 ;a++)
		for(int b = -1; b<2 ;b++)
		value += map[(UBYTE)(x+a)][(UBYTE)(y+b)];

		map[x][y] = value/9;
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
void AddHeightToColorMap(UBYTE (*mapColor)[256], UBYTE (*mapHeight)[256])
{
	int value;
	for (int x = 0; x < 256; x++)
	for (int y = 0; y < 256; y++)
	{
		value = mapColor[x][y]+mapHeight[x][y]/32-5;
		if(value < 1) value = 1;
		if(value > 28) value = 28;
		mapColor[x][y] = (UBYTE)(value);
	}
}
//add light and shadow contrast on the left and right sides of the terrain
void AddBumpToColorMap(UBYTE (*mapColor)[256], UBYTE (*mapHeight)[256])
{
	int value;

	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			value =  mapHeight[x][y] - mapHeight[x+1][y];//  + heightMap0[x][y] - heightMap0[x][y-1];
			value = mapColor[x][y] + 4 +  ( value /2);
			if(value < 1) value = 1;
			if(value > 28) value = 28;
			mapColor[x][y] = (UBYTE)(value);
		}
	}

}
void CopyMap(UBYTE (*source)[256], UBYTE (*destination)[256])
{
	for (int x = 0; x < 256; x++)
	for (int y = 0; y < 256; y++)
	{
		destination[x][y] = source[x][y];
	}
}
