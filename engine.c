#include <stdio.h>
#include <stdlib.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/dos.h>
#include <stddef.h>
#include <hardware/custom.h>

//---------------------------------------------------------------------- DEFINES

// First OS interrupt is at offset 0x64 - each is 4 bytes wide
#define SYSTEM_INT_VECTOR_FIRST (0x64/4)
#define SYSTEM_INT_VECTOR_COUNT 7
#define SYSTEM_INT_HANDLER_COUNT 15


// Saved regs
UWORD s_uwOsIntEna;
UWORD s_uwOsDmaCon;
UWORD s_uwAceDmaCon = 0;
UWORD s_uwOsInitialDma;


//#define REGPTR volatile * const
#define CUSTOM_BASE 0xDFF000
#define INTENAR 0xDFF09A //DFF01C //http://amiga-dev.wikidot.com/hardware:intenar
#define INTREQ 0xDFF09C //http://amiga-dev.wikidot.com/hardware:intreqr
#define DMACON 0xDFF096 //http://amiga-dev.wikidot.com/hardware:dmaconr

//typedef struct Custom tCustom;
//tCustom FAR REGPTR g_pCustom = (tCustom REGPTR)CUSTOM_BASE;

//m68k-amigaos-gcc -s -O2 -noixemul -m68020 -o engine2 engine2.c

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
UBYTE alpha[WIDTH/8][HEIGHT/8];
UBYTE color[WIDTH/8][HEIGHT/8];
UBYTE fastPlane[DEPTH][PLANEWIDTH*HEIGHT];


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

int divTableMaxH = 256;
float mapScaleH;
float mapScaleX;
float mapScaleY;
float xStart, yStart;
float xPos, yPos;
int currentHeight;
int heightAheadRange;
float flightHeight;
int horizon;
float speed = 1;
int pixelsDrawn = 0;
int pixelsChecked = 0;
int pixelsRead = 0;
int linesDrawn = 0;
UBYTE col[DEPTH];
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
	for(int p=0;p<DEPTH;p++)
	for(int i=0;i<PLANEWIDTH*HEIGHT;i++)
	{
		*(bm->Planes[p] + i) = fastPlane[p][i];
		fastPlane[p][i] = 0x00;
	}
}
void ClearAlpha()
{
	for (int x = 0; x < WIDTH; x+=8)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			alpha[x][y] = 0;
		}
	}
}
void WritePlane(struct BitMap *bm, char plane, UWORD position, char bit, char color)
{
	UWORD value;
	UWORD *address;

	address = bm->Planes[plane] + position;
	value = *address;

	if(color) {
		value = value | (1 << bit);
	} else {
		value = value & ~(1 << bit);
	}
	*address = value;
}


void SetPixel(struct BitMap *bm, int x, int y, UBYTE color)
{
	UWORD position = y*bm->BytesPerRow + x/8;

	//calculate pixel to change in a word
	UWORD bit =  8 - x % 8;
	//change only selected pixel

	WritePlane(bm, 0, position, bit, color & 1);
	WritePlane(bm, 1, position, bit, (color>>1) & 1);
	WritePlane(bm, 2, position, bit, (color>>2) & 1);
	WritePlane(bm, 3, position, bit, (color>>3) & 1);
	//WritePlane(bm, 4, position, bit, 1); //alpha

	//	alpha[x][y] = 1;
}
#define TERRAINDEPTH 30

WORD rayCastX[40][TERRAINDEPTH];
WORD rayCastY[32][TERRAINDEPTH];

