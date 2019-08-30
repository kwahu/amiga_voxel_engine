#include <stdio.h>
#include <stdlib.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/dos.h>
#include <stddef.h>
#include <hardware/custom.h>

//m68k-amigaos-gcc -s -O3 -noixemul -o engine engine.c

/*
docker run --rm \
-v ${PWD}:/work \
-v /path/to/extra/m68k-amigaos/lib:/tools/usr/lib \
-v /path/to/extra/m68k-amigaos/include:/tools/usr/include \
-it amigadev/crosstools:m68k-amigaos bash
*/

#define WIDTH 320
#define HEIGHT 256
#define DEPTH 4

#define PLANEWIDTH 40

UBYTE height[256][256];
UBYTE fastPlane[DEPTH][PLANEWIDTH*HEIGHT];
UBYTE *test = height;

ULONG kolory[] = { 0x00100000,
	0x00000000, 0x00000000, 0x00000000,
	0x22222222, 0x82222222, 0x22222222,
	0x44444444, 0x94444444, 0x44444444,
	0x66666666, 0x96666666, 0x66666666,
	0x88888888, 0xa8888888, 0x88888888,
	0x90909090, 0xa0909090, 0x90909090,
	0x98989898,	0xb8989898,	0x98989898,
	0xa0a0a0a0,	0xb0a0a0a0,	0xa0a0a0a0,
	0xa8a8a8a8, 0xc8a8a8a8, 0xa8a8a8a8,
	0xb0b0b0b0, 0xc0b0b0b0,	0xb0b0b0b0,
	0xb8b8b8b8, 0xd8b8b8b8,	0xb8b8b8b8,
	0xc0c0c0c0, 0xd0c0c0c0,	0xc0c0c0c0,
	0xc8c8c8c8, 0xe8c8c8c8,	0xc8c8c8c8,
	0xd0d0d0d0, 0xe0d0d0d0,	0xd0d0d0d0,
	0xeeeeeeee, 0xfeeeeeee,	0xeeeeeeee,
	0xffffffff, 0xffffffff,	0xffffffff,
	0x00000000
};

float mapScaleH;
float mapScaleX;
float mapScaleY;
int xStart, yStart;
int xPos, yPos;
int currentHeight = 10;
int heightAheadRange;
int flightHeight;
int horizon;
int speed = 1;
int pixelsDrawn = 0;
int pixelsChecked = 0;
int pixelsRead = 0;
int linesDrawn = 0;
UWORD *address;
UWORD position;

void ReadFile(char name[])
{
	int c;
	FILE *file;
	file = fopen(name, "r");
	if (file) {
		for (int x = 0; x < 256; x++)
		for (int y = 0; y < 256; y++)
		{
			height[x][y] = getc(file);
		}
		fclose(file);
	}
}

void ClearFastPlane(struct BitMap *bm)
{
	for(int p=0;p<DEPTH;p++) {
		CopyMemQuick(fastPlane[p], bm->Planes[p], PLANEWIDTH*HEIGHT);
	}
}

#define TERRAINDEPTH 150
#define XSIZE 20
#define YSIZE 16
#define PIXELHEIGHT 8

WORD rayCastX[XSIZE*2][TERRAINDEPTH];
WORD rayCastY[YSIZE*2][TERRAINDEPTH];

void CalculateTables()
{
	float tzz;
	int sxx;
	int syy;

	for(float tz=1;tz<TERRAINDEPTH;tz++)
	{
		tzz = tz/8;
		for(int sx=-XSIZE;sx<XSIZE;sx++)
		{
			sxx = sx * tzz;

			for(int sy=-YSIZE;sy<YSIZE;sy++)
			{
				syy = sy * tzz;
				rayCastX[XSIZE+(int)sx][(int)tz] = sxx;
				rayCastY[YSIZE+(int)sy][(int)tz] = syy;
			}
		}
	}
}

void DrawTerrain(struct BitMap *bm,struct RastPort *rp)
{
	currentHeight = (currentHeight*9 + flightHeight + height[xPos][yPos+heightAheadRange])/10;
	UBYTE color;
	UBYTE sx,sy,tz,line,plane;
	WORD rayHeight;
	UBYTE tx;//screen x offest on terrain - perspective change with depth
	UBYTE ty;
	UBYTE th;

	for(sx=0;sx<XSIZE*2;sx++)
	{
		sy = 0;
		tz = 0;
		while(tz < TERRAINDEPTH && sy <YSIZE*2)
		{
			pixelsChecked++;
			rayHeight = currentHeight + rayCastY[sy][tz];
			tx = xPos + rayCastX[sx][tz];//screen x offest on terrain - perspective change with depth
			ty = yPos + tz;
			th = height[tx][ty];
			//th = test[(tx << 8) | ty];
			//height to look for at a given x,y terrain coordinate accounting for z depth
			//************************************************************8
			if(th>rayHeight)
			{
				//pixelsDrawn++;
				color = 1+th/16;
				position = ((YSIZE*2-sy)*PIXELHEIGHT)*PLANEWIDTH + sx;//OK
				//******************DRAW
				for(line=0;line<PIXELHEIGHT;line++)
				{
					for(plane=0;plane<DEPTH;plane++)
					{
						if((color>>plane) & 1) fastPlane[plane][position] = 0xff; else fastPlane[plane][position] = 0x00;//OK
					}
					position+=40;
				}
				//******************DRAW
				sy++;
				//tz++;
			}
			else
			{
				tz++;
			}
		}
		//finish vertical line with 0x00
		while(sy < YSIZE*2)
		{
			sy++;
			position = ((YSIZE*2-sy)*PIXELHEIGHT)*PLANEWIDTH + sx;//OK
			//******************DRAW
			for(line=0;line<PIXELHEIGHT;line++)
			{
				for(plane=0;plane<DEPTH;plane++)
				{
					fastPlane[plane][position] = 0xff;
				}
				position+=40;
			}
		}

	}
}

int main(void)
{
	struct Screen *screen;

	screen = OpenScreenTags(NULL,
		SA_DisplayID, PAL_MONITOR_ID|LORES_KEY,
		SA_Width,     WIDTH,
		SA_Height,    HEIGHT,
		SA_Depth,     DEPTH,
		SA_Quiet,     TRUE,
		TAG_DONE);


	//	horizon = HEIGHT / 2;

	//	mapScaleH = 10;
	//	mapScaleX = 4;
	//	mapScaleY = 4;

		xStart = 64;
		yStart = 0;

		xPos = xStart;
		yPos = yStart;

		heightAheadRange = 5;

		flightHeight = 0;

		currentHeight = flightHeight;

		ReadFile("height.raw");

		if (screen)
		{
			struct RastPort *rp = &screen->RastPort;
			struct ViewPort *vp = &screen->ViewPort;
			struct BitMap *bm = rp->BitMap;
			LoadRGB32(vp, kolory);

			CalculateTables();

			for(int i=0;i<200;i++)
			{
				DrawTerrain(bm,rp);
				ClearFastPlane(bm);
				flightHeight += 1;
				yPos += 1;
				//xPos -= 1;
			}
			//Delay(100);
			CloseScreen(screen);
		}
		printf("seconds=%ld pixelsDrawn=%d pixelsChecked=%d pixelsRead=%d linesDrawn=%d", 0,pixelsDrawn, pixelsChecked, pixelsRead, linesDrawn);

		return 0;
	}
