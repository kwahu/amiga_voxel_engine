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
	LoadMap("data/height11.bmp", "data/color11.bmp", mapSource[0]);mapLoaded0 = 1;
	LoadMap("data/height10.bmp", "data/color10.bmp", mapSource[1]);mapLoaded1=1;
	LoadMap("data/height9.bmp", "data/color9.bmp", mapSource[2]);mapLoaded2=1;
	LoadMap("data/height8_alt.bmp", "data/color8.bmp", mapSource[3]);mapLoaded3=1;
	LoadMap("data/height7.bmp", "data/color7.bmp", mapSource[4]);mapLoaded4=1;
	LoadMap("data/height6.bmp", "data/color6.bmp", mapSource[5]);mapLoaded5=1;
	LoadMap("data/height5_alt.bmp", "data/color5.bmp", mapSource[6]);mapLoaded6=1;
	LoadMap("data/height4.bmp", "data/color4.bmp", mapSource[7]);mapLoaded7=1;
	LoadMap("data/height3.bmp", "data/color3.bmp", mapSource[8]);mapLoaded8=1;
	LoadMap("data/height2.bmp", "data/color2.bmp", mapSource[9]);mapLoaded9=1;
	LoadMap("data/height1.bmp", "data/color1.bmp", mapSource[10]);mapLoaded10=1;

	//prepare starting map
	CopyMapWord(mapSource[0], mapHigh);
	//CopyMapWord(mapMed0, mapMed);
	//CopyMapWord(mapLow0, mapLow);
}