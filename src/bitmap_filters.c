#include "engine.h"
//smooth the map so that there are less "spike" artifacts from the coarse rendering
void SmoothHeightMap(UBYTE (*map)[256])
{
	UWORD value;
	for (UBYTE x = 1; x < 255; x++)
	for (UBYTE y = 1; y < 255; y++)
	{

		value = map[x+1][y];
		value += map[x-1][y];
		value += map[x][y+1];
		value += map[x][y-1];

		map[x][y] = value/4;
	}
}

//smooth color map so that there are gradients between contrasting colours
void SmoothColorMap(UBYTE (*map)[256])
{
	UBYTE value;
	for (UBYTE x = 1; x < 255; x++)
	for (UBYTE y = 1; y < 255; y++)
	{

		value = map[x+1][y];
		value += map[x-1][y];
		value += map[x][y+1];
		value += map[x][y-1];

		map[x][y] = value/4;
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
/*
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
}*/
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
void CopyMapByte(UBYTE (*source)[256], UBYTE (*destination)[256])
{
	for (int x = 0; x < 256; x++)
	for (int y = 0; y < 256; y++)
	{
		destination[x][y] = source[x][y];
	}
}
void CopyMapWord(UWORD (*source)[256], UWORD (*destination)[256])
{
	for (int x = 0; x < 256; x++)
	for (int y = 0; y < 256; y++)
	{
		destination[x][y] = source[x][y];
	}
}
