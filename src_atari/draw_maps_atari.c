#include "../src/engine.h"

void ClearScreen()
{
	for(UWORD p = 0; p< 20*200*4;p++)
		planes[p] = 0;
}

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

	WORD planePosX = (posX/16);
	BYTE leftGap = posX - (planePosX*16);

	UBYTE xSteps = (spriteSizeX/16);
	
	UWORD baseX = spriteIndexX*xSteps;
	UWORD baseY = spriteIndexY*spriteSizeY;



	for (ULONG y = baseY+spriteSizeY; y > baseY; y--)
	{
		yy = (y - 1) * bhLogo->biWidth/2;
		position = (spriteSizeY/2 - (y - baseY - posY)) * PLANEWIDTHWORD*4 + planePosX*4 - (spriteSizeX/32)*4;

		WORD howManyPixels = -leftGap/2;

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

				xx = ((baseX + x) * 8 - (leftGap/2));
			}
			
			

			WORD plane1Value = planes[position];
			WORD plane2Value = planes[position+1];
			WORD plane3Value = planes[position+2];
			WORD plane4Value = planes[position+3];


			
			planes[position] = BlendSprite(plane1Value, 15, b1, m1, n1, 0) +
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
			planes[position+1] = BlendSprite(plane2Value, 15, b1, m1, n1, 1) +
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
			planes[position+2] = BlendSprite(plane3Value, 15, b1, m1, n1, 2) +
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
			planes[position+3] = BlendSprite(plane4Value, 15, b1, m1, n1, 3) +
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
			position+=4;
			howManyPixels = 0;
		}
		position+=PLANEWIDTHWORD*4;
	}
}

