
#ifdef AMIGA
void ClearBuffor()
{
	memset(engine.renderer.plane1W, 0, PLANEWIDTH*PLANEHEIGHT);
	memset(engine.renderer.plane2W, 0, PLANEWIDTH*PLANEHEIGHT);
	memset(engine.renderer.plane3W, 0, PLANEWIDTH*PLANEHEIGHT);
	memset(engine.renderer.plane4W, 0, PLANEWIDTH*PLANEHEIGHT);
	
}
#else
void ClearBuffor()
{
	memset(engine.renderer.planes, 0, PLANEWIDTHWORD*PLANEHEIGHT*sizeof(WORD));
}
#endif

void DrawBar(UWORD posX, UWORD posY, UBYTE length)
{
	UWORD position;
	posX = posX >> 4; 
	length &= 0xF;

	UWORD mask1;
	UWORD mask2;
	if(length & 0x8)
	{
		mask1 = 0b1111111111111111;
		mask2 = 0b1111111111111111 << (16 - (length & 0x7)*2);

	}
	else
	{
		mask1 = 0b1111111111111111 << (16 - (length & 0x7)*2);

		mask2 = 0;
	}
	


	UWORD mask = 0b1111111111111111 << (16 - (length));
	UWORD leftStamp4 = 0b0000000000000000 & mask1;
	UWORD leftStamp3 = 0b1111111111111111 & mask1;
	UWORD leftStamp2 = 0b0000000011111111 & mask1;
	UWORD leftStamp1 = 0b0000111100001111 & mask1;


	UWORD rightStamp4 = 0b1111111111111111 & mask2;
	UWORD rightStamp3 = 0b0000000000000000 & mask2;
	UWORD rightStamp2 = 0b0000000011111111 & mask2;
	UWORD rightStamp1 = 0b0000111100001111 & mask2;
	
#ifdef AMIGA
	position = (posY) * PLANEWIDTHWORD + posX;
    UWORD *firstCol = engine.renderer.plane1W + position;
    UWORD *secondCol = engine.renderer.plane2W + position;
    UWORD *thirdCol = engine.renderer.plane3W + position;
    UWORD *fourthCol = engine.renderer.plane4W + position;
#else
	position = (posY) * PLANEWIDTHWORD + posX*4;
    UWORD *firstCol = engine.renderer.planes + position;
    UWORD *secondCol = engine.renderer.planes + position + 1;
    UWORD *thirdCol = engine.renderer.planes + position + 2;
    UWORD *fourthCol = engine.renderer.planes + position + 3;
#endif

	int y;
	for(y = 0; y < 5; ++y)
	{
		*firstCol = leftStamp1;
		*secondCol = leftStamp2;
		*thirdCol = leftStamp3;
		*fourthCol = leftStamp4;
		*(firstCol + 1) = rightStamp1;
		*(secondCol + 1) = rightStamp2;
		*(thirdCol + 1) = rightStamp3;
		*(fourthCol + 1) = rightStamp4;
		
        firstCol += PLANEWIDTHWORD;
        secondCol += PLANEWIDTHWORD;
        thirdCol += PLANEWIDTHWORD;
        fourthCol += PLANEWIDTHWORD;
	}

}

