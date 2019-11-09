void DrawPixel(UWORD x, UWORD y, UBYTE color)
{
  plane1W[y*20  + x] = 0xff;
  plane2W[y*20  + x] = 0xff;
  plane3W[y*20  + x] = 0xff;
  plane4W[y*20  + x] = 0xff;
}
