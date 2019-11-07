#include "ray_casting.h"

//calculate paths for raycasts going from the camera
void CalculateRayCasts(WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH], UBYTE xSize, UBYTE ySize)
{
	int sxx;
	int syy;
	int tzz; //depth step value
	int fovX = xSize/20; //this changes the field of view
	int fovY = 4; //4 nice height change

	tzz = 1;
	for(int tz=0;tz<TERRAINDEPTH;tz++)
	{
		//high - 2 - 8
		tzz += debugValue3+tz/debugValue2;//+tz/16; //increase step with the distance from camera
		for(int sx=-xSize/2;sx<xSize/2;sx++)
		{
			sxx = (sx * tzz)/30; //make smaller steps
			for(int sy=-ySize/2;sy<ySize/2;sy++)
			{
				syy = (sy * tzz)/30;//make smaller steps
				rayCastX[xSize/2+sx][tz] = sxx/fovX;
				rayCastY[ySize/2+sy][tz] = syy/fovY;
			}
		}
	}
}
void CalculateRayCastsSingle(WORD *rayCastXY, UBYTE xSize, UBYTE ySize, UBYTE adrSize)
{
	int sxx;
	int syy;
	int tzz; //depth step value
	int fovX = 4;//xSize/20; //this changes the field of view
	int fovY = 4;
	BYTE sxxx, syyy;
	UBYTE xa,ya,ta;
	UWORD address;

	tzz = 1;
	for(UBYTE tz=0;tz<TERRAINDEPTH;tz++)
	{



		tzz += 1; //increase step with the distance from camera
		for(int sx=-xSize/2;sx<xSize/2;sx++)
		{
			sxx = sx * tzz/2; //make smaller steps
sxx = sxx/fovX;

			for(int sy=-ySize/2;sy<ySize/2;sy++)
			{
				syy = sy * tzz/2;//make smaller steps

				syy = syy/fovY;
			//	sxxx = sxx;
				//syyy = syy;

				if(sxx<-127) sxx = -127;
				if(sxx>127) sxx = 127;
				//sxx += 128;
				sxxx = sxx;

				if(syy<-127) syy = -127;
				if(syy>127) syy = 127;
				//syy += 128;
				syyy = syy;

				//rayCastXY[xSize/2+sx][ySize/2+sy][tz] = (sxx<<8) + syy;

				xa = xSize/2+sx;
				ya = ySize/2+sy;
				ta = tz;

				address = (ta<<(2*adrSize)) + (xa<<adrSize) + ya;
				rayCastXY[address] = (sxxx<<8) + syyy;
			//	rayCastXY[xSize/2+sx][ySize/2+sy][tz][0] = sxx;//
			//	rayCastXY[xSize/2+sx][ySize/2+sy][tz][1] = syy;
			//	rayCastXY[xSize/2+sx][ySize/2+sy][tz] = (sxxx<<8) + syyy;
			}
		}

	}
}
/*
void CalculateRayCastsSlow(WORD (*rayCastX)[TERRAINDEPTH], WORD (*rayCastY)[TERRAINDEPTH])
{
	UWORD position = 0;

	for(int y=0;y<32;y++)
	{
		for(int tz=0;tz<TERRAINDEPTH;tz++)
		{
			rayCastYY[position] = rayCastY[y*4][tz];
			position ++;
		}
	}

	position = 0;
	for(int x=0;x<40;x++)
	{
		for(int tz=0;tz<TERRAINDEPTH;tz++)
		{
			rayCastXX[position] = rayCastX[x*4][tz]/4;
			position ++;
		}
	}
}
*/
void CombineMapsLow(UBYTE (*height)[64], UBYTE (*color)[64], UWORD (*map)[256])
{
	for (int x = 0; x < 256; x++) {
		for (int y = 0; y < 256; y++) {
			map[x][y] = (color[x/4][y/4] << 8) + height[x/4][y/4];
		}
	}
}
void CombineMapsMed(UBYTE (*height)[128], UBYTE (*color)[128], UWORD (*map)[256])
{
	for (int x = 0; x < 256; x++) {
		for (int y = 0; y < 256; y++) {
			map[x][y] = (color[x/2][y/2] << 8) + height[x/2][y/2];
		}
	}
}

void CombineMapsHigh(UBYTE (*height)[256], UBYTE (*color)[256], UWORD (*map)[256])
{
	for (int x = 0; x < 256; x++) {
		for (int y = 0; y < 256; y++) {
			map[x][y] = (color[x][y] << 8) + height[x][y];
		}
	}
}
