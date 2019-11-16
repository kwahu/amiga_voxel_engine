#include "engine.h"

void LoadMap(const char *heightName, const char *colorName, 
UWORD (*high)[256],UWORD (*med)[256],UWORD (*low)[256])
{
	//read and prepare map0
	ReadHeight(heightName, heightMap);
	ReadColor(colorName, colorMap);
	
	//AddBumpToColorMap(colorMap,heightMap);
	LimitColorMap(colorMap,heightMap);
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
	LoadMap("height0.raw", "color0.raw", mapHigh0, mapMed0, mapLow0);
	LoadMap("height1.raw", "color1.raw", mapHigh1, mapMed1, mapLow1);
	LoadMap("height2.raw", "color2.raw", mapHigh2, mapMed2, mapLow2);
	LoadMap("height3.raw", "color3.raw", mapHigh3, mapMed3, mapLow3);
	LoadMap("height4.raw", "color4.raw", mapHigh4, mapMed4, mapLow4);
	LoadMap("height5.raw", "color5.raw", mapHigh5, mapMed5, mapLow5);
	LoadMap("height6.raw", "color6.raw", mapHigh6, mapMed6, mapLow6);
	LoadMap("height7.raw", "color7.raw", mapHigh7, mapMed7, mapLow7);
	LoadMap("height8.raw", "color8.raw", mapHigh8, mapMed8, mapLow8);
	LoadMap("height9.raw", "color9.raw", mapHigh9, mapMed9, mapLow9);
	LoadMap("height10.raw", "color10.raw", mapHigh10, mapMed10, mapLow10);

	//prepare starting map
	CopyMapWord(mapHigh0, mapHigh);
	//CopyMapWord(mapMed0, mapMed);
	//CopyMapWord(mapLow0, mapLow);
}