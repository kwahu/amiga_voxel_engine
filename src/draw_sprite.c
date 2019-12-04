#include "engine.h"
#include "settings_amiga.h"

void DrawPixel(UWORD x, UWORD y, UBYTE color)
{
  UWORD posX = x/16;
  UWORD leftGap = x - posX*16;
  UWORD rightGap = 16 - leftGap;
  plane1W[y * PLANEWIDTHWORD + posX] = (0b1000100110010001 >> leftGap) + (plane1W[y * PLANEWIDTHWORD + posX] & (0b1111111111111111 << rightGap));
  plane2W[y * PLANEWIDTHWORD + posX] = (0b1000100110010001 >> leftGap) + (plane2W[y * PLANEWIDTHWORD + posX] & (0b1111111111111111 << rightGap));
  plane3W[y * PLANEWIDTHWORD + posX] = (0b1000100110010001 >> leftGap) + (plane3W[y * PLANEWIDTHWORD + posX] & (0b1111111111111111 << rightGap));
  plane4W[y * PLANEWIDTHWORD + posX] = (0b1000100110010001 >> leftGap) + (plane4W[y * PLANEWIDTHWORD + posX] & (0b1111111111111111 << rightGap));
  
  plane1W[y * PLANEWIDTHWORD + posX + 1] = (0b1000100110010001 << rightGap) + (plane1W[y * PLANEWIDTHWORD + posX + 1] & (0b1111111111111111 >> leftGap));
  plane2W[y * PLANEWIDTHWORD + posX + 1] = (0b1000100110010001 << rightGap) + (plane2W[y * PLANEWIDTHWORD + posX + 1] & (0b1111111111111111 >> leftGap));
  plane3W[y * PLANEWIDTHWORD + posX + 1] = (0b1000100110010001 << rightGap) + (plane3W[y * PLANEWIDTHWORD + posX + 1] & (0b1111111111111111 >> leftGap));
  plane4W[y * PLANEWIDTHWORD + posX + 1] = (0b1000100110010001 << rightGap) + (plane4W[y * PLANEWIDTHWORD + posX + 1] & (0b1111111111111111 >> leftGap));
}

void DrawPixelWord(UWORD x, UWORD y, UBYTE color)
{
  plane1W[y * PLANEWIDTHWORD + x] = ( (color>>0) & 1) * 0xffff;
  plane2W[y * PLANEWIDTHWORD + x] = ( (color>>1) & 1) * 0xffff;
  plane3W[y * PLANEWIDTHWORD + x] = ( (color>>2) & 1) * 0xffff;
  plane4W[y * PLANEWIDTHWORD + x] = ( (color>>3) & 1) * 0xffff;
}