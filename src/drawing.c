#include "engine.h"
#include "settings.h"



#ifdef AMIGA

void ClearBuffor()
{
	for(UWORD p = 0; p< 20*256;p++)
	{
		plane1W[p] = 0;
		plane2W[p] = 0;
		plane3W[p] = 0;
		plane4W[p] = 0;
	}
		
}


void CopyFastToChipW(tBitMap *bm)
{
	CopyMemQuick(plane1W, bm->Planes[0], PLANEWIDTH*PLANEHEIGHT);
	CopyMemQuick(plane2W, bm->Planes[1], PLANEWIDTH*PLANEHEIGHT);
	CopyMemQuick(plane3W, bm->Planes[2], PLANEWIDTH*PLANEHEIGHT);
	CopyMemQuick(plane4W, bm->Planes[3], PLANEWIDTH*PLANEHEIGHT);
}
#else

void ClearBuffor()
{
	for(UWORD p = 0; p< 20*200*4;p++)
		planes[p] = 0;
}
#endif

#define UnpackSpriteByte(byte, value, b1, m1, n1, b2, m2, n2, howMany)  \
if((howMany) < 0)						\
{											\
	(m1) = (m2) = 0;							\
	(n1) = (n2) = 1;							\
} 											\												
else										\
{											\
	(b2) = (byte) & 0x0F;						\
	(m2) = ((b2) ^ (value));			\
	if((m2) > 0)								\
	{										\
		(m2) = 1;								\
	}										\
	(n2) = 1 - (m2);							\
	(b1) = (byte) >> 4;							\
	(m1) = ((b1) ^ (value));			\
	if((m1) > 0)								\
	{										\
		(m1) = 1;								\
	}										\
	(n1) = 1 - (m1);						\
}										\
(howMany)++;	


#define UnpackSpriteByteMinus(byte, value, b1, m1, n1, b2, m2, n2, howMany)  \
if((howMany) < 0)						\
{											\
	(m1) = (m2) = 0;							\
	(n1) = (n2) = 1;							\
} 											\												
else										\
{											\
	(b2) = (byte) & 0x0F;						\
	(m2) = ((b2) ^ (value));			\
	if((m2) > 0)								\
	{										\
		(m2) = 1;								\
	}										\
	(n2) = 1 - (m2);							\
	(b1) = (byte) >> 4;							\
	(m1) = ((b1) ^ (value));			\
	if((m1) > 0)								\
	{										\
		(m1) = 1;								\
	}										\
	(n1) = 1 - (m1);						\
}										\
(howMany)--;

#define BlendSprite(planeValue, planeShift, b, m, n, bitIndex)	\
(((n)*(((planeValue) >> (planeShift)) & 1) + (m)*(((b) >> (bitIndex)) & 1)) << (planeShift))	


