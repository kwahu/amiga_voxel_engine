#include "engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <ace/utils/file.h>
void ReadHeight(const char *name, UBYTE (*map)[256])
{
	UBYTE byte;
	tFile *file;
	file = fileOpen(name, "r");
	if (file) {
		for (int x = 0; x < 256; x++) {
			for (int y = 0; y < 256; y++) {
				fileRead(file, &map[x][y] , 1);
				//map[x][y] = byte;
			}
		}
		fileClose(file);
	}
}

void ReadPalette(const char *name)
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
void ReadColor(const char *name, UBYTE (*map)[256])
{
	UBYTE byte;
	tFile *file;
	file = fileOpen(name, "r");
	if (file) {
		for (int x = 0; x < 256; x++) {
			for (int y = 0; y < 256; y++) {
				fileRead(file, &map[x][y], 1);
				//map[x][y] = byte;
			}
		}
		fileClose(file);
	}
}
