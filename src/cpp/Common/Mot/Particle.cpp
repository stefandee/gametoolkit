#include "Particle.h"
#include <iostream>
using namespace std;

CParticle::CParticle(double _maxAge)
{ 
  mAge = 0.0; 
  mMaxAge = _maxAge;
  mId = 0;//drand48() * 32;
}

CParticle::~CParticle() 
{
}

void CParticle::Update()
{
  mAge += 0.1;

  CVector3D lSpeed = GetSpeed();
  CVector3D lAccel = GetAccel();
  
  double lXSpeed = lSpeed.GetX() + lAccel.GetX() * 0.1;
  double lYSpeed = lSpeed.GetY() + lAccel.GetY() * 0.1;
  double lZSpeed = lSpeed.GetZ() + lAccel.GetZ() * 0.1;
  
  //cout << lXSpeed << "," << lYSpeed << "," << lZSpeed << endl;
  
  SetSpeed(CVector3D(lXSpeed, lYSpeed, lZSpeed));
  
  SetX(GetX() + lXSpeed * 0.1);
  SetY(GetY() + lYSpeed * 0.1);
  SetZ(GetZ() + lZSpeed * 0.1);
}

void CParticle::Paint(SDL_Surface* _surface, TPos2D _origin)
{
  SDL_Rect lRect;

  lRect.x = _origin.x + GetX() - 1;
  lRect.y = _origin.y + GetY() - 1;
  lRect.w = 2;
  lRect.h = 2;
	  
  SDL_FillRect(_surface, &lRect, mColor);	    
}


