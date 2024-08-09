#include <math.h>
#include <stdlib.h>

#include "TGenHillAlg.h"
#include "PP_StdLib.h"

TGenHillAlg::TGenHillAlg()
{
  SetWidth(50);
  SetHeight(50);
  mHills = 10;
  
  mMaxRadius = 20;
  mMinRadius = 10;
}

HeightMap* TGenHillAlg::Generate()
{
  HeightMap* lMap = new HeightMap(GetWidth(), GetHeight());
  
  for(int i = 0; i < mHills; i++)
  {
    GenerateHill(lMap);
  }
  
  return lMap;
}

void TGenHillAlg::GenerateHill(HeightMap* _map)
{
  float lRadius = mMinRadius + (mMaxRadius - mMinRadius) * gfRandom48();
  
  float lCX = GetWidth()  * gfRandom48();
  float lCY = GetHeight() * gfRandom48();  
  
  for(int x = (int)(lCX - lRadius); x < (int)(lCX + lRadius); x++)
  {
    if (x < 0 || x >= GetWidth())
    {
      continue;
    }
  
    for(int y = (int)(lCY - lRadius); y < (int)(lCY + lRadius); y++)
    {
      if (y < 0 || y >= GetHeight()) 
      {
        continue;
      }
      
      if ((x - lCX) * (x - lCX) + (y - lCY) * (y - lCY) <= lRadius * lRadius)
      {
        _map->Set(x, y, _map->Get(x, y) + sqrt(lRadius * lRadius - (x - lCX) * (x - lCX) - (y - lCY) * (y - lCY)));
      }
    }
  }
}