void DrawSprite4b(unsigned char *bLogo, BITMAPINFOHEADER *bhLogo, 
					UWORD posX, UWORD posY, WORD spriteIndexX, WORD spriteIndexY,
					UWORD spriteSizeX, UWORD spriteSizeY, unsigned char backgroundValue)
{
	UWORD position;
	unsigned char b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16;
	unsigned char m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16;
	unsigned char n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15, n16;
	unsigned char a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16;
	unsigned char byte;
	ULONG xx, yy;

	WORD planePosX = posX/16;
	BYTE leftGap = posX - planePosX*16;

	UBYTE xSteps = (spriteSizeX/16);


	//position = startOffset;

	UWORD baseX = spriteIndexX*spriteSizeX/16;
	UWORD baseY = spriteIndexY*spriteSizeY;

    #ifdef AMIGA
	position = (posY - (spriteSizeY/2)) * PLANEWIDTHWORD + planePosX - spriteSizeX/32;
    UWORD *firstCol = plane1W + position;
    UWORD *secondCol = plane2W + position;
    UWORD *thirdCol = plane3W + position;
    UWORD *fourthCol = plane4W + position;
    #else 
	position = (posY - (spriteSizeY/2)) * PLANEWIDTHWORD + planePosX*4 - spriteSizeX/32*4;
    UWORD *firstCol = planes + position;
    UWORD *secondCol = planes + position + 1;
    UWORD *thirdCol = planes + position + 2;
    UWORD *fourthCol = planes + position + 3;

    #endif 

	for (ULONG y =baseY+spriteSizeY; y > baseY; y--)
	{
		yy = (y - 1) * bhLogo->biWidth/2;

        UWORD *firstPos = firstCol;
        UWORD *secondPos = secondCol;
        UWORD *thirdPos = thirdCol;
        UWORD *fourthPos = fourthCol;

		BYTE howManyPixels = -leftGap/2;

		for (ULONG x = 0; x <= xSteps; x++)
		{
			if(x < xSteps)
			{
				xx = ((baseX + x) * 8 - (leftGap/2));
                    
                byte = bLogo[xx + yy];
                
                UnpackSpriteByte(byte, backgroundValue, b1, m1, n1, b2, m2, n2, howManyPixels)
                ++xx;
                byte = bLogo[xx + yy];
                
                UnpackSpriteByte(byte, backgroundValue, b3, m3, n3, b4, m4, n4, howManyPixels)
                ++xx;
                byte = bLogo[xx + yy];
                
                UnpackSpriteByte(byte, backgroundValue, b5, m5, n5, b6, m6, n6, howManyPixels)
                ++xx;
                byte = bLogo[xx + yy];
                
                UnpackSpriteByte(byte, backgroundValue, b7, m7, n7, b8, m8, n8, howManyPixels)
                ++xx;
                byte = bLogo[xx + yy];
                
                UnpackSpriteByte(byte, backgroundValue, b9, m9, n9, b10, m10, n10, howManyPixels)
                ++xx;
                byte = bLogo[xx + yy];
                
                UnpackSpriteByte(byte, backgroundValue, b11, m11, n11, b12, m12, n12, howManyPixels)
                ++xx;
                byte = bLogo[xx + yy];
                
                UnpackSpriteByte(byte, backgroundValue, b13, m13, n13, b14, m14, n14, howManyPixels)
                ++xx;
                byte = bLogo[xx + yy];
                
                UnpackSpriteByte(byte, backgroundValue, b15, m15, n15, b16, m16, n16, howManyPixels)
				
			}
			else
			{
				howManyPixels = leftGap/2;
				xx = ((baseX + x) * 8 - (leftGap/2));
                
				byte = bLogo[xx + yy];
			
				UnpackSpriteByteMinus(byte, backgroundValue, b1, m1, n1, b2, m2, n2, howManyPixels)
				++xx;
				byte = bLogo[xx + yy];
				
				UnpackSpriteByteMinus(byte, backgroundValue, b3, m3, n3, b4, m4, n4, howManyPixels)
				++xx;
				byte = bLogo[xx + yy];
				
				UnpackSpriteByteMinus(byte, backgroundValue, b5, m5, n5, b6, m6, n6, howManyPixels)
				++xx;
				byte = bLogo[xx + yy];
				
				UnpackSpriteByteMinus(byte, backgroundValue, b7, m7, n7, b8, m8, n8, howManyPixels)
				++xx;
				byte = bLogo[xx + yy];
				
				UnpackSpriteByteMinus(byte, backgroundValue, b9, m9, n9, b10, m10, n10, howManyPixels)
				++xx;
				byte = bLogo[xx + yy];
				
				UnpackSpriteByteMinus(byte, backgroundValue, b11, m11, n11, b12, m12, n12, howManyPixels)
				++xx;
				byte = bLogo[xx + yy];
				
				UnpackSpriteByteMinus(byte, backgroundValue, b13, m13, n13, b14, m14, n14, howManyPixels)
				++xx;
				byte = bLogo[xx + yy];
				
				UnpackSpriteByteMinus(byte, backgroundValue, b15, m15, n15, b16, m16, n16, howManyPixels)
                
			}
			

			WORD plane1Value = *firstPos;
			WORD plane2Value = *secondPos;
			WORD plane3Value = *thirdPos;
			WORD plane4Value = *fourthPos;

			*firstPos = BlendSprite(plane1Value, 15, b1, m1, n1, 0) +
								BlendSprite(plane1Value, 14, b2, m2, n2, 0) +
								BlendSprite(plane1Value, 13, b3, m3, n3, 0) +
								BlendSprite(plane1Value, 12, b4, m4, n4, 0) +
								BlendSprite(plane1Value, 11, b5, m5, n5, 0) +
								BlendSprite(plane1Value, 10, b6, m6, n6, 0) +
								BlendSprite(plane1Value, 9, b7, m7, n7, 0) +
								BlendSprite(plane1Value, 8, b8, m8, n8, 0) +
								BlendSprite(plane1Value, 7, b9, m9, n9, 0) +
								BlendSprite(plane1Value, 6, b10, m10, n10, 0) +
								BlendSprite(plane1Value, 5, b11, m11, n11, 0) +
								BlendSprite(plane1Value, 4, b12, m12, n12, 0) +
								BlendSprite(plane1Value, 3, b13, m13, n13, 0) +
								BlendSprite(plane1Value, 2, b14, m14, n14, 0) +
								BlendSprite(plane1Value, 1, b15, m15, n15, 0) +
								BlendSprite(plane1Value, 0, b16, m16, n16, 0);
			*secondPos = BlendSprite(plane2Value, 15, b1, m1, n1, 1) +
								BlendSprite(plane2Value, 14, b2, m2, n2, 1) +
								BlendSprite(plane2Value, 13, b3, m3, n3, 1) +
								BlendSprite(plane2Value, 12, b4, m4, n4, 1) +
								BlendSprite(plane2Value, 11, b5, m5, n5, 1) +
								BlendSprite(plane2Value, 10, b6, m6, n6, 1) +
								BlendSprite(plane2Value, 9, b7, m7, n7, 1) +
								BlendSprite(plane2Value, 8, b8, m8, n8, 1) +
								BlendSprite(plane2Value, 7, b9, m9, n9, 1) +
								BlendSprite(plane2Value, 6, b10, m10, n10, 1) +
								BlendSprite(plane2Value, 5, b11, m11, n11, 1) +
								BlendSprite(plane2Value, 4, b12, m12, n12, 1) +
								BlendSprite(plane2Value, 3, b13, m13, n13, 1) +
								BlendSprite(plane2Value, 2, b14, m14, n14, 1) +
								BlendSprite(plane2Value, 1, b15, m15, n15, 1) +
								BlendSprite(plane2Value, 0, b16, m16, n16, 1);
			*thirdPos = BlendSprite(plane3Value, 15, b1, m1, n1, 2) +
								BlendSprite(plane3Value, 14, b2, m2, n2, 2) +
								BlendSprite(plane3Value, 13, b3, m3, n3, 2) +
								BlendSprite(plane3Value, 12, b4, m4, n4, 2) +
								BlendSprite(plane3Value, 11, b5, m5, n5, 2) +
								BlendSprite(plane3Value, 10, b6, m6, n6, 2) +
								BlendSprite(plane3Value, 9, b7, m7, n7, 2) +
								BlendSprite(plane3Value, 8, b8, m8, n8, 2) +
								BlendSprite(plane3Value, 7, b9, m9, n9, 2) +
								BlendSprite(plane3Value, 6, b10, m10, n10, 2) +
								BlendSprite(plane3Value, 5, b11, m11, n11, 2) +
								BlendSprite(plane3Value, 4, b12, m12, n12, 2) +
								BlendSprite(plane3Value, 3, b13, m13, n13, 2) +
								BlendSprite(plane3Value, 2, b14, m14, n14, 2) +
								BlendSprite(plane3Value, 1, b15, m15, n15, 2) +
								BlendSprite(plane3Value, 0, b16, m16, n16, 2);
			*fourthPos = BlendSprite(plane4Value, 15, b1, m1, n1, 3) +
								BlendSprite(plane4Value, 14, b2, m2, n2, 3) +
								BlendSprite(plane4Value, 13, b3, m3, n3, 3) +
								BlendSprite(plane4Value, 12, b4, m4, n4, 3) +
								BlendSprite(plane4Value, 11, b5, m5, n5, 3) +
								BlendSprite(plane4Value, 10, b6, m6, n6, 3) +
								BlendSprite(plane4Value, 9, b7, m7, n7, 3) +
								BlendSprite(plane4Value, 8, b8, m8, n8, 3) +
								BlendSprite(plane4Value, 7, b9, m9, n9, 3) +
								BlendSprite(plane4Value, 6, b10, m10, n10, 3) +
								BlendSprite(plane4Value, 5, b11, m11, n11, 3) +
								BlendSprite(plane4Value, 4, b12, m12, n12, 3) +
								BlendSprite(plane4Value, 3, b13, m13, n13, 3) +
								BlendSprite(plane4Value, 2, b14, m14, n14, 3) +
								BlendSprite(plane4Value, 1, b15, m15, n15, 3) +
								BlendSprite(plane4Value, 0, b16, m16, n16, 3);
			
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
			howManyPixels = 0;
		}

        firstCol += PLANEWIDTHWORD;
        secondCol += PLANEWIDTHWORD;
        thirdCol += PLANEWIDTHWORD;
        fourthCol += PLANEWIDTHWORD;
	}
}


