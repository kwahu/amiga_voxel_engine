#include "engine.h"

void DrawPixel(UWORD x, UWORD y, UBYTE color)
{
  plane1W[y*20  + x] = 0b1000100110010001;
  plane2W[y*20  + x] = 0b1000100110010001;
  plane3W[y*20  + x] = 0b1000100110010001;
  plane4W[y*20  + x] = 0b1000100110010001;
}
