//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Fx_Static2.h"
#include "colors16.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

CFx_Static2::CFx_Static2() : CFx()
{
}
//---------------------------------------------------------------------------

void CFx_Static2::Update()
{
}
//---------------------------------------------------------------------------

void CFx_Static2::Paint()
{
  if (!mFxCanvas)
  {
    return;
  }

  // start of crap DX code----
  DDSURFACEDESC2 lSurfDesc;
  HRESULT        lHRet;

  lSurfDesc.dwSize = sizeof(lSurfDesc);
  lHRet = mFxCanvas->Lock(NULL, &lSurfDesc, DDLOCK_WAIT, NULL);
  if (lHRet !=DD_OK)
  {
    return;
  }

  if (lSurfDesc.ddpfPixelFormat.dwRGBBitCount != 16)
  {
    return;
  }

  unsigned char* lBuffer = (unsigned char*)lSurfDesc.lpSurface;
  // ----end of crap DX code

  // must unroll this loop sometime - odd/even Width
  for(int j = 0 ; j < mFxWidth; j++)
  {
    for(int i = 0; i < mFxHeight; i++)
    {
      // unsigned char lPixelColor = random(255);
      //((__int16*)lBuffer)[j * lSurfDesc.lPitch / 2 + i] = makeRGB16(lPixelColor, lPixelColor, lPixelColor);
      unsigned __int8  lPixelColor = random(255);
      unsigned __int16 lColor16    = makeRGB16(lPixelColor, lPixelColor, lPixelColor);
      unsigned __int32 lDouble     = (lColor16 << 16) | lColor16;
      ((__int32*)lBuffer)[j * lSurfDesc.lPitch / 4 + i] = lDouble;
    }
  }

  // some small crap code again
  mFxCanvas->Unlock(NULL);
}
//---------------------------------------------------------------------------

CFx_Static2::~CFx_Static2()
{
}
//---------------------------------------------------------------------------

