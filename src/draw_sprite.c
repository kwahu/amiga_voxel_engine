#include "engine.h"
#include "settings_amiga.h"

void DrawPixel(UWORD x, UWORD y, UBYTE color)
{
  plane1W[y * PLANEWIDTHWORD + x] = 0b1000100110010001;
  plane2W[y * PLANEWIDTHWORD + x] = 0b1000100110010001;
  plane3W[y * PLANEWIDTHWORD + x] = 0b1000100110010001;
  plane4W[y * PLANEWIDTHWORD + x] = 0b1000100110010001;
}

void DrawPixelWord(UWORD x, UWORD y, UBYTE color)
{
  plane1W[y * PLANEWIDTHWORD + x] = ( (color>>0) & 1) * 0xffff;
  plane2W[y * PLANEWIDTHWORD + x] = ( (color>>1) & 1) * 0xffff;
  plane3W[y * PLANEWIDTHWORD + x] = ( (color>>2) & 1) * 0xffff;
  plane4W[y * PLANEWIDTHWORD + x] = ( (color>>3) & 1) * 0xffff;
}