void CalculateTables()
{
	for(float sx=-20;sx<20;sx++)
	for(float sy=-16;sy<16;sy++)
	for(float tz=1;tz<TERRAINDEPTH;tz++)
	{
		rayCastX[20+(int)sx][(int)tz] = (WORD)(sx * tz/8);
		rayCastY[16+(int)sy][(int)tz] = (WORD)(sy * tz/8);
	}
}
void DrawTerrain2(struct BitMap *bm,struct RastPort *rp)
{
	//currentHeight = flightHeight + height[(int)(xPos * mapScaleX)][(int)((yPos+heightAheadRange) * mapScaleY )];
	UBYTE color;
	BYTE sx,sy,tz,line,plane;
	WORD rayHeight;
	UBYTE tx;//screen x offest on terrain - perspective change with depth
	UBYTE ty;
	UBYTE th;



	for(sx=0;sx<40;sx++)
	{
		sy = 0;
		tz = 0;
		while(tz < TERRAINDEPTH)
		{
			pixelsChecked++;
			rayHeight = flightHeight + rayCastY[sy][tz];
			tx = xPos + rayCastX[sx][tz] ;//screen x offest on terrain - perspective change with depth
			ty = yPos + tz;
			th = height[tx][ty];
			//height to look for at a given x,y terrain coordinate accounting for z depth
			//************************************************************8
			if(th>rayHeight)
			{
				pixelsDrawn++;
				color = 1+th/16;
				position = ((31-sy)*8)*40 + sx;
				//******************DRAW
				for(line=0;line<8;line++)
				{
					for(plane=0;plane<DEPTH;plane++)
					{
						if((color>>plane) & 1) fastPlane[plane][position] = 0xff; else fastPlane[plane][position] = 0x00;
					}
					position+=40;
				}
				//******************DRAW
				sy++;
				tz++;
			}
			else
			{
				tz++;
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


		horizon = HEIGHT / 2;

		mapScaleH = 10;
		mapScaleX = 4;
		mapScaleY = 4;

		xStart = 50;
		yStart = 0;

		xPos = xStart;
		yPos = yStart;

		heightAheadRange = 2;

		flightHeight = 0;

		ReadFile("height.raw");

		// save the state of the hardware registers (INTENA, DMA, ADKCON etc.)
	//	s_uwOsIntEna = g_pCustom->intenar;
		//s_uwOsDmaCon = g_pCustom->dmaconr;
		//s_uwOsInitialDma = s_uwOsDmaCon;

		// Disable interrupts (this is the actual "kill system/OS" part)
	/*	address = INTENAR;
		*address = 0x7FFF;
		address  = INTREQ;
		*address = 0x7FFF;*/
		//address = DMACON;
		//*address = 0x7FFF;

		// Disable all DMA - only once
		// Wait for vbl before disabling sprite DMA
	//	while (!(g_pCustom->intreqr & INTF_VERTB)) {}
	//	g_pCustom->dmacon = 0x07FF;

		if (screen)
		{
			struct RastPort *rp = &screen->RastPort;
			struct ViewPort *vp = &screen->ViewPort;
			struct BitMap *bm = rp->BitMap;
			LoadRGB32(vp, kolory);



			CalculateTables();

			for(int i=0;i<100;i++)
			{
				DrawTerrain2(bm,rp);

				ClearFastPlane(bm);
				//flightHeight += 1;
				yPos += 1;
				//xPos += 10;
			}


			//Delay(100);
			CloseScreen(screen);
		}

		printf("seconds=%ld pixelsDrawn=%d pixelsChecked=%d pixelsRead=%d linesDrawn=%d", 0,pixelsDrawn, pixelsChecked, pixelsRead, linesDrawn);

		return 0;
	}

	//  map_height = height[(int)x % 256][(int)y % 256];
	//    pixelsRead++;

	// ********** table [height,z] -> value
	// a - number of
	/*   if ((-map_height + currentHeight) < 0)
	{
	height_on_screen = -heightDiv[(int)( -(-map_height + currentHeight) )][a] ;
}
else
{
height_on_screen = heightDiv[(int)( (-map_height + currentHeight) )][a] ;
}*/


//height_on_screen = horizon + -map_height + currentHeight;
//height_on_screen = horizon + ( ( (-map_height + currentHeight) )  ) ;




/*
void DrawTerrain(struct BitMap *bm,struct RastPort *rp)
{
//  currentHeight = //(currentHeight + (flightHeight + height[(int)(xPos * mapScaleX)][(int)((yPos+heightAheadRange) * mapScaleY )])) / 2;
currentHeight = flightHeight + height[(int)(xPos * mapScaleX)][(int)((yPos+heightAheadRange) * mapScaleY )];

int height_on_screen = 0;
int map_height = 0;
float dx = 0;
float dxSum;
int z = 0;
int pleft;
int pright;

int y;
int x;

int drawStart;
int drawEnd;

int bigestZ = 0, bigestH = 0;



char color;
for (int a = 0; a < TERRAINDEPTH; a++)
{
z = zLines[a];
//linesDrawn++;

pleft = -z + xPos;
pright = z + xPos;
dx = (float)(pright - pleft) / (float)WIDTH * 8;
y = (-z + yPos) * mapScaleY;

// ************** tablica zapisu danych na ekranie 90 stopni
drawStart = 0;//(WIDTH - zLength[a]) / 2;
drawEnd = WIDTH;//(WIDTH - (WIDTH - zLength[a] - 2) / 2);
//******************
dxSum = drawStart * dx;

for (int xs = drawStart; xs < drawEnd; xs += 8)
{
x = tableMul[(pleft + (int)dxSum)];
map_height = height[x][y];
height_on_screen = heightDiv[256 + currentHeight - map_height][a];
if (height_on_screen < HEIGHT)
{
//set colors for bit planes
color = tableDiv8[map_height]+1;
for(int ii=0;ii<DEPTH;ii++)
if((color>>ii) & 1)
col[ii] = 0xff;
else
col[ii] = 0x00;

for (int b = 0; b < HEIGHT; b++)
{
if (height_on_screen > 0 && height_on_screen < HEIGHT)
{
if (alpha[xs][height_on_screen])
b=HEIGHT; //exit if already drawn
else
{
for(int a=0;a<8;a++)
{
position = tableBytesPerRow[height_on_screen] + tableDiv8[xs]; //1s
alpha[xs][height_on_screen] = 1;
for(int iii=0;iii<DEPTH;iii++)
{
//*(bm->Planes[iii] + position) = col[iii];
fastPlane[iii][position] = col[iii];
}
height_on_screen++;
}
}
}
}
}
dxSum += dx;
}
}
}*/
/*
UBYTE GetPixel(int x,int y)
{
return alpha[x][y];
}

void DrawVerticalLine(struct BitMap *bm, int x, int y, int pixelSize, char color)
{
int yy;
int height = HEIGHT;
pixelSize = 1; //bypass

for (int a = 0; a < pixelSize; a++)
{
yy = y;
for (int b = 0; b < height; b++)
{
//+ (int)(height * depth) + drawingHeightOffset + currentHeight - b;
if (yy > 0)
{
++pixelsChecked;

if (yy < height)
{
if (!(b % 8) && GetPixel(x + a, yy) )
b=height; //exit if already drawn
else
{
SetPixelByte(bm,x + a, yy, color);
++pixelsDrawn;
}

}

}
yy++;
}
}
}

void DrawVerticalByte(struct BitMap *bm, struct RastPort *rp, int x, int y, char color)
{
//set colors for bit planes
for(int i=0;i<DEPTH;i++)
if((color>>i) & 1)
col[i] = 0xff;
else
col[i] = 0x00;

for (int b = 0; b < HEIGHT; b++)
{
if (y > 0)
if (y < HEIGHT)
{
if (alpha[x][y])
b=height; //exit if already drawn
else
{
for(int a=0;a<8;a++)
{
position = tableBytesPerRow[y] + tableDiv8[x];
alpha[x][y] = 1;
for(int i=0;i<DEPTH;i++)
{
*(bm->Planes[i] + position) = col[i];
}
y++;
}
}
}
}
}

void WritePlane(struct BitMap *bm, char plane, UWORD position, char bit, char color)
{
UWORD value;
UWORD *address;

address = bm->Planes[plane] + position;
value = *address;

if(color) {
value = value | (1 << bit);
} else {
value = value & ~(1 << bit);
}
*address = value;
}

char ReadPlane(struct BitMap *bm, char plane, UWORD position, char bit)
{
UWORD value;
UWORD *address;

address = bm->Planes[plane] + position;
value = *address;

return value & (1<<bit);
}

void SetPixel(struct BitMap *bm, int x, int y, UBYTE color)
{
UWORD position = y*bm->BytesPerRow + x/8;

//calculate pixel to change in a word
UWORD bit =  8 - x % 8;
//change only selected pixel

WritePlane(bm, 0, position, bit, color & 1);
WritePlane(bm, 1, position, bit, (color>>1) & 1);
WritePlane(bm, 2, position, bit, (color>>2) & 1);
WritePlane(bm, 3, position, bit, (color>>3) & 1);
//WritePlane(bm, 4, position, bit, 1); //alpha

alpha[x][y] = 1;
}

void SetPixelByte(struct BitMap *bm, int x, int y, UBYTE color)
{
UWORD position = tableBytesPerRow[y] + tableDiv8[x];
UBYTE col;
UWORD *address;

for(int i=0;i<DEPTH;i++)
{

if((color>>i) & 1)
col = 0xff;
else
col = 0x00;
address = bm->Planes[i];
for(int a=0;a<8;a++)
//*(bm->Planes[i] + position) = col;
*address++ = col;
}
alpha[x][y] = 1;
}
*/
void ClearScr(struct BitMap *bm)
{
	UWORD address;
	/*	for(int i=0;i<bm->BytesPerRow * HEIGHT;i++)
	{

	for(int p=0;p<DEPTH;p++)
	{
	*(bm->Planes[p] + i) = 0x00;
}
}*/

for(int depth=0; depth<DEPTH; depth++)
{
	UBYTE *displaymem = (UBYTE *)bm->Planes[depth];
	BltClear(displaymem, (bm->BytesPerRow * bm->Rows), 1L);
}
}



/*char GetPixel(struct BitMap *bm, int x, int y)
{
UWORD position = y*bm->BytesPerRow + x/8;

//calculate pixel to change in a word
UWORD bit =  8 - x % 8;
//change only selected pixel

return ReadPlane(bm, 4, position, bit);
}*/

void DrawHeightMap(struct BitMap *bm)
{
	for (int x = 0; x < 256; x++)
	for (int y = 0; y < 256; y++)
	{

		SetPixel(bm,  x,  y, height[x][y]/16);
		//SetAPen(rp, height[x][y] / 16);
		//WritePixel(rp, x, y);
	}
}
