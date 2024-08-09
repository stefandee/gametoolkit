//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Fx_Drain.h"
#include "logfile.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

CFx_Drain::CFx_Drain() : CFx()
{
  mDrainsCount = 16;
  mTailLength  = 48;
  mHeadColor   = 0xFFFFFF;
  mTailColor   = 0x000000;

  for(int i = 0; i < mDrainsCount; i++)
  {
    GenerateDrain(i);
  }
}
//---------------------------------------------------------------------------

void CFx_Drain::Update()
{
  if (!GetEnabled())
  {
    return;
  }

  for(int i = 0; i < mDrainsCount; i++)
  {
    if (mDrains[i].mHead.x == mDrains[i].mTrail[mDrains[i].mTrailGoal].x)
    {
      mDrains[i].mHead.y += mDrains[i].mSpeed;

      if (mDrains[i].mHead.y >= mDrains[i].mTrail[mDrains[i].mTrailGoal].y)
      {
        mDrains[i].mHead.y = mDrains[i].mTrail[mDrains[i].mTrailGoal].y;
        mDrains[i].mTrailGoal++;

        if (mDrains[i].mTrailGoal > mDrains[i].mTrailLength)
        {
          GenerateDrain(i);
          continue;
        }
      }

      if (mDrains[i].mHead.y - mTailLength > mFxHeight)
      {
        GenerateDrain(i);
      }
    }
    else
    {
      if (mDrains[i].mHead.x > mDrains[i].mTrail[mDrains[i].mTrailGoal].x)
      {
        mDrains[i].mHead.x -= mDrains[i].mSpeed;

        if (mDrains[i].mHead.x < mDrains[i].mTrail[mDrains[i].mTrailGoal].x)
        {
          mDrains[i].mHead.x = mDrains[i].mTrail[mDrains[i].mTrailGoal].x;
          mDrains[i].mTrailGoal++;
          
          if (mDrains[i].mTrailGoal > mDrains[i].mTrailLength)
          {
            GenerateDrain(i);
          }
        }
      }
      else
      {
        if (mDrains[i].mHead.x < mDrains[i].mTrail[mDrains[i].mTrailGoal].x)
        {
          mDrains[i].mHead.x += mDrains[i].mSpeed;

          if (mDrains[i].mHead.x >= mDrains[i].mTrail[mDrains[i].mTrailGoal].x)
          {
            mDrains[i].mHead.x = mDrains[i].mTrail[mDrains[i].mTrailGoal].x;
            mDrains[i].mTrailGoal++;

            if (mDrains[i].mTrailGoal > mDrains[i].mTrailLength)
            {
              GenerateDrain(i);
            }
          }
        }
      }
    }
  }
}
//---------------------------------------------------------------------------

