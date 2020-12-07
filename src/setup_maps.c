
void MakeArenasForMaps(ULONG mapSize, ULONG mapCount)
{
	UWORD secondCount = 0;
	
	NewArena(&engine.firstMapArena, mapSize*(mapCount)*sizeof(UWORD));

	while(engine.firstMapArena.Memory == 0)
	{
		++secondCount;
		NewArena(&engine.firstMapArena, mapSize*(mapCount+1-secondCount)*sizeof(UWORD));

	}


	engine.firstMapLength = (mapCount-secondCount)*MAPSIZE;
	
	if(secondCount > 0)
	{
		NewArena(&engine.secondMapArena, mapSize*(secondCount+1)*sizeof(UWORD));
	}
	engine.secondMapLength = (secondCount)*MAPSIZE;
}

void LoadNoiseMap(const char *noiseName)
{
	BITMAPINFOHEADER tempHeader;
	unsigned char tempColorTable[16 * 4];
	unsigned char *noiseBitmap;

	noiseBitmap = LoadBitmapFile(noiseName, &tempHeader, tempColorTable, 0, 0);

	ULONG size = tempHeader.biWidth*tempHeader.biHeight;
	engine.renderer.noiseMap = (UBYTE *)malloc(size);
	//size = size >> 1;

	UBYTE *bitmapPtr = (UBYTE *)noiseBitmap;
	UBYTE *ptr = (UBYTE *)engine.renderer.noiseMap;

	for(ULONG i = 0; i < size; ++i)
	{
		*ptr++ = *bitmapPtr++;
	}

}

