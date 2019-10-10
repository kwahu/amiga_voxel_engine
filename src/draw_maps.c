//draws a map of the terrain straight to planes
//reads 16 pixels and writes them in 1 WORD
static void DrawColorMap(UBYTE player)
{
	UWORD p1,p2,p3,p4;
	UWORD position;
	UBYTE b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16;
	UBYTE xx,yy;
	UBYTE offsetx,offsety;
	UWORD startOffset;

	if(player == 1)
	{
		offsety = p1y/4;
		startOffset = 5;
	}
	else
	{
		offsety = p2y/4;
		startOffset = 15;
	}

	position = startOffset+192*20;

	for (int y = 0; y < 64; y++)
	{
		yy = (y - offsety) % 64;

		for (int x = 0; x < 64/16; x++)
		{
			xx = x*16;
			//if(xx > 48) xx -= 48;

			b1 = colorMap2[(UBYTE)(xx)][yy];
			b2 = colorMap2[(UBYTE)(xx+1)][yy];
			b3 = colorMap2[(UBYTE)(xx+2)][yy];
			b4 = colorMap2[(UBYTE)(xx+3)][yy];
			b5 = colorMap2[(UBYTE)(xx+4)][yy];
			b6 = colorMap2[(UBYTE)(xx+5)][yy];
			b7 = colorMap2[(UBYTE)(xx+6)][yy];
			b8 = colorMap2[(UBYTE)(xx+7)][yy];
			b9 = colorMap2[(UBYTE)(xx+8)][yy];
			b10 = colorMap2[(UBYTE)(xx+9)][yy];
			b11 = colorMap2[(UBYTE)(xx+10)][yy];
			b12 = colorMap2[(UBYTE)(xx+11)][yy];
			b13 = colorMap2[(UBYTE)(xx+12)][yy];
			b14 = colorMap2[(UBYTE)(xx+13)][yy];
			b15 = colorMap2[(UBYTE)(xx+14)][yy];
			b16 = colorMap2[(UBYTE)(xx+15)][yy];
			fastPlane1W[position] = ((b1>>0) & 1) *0b1000000000000000+
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
			fastPlane2W[position] = ((b1>>1) & 1) *0b1000000000000000+
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
			fastPlane3W[position] = ((b1>>2) & 1) *0b1000000000000000+
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
			fastPlane4W[position] = ((b1>>3) & 1) *0b1000000000000000+
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
			position++;
		}
		position+=PLANEWIDTH/2-4;
	}
}
//draws a height map of the terrain straight to planes
//reads 16 pixels and writes them in 1 WORD
static void DrawHeightMap(UBYTE player)
{
	UWORD p1,p2,p3,p4;
	UWORD position;
	UBYTE b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16;
	UBYTE xx,yy;
	UBYTE offsetx,offsety;
	UBYTE xposition;
	UWORD startOffset;

	if(player == 1)
	{
		offsety = p1y/4;
		offsetx = p1x/4;
		startOffset = 0;
	}
	else
	{
		offsety = p2y/4;
		offsetx = p2x/4;
		startOffset = 10;
	}



	position = startOffset+256*20;
	for (int y = 0; y < 64; y++)
	{
		yy = (y - offsety) % 64;

		for (int x = 0; x < 64/16; x++)
		{
			xx = x*16;
			//if(xx > 48) xx -= 48;

			b1 = heightMap2[(UBYTE)(xx)][yy]/16;
			b2 = heightMap2[(UBYTE)(xx+1)][yy]/16;
			b3 = heightMap2[(UBYTE)(xx+2)][yy]/16;
			b4 = heightMap2[(UBYTE)(xx+3)][yy]/16;
			b5 = heightMap2[(UBYTE)(xx+4)][yy]/16;
			b6 = heightMap2[(UBYTE)(xx+5)][yy]/16;
			b7 = heightMap2[(UBYTE)(xx+6)][yy]/16;
			b8 = heightMap2[(UBYTE)(xx+7)][yy]/16;
			b9 = heightMap2[(UBYTE)(xx+8)][yy]/16;
			b10 = heightMap2[(UBYTE)(xx+9)][yy]/16;
			b11 = heightMap2[(UBYTE)(xx+10)][yy]/16;
			b12 = heightMap2[(UBYTE)(xx+11)][yy]/16;
			b13 = heightMap2[(UBYTE)(xx+12)][yy]/16;
			b14 = heightMap2[(UBYTE)(xx+13)][yy]/16;
			b15 = heightMap2[(UBYTE)(xx+14)][yy]/16;
			b16 = heightMap2[(UBYTE)(xx+15)][yy]/16;

			if(offsety == yy && x == offsetx/16)
			{
				xposition = offsetx % 16;
				switch(xposition)
				{
					case 0: b1 = 0x1f;break;
					case 1: b2 = 0x1f;break;
					case 2: b3 = 0x1f;break;
					case 3: b4 = 0x1f;break;
					case 4: b5 = 0x1f;break;
					case 5: b6 = 0x1f;break;
					case 6: b7 = 0x1f;break;
					case 7: b8 = 0x1f;break;
					case 8: b9 = 0x1f;break;
					case 9: b10 = 0x1f;break;
					case 10: b11 = 0x1f;break;
					case 11: b12 = 0x1f;break;
					case 12: b13 = 0x1f;break;
					case 13: b14 = 0x1f;break;
					case 14: b15 = 0x1f;break;
					case 15: b16 = 0x1f;break;
				}
			}

			fastPlane1W[position] = ((b1>>0) & 1) *0b1000000000000000+
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
			fastPlane2W[position] = ((b1>>1) & 1) *0b1000000000000000+
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
			fastPlane3W[position] = ((b1>>2) & 1) *0b1000000000000000+
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
			fastPlane4W[position] = ((b1>>3) & 1) *0b1000000000000000+
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
			position++;
		}
		position-=PLANEWIDTH/2-4;
	}
}