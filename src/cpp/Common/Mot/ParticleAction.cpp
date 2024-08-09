#include "ParticleAction.h"
#include <iostream>
#include <math.h>
using namespace std;

CMotActionMatchColor::CMotActionMatchColor(int _targetR, int _targetG, int _targetB, int _scale)
{
  mTargetR = _targetR;
  mTargetG = _targetG;
  mTargetB = _targetB;
  
  mScale   = _scale;
}
    
void CMotActionMatchColor::Update()
{

  for(int i = 0; i < GetParticles()->size(); i++)
  {
    CParticle& lP = GetParticles()->operator[](i);
    
    int lColor = lP.GetColor();
    
    int lR = (lColor & 0xFF0000) >> 16;
    int lG = (lColor & 0x00FF00) >> 8;
    int lB = (lColor & 0x0000FF);
    
    // red
    if (lR < mTargetR)
    {
      lR += mScale;
    }
    else
    {
      lR -= mScale;
    }
    
    // green
    if (lG < mTargetG)
    {
      lG += mScale;
    }
    else
    {
      lG -= mScale;
    }
    
    // blue
    if (lB < mTargetB)
    {
      lB += mScale;
    }
    else
    {
      lB -= mScale;
    }
    
    if (lR < 0)
    {
      lR = 0;
    }
    
    if (lG < 0)
    {
      lG = 0;
    }
    
    if (lB < 0)
    {
      lB = 0;
    }

    if (lR > 255)
    {
      lR = 255;
    }
    
    if (lG > 255)
    {
      lG = 255;
    }
    
    if (lB > 255)
    {
      lB = 255;
    }
    
    //cout << "action " << i << ": " << lR << "," << lG << "," << lB << endl;
    
    lP.SetColor((lR << 16) | (lG << 8) | (lB));
  }
}

CMotActionSpeedLimit::CMotActionSpeedLimit(double _min, double _max)
{
  mMin = std::min(_min, _max);
  mMax = std::max(_min, _max);
}
    
void CMotActionSpeedLimit::Update()
{
  for(int i = 0; i < GetParticles()->size(); i++)
  {
    CParticle& lP = GetParticles()->operator[](i);    
  }  
}

CMotActionFollow::CMotActionFollow(double _scale, double _radius)
{
  mScale = abs(_scale);
  mRadius = abs(_radius);
}
    
void CMotActionFollow::Update()
{
  int lSize = GetParticles()->size();

  for(int i = 0; i < lSize - 1; i++)
  {
    CParticle& lP = GetParticles()->operator[](i);
    CParticle& lP1 = GetParticles()->operator[](i+1);
    
    TPos3D lPos1 = lP.GetPos();
    TPos3D lPos2 = lP1.GetPos();
    
    float lDist = Dist3D(lPos1, lPos2);
    
    if (lDist > mRadius)
    {
      break;
    }
    
    CVector3D lBetween(lPos2.x - lPos1.x, lPos2.y - lPos1.y, lPos2.z - lPos1.z);
    CVector3D lSpeed = lP.GetSpeed();
    
    lSpeed += lBetween * ((mScale * 0.1) / (lDist + 1e-3f));
    
    lP.SetSpeed(lSpeed);
  }  
}

void CMotActionOrbitPoint::Update()
{
  int lSize = GetParticles()->size();
  
  //cout << "list addr: " << GetParticles() << endl;

  for(int i = 0; i < lSize; i++)
  {
    CParticle& lP = GetParticles()->operator[](i);
    
    TPos3D lPos = lP.GetPos();
    
    float lDist = Dist3D(lPos, mCenter);
    
    if (lDist > mRadius)
    {
      break;
    }
    
    CVector3D lBetween(mCenter.x - lPos.x, mCenter.y - lPos.y, mCenter.z - lPos.z);
    CVector3D lSpeed = lP.GetSpeed();
    
    lSpeed += lBetween * ((mScale * 0.1) / (lDist + 1e-3f));
    
    lP.SetSpeed(lSpeed);
    
    //cout << lP.Id() << " ";
  }  
  
  //cout << endl;
}

void CMotActionAccelerate::Update()
{
  int lSize = GetParticles()->size();

  for(int i = 0; i < lSize; i++)
  {
    CParticle& lP = GetParticles()->operator[](i);
    
    lP.SetAccel(lP.GetAccel() + mAccel);    
  }    
}