void DrawSprite4b(unsigned char *bLogo, BITMAPINFOHEADER *bhLogo, 
					UWORD posX, UWORD posY, WORD spriteIndexX, WORD spriteIndexY,
					UWORD spriteSizeX, UWORD spriteSizeY)
{
	UWORD position;
	UWORD word1, word2, word3, word4, mask;

	WORD planePosX = posX>>4;
	BYTE leftGap = posX - (planePosX<<4);
	BYTE rightGap = 16 - leftGap;

	UBYTE xSteps = (spriteSizeX>>4);


	//position = startOffset;

	UWORD baseX = spriteIndexX*(spriteSizeX>>4);
	UWORD baseY = spriteIndexY*spriteSizeY;


#ifdef AMIGA
	position = (posY - (spriteSizeY>>1)) * PLANEWIDTHWORD + planePosX - (spriteSizeX>>5);
    UWORD *firstCol = engine.renderer.plane1W + position;
    UWORD *secondCol = engine.renderer.plane2W + position;
    UWORD *thirdCol = engine.renderer.plane3W + position;
    UWORD *fourthCol = engine.renderer.plane4W + position;
    #else 
	position = (posY - (spriteSizeY/2)) * PLANEWIDTHWORD + planePosX*4 - spriteSizeX/32*4;
    UWORD *firstCol = engine.renderer.planes + position;
    UWORD *secondCol = engine.renderer.planes + position + 1;
    UWORD *thirdCol = engine.renderer.planes + position + 2;
    UWORD *fourthCol = engine.renderer.planes + position + 3;
	#endif

	UWORD wordWidth = bhLogo->biWidth>>4;
	UWORD upStride = (wordWidth << 2) + (xSteps << 2);

	UWORD offset = (baseY + spriteSizeY - 1)*wordWidth*4 + baseX*4;
	UWORD maskOffset = (baseY + spriteSizeY - 1)*wordWidth + baseX;

	UWORD *bmpRowPtr1 = (UWORD *)bLogo + offset;
	UWORD *bmpRowPtr2 = bmpRowPtr1 + 1;
	UWORD *bmpRowPtr3 = bmpRowPtr2 + 1;
	UWORD *bmpRowPtr4 = bmpRowPtr3 + 1;

	UWORD *maskPtr = (UWORD *)bLogo  + maskOffset + (bhLogo->biSizeImage>>1);
	

	for (UWORD y =0; y < spriteSizeY; y++)
	{
		
        UWORD *firstPos = firstCol;
        UWORD *secondPos = secondCol;
        UWORD *thirdPos = thirdCol;
        UWORD *fourthPos = fourthCol;

		UWORD prevValue1 = 0;
		UWORD prevValue2 = 0;
		UWORD prevValue3 = 0;
		UWORD prevValue4 = 0;
		UWORD prevMask = 0;

		UWORD currValue1;
		UWORD currValue2;
		UWORD currValue3;
		UWORD currValue4;
		UWORD currMask;

		for (UWORD x = 0; x < xSteps; x++)
		{
			currValue1 = *(bmpRowPtr1);
			currValue2 = *(bmpRowPtr2);
			currValue3 = *(bmpRowPtr3);
			currValue4 = *(bmpRowPtr4);
			currMask = *(maskPtr+x);

			word1 = (prevValue1 << rightGap) + (currValue1 >> leftGap);
			word2 = (prevValue2 << rightGap) + (currValue2 >> leftGap);
			word3 = (prevValue3 << rightGap) + (currValue3 >> leftGap);
			word4 = (prevValue4 << rightGap) + (currValue4 >> leftGap);
			mask = (prevMask << rightGap) + (currMask >> leftGap);

			*firstPos = (word1 & mask) | (*firstPos & ~mask );
			*secondPos = (word2 & mask) | (*secondPos & ~mask );
			*thirdPos = (word3 & mask) | (*thirdPos & ~mask );
			*fourthPos = (word4 & mask) | (*fourthPos & ~mask );
			
			prevValue1 = currValue1;
			prevValue2 = currValue2;
			prevValue3 = currValue3;
			prevValue4 = currValue4;
			prevMask = currMask;


			
			#ifdef AMIGA
			firstPos++;
			secondPos++;
			thirdPos++;
			fourthPos++;
			#else
            firstPos+=4;
            secondPos+=4;
            thirdPos+=4;
            fourthPos+=4;
			#endif

			bmpRowPtr1 += 4;
			bmpRowPtr2 += 4;
			bmpRowPtr3 += 4;
			bmpRowPtr4 += 4;
		
		}

		word1 = prevValue1 << rightGap;
		word2 = prevValue2 << rightGap;
		word3 = prevValue3 << rightGap;
		word4 = prevValue4 << rightGap;
		mask = prevMask << rightGap;

		*firstPos = (word1 & mask) | (*firstPos & ~mask );
		*secondPos = (word2 & mask) | (*secondPos & ~mask );
		*thirdPos = (word3 & mask) | (*thirdPos & ~mask );
		*fourthPos = (word4 & mask) | (*fourthPos & ~mask );


        firstCol += PLANEWIDTHWORD;
        secondCol += PLANEWIDTHWORD;
        thirdCol += PLANEWIDTHWORD;
        fourthCol += PLANEWIDTHWORD;

		bmpRowPtr1 -= upStride;
		bmpRowPtr2 -= upStride;
		bmpRowPtr3 -= upStride;
		bmpRowPtr4 -= upStride;

		maskPtr -= wordWidth;
	}
}

