#include "engine.h"
#include "bitmap.c"

void LoadMap(const char *heightName, const char *colorName,
			 UWORD (*high)[MAPSIZE])
{
	BITMAPINFOHEADER tempHeader;
	unsigned char tempColorTable[16 * 4];
	unsigned char *tempMap, *colorMap;

	tempMap = LoadBitmapFile(heightName, &tempHeader, tempColorTable);
	colorMap = LoadBitmapFile(colorName, &tempHeader, tempColorTable);

	CombineMapsHigh(tempMap, colorMap,high);
						//GenerateMap(high);

	//free(heightMap);
	//free(colorMap);

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
	LoadMap("data/height11.bmp", "data/color11.bmp", mapHigh0);mapLoaded0 = 1;
	LoadMap("data/height10.bmp", "data/color10.bmp", mapHigh1);mapLoaded1=1;
	LoadMap("data/height9.bmp", "data/color9.bmp", mapHigh2);mapLoaded2=1;
	LoadMap("data/height8.bmp", "data/color8.bmp", mapHigh3);mapLoaded3=1;
	LoadMap("data/height7.bmp", "data/color7.bmp", mapHigh4);mapLoaded4=1;
	LoadMap("data/height6.bmp", "data/color6.bmp", mapHigh5);mapLoaded5=1;
	LoadMap("data/height5.bmp", "data/color5.bmp", mapHigh6);mapLoaded6=1;
	LoadMap("data/height4.bmp", "data/color4.bmp", mapHigh7);mapLoaded7=1;
	LoadMap("data/height3.bmp", "data/color3.bmp", mapHigh8);mapLoaded8=1;
	LoadMap("data/height2.bmp", "data/color2.bmp", mapHigh9);mapLoaded9=1;
	LoadMap("data/height1.bmp", "data/color1.bmp", mapHigh10);mapLoaded10=1;

	//prepare starting map
	CopyMapWord(mapHigh0, mapHigh);
	//CopyMapWord(mapMed0, mapMed);
	//CopyMapWord(mapLow0, mapLow);
}