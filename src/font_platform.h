#pragma once

#ifdef AMIGA


Font * InitFont(char *fileName)
{
    return fontCreate(fileName);
}

Font *CreateFontBitmap(Font *font)
{
    return fontCreateTextBitMapFromStr(font, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
}

Font *CreateBitmapFromText(Font *font, char * text)
{
    return fontCreateTextBitMapFromStr(font, text);
}

void FillTextBitmap(Font *font, TextBitmap *Bitmap, char *Text)
{
    
	fontFillTextBitMap(font, Bitmap, Text);
}

void DrawTextBitmap(TextBitmap *Bitmap, UWORD x, UWORD y, UBYTE colorBit)
{
    fontDrawTextBitMap(engine.platformScreen.s_pBuffer->pBack, Bitmap, x, y, colorBit, FONT_LEFT|FONT_COOKIE);
}

void FreeTextBitmap(TextBitmap *Bitmap)
{
    fontDestroyTextBitMap(Bitmap);
}

void FreeFont()
{
	fontDestroy(engine.font);

}

#else

Font *CreateFontBitmap(Font *font)
{
    return 0;
}
Font *CreateBitmapFromText(Font *font, char * text)
{
    return 0;
}


void FillTextBitmap(Font *font, TextBitmap *Bitmap, char *Text)
{
    

}

void DrawTextBitmap(TextBitmap *Bitmap, UWORD x, UWORD y, UBYTE colorBit)
{

}

void FreeTextBitmap(TextBitmap *Bitmap)
{
}

Font * InitFont(char *fileName)
{
    return 0;
}
#endif