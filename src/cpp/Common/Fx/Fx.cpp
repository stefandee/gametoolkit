//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Fx.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

CFx::CFx()
{
  randomize();

  mFxWidth  = 50;
  mFxHeight = 50;
  mFxCanvas = NULL;
  mEnabled  = true;

  CreateCanvas();
}
//---------------------------------------------------------------------------

void CFx::CreateCanvas()
{
  if (mFxCanvas)
  {
    GetGraphicInstance()->ReleaseSurface(mFxCanvas);
  }

  try
  {
    mFxCanvas = GetGraphicInstance()->CreateSurface(mFxWidth, mFxHeight); // e 16 bit color
  }
  catch(...)
  {
    mFxCanvas = NULL;
  }
}
//---------------------------------------------------------------------------

void CFx::Update()
{
}
//---------------------------------------------------------------------------

void CFx::Paint()
{
}
//---------------------------------------------------------------------------

CPGISurface* CFx::GetCanvas()
{
  return mFxCanvas;
}
//---------------------------------------------------------------------------

void CFx::SetWidth(int _v)
{
  if (_v < 0)
  {
    return;
  }

  mFxWidth = _v;

  CreateCanvas();
}
//---------------------------------------------------------------------------

int  CFx::GetWidth()
{
  return mFxWidth;
}
//---------------------------------------------------------------------------

void CFx::SetHeight(int _v)
{
  if (_v < 0)
  {
    return;
  }

  mFxHeight = _v;

  CreateCanvas();
}
//---------------------------------------------------------------------------

int  CFx::GetHeight()
{
  return mFxHeight;
}
//---------------------------------------------------------------------------

CFx::~CFx()
{
  GetGraphicInstance()->ReleaseSurface(mFxCanvas);
}
//---------------------------------------------------------------------------


