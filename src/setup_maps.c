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
	//LoadMap("height1.raw", "color1.raw", mapHigh1);mapLoaded1=1;
	/*LoadMap("height2.raw", "color2.raw", mapHigh2);
	LoadMap("height3.raw", "color3.raw", mapHigh3);
	LoadMap("height4.raw", "color4.raw", mapHigh4);
	LoadMap("height5.raw", "color5.raw", mapHigh5);
	LoadMap("height6.raw", "color6.raw", mapHigh6);
	LoadMap("height7.raw", "color7.raw", mapHigh7);
	LoadMap("height8.raw", "color8.raw", mapHigh8);
	LoadMap("height9.raw", "color9.raw", mapHigh9);
	LoadMap("height10.raw", "color10.raw", mapHigh10);*/

	//prepare starting map
	CopyMapWord(mapHigh0, mapHigh);
	//CopyMapWord(mapMed0, mapMed);
	//CopyMapWord(mapLow0, mapLow);
}