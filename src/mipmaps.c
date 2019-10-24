#include "ray_casting.h"

void CalculateColorMipMaps()
{
	UWORD value;
	for (int x = 0; x < 128; x++)
	for (int y = 0; y < 128; y++)
	{
		value = 0;
		for(int a = 0; a<2 ;a++)
		for(int b = 0; b<2 ;b++)
		value += colorMap0[x*2+a][y*2+b];
		colorMap1[x][y] = value/4;
	}
	for (int x = 0; x < 64; x++)
	for (int y = 0; y < 64; y++)
	{
		value = 0;
		for(int a = 0; a<4 ;a++)
		for(int b = 0; b<4 ;b++)
		value += colorMap0[x*4+a][y*4+b];
		colorMap2[x][y] = value/16;
	}
	for (int x = 0; x < 32; x++)
	for (int y = 0; y < 32; y++)
	{
		value = 0;
		for(int a = 0; a<8 ;a++)
		for(int b = 0; b<8 ;b++)
		value += colorMap0[x*8+a][y*8+b];
		colorMap3[x][y] = value/64;
	}
	for (int x = 0; x < 16; x++)
	for (int y = 0; y < 16; y++)
	{
		value = 0;
		for(int a = 0; a<16 ;a++)
		for(int b = 0; b<16 ;b++)
		value += colorMap0[x*16+a][y*16+b];
		colorMap4[x][y] = value/256;
	}
}
void CalculateHeightMipMaps()
{
	int value;
	for (int x = 0; x < 128; x++)
	for (int y = 0; y < 128; y++)
	{
		value = 0;
		for(int a = 0; a<2 ;a++)
		for(int b = 0; b<2 ;b++)
		value += heightMap0[x*2+a][y*2+b];
		value /= 4;
		//value -= 5; //gradually lower the height so that it better blends with the horizon
		if(value < 0) value = 0;
		heightMap1[x][y] = value;
	}
	for (int x = 0; x < 64; x++)
	for (int y = 0; y < 64; y++)
	{
		value = 0;
		for(int a = 0; a<4 ;a++)
		for(int b = 0; b<4 ;b++)
		value += heightMap0[x*4+a][y*4+b];
		value /= 16;
		//value -= 15;//gradually lower the height so that it better blends with the horizon
		if(value < 0) value = 0;
		heightMap2[x][y] = value;
	}
	for (int x = 0; x < 32; x++)
	for (int y = 0; y < 32; y++)
	{
		value = 0;
		for(int a = 0; a<8 ;a++)
		for(int b = 0; b<8 ;b++)
		value += heightMap0[x*8+a][y*8+b];
		value /= 64;
		//value -= 30;//gradually lower the height so that it better blends with the horizon
		if(value < 0) value = 0;
		heightMap3[x][y] = value;
	}
	for (int x = 0; x < 16; x++)
	for (int y = 0; y < 16; y++)
	{
		value = 0;
		for(int a = 0; a<16 ;a++)
		for(int b = 0; b<16 ;b++)
		value += heightMap0[x*16+a][y*16+b];
		value /= 256;
		//value -= 50;//gradually lower the height so that it better blends with the horizon
		if(value < 0) value = 0;
		heightMap4[x][y] = value;
	}
}
