#ifndef PARTICLEDISTRIBUTIONH
#define PARTICLEDISTRIBUTIONH

#include <stdlib.h>
#include <time.h>
#include <algorithm>

class CParticleDistribution
{
  public:
    CParticleDistribution();
    virtual ~CParticleDistribution();
    
  public:
    virtual double GetValue() = 0;
  
  protected:
  
  private:
      
};

class CDistributionRandom : public CParticleDistribution
{
  public:
    CDistributionRandom() { srand48(time(0)); mMin = 0.0; mMax = 1.0; }
    CDistributionRandom(double _min, double _max) { mMin = std::min(_min, _max); mMax = std::max(_min, _max); }
    
  public:
    double GetValue() { return mMin + drand48() * (mMax - mMin); }  
    
  protected:
  
  private:
    double mMin, mMax;  
};

class CDistributionConstant : public CParticleDistribution
{
  public:
    CDistributionConstant() { mConst = 0.0; }
    CDistributionConstant(double _const) { mConst = _const; }
    
  public:
    double GetValue() { return mConst; }  
    
  private:
    double mConst;  
};

class CDistributionColor
{
  public:
    CDistributionColor();
    CDistributionColor(CParticleDistribution*, CParticleDistribution*, CParticleDistribution*);
    virtual ~CDistributionColor();
    
    CDistributionColor(const CDistributionColor&);
    CDistributionColor& operator = (const CDistributionColor&);
    
  public:
    double GetRValue() { return mRD->GetValue(); }
    double GetGValue() { return mGD->GetValue(); }
    double GetBValue() { return mBD->GetValue(); }
    
  private:
    CParticleDistribution* mRD, *mGD, *mBD;

};

/*
class CDistributionVector
{
    CDistributionVector();
    CDistributionVector(CParticleDistribution*, CParticleDistribution*, CParticleDistribution*);
    virtual ~CDistributionVector();
    
    CDistributionVector(const CDistributionVector&);
    CDistributionVector& operator = (const CDistributionVector&);
    
  public:
    double GetXValue() { return mXD->GetValue(); }
    double GetYValue() { return mYD->GetValue(); }
    double GetZValue() { return mZD->GetValue(); }
    
  private:
    CParticleDistribution* mXD, *mYD, *mZD;
};
*/

#endif