void CFx_Drain::Paint()
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
  // test code - trail paint
  for(int i = 0; i < 1; i++)
  {
    // desenez liniile verticale intii
    for(int j = 0; j < mDrains[i].mTrailLength / 2; j++)
    {
      GetGraphicInstance()->Line(mFxCanvas, mDrains[i].mTrail[j*2], mDrains[i].mTrail[j*2+1]);
    }

    // apoi liniile orizontale
    for(int j = 1; j < mDrains[i].mTrailLength / 2; j++)
    {
      GetGraphicInstance()->Line(mFxCanvas, mDrains[i].mTrail[j*2-1], mDrains[i].mTrail[j*2]);
    }
  }
  */

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

  // pictez trailul
  int lX, lY, lPrevIndex;
  
  for(int i = 0; i < mDrainsCount; i++)
  {
    // coord stuff
    lX         = mDrains[i].mHead.x;
    lY         = mDrains[i].mHead.y;
    lPrevIndex = mDrains[i].mTrailGoal - 1;

    // color stuff
    float lHR = (mHeadColor & 0xFF0000) >> 16,
          lHG = (mHeadColor & 0x00FF00) >> 8,
          lHB = (mHeadColor & 0x0000FF);

    float lTR = (mTailColor & 0xFF0000) >> 16,
          lTG = (mTailColor & 0x00FF00) >> 8,
          lTB = (mTailColor & 0x0000FF);
      
    float lR = lHR, lG = lHG, lB = lHB;
    float lRInc = (float)(lTR - lHR) / mTailLength,
          lGInc = (float)(lTG - lHG) / mTailLength,
          lBInc = (float)(lTB - lHB) / mTailLength;

    // optimizarea lu' peshte prajit
    if (lPrevIndex < 0)
    {
      continue;
    }

    if (lY < 0)
    {
      continue;
    }

    // paint stuff
    for(int j = 0; j < mTailLength; j++)
    {
      if (lX == mDrains[i].mTrail[lPrevIndex].x)
      {
        if ((lY > 0) && (lY < mFxHeight))
        {
          ((__int16*)lBuffer)[lX + (lSurfDesc.lPitch * lY) / 2] = makeRGB16(lR, lG, lB);
        }

        lY--;

        if (lY <= mDrains[i].mTrail[lPrevIndex].y)
        {
          lY = mDrains[i].mTrail[lPrevIndex].y;

          lPrevIndex--;

          if (lPrevIndex < 0)
          {
            break;
          }
        }
      }
      else
      {
        if (lX > mDrains[i].mTrail[lPrevIndex].x)
        {
          if ((lY > 0) && (lY < mFxHeight))
          {
            ((__int16*)lBuffer)[lX + (lSurfDesc.lPitch * lY) / 2] = makeRGB16(lR, lG, lB);
          }

          lX--;

          if (lX == mDrains[i].mTrail[lPrevIndex].x)
          {
            lPrevIndex--;

            if (lPrevIndex < 0)
            {
              break;
            }
          }
        }
        else
        {
          if ((lY > 0) && (lY < mFxHeight))
          {
            ((__int16*)lBuffer)[lX + (lSurfDesc.lPitch * lY) / 2] = makeRGB16(lR, lG, lB);
          }

          lX++;

          if (lX == mDrains[i].mTrail[lPrevIndex].x)
          {
            lPrevIndex--;

            if (lPrevIndex < 0)
            {
              break;
            }
          }
        }
      }

      lR += lRInc;
      lG += lGInc;
      lB += lBInc;
    }
  }

  // some small crap code again
  mFxCanvas->Unlock(NULL);
}
//---------------------------------------------------------------------------

void CFx_Drain::GenerateDrain(int _index)
{
  // capatul
  mDrains[_index].mHead.x = random(mFxWidth);
  mDrains[_index].mHead.y = 10;//-random(20);

  // urmele
  mDrains[_index].mTrailLength = 2 * (random(7) + 8);

  mDrains[_index].mTrail[0].x  = mDrains[_index].mHead.x;
  mDrains[_index].mTrail[0].y  = random(mFxHeight / mDrains[_index].mTrailLength);
  mDrains[_index].mTrail[1].x  = mDrains[_index].mHead.x;
  //mDrains[_index].mTrail[1].y  = mDrains[_index].mTrail[0].y + mFxHeight / mDrains[_index].mTrailLength + random(mFxHeight - mDrains[_index].mTrail[0].y - mFxHeight / mDrains[_index].mTrailLength);
  mDrains[_index].mTrail[1].y  = mDrains[_index].mTrail[0].y + mFxHeight / mDrains[_index].mTrailLength + random(mFxHeight / mDrains[_index].mTrailLength);

  for(int i = 2; i < mDrains[_index].mTrailLength; i+=2)
  {
    // generez X'ul pentru i
    if (random(2) == 1)
    {
      mDrains[_index].mTrail[i].x  = mDrains[_index].mTrail[i - 1].x - random(mFxWidth / 4);

      if (mDrains[_index].mTrail[i].x < 0)
      {
        mDrains[_index].mTrail[i].x = 0;
      }
    }
    else
    {
      mDrains[_index].mTrail[i].x  = mDrains[_index].mTrail[i - 1].x + random(mFxWidth / 4);

      if (mDrains[_index].mTrail[i].x > mFxWidth - 1)
      {
        mDrains[_index].mTrail[i].x = mFxWidth - 1;
      }
    }

    // generez X'ul pentru i + 1
    mDrains[_index].mTrail[i+1].x = mDrains[_index].mTrail[i].x;

    // generez Y'ul pentru i
    mDrains[_index].mTrail[i].y = mDrains[_index].mTrail[i - 1].y;

    // generez Y'ul pentru i + 1
    mDrains[_index].mTrail[i + 1].y =
      mDrains[_index].mTrail[i].y +
      mFxHeight / mDrains[_index].mTrailLength +
      //random(mFxHeight - mDrains[_index].mTrail[i].y - mFxHeight / mDrains[_index].mTrailLength);
      random(mFxHeight / mDrains[_index].mTrailLength);
  }

  // ultimul punct trebuie sa fie in afara ecranului
  if (mDrains[_index].mTrail[mDrains[_index].mTrailLength - 1].y < mFxHeight)
  {
    // ca sa se termine complet in afara ecranului
    mDrains[_index].mTrail[mDrains[_index].mTrailLength - 1].y = mFxHeight + mTailLength;
  }

  // viteza
  mDrains[_index].mSpeed = random(4) + 2;

  // goal
  mDrains[_index].mTrailGoal = 0;
}
//---------------------------------------------------------------------------

