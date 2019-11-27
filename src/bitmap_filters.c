#include "engine.h"
//smooth the map so that there are less "spike" artifacts from the coarse rendering
void SmoothHeightMap(UBYTE (*map)[MAPSIZE])
{
	UWORD value;
	for (UBYTE x = 0; x < MAPSIZE; ++x)
	for (UBYTE y = 0; y < MAPSIZE; ++y)
	{

		value = map[x+1][y];
		value += map[x-1][y];
		value += map[x][y+1];
		value += map[x][y-1];

		map[x][y] = value/4;
	}
}

//smooth color map so that there are gradients between contrasting colours
void SmoothColorMap(UBYTE (*map)[MAPSIZE])
{
	UBYTE value;
	for (UBYTE x = 0; x < MAPSIZE; ++x)
	for (UBYTE y = 0; y < MAPSIZE; ++y)
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
	for (int x = 0; x < MAPSIZE; x++)
	for (int y = 0; y < MAPSIZE; y++)
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
void AddHeightToColorMap(UBYTE (*mapColor)[MAPSIZE], UBYTE (*mapHeight)[MAPSIZE])
{
	int value;
	for (int x = 0; x < MAPSIZE; x++)
	for (int y = 0; y < MAPSIZE; y++)
	{
		value = mapColor[x][y]+mapHeight[x][y]/32-5;
		if(value < 1) value = 1;
		if(value > 28) value = 28;
		mapColor[x][y] = (UBYTE)(value);
	}
}
//add light and shadow contrast on the left and right sides of the terrain
void AddBumpToColorMap(UBYTE (*mapColor)[MAPSIZE], UBYTE (*mapHeight)[MAPSIZE])
{
	int value;

	for (int x = 0; x < MAPSIZE; x++)
	{
		for (int y = 0; y < MAPSIZE; y++)
		{
			value =  mapHeight[x][y] - mapHeight[x+1][y];//  + heightMap0[x][y] - heightMap0[x][y-1];
			value = mapColor[x][y] + 4 +  ( value /2);
			if(value < 1) value = 1;
			if(value > 28) value = 28;
			mapColor[x][y] = (UBYTE)(value);
		}
	}

}
//add light and shadow contrast on the left and right sides of the terrain
void LimitColorMap(UBYTE (*mapColor)[MAPSIZE])
{
	int value;

	for (int x = 0; x < MAPSIZE; x++)
	{
		for (int y = 0; y < MAPSIZE; y++)
		{
			value = mapColor[x][y]/2;
			if(value < 1) value = 1;
			//if(value > 4) value = 4;
			mapColor[x][y] = (UBYTE)(value);
		}
	}

}
void CopyMapByte(UBYTE (*source)[MAPSIZE], UBYTE (*destination)[MAPSIZE])
{
	for (int x = 0; x < MAPSIZE; x++)
	for (int y = 0; y < MAPSIZE; y++)
	{
		destination[x][y] = source[x][y];
	}
}
void CopyMapWord(UWORD (*source)[MAPSIZE], UWORD (*destination)[256])
{
	UWORD word;
	//CopyMemQuick(source, destination, MAPSIZE*MAPSIZE*2);
	//memcpy(destination, source, MAPSIZE*MAPSIZE*2);
	for (int x = 0; x < MAPSIZE; x++)
		for (int y = 0; y < MAPSIZE; y++)
		{
			word = source[x][y];
			destination[x*2][y*2] = word;
			destination[x*2+1][y*2] = word;
			destination[x*2][y*2+1] = word;
			destination[x*2+1][y*2+1] = word;
		}
}