void DrawBitmap4b(unsigned char *bLogo, BITMAPINFOHEADER *bhLogo)
{
	UWORD position;
	unsigned char b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16;
	unsigned char byte;
	ULONG xx, yy;
	UBYTE offsety;
	UWORD startOffset;

	offsety = 0;
	startOffset = 0;

	position = startOffset;

	for (ULONG y = bhLogo->biHeight; y > 0; y--)
	{
		yy = (y - 1 - offsety) * bhLogo->biWidth/2;
		position = (bhLogo->biHeight - (y - offsety)) * PLANEWIDTHWORD*4;

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

			planes[position+0] = ((b1 >> 0) & 1) * 0b1000000000000000 +
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
			planes[position+1] = ((b1 >> 1) & 1) * 0b1000000000000000 +
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
			planes[position+2] = ((b1 >> 2) & 1) * 0b1000000000000000 +
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
			planes[position+3] = ((b1 >> 3) & 1) * 0b1000000000000000 +
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
			position+=4;
		}
		//position+=PLANEWIDTH/2;
	}
}
void DrawBitmap4bCenter(unsigned char *bLogo, BITMAPINFOHEADER *bhLogo)
{
	UWORD position;
	unsigned char b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16;
	unsigned char byte;
	ULONG xx, yy;



	//position = startOffset;

	for (ULONG y = bhLogo->biHeight; y > 0; y--)
	{
		yy = (y - 1) * bhLogo->biWidth/2;
		position = (bhLogo->biHeight - (y - (PLANEHEIGHT-bhLogo->biHeight)/2)) * PLANEWIDTHWORD*4 + (320-bhLogo->biWidth)/32*4;

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

			planes[position+0] = ((b1 >> 0) & 1) * 0b1000000000000000 +
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
			planes[position+1] = ((b1 >> 1) & 1) * 0b1000000000000000 +
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
			planes[position+2] = ((b1 >> 2) & 1) * 0b1000000000000000 +
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
			planes[position+3] = ((b1 >> 3) & 1) * 0b1000000000000000 +
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
			position+=4;
		}
		//position+=PLANEWIDTH/2;
	}
}
void DrawBitmap8b(unsigned char *bLogo, BITMAPINFOHEADER *bhLogo)
{
	UWORD position;
	unsigned char b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16;
	unsigned char byte;
	ULONG xx,yy;
	UBYTE offsety;
	UWORD startOffset;

	offsety = 0;
	startOffset = 0;

	position = startOffset;

	for (ULONG y = bhLogo->biHeight; y > 0; y--)
	{
		yy = (y - 1 - offsety) * bhLogo->biWidth;
		position = (bhLogo->biHeight - (y - offsety)) * PLANEWIDTH/2 * 4;

		for (ULONG x = 0; x < bhLogo->biWidth/16; x++)
		{
			xx = x*16;
	

			b1 = bLogo[xx+yy];
			b2 = bLogo[xx+1+yy];
			b3 = bLogo[xx+2+yy];
			b4 = bLogo[xx+3+yy];
			b5 = bLogo[xx+4+yy];
			b6 = bLogo[xx+5+yy];
			b7 = bLogo[xx+6+yy];
			b8 = bLogo[xx+7+yy];
			b9 = bLogo[xx+8+yy];
			b10 = bLogo[xx+9+yy];
			b11 = bLogo[xx+10+yy];
			b12 = bLogo[xx+11+yy];
			b13 = bLogo[xx+12+yy];   
			b14 = bLogo[xx+13+yy];
			b15 = bLogo[xx+14+yy];
			b16 = bLogo[xx+15+yy];
			


			planes[position+0] = ((b1>>0) & 1) *0b1000000000000000+
			((b2>>0) & 1) *0b0100000000000000+
			((b3>>0) & 1) *0b0010000000000000+
			((b4>>0) & 1) *0b0001000000000000+
			((b5>>0) & 1) *0b0000100000000000+
			((b6>>0) & 1) *0b0000010000000000+
			((b7>>0) & 1) *0b0000001000000000+
			((b8>>0) & 1) *0b0000000100000000+
			((b9>>0) & 1) *0b0000000010000000+
			((b10>>0) & 1) *0b0000000001000000+
			((b11>>0) & 1) *0b0000000000100000+
			((b12>>0) & 1) *0b0000000000010000+
			((b13>>0) & 1) *0b0000000000001000+
			((b14>>0) & 1) *0b0000000000000100+
			((b15>>0) & 1) *0b0000000000000010+
			((b16>>0) & 1) *0b0000000000000001;
			planes[position+1] = ((b1>>1) & 1) *0b1000000000000000+
			((b2>>1) & 1) *0b0100000000000000+
			((b3>>1) & 1) *0b0010000000000000+
			((b4>>1) & 1) *0b0001000000000000+
			((b5>>1) & 1) *0b0000100000000000+
			((b6>>1) & 1) *0b0000010000000000+
			((b7>>1) & 1) *0b0000001000000000+
			((b8>>1) & 1) *0b0000000100000000+
			((b9>>1) & 1) *0b0000000010000000+
			((b10>>1) & 1) *0b0000000001000000+
			((b11>>1) & 1) *0b0000000000100000+
			((b12>>1) & 1) *0b0000000000010000+
			((b13>>1) & 1) *0b0000000000001000+
			((b14>>1) & 1) *0b0000000000000100+
			((b15>>1) & 1) *0b0000000000000010+
			((b16>>1) & 1) *0b0000000000000001;
			planes[position+2] = ((b1>>2) & 1) *0b1000000000000000+
			((b2>>2) & 1) *0b0100000000000000+
			((b3>>2) & 1) *0b0010000000000000+
			((b4>>2) & 1) *0b0001000000000000+
			((b5>>2) & 1) *0b0000100000000000+
			((b6>>2) & 1) *0b0000010000000000+
			((b7>>2) & 1) *0b0000001000000000+
			((b8>>2) & 1) *0b0000000100000000+
			((b9>>2) & 1) *0b0000000010000000+
			((b10>>2) & 1) *0b0000000001000000+
			((b11>>2) & 1) *0b0000000000100000+
			((b12>>2) & 1) *0b0000000000010000+
			((b13>>2) & 1) *0b0000000000001000+
			((b14>>2) & 1) *0b0000000000000100+
			((b15>>2) & 1) *0b0000000000000010+
			((b16>>2) & 1) *0b0000000000000001;
			planes[position+3] = ((b1>>3) & 1) *0b1000000000000000+
			((b2>>3) & 1) *0b0100000000000000+
			((b3>>3) & 1) *0b0010000000000000+
			((b4>>3) & 1) *0b0001000000000000+
			((b5>>3) & 1) *0b0000100000000000+
			((b6>>3) & 1) *0b0000010000000000+
			((b7>>3) & 1) *0b0000001000000000+
			((b8>>3) & 1) *0b0000000100000000+
			((b9>>3) & 1) *0b0000000010000000+
			((b10>>3) & 1) *0b0000000001000000+
			((b11>>3) & 1) *0b0000000000100000+
			((b12>>3) & 1) *0b0000000000010000+
			((b13>>3) & 1) *0b0000000000001000+
			((b14>>3) & 1) *0b0000000000000100+
			((b15>>3) & 1) *0b0000000000000010+
			((b16>>3) & 1) *0b0000000000000001;
			position+=4;
		}
		//position+=PLANEWIDTH/2;
	}
}

