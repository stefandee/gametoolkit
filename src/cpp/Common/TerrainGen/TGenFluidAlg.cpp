#include <math.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#include "TGenFluidAlg.h"
#include "PP_StdLib.h"

HeightMap* TGenFluidAlg::Generate()
{
  HeightMap* lP1, *lP2, *lF;
  
  lP1      = new HeightMap(GetWidth(), GetHeight());
  lP2      = new HeightMap(GetWidth(), GetHeight());
  //lF       = new HeightMap(GetWidth(), GetHeight());
  
  RandomGen(lP1);
  RandomGen(lP2);
  
  float lC1 = (4 - (8 * mC * mC * mT * mT) / (mD * mD)) / (mMu * mT + 2.);
  float lC2 = (mMu * mT - 2.) / (mMu * mT + 2.);
  float lC3 = ((2 * mC * mC * mT * mT) / (mD * mD)) / (mMu * mT + 2.);
  
  //cout << lC1 << " | " << lC2 << " | " << lC3 << endl;
  
  //float lC1 = 0., lC2 = 0., lC3 = .25;
	
  for(int i = 0; i < mPasses; i++)
  {
    //cout << "PASS " << i << endl << "-------------" << endl;
    for(int x = 1; x < GetWidth()-1; x++)
    {
      for(int y = 1; y < GetHeight()-1; y++)
      {
        lP1->Set(x, y, lC1 * lP2->Get(x, y) + lC2 * lP1->Get(x, y) + lC3 * (lP2->Get(x + 1, y) + lP2->Get(x - 1, y) + lP2->Get(x, y - 1) + lP2->Get(x, y + 1))); 
      }
      
      //cout << endl;
    }  
    
    //*lP1 = *lP2;
    //lP1->Print();    
    *lP2 = *lP1;
    //lP2->Print();
  }
  
  return lP1;
}

void TGenFluidAlg::RandomGen(HeightMap* _map)
{
  _map->Reset(0.);

  for(int x = 1; x < GetWidth() - 1; x++)
  {
    for(int y = 1; y < GetHeight() - 1; y++)
    {
      _map->Set(x, y, mMinHeight + (mMaxHeight - mMinHeight) * gfRandom48());
    }
  }
}