void DrawBitmap4bCenter(unsigned char *bLogo, BITMAPINFOHEADER *bhLogo)
{

	UWORD position;
	UWORD word1, word2, word3, word4;
	ULONG xx, yy;
#ifdef AMIGA
	position = ((PLANEHEIGHT-bhLogo->biHeight)/2) * PLANEWIDTHWORD + (320-bhLogo->biWidth)/32;
    UWORD *firstCol = engine.renderer.plane1W + position;
    UWORD *secondCol = engine.renderer.plane2W + position;
    UWORD *thirdCol = engine.renderer.plane3W + position;
    UWORD *fourthCol = engine.renderer.plane4W + position;
    #else 
	position = ((PLANEHEIGHT-bhLogo->biHeight)/2) * PLANEWIDTHWORD + (320-bhLogo->biWidth)/32*4;
    UWORD *firstCol = engine.renderer.planes + position;
    UWORD *secondCol = engine.renderer.planes + position + 1;
    UWORD *thirdCol = engine.renderer.planes + position + 2;
    UWORD *fourthCol = engine.renderer.planes + position + 3;
	#endif


	//position = startOffset;

	UWORD wordWidth = bhLogo->biWidth >> 4;
	UWORD twoWordWidth = wordWidth << 3;

	UWORD *bmpPtr1 = (UWORD *)bLogo + (bhLogo->biHeight - 1)*wordWidth*4;
	UWORD *bmpPtr2 = (UWORD *)bmpPtr1+1;
	UWORD *bmpPtr3 = (UWORD *)bmpPtr1+2;
	UWORD *bmpPtr4 = (UWORD *)bmpPtr1+3;

	for (ULONG y = 0; y < bhLogo->biHeight; y++)
	{

        UWORD *firstPos = firstCol;
        UWORD *secondPos = secondCol;
        UWORD *thirdPos = thirdCol;
        UWORD *fourthPos = fourthCol;

		for (ULONG x = 0; x < wordWidth; x++)
		{

			word1 = *bmpPtr1;
			word2 = *bmpPtr2;
			word3 = *bmpPtr3;
			word4 = *bmpPtr4;

			*firstPos = word1;
			*secondPos = word2;
			*thirdPos = word3;
			*fourthPos = word4;

			 #ifdef AMIGA
            firstPos++;
            secondPos++;
            thirdPos++;
            fourthPos++;
            #else
            firstPos+=4;
            secondPos+=4;
            thirdPos+=4;
            fourthPos+=4;
			#endif

			bmpPtr1 += 4;
			bmpPtr2 += 4;
			bmpPtr3 += 4;
			bmpPtr4 += 4;
		}
        firstCol += PLANEWIDTHWORD;
        secondCol += PLANEWIDTHWORD;
        thirdCol += PLANEWIDTHWORD;
        fourthCol += PLANEWIDTHWORD;
		
		bmpPtr1 -= twoWordWidth;
		bmpPtr2 -= twoWordWidth;
		bmpPtr3 -= twoWordWidth;
		bmpPtr4 -= twoWordWidth;
		
	}
}

