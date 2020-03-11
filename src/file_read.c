
// void ReadHeight(const char *name, UBYTE (*map)[MAPSIZE])
// {
// 	UBYTE byte;
// 	FILE *file;
// 	file = fopen(name, "r");
// 	if (file) {
// 		//printf("HEIGHT FILE OPENED\r\n");
// 		for (int x = 0; x < MAPSIZE; x++) {
// 			for (int y = 0; y < MAPSIZE; y++) {
// 				fread(&byte , sizeof(UBYTE),1,file);
// 				map[x][y] = byte;
// 			}
// 		}
// 		fclose(file);
// 	}
// }

// void ReadColor(const char *name, UBYTE (*map)[MAPSIZE])
// {
// 	UBYTE byte;
// 	FILE *file;
// 	file = fopen(name, "r");
// 	if (file) {
// 		//printf("COLOR FILE OPENED\r\n");
// 		for (int x = 0; x < MAPSIZE; x++) {
// 			for (int y = 0; y < MAPSIZE; y++) {
// 				fread(&byte , sizeof(UBYTE),1,file);
// 				map[x][y] = byte;
// 			}
// 		}
// 		fclose(file);
// 	}
// }
