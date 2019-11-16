#include "engine.h"
void OverwriteMap()
{
	UBYTE mapNumber;
	
	UWORD (*_mapHigh)[256];
	UWORD (*_mapMed)[256];
	UWORD (*_mapLow)[256];

	mapNumber = (p1y/256) % 11 ;


	if(mapNumber == 0){_mapHigh = mapHigh1;_mapMed = mapMed1;_mapLow = mapLow1;}
	else if(mapNumber == 1){_mapHigh = mapHigh2;_mapMed = mapMed2;_mapLow = mapLow2;}
	else if(mapNumber == 2){_mapHigh = mapHigh3;_mapMed = mapMed3;_mapLow = mapLow3;}
	else if(mapNumber == 3){_mapHigh = mapHigh4;_mapMed = mapMed4;_mapLow = mapLow4;}
	else if(mapNumber == 4){_mapHigh = mapHigh5;_mapMed = mapMed5;_mapLow = mapLow5;}
	else if(mapNumber == 5){_mapHigh = mapHigh6;_mapMed = mapMed6;_mapLow = mapLow6;}
	else if(mapNumber == 6){_mapHigh = mapHigh7;_mapMed = mapMed7;_mapLow = mapLow7;}
	else if(mapNumber == 7){_mapHigh = mapHigh8;_mapMed = mapMed8;_mapLow = mapLow8;}
	else if(mapNumber == 8){_mapHigh = mapHigh9;_mapMed = mapMed9;_mapLow = mapLow9;}
	else if(mapNumber == 9){_mapHigh = mapHigh10;_mapMed = mapMed10;_mapLow = mapLow10;}
	else if(mapNumber == 10){_mapHigh = mapHigh0;_mapMed = mapMed0;_mapLow = mapLow0;}

	for(UWORD y=lastOverwrittenLine;y<p1y;y++)
	for(UWORD x=0;x<256;x++)
		{
			//CopyMem(_mapHigh[x][(UBYTE)y], mapHigh[x][(UBYTE)y], 256);
			// = ;
			mapHigh[x][(UBYTE)y] = _mapHigh[x][(UBYTE)y];
			//mapLow[x][(UBYTE)y] =  _mapLow[x][(UBYTE)y];
		}

	lastOverwrittenLine = p1y;
}