void DrawColorMap(UBYTE (*map)[MAPSIZE])
{
	UWORD position;
	unsigned char b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16;
	unsigned char byte;
	ULONG xx,yy;
	UBYTE offsety;
	UWORD startOffset;

	offsety = 0;
	startOffset = 0;

	position = startOffset;

	for (ULONG y=0; y < MAPSIZE; y++)
	{
		//yy = (y - 1) * 256*4;
		position = y * PLANEWIDTH/2 * 4;

		for (ULONG x = 0; x < MAPSIZE/16; x++)
		{
			xx = x*16;

			b1 = map[xx+0][y];
			b2 = map[xx+1][y];
			b3 = map[xx+2][y];
			b4 = map[xx+3][y];
			b5 = map[xx+4][y];
			b6 = map[xx+5][y];
			b7 = map[xx+6][y];
			b8 = map[xx+7][y];
			b9 = map[xx+8][y];
			b10 = map[xx+9][y];
			b11 = map[xx+10][y];
			b12 = map[xx+11][y];
			b13 = map[xx+12][y];  
			b14 = map[xx+13][y];
			b15 = map[xx+14][y];
			b16 = map[xx+15][y];
			


			planes[position+0] = ((b1>>0) & 1) *0b1000000000000000+
			((b2>>0) & 1) *0b0100000000000000+
			((b3>>0) & 1) *0b0010000000000000+
			((b4>>0) & 1) *0b0001000000000000+
			((b5>>0) & 1) *0b0000100000000000+
			((b6>>0) & 1) *0b0000010000000000+
			((b7>>0) & 1) *0b0000001000000000+
			((b8>>0) & 1) *0b0000000100000000+
			((b9>>0) & 1) *0b0000000010000000+
			((b10>>0) & 1) *0b0000000001000000+
			((b11>>0) & 1) *0b0000000000100000+
			((b12>>0) & 1) *0b0000000000010000+
			((b13>>0) & 1) *0b0000000000001000+
			((b14>>0) & 1) *0b0000000000000100+
			((b15>>0) & 1) *0b0000000000000010+
			((b16>>0) & 1) *0b0000000000000001;
			planes[position+1] = ((b1>>1) & 1) *0b1000000000000000+
			((b2>>1) & 1) *0b0100000000000000+
			((b3>>1) & 1) *0b0010000000000000+
			((b4>>1) & 1) *0b0001000000000000+
			((b5>>1) & 1) *0b0000100000000000+
			((b6>>1) & 1) *0b0000010000000000+
			((b7>>1) & 1) *0b0000001000000000+
			((b8>>1) & 1) *0b0000000100000000+
			((b9>>1) & 1) *0b0000000010000000+
			((b10>>1) & 1) *0b0000000001000000+
			((b11>>1) & 1) *0b0000000000100000+
			((b12>>1) & 1) *0b0000000000010000+
			((b13>>1) & 1) *0b0000000000001000+
			((b14>>1) & 1) *0b0000000000000100+
			((b15>>1) & 1) *0b0000000000000010+
			((b16>>1) & 1) *0b0000000000000001;
			planes[position+2] = ((b1>>2) & 1) *0b1000000000000000+
			((b2>>2) & 1) *0b0100000000000000+
			((b3>>2) & 1) *0b0010000000000000+
			((b4>>2) & 1) *0b0001000000000000+
			((b5>>2) & 1) *0b0000100000000000+
			((b6>>2) & 1) *0b0000010000000000+
			((b7>>2) & 1) *0b0000001000000000+
			((b8>>2) & 1) *0b0000000100000000+
			((b9>>2) & 1) *0b0000000010000000+
			((b10>>2) & 1) *0b0000000001000000+
			((b11>>2) & 1) *0b0000000000100000+
			((b12>>2) & 1) *0b0000000000010000+
			((b13>>2) & 1) *0b0000000000001000+
			((b14>>2) & 1) *0b0000000000000100+
			((b15>>2) & 1) *0b0000000000000010+
			((b16>>2) & 1) *0b0000000000000001;
			planes[position+3] = ((b1>>3) & 1) *0b1000000000000000+
			((b2>>3) & 1) *0b0100000000000000+
			((b3>>3) & 1) *0b0010000000000000+
			((b4>>3) & 1) *0b0001000000000000+
			((b5>>3) & 1) *0b0000100000000000+
			((b6>>3) & 1) *0b0000010000000000+
			((b7>>3) & 1) *0b0000001000000000+
			((b8>>3) & 1) *0b0000000100000000+
			((b9>>3) & 1) *0b0000000010000000+
			((b10>>3) & 1) *0b0000000001000000+
			((b11>>3) & 1) *0b0000000000100000+
			((b12>>3) & 1) *0b0000000000010000+
			((b13>>3) & 1) *0b0000000000001000+
			((b14>>3) & 1) *0b0000000000000100+
			((b15>>3) & 1) *0b0000000000000010+
			((b16>>3) & 1) *0b0000000000000001;
			position+=4;
		}
		//position+=PLANEWIDTH/2;
	}
}