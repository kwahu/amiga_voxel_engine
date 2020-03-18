#pragma once

#ifdef AMIGA

#define BASELINE 28

// Font * InitFont(char *fileName)
// {
//     return fontCreate(fileName);
// }

// TextBitMap *CreateFontBitmap(Font *font)
// {
//     return fontCreateTextBitMapFromStr(font, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
// }

// TextBitMap *CreateBitmapFromText(Font *font, char * text)
// {
//     return fontCreateTextBitMapFromStr(font, text);
// }

// void FillTextBitmap(Font *font, TextBitMap *Bitmap, char *Text)
// {
    
// 	fontFillTextBitMap(font, Bitmap, Text);
// }

// void DrawTextBitmap(TextBitMap *Bitmap, UWORD x, UWORD y, UBYTE colorBit)
// {
//     fontDrawTextBitMap(engine.platformScreen.s_pBuffer->pBack, Bitmap, x, y, colorBit, FONT_LEFT|FONT_COOKIE);
// }

// void DrawTextBitmapOverwrite(TextBitMap *Bitmap, UWORD x, UWORD y, UBYTE colorBit)
// {
//     fontDrawTextBitMap(engine.platformScreen.s_pBuffer->pBack, Bitmap, x, y, colorBit, FONT_LEFT|FONT_COOKIE);
// }

// void FreeTextBitmap(TextBitMap *Bitmap)
// {
//     fontDestroyTextBitMap(Bitmap);
// }

// void FreeFont()
// {
// 	fontDestroy(engine.font);

// }

#else

#define BASELINE 0
#endif

TextBitMap *CreateBitmapFromText(Font *font, char * text)
{

	UWORD Width = 0;
	UWORD MaxWidth = 0;
	UWORD Height = font->Height;
	// Text width measurement
	for (const char *p = text; *p; ++p) 
    {
    
        UBYTE Idx = (UBYTE)*p;
        Width += (font->CharOffsets[Idx + 1] - font->CharOffsets[Idx]) + 1;
	}
    
    TextBitMap *textBitMap = malloc(sizeof(TextBitMap));
    UBYTE WordWidth = ((Width+15)/16);
	textBitMap->BitMap = (UWORD *)malloc(sizeof(UWORD)*WordWidth*Height);
	textBitMap->Width = WordWidth;
	textBitMap->Height = Height;
	textBitMap->ActualWidth = 0;
	textBitMap->ActualHeight = 0;
	FillTextBitmap(font, textBitMap, text);
	return textBitMap;
}


