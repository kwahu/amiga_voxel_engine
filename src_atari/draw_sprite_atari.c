#include "../src/engine.h"
#include "settings_atari.h"

void DrawPixel(UWORD x, UWORD y, UBYTE color)
{
  planes[y * 80 + x*4] = 0b1000100110010001;
  planes[y * 80 + x*4+1] = 0b1000100110010001;
  planes[y * 80 + x*4+2] = 0b1000100110010001;
  planes[y * 80 + x*4+3] = 0b1000100110010001;
}

void DrawPixelWord(UWORD x, UWORD y, UBYTE color)
{
  planes[y * 80 + x*4] = ( (color>>0) & 1) * 0xffff;
  planes[y * 80 + x*4+1] = ( (color>>1) & 1) * 0xffff;
  planes[y * 80 + x*4+2] = ( (color>>2) & 1) * 0xffff;
  planes[y * 80 + x*4+3] = ( (color>>3) & 1) * 0xffff;
}