//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Fx_Rain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

CFx_Rain::CFx_Rain() : CFx_Weather()
{
}
//---------------------------------------------------------------------------

void CFx_Rain::Update()
{
  for(int i = 0; i < mParticlesCount; i++)
  {
    mParticles[i].mX += mParticles[i].mSpeedX + mWindSpeed;
    mParticles[i].mY += mParticles[i].mSpeedY;

    if (mParticles[i].mY > mParticles[i].mEndY)
    {
      GenerateParticle(i);
    }

    if (mParticles[i].mX > mFxWidth)
    {
      GenerateParticle(i);
    }

    if (mParticles[i].mX < 0)
    {
      GenerateParticle(i);
    }
  }
}
//---------------------------------------------------------------------------

void CFx_Rain::Paint()
{
  if (!GetEnabled())
  {
    return;
  }

  if (!mFxCanvas)
  {
    return;
  }

  GetGraphicInstance()->FillSurface(mFxCanvas, 0);

  /*
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

  for(int i = 0; i < mParticlesCount; i++)
  {
    if ((mParticles[i].mX >= 0) && (mParticles[i].mX < mFxWidth - 1) &&
        (mParticles[i].mY >= 0) && (mParticles[i].mY < mFxHeight - 1))
    {

      ((__int16*)lBuffer)[mParticles[i].mX + (lSurfDesc.lPitch * mParticles[i].mY) / 2] = makeRGB16(150, 150, 150);
    }
  }

  // some small crap code again
  mFxCanvas->Unlock(NULL);
  */
  
  for(int i = 0; i < mParticlesCount; i++)
  {
    if ((mParticles[i].mX >= 0) && (mParticles[i].mX < mFxWidth - 1) &&
        (mParticles[i].mY >= 0) && (mParticles[i].mY < mFxHeight - 1))
    {
      TPoint lStart = Point(mParticles[i].mX, mParticles[i].mY);
      TPoint lEnd   = Point(mParticles[i].mX - mParticles[i].mSpeedX - mWindSpeed, mParticles[i].mY - mParticles[i].mSpeedY);

      GetGraphicInstance()->Line(NULL, lStart, lEnd);
    }
  }
}
//---------------------------------------------------------------------------

void CFx_Rain::GenerateParticle(int _index)
{
  if (!mParticles)
  {
    return;
  }

  if ((_index < 0) || (_index >= mParticlesCount))
  {
    return;
  }

  mParticles[_index].mX         = random(GetWidth());
  mParticles[_index].mEndY      = random(mFxHeight);
  mParticles[_index].mY         = random(mParticles[_index].mEndY);
  mParticles[_index].mParams[0] = 0;
  mParticles[_index].mParams[1] = random(15);

  mParticles[_index].mSpeedX = 0;
  mParticles[_index].mSpeedY = (random(5) + 9);
}
//---------------------------------------------------------------------------


