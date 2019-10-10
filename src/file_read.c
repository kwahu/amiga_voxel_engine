#include "engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <ace/utils/file.h>
static void ReadHeight(const char *name)
{
	UBYTE byte;
	tFile *file;
	file = fileOpen(name, "r");
	if (file) {
		for (int x = 0; x < 256; x++) {
			for (int y = 0; y < 256; y++) {
				fileRead(file, &byte , 1);
				heightMap0[x][y] = byte;
			}
		}
		fileClose(file);
	}
}
static void ReadPalette(const char *name)
{
	char r,g,b;
	tFile *file;
	file = fileOpen(name, "r");
	if (file)
	{
		for(int i=0;i<15;i++)
		{
			fileRead(file, &r, 1);
			fileRead(file, &g, 1);
			fileRead(file, &b, 1);
			kolory[i] = r*256 + g*16 + b;
		}
		fileClose(file);
	}
	kolory[15] = 0xaaf;
}
static void ReadColor(const char *name)
{
	tFile *file;
	file = fileOpen(name, "r");
	if (file) {
		for (int x = 0; x < 256; x++) {
			for (int y = 0; y < 256; y++) {
				fileRead(file, &colorMap0[x][y], 1);
			}
		}
		fileClose(file);
	}
}
