#pragma once

typedef enum BitmapType
{
	Bitmap_Map = 0,
	Bitmap_Texture = 1,
	Bitmap_Sprite = 2
}BitmapType;

ULONG ConvertEndianLONG(ULONG number)
{

    return ((number >> 24) & 0xff) |    // move byte 3 to byte 0
           ((number << 8) & 0xff0000) | // move byte 1 to byte 2
           ((number >> 8) & 0xff00) |   // move byte 2 to byte 1
           ((number << 24) & 0xff000000);
}
UWORD ConvertEndianWORD(UWORD number)
{

    return (number >> 8) | (number << 8);
}

UBYTE *LoadBitmapFile(BYTE *filename, BITMAPINFOHEADER *bitmapInfoHeader, UBYTE *bitmapColorTable, BitmapType type, UBYTE maskBit)
{
    BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
    UBYTE *bitmapImage;        //store image data
    int imageIdx = 0;                  //image index counter
    UBYTE tempRGB;             //our swap variable

    //open filename in read binary mode
#ifdef AMIGA
	BPTR filePtr = Open(filename, 1005);
#else
    FILE *filePtr = fopen(filename, "rb");
#endif
	if (filePtr == NULL)
        return NULL;

    //read the bitmap file header
    
#ifdef AMIGA
	Read(filePtr, &bitmapFileHeader, sizeof(BITMAPFILEHEADER));
#else
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
#endif


    bitmapFileHeader.bfType = ConvertEndianWORD(bitmapFileHeader.bfType);
    bitmapFileHeader.bfSize = ConvertEndianLONG(bitmapFileHeader.bfSize);
    bitmapFileHeader.bfOffBits = ConvertEndianLONG(bitmapFileHeader.bfOffBits);

    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.bfType != 0x4D42)
    {
		
#ifdef AMIGA
		Close(filePtr);
#else
	    fclose(filePtr);
#endif
		printf("verify that this is a bmp file by check bitmap id=%lu", bitmapFileHeader.bfType);
        return NULL;
    }

    //read the bitmap info header
#ifdef AMIGA
	Read(filePtr, bitmapInfoHeader, sizeof(BITMAPINFOHEADER));
#else
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr); // small edit. forgot to add the closing bracket at sizeof
#endif


    bitmapInfoHeader->biSize = ConvertEndianLONG(bitmapInfoHeader->biSize);
    bitmapInfoHeader->biWidth = ConvertEndianLONG(bitmapInfoHeader->biWidth);
    bitmapInfoHeader->biHeight = ConvertEndianLONG(bitmapInfoHeader->biHeight);
    bitmapInfoHeader->biPlanes = ConvertEndianWORD(bitmapInfoHeader->biPlanes);
    bitmapInfoHeader->biBitCount = ConvertEndianWORD(bitmapInfoHeader->biBitCount);
    bitmapInfoHeader->biCompression = ConvertEndianLONG(bitmapInfoHeader->biCompression);
    bitmapInfoHeader->biSizeImage = ConvertEndianLONG(bitmapInfoHeader->biSizeImage);
    bitmapInfoHeader->biClrUsed = ConvertEndianLONG(bitmapInfoHeader->biClrUsed);
    bitmapInfoHeader->biClrImportant = ConvertEndianLONG(bitmapInfoHeader->biClrImportant);

    //read the bitmap color table
#ifdef AMIGA
	Read(filePtr, bitmapColorTable, 64);
#else
	fread(bitmapColorTable, 4 * 16, 1, filePtr); // small edit. forgot to add the closing bracket at sizeof
#endif


    //move file point to the begging of bitmap data
#ifdef AMIGA
	Seek(filePtr, bitmapFileHeader.bfOffBits, OFFSET_BEGINNING);
