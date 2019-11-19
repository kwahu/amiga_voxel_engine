#include "engine.h"
void OverwriteMap()
{
	UBYTE mapNumber;
	
	UWORD (*_mapHigh)[256];

	mapNumber = (p1y/256) % 11 ;


	if(mapNumber == 0){_mapHigh = mapHigh1;}
	else if(mapNumber == 1){_mapHigh = mapHigh2;}
	else if(mapNumber == 2){_mapHigh = mapHigh3;}
	else if(mapNumber == 3){_mapHigh = mapHigh4;}
	else if(mapNumber == 4){_mapHigh = mapHigh5;}
	else if(mapNumber == 5){_mapHigh = mapHigh6;}
	else if(mapNumber == 6){_mapHigh = mapHigh7;}
	else if(mapNumber == 7){_mapHigh = mapHigh8;}
	else if(mapNumber == 8){_mapHigh = mapHigh9;}
	else if(mapNumber == 9){_mapHigh = mapHigh10;}
	else if(mapNumber == 10){_mapHigh = mapHigh0;}

	for(UWORD y=lastOverwrittenLine;y<p1y;y++)
	for(UWORD x=0;x<256;x++)
		{
			//int x = 0;
			//CopyMem(&_mapHigh[x][(UBYTE)y], &mapHigh[x][(UBYTE)y], 256);
			
			mapHigh[x][(UBYTE)y] = _mapHigh[x][(UBYTE)y];
			//mapLow[x][(UBYTE)y] =  _mapLow[x][(UBYTE)y];
		}

	lastOverwrittenLine = p1y;
}