void DrawPixel(UWORD x, UWORD y, UBYTE color)
{
		UWORD posX = x/16;
	UWORD leftGap = x - posX*16;
	UWORD rightGap = 16 - leftGap;

	#ifdef AMIGA
	UWORD firstPos = y*PLANEWIDTHWORD+posX;
	UWORD secondPos = firstPos+1;
	#else
	UWORD firstPos = y*PLANEWIDTHWORD+posX*4;
	UWORD secondPos = firstPos+4;
	#endif

	UBYTE col = color & 15;
	WORD pattern = col|(col << 4)|(col << 8)|(col << 12);

	WORD leftPixelPattern = pattern >> leftGap;
	WORD leftScreenPattern = 0b1111111111111111 << rightGap;

	WORD rightPixelPattern = pattern << rightGap;
	WORD rightScreenPattern = 0b1111111111111111 >> leftGap;

	#ifdef AMIGA
	engine.renderer.plane1W[firstPos] = (leftPixelPattern) + (engine.renderer.plane1W[firstPos] & (leftScreenPattern));
	engine.renderer.plane2W[firstPos] = (leftPixelPattern) + (engine.renderer.plane2W[firstPos] & (leftScreenPattern));
	engine.renderer.plane3W[firstPos] = (leftPixelPattern) + (engine.renderer.plane3W[firstPos] & (leftScreenPattern));
	engine.renderer.plane4W[firstPos] = (leftPixelPattern) + (engine.renderer.plane4W[firstPos] & (leftScreenPattern));
	
	engine.renderer.plane1W[secondPos] = (rightPixelPattern) + (engine.renderer.plane1W[secondPos] & (rightScreenPattern));
	engine.renderer.plane2W[secondPos] = (rightPixelPattern) + (engine.renderer.plane2W[secondPos] & (rightScreenPattern));
	engine.renderer.plane3W[secondPos] = (rightPixelPattern) + (engine.renderer.plane3W[secondPos] & (rightScreenPattern));
	engine.renderer.plane4W[secondPos] = (rightPixelPattern) + (engine.renderer.plane4W[secondPos] & (rightScreenPattern));
	#else
	engine.renderer.planes[firstPos] = (leftPixelPattern) + (engine.renderer.planes[firstPos] & (leftScreenPattern));
	engine.renderer.planes[firstPos+1] = (leftPixelPattern) + (engine.renderer.planes[firstPos+1] & (leftScreenPattern));
	engine.renderer.planes[firstPos+2] = (leftPixelPattern) + (engine.renderer.planes[firstPos+2] & (leftScreenPattern));
	engine.renderer.planes[firstPos+3] = (leftPixelPattern) + (engine.renderer.planes[firstPos+3] & (leftScreenPattern));
	
	engine.renderer.planes[secondPos] = (rightPixelPattern) + (engine.renderer.planes[secondPos] & (rightScreenPattern));
	engine.renderer.planes[secondPos+1] = (rightPixelPattern) + (engine.renderer.planes[secondPos+1] & (rightScreenPattern));
	engine.renderer.planes[secondPos+2] = (rightPixelPattern) + (engine.renderer.planes[secondPos+2] & (rightScreenPattern));
	engine.renderer.planes[secondPos+3] = (rightPixelPattern) + (engine.renderer.planes[secondPos+3] & (rightScreenPattern));
	#endif
}