void DrawBitmap4b(unsigned char *bLogo, BITMAPINFOHEADER *bhLogo, UWORD offsety)
{
	UWORD position;
	unsigned char b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16;
	unsigned char byte;
	ULONG xx, yy;

	position = offsety * PLANEWIDTHWORD;
    #ifdef AMIGA
    UWORD *firstCol = plane1W + position;
    UWORD *secondCol = plane2W + position;
    UWORD *thirdCol = plane3W + position;
    UWORD *fourthCol = plane4W + position;
    #else 
    UWORD *firstCol = planes + position;
    UWORD *secondCol = planes + position + 1;
    UWORD *thirdCol = planes + position + 2;
    UWORD *fourthCol = planes + position + 3;

    #endif 

	//position = startOffset;

	for (ULONG y = bhLogo->biHeight; y > 0; y--)
	{
		yy = (y - 1 - offsety) * bhLogo->biWidth/2;
        
        UWORD *firstPos = firstCol;
        UWORD *secondPos = secondCol;
        UWORD *thirdPos = thirdCol;
        UWORD *fourthPos = fourthCol;

		for (ULONG x = 0; x < bhLogo->biWidth / 16; x++)
		{
			xx = x * 8;

			byte = bLogo[xx + yy];
			b2 = byte & 0x0F;
			b1 = byte >> 4;
			byte = bLogo[xx + 1 + yy];
			b4 = byte & 0x0F;
			b3 = byte >> 4;
			byte = bLogo[xx + 2 + yy];
			b6 = byte & 0x0F;
			b5 = byte >> 4;
			byte = bLogo[xx + 3 + yy];
			b8 = byte & 0x0F;
			b7 = byte >> 4;
			byte = bLogo[xx + 4 + yy];
			b10 = byte & 0x0F;
			b9 = byte >> 4;
			byte = bLogo[xx + 5 + yy];
			b12 = byte & 0x0F;
			b11 = byte >> 4;
			byte = bLogo[xx + 6 + yy];
			b14 = byte & 0x0F;
			b13 = byte >> 4;
			byte = bLogo[xx + 7 + yy];
			b16 = byte & 0x0F;
			b15 = byte >> 4;

			*firstPos = ((b1 >> 0) & 1) * 0b1000000000000000 +
								((b2 >> 0) & 1) * 0b0100000000000000 +
								((b3 >> 0) & 1) * 0b0010000000000000 +
								((b4 >> 0) & 1) * 0b0001000000000000 +
								((b5 >> 0) & 1) * 0b0000100000000000 +
								((b6 >> 0) & 1) * 0b0000010000000000 +
								((b7 >> 0) & 1) * 0b0000001000000000 +
								((b8 >> 0) & 1) * 0b0000000100000000 +
								((b9 >> 0) & 1) * 0b0000000010000000 +
								((b10 >> 0) & 1) * 0b0000000001000000 +
								((b11 >> 0) & 1) * 0b0000000000100000 +
								((b12 >> 0) & 1) * 0b0000000000010000 +
								((b13 >> 0) & 1) * 0b0000000000001000 +
								((b14 >> 0) & 1) * 0b0000000000000100 +
								((b15 >> 0) & 1) * 0b0000000000000010 +
								((b16 >> 0) & 1) * 0b0000000000000001;
			*secondPos = ((b1 >> 1) & 1) * 0b1000000000000000 +
								((b2 >> 1) & 1) * 0b0100000000000000 +
								((b3 >> 1) & 1) * 0b0010000000000000 +
								((b4 >> 1) & 1) * 0b0001000000000000 +
								((b5 >> 1) & 1) * 0b0000100000000000 +
								((b6 >> 1) & 1) * 0b0000010000000000 +
								((b7 >> 1) & 1) * 0b0000001000000000 +
								((b8 >> 1) & 1) * 0b0000000100000000 +
								((b9 >> 1) & 1) * 0b0000000010000000 +
								((b10 >> 1) & 1) * 0b0000000001000000 +
								((b11 >> 1) & 1) * 0b0000000000100000 +
								((b12 >> 1) & 1) * 0b0000000000010000 +
								((b13 >> 1) & 1) * 0b0000000000001000 +
								((b14 >> 1) & 1) * 0b0000000000000100 +
								((b15 >> 1) & 1) * 0b0000000000000010 +
								((b16 >> 1) & 1) * 0b0000000000000001;
			*thirdPos = ((b1 >> 2) & 1) * 0b1000000000000000 +
								((b2 >> 2) & 1) * 0b0100000000000000 +
								((b3 >> 2) & 1) * 0b0010000000000000 +
								((b4 >> 2) & 1) * 0b0001000000000000 +
								((b5 >> 2) & 1) * 0b0000100000000000 +
								((b6 >> 2) & 1) * 0b0000010000000000 +
								((b7 >> 2) & 1) * 0b0000001000000000 +
								((b8 >> 2) & 1) * 0b0000000100000000 +
								((b9 >> 2) & 1) * 0b0000000010000000 +
								((b10 >> 2) & 1) * 0b0000000001000000 +
								((b11 >> 2) & 1) * 0b0000000000100000 +
								((b12 >> 2) & 1) * 0b0000000000010000 +
								((b13 >> 2) & 1) * 0b0000000000001000 +
								((b14 >> 2) & 1) * 0b0000000000000100 +
								((b15 >> 2) & 1) * 0b0000000000000010 +
								((b16 >> 2) & 1) * 0b0000000000000001;
			*fourthPos = ((b1 >> 3) & 1) * 0b1000000000000000 +
								((b2 >> 3) & 1) * 0b0100000000000000 +
								((b3 >> 3) & 1) * 0b0010000000000000 +
								((b4 >> 3) & 1) * 0b0001000000000000 +
								((b5 >> 3) & 1) * 0b0000100000000000 +
								((b6 >> 3) & 1) * 0b0000010000000000 +
								((b7 >> 3) & 1) * 0b0000001000000000 +
								((b8 >> 3) & 1) * 0b0000000100000000 +
								((b9 >> 3) & 1) * 0b0000000010000000 +
								((b10 >> 3) & 1) * 0b0000000001000000 +
								((b11 >> 3) & 1) * 0b0000000000100000 +
								((b12 >> 3) & 1) * 0b0000000000010000 +
								((b13 >> 3) & 1) * 0b0000000000001000 +
								((b14 >> 3) & 1) * 0b0000000000000100 +
								((b15 >> 3) & 1) * 0b0000000000000010 +
								((b16 >> 3) & 1) * 0b0000000000000001;
                                
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
		}

        firstCol += PLANEWIDTHWORD;
        secondCol += PLANEWIDTHWORD;
        thirdCol += PLANEWIDTHWORD;
        fourthCol += PLANEWIDTHWORD;
	}
}

