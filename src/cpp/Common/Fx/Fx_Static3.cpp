//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Fx_Static3.h"
#include "colors16.h"
#include "logfile.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

CFx_Static3::CFx_Static3() : CFx()
{
  mHeadY = NULL;
}
//---------------------------------------------------------------------------

void CFx_Static3::Update()
{
}
//---------------------------------------------------------------------------

void CFx_Static3::Paint()
{
  if (!mFxCanvas)
  {
    return;
  }

  if (!mHeadY)
  {
    return;
  }

  if (!mEnabled)
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

  // parcurg lista de y
  TFxStatic3_Y* mYList = mHeadY;
  TFxStatic3_X* mXList;

  while(mYList != NULL)
  {
    //logWrite("Y : "); logWriteILn(mYList->mY);

    mXList = mYList->mHeadX;

    /*
    for(int x = 0; x < mFxWidth; x++)
    {
      unsigned __int8  lPixelColor = (unsigned __int8)random(255);
      unsigned __int16 lColor16    = makeRGB16(lPixelColor, lPixelColor, lPixelColor);
      ((__int16*)lBuffer)[mYList->mY * lSurfDesc.lPitch / 2 + x] = lColor16;
    }
    */

    // parcurg lista de x
    while(mXList != NULL)
    {
      //logWrite("X : "); logWriteI(mXList->mXStart); logWrite("-"); logWriteILn(mXList->mXEnd);

      for(int x = mXList->mXStart; x < mXList->mXEnd; x++)
      {
        unsigned __int8  lPixelColor = (unsigned __int8)random(255);
        unsigned __int16 lColor16    = makeRGB16(lPixelColor, lPixelColor, lPixelColor);
        ((__int16*)lBuffer)[mYList->mY * lSurfDesc.lPitch / 2 + x] = lColor16;
      }

      mXList = mXList->mNext;
    }

    mYList = mYList->mNext;
  }

  // some small crap code again
  mFxCanvas->Unlock(NULL);
}
//---------------------------------------------------------------------------

void CFx_Static3::SetMask(TPGISpriteReference _sprite, int _cw, int _ch)
{
  // fx'ul va avea dimensiunile unei celule de sprite
  SetWidth(_sprite->GetAnimWidth() + 10);
  SetHeight(_sprite->GetAnimHeight() + 10);

  // pictez sprite'ul in canvas
  GetGraphicInstance()->FillSurface(mFxCanvas, 0);
  GetGraphicInstance()->PaintSpriteToSurface(mFxCanvas, 0, 0, _sprite, _cw, _ch);

  // acum generez masca
  CreateMask();

  // sterg canvasul, nu mai e nevoie de el
  GetGraphicInstance()->FillSurface(mFxCanvas, 0);
}
//---------------------------------------------------------------------------

void CFx_Static3::CreateMask()
{
  if (!mFxCanvas)
  {
    return;
  }

  //logWriteLn("CFx_Static3::CreateMask - method started");

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

  // constructia scanline'urilor (ugh)
  TFxStatic3_Y* lYLine, *lYLineTmp, *lYLine2 = NULL;
  TFxStatic3_X* lXLine, *lXLineTmp;
  bool lStart;

  for(int y = 0; y < mFxHeight; y++)
  {
    lStart = false;
    lXLine = NULL;
    lYLine = NULL;

    for(int x = 0; x < mFxWidth; x++)
    {
      if (((__int16*)lBuffer)[y * lSurfDesc.lPitch / 2 + x] != 0)
      {
        if (!lStart)
        {
          //logWrite("Starting a scanline at : "); logWriteI(x); logWrite(","); logWriteILn(y);

          lStart  = true;

          if (!lYLine)
          {
            //logWriteLn("Scanline was empty.");

            lYLine         = new TFxStatic3_Y;
            lYLine->mY     = y;
            lYLine->mHeadX = NULL;
            lYLine->mNext  = NULL;
          }
        }

        // aloc o noua pereche xstart-xend, daca nu am deja una alocata
        if (!lXLine)
        {
          //logWriteLn("This is the first xs-xe pair.");

          lXLine          = new TFxStatic3_X;
          lXLine->mXStart = x;
          lXLine->mXEnd   = x;
          lXLine->mNext   = NULL;
        }
        else
        {
          lXLine->mXEnd++;
        }
      }
      else
      {
        if (lStart)
        {
          //logWrite("XE-XS pair search ended at "); logWriteI(lXLine->mXEnd);


          // adaug scanline'ul si reiau cautarea
          lStart = false;

          if (!lYLine->mHeadX)
          {
            //logWriteLn("It is the first xs-xe pair.");
            lYLine->mHeadX = lXLine;
          }
          else
          {
            //logWriteLn("Adding xs-xe pair.");
            lXLineTmp = lYLine->mHeadX;

            while(lXLineTmp->mNext != NULL)
            {

              lXLineTmp = lXLineTmp->mNext;
            }

            lXLineTmp->mNext = lXLine;
            //logWriteLn("Added xs-xe pair.");
          }

          lXLine = NULL;
        }
      }
    }

    // x'ul iese in afara ecranului, scanline'ul s'a terminat la mFxWidth
    if (lStart)
    {
      lXLineTmp = lYLine->mHeadX;

      while(lXLineTmp->mNext != NULL)
      {
        lXLineTmp = lXLineTmp->mNext;
      }

      lXLineTmp->mNext = lXLine;
    }

    // acum trebuie adaugat lYLine in lista de y
    if (!lYLine)
    {
      continue;
    }

    if (!lYLine2)
    {
      //logWriteLn("Y List was void.");
      lYLine2 = lYLine;  // lista este initial vida
    }
    else
    {
      //logWriteLn("Adding scanline to Y List.");

      lYLineTmp = lYLine2; // adaug la sfirsitul listei

      while(lYLineTmp->mNext != NULL)
      {
        lYLineTmp = lYLineTmp->mNext;
      }

      lYLineTmp->mNext = lYLine;

      //logWriteLn("Added scanline to Y List.");
    }
  }

  mHeadY = lYLine2;

  //logWriteLn("CFx_Static3::CreateMask - method done");

  // some small crap code again
  mFxCanvas->Unlock(NULL);
}
//---------------------------------------------------------------------------

CFx_Static3::~CFx_Static3()
{
}
//---------------------------------------------------------------------------