void  CFx_Drain::SetDensity(int _v)
{
  if ((_v < 0) || (_v > FX_MAX_DRAINS))
  {
    return;
  }

  mDrainsCount = _v;

  for(int i = 0; i < mDrainsCount; i++)
  {
    GenerateDrain(i);
  }
}
//---------------------------------------------------------------------------

void CFx_Drain::SetHeight(int _v)
{
  CFx::SetHeight(_v);

  for(int i = 0; i < mDrainsCount; i++)
  {
    GenerateDrain(i);
  }
}
//---------------------------------------------------------------------------

void CFx_Drain::SetWidth(int _v)
{
  CFx::SetWidth(_v);

  for(int i = 0; i < mDrainsCount; i++)
  {
    GenerateDrain(i);
  }
}
//---------------------------------------------------------------------------

CFx_Drain::~CFx_Drain()
{
}
//---------------------------------------------------------------------------

/*
CFx_Drain::CFx_Drain() : CFx()
{
  mDrainsCount = 8;
  mTailLength  = 32;
  mHeadColor   = makeRGB16(255, 255, 255);
  mTailColor   = makeRGB16(0, 0, 0);

  for(int i = 0; i < mDrainsCount; i++)
  {
    GenerateDrain(i);
  }
}
//---------------------------------------------------------------------------

void CFx_Drain::Update()
{
  for(int i = 0; i < mDrainsCount; i++)
  {
    mDrains[i].mPos.y += mDrains[i].mSpeed;

    if (mDrains[i].mPos.y - mTailLength > mFxHeight)
    {
      GenerateDrain(i);
    }
  }
}
//---------------------------------------------------------------------------

void CFx_Drain::Paint()
{
  if (!mFxCanvas)
  {
    return;
  }

  GetGraphicInstance()->FillSurface(mFxCanvas, 0);

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

  for(int i = 0; i < mDrainsCount; i++)
  {
    if (mDrains[i].mPos.y >= 0)
    {
      // go go Y
      for(int j = 0; j < mTailLength; j++)
      {
        if (mDrains[i].mPos.y - j >= 0)
        {
          ((__int16*)lBuffer)[(int)mDrains[i].mPos.x + (lSurfDesc.lPitch * ((int)mDrains[i].mPos.y - j)) / 2] = makeRGB16(255 - j * 7, 255 - j * 7, 255 - j * 7);
        }
      }
    }
  }

  // some small crap code again
  mFxCanvas->Unlock(NULL);
}
//---------------------------------------------------------------------------

void CFx_Drain::GenerateDrain(int _index)
{
  mDrains[_index].mPos.x = random(mFxWidth);
  mDrains[_index].mPos.y = -random(20);
  mDrains[_index].mSpeed = random(6) + 4;
}
//---------------------------------------------------------------------------

CFx_Drain::~CFx_Drain()
{
}
//---------------------------------------------------------------------------
*/

/*
logWrite("Point "); logWriteI(j*2);
logWrite(" X:");logWriteI(mDrains[i].mTrail[j*2].x);
logWrite(", Y:");logWriteI(mDrains[i].mTrail[j*2].y);
logWriteLn("");

logWrite("Point "); logWriteI(j*2+1);
logWrite(" X:");logWriteI(mDrains[i].mTrail[j*2+1].x);
logWrite(", Y:");logWriteI(mDrains[i].mTrail[j*2+1].y);
logWriteLn("");
*/
        
