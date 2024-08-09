#include "ParticleEmitter.h"

#include <time.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

//CParticleEmitter::CParticleEmitter()
//{
//}

//CParticleEmitter::CParticleEmitter(const CParticleEmitter&)
//{
//}

//CParticleEmitter::~CParticleEmitter()
//{
//}
    
//CParticleEmitter& CParticleEmitter::operator=(const CParticleEmitter&)
//{
//}

CParticleEmitter::CParticleEmitter(int _count, CParticleDistribution* _lifed, CDistributionColor* _colord, CMotDistributionPos* _posd, CMotDistributionVector* _speedd)
{
  srand48(time(0));
  
  mColorDistribution = _colord;
  mPosDistribution   = _posd;
  mLifeDistribution  = _lifed;
  mSpeedDistribution = _speedd;
  
  for(int i = 0; i < _count; i++)
  {
    GenerateParticle(i);
  }
  
  //mColorDistribution.push_back(0);    
  //mColorDistribution.push_back(0);    
  //mColorDistribution.push_back(0);    
}

CParticleEmitter::~CParticleEmitter()
{
}

void CParticleEmitter::GenerateParticle(int i)
{
  CParticle lP(mLifeDistribution->GetValue());
    
  lP.SetPos(mPosDistribution->GetPos());
  
  //lP.SetX(random(10));
  //lP.SetY(0.);
  //lP.SetZ(0.0);
  
  //int lXS = 0.;//random(5);
  
  //lXS += SIGN(lXS) * 15;
  
  //int lYS = -random(40) - 30;
  //int lZS = random(8) - 4;
  
  //cout << lXS << "," << lYS << "," << lZS << endl;
  
  lP.SetSpeed(mSpeedDistribution->GetValue());
  
  int lRed   = (int)mColorDistribution->GetRValue(), 
      lGreen = (int)mColorDistribution->GetGValue(), 
      lBlue  = (int)mColorDistribution->GetBValue();
  

  //cout << lRed << "," << lGreen << "," << lBlue << endl;
            
  lP.SetColor((lRed << 16) | (lGreen << 8) | lBlue);
  //lP.SetAccel(CVector3D(random(4) - random(4), random(4) - random(4), random(8)));

  //int lXA = random(10) - 5;  
  //lP.SetAccel(CVector3D(lXA, -random(8), 0.0));
  lP.SetAccel(CVector3D(0., 0., 0));
  
  if (i >= mParticles.size())
  {
    mParticles.push_back(lP);
  }
  else
  {
    mParticles[i] = lP;  
  }  
}
    
void CParticleEmitter::Update()
{
  // apply actions to the particles
  //cout << "updating actiuons: " << mActionsList.size() << endl;
  
  for(int i = 0; i < mActionsList.size(); i++)
  {
  //  cout << i << endl;
    mActionsList[i]->Update();
  }
  
  
  //cout << "particle update:" << endl;
          
  for(int i = 0; i < mParticles.size(); i++)
  {
    mParticles[i].Update();
    
    if (mParticles[i].IsDead())
    {
      GenerateParticle(i);
    }
    
    //cout << mParticles[i].Id() << " ";
  }
  
  //cout << endl;
}

void CParticleEmitter::Paint(SDL_Surface* _surface, TPos2D _origin)
{
  TPos2D lOrigin;
  
  lOrigin.x = GetX() + _origin.x;
  lOrigin.y = GetY() + _origin.y;

  for(int i = 0; i < mParticles.size(); i++)
  {
    mParticles[i].Paint(_surface, lOrigin);
  }  
}

