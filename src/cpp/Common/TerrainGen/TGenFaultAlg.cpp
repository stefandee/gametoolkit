#include <stdlib.h>
#include "TGenFaultAlg.h"
#include "PP_StdLib.h"

TGenFaultAlg::TGenFaultAlg()
{
  mDisplacement = 4.;
}
//---------------------------------------------------------------------------

HeightMap* TGenFaultAlg::Generate()
{
  HeightMap* lMap = new HeightMap(GetWidth(), GetHeight()); 
    
  if (GetInputMap())
  {
    *lMap = *(GetInputMap());
  }  
  
  for(int i = 0; i < mPasses; i++)
  {
    Displace(lMap);
  }
  
  return lMap;
}
//---------------------------------------------------------------------------

void TGenFaultAlg::Displace(HeightMap* _map)
{
  float lXS = GetWidth() * gfRandom48();
  float lXE = GetWidth() * gfRandom48();
  float lYS = GetHeight() * gfRandom48();
  float lYE = GetHeight() * gfRandom48();

  for(int y = 0; y < GetHeight(); y++)
  {
    for(int x = 0; x < GetWidth(); x++)
    {
      float lScalar = (y - lYS) * (lXE - lXS) - (x - lXS) * (lYE - lYS);

      if (lScalar > 0)
      {
        _map->Set(x, y, _map->Get(x, y) + mDisplacement);
      }
      else
      {
        _map->Set(x, y, _map->Get(x, y) -mDisplacement);
      }
    }
  }
}
//---------------------------------------------------------------------------

