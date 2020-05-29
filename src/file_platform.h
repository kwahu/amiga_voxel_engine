#pragma once


#ifdef AMIGA

typedef BPTR File;

File OpenFile(UBYTE *filename)
{
    return Open(filename, 1005);
}

ULONG FileGetSize(UBYTE *filename)
{
    return fileGetSize(filename);
}

void ReadFile(File file, void *mem, ULONG size)
{
    Read(file, mem, size);
}

void CloseFile(File file)
{
    Close(file);
}

#else

typedef long File;

File OpenFile(UBYTE *filename)
{
    return Fopen(filename, 0);
}

// ULONG FileGetSize(File file)
// {
//     fseek(file, 0, SEEK_END);
//     ULONG res = ftell(file);
//     fseek(file, 0, SEEK_SET);
//     return res;
// }

void ReadFile(File file, void *mem, ULONG size)
{
    Fread(file, size, mem);
}

void CloseFile(File file)
{
    Fclose(file);
}

#endif