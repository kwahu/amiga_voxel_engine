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
	LoadMap("data/height11", "data/color11", mapSource[0]);mapLoaded0 = 1;
	LoadMap("data/height10", "data/color10", mapSource[1]);mapLoaded1=1;
	LoadMap("data/height9", "data/color9", mapSource[2]);mapLoaded2=1;
	LoadMap("data/height8", "data/color8", mapSource[3]);mapLoaded3=1;
	LoadMap("data/height7", "data/color7", mapSource[4]);mapLoaded4=1;
	LoadMap("data/height6", "data/color6", mapSource[5]);mapLoaded5=1;
	LoadMap("data/height5", "data/color5", mapSource[6]);mapLoaded6=1;
	LoadMap("data/height4", "data/color4", mapSource[7]);mapLoaded7=1;
	LoadMap("data/height3", "data/color3", mapSource[8]);mapLoaded8=1;
	LoadMap("data/height2", "data/color2", mapSource[9]);mapLoaded9=1;
	LoadMap("data/height1", "data/color1", mapSource[10]);mapLoaded10=1;

	//prepare starting map
	CopyMapWord(mapSource[0], mapHigh);
	//CopyMapWord(mapMed0, mapMed);
	//CopyMapWord(mapLow0, mapLow);
}