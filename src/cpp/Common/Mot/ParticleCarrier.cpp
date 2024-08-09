#include "ParticleCarrier.h"

void CParticleCarrier::Update() 
{ 
  mTra->Update();

  TPos3D lP = mTra->GetPos();

  mBase->SetX(GetX() + lP.x); 
  mBase->SetY(GetY() + lP.y); 
  mBase->SetZ(GetZ() + lP.z); 
}  


