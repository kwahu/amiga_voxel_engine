#include "engine.h"
#include "settings.h"

void DrawPixel(UWORD x, UWORD y, UBYTE color)
{
  UWORD posX = x/16;
  UWORD leftGap = x - posX*16;
  UWORD rightGap = 16 - leftGap;

  UWORD firstPos = y*PLANEWIDTHWORD+posX;
  UWORD secondPos = firstPos+1;

  WORD leftUpPattern = 0b1000100110010001 >> leftGap;
  WORD rightUpPattern = 0b1111111111111111 << rightGap;

  WORD leftDownPattern = 0b1000100110010001 << rightGap;
  WORD rightDownPattern = 0b1111111111111111 >> leftGap;

  plane1W[firstPos] = (leftUpPattern) + (plane1W[firstPos] & (rightUpPattern));
  plane2W[firstPos] = (leftUpPattern) + (plane2W[firstPos] & (rightUpPattern));
  plane3W[firstPos] = (leftUpPattern) + (plane3W[firstPos] & (rightUpPattern));
  plane4W[firstPos] = (leftUpPattern) + (plane4W[firstPos] & (rightUpPattern));
  
  plane1W[secondPos] = (leftDownPattern) + (plane1W[secondPos] & (rightDownPattern));
  plane2W[secondPos] = (leftDownPattern) + (plane2W[secondPos] & (rightDownPattern));
  plane3W[secondPos] = (leftDownPattern) + (plane3W[secondPos] & (rightDownPattern));
  plane4W[secondPos] = (leftDownPattern) + (plane4W[secondPos] & (rightDownPattern));
}

void DrawPixelWord(UWORD x, UWORD y, UBYTE color)
{
  plane1W[y * PLANEWIDTHWORD + x] = ( (color>>0) & 1) * 0xffff;
  plane2W[y * PLANEWIDTHWORD + x] = ( (color>>1) & 1) * 0xffff;
  plane3W[y * PLANEWIDTHWORD + x] = ( (color>>2) & 1) * 0xffff;
  plane4W[y * PLANEWIDTHWORD + x] = ( (color>>3) & 1) * 0xffff;
}