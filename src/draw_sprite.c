#include "engine.h"
#include "settings_amiga.h"

void DrawPixel(UWORD x, UWORD y, UBYTE color)
{
  plane1W[y * PLANEWIDTHWORD + x] = 0b1000100110010001;
  plane2W[y * PLANEWIDTHWORD + x] = 0b1000100110010001;
  plane3W[y * PLANEWIDTHWORD + x] = 0b1000100110010001;
  plane4W[y * PLANEWIDTHWORD + x] = 0b1000100110010001;
}