void DrawBitmap4bCenter(unsigned char *bLogo, BITMAPINFOHEADER *bhLogo)
{
	UWORD position;
	unsigned char b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16;
	unsigned char byte;
	ULONG xx, yy;

	position = ((PLANEHEIGHT-bhLogo->biHeight)/2) * PLANEWIDTHWORD + (320-bhLogo->biWidth)/32;

    #ifdef AMIGA
	position = ((PLANEHEIGHT-bhLogo->biHeight)/2) * PLANEWIDTHWORD + (320-bhLogo->biWidth)/32;
    UWORD *firstCol = plane1W + position;
    UWORD *secondCol = plane2W + position;
    UWORD *thirdCol = plane3W + position;
    UWORD *fourthCol = plane4W + position;
    #else 
	position = ((PLANEHEIGHT-bhLogo->biHeight)/2) * PLANEWIDTHWORD + (320-bhLogo->biWidth)/32*4;
    UWORD *firstCol = planes + position;
    UWORD *secondCol = planes + position + 1;
    UWORD *thirdCol = planes + position + 2;
    UWORD *fourthCol = planes + position + 3;

    #endif 

	//position = startOffset;

	for (ULONG y = bhLogo->biHeight; y > 0; y--)
	{
		yy = (y - 1) * bhLogo->biWidth/2;

        UWORD *firstPos = firstCol;
        UWORD *secondPos = secondCol;
        UWORD *thirdPos = thirdCol;
        UWORD *fourthPos = fourthCol;

		for (ULONG x = 0; x < bhLogo->biWidth / 16; x++)
		{
			xx = x * 8;

			byte = bLogo[xx + yy];
			b2 = byte & 0x0F;
			b1 = byte >> 4;
			byte = bLogo[xx + 1 + yy];
			b4 = byte & 0x0F;
			b3 = byte >> 4;
			byte = bLogo[xx + 2 + yy];
			b6 = byte & 0x0F;
			b5 = byte >> 4;
			byte = bLogo[xx + 3 + yy];
			b8 = byte & 0x0F;
			b7 = byte >> 4;
			byte = bLogo[xx + 4 + yy];
			b10 = byte & 0x0F;
			b9 = byte >> 4;
			byte = bLogo[xx + 5 + yy];
			b12 = byte & 0x0F;
			b11 = byte >> 4;
			byte = bLogo[xx + 6 + yy];
			b14 = byte & 0x0F;
			b13 = byte >> 4;
			byte = bLogo[xx + 7 + yy];
			b16 = byte & 0x0F;
			b15 = byte >> 4;

			*firstPos = ((b1 >> 0) & 1) * 0b1000000000000000 +
								((b2 >> 0) & 1) * 0b0100000000000000 +
								((b3 >> 0) & 1) * 0b0010000000000000 +
								((b4 >> 0) & 1) * 0b0001000000000000 +
								((b5 >> 0) & 1) * 0b0000100000000000 +
								((b6 >> 0) & 1) * 0b0000010000000000 +
								((b7 >> 0) & 1) * 0b0000001000000000 +
								((b8 >> 0) & 1) * 0b0000000100000000 +
								((b9 >> 0) & 1) * 0b0000000010000000 +
								((b10 >> 0) & 1) * 0b0000000001000000 +
								((b11 >> 0) & 1) * 0b0000000000100000 +
								((b12 >> 0) & 1) * 0b0000000000010000 +
								((b13 >> 0) & 1) * 0b0000000000001000 +
								((b14 >> 0) & 1) * 0b0000000000000100 +
								((b15 >> 0) & 1) * 0b0000000000000010 +
								((b16 >> 0) & 1) * 0b0000000000000001;
			*secondPos = ((b1 >> 1) & 1) * 0b1000000000000000 +
								((b2 >> 1) & 1) * 0b0100000000000000 +
								((b3 >> 1) & 1) * 0b0010000000000000 +
								((b4 >> 1) & 1) * 0b0001000000000000 +
								((b5 >> 1) & 1) * 0b0000100000000000 +
								((b6 >> 1) & 1) * 0b0000010000000000 +
								((b7 >> 1) & 1) * 0b0000001000000000 +
								((b8 >> 1) & 1) * 0b0000000100000000 +
								((b9 >> 1) & 1) * 0b0000000010000000 +
								((b10 >> 1) & 1) * 0b0000000001000000 +
								((b11 >> 1) & 1) * 0b0000000000100000 +
								((b12 >> 1) & 1) * 0b0000000000010000 +
								((b13 >> 1) & 1) * 0b0000000000001000 +
								((b14 >> 1) & 1) * 0b0000000000000100 +
								((b15 >> 1) & 1) * 0b0000000000000010 +
								((b16 >> 1) & 1) * 0b0000000000000001;
			*thirdPos = ((b1 >> 2) & 1) * 0b1000000000000000 +
								((b2 >> 2) & 1) * 0b0100000000000000 +
								((b3 >> 2) & 1) * 0b0010000000000000 +
								((b4 >> 2) & 1) * 0b0001000000000000 +
								((b5 >> 2) & 1) * 0b0000100000000000 +
								((b6 >> 2) & 1) * 0b0000010000000000 +
								((b7 >> 2) & 1) * 0b0000001000000000 +
								((b8 >> 2) & 1) * 0b0000000100000000 +
								((b9 >> 2) & 1) * 0b0000000010000000 +
								((b10 >> 2) & 1) * 0b0000000001000000 +
								((b11 >> 2) & 1) * 0b0000000000100000 +
								((b12 >> 2) & 1) * 0b0000000000010000 +
								((b13 >> 2) & 1) * 0b0000000000001000 +
								((b14 >> 2) & 1) * 0b0000000000000100 +
								((b15 >> 2) & 1) * 0b0000000000000010 +
								((b16 >> 2) & 1) * 0b0000000000000001;
			*fourthPos = ((b1 >> 3) & 1) * 0b1000000000000000 +
								((b2 >> 3) & 1) * 0b0100000000000000 +
								((b3 >> 3) & 1) * 0b0010000000000000 +
								((b4 >> 3) & 1) * 0b0001000000000000 +
								((b5 >> 3) & 1) * 0b0000100000000000 +
								((b6 >> 3) & 1) * 0b0000010000000000 +
								((b7 >> 3) & 1) * 0b0000001000000000 +
								((b8 >> 3) & 1) * 0b0000000100000000 +
								((b9 >> 3) & 1) * 0b0000000010000000 +
								((b10 >> 3) & 1) * 0b0000000001000000 +
								((b11 >> 3) & 1) * 0b0000000000100000 +
								((b12 >> 3) & 1) * 0b0000000000010000 +
								((b13 >> 3) & 1) * 0b0000000000001000 +
								((b14 >> 3) & 1) * 0b0000000000000100 +
								((b15 >> 3) & 1) * 0b0000000000000010 +
								((b16 >> 3) & 1) * 0b0000000000000001;   
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
		}
        firstCol += PLANEWIDTHWORD;
        secondCol += PLANEWIDTHWORD;
        thirdCol += PLANEWIDTHWORD;
        fourthCol += PLANEWIDTHWORD;
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

  WORD leftUpPattern = 0b1000100110010001 >> leftGap;
  WORD rightUpPattern = 0b1111111111111111 << rightGap;

  WORD leftDownPattern = 0b1000100110010001 << rightGap;
  WORD rightDownPattern = 0b1111111111111111 >> leftGap;

#ifdef AMIGA
  plane1W[firstPos] = (leftUpPattern) + (plane1W[firstPos] & (rightUpPattern));
  plane2W[firstPos] = (leftUpPattern) + (plane2W[firstPos] & (rightUpPattern));
  plane3W[firstPos] = (leftUpPattern) + (plane3W[firstPos] & (rightUpPattern));
  plane4W[firstPos] = (leftUpPattern) + (plane4W[firstPos] & (rightUpPattern));
  
  plane1W[secondPos] = (leftDownPattern) + (plane1W[secondPos] & (rightDownPattern));
  plane2W[secondPos] = (leftDownPattern) + (plane2W[secondPos] & (rightDownPattern));
  plane3W[secondPos] = (leftDownPattern) + (plane3W[secondPos] & (rightDownPattern));
  plane4W[secondPos] = (leftDownPattern) + (plane4W[secondPos] & (rightDownPattern));
  #else
  
  planes[firstPos] = (leftUpPattern) + (planes[firstPos] & (rightUpPattern));
  planes[firstPos+1] = (leftUpPattern) + (planes[firstPos+1] & (rightUpPattern));
  planes[firstPos+2] = (leftUpPattern) + (planes[firstPos+2] & (rightUpPattern));
  planes[firstPos+3] = (leftUpPattern) + (planes[firstPos+3] & (rightUpPattern));
  
  planes[secondPos] = (leftDownPattern) + (planes[secondPos] & (rightDownPattern));
  planes[secondPos+1] = (leftDownPattern) + (planes[secondPos+1] & (rightDownPattern));
  planes[secondPos+2] = (leftDownPattern) + (planes[secondPos+2] & (rightDownPattern));
  planes[secondPos+3] = (leftDownPattern) + (planes[secondPos+3] & (rightDownPattern));
  #endif
}

void DrawPixelWord(UWORD x, UWORD y, UBYTE color)
{
    #ifdef AMIGA
  plane1W[y * PLANEWIDTHWORD + x] = ( (color>>0) & 1) * 0xffff;
  plane2W[y * PLANEWIDTHWORD + x] = ( (color>>1) & 1) * 0xffff;
  plane3W[y * PLANEWIDTHWORD + x] = ( (color>>2) & 1) * 0xffff;
  plane4W[y * PLANEWIDTHWORD + x] = ( (color>>3) & 1) * 0xffff;
  #else
  
  planes[y * PLANEWIDTHWORD + x*4] = ( (color>>0) & 1) * 0xffff;
  planes[y * PLANEWIDTHWORD + x*4 + 1] = ( (color>>1) & 1) * 0xffff;
  planes[y * PLANEWIDTHWORD + x*4 + 2] = ( (color>>2) & 1) * 0xffff;
  planes[y * PLANEWIDTHWORD + x*4 + 3] = ( (color>>3) & 1) * 0xffff;
  #endif
}