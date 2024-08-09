#ifndef TGENFAULTALGH
#define TGENFAULTALGH

#include "TGenAlgorithm.h"

class TGenFaultAlg : public TGenAlgorithm
{
  public:
    TGenFaultAlg();
    HeightMap* Generate();  
  
  public:
    void SetDisplacement(float _v) { mDisplacement = _v; }
    void SetPasses(int _v) { mPasses = _v; }
  
  private:
    float mDisplacement;
    int   mPasses;

    void Displace(HeightMap* _map);        
};

#endif // TGENFAULTALGH
