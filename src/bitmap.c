#include "engine.h"

ULONG ConvertEndianLONG(ULONG number)
{
 

    return ((number>>24)&0xff) | // move byte 3 to byte 0
                    ((number<<8)&0xff0000) | // move byte 1 to byte 2
                    ((number>>8)&0xff00) | // move byte 2 to byte 1
                    ((number<<24)&0xff000000);
}
UWORD ConvertEndianWORD(UWORD number)
{
 

    return  (number>>8) | (number<<8);
}

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char bitmapColorTable[16])
{
    FILE *filePtr; //our file pointer
    BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
    unsigned char *bitmapImage;  //store image data
    int imageIdx=0;  //image index counter
    unsigned char tempRGB;  //our swap variable

    //open filename in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL)
        return NULL;

    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,filePtr);

    bitmapFileHeader.bfType = ConvertEndianWORD(bitmapFileHeader.bfType);
    bitmapFileHeader.bfSize = ConvertEndianLONG(bitmapFileHeader.bfSize);
    bitmapFileHeader.bfOffBits = ConvertEndianLONG(bitmapFileHeader.bfOffBits);

    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        fclose(filePtr);
        printf("verify that this is a bmp file by check bitmap id=%lu",bitmapFileHeader.bfType);
        return NULL;
    }

    //read the bitmap info header
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr); // small edit. forgot to add the closing bracket at sizeof

    bitmapInfoHeader->biSize = ConvertEndianLONG(bitmapInfoHeader->biSize);
    bitmapInfoHeader->biWidth = ConvertEndianLONG(bitmapInfoHeader->biWidth);
    bitmapInfoHeader->biHeight = ConvertEndianLONG(bitmapInfoHeader->biHeight);
    bitmapInfoHeader->biPlanes = ConvertEndianWORD(bitmapInfoHeader->biPlanes);
    bitmapInfoHeader->biBitCount = ConvertEndianWORD(bitmapInfoHeader->biBitCount);
    bitmapInfoHeader->biCompression = ConvertEndianLONG(bitmapInfoHeader->biCompression);
    bitmapInfoHeader->biSizeImage = ConvertEndianLONG(bitmapInfoHeader->biSizeImage);
    bitmapInfoHeader->biClrUsed = ConvertEndianLONG(bitmapInfoHeader->biClrUsed);
    bitmapInfoHeader->biClrImportant = ConvertEndianLONG(bitmapInfoHeader->biClrImportant);

  //read the bitmap color table
    fread(bitmapColorTable, 4*16,1,filePtr); // small edit. forgot to add the closing bracket at sizeof



    //move file point to the begging of bitmap data
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

    //verify memory allocation
    if (!bitmapImage)
    {
        printf("verify memory allocation");
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    //read in the bitmap image data
    fread(bitmapImage,bitmapInfoHeader->biSizeImage,1,filePtr);

	unsigned char byte;
    for(int i=0;i<bitmapInfoHeader->biSizeImage/3;i++)
    {
        byte = bitmapImage[i*3+2];
        bitmapImage[i*3+2] = bitmapImage[i*3];
        bitmapImage[i*3] = byte;
    }

    // UBYTE byte;
    //read in the bitmap image data
    // for(int i=0;i<256*256;i++)
    // {
    //     fileRead(filePtr, &byte , 1);
	// 			bitmapImage[i] = byte;
    // }

    //make sure bitmap image data was read
    if (bitmapImage == NULL)
    {
        printf("make sure bitmap image data was read");
        fclose(filePtr);
        return NULL;
    }

    //swap the r and b values to get RGB (bitmap is BGR)
    for (imageIdx = 0;imageIdx < bitmapInfoHeader->biSizeImage;imageIdx+=3) // fixed semicolon
    {
        tempRGB = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }

    //close file and return bitmap iamge data
    fclose(filePtr);
    return bitmapImage;
}