TextBitMap *CreateFontBitmap(Font *font)
{
    return CreateBitmapFromText(font, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
}

void FillTextBitmap(Font *font, TextBitMap *Bitmap, char *Text)
{
    memset(Bitmap->BitMap, 0, Bitmap->Width*Bitmap->Height*sizeof(UWORD));

    UWORD X = 0;
	UWORD Y = 0;
	Bitmap->ActualWidth = 0;
	for(const char *p = Text; *p; ++p) 
    {
        
        UBYTE Idx = (UBYTE)*p;
        UBYTE GlyphWidth = (font->CharOffsets[Idx + 1] - font->CharOffsets[Idx]);
        UBYTE blitX = X/16;
        UBYTE leftShift = X%16;
        UBYTE rightShift = 16 - leftShift;

        UWORD charOffset = font->CharOffsets[Idx];
        UWORD charOffsetBase = charOffset/16;
        UWORD charLeftShift = charOffset%16;
        UWORD charRightShift = 16 - charLeftShift;
        UWORD *charPtr = font->RawData + charOffsetBase;
        UWORD charBlit = 0;

        for(int j = 0; j < font->Height; ++j)
        {
            UWORD charLeftMask = (0b1111111111111111 << (16 - GlyphWidth));

            charBlit = (charLeftMask & ((*charPtr) << charLeftShift));
            UWORD Diff = (GlyphWidth - charRightShift); 
            if(Diff < 16)
            {
                charBlit |= (charLeftMask & ((*(charPtr+1)) >> charRightShift));
            }



            Bitmap->BitMap[j*Bitmap->Width + blitX] |= (charBlit >> leftShift);
            
            Bitmap->BitMap[j*Bitmap->Width + blitX + 1] |= (charBlit << rightShift);
            
            charPtr += (font->Width/16);
            charBlit = 0;
        }
        
        X += GlyphWidth + 1;
        Bitmap->ActualWidth = (X+15)/16;
	}
	Bitmap->ActualHeight = font->Height;

}

void DrawTextBitmap(TextBitMap *Bitmap, UWORD x, UWORD y, UBYTE colorBit)
{

    UWORD posX = x/16;
    UWORD leftGap = x - posX*16;
    UWORD rightGap = 16 - leftGap;

    
    UWORD *bitmapPtr = Bitmap->BitMap;
    UWORD bitmapValue = 0;
    UWORD bitmapValue2 = 0;

    for(int j = 0; j < Bitmap->ActualHeight; ++j)
    {

#if AMIGA
        UWORD firstPos = (y+j)*PLANEWIDTHWORD+posX;
        UWORD *firstPtr = engine.renderer.plane1W + firstPos;
        UWORD *secondPtr = engine.renderer.plane2W + firstPos;
        UWORD *thirdPtr = engine.renderer.plane3W + firstPos;
        UWORD *fourthPtr = engine.renderer.plane4W + firstPos;
#else
        UWORD firstPos = (y+j)*PLANEWIDTHWORD+posX*4;
        UWORD *firstPtr = engine.renderer.planes + firstPos;
        UWORD *secondPtr = engine.renderer.planes + firstPos+1;
        UWORD *thirdPtr = engine.renderer.planes + firstPos+2;
        UWORD *fourthPtr = engine.renderer.planes + firstPos+3;
#endif

        UWORD *bitmapPtr = Bitmap->BitMap + j*Bitmap->Width;

        bitmapValue = *bitmapPtr;
        UWORD leftPixelPattern = bitmapValue >> leftGap;
        UWORD leftScreenPattern = ~leftPixelPattern;
        
    
        *firstPtr = ((colorBit) & 1)*(leftPixelPattern) + (*firstPtr & (leftScreenPattern));
        *secondPtr = ((colorBit >> 1) & 1)*(leftPixelPattern) + (*secondPtr & (leftScreenPattern));
        *thirdPtr = ((colorBit >> 2) & 1)*(leftPixelPattern) + (*thirdPtr & (leftScreenPattern));
        *fourthPtr = ((colorBit >> 3) & 1)*(leftPixelPattern) + (*fourthPtr & (leftScreenPattern));

#if AMIGA

        firstPtr++;
        secondPtr++;
        thirdPtr++;
        fourthPtr++;
#else
        firstPtr += 4;
        secondPtr += 4;
        thirdPtr += 4;
        fourthPtr += 4;
#endif


        for(int i = 1; i < Bitmap->ActualWidth; ++i)
        {
            
            bitmapValue = *bitmapPtr;
            bitmapPtr++;
            bitmapValue2 = *bitmapPtr;

                        
            UWORD middlePixelPattern = (bitmapValue << rightGap) + (bitmapValue2 >> leftGap);
            UWORD middleScreenPattern = ~middlePixelPattern;            

  
            *firstPtr = ((colorBit) & 1)*(middlePixelPattern) + (*firstPtr & (middleScreenPattern));
            *secondPtr = ((colorBit >> 1) & 1)*(middlePixelPattern) + (*secondPtr & (middleScreenPattern));
            *thirdPtr = ((colorBit >> 2) & 1)*(middlePixelPattern) + (*thirdPtr & (middleScreenPattern));
            *fourthPtr = ((colorBit >> 3) & 1)*(middlePixelPattern) + (*fourthPtr & (middleScreenPattern));

#if AMIGA

            firstPtr++;
            secondPtr++;
            thirdPtr++;
            fourthPtr++;
#else
            firstPtr += 4;
            secondPtr += 4;
            thirdPtr += 4;
            fourthPtr += 4;
#endif
        }

        bitmapValue = *bitmapPtr;
        UWORD rightPixelPattern = (bitmapValue << rightGap);
        UWORD rightScreenPattern = ~rightPixelPattern;
        

        *firstPtr = ((colorBit) & 1)*(rightPixelPattern) + (*firstPtr & (rightScreenPattern));
        *secondPtr = ((colorBit >> 1) & 1)*(rightPixelPattern) + (*secondPtr & (rightScreenPattern));
        *thirdPtr = ((colorBit >> 2) & 1)*(rightPixelPattern) + (*thirdPtr & (rightScreenPattern));
        *fourthPtr = ((colorBit >> 3) & 1)*(rightPixelPattern) + (*fourthPtr & (rightScreenPattern));

#if AMIGA

        firstPtr++;
        secondPtr++;
        thirdPtr++;
        fourthPtr++;
#else
        firstPtr += 4;
        secondPtr += 4;
        thirdPtr += 4;
        fourthPtr += 4;
#endif

    }

}


void DrawTextBitmapOverwrite(TextBitMap *Bitmap, UWORD x, UWORD y, UBYTE colorBit)
{

    UWORD posX = x/16;
    UWORD leftGap = x - posX*16;
    UWORD rightGap = 16 - leftGap;

    UWORD *bitmapPtr = Bitmap->BitMap;
    UWORD bitmapValue = 0;
    UWORD bitmapValue2 = 0;

    for(int j = 0; j < Bitmap->ActualHeight; ++j)
    {

      
#if AMIGA
        UWORD firstPos = (y+j)*PLANEWIDTHWORD+posX;
        UWORD *firstPtr = engine.renderer.plane1W + firstPos;
        UWORD *secondPtr = engine.renderer.plane2W + firstPos;
        UWORD *thirdPtr = engine.renderer.plane3W + firstPos;
        UWORD *fourthPtr = engine.renderer.plane4W + firstPos;
#else
        UWORD firstPos = (y+j)*PLANEWIDTHWORD+posX*4;
        UWORD *firstPtr = engine.renderer.planes + firstPos;
        UWORD *secondPtr = engine.renderer.planes + firstPos+1;
        UWORD *thirdPtr = engine.renderer.planes + firstPos+2;
        UWORD *fourthPtr = engine.renderer.planes + firstPos+3;
#endif
        UWORD *bitmapPtr = Bitmap->BitMap + j*Bitmap->Width;

        bitmapValue = *bitmapPtr;
        UWORD leftPixelPattern = bitmapValue >> leftGap;
        UWORD leftScreenPattern = ~leftPixelPattern;
        
          
        *firstPtr = ((colorBit) & 1)*(leftPixelPattern);
        *secondPtr = ((colorBit >> 1) & 1)*(leftPixelPattern);
        *thirdPtr = ((colorBit >> 2) & 1)*(leftPixelPattern);
        *fourthPtr = ((colorBit >> 3) & 1)*(leftPixelPattern);

#if AMIGA

        firstPtr++;
        secondPtr++;
        thirdPtr++;
        fourthPtr++;
#else

        firstPtr += 4;
        secondPtr += 4;
        thirdPtr += 4;
        fourthPtr += 4;
#endif

        for(int i = 1; i < Bitmap->ActualWidth; ++i)
        {
            
            bitmapValue = *bitmapPtr;
            bitmapPtr++;
            bitmapValue2 = *bitmapPtr;

                        
            UWORD middlePixelPattern = (bitmapValue << rightGap) + (bitmapValue2 >> leftGap);
            UWORD middleScreenPattern = ~middlePixelPattern;            
        
            *firstPtr = ((colorBit) & 1)*(middlePixelPattern);
            *secondPtr = ((colorBit >> 1) & 1)*(middlePixelPattern);
            *thirdPtr = ((colorBit >> 2) & 1)*(middlePixelPattern);
            *fourthPtr = ((colorBit >> 3) & 1)*(middlePixelPattern);

#if AMIGA

            firstPtr++;
            secondPtr++;
            thirdPtr++;
            fourthPtr++;
#else

            firstPtr += 4;
            secondPtr += 4;
            thirdPtr += 4;
            fourthPtr += 4;
#endif

        }

        bitmapValue = *bitmapPtr;
        UWORD rightPixelPattern = (bitmapValue << rightGap);
        UWORD rightScreenPattern = ~rightPixelPattern;
        
        
        *firstPtr = ((colorBit) & 1)*(rightPixelPattern);
        *secondPtr = ((colorBit >> 1) & 1)*(rightPixelPattern);
        *thirdPtr = ((colorBit >> 2) & 1)*(rightPixelPattern);
        *fourthPtr = ((colorBit >> 3) & 1)*(rightPixelPattern);

#if AMIGA

        firstPtr++;
        secondPtr++;
        thirdPtr++;
        fourthPtr++;
#else

        firstPtr += 4;
        secondPtr += 4;
        thirdPtr += 4;
        fourthPtr += 4;
#endif

    }

}

void FreeTextBitmap(TextBitMap *Bitmap)
{
    free(Bitmap->BitMap);
    free(Bitmap);
}

void FreeFont()
{
    free(engine.font->CharOffsets);
    free(engine.font->RawData);
    free(engine.font);
}

Font * InitFont(char *FontName)
{
    FILE *FontFile;
	Font *font;

	FontFile = fopen(FontName, "r");

	font = (Font *) malloc(sizeof(Font));

	fread(&font->Width, sizeof(UWORD), 1, FontFile);
	fread(&font->Height, sizeof(UWORD), 1, FontFile);
	fread(&font->Chars, sizeof(UBYTE), 1, FontFile);

	font->CharOffsets = malloc(sizeof(UWORD) * font->Chars);
	fread(font->CharOffsets, sizeof(UWORD) * font->Chars, 1, FontFile);

	font->RawData = (UWORD *)malloc(sizeof(UWORD)*((font->Width+15)/16)*font->Height);

	UWORD PlaneByteSize = ((font->Width+15)/16) * 2 * font->Height;
	fread(font->RawData, PlaneByteSize, 1, FontFile);

	fclose(FontFile);
	return font;
}
