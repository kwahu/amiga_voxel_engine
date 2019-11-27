#include "engine.h"
#include <stdio.h>
#include <stdlib.h>

void ReadHeight(const char *name, UBYTE (*map)[256])
{
	UBYTE byte;
	FILE *file;
	file = fopen(name, "r");
	if (file) {
		//printf("HEIGHT FILE OPENED\r\n");
		for (int x = 0; x < 256; x++) {
			for (int y = 0; y < 256; y++) {
				fread(&byte , sizeof(UBYTE),1,file);
				map[x][y] = byte;
			}
		}
		fclose(file);
	}
}

// void ReadPalette(const char *name)
// {
// 	char r,g,b;
// 	tFile *file;
// 	file = fileOpen(name, "r");
// 	if (file)
// 	{
// 		for(int i=0;i<15;i++)
// 		{
// 			fileRead(file, &r, 1);
// 			fileRead(file, &g, 1);
// 			fileRead(file, &b, 1);
// 			kolory[i] = r*256 + g*16 + b;
// 		}
// 		fileClose(file);
// 	}
// 	kolory[15] = 0xaaf;
// }
void ReadColor(const char *name, UBYTE (*map)[256])
{
	UBYTE byte;
	FILE *file;
	file = fopen(name, "r");
	if (file) {
		//printf("COLOR FILE OPENED\r\n");
		for (int x = 0; x < 256; x++) {
			for (int y = 0; y < 256; y++) {
				fread(&byte , sizeof(UBYTE),1,file);
				map[x][y] = byte;
			}
		}
		fclose(file);
	}
}
