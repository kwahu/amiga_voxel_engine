#include "engine.h"
#include "bitmap.c"

void LoadMap(const char *heightName, const char *colorName,
			 UWORD (*high)[MAPSIZE])
{
	BITMAPINFOHEADER tempHeader;
	unsigned char tempColorTable[16 * 4];
	unsigned char *heightMap, *colorMap;

	heightMap = LoadBitmapFile(heightName, &tempHeader, tempColorTable);
	colorMap = LoadBitmapFile(colorName, &tempHeader, tempColorTable);

	CombineMapsHigh(heightMap, colorMap,high);
						//GenerateMap(high);

	free(heightMap);
	free(colorMap);

	//ReadHeight(heightName, heightMap);
	//ReadColor(colorName, colorMap);

	//AddBumpToColorMap(colorMap,heightMap);
	//LimitColorMap(colorMap);
	//SmoothColorMap(colorMap);
	//SmoothHeightMap(heightMap);


	//SmoothHeightMap(heightMap);
	//SmoothColorMap(colorMap);
	//CombineMapsHigh(heightMap, colorMap, med);
	//SmoothHeightMap(heightMap);
	//SmoothColorMap(colorMap);
	//CombineMapsHigh(heightMap, colorMap, low);
}
void SetupMaps()
{
	LoadMap("data/h11", "data/c11", mapSource[0]);mapLoaded0 = 1;
	LoadMap("data/h10", "data/c10", mapSource[1]);mapLoaded1=1;
	LoadMap("data/h9", "data/c9", mapSource[2]);mapLoaded2=1;
	LoadMap("data/h8", "data/c8", mapSource[3]);mapLoaded3=1;
	LoadMap("data/h7", "data/c7", mapSource[4]);mapLoaded4=1;
	LoadMap("data/h6", "data/c6", mapSource[5]);mapLoaded5=1;
	LoadMap("data/h5", "data/c5", mapSource[6]);mapLoaded6=1;
	LoadMap("data/h4", "data/c4", mapSource[7]);mapLoaded7=1;
	LoadMap("data/h3", "data/c3", mapSource[8]);mapLoaded8=1;
	LoadMap("data/h7", "data/c7", mapSource[9]);mapLoaded9=1;
	LoadMap("data/h1", "data/c1", mapSource[10]);mapLoaded10=1;

	//prepare starting map
	CopyMapWord(mapSource[0], mapHigh);
	//CopyMapWord(mapMed0, mapMed);
	//CopyMapWord(mapLow0, mapLow);
}