#ifndef PARTICLECARRIERH
#define PARTICLECARRIERH

#include "ParticleBase.h"
#include "MotTrajectory.h"

class CParticleCarrier : public CParticleBase
{
  public:
    CParticleCarrier(CParticleBase* _base, CMotTrajectory* _tra) { mBase = _base; mTra = _tra; }
    ~CParticleCarrier();
    
  public:
    virtual void Update();  
    virtual void Paint(SDL_Surface* _surface, TPos2D _origin) {};
    
  private:
    CParticleBase* mBase;
    CMotTrajectory* mTra;  
};

#endif
