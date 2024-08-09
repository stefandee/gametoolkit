//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Fx_Static.h"
#include "colors16.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

CFx_Static::CFx_Static()
{
}
//---------------------------------------------------------------------------

void CFx_Static::Update()
{
}
//---------------------------------------------------------------------------

void CFx_Static::Paint(char* _buffer, int _w, int _h)
{
  for(int j = 0 ; j < _h; j++)
  {
    for(int i = 0; i < _w; i++)
    {
      unsigned char lPixelColor = random(255);
      ((__int16*)_buffer)[j * _w + i] = makeRGB16(lPixelColor, lPixelColor, lPixelColor);
    }
  }
}
//---------------------------------------------------------------------------

CFx_Static::~CFx_Static()
{
}
//---------------------------------------------------------------------------