#else
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
#endif


	if(bitmapImage != NULL)
	{
		//free(bitmapImage);
	}

	switch(type)
	{
		default:
		{
		    bitmapImage = (unsigned char *)AllocateFromArena(&engine.temporaryArena, bitmapInfoHeader->biSizeImage);
		} break;
		case Bitmap_Sprite:
		{
		    bitmapImage = (unsigned char *)AllocateFromArena(&engine.temporaryArena, bitmapInfoHeader->biSizeImage + bitmapInfoHeader->biSizeImage/4);
		} break;
 	}
	//allocate enough memory for the bitmap image data

    //verify memory allocation
    if (!bitmapImage)
    {
        printf("verify memory allocation");
        //free(bitmapImage);
			
#ifdef AMIGA
		Close(filePtr);
#else
	    fclose(filePtr);
#endif
        return NULL;
    }

    //read in the bitmap image data

#ifdef AMIGA
	Read(filePtr, bitmapImage, bitmapInfoHeader->biSizeImage);
#else
	fread(bitmapImage, bitmapInfoHeader->biSizeImage, 1, filePtr); // small edit. forgot to add the closing bracket at sizeof
#endif


	// UWORD remainder = bitmapInfoHeader->biSizeImage&0x3FF;
	// UWORD count = bitmapInfoHeader->biSizeImage >> 10;
	// UBYTE *tmp = bitmapImage;

	// for(UWORD i = 0; i < count; ++i)
	// {
    // 	fseek(filePtr, bitmapFileHeader.bfOffBits + (i<<10), SEEK_SET);
	// 	fread(tmp, 1024, 1, filePtr);
		
	// 	tmp += 1024;

	// }
	// fread(tmp, remainder, 1, filePtr);

	if(type == Bitmap_Sprite)
	{
		for(WORD y = 0; y < bitmapInfoHeader->biHeight; y++)
		{
			UWORD *rowPtr = ((UWORD *)(bitmapImage + y*bitmapInfoHeader->biWidth/2));
			UWORD *maskRowPtr = ((UWORD *)(bitmapImage + bitmapInfoHeader->biSizeImage +
			 (y)*bitmapInfoHeader->biWidth/8));

			for(UWORD wordIndex = 0; wordIndex < bitmapInfoHeader->biWidth/16; wordIndex++)
			{
				UWORD *maskPtr = ((UWORD *)maskRowPtr);
				maskPtr += wordIndex;		
				
				UWORD *bitmapPtr = ((UWORD *)rowPtr);
				bitmapPtr += wordIndex*4;		

				UWORD word1 = *bitmapPtr;
				++bitmapPtr;
				UWORD word2 = *bitmapPtr;
				++bitmapPtr;
				UWORD word3 = *bitmapPtr;
				++bitmapPtr;
				UWORD word4 = *bitmapPtr;
			
				UWORD packedMask = 0;
				
				UWORD shift = 0;
				for(WORD bitIndex = 12; bitIndex < 16; ++bitIndex)
				{
					UBYTE checkedPixel = ((word1 >> shift) & 0xF) ^ maskBit;
					if(checkedPixel != 0)
					{
						packedMask |= 1 << bitIndex;
					}

					shift += 4;
				}
				shift = 0;
				for(WORD bitIndex = 8; bitIndex < 12; ++bitIndex)
				{
					UBYTE checkedPixel = ((word2 >> shift) & 0xF) ^ maskBit;
					if(checkedPixel != 0)
					{
						packedMask |= 1 << bitIndex;
					}

					shift += 4;
				}
				shift = 0;
				for(WORD bitIndex = 4; bitIndex < 8; ++bitIndex)
				{
					UBYTE checkedPixel = ((word3 >> shift) & 0xF) ^ maskBit;
					if(checkedPixel != 0)
					{
						packedMask |= 1 << bitIndex;
					}

					shift += 4;
				}
				shift = 0;
				for(WORD bitIndex = 0; bitIndex < 4; ++bitIndex)
				{
					UBYTE checkedPixel = ((word4 >> shift) & 0xF) ^ maskBit;
					if(checkedPixel != 0)
					{
						packedMask |= 1 << bitIndex;
					}

					shift += 4;
				}

				*maskPtr = packedMask;
			}
		}
	}

	if(type != Bitmap_Map)
	{

		//unsigned char *temp = (unsigned char *)AllocateFromArena(&engine.tempArena, bitmapInfoHeader->biSizeImage);
		//UWORD temp[20];

		for(WORD y = bitmapInfoHeader->biHeight - 1; y >= 0; --y)
		{
			UWORD *rowPtr = ((UWORD *)(bitmapImage + y*bitmapInfoHeader->biWidth/2));
		
			//UWORD *tempRowPtr = (UWORD *)(temp + (bitmapInfoHeader->biHeight - 1 - y)*bitmapInfoHeader->biWidth/8);

			for(UWORD wordIndex = 0; wordIndex < bitmapInfoHeader->biWidth/16; wordIndex++)
			{
				// UWORD *tempPtr = (UWORD *)temp;
				// tempPtr += wordIndex;	

				UWORD word1 = *rowPtr;
				++rowPtr;
				UWORD word2 = *rowPtr;
				++rowPtr;
				UWORD word3 = *rowPtr;
				++rowPtr;
				UWORD word4 = *rowPtr;
				rowPtr -= 3;

				UWORD packedWord1 = 0;
				UWORD packedWord2 = 0;
				UWORD packedWord3 = 0;
				UWORD packedWord4 = 0;

				UWORD shift1 = 0;
				UWORD shift2 = 1;
				UWORD shift3 = 2;
				UWORD shift4 = 3;
				for(WORD bitIndex = 12; bitIndex < 16; ++bitIndex)
				{
					packedWord1 |= ((word1 >> shift1) & 1) << bitIndex;
					packedWord2 |= ((word1 >> shift2) & 1) << bitIndex;
					packedWord3 |= ((word1 >> shift3) & 1) << bitIndex;
					packedWord4 |= ((word1 >> shift4) & 1) << bitIndex;

					shift1 += 4;
					shift2 += 4;
					shift3 += 4;
					shift4 += 4;
				}
				
				shift1 = 0;
				shift2 = 1;
				shift3 = 2;
				shift4 = 3;
				for(WORD bitIndex = 8; bitIndex < 12; ++bitIndex)
				{
					packedWord1 |= ((word2 >> shift1) & 1) << bitIndex;
					packedWord2 |= ((word2 >> shift2) & 1) << bitIndex;
					packedWord3 |= ((word2 >> shift3) & 1) << bitIndex;
					packedWord4 |= ((word2 >> shift4) & 1) << bitIndex;

					shift1 += 4;
					shift2 += 4;
					shift3 += 4;
					shift4 += 4;
				}
				
				shift1 = 0;
				shift2 = 1;
				shift3 = 2;
				shift4 = 3;
				for(WORD bitIndex = 4; bitIndex < 8; ++bitIndex)
				{
					packedWord1 |= ((word3 >> shift1) & 1) << bitIndex;
					packedWord2 |= ((word3 >> shift2) & 1) << bitIndex;
					packedWord3 |= ((word3 >> shift3) & 1) << bitIndex;
					packedWord4 |= ((word3 >> shift4) & 1) << bitIndex;

					shift1 += 4;
					shift2 += 4;
					shift3 += 4;
					shift4 += 4;
				}
				
				shift1 = 0;
				shift2 = 1;
				shift3 = 2;
				shift4 = 3;
				for(WORD bitIndex = 0; bitIndex < 4; ++bitIndex)
				{
					packedWord1 |= ((word4 >> shift1) & 1) << bitIndex;
					packedWord2 |= ((word4 >> shift2) & 1) << bitIndex;
					packedWord3 |= ((word4 >> shift3) & 1) << bitIndex;
					packedWord4 |= ((word4 >> shift4) & 1) << bitIndex;

					shift1 += 4;
					shift2 += 4;
					shift3 += 4;
					shift4 += 4;
				}

				*rowPtr++ = packedWord1;
				//tempPtr += bitmapInfoHeader->biSizeImage/8;
				*rowPtr++ = packedWord2;
				//tempPtr += bitmapInfoHeader->biSizeImage/8;
				*rowPtr++ = packedWord3;
				//tempPtr += bitmapInfoHeader->biSizeImage/8;
				*rowPtr++ = packedWord4;



			}
		}

		// UWORD *tempPtr = ((UWORD *)temp);
		// UWORD *bitmapPtr = ((UWORD *)bitmapImage);
		// for(UWORD wordIndex = 0; wordIndex < bitmapInfoHeader->biSizeImage/2; wordIndex++)
		// {
		// 	*bitmapPtr++ = *tempPtr++;

			
		// }
		// ClearArena(&engine.tempArena);

	}



	

    //close file and return bitmap iamge data

