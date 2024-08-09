// terrain generation using fluid technics
// implementation of http://www.gamedev.net/reference/articles/article2001.asp

#ifndef TGENFLUIDALGH
#define TGENFLUIDALGH

#include "TGenAlgorithm.h"

class TGenFluidAlg : public TGenAlgorithm
{
  public:
    HeightMap* Generate();
    
  public: // get-set
    void SetC(float _v) { mC = _v; }
    void SetD(float _v) { mD = _v; }
    void SetT(float _v) { mT = _v; }
    void SetMu(float _v) { mMu = _v; }
    void SetPasses(int _v) { mPasses = _v; }
    void SetMinHeight(float _v) { mMinHeight = _v; }
    void SetMaxHeight(float _v) { mMaxHeight = _v; }
  
  private:
    float mC, mD, mT, mMu, mMinHeight, mMaxHeight;
    int   mPasses;   
    
    void RandomGen(HeightMap* _map);
};

#endif // TGENFLUIDALGH
