#include "TGenAlgorithm.h"
#include <time.h>
#include <stdlib.h>

TGenAlgorithm::TGenAlgorithm()
{
  //srand48(time(0));
  
  mInputMap = 0;
  
  mWidth = 50;
  mHeight = 50;
}
//---------------------------------------------------------------------------

void TGenAlgorithm::SetWidth(int _width)
{
  if (_width < 0)
  {
    return;
  }

  if (mInputMap)
  {
    mHeight = mInputMap->GetHeight();
  }
  else
  {
    mWidth = _width;
  }
}
//---------------------------------------------------------------------------

void TGenAlgorithm::SetHeight(int _height)
{
  if (_height < 0)
  {
    return;
  }

  if (mInputMap)
  {
    mHeight = mInputMap->GetHeight();
  }
  else
  {
    mHeight = _height;
  }
}
//---------------------------------------------------------------------------

void TGenAlgorithm::SetInputMap(HeightMap* _map)
{
  mInputMap = _map;

  if (mInputMap)
  {
    mWidth  = mInputMap->GetWidth();
    mHeight = mInputMap->GetHeight();
  }
}
//---------------------------------------------------------------------------