#ifdef AMIGA
	Close(filePtr);
#else    
	fclose(filePtr);
#endif 
	return bitmapImage;
}

// void SmoothHeightMap(UBYTE (*map)[MAPSIZE])
// {
// 	UWORD value;
// 	for (UBYTE x = 0; x < MAPSIZE; ++x)
// 	for (UBYTE y = 0; y < MAPSIZE; ++y)
// 	{

// 		value = map[x+1][y];
// 		value += map[x-1][y];
// 		value += map[x][y+1];
// 		value += map[x][y-1];

// 		map[x][y] = value/4;
// 	}
// }

// //smooth color map so that there are gradients between contrasting colours
// void SmoothColorMap(UBYTE (*map)[MAPSIZE])
// {
// 	UBYTE value;
// 	for (UBYTE x = 0; x < MAPSIZE; ++x)
// 	for (UBYTE y = 0; y < MAPSIZE; ++y)
// 	{

// 		value = map[x+1][y];
// 		value += map[x-1][y];
// 		value += map[x][y+1];
// 		value += map[x][y-1];

// 		map[x][y] = value/4;
// 	}
// }

// //add more light to higher ground
// void AddHeightToColorMap(UBYTE (*mapColor)[MAPSIZE], UBYTE (*mapHeight)[MAPSIZE])
// {
// 	int value;
// 	for (int x = 0; x < MAPSIZE; x++)
// 	for (int y = 0; y < MAPSIZE; y++)
// 	{
// 		value = mapColor[x][y]+mapHeight[x][y]/32-5;
// 		if(value < 1) value = 1;
// 		if(value > 28) value = 28;
// 		mapColor[x][y] = (UBYTE)(value);
// 	}
// }
// //add light and shadow contrast on the left and right sides of the terrain
// void AddBumpToColorMap(UBYTE (*mapColor)[MAPSIZE], UBYTE (*mapHeight)[MAPSIZE])
// {
// 	int value;

