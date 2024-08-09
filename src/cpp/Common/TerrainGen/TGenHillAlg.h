// generates random hemi-spheres 

#ifndef TGENHILLALGH
#define TGENHILLALGH

#include "TGenAlgorithm.h"

class TGenHillAlg : public TGenAlgorithm
{
  public:
    TGenHillAlg();
    
    HeightMap* Generate();
    
  public: // get-set
    //void SetMinHeight(float _v) { mMinHeight = _v; }
    //void SetMaxHeight(float _v) { mMaxHeight = _v; }
    void SetMinRadius(float _v) { mMinRadius = _v; }
    void SetMaxRadius(float _v) { mMaxRadius = _v; }
    void SetHills(int _v) { mHills = _v; }
  
  private:
    float mMinHeight, mMaxHeight, mMinRadius, mMaxRadius;
    int   mHills;   
    
    void GenerateHill(HeightMap* _map);
};

#endif // TGENFLUIDALGH