void DrawCrosshair(UWORD x, UWORD y)
{
	UWORD posX = x/16;
	UWORD leftGap = x - posX*16;
	UWORD rightGap = 16 - leftGap;

	#ifdef AMIGA
	UWORD firstPos = y*PLANEWIDTHWORD+posX;
	UWORD secondPos = firstPos+1;
	#else
	UWORD firstPos = y*PLANEWIDTHWORD+posX*4;
	UWORD secondPos = firstPos+4;
	#endif

	WORD leftPixelPattern = 0b1000100110010001 >> leftGap;
	WORD leftScreenPattern = 0b1111111111111111 << rightGap;

	WORD rightPixelPattern = 0b1000100110010001 << rightGap;
	WORD rightScreenPattern = 0b1111111111111111 >> leftGap;

	#ifdef AMIGA
	engine.renderer.plane1W[firstPos] = (leftPixelPattern) + (engine.renderer.plane1W[firstPos] & (leftScreenPattern));
	engine.renderer.plane2W[firstPos] = (leftPixelPattern) + (engine.renderer.plane2W[firstPos] & (leftScreenPattern));
	engine.renderer.plane3W[firstPos] = (leftPixelPattern) + (engine.renderer.plane3W[firstPos] & (leftScreenPattern));
	engine.renderer.plane4W[firstPos] = (leftPixelPattern) + (engine.renderer.plane4W[firstPos] & (leftScreenPattern));
	
	engine.renderer.plane1W[secondPos] = (rightPixelPattern) + (engine.renderer.plane1W[secondPos] & (rightScreenPattern));
	engine.renderer.plane2W[secondPos] = (rightPixelPattern) + (engine.renderer.plane2W[secondPos] & (rightScreenPattern));
	engine.renderer.plane3W[secondPos] = (rightPixelPattern) + (engine.renderer.plane3W[secondPos] & (rightScreenPattern));
	engine.renderer.plane4W[secondPos] = (rightPixelPattern) + (engine.renderer.plane4W[secondPos] & (rightScreenPattern));
	#else
	engine.renderer.planes[firstPos] = (leftPixelPattern) + (engine.renderer.planes[firstPos] & (leftScreenPattern));
	engine.renderer.planes[firstPos+1] = (leftPixelPattern) + (engine.renderer.planes[firstPos+1] & (leftScreenPattern));
	engine.renderer.planes[firstPos+2] = (leftPixelPattern) + (engine.renderer.planes[firstPos+2] & (leftScreenPattern));
	engine.renderer.planes[firstPos+3] = (leftPixelPattern) + (engine.renderer.planes[firstPos+3] & (leftScreenPattern));
	
	engine.renderer.planes[secondPos] = (rightPixelPattern) + (engine.renderer.planes[secondPos] & (rightScreenPattern));
	engine.renderer.planes[secondPos+1] = (rightPixelPattern) + (engine.renderer.planes[secondPos+1] & (rightScreenPattern));
	engine.renderer.planes[secondPos+2] = (rightPixelPattern) + (engine.renderer.planes[secondPos+2] & (rightScreenPattern));
	engine.renderer.planes[secondPos+3] = (rightPixelPattern) + (engine.renderer.planes[secondPos+3] & (rightScreenPattern));
	#endif
}

void DrawPixelWord(UWORD x, UWORD y, UBYTE color)
{
  
#ifdef AMIGA
  engine.renderer.plane1W[y * PLANEWIDTHWORD + x] = ( (color>>0) & 1) * 0xffff;
  engine.renderer.plane2W[y * PLANEWIDTHWORD + x] = ( (color>>1) & 1) * 0xffff;
  engine.renderer.plane3W[y * PLANEWIDTHWORD + x] = ( (color>>2) & 1) * 0xffff;
  engine.renderer.plane4W[y * PLANEWIDTHWORD + x] = ( (color>>3) & 1) * 0xffff;
  #else
  engine.renderer.planes[y * PLANEWIDTHWORD + x*4] = ( (color>>0) & 1) * 0xffff;
  engine.renderer.planes[y * PLANEWIDTHWORD + x*4 + 1] = ( (color>>1) & 1) * 0xffff;
  engine.renderer.planes[y * PLANEWIDTHWORD + x*4 + 2] = ( (color>>2) & 1) * 0xffff;
  engine.renderer.planes[y * PLANEWIDTHWORD + x*4 + 3] = ( (color>>3) & 1) * 0xffff;
  #endif
}



void ConvertIntToChar(int number, char *test, int size)
{
	int temp;
	int i = size-2;
	while (number != 0)
	{
		temp = number % 10;
		number /= 10;
		test[i] = '0' + temp;
		i--;
	}
	while (i >= 0)
	{
		test[i] = ' ';
		i--;
	}
	test[size-1] = 0;
}