// 	for (int x = 0; x < MAPSIZE; x++)
// 	{
// 		for (int y = 0; y < MAPSIZE; y++)
// 		{
// 			value =  mapHeight[x][y] - mapHeight[x+1][y];//  + heightMap0[x][y] - heightMap0[x][y-1];
// 			value = mapColor[x][y] + 4 +  ( value /2);
// 			if(value < 1) value = 1;
// 			if(value > 28) value = 28;
// 			mapColor[x][y] = (UBYTE)(value);
// 		}
// 	}

// }
// //add light and shadow contrast on the left and right sides of the terrain
// void LimitColorMap(UBYTE (*mapColor)[MAPSIZE])
// {
// 	int value;

// 	for (int x = 0; x < MAPSIZE; x++)
// 	{
// 		for (int y = 0; y < MAPSIZE; y++)
// 		{
// 			value = mapColor[x][y]/2;
// 			if(value < 1) value = 1;
// 			//if(value > 4) value = 4;
// 			mapColor[x][y] = (UBYTE)(value);
// 		}
// 	}

//}
// void CopyMapByte(UBYTE (*source)[MAPSIZE], UBYTE (*destination)[MAPSIZE])
// {
// 	for (int x = 0; x < MAPSIZE; x++)
// 	for (int y = 0; y < MAPSIZE; y++)
// 	{
// 		destination[x][y] = source[x][y];
// 	}
// }



void CombineMapsHigh(UBYTE (*height)[MAPSIZE], UBYTE (*color)[MAPSIZE], UWORD (*map), ULONG length)
{


	for (int x = 0; x < MAPSIZE; x++) {
		for (int y = 0; y < MAPSIZE; y++) {
			*(map + y*length + x) = (color[x][y] << 8) + height[x][y];
		}
	}
}

