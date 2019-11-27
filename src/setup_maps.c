#include "engine.h"

void LoadMap(const char *heightName, const char *colorName,
			 UWORD (*high)[256])
{
	//read and prepare map0
	ReadHeight(heightName, heightMap);
	ReadColor(colorName, colorMap);

	//AddBumpToColorMap(colorMap,heightMap);
	//LimitColorMap(colorMap);
	//SmoothColorMap(colorMap);
	//SmoothHeightMap(heightMap);
	CombineMapsHigh(heightMap, colorMap, high);
	//SmoothHeightMap(heightMap);
	//SmoothColorMap(colorMap);
	//CombineMapsHigh(heightMap, colorMap, med);
	//SmoothHeightMap(heightMap);
	//SmoothColorMap(colorMap);
	//CombineMapsHigh(heightMap, colorMap, low);
}
void SetupMaps()
{
	LoadMap("data/height0.raw", "data/color0.raw", mapHigh0);
	mapLoaded0 = 1;
	LoadMap("data/height1.raw", "data/color1.raw", mapHigh1);mapLoaded1=1;
	LoadMap("data/height2.raw", "data/color2.raw", mapHigh2);mapLoaded2=1;
	LoadMap("data/height3.raw", "data/color3.raw", mapHigh3);mapLoaded3=1;
	LoadMap("data/height4.raw", "data/color4.raw", mapHigh4);mapLoaded4=1;
	LoadMap("data/height5.raw", "data/color5.raw", mapHigh5);mapLoaded5=1;
	LoadMap("data/height6.raw", "data/color6.raw", mapHigh6);mapLoaded6=1;
	LoadMap("data/height7.raw", "data/color7.raw", mapHigh7);mapLoaded7=1;
	LoadMap("data/height8.raw", "data/color8.raw", mapHigh8);mapLoaded8=1;
	LoadMap("data/height9.raw", "data/color9.raw", mapHigh9);mapLoaded9=1;
	LoadMap("data/height10.raw", "data/color10.raw", mapHigh10);mapLoaded10=1;

	//prepare starting map
	CopyMapWord(mapHigh0, mapHigh);
	//CopyMapWord(mapMed0, mapMed);
	//CopyMapWord(mapLow0, mapLow);
}