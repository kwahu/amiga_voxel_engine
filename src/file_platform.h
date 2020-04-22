#pragma once

void LoadBitmapToMemory(BYTE *fileName)    
{                                      
    #ifdef AMIGA                   
    UseSystem();                        
    engine.activeBitmap = LoadBitmapFile(fileName, &engine.activeBitmapHeader, engine.activePalette, 1, 0);      
    UnuseSystem();        
    #else                       
    free(engine.activeBitmap);      
    engine.activeBitmap = LoadBitmapFile(fileName, &engine.activeBitmapHeader, engine.activePalette, 1, 0);     
    #endif                  
}   

#ifdef AMIGA
ULONG GetFileSize(char *fileName)
{
    SystemUse();
	BPTR lock = Lock((CONST_STRPTR)fileName, ACCESS_READ);
	if(!lock) {
		SystemUnuse();
		return -1;
	}
	struct FileInfoBlock fileBlock;
	LONG result = Examine(lock, &fileBlock);
	UnLock(lock);
	SystemUnuse();
	if(result == DOSFALSE) {
		return -1;
	}
	return fileBlock.fib_Size;
}

FILE * OpenFile(char *fileName, char *mode)
{
    SystemUse();
	FILE *file = fopen(fileName, mode);
	SystemUnuse();
	return file;
}

ULONG ReadFile(FILE *file, void *dest, ULONG size) {
	SystemUse();
	ULONG result = fread(dest, size, 1, file);
	SystemUnuse();
	return result;
}

void CloseFile(FILE *file)
{
    SystemUse();
	fclose(file);
	SystemUnuse();
}
#endif