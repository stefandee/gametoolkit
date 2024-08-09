//---------------------------------------------------------------------------


#pragma hdrstop

#include "PP_StdLib.h"
#include "EcoSysGenRandom.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

std::vector<EcoSysItemOut> EcoSysGenRandom::Generate()
{
  std::vector<EcoSysItemOut> lResult;

  std::vector<EcoSysItemIn*> lItems = GetItems();

  if (lItems.size() == 0)
  {
    return lResult;
  }

  CPRect lZone = GetZone();

  for(int i = 0; i < mSteps; i++)
  {
    // generate random coords inside the zone
    EcoSysItemOut item;

    int lIndexIn;

    lIndexIn = gfRandom(lItems.size());

    item.mSize      = lItems[lIndexIn]->mSize;
    item.mCode      = lItems[lIndexIn]->mCode;
    item.mColorCode = lItems[lIndexIn]->mColorCode;
    item.mFilePath  = lItems[lIndexIn]->mFilePath;

    if (!mRepeatOnCollision)
    {
      item.mLocation = CPPoint(lZone.left + gfRandom(lZone.Width()), lZone.top + gfRandom(lZone.Height()));
    }
    else
    {
      /*
      do
      {
        item.mLocation = CPPoint(lZone.left + gfRandom(lZone.GetWidth()), lZone.top + gfRandom(lZone.GetHeight()));

        bool lCollision = false;

        for(int j = 0; j < lResult; j++)
        {
          if (
        }

      }
      while(true)
      */
    }

    lResult.push_back(item);
  }

  return lResult;
}
//---------------------------------------------------------------------------
