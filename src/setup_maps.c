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
	LoadMap("data/h11", "data/c11", engine.renderer.mapSource[0]);
	engine.renderer.mapLoaded0 = 1;
	LoadMap("data/h10", "data/c10", engine.renderer.mapSource[1]);
	engine.renderer.mapLoaded1=1;
	LoadMap("data/h9", "data/c9", engine.renderer.mapSource[2]);
	engine.renderer.mapLoaded2=1;
	LoadMap("data/h8", "data/c8", engine.renderer.mapSource[3]);
	engine.renderer.mapLoaded3=1;
	LoadMap("data/h7", "data/c7", engine.renderer.mapSource[4]);
	engine.renderer.mapLoaded4=1;
	LoadMap("data/h6", "data/c6", engine.renderer.mapSource[5]);
	engine.renderer.mapLoaded5=1;
	LoadMap("data/h5", "data/c5", engine.renderer.mapSource[6]);
	engine.renderer.mapLoaded6=1;
	LoadMap("data/h4", "data/c4", engine.renderer.mapSource[7]);
	engine.renderer.mapLoaded7=1;
	LoadMap("data/h3", "data/c3", engine.renderer.mapSource[8]);
	engine.renderer.mapLoaded8=1;
	LoadMap("data/h7", "data/c7", engine.renderer.mapSource[9]);
	engine.renderer.mapLoaded9=1;
	LoadMap("data/h1", "data/c1", engine.renderer.mapSource[10]);
	engine.renderer.mapLoaded10=1;

	//prepare starting map
	CopyMapWord(engine.renderer.mapSource[0], engine.renderer.mapHigh);
	//CopyMapWord(mapMed0, mapMed);
	//CopyMapWord(mapLow0, mapLow);
}