void LoadMap(const char *heightName, const char *colorName,
			 UWORD (*high), ULONG rawLength)
{
	BITMAPINFOHEADER tempHeader;
	unsigned char tempColorTable[16 * 4];
	unsigned char *heightMap, *colorMap;

	File filePtr = OpenFile(heightName);

    //read the bitmap file header
    heightMap = (unsigned char *)AllocateFromArena(&engine.temporaryArena, 128*128);
	colorMap = (unsigned char *)AllocateFromArena(&engine.temporaryArena, 128*128);
		
	ReadFile(filePtr, heightMap, MAPSIZE*MAPSIZE);
	CloseFile(filePtr);
	filePtr = OpenFile(heightName);
	ReadFile(filePtr, colorMap, MAPSIZE*MAPSIZE);
	CloseFile(filePtr);
	
	CombineMapsHigh(heightMap, colorMap,high, rawLength);
						//GenerateMap(high);
						
	ClearArena(&engine.temporaryArena);


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


// void MakeMap2(UBYTE bezierPoints [289] , UWORD (*high)[MAPSIZE])
// {

// 	UWORD powers[16] = 
// 	{
// 		0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121, 144, 169, 196, 225
// 	};
// 	UWORD powers2[16] = 
// 	{
// 		0, 28, 52, 72, 88, 100, 108, 112, 112, 108, 100, 88, 72, 52, 28, 0
// 	};
	
// 	for(UBYTE mapY = 0; mapY < 128; ++mapY)
// 	{

// 		UWORD v = mapY & 0xF;

// 		UWORD TV1 = powers[15-v];
// 		UWORD TV2 = powers2[v];
// 		UWORD TV3 = powers[v];

// 		UBYTE yStride = 16 - ((mapY/16)*2);
// 		for(UBYTE mapX = 0; mapX < 128; ++mapX)
// 		{
// 			UBYTE xStride = ((mapX/16)*2);

// 			UWORD u = mapX & 0xF;

// 			UWORD TU1 = powers[15-u];
// 			UWORD TU2 = powers2[u];
// 			UWORD TU3 = powers[u];


// 			UBYTE *bez = bezierPoints + yStride*17 + xStride;
// 			UWORD P1 = (TU1*(*bez) + TU2*(*(bez+1)) + TU3*(*(bez+2)))/225;
// 			bez -= 17;
// 			UWORD P2 = (TU1*(*bez) + TU2*(*(bez+1)) + TU3*(*(bez+2)))/225;
// 			bez -= 17;
// 			UWORD P3 = (TU1*(*bez) + TU2*(*(bez+1)) + TU3*(*(bez+2)))/225;
			
// 			UWORD P = ((TV1*P1) + (TV2*P2) + (TV3*P3))/225; 
			
// 			//UWORD oldValue = high[mapX][mapY];
// 			UWORD t = timerGetPrec();
// 			t *= 532;
// 			UWORD setP = P + (t%20) - 10;
// 			if(setP > 255)
// 			{
// 				setP = P;
// 			}
			
// 			setP |= ((( 29 -  (setP/28)) << 8) & 0xFF00);
// 			//UWORD newValue = (setP & 0xFF) + (oldValue & 0xFF00);
			
// 			 high[mapX][mapY] = setP;
// 		}
// 	}
// }

void MakeMap3(UBYTE *bezierPoints , UWORD (*high), UWORD xSamples, UWORD ySamples)
{
	UBYTE randX = timerGetPrec();
	UBYTE randY = timerGetPrec();

	randX = (randX * 532) &0x7F;
	randY = (randY * 487) &0x7F;


	UBYTE randX2 = timerGetPrec();
	UBYTE randY2 = timerGetPrec();

	randX2 = (randX2 * 371) &0x7F;
	randY2 = (randY2 * 719) &0x7F;

	UWORD xSize = ((xSamples-1)) << 3;
	UWORD ySize = ((ySamples-1)) << 3;

	UWORD yN = ySize >> 7;
	
	for(UWORD mapX = 0; mapX < xSize; ++mapX)
	{

		UWORD u = mapX & 0x7;

		UWORD TU1 = 8-u;
		UWORD TU2 = u;
		
		UWORD xStride = (mapX>>3);
		
		UBYTE *bezierptr = bezierPoints + (ySamples)*xSamples + xStride;
		UWORD newY = randY;
		UWORD newX = randX;
		
		UWORD newY2 = randY2;
		UWORD newX2 = randX2;
		for(UWORD y = 0; y < yN; ++y)
		{
			UWORD yStart = (y) << 7;
			UWORD yEnd = (y+1) << 7;

			UBYTE *noisePtr = engine.renderer.noiseMap + (newX << 8) + newY;
			UBYTE *noisePtr2 = engine.renderer.noiseMap + (newX2 << 8) + newY2;
		for(UWORD mapY = yStart; mapY < yEnd; ++mapY)
		{

			UWORD v = mapY & 0x7;

			UWORD TV1 = 8-v;
			UWORD TV2 = v;

			bezierptr -= (TV1>>3)*xSamples;

			UWORD bez = *((UWORD *)bezierptr);
			UWORD bez2 = *((UWORD *)(bezierptr-xSamples));
			UWORD P1 = (TU1*((bez>>8)&0xFF) + TU2*(bez&0xFF));
			UWORD P2 = (TU1*((bez2>>8)&0xFF) + TU2*(bez2&0xFF));
			
			UWORD P = ((TV1*P1) + (TV2*P2))>>6; 
			
			//UWORD oldValue = high[mapX][mapY];
			//UWORD t = timerGetPrec();
			//t *= 532;
			UWORD setP = (P) + ((UWORD)(*noisePtr) >> 1);// + ((UWORD)(*noisePtr2) >> 2)));
			
			if(setP > 255)
			{
				setP = 255;
			}

			setP = setP & 0xFF;
			
			setP |= ((( 29 -  (setP/28)) << 8) & 0xFF00);
			//UWORD newValue = (setP & 0xFF) + (oldValue & 0xFF00);
			
			*high = setP;
			++high;
			++noisePtr;
			++noisePtr2;
		}
		newY = (newY + 36424) & 0x7F;
		newX = (newX + 47528) & 0x7F;
		newY2 = (newY2 + 75234) & 0x7F;
		newX2 = (newX2 + 18594) & 0x7F;
		}

		++randX;
		++randX2;
	}
}

void SetupMaps()
{

	
// 	UBYTE bezier2[289] = 
// 	{
// 		255,	220,	220,	100,	24,		42,		144,	220,	84,		32,		12,		4,		8,		10,		180,	220,	255,

// 		255,	220,	100,	80,		48,		80,		220,	220,	220,	100,	40,		20,		30,		20,		124,	220,	255,
		
// 		255,	96,		64,		32,		24,		60,		220,	220,	112,	60,		30,		10,		10,		10,		30,		220,	255,
		
// 		255,	58,		36,		120,	50,		30,		220,	140,	32,		0,		0,		220,	20,		20,		20,		220,	255,
		
// 		255,	30,		24,		200,	80,		40,		220,	180,	64,		0,		0,		220,	220,	150,	32,		220,	255,
		
// 		255,	16,		20,		220,	220,	60,		50,		220,	100,	0,		0,		220,	220,	220,	16,		220,	255,
		
// 		255,	8,		12,		190,	220,	80,		44,		60,		80,		0,		220,	220,	220,	220,	34,		220,	255,
		
// 		255,	52,		22,		160,	220,	120,	14,		42,		52,		0,		220,	220, 	220,	240,	10,		220,	255,
		
// 		255,	80,		32,		40,		140,	110,	4,		36,		22,		220,	220,	220,	220,	220,	10,		220,	255,
		
// 		255,	128,	62,		28,		80,		100,	20,		30,		18,		220,	220,	220,	220,	200,	22,		220,	255,
		
// 		255,	200,	150,	42,		34,		64,		36,		48,		24,		220,	220,	220,	220,	180,	28,		220,	255,
		
// 		255,	220,	180,	140,	52,		28,		22,		64,		40,		68,		220,	220,	220,	186,	8,		220,	255,
		
// 		255,	220,	220,	165,	130,	84,		24,		136,	56,		74,		128,	220,	220,	196,	14,		220,	255,
		
// 		255,	220,	220,	220,	174,	60,		16,		158,	94,		60,		54,		128,	144,	64,		22,		220,	255,
		
// 		255,	220,	220,	220,	112,	20,		38,		200,	224,	70,		4,		100,	52,		26,		32,		220,	255,
		
// 		255,	220,	220,	126,	60,		32,		74,		220,	200,	50,		8,		50,		32,		18,		22,		220,	255,
		
// 		255,	220,	220,	100,	24,		42,		144,	220,	84,		32,		12,		4,		8,		10,		180,	220,	255,
// };
	
// 	UBYTE bezier3[3009] = 
// 	{
// 		255,	220,	220,	100,	24,		42,		144,	220,	84,		32,		12,		4,		8,		10,		180,	220,	255,

// 		255,	220,	100,	80,		48,		80,		220,	220,	220,	100,	40,		20,		30,		20,		124,	220,	255,
		
// 		255,	96,		64,		32,		24,		60,		220,	220,	112,	60,		30,		10,		10,		10,		30,		220,	255,
		
// 		255,	58,		36,		120,	50,		30,		220,	140,	32,		0,		0,		220,	20,		20,		20,		220,	255,
		
// 		255,	30,		24,		200,	80,		40,		220,	180,	64,		0,		0,		220,	220,	150,	32,		220,	255,
		
// 		255,	16,		20,		220,	220,	60,		50,		220,	100,	0,		0,		220,	220,	220,	16,		220,	255,
		
// 		255,	8,		12,		190,	220,	80,		44,		60,		80,		0,		220,	220,	220,	220,	34,		220,	255,
		
// 		255,	52,		22,		160,	220,	120,	14,		42,		52,		0,		220,	220, 	220,	240,	10,		220,	255,
		
// 		255,	80,		32,		40,		140,	110,	4,		36,		22,		220,	220,	220,	220,	220,	10,		220,	255,
		
// 		255,	128,	62,		28,		80,		100,	20,		30,		18,		220,	220,	220,	220,	200,	22,		220,	255,
		
// 		255,	200,	150,	42,		34,		64,		36,		48,		24,		220,	220,	220,	220,	180,	28,		220,	255,
		
// 		255,	220,	180,	140,	52,		28,		22,		64,		40,		68,		220,	220,	220,	186,	8,		220,	255,
		
// 		255,	220,	220,	165,	130,	84,		24,		136,	56,		74,		128,	220,	220,	196,	14,		220,	255,
		
// 		255,	220,	220,	220,	174,	60,		16,		158,	94,		60,		54,		128,	144,	64,		22,		220,	255,
		
// 		255,	220,	220,	220,	112,	20,		38,		200,	224,	70,		4,		100,	52,		26,		32,		220,	255,
		
// 		255,	220,	220,	126,	60,		32,		74,		220,	200,	50,		8,		50,		32,		18,		22,		220,	255,
		
// 		255,	220,	220,	100,	24,		42,		144,	220,	84,		32,		12,		4,		8,		10,		180,	220,	255,

// 		255,	220,	100,	80,		48,		80,		220,	220,	220,	100,	40,		20,		30,		20,		124,	220,	255,
		
// 		255,	96,		64,		32,		24,		60,		220,	220,	112,	60,		30,		10,		10,		10,		30,		220,	255,
		
// 		255,	58,		36,		120,	50,		30,		220,	140,	32,		0,		0,		220,	20,		20,		20,		220,	255,
		
// 		255,	30,		24,		200,	80,		40,		220,	180,	64,		0,		0,		220,	220,	150,	32,		220,	255,
		
// 		255,	16,		20,		220,	220,	60,		50,		220,	100,	0,		0,		220,	220,	220,	16,		220,	255,
		
// 		255,	8,		12,		190,	220,	80,		44,		60,		80,		0,		220,	220,	220,	220,	34,		220,	255,
		
// 		255,	52,		22,		160,	220,	120,	14,		42,		52,		0,		220,	220, 	220,	240,	10,		220,	255,
		
// 		255,	80,		32,		40,		140,	110,	4,		36,		22,		220,	220,	220,	220,	220,	10,		220,	255,
		
// 		255,	128,	62,		28,		80,		100,	20,		30,		18,		220,	220,	220,	220,	200,	22,		220,	255,
		
// 		255,	200,	150,	42,		34,		64,		36,		48,		24,		220,	220,	220,	220,	180,	28,		220,	255,
		
// 		255,	220,	180,	140,	52,		28,		22,		64,		40,		68,		220,	220,	220,	186,	8,		220,	255,
		
// 		255,	220,	220,	165,	130,	84,		24,		136,	56,		74,		128,	220,	220,	196,	14,		220,	255,
		
// 		255,	220,	220,	220,	174,	60,		16,		158,	94,		60,		54,		128,	144,	64,		22,		220,	255,
		
// 		255,	220,	220,	220,	112,	20,		38,		200,	224,	70,		4,		100,	52,		26,		32,		220,	255,
		
// 		255,	220,	220,	126,	60,		32,		74,		220,	200,	50,		8,		50,		32,		18,		22,		220,	255,
		
// 		255,	220,	220,	100,	24,		42,		144,	220,	84,		32,		12,		4,		8,		10,		180,	220,	255,

// 		255,	220,	100,	80,		48,		80,		220,	220,	220,	100,	40,		20,		30,		20,		124,	220,	255,
		
// 		255,	96,		64,		32,		24,		60,		220,	220,	112,	60,		30,		10,		10,		10,		30,		220,	255,
		
// 		255,	58,		36,		120,	50,		30,		220,	140,	32,		0,		0,		220,	20,		20,		20,		220,	255,
		
// 		255,	30,		24,		200,	80,		40,		220,	180,	64,		0,		0,		220,	220,	150,	32,		220,	255,
		
// 		255,	16,		20,		220,	220,	60,		50,		220,	100,	0,		0,		220,	220,	220,	16,		220,	255,
		
// 		255,	8,		12,		190,	220,	80,		44,		60,		80,		0,		220,	220,	220,	220,	34,		220,	255,
		
// 		255,	52,		22,		160,	220,	120,	14,		42,		52,		0,		220,	220, 	220,	240,	10,		220,	255,
		
// 		255,	80,		32,		40,		140,	110,	4,		36,		22,		220,	220,	220,	220,	220,	10,		220,	255,
		
// 		255,	128,	62,		28,		80,		100,	20,		30,		18,		220,	220,	220,	220,	200,	22,		220,	255,
		
// 		255,	200,	150,	42,		34,		64,		36,		48,		24,		220,	220,	220,	220,	180,	28,		220,	255,
		
// 		255,	220,	180,	140,	52,		28,		22,		64,		40,		68,		220,	220,	220,	186,	8,		220,	255,
		
// 		255,	220,	220,	165,	130,	84,		24,		136,	56,		74,		128,	220,	220,	196,	14,		220,	255,
		
// 		255,	220,	220,	220,	174,	60,		16,		158,	94,		60,		54,		128,	144,	64,		22,		220,	255,
		
// 		255,	220,	220,	220,	112,	20,		38,		200,	224,	70,		4,		100,	52,		26,		32,		220,	255,
		
// 		255,	220,	220,	126,	60,		32,		74,		220,	200,	50,		8,		50,		32,		18,		22,		220,	255,
		
// 		255,	220,	220,	100,	24,		42,		144,	220,	84,		32,		12,		4,		8,		10,		180,	220,	255,

// 		255,	220,	100,	80,		48,		80,		220,	220,	220,	100,	40,		20,		30,		20,		124,	220,	255,
		
// 		255,	96,		64,		32,		24,		60,		220,	220,	112,	60,		30,		10,		10,		10,		30,		220,	255,
		
// 		255,	58,		36,		120,	50,		30,		220,	140,	32,		0,		0,		220,	20,		20,		20,		220,	255,
		
// 		255,	30,		24,		200,	80,		40,		220,	180,	64,		0,		0,		220,	220,	150,	32,		220,	255,
		
// 		255,	16,		20,		220,	220,	60,		50,		220,	100,	0,		0,		220,	220,	220,	16,		220,	255,
		
// 		255,	8,		12,		190,	220,	80,		44,		60,		80,		0,		220,	220,	220,	220,	34,		220,	255,
		
// 		255,	52,		22,		160,	220,	120,	14,		42,		52,		0,		220,	220, 	220,	240,	10,		220,	255,
		
// 		255,	80,		32,		40,		140,	110,	4,		36,		22,		220,	220,	220,	220,	220,	10,		220,	255,
		
// 		255,	128,	62,		28,		80,		100,	20,		30,		18,		220,	220,	220,	220,	200,	22,		220,	255,
		
// 		255,	200,	150,	42,		34,		64,		36,		48,		24,		220,	220,	220,	220,	180,	28,		220,	255,
		
// 		255,	220,	180,	140,	52,		28,		22,		64,		40,		68,		220,	220,	220,	186,	8,		220,	255,
		
// 		255,	220,	220,	165,	130,	84,		24,		136,	56,		74,		128,	220,	220,	196,	14,		220,	255,
		
// 		255,	220,	220,	220,	174,	60,		16,		158,	94,		60,		54,		128,	144,	64,		22,		220,	255,
		
// 		255,	220,	220,	220,	112,	20,		38,		200,	224,	70,		4,		100,	52,		26,		32,		220,	255,
		
// 		255,	220,	220,	126,	60,		32,		74,		220,	200,	50,		8,		50,		32,		18,		22,		220,	255,
		
// 		255,	220,	220,	100,	24,		42,		144,	220,	84,		32,		12,		4,		8,		10,		180,	220,	255,
		
// 		255,	220,	100,	80,		48,		80,		220,	220,	220,	100,	40,		20,		30,		20,		124,	220,	255,
		
// 		255,	96,		64,		32,		24,		60,		220,	220,	112,	60,		30,		10,		10,		10,		30,		220,	255,
		
// 		255,	58,		36,		120,	50,		30,		220,	140,	32,		0,		0,		220,	20,		20,		20,		220,	255,
		
// 		255,	30,		24,		200,	80,		40,		220,	180,	64,		0,		0,		220,	220,	150,	32,		220,	255,
		
// 		255,	16,		20,		220,	220,	60,		50,		220,	100,	0,		0,		220,	220,	220,	16,		220,	255,
		
// 		255,	8,		12,		190,	220,	80,		44,		60,		80,		0,		220,	220,	220,	220,	34,		220,	255,
		
// 		255,	52,		22,		160,	220,	120,	14,		42,		52,		0,		220,	220, 	220,	240,	10,		220,	255,
		
// 		255,	80,		32,		40,		140,	110,	4,		36,		22,		220,	220,	220,	220,	220,	10,		220,	255,
		
// 		255,	128,	62,		28,		80,		100,	20,		30,		18,		220,	220,	220,	220,	200,	22,		220,	255,
		
// 		255,	200,	150,	42,		34,		64,		36,		48,		24,		220,	220,	220,	220,	180,	28,		220,	255,
		
// 		255,	220,	180,	140,	52,		28,		22,		64,		40,		68,		220,	220,	220,	186,	8,		220,	255,
		
// 		255,	220,	220,	165,	130,	84,		24,		136,	56,		74,		128,	220,	220,	196,	14,		220,	255,
		
// 		255,	220,	220,	220,	174,	60,		16,		158,	94,		60,		54,		128,	144,	64,		22,		220,	255,
		
// 		255,	220,	220,	220,	112,	20,		38,		200,	224,	70,		4,		100,	52,		26,		32,		220,	255,
		
// 		255,	220,	220,	126,	60,		32,		74,		220,	200,	50,		8,		50,		32,		18,		22,		220,	255,
		
// 		255,	220,	220,	100,	24,		42,		144,	220,	84,		32,		12,		4,		8,		10,		180,	220,	255,

// 		255,	220,	100,	80,		48,		80,		220,	220,	220,	100,	40,		20,		30,		20,		124,	220,	255,
		
// 		255,	96,		64,		32,		24,		60,		220,	220,	112,	60,		30,		10,		10,		10,		30,		220,	255,
		
// 		255,	58,		36,		120,	50,		30,		220,	140,	32,		0,		0,		220,	20,		20,		20,		220,	255,
		
// 		255,	30,		24,		200,	80,		40,		220,	180,	64,		0,		0,		220,	220,	150,	32,		220,	255,
		
// 		255,	16,		20,		220,	220,	60,		50,		220,	100,	0,		0,		220,	220,	220,	16,		220,	255,
		
// 		255,	8,		12,		190,	220,	80,		44,		60,		80,		0,		220,	220,	220,	220,	34,		220,	255,
		
// 		255,	52,		22,		160,	220,	120,	14,		42,		52,		0,		220,	220, 	220,	240,	10,		220,	255,
		
// 		255,	80,		32,		40,		140,	110,	4,		36,		22,		220,	220,	220,	220,	220,	10,		220,	255,
		
// 		255,	128,	62,		28,		80,		100,	20,		30,		18,		220,	220,	220,	220,	200,	22,		220,	255,
		
// 		255,	200,	150,	42,		34,		64,		36,		48,		24,		220,	220,	220,	220,	180,	28,		220,	255,
		
// 		255,	220,	180,	140,	52,		28,		22,		64,		40,		68,		220,	220,	220,	186,	8,		220,	255,
		
// 		255,	220,	220,	165,	130,	84,		24,		136,	56,		74,		128,	220,	220,	196,	14,		220,	255,
		
// 		255,	220,	220,	220,	174,	60,		16,		158,	94,		60,		54,		128,	144,	64,		22,		220,	255,
		
// 		255,	220,	220,	220,	112,	20,		38,		200,	224,	70,		4,		100,	52,		26,		32,		220,	255,
		
// 		255,	220,	220,	126,	60,		32,		74,		220,	200,	50,		8,		50,		32,		18,		22,		220,	255,
		
// 		255,	220,	220,	100,	24,		42,		144,	220,	84,		32,		12,		4,		8,		10,		180,	220,	255,
		
// 		255,	220,	100,	80,		48,		80,		220,	220,	220,	100,	40,		20,		30,		20,		124,	220,	255,
		
// 		255,	96,		64,		32,		24,		60,		220,	220,	112,	60,		30,		10,		10,		10,		30,		220,	255,
		
// 		255,	58,		36,		120,	50,		30,		220,	140,	32,		0,		0,		220,	20,		20,		20,		220,	255,
		
// 		255,	30,		24,		200,	80,		40,		220,	180,	64,		0,		0,		220,	220,	150,	32,		220,	255,
		
// 		255,	16,		20,		220,	220,	60,		50,		220,	100,	0,		0,		220,	220,	220,	16,		220,	255,
		
// 		255,	8,		12,		190,	220,	80,		44,		60,		80,		0,		220,	220,	220,	220,	34,		220,	255,
		
// 		255,	52,		22,		160,	220,	120,	14,		42,		52,		0,		220,	220, 	220,	240,	10,		220,	255,
		
// 		255,	80,		32,		40,		140,	110,	4,		36,		22,		220,	220,	220,	220,	220,	10,		220,	255,
		
// 		255,	128,	62,		28,		80,		100,	20,		30,		18,		220,	220,	220,	220,	200,	22,		220,	255,
		
// 		255,	200,	150,	42,		34,		64,		36,		48,		24,		220,	220,	220,	220,	180,	28,		220,	255,
		
// 		255,	220,	180,	140,	52,		28,		22,		64,		40,		68,		220,	220,	220,	186,	8,		220,	255,
		
// 		255,	220,	220,	165,	130,	84,		24,		136,	56,		74,		128,	220,	220,	196,	14,		220,	255,
		
// 		255,	220,	220,	220,	174,	60,		16,		158,	94,		60,		54,		128,	144,	64,		22,		220,	255,
		
// 		255,	220,	220,	220,	112,	20,		38,		200,	224,	70,		4,		100,	52,		26,		32,		220,	255,
		
// 		255,	220,	220,	126,	60,		32,		74,		220,	200,	50,		8,		50,		32,		18,		22,		220,	255,
		
// 		255,	220,	220,	100,	24,		42,		144,	220,	84,		32,		12,		4,		8,		10,		180,	220,	255,
		
// 		255,	220,	100,	80,		48,		80,		220,	220,	220,	100,	40,		20,		30,		20,		124,	220,	255,
		
// 		255,	96,		64,		32,		24,		60,		220,	220,	112,	60,		30,		10,		10,		10,		30,		220,	255,
		
// 		255,	58,		36,		120,	50,		30,		220,	140,	32,		0,		0,		220,	20,		20,		20,		220,	255,
		
// 		255,	30,		24,		200,	80,		40,		220,	180,	64,		0,		0,		220,	220,	150,	32,		220,	255,
		
// 		255,	16,		20,		220,	220,	60,		50,		220,	100,	0,		0,		220,	220,	220,	16,		220,	255,
		
// 		255,	8,		12,		190,	220,	80,		44,		60,		80,		0,		220,	220,	220,	220,	34,		220,	255,
		
// 		255,	52,		22,		160,	220,	120,	14,		42,		52,		0,		220,	220, 	220,	240,	10,		220,	255,
		
// 		255,	80,		32,		40,		140,	110,	4,		36,		22,		220,	220,	220,	220,	220,	10,		220,	255,
		
// 		255,	128,	62,		28,		80,		100,	20,		30,		18,		220,	220,	220,	220,	200,	22,		220,	255,
		
// 		255,	200,	150,	42,		34,		64,		36,		48,		24,		220,	220,	220,	220,	180,	28,		220,	255,
		
// 		255,	220,	180,	140,	52,		28,		22,		64,		40,		68,		220,	220,	220,	186,	8,		220,	255,
		
// 		255,	220,	220,	165,	130,	84,		24,		136,	56,		74,		128,	220,	220,	196,	14,		220,	255,
		
// 		255,	220,	220,	220,	174,	60,		16,		158,	94,		60,		54,		128,	144,	64,		22,		220,	255,
		
// 		255,	220,	220,	220,	112,	20,		38,		200,	224,	70,		4,		100,	52,		26,		32,		220,	255,
		
// 		255,	220,	220,	126,	60,		32,		74,		220,	200,	50,		8,		50,		32,		18,		22,		220,	255,
		
// 		255,	220,	220,	100,	24,		42,		144,	220,	84,		32,		12,		4,		8,		10,		180,	220,	255,
		
// 		255,	220,	100,	80,		48,		80,		220,	220,	220,	100,	40,		20,		30,		20,		124,	220,	255,
		
// 		255,	96,		64,		32,		24,		60,		220,	220,	112,	60,		30,		10,		10,		10,		30,		220,	255,
		
// 		255,	58,		36,		120,	50,		30,		220,	140,	32,		0,		0,		220,	20,		20,		20,		220,	255,
		
// 		255,	30,		24,		200,	80,		40,		220,	180,	64,		0,		0,		220,	220,	150,	32,		220,	255,
		
// 		255,	16,		20,		220,	220,	60,		50,		220,	100,	0,		0,		220,	220,	220,	16,		220,	255,
		
// 		255,	8,		12,		190,	220,	80,		44,		60,		80,		0,		220,	220,	220,	220,	34,		220,	255,
		
// 		255,	52,		22,		160,	220,	120,	14,		42,		52,		0,		220,	220, 	220,	240,	10,		220,	255,
		
// 		255,	80,		32,		40,		140,	110,	4,		36,		22,		220,	220,	220,	220,	220,	10,		220,	255,
		
// 		255,	128,	62,		28,		80,		100,	20,		30,		18,		220,	220,	220,	220,	200,	22,		220,	255,
		
// 		255,	200,	150,	42,		34,		64,		36,		48,		24,		220,	220,	220,	220,	180,	28,		220,	255,
		
// 		255,	220,	180,	140,	52,		28,		22,		64,		40,		68,		220,	220,	220,	186,	8,		220,	255,
		
// 		255,	220,	220,	165,	130,	84,		24,		136,	56,		74,		128,	220,	220,	196,	14,		220,	255,
		
// 		255,	220,	220,	220,	174,	60,		16,		158,	94,		60,		54,		128,	144,	64,		22,		220,	255,
		
// 		255,	220,	220,	220,	112,	20,		38,		200,	224,	70,		4,		100,	52,		26,		32,		220,	255,
		
// 		255,	220,	220,	126,	60,		32,		74,		220,	200,	50,		8,		50,		32,		18,		22,		220,	255,
		
// 		255,	220,	220,	100,	24,		42,		144,	220,	84,		32,		12,		4,		8,		10,		180,	220,	255,
		
// 		255,	220,	100,	80,		48,		80,		220,	220,	220,	100,	40,		20,		30,		20,		124,	220,	255,
		
// 		255,	96,		64,		32,		24,		60,		220,	220,	112,	60,		30,		10,		10,		10,		30,		220,	255,
		
// 		255,	58,		36,		120,	50,		30,		220,	140,	32,		0,		0,		220,	20,		20,		20,		220,	255,
		
// 		255,	30,		24,		200,	80,		40,		220,	180,	64,		0,		0,		220,	220,	150,	32,		220,	255,
		
// 		255,	16,		20,		220,	220,	60,		50,		220,	100,	0,		0,		220,	220,	220,	16,		220,	255,
		
// 		255,	8,		12,		190,	220,	80,		44,		60,		80,		0,		220,	220,	220,	220,	34,		220,	255,
		
// 		255,	52,		22,		160,	220,	120,	14,		42,		52,		0,		220,	220, 	220,	240,	10,		220,	255,
		
// 		255,	80,		32,		40,		140,	110,	4,		36,		22,		220,	220,	220,	220,	220,	10,		220,	255,
		
// 		255,	128,	62,		28,		80,		100,	20,		30,		18,		220,	220,	220,	220,	200,	22,		220,	255,
		
// 		255,	200,	150,	42,		34,		64,		36,		48,		24,		220,	220,	220,	220,	180,	28,		220,	255,
		
// 		255,	220,	180,	140,	52,		28,		22,		64,		40,		68,		220,	220,	220,	186,	8,		220,	255,
		
// 		255,	220,	220,	165,	130,	84,		24,		136,	56,		74,		128,	220,	220,	196,	14,		220,	255,
		
// 		255,	220,	220,	220,	174,	60,		16,		158,	94,		60,		54,		128,	144,	64,		22,		220,	255,
		
// 		255,	220,	220,	220,	112,	20,		38,		200,	224,	70,		4,		100,	52,		26,		32,		220,	255,
		
// 		255,	220,	220,	126,	60,		32,		74,		220,	200,	50,		8,		50,		32,		18,		22,		220,	255,
		
// 		255,	220,	220,	100,	24,		42,		144,	220,	84,		32,		12,		4,		8,		10,		180,	220,	255,
		
// 		255,	220,	100,	80,		48,		80,		220,	220,	220,	100,	40,		20,		30,		20,		124,	220,	255,
		
// 		255,	96,		64,		32,		24,		60,		220,	220,	112,	60,		30,		10,		10,		10,		30,		220,	255,
		
// 		255,	58,		36,		120,	50,		30,		220,	140,	32,		0,		0,		220,	20,		20,		20,		220,	255,
		
// 		255,	30,		24,		200,	80,		40,		220,	180,	64,		0,		0,		220,	220,	150,	32,		220,	255,
		
// 		255,	16,		20,		220,	220,	60,		50,		220,	100,	0,		0,		220,	220,	220,	16,		220,	255,
		
// 		255,	8,		12,		190,	220,	80,		44,		60,		80,		0,		220,	220,	220,	220,	34,		220,	255,
		
// 		255,	52,		22,		160,	220,	120,	14,		42,		52,		0,		220,	220, 	220,	240,	10,		220,	255,
		
// 		255,	80,		32,		40,		140,	110,	4,		36,		22,		220,	220,	220,	220,	220,	10,		220,	255,
		
// 		255,	128,	62,		28,		80,		100,	20,		30,		18,		220,	220,	220,	220,	200,	22,		220,	255,
		
// 		255,	200,	150,	42,		34,		64,		36,		48,		24,		220,	220,	220,	220,	180,	28,		220,	255,
		
// 		255,	220,	180,	140,	52,		28,		22,		64,		40,		68,		220,	220,	220,	186,	8,		220,	255,
		
// 		255,	220,	220,	165,	130,	84,		24,		136,	56,		74,		128,	220,	220,	196,	14,		220,	255,
		
// 		255,	220,	220,	220,	174,	60,		16,		158,	94,		60,		54,		128,	144,	64,		22,		220,	255,
		
// 		255,	220,	220,	220,	112,	20,		38,		200,	224,	70,		4,		100,	52,		26,		32,		220,	255,
		
// 		255,	220,	220,	126,	60,		32,		74,		220,	200,	50,		8,		50,		32,		18,		22,		220,	255,
		
// 		255,	220,	220,	100,	24,		42,		144,	220,	84,		32,		12,		4,		8,		10,		180,	220,	255,
// };
	
	//LoadNoiseMap("\\data\\skaly.bmp");

	UWORD currentMapsLength = engine.firstMapLength/MAPSIZE;
	ULONG currentRawLength = engine.firstMapLength;

	if(engine.secondMapLength > 0)
	{
		engine.renderer.firstMap = (UWORD *)AllocateFromArena(&engine.firstMapArena, (currentMapsLength+1)*MAPSIZE*MAPSIZE*sizeof(UWORD));
		engine.renderer.secondMap = (UWORD *)AllocateFromArena(&engine.secondMapArena, ((engine.secondMapLength/MAPSIZE)+1)*MAPSIZE*MAPSIZE*sizeof(UWORD));
		currentRawLength += MAPSIZE;
	}
	else
	{
		engine.renderer.firstMap = (UWORD *)AllocateFromArena(&engine.firstMapArena, (currentMapsLength)*MAPSIZE*MAPSIZE*sizeof(UWORD));
	
	}
	
	
	UWORD *currentMap = engine.renderer.firstMap;

	LoadMap(DATA_DIR(h1), DATA_DIR(c1), engine.renderer.firstMap, currentRawLength);

	currentMap += MAPSIZE;
	LoadMap(DATA_DIR(h2), DATA_DIR(c2), currentMap, currentRawLength);

	currentMap += MAPSIZE;
	

	// LoadMap(DATA_DIR(h11), DATA_DIR(c11), engine.renderer.firstMap, currentRawLength);

	// currentMap += MAPSIZE;

	// if((--currentMapsLength) == 0)
	// {
	// 	LoadMap(DATA_DIR(h10), DATA_DIR(c10), currentMap, currentRawLength);
	
	// 	currentMap = engine.renderer.secondMap;
	// 	currentRawLength = engine.secondMapLength + MAPSIZE;
	// 	currentMapsLength = 11;
	// 	LoadMap(DATA_DIR(h10), DATA_DIR(c10), currentMap, currentRawLength);

	// }
	// else
	// {
	// 	LoadMap(DATA_DIR(h10), DATA_DIR(c10), currentMap, currentRawLength);
		
	// }





	// currentMap += MAPSIZE;

	// if((--currentMapsLength) == 0)
	// {
	// 	LoadMap(DATA_DIR(h9), DATA_DIR(c9), currentMap, currentRawLength);
		
	// 	currentMap = engine.renderer.secondMap;
	// 	currentRawLength = engine.secondMapLength + MAPSIZE;
	// 	currentMapsLength = 11;
	// 	LoadMap(DATA_DIR(h9), DATA_DIR(c9), currentMap, currentRawLength);

	// }
	// else
	// {
	// 	LoadMap(DATA_DIR(h9), DATA_DIR(c9), currentMap, currentRawLength);
		
	// }


	
	// currentMap += MAPSIZE;

	// if((--currentMapsLength) == 0)
	// {
	// 	LoadMap(DATA_DIR(h8), DATA_DIR(c8), currentMap, currentRawLength);
		
	// 	currentMap = engine.renderer.secondMap;
	// 	currentRawLength = engine.secondMapLength + MAPSIZE;
	// 	currentMapsLength = 11;
	// 	LoadMap(DATA_DIR(h8), DATA_DIR(c8), currentMap, currentRawLength);

	// }
	// else
	// {
	// 	LoadMap(DATA_DIR(h8), DATA_DIR(c8), currentMap, currentRawLength);
		
	// }




	// currentMap += MAPSIZE;

	// if((--currentMapsLength) == 0)
	// {
	// 	LoadMap(DATA_DIR(h7), DATA_DIR(c7), currentMap, currentRawLength);
		
	// 	currentMap = engine.renderer.secondMap;
	// 	currentRawLength = engine.secondMapLength + MAPSIZE;
	// 	currentMapsLength = 11;
	// 	LoadMap(DATA_DIR(h7), DATA_DIR(c7), currentMap, currentRawLength);

	// }
	// else
	// {
	// 	LoadMap(DATA_DIR(h7), DATA_DIR(c7), currentMap, currentRawLength);
		
	// }




	// currentMap += MAPSIZE;

	// if((--currentMapsLength) == 0)
	// {
	// 	LoadMap(DATA_DIR(h6), DATA_DIR(c6), currentMap, currentRawLength);
		
	// 	currentMap = engine.renderer.secondMap;
	// 	currentRawLength = engine.secondMapLength + MAPSIZE;
	// 	currentMapsLength = 11;
	// 	LoadMap(DATA_DIR(h6), DATA_DIR(c6), currentMap, currentRawLength);

	// }
	// else
	// {
	// 	LoadMap(DATA_DIR(h6), DATA_DIR(c6), currentMap, currentRawLength);
		
	// }





	// currentMap += MAPSIZE;

	// if((--currentMapsLength) == 0)
	// {
	// 	LoadMap(DATA_DIR(h5), DATA_DIR(c5), currentMap, currentRawLength);
		
	// 	currentMap = engine.renderer.secondMap;
	// 	currentRawLength = engine.secondMapLength + MAPSIZE;
	// 	currentMapsLength = 11;
	// 	LoadMap(DATA_DIR(h5), DATA_DIR(c5), currentMap, currentRawLength);

	// }
	// else
	// {
	// 	LoadMap(DATA_DIR(h5), DATA_DIR(c5), currentMap, currentRawLength);
		
	// }





	// currentMap += MAPSIZE;

	// if((--currentMapsLength) == 0)
	// {
	// 	LoadMap(DATA_DIR(h4), DATA_DIR(c4), currentMap, currentRawLength);
		
	// 	currentMap = engine.renderer.secondMap;
	// 	currentRawLength = engine.secondMapLength + MAPSIZE;
	// 	currentMapsLength = 11;
	// 	LoadMap(DATA_DIR(h4), DATA_DIR(c4), currentMap, currentRawLength);

	// }
	// else
	// {
	// 	LoadMap(DATA_DIR(h4), DATA_DIR(c4), currentMap, currentRawLength);
		
	// }





	// currentMap += MAPSIZE;

	// if((--currentMapsLength) == 0)
	// {
	// 	LoadMap(DATA_DIR(h3), DATA_DIR(c3), currentMap, currentRawLength);
		
	// 	currentMap = engine.renderer.secondMap;
	// 	currentRawLength = engine.secondMapLength + MAPSIZE;
	// 	currentMapsLength = 11;
	// 	LoadMap(DATA_DIR(h3), DATA_DIR(c3), currentMap, currentRawLength);

	// }
	// else
	// {
	// 	LoadMap(DATA_DIR(h3), DATA_DIR(c3), currentMap, currentRawLength);
		
	// }



	// currentMap += MAPSIZE;

		

	// if((--currentMapsLength) == 0)
	// {
	// 	LoadMap(DATA_DIR(h7), DATA_DIR(c7), currentMap, currentRawLength);
		
	// 	currentMap = engine.renderer.secondMap;
	// 	currentRawLength = engine.secondMapLength + MAPSIZE;
	// 	currentMapsLength = 11;
	// 	LoadMap(DATA_DIR(h7), DATA_DIR(c7), currentMap, currentRawLength);

	// }
	// else
	// {
	// 	LoadMap(DATA_DIR(h7), DATA_DIR(c7), currentMap, currentRawLength);
		
	// }





	// currentMap += MAPSIZE;

	// LoadMap(DATA_DIR(h1), DATA_DIR(c1), currentMap, currentRawLength);

	// currentMap += MAPSIZE;

	// if(engine.secondMapLength > 0)
	// {
		
	// 	LoadMap(DATA_DIR(h11), DATA_DIR(c11), currentMap, currentRawLength);
	// }


	// MakeMap3(bezier3, engine.renderer.mapSource, 17, 177);
	// MakeMap3(bezier2, engine.renderer.mapSource, 17, 17);
	// MakeMap3(bezier2, engine.renderer.mapSource + MAPSIZE*MAPSIZE, 17, 17);
	// MakeMap3(bezier2, engine.renderer.mapSource + 2*MAPSIZE*MAPSIZE, 17, 17);
	// MakeMap3(bezier2, engine.renderer.mapSource + 3*MAPSIZE*MAPSIZE, 17, 17);
	// MakeMap3(bezier2, engine.renderer.mapSource + 4*MAPSIZE*MAPSIZE, 17, 17);
	// MakeMap3(bezier2, engine.renderer.mapSource + 5*MAPSIZE*MAPSIZE, 17, 17);
	// MakeMap3(bezier2, engine.renderer.mapSource + 6*MAPSIZE*MAPSIZE, 17, 17);
	// MakeMap3(bezier2, engine.renderer.mapSource + 7*MAPSIZE*MAPSIZE, 17, 17);
	// MakeMap3(bezier2, engine.renderer.mapSource + 8*MAPSIZE*MAPSIZE, 17, 17);
	// MakeMap3(bezier2, engine.renderer.mapSource + 9*MAPSIZE*MAPSIZE, 17, 17);
	// MakeMap3(bezier2, engine.renderer.mapSource + 10*MAPSIZE*MAPSIZE, 17, 17);
	
	//prepare starting map
	engine.renderer.currentMap = engine.renderer.firstMap;
	engine.renderer.mapHigh = engine.renderer.currentMap;
	//CopyMapWord(engine.renderer.mapSource, engine.renderer.mapHigh);
	//CopyMapWord(mapMed0, mapMed);
	//CopyMapWord(mapLow0, mapLow);
}