#include "ParticleDistribution.h"
#include <iostream>
using namespace std;

CParticleDistribution::CParticleDistribution()
{
}

CParticleDistribution::~CParticleDistribution()
{
}

CDistributionColor::CDistributionColor()
{  
  mRD = new CDistributionConstant(0.0);
  mGD = new CDistributionConstant(0.0);
  mBD = new CDistributionConstant(0.0);
}

CDistributionColor::CDistributionColor(CParticleDistribution* _rd, CParticleDistribution* _gd, CParticleDistribution* _bd)
{
  if (_rd)
  {
    mRD = _rd;
  }
  else
  {
    mRD = new CDistributionConstant(50.0);
  }

  if (_gd)
  {
    mGD = _gd;
  }
  else
  {
    mGD = new CDistributionConstant(50.0);
  }
  
  if (_bd)
  {
    mBD = _bd;
  }
  else
  {
    mBD = new CDistributionConstant(50.0);
  }
}

CDistributionColor::CDistributionColor(const CDistributionColor& _a)
{  
  mRD = new CDistributionConstant(0.0);
  mGD = new CDistributionConstant(0.0);
  mBD = new CDistributionConstant(0.0);
  
  *mRD = *_a.mRD; 
  *mGD = *_a.mGD; 
  *mBD = *_a.mBD; 
}

CDistributionColor& CDistributionColor::operator = (const CDistributionColor& _a)
{
  //if (_a == *this)
  //{
  //  return *this;
  //}
  
  *mRD = *_a.mRD; 
  *mGD = *_a.mGD; 
  *mBD = *_a.mBD;
  
  return *this;  
}

CDistributionColor::~CDistributionColor()
{
  delete mRD;
  delete mGD;
  delete mBD